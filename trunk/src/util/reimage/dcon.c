/*
 * dcon version 0.97 Dec 1996
 *
 * Copyright (C) 1989,1996  Daniel Chouinard
 *
 * Permission to use, copy, and distribute this software and its
 * documentation for any purpose without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 *
 * Permission to modify the software is granted, but not the right to
 * distribute the modified code so that only one version of the language
 * syntax may be agreed upon.
 *
 * This software is provided "as is" without express or implied warranty.
 *
 * See dcon.doc for more information.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <termio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "ymodem.h"

#define DEFAULT_DEVICE "/dev/ttyUSB0"
#define MAXLABELS 3000  /* Maximum number of labels */
#define MAXGOSUBS 128  /* Max depth */
#define STRINGL 1024   /* String lengths.  Also, max script line length */
#define MAXPATH 1024   /* Max filename length (less or equal to STRINGL) */
#define MAXTOKEN 20   /* Maximum token or label length */

#define BOOL unsigned char
#define NVARS 286 /* a-z, a0-z9 == 26*11 */

#define FILEOUTNAME "reimage.txt"

FILE *fileout = NULL;
extern char *optarg;
extern int optind, opterr;
FILE *filep;
unsigned char *script;
int scriptspace;
BOOL ifres;
int lastpc,pc; /* program "counters" */
long resultcode=0; /* result code */
int ignorecase=1;  /* no case sensitivity */
BOOL comecho=0; /* echo what's comin' in */
long senddelay=0; /* 0/100th second character delay for sending */
long number;  /* For getonearg() returning an long */
char **label; /* Index of labels for goto and gosub */
int *labelpc; /* Positions of said labels in script */
int labels;   /* Number of labels found in script */
long intvars[NVARS]; /* [a-z][0-9] integer variables */
char string[STRINGL]; /* For getstring() returns and misc. use (misuse) */
char *stringvars[NVARS]; /* $[a-z][0-9] string variables */
char cspeed[10];  /* Ascii representation of baudrate */
int speed=B0; /* Set to B110, B150, B300,..., B38400 */
char device[MAXPATH]; /* Comm device.  May be "-" */
char token[MAXTOKEN];   /* For gettoken() returns */
char scriptfile[MAXPATH]; /* Script file name */
BOOL verbose=0; /* Log actions */
struct termio cons, stbuf, svbuf;  /* termios: svbuf=before, stbuf=while */
int comfd=0; /* Communication file descriptor.  Defaults to stdin. */
char msg[STRINGL]; /* Massage messages here */
int preturn,returns[MAXGOSUBS];
int clocal=0;
int parity=0, bits=CS8, stopbits=0;
unsigned long hstart,hset;
char NullString[]={ "" };
BOOL lastcharnl=1; /* Indicate that last char printed from getonebyte
		      was a nl, so no new one is needed */
unsigned char *datafile;
int datafilesize = 0;
BOOL ymodem = 0;

int com_opened = 0;

/* Prototypes. */
long htime(void);
void dormir(unsigned long microsecs);
void dotestkey(void);
void ext(long xtc);
void vmsg(char *text);
void skipline(void);
void printwhere(void);
void writecom(char *text);
int getonebyte(void);
void dodump(void);
void serror(char *text, int exitcode);
void skipspaces(void);
void getopen(void);
void getclose(void);
int gettoken(void);
void skiptoken(void);
long getvalue(void);
void getcomma(void);
void gethardstring(void);
void getstring(void);
unsigned long getdvalue(void);
void dolet(void);
int dowaitquiet(void);
int dowaitfor(void);
BOOL getonoroff(void);
void setcom(void);
void doset(void);
void dogoto(void);
void dogosub(void);
unsigned char getonearg(void);
void doif(void);
int getindex(void);
int getintindex(void);
int getstringindex(void);
void doget(void);
void doprint(int channel);
void doclose(void);
void opendevice(void);
void doopen(void);
int doscript(void);
void flushcom(void);

int get_octal(char *string)
{
  int i;
  sscanf(string, "%o", &i);
  return i;
}

unsigned char *find_file(char *filename, int *plen)
{
  int i = 0;
  int size;

  if (datafilesize<0x200)
    return NULL;

  while(1)
    {
      size = get_octal((char *)datafile+i+124);
      
      if (strcmp((char *)datafile+i, filename)==0)
	{
	  if (plen)
	    *plen = size;
	  return datafile+i+0x200;
	}
      else
	{
	  i += size + 0x3ff;
	  i &= ~0x1ff;
	}
      if (i>datafilesize-0x200)
	return NULL;
    }

}

#define BUFSIZE 0x100000
#define BLOCKSIZE 0x1000

// read script into buffer
int load_script(void)
{
  FILE *fp;
  int i = 0, bufsize = BUFSIZE, res;

  datafile = (unsigned char *)malloc(BUFSIZE);
  fp = fopen(scriptfile, "r");
  
  if (fp==NULL)
    {
      perror("The following error occurred");
      return -1;
    }
  while(1)
    {
      for (; i<(bufsize-BLOCKSIZE); i+=BLOCKSIZE)
	{
	  res = fread((char *)datafile+i, 1, BLOCKSIZE, fp);
	  if (res<BLOCKSIZE)
	    {
	      i += res;
	      break;
	    }
	}
      if (i>=(bufsize-BLOCKSIZE))
	datafile = (unsigned char *)realloc(datafile, bufsize+=BUFSIZE);
      else
	break;      
    }
  datafilesize = i;
  return 0;
}
  

/* Returns hundreds of seconds */
long htime(void) {
  struct timeval timenow;
  gettimeofday(&timenow,NULL);
  return(100L*(timenow.tv_sec-hstart)+(timenow.tv_usec)/10000L-hset);
}

/* I use select() 'cause CX/UX 6.2 doesn't have usleep().
   On Linux, usleep() uses select() anyway.
*/
void dormir(unsigned long microsecs) {
  struct timeval timeout;
  timeout.tv_sec=microsecs/1000000L;
  timeout.tv_usec=microsecs-(timeout.tv_sec*1000000L);
  select(1,0,0,0,&timeout);
}

/* Tests for ENTER key */
void dotestkey(void) {
  fd_set fds;
  struct timeval timeout;
  timeout.tv_sec=0L;
  timeout.tv_usec=10000L;
  FD_ZERO(&fds);
  FD_SET(0,&fds);  /* Prepare to select() from stdin */
  resultcode=select(1,&fds,0,0,&timeout);
  if(resultcode) getchar();
}

/* Exit after resetting terminal settings */
void ext(long xtc) {
  ioctl(1, TCSETA, &cons);
  if (fileout!=stderr && fileout!=NULL)
    fclose(fileout);
  if (datafile)
    free(datafile);
  exit(xtc);
}

/* Log message if verbose is on */
void vmsg(char *text) {
  time_t t;
  char *ct;
  if(verbose) {
    if(lastcharnl==0) {
      fprintf(fileout,"\n");
      lastcharnl=1;
    }
    t=time(0);
    ct=ctime(&t);
    fprintf(fileout,"dcon %c%c:%c%c:%c%c -> %s\n",
            ct[11],ct[12],ct[14],ct[15],ct[17],ct[18],
	    text);
  }
}

/* Skip to next statement */
void skipline(void) {
  while(script[pc]!='\n' && script[pc]!=0) pc++;
  if(script[pc]) pc++;
}

void printwhere(void) {
  int a,b,c;
  char buf[0x100];
  sprintf(buf,"@%04d ",pc);
  a=pc;
  skipline();
  b=pc-1;
  pc=a;
  c=strlen(buf);
  for(;a<b;a++) buf[c++]=script[a];
  buf[c]=0;
  vmsg(buf);
}

/* Write a null-terminated string to communication device */
void writecom(char *text) {
  int res,a;
  char ch;
  for(a=0;(unsigned int)a<strlen(text);a++) {
    ch=text[a];
    if (ch=='\r')
      flushcom();
    while(1) {
      res=write(comfd,&ch,1);
      if(senddelay) dormir(senddelay);
      if(res==1) 
	break;
    }
  }
}

inline void putonebyte(char c) {
  while(write(comfd, &c, 1)!=1);
}

/* Gets a single byte from comm. device.  Return -1 if none avail. */
int getonebyte(void) {
#if 1
  fd_set rfds;
  int res;
  char ch;
  struct timeval timeout;
  timeout.tv_sec=0L;
  timeout.tv_usec=10000;
  FD_ZERO(&rfds);
  FD_SET(comfd, &rfds);
  res=select(comfd+1,&rfds,NULL,NULL,&timeout);
  if(res) {
    res=read(comfd,&ch,1);
    if(res==1) { 
      if(comecho&&!ymodem) {
        if(ch=='\n') lastcharnl=1;
        else {
          if(ch!='\r') lastcharnl=0;
        }
        fputc(ch,fileout);
      }
      return(ch);
    }
  }
  else {
    return(-1); /* Nada. */
  }
  return(0);
#else
  char ch;
  int res;
  struct timeval timeout;
  unsigned long usec;

  gettimeofday(&timeout, NULL);
  usec = timeout.tv_usec;
  while(1) {
    res = read(comfd, &ch, 1);
    if(res==1) { 
      if(comecho&&!ymodem) {
	if(ch=='\n') lastcharnl=1;
	else 
	  if(ch!='\r') lastcharnl=0;
	fputc(ch,fileout);
      }
      return(ch);
    }
    else {
      gettimeofday(&timeout, NULL);
      if (timeout.tv_usec-usec > 10000)
	return -1;
    }
  }
#endif
}

void flushcom(void)
{
  dormir(100000);
  while(getonebyte()!=-1);
  fflush(fileout);
}

void dodump(void) {
  char lmsg[STRINGL];
  int a,b,c;
  c=verbose;
  verbose=1;
  sprintf(lmsg,"-- Integer variables --"); vmsg(lmsg);
  for(a=0;a<26;a++) {
    for(b=0;b<11;b++) {
      if(intvars[b*26+a]) {
        sprintf(lmsg,"   = %ld",intvars[b*26+a]);
        lmsg[1]='a'+a;
        lmsg[2]=' ';
        if(b) lmsg[2]='0'+b-1;
        vmsg(lmsg);
      }
    }
  }
  sprintf(lmsg,"-- String variables --"); vmsg(lmsg);
  for(a=0;a<26;a++) {
    for(b=0;b<11;b++) {
      if(stringvars[b*26+a]!=NullString) {
        sprintf(lmsg,"$  = \"%s\"",stringvars[b*26+a]);
        lmsg[1]='a'+a;
        lmsg[2]=' ';
        if(b) lmsg[2]='0'+b-1;
        vmsg(lmsg);
      }
    }
  }
  verbose=c;
}

/* Report script errors and quit */
void serror(char *text, int exitcode) {
  int a,line;
  char lmsg[STRINGL];
  verbose=1;
  dodump();
  vmsg("-- Error Report --");
  a=pc;
  pc=lastpc;
  printwhere();
  pc=a;
  while(pc!=0 && (script[pc]==' ' || script[pc]=='\t')) pc--;
  strcpy(lmsg,"----> ");
  for(a=6;a<STRINGL;a++) lmsg[a]=' ';
  for(a=0;a<pc-lastpc;a++) {
    if(script[a+lastpc]=='\t') lmsg[a+8]='\t';
  }
  a+=6;
  lmsg[a++]='^';
  lmsg[a]=0;
  vmsg(lmsg);
  a=0; line=1;
  while(a<pc) {
    if(script[a++]=='\n') {
      if(a<pc) line++;
    }
  }
  sprintf(lmsg,"\nError @%d, line %d, %s. (%d)\n",pc,line,text,exitcode);
  fputs(lmsg, stderr);
  vmsg(lmsg);
  ext(exitcode);
}

void skipspaces(void) {
  while(script[pc]==' ' || script[pc]=='\t' ) pc++;
}

void getopen(void) {
  int a;
  a=pc;
  skipspaces();
  if(script[pc++]!='(') {
    pc=a;
    serror("Function requires open parenthesis",5);
  }
}

void getclose(void) {
  int a;
  a=pc;
  skipspaces();
  if(script[pc++]!=')') {
    pc=a;
    serror("Function requires close parenthesis",5);
  }
}

void skiptoken(void) {
  skipspaces();
  while(script[pc]!=' ' && script[pc]!='\t' && script[pc]!='\n' ) pc++;
  skipspaces();
}

/* Parse script[pc] to get next statement.  Resolve comments and labels... */
int gettoken(void) {
  int tokenp=0;
  skipspaces();
  if(script[pc]=='=' || script[pc]=='<' || script[pc]=='>' || script[pc]=='!') {
    token[0]=0;
    while(script[pc]=='=' || script[pc]=='<' || script[pc]=='>' || script[pc]=='!') {
      token[tokenp++]=script[pc++];
    }
    token[tokenp]=0;      
    return(0);
  }
  if(script[pc]=='#') {
    strcpy(token,"rem");
    pc++;
    return(0);
  }
  if(script[pc]=='/' && script[pc+1]=='/') {
    strcpy(token,"rem");
    pc+=2;
    return(0);
  }
  if(script[pc]==':') {
    strcpy(token,"label");
    pc++;
    return(0);
  }
  while(script[pc]!=' ' && script[pc]!='\n' &&
        script[pc]!='\t' && script[pc]!='(') {
    token[tokenp]=script[pc++];
    if(token[tokenp]>='A' && token[tokenp]<='Z') {
      token[tokenp]=token[tokenp]-'A'+'a';
    }
    if(tokenp++==MAXTOKEN-1) serror("Token too long",5);
  }
  token[tokenp]=0;
  if(strcmp(token,"then")==0) return(gettoken()); /* Ignore then for if */
  return(0);
}

/* shitfaced recursive value parser. must write better one */
long getvalue(void) {
  unsigned long p=0;
  int goone=1;
  int a,base,amode;
  skipspaces();
  if(script[pc]=='(') {
    pc++;
    p=getvalue();
    getclose();
  }
  else if(script[pc]==')') {
    return(p);
  }
  else if(script[pc]=='"' || script[pc]=='\'' || script[pc]=='$' ) {
    serror("Did not expect a string",7);
  }
  else if( (script[pc]>='a' && script[pc]<='z' &&
            script[pc+1]>='a' && script[pc+1]<='z') ||
           (script[pc]>='A' && script[pc]<='Z' &&
            script[pc+1]>='A' && script[pc+1]<='Z') ) {
    gettoken();
    getopen();
    if(strcmp(token,"len")==0) {
      char toto[STRINGL];
      strcpy(toto,string);
      getstring();
      p=strlen(string);
      strcpy(string,toto);
    }
    else if(strcmp(token,"htime")==0) {
      p=htime();
    }
    else if(strcmp(token,"time")==0) {
      p=time(0);
    }
    else if(strcmp(token,"pid")==0) {
      p=getpid();
    }
    else if(strcmp(token,"ppid")==0) {
      p=getppid();
    }
    else if(strcmp(token,"verbose")==0) {
      p=verbose;
    }
    else if(strcmp(token,"isatty")==0) {
      p=isatty(getvalue());
    }
    else if(strcmp(token,"baud")==0) {
      p=atol(cspeed);
    }
    else if(strcmp(token,"access")==0) {
      char toto[STRINGL];
      char afile[STRINGL];
      strcpy(toto,string);
      getstring();
      strcpy(afile,string);
      getcomma();
      getstring();
      if(string[0]==0) serror("Missing access mode[s]",5);
      amode=0;
      for(a=0;(unsigned int)a<strlen(string);a++) {
        switch(string[a]) {
	case 'R' :
	case 'r' : amode|=R_OK; break;
	case 'W' :
	case 'w' : amode|=W_OK; break;
	case 'X' :
	case 'x' : amode|=X_OK; break;
	case 'F' :
	case 'f' : amode|=F_OK; break;
	default : serror("Access modes are r,w,x, and f",5);
        }
      }
      p=access(afile,amode);
      strcpy(string,toto);
    }
    else if(strcmp(token,"val")==0 || strcmp(token,"atol")==0) {
      char toto[STRINGL];
      strcpy(toto,string);
      getstring();
      p=atol(string);
      strcpy(string,toto);
    }
    else serror("Unknown Integer function",5);
    getclose();
  }
  if(script[pc]=='%') {
    pc++;
    skipspaces();
    p=resultcode;
  }
  while(goone) {
    if(script[pc]=='+') {
      pc++;
      p+=getvalue();
    }
    else if(script[pc]=='-') {
      pc++;
      p-=getvalue();
    }
    else if(script[pc]=='^') {
      pc++;
      p^=getvalue();
    }
    else if(script[pc]=='&') {
      pc++;
      p&=getvalue();
    }
    else if(script[pc]=='|') {
      pc++;
      p|=getvalue();
    }
    else if(script[pc]=='*') {
      pc++;
      p*=getvalue();
    }
    else if(script[pc]=='/') {
      pc++;
      a=getvalue();
      if(a==0) serror("Division by zero",6);
      p/=a;
    }
    else if((script[pc]>='a' && script[pc]<='z') ||
            (script[pc]>='A' && script[pc]<='Z') ) {
      p=intvars[getintindex()];
    }
    else if(script[pc]>='0' && script[pc]<='9') { 
      base=10;
      if(script[pc]=='0') {
        base=8;
        pc++;
        if(script[pc]=='x' || script[pc]=='X') {
          base=16;
          pc++;
        }
        if(script[pc]=='%') {
          base=2;
          pc++;
        }
      }
      while((script[pc]>='0' && script[pc]<='9') ||
            (script[pc]>='a' && script[pc]<='f') ||
            (script[pc]>='A' && script[pc]<='F')) {
        if(script[pc]>='a' && script[pc]<='f') {
          p=p*base+script[pc++]-'a'+10;
        }
        else if(script[pc]>='A' && script[pc]<='F') {
          p=p*base+script[pc++]-'A'+10;
        }
        else {
          p=p*base+script[pc++]-'0';
        }
      }
    }
    else {
      goone=0;
    }
  }
  return(p);
}

void getcomma(void) {
  skipspaces();
  if(script[pc++]!=',') serror("Comma expected",5);
}

void gethardstring(void) {
  int a=0;
  skipspaces();
  while(script[pc]!=0 && script[pc]!=' ' && script[pc]!='\t' &&
        script[pc]!='\n') {
    string[a++]=script[pc++];
  }
  string[a]=0;
}

/* Parse a string from script[pc] */
void getstring(void) {
  FILE *fp;
  time_t t;
  int a,b,c,p=0;
  unsigned char ch,match;
  string[0]=0;
  skipspaces();
  if(script[pc]!='"' && script[pc]!='\'' && script[pc]!='$' ) {
    serror("Expected a string",7);
  }
  while(script[pc]!=' ' && script[pc]!='\t' && script[pc]!='\n' && script[pc]!=','  && script[pc]!=')' && script[pc]!='=' && script[pc]!='<' && script[pc]!='>' && script[pc]!='!' ) {
    if(script[pc]=='+') pc++;
    skipspaces();
    if( (script[pc]=='$' && script[pc+1]>='a' && script[pc+1]<='z' && 
         script[pc+2]>='a' && script[pc+2]<='z') ||
        (script[pc]=='$' && script[pc+1]>='A' && script[pc+1]<='Z' && 
         script[pc+2]>='A' && script[pc+2]<='Z') ) {
      pc++;
      gettoken();
      getopen();
      if(strcmp(token,"time")==0) {
        t=time(0);
        strcat(string,ctime(&t));
        string[strlen(string)-1]=0;
      }
      else if(strcmp(token,"rpipe")==0) {
        char toto[STRINGL];
        strcpy(toto,string);
        getstring();
        if((fp=popen(string,"r"))==NULL) serror("Could not popen",4);
        fgets(string,STRINGL-1,fp);
        string[strlen(string)-1]=0;
        pclose(fp);
        strcat(toto,string);
        strcpy(string,toto);
      }
      else if(strcmp(token,"env")==0) {
        char toto[STRINGL];
        strcpy(toto,string);
        getstring();
        if(getenv(string)) strcat(toto,(char *)getenv(string));
        strcpy(string,toto);
      }
      else if(strcmp(token,"hms")==0) {
        long sec,min,hour;
        sec=getvalue();
        min=sec/60L;
        sec-=min*60L;
        hour=min/60L;
        min-=hour*60L;
        sprintf(string,"%s%02ld:%02ld:%02ld",string,hour,min,sec);
      }
      else if(strcmp(token,"dev")==0) {
        strcat(string,device);
      }
      else if(strcmp(token,"cwd")==0) {
        getcwd(string,STRINGL);
      }
      else if(strcmp(token,"baud")==0) {
        strcat(string,cspeed);
      }
      else if(strcmp(token,"str")==0 || strcmp(token,"ltoa")==0) {
        sprintf(string,"%s%ld",string,getvalue());
      }
      else if(strcmp(token,"hexu")==0) {
        sprintf(string,"%s%lX",string,getvalue());
      }
      else if(strcmp(token,"hex")==0) {
        sprintf(string,"%s%lx",string,getvalue());
      }
      else if(strcmp(token,"oct")==0) {
        sprintf(string,"%s%lo",string,getvalue());
      }
      else if(strcmp(token,"dirname")==0) {
        char toto[STRINGL];
        strcpy(toto,string);
        getstring();
        b=0;
        for(a=0;(unsigned int)a<strlen(string);a++) {
          if(string[a]=='/' || string[a]=='\\') b=a;
        }
        string[b]=0;
        strcat(toto,string);
        strcpy(string,toto);
      }
      else if(strcmp(token,"tolower")==0) {
        char toto[STRINGL];
        strcpy(toto,string);
        getstring();
        for(a=0;(unsigned int)a<strlen(string);a++) {
          if(string[a]>='A' && string[a]<='Z' ) string[a]=string[a]-'A'+'a';
        }
        strcat(toto,string);
        strcpy(string,toto);
      }
      else if(strcmp(token,"toupper")==0) {
        char toto[STRINGL];
        strcpy(toto,string);
        getstring();
        for(a=0;(unsigned int)a<strlen(string);a++) {
          if(string[a]>='a' && string[a]<='z' ) string[a]=string[a]-'a'+'A';
        }
        strcat(toto,string);
        strcpy(string,toto);
      }
      else if(strcmp(token,"basename")==0) {
        char toto[STRINGL];
        strcpy(toto,string);
        getstring();
        b=0;
        for(a=0;(unsigned int)a<strlen(string);a++) {
          if(string[a]=='/' || string[a]=='\\') b=a+1;
        }
        a=strlen(toto);
        while(string[b]) toto[a++]=string[b++];
        toto[a]=0;
        strcpy(string,toto);
      }
      else if(strcmp(token,"script")==0) {
        strcat(string,scriptfile);
      }
      else if(strcmp(token,"right")==0) {
        char toto[STRINGL];
        strcpy(toto,string);
        getstring();
        getcomma();
        b=getvalue();
        if((unsigned int)b>strlen(string)) serror("String is shorter than second argument",7);
        c=strlen(toto);
        a=strlen(string)-b;
        while(b!=0 && string[a]!=0) {
          toto[c++]=string[a++];
          b--;
        }
        toto[c]=0;
        strcpy(string,toto);
      }
      else if(strcmp(token,"left")==0) {
        char toto[STRINGL];
        strcpy(toto,string);
        getstring();
        getcomma();
        b=getvalue();
        if((unsigned int)b>strlen(string)) serror("String is shorter than second argument",7);
        c=strlen(toto);
        a=0;
        while(b!=0 && string[a]!=0) {
          toto[c++]=string[a++];
          b--;
        }
        toto[c]=0;
        strcpy(string,toto);
      }
      else if(strcmp(token,"mid")==0) {
        char toto[STRINGL];
        strcpy(toto,string);
        getstring();
        getcomma();
        a=getvalue();
        getcomma();
        b=getvalue();
        if((unsigned int)a>strlen(string)) serror("String is shorter than second argument",7);
        c=strlen(toto);
        while(b!=0 && string[a]!=0) {
          toto[c++]=string[a++];
          b--;
        }
        toto[c]=0;
        strcpy(string,toto);
      }
      else if(strcmp(token,"sex")==0) {
        strcat(string,"You're a naughty boy, you!");
      }
      else serror("Invalid string funtion",5);
      getclose();
    }
    else if(script[pc]=='$') {
      strcat(string,stringvars[getstringindex()]);
    }
    else if(script[pc]=='"' || script[pc]=='\'') {
      match=script[pc++];
      while(script[pc]!=match) {
        ch=script[pc++];
        if(ch==0) serror("Umatched quote",5);
        if(ch=='\\') {
          if(script[pc]<='7' && script[pc]>='0' &&
	     script[pc+1]<='7' && script[pc+1]>='0' ) {
            ch=0;
            while(script[pc]>='0' && script[pc]<='7') {
              ch=8*ch+script[pc++]-'0';
            }
          }
          else {
            switch(script[pc]) {
	    case 'T' :
	    case 't' : ch=9;  break;
	    case 'R' :
	    case 'r' : ch=13; break;
	    case 'N' :
	    case 'n' : ch=10; break;
	    case 'B' :
	    case 'b' : ch=8;  break;
	    case 'F' :
	    case 'f' : ch=12; break;
	    case '"' :
	    case '^' :
	    case '\'' :
	    case '\\' : ch=script[pc]; break;
	    default :  serror("Malformed escaped character",5);
            }
            pc++;
          }
        }
        else if(ch=='^') {
          ch=script[pc];
          if(ch!='^' && ch!='"' && ch!='\'' && ch!='\\' ) {
            ch=ch&31; /* Control char */
          }
          pc++;
        }
        p=strlen(string);
        string[p++]=ch;
        string[p]=0;
      }
      pc++; /* Space over quote */
    }
    else {
      p=strlen(string);
      string[p++]=script[pc++];
      string[p]=0;
    }
  }
}

/* Get a value, multiply by a hundred (for time values) */
unsigned long getdvalue(void) {
  float f;
  gettoken();
  skipspaces();
  sscanf(token,"%f",&f);
  f+=0.00001; /* Rounding errors */
  return((unsigned long)(100.0*f));
}

void dolet(void) {
  int index;
  BOOL svar=0;
  skipspaces();
  if(script[pc]=='$') {
    svar=1;
    index=getstringindex();
  }
  else index=getintindex();
  skipspaces();
  gettoken();
  if(strcmp(token,"=")!=0) serror("Bad LET assignment, '=' missing",5);
  skipspaces();
  if(svar) {
    getstring();
    strcpy(stringvars[index],string);
  }
  else {
    intvars[index]=getvalue();
  }
}

/* See documentation for doXXX() functions */
int dowaitquiet(void) {
  unsigned long timeout,timequiet,quiet,now;
  int c,quit;
  timeout=htime()+getdvalue();
  quiet=getdvalue();
  timequiet=htime()+quiet;
  quit=1;
  while(quit==1) {
    now=htime();
    c=getonebyte();
    if(c!= -1) timequiet=now+quiet;
    if(now>=timequiet) quit=0;
    if(now>=timeout) quit=255;
  }
  return(quit);
}

int dowaitfor(void) {
  char strings[20][80];
  char buffer[128];
  unsigned long timeout;
  int a,b,c;
  b=0;
  buffer[127]=0;
  skipspaces();
  timeout=htime()+getdvalue();
  while(script[pc]==',' || script[pc]=='$' || script[pc]=='"' ||
        script[pc]=='\'' ) {
    if(script[pc]==',') pc++;
    getstring();
    skipspaces();
    strcpy(strings[b],string);
    if(ignorecase) {
      for(a=0;(unsigned int)a<strlen(strings[b]);a++) {
        if(strings[b][a]>='A' && strings[b][a]<='Z') {
          strings[b][a]=strings[b][a]-'A'+'a';
        }
      }
    }
    b++;
  }
  strings[b][0]=0;
  while((unsigned long)htime()<timeout) {
    c=getonebyte();
    if(c!= -1) {
      if(ignorecase) {
        if(c>='A' && c<='Z') c=c-'A'+'a';
      }
      for(a=0;a<126;a++) buffer[a]=buffer[a+1];
      buffer[126]=c;
      b=0;
      while(strings[b][0]) {
        c=strlen(strings[b]);
        a=c-1;
        while(a>=0 && strings[b][a]==buffer[127-c+a]) a--;
        if(a<0) return(b);
        b++;
      }
    }
  }
  serror("timeout exceeded", 1);
  return(-1);
}

/* Parse script for "on" or "off" wich are tokens, not strings */
BOOL getonoroff(void) {
  int a,b;
  b=pc;
  gettoken();
  if(strcmp(token,"on")==0) return(1);
  if(strcmp(token,"off")==0) return(0);
  pc=b;
  a=getvalue();
  if(a!=0 && a!=1) serror("Bad value (should be on or off, 1 or 0)",5);
  return(a);
}

void setcom(void) {
  if (!com_opened) {
    strcpy(device, DEFAULT_DEVICE);
    opendevice();
  }
  stbuf.c_cflag &= ~(CBAUD | CSIZE | CSTOPB | CLOCAL | PARENB | ICANON | CRTSCTS);
  stbuf.c_cflag |= (speed | bits | CREAD | clocal | parity | stopbits );
  if (ioctl(comfd, TCSETA, &stbuf) < 0) {
    serror("Can't ioctl set device",1);
  }
}

void doset(void) {
  struct termio console;
  int a,b;
  gettoken();
  if(strcmp(token,"echo")==0) {
    a=0;
    if(getonoroff()) a=ECHO|ECHOE;
    if(ioctl(0, TCGETA, &console)<0) {
      serror("Can't ioctl FD zero",2);
    }
    console.c_lflag &= ~(ECHO | ECHOE);
    console.c_lflag |= a;
    ioctl(0, TCSETA, &console);
  }
  else if(strcmp(token,"senddelay")==0) {
    senddelay=10000L*getdvalue();
  }
  else if(strcmp(token,"clocal")==0) {
    clocal=0;
    if(getonoroff()) clocal=CLOCAL;
    setcom();
  }
  else if(strcmp(token,"umask")==0) {
    umask(getvalue()&0777);
  }
  else if(strcmp(token,"verbose")==0) {
    verbose=getonoroff();
  }
  else if(strcmp(token,"comecho")==0) {
    comecho=getonoroff();
  }
  else if(strcmp(token,"ignorecase")==0) {
    ignorecase=getonoroff();
  }
  else if(strcmp(token,"com")==0) {
    skipspaces();
    if(script[pc]=='$' || script[pc]=='\'' || script[pc]=='"') {
      getstring();
      strcpy(token,string);
    }
    else gettoken();
    a=0;
    b=0;
    while(token[b]>='0' && token[b]<='9') {
      a=10*a+token[b++]-'0';
    }
    if(token[b]) {
      switch(token[b]) {
      case 'n': parity=0; break;
      case 'e': parity=PARENB; break;
      case 'o': parity=PARENB|PARODD; break;
      default : serror("Parity can only ben E, N, or O",5);
      }
      b++;
      if(token[b]) {
        switch(token[b]) {
	case '5' : bits=CS5; break;
	case '6' : bits=CS6; break;
	case '7' : bits=CS7; break;
	case '8' : bits=CS8; break;
	default : serror("Bits can only be 5, 6, 7, or 8",5);
        }
        b++;
        if(token[b]) {
          switch(token[b]) {
	  case '1': stopbits=0; break;
	  case '2': stopbits=CSTOPB; break;
	  default : serror("Stop bits can only be 1 or 2",5);
          }
        }
      }
    }
    sprintf(cspeed,"%d",a);
    switch(a) {
    case 0: speed = B0;break;
    case 50: speed = B50;break;
    case 75: speed = B75;break;
    case 110: speed = B110;break;
    case 150: speed = B150;break;
    case 300: speed = B300;break;
    case 600: speed = B600;break;
    case 1200: speed = B1200;break;
    case 2400: speed = B2400;break;
    case 4800: speed = B4800;break;
    case 9600: speed = B9600;break;
    case 19200: speed = B19200;break;
    case 38400: speed = B38400;break;
    case 57600: speed = B57600;break;
    case 115200: speed = B115200;break;
    case 230400: speed = B230400;break;
    case 460800: speed = B460800;break;
    default: serror("Invalid baudrate",1);
    }
    setcom();
  }
}

void dogoto(void) {
  int a,originalpos;
  originalpos=pc;
  gettoken();
  for(a=0;a<labels;a++) {
    if(strcmp(token,label[a])==0) break;
  }
  if(a>=labels) { 
    pc=originalpos;
    sprintf(msg,"Label \"%s\" not found",token);
    serror(msg,5);
  }
  else {
    pc=labelpc[a];
  }
}

void dogosub(void) {
  int a;
  if(preturn==MAXGOSUBS) serror("Reached maximum GOSUB depth",3);
  a=pc;
  gettoken();
  returns[preturn++]=pc;
  pc=a;
  dogoto();
}

/* Gets arguments and returns 0 for a string, 1 for an int. Used with if */
BOOL getonearg(void) {
  if(script[pc]=='"' || script[pc]=='\'' || script[pc]=='$' ) {
    getstring();
    return(0);
  }
  else {
    number=getvalue();
    return(1);
  }
}

void doif(void) {
  char stringarg[STRINGL];
  char tokencopy[MAXTOKEN];
  int intarg;
  skipspaces();
  ifres=0;
  if(getonearg()) {
    intarg=number;
    gettoken();
    skipspaces();
    if(getonearg()!=1) serror("Comparison mis-match",7);
    if(strcmp(token,"<")==0) {
      if(intarg<number) ifres=1;
    }
    if(strcmp(token,"<=")==0) {
      if(intarg<=number) ifres=1;
    }
    else if(strcmp(token,"=")==0) {
      if(intarg==number) ifres=1;
    }
    else if(strcmp(token,">")==0) {
      if(intarg>number) ifres=1;
    }
    else if(strcmp(token,">=")==0) {
      if(intarg>=number) ifres=1;
    }
    else if(strcmp(token,"<>")==0) {
      if(intarg!=number) ifres=1;
    }
    else if(strcmp(token,"!=")==0 || strcmp(token,"<>")==0) {
      if(intarg!=number) ifres=1;
    }
  }
  else {
    strcpy(stringarg,string);
    gettoken();
    strcpy(tokencopy,token);
    skipspaces();
    if(getonearg()!=0) serror("Comparison mis-match",7);
    if(strcmp(tokencopy,"<")==0) {
      if(strcmp(stringarg,string)<0) ifres=1;
    }
    if(strcmp(tokencopy,"<=")==0) {
      if(strcmp(stringarg,string)<=0) ifres=1;
    }
    else if(strcmp(tokencopy,"=")==0) {
      if(strcmp(stringarg,string)==0) ifres=1;
    }
    else if(strcmp(tokencopy,">")==0) {
      if(strcmp(stringarg,string)>0) ifres=1;
    }
    else if(strcmp(tokencopy,">=")==0) {
      if(strcmp(stringarg,string)>=0) ifres=1;
    }
    else if(strcmp(tokencopy,"!=")==0 || strcmp(tokencopy,"<>")==0) {
      if(strcmp(stringarg,string)!=0) ifres=1;
    }
  }
  if(!ifres) skipline();
}

int getindex(void) {
  int index;
  index=script[pc++];
  if(index>='A' && index<='Z') index=index-'A'+'a';
  if(index>'z' || index<'a') serror("Malformed variable name",7);
  index=index-'a';
  if(script[pc]>='0' && script[pc]<='9') {
    index=index+(1+script[pc++]-'0')*26;
  }
  return(index);
}

/* Parse script to find integer variable index */
int getintindex(void) {
  skipspaces();
  if(script[pc]=='$') serror("Integer variable expected",7);
  return(getindex());
}

/* Parse script to find string variable index and allocate memory for storage
   as needed. */
int getstringindex(void) {
  int index;
  skipspaces();
  if(script[pc++]!='$') serror("String variable expected",7);
  index=getindex();
  if(stringvars[index]==NullString) {
    stringvars[index]=(char *)malloc(STRINGL);
    if(stringvars[index]==NULL) serror("Could not malloc",3);
    stringvars[index][0]=0;
  }
  return(index);
}

void doget(void) {
  char terminators[STRINGL];
  int a,b,c,index;
  int goahead=1;
  unsigned long timeout;
  timeout=htime()+getdvalue();
  getstring();
  strcpy(terminators,string);
  index=getstringindex();
  string[0]=0;
  b=0;
  resultcode=0;
  while(goahead && (unsigned long)htime()<timeout) {
    c=getonebyte();
    if(c!= -1) {
      for(a=0;(unsigned int)a<strlen(terminators);a++) {
        if(c==terminators[a]) goahead=0;
      }
      if(goahead==0 && b==0) goahead=1; /* Ignore terminators if nothing yet */
      else if(goahead) {
        string[b++]=c;
        string[b]=0;
      }
    }
  }
  if(goahead) resultcode= -1;
  strcpy(stringvars[index],string);
}

void doprint(int channel) {
  skipspaces();
  msg[0]=0;
  while(script[pc]!=' ' && script[pc]!='\t' && script[pc]!='\n') {
    if(script[pc]==',') pc++;
    else {
      if(script[pc]=='"' || script[pc]=='\'' || script[pc]=='$' ) {
        getstring();
        strcat(msg,string);
      }
      else {
        sprintf(string,"%ld",getvalue());
        strcat(msg,string);
      }
    }
  }
  switch(channel) {
  case 1: printf("%s",msg); fflush(stdout); break;
  case 2: fputs(msg,fileout); break;
  case 3: 
    if(msg[strlen(msg)-1]=='\n') msg[strlen(msg)-1]=0;
    vmsg(msg);
    break;
  case 4: 
    if(filep==NULL) serror("File not opened",4);
    fputs(msg,filep);
    break;
  }
}

void doclose(void) {
  gettoken();
  if(strcmp(token,"hardcom")==0) {
    if(comfd== -1) serror("Com device not open",1);
    vmsg("Closing device");
    if (ioctl(comfd, TCSETA, &svbuf) < 0) {
      sprintf(msg,"Can't ioctl set device %s",device);
      serror(msg,1);
    }
    close(comfd);
    comfd= -1;
  }
  else if(strcmp(token,"com")==0) {
    if(comfd== -1) serror("Com device not open",1);
    vmsg("Closing com fd");
    close(comfd);
    comfd= -1;
  }
  else if(strcmp(token,"file")==0) {
    if(filep==NULL) serror("Log file not open",4);
    fclose(filep);
    filep=NULL;
  }
}

void opendevice(void) {
  if(strcmp(device,"-")!=0) {
    if ((comfd = open(device, O_RDWR|O_EXCL|O_NDELAY)) <0) {
      sprintf(msg,"Can't open device %s",device);
      serror(msg,1);
    }
  }
  else comfd=0;
  com_opened = 1;
  if (ioctl (comfd, TCGETA, &svbuf) < 0) {
    sprintf(msg,"Can't ioctl get device %s",device);
    serror(msg,1);
  }
  ioctl(comfd, TCGETA, &stbuf);
  speed=stbuf.c_cflag & CBAUD;
  switch(speed) {
  case B0: strcpy(cspeed,"0");break;
  case B50: strcpy(cspeed,"50");break;
  case B75: strcpy(cspeed,"75");break;
  case B110: strcpy(cspeed,"110");break;
  case B300: strcpy(cspeed,"300");break;
  case B600: strcpy(cspeed,"600");break;
  case B1200: strcpy(cspeed,"1200");break;
  case B2400: strcpy(cspeed,"2400");break;
  case B4800: strcpy(cspeed,"4800");break;
  case B9600: strcpy(cspeed,"9600");break;
  case B19200: strcpy(cspeed,"19200");break;
  case B38400: strcpy(cspeed,"38400");break;
  }
  bits=stbuf.c_cflag & CSIZE;
  clocal=stbuf.c_cflag & CLOCAL;
  stopbits=stbuf.c_cflag & CSTOPB;
  parity=stbuf.c_cflag & (PARENB | PARODD);
  stbuf.c_iflag &= ~(IGNCR | ICRNL | IUCLC | INPCK | IXON | IXANY | IGNPAR );
  stbuf.c_oflag &= ~(OPOST | OLCUC | OCRNL | ONLCR | ONLRET);
  stbuf.c_lflag &= ~(ICANON | XCASE | ECHO | ECHOE | ECHONL);
  stbuf.c_lflag &= ~(ECHO | ECHOE);
  stbuf.c_cc[VMIN] = 1;
  stbuf.c_cc[VTIME] = 0;
  stbuf.c_cc[VEOF] = 1;
  setcom();
  dormir(200000); /* Wait a bit (DTR raise) */
  sprintf(msg,"Opened %s as FD %d",device,comfd);
  vmsg(msg);
}

void doopen(void) {
  gettoken();
  if(strcmp(token,"com")==0) {
    skipspaces();
    if(script[pc]=='$' || script[pc]=='\'' || script[pc]=='"') {
      getstring();
    }
    else gethardstring();
    strcpy(device,string);
    opendevice();
  }
  else if (strcmp(token,"file")==0) {
    if(filep!=NULL) serror("File already open",4);
    getstring();
    if((filep=fopen(string,"a"))==NULL) serror("Could not open file",4);
  }
  else serror("OPEN only takes com or file argument",5);
}

int doscript(void) {
  int a,b,len;
  int exitcode=0;
  unsigned char *file;
  char line[STRINGL];

  pc=0;
  while(script[pc]) {
    if(script[pc]=='\n') pc++;
    lastpc=pc;
    skipspaces();
    if(verbose) printwhere();
    if(gettoken()) serror("Could not gettoken()",5);
    if(strcmp(token,"rem")==0) {
      skipline();
    }
    else if (strcmp(token,"label")==0) {
      skiptoken(); /* Get rid of keyword */
    }
    else if(strcmp(token,"open")==0) {
      doopen();
    }
    else if(strcmp(token,"close")==0) {
      doclose();
    }
    else if(strcmp(token,"exec")==0) {
      getstring();
      strcpy(msg,"exec ");
      strcat(msg,string); /* Let sh do all the command line work! */
      execl("/bin/sh","sh","-c",msg,(char *)0);
      serror("Could not execl /bin/sh!",8);
    }
    else if(strcmp(token,"exit")==0) {
      ext(getvalue());
    }
    else if(strcmp(token,"testkey")==0) {
      dotestkey();
    }
    else if(strcmp(token,"kill")==0) {
      a=getvalue();
      resultcode=kill(getvalue(),a);
    }
    else if(strcmp(token,"fork")==0) {
      resultcode=fork();
    }
    else if(strcmp(token,"hset")==0) {
      hset=0;
      hstart=time(0);
      hset=htime()-getvalue();
    }
    else if(strcmp(token,"cd")==0) {
      getstring();
      resultcode=chdir(string);
    }
    else if(strcmp(token,"putenv")==0) {
      getstring();
      strcpy(line,string);
      resultcode=putenv(line); /* putenv can't read from global string[] */
    }
    else if(strcmp(token,"wait")==0) {
      resultcode=wait(0);
    }
    else if(strcmp(token,"system")==0) {
      getstring();
      system(string);
    }
    else if(strcmp(token,"input")==0) {
      gets(stringvars[getstringindex()]);
    }
    else if(strcmp(token,"get")==0) {
      doget();
    }
    else if(strcmp(token,"print")==0) {
      doprint(1);
    }
    else if(strcmp(token,"eprint")==0) {
      doprint(2);
    }
    else if(strcmp(token,"lprint")==0) {
      doprint(3);
    }
    else if(strcmp(token,"fprint")==0) {
      doprint(4);
    }
    else if(strcmp(token,"if")==0) {
      doif();
    }
    else if(strcmp(token,"else")==0) {
      if(ifres) skipline();
    }
    else if(strcmp(token,"gosub")==0) {
      dogosub();
    }
    else if(strcmp(token,"return")==0) {
      if(preturn==0) serror("RETURN without gosub",5);
      pc=returns[--preturn];
    }
    else if(strcmp(token,"goto")==0) {
      dogoto();
    }
    else if(strcmp(token,"waitfor")==0) {
      resultcode=dowaitfor();
    }
    else if(strcmp(token,"waitquiet")==0) {
      resultcode=dowaitquiet();
    }
    else if(strcmp(token,"set")==0) {
      doset();
    }
    else if(strcmp(token,"dec")==0) {
      intvars[getintindex()]--;
    }
    else if(strcmp(token,"inc")==0) {
      intvars[getintindex()]++;
    }
    else if(strcmp(token,"let")==0) {
      dolet();
    }
    else if(strcmp(token,"dump")==0) {
      dodump();
    }
    else if(strcmp(token,"abort")==0) {
      vmsg("Aborting");
      abort();
    }
    else if(strcmp(token,"send")==0) {
      getstring();
      writecom(string);
    }
    else if (strcmp(token,"ymodem")==0) {
      getstring();
      file=find_file(string, &len);
      if (file==NULL)
	{
	  sprintf(msg, "Can't find file %s", string);
	  vmsg(msg);
	}
      else
	{
	  sprintf(msg, "Sending ymodem file %s", string);
	  vmsg(msg);
	  ymodem = 1;
	  ymodem_send_data(file, len, string);
	  ymodem = 0;
	  fprintf(fileout, "\n");
	}
    }
    else if(strcmp(token,"flash")==0) {
      b=speed;
      speed=0;
      setcom();
      a=getdvalue();
      dormir(10000L*a);
      speed=b;
      setcom();
    }
    else if(strcmp(token,"sleep")==0) {
      a=getdvalue();
      if(a<10000) dormir(10000L*a);
      else sleep(a/100); /* I guess it's the same.  Oh well, past 100 secs,
                            use sleep instead.  */
    }
    else {
      /* Humour is the spice of life. */
      switch(time(0)&7) {
      case 0 : serror("That's human mumbo-jumbo to me",5); break;
      case 1 : serror("Lovely but incomprehensible",5); break;
      case 2 : serror("What's that, governor?",5); break;
      case 3 : serror("Very funny.  I don't get it",5); break;
      case 4 : serror("Huh?",5); break;
      case 5 : serror("dcon doesn't speak spanish",5); break;
      case 6 : serror("Mais, qu'est-ce que vous dites?",5); break;
      default: serror("%E-6837-% : Corrupted human data detected",5); break;
      }
    }
    skipspaces();
    while(script[pc]=='\n') pc++;
  }
  return(exitcode);
}

int main(int argc,char **argv) {
  int a,b,i;
  unsigned char ch;
  unsigned char terminator='\n';

  printf("\nreimage -- firmware utility for VEXPro\n\n");

  fileout = fopen(FILEOUTNAME, "w");

  hstart=time(0);
  hset=htime();
  preturn=0;
  filep=NULL;
  scriptspace=4096;
  ioctl(1, TCGETA, &cons);
  if((script=(unsigned char *)malloc(scriptspace))==NULL) {
    serror("Could not malloc()",3);
  }
  for(a=0;a<NVARS;a++) {
    intvars[a]=0;
    stringvars[a]=NullString;
  }
  strcpy(cspeed,"0");
  scriptfile[0]=0;
  b=0; a=0;
  for(a=0;(unsigned int)a<strlen(argv[0]);a++) {
    if(argv[0][a]=='/') b=a+1;
  }
  while((a=getopt(argc,argv,"evd:t:s:b:"))!= -1) {
    switch(a) {
    case 't' :
      terminator=optarg[0];
      sprintf(msg,"Alternate line terminator set to \"%c\"",terminator);
      vmsg(msg);
      break;
    case 'd' :
      strcpy(device,optarg);
      opendevice();
      break;
    case 'e' :
      comecho=1;
      vmsg("Communication echo turned on");
      break;
    case 'v' :
      verbose=1;
      vmsg("Verbose output enabled");
      break;
    case 's' :
      scriptspace=strlen(optarg)+2;
      if((script=(unsigned char *)realloc(script,scriptspace))==0) {
	serror("Could not malloc()",3);
      }
      strcpy((char *)script,optarg);
      for(a=0;a<scriptspace;a++) {
	if(script[a]==terminator) script[a]='\n';
      }
      sprintf(msg,"Read %d bytes mini-script from command line",scriptspace-1);
      vmsg(msg);
      break;
    default :
      fprintf(fileout,"I don't get '%s'.\n",optarg);
      exit(1);
      break;
    }
  }
  if(optind<argc) {
    strcpy(scriptfile,argv[optind++]);
    sprintf(msg,"Script file: %s",scriptfile);
    vmsg(msg);
  }
  load_script();

  script = find_file("script", NULL);
  if (script==NULL)
    {
      sprintf(msg,"Could not find script file");
      serror(msg,1);
    }

  i=strlen((char *)script); /* Script is one huge string */
  /* Indexing labels */
  label=(char **)malloc(sizeof(char *)*MAXLABELS);
  labelpc=(int *)malloc(sizeof(int *)*MAXLABELS);
  if(label==NULL || labelpc==NULL) {
    serror("Can't malloc",3);
  }
  labels=0;
  pc=0;
  while(script[pc]) {
    lastpc=pc;
    gettoken();
    if(strcmp(token,"label")==0) {
      gettoken();
      for(a=0;a<labels;a++) {
        if(strcmp(token,label[a])==0) {
          pc=lastpc;
          serror("Duplicate label",5);
        }
      }
      if(labels==MAXLABELS) serror("Maximum number of labels reached",3);
      labelpc[labels]=lastpc;
      label[labels]=(char *)malloc(strlen(token)+1);
      if(label[labels]==NULL) serror("Can't malloc one label",3);
      strcpy(label[labels],token);
      labels++;
    }
    skipline();
  }
  if(verbose) {
    sprintf(msg,"argc:%d",argc);
    vmsg(msg);
    for(a=0;a<argc;a++) {
      sprintf(msg,"argv[%d]=%s",a,argv[a]);
      vmsg(msg);
    }
    vmsg(" ---Script---");
    a=0; b=0; ch='\n';
    while(a<i) {
      if(ch=='\n' && script[a]!=0) {
        fprintf(fileout,"%4d@%04d ",++b,a);
      }
      ch=script[a++];
      fputc(ch,fileout);
    }
    vmsg(" ---End of script---");
  }
  if(script[0]==0) {
    fprintf(fileout,"No script!\n");
    ext(1);
  }
  a=doscript();
  dormir(200000);
  if(comfd!= -1) close(comfd);
  sprintf(msg,"Exit with code %d.\n",a);
  vmsg(msg);
  ext(a);
  exit (0);
}
