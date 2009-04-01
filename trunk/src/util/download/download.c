//****************************************************************************
//
// DOWNLOAD.C - Automates the download of code into the NOR FLASH on the
//              EP73xx and EP93xx boards.
//
// Copyright (c) 1999,2000,2001,2002,2003,2004 Cirrus Logic, Inc.
//
//****************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flasher.h"
#ifdef __CYGWIN__
#define _WIN32
#endif
#ifdef _WIN32
#include <windows.h>
#include "getopt.h"
#endif
#ifdef __linux__
#include <sys/ioctl.h>
#include <sys/timeb.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#endif

//****************************************************************************
//
// lSerialPort is the serial port which is being used.
//
//****************************************************************************
#ifdef _WIN32
HANDLE hSerialPort;
#endif
#ifdef __linux__
long lSerialPort;
#endif

//****************************************************************************
//
// OpenPort opens the specified serial port.
//
//****************************************************************************
int
OpenPort(long lPort)
{
#ifdef _WIN32
    char pcName[16];

    //
    // Create the device name for the given serial port.
    //
    sprintf(pcName, "COM%d", lPort);

    //
    // Open the serial port.
    //
    hSerialPort = CreateFile(pcName, GENERIC_READ | GENERIC_WRITE, 0, 0,
                             OPEN_EXISTING, 0, 0);
    if(hSerialPort == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Could not open serial port %s.\n", pcName);
        return(0);
    }

    //
    // Success.
    //
    return(1);
#elif defined(__linux__)
    char pcName[16], pcLock[32], pcBuffer[64];
    long lLock, lCount, lPid;
    struct termios buf;

    //
    // Create the file name for the lock file.
    //
    sprintf(pcLock, "/var/lock/LCK..ttyUSB%d", lPort - 1);

    //
    // Attempt to open the lock file.
    //
    lLock = open(pcLock, O_RDONLY);
    if(lLock)
    {
        //
        // The lock file exists, so read from it and then close it.
        //
        lCount = read(lLock, pcBuffer, 63);
        close(lLock);

        //
        // See if any data was read from the lock file.
        //
        if(lCount > 0)
        {
            //
            // The PID is currently unknown, so set it to -1.
            //
            lPid = -1;

            //
            // See if the lock file contained only four bytes, indicating a
            // Kermit-style lock file.
            //
            if(lCount == 4)
            {
                //
                // Extract the PID from the lock file data.
                //
                lPid = *(long *)pcBuffer;
            }

            //
            // Otherwise, the lock file is in ASCII.
            //
            else
            {
                //
                // Make sure that the string is NULL terminated.
                //
                pcBuffer[lCount] = 0;

                //
                // Convert the PID from ASCII to an integer.
                //
                sscanf(pcBuffer, "%d", &lPid);
            }

            //
            // If a PID was found, see if the process still exists.
            //
            if((lPid > 0) && (kill((pid_t)lPid, 0) < 0) && (errno == ESRCH))
            {
                //
                // The process no longer exists, meaning that the lock is
                // stale.  Delete the lock file.
                //
                unlink(pcLock);
            }
            else
            {
                //
                // The process still exists, so the port is locked.
                //
                lCount = 0;
            }
        }

        //
        // See if the port is locked.
        //
        if(lCount == 0)
        {
            //
            // Indicate that the port is locked.
            //
            fprintf(stderr, "Serial port /dev/ttyUSB%d is locked!\n", lPort - 1);
            return(0);
        }
    }

    //
    // Create the lock file.
    //
    lLock = creat(pcLock, 0666);
    if(lLock >= 0)
    {
        //
        // Print our PID to the lock file.
        //
        sprintf(pcBuffer, "%ld\n", (long)getpid());
        write(lLock, pcBuffer, strlen(pcBuffer));
        close(lLock);
    }

    //
    // Create the device name for the given serial port.
    //
    sprintf(pcName, "/dev/ttyUSB%d", lPort - 1);

    //
    // Open the serial port.
    //
    lSerialPort = open(pcName, O_RDWR | O_NONBLOCK);
    if(lSerialPort < 0)
    {
        unlink(pcLock);
        fprintf(stderr, "Could not open serial port %s.\n", pcName);
        return(0);
    }

    //
    // Get the attributes of the serial port.  This will fail if the serial
    // port does not exist (even though the open will succeed if the serial
    // port does not exist).
    //
    if(tcgetattr(lSerialPort, &buf))
    {
        unlink(pcLock);
        fprintf(stderr, "Could not open serial port %s.\n", pcName);
        return(0);
    }

    //
    // Success.
    //
    return(1);
#else
    //
    // There is no serial port support for the target environment, so return
    // an error.
    //
    fprintf(stderr, "No serial port support!\n");
    return(0);
#endif
}

//****************************************************************************
//
// ClosePort closes the currently opened serial port.
//
//****************************************************************************
void
ClosePort(long lPort)
{
#ifdef _WIN32
    //
    // Close the serial port.
    //
    CloseHandle(hSerialPort);
#endif
#ifdef __linux__
    char pcLock[32];

    //
    // Close the serial port.
    //
    close(lSerialPort);

    //
    // Create the file name for the lock file.
    //
    sprintf(pcLock, "/var/lock/LCK..ttyUSB%d", lPort - 1);

    //
    // Delete the lock file.
    //
    unlink(pcLock);
#endif
}

//****************************************************************************
//
// SetBaud sets the baud rate and data format of the serial port.
//
//****************************************************************************
void
SetBaud(long lRate)
{
#ifdef _WIN32
    DCB dcb;

    //
    // Purge any pending characters in the serial port.
    //
    PurgeComm(hSerialPort, (PURGE_TXABORT | PURGE_RXABORT |
                            PURGE_TXCLEAR | PURGE_RXCLEAR));

    //
    // Fill in the device control block.
    //
    dcb.DCBlength = sizeof(DCB);
    dcb.BaudRate = lRate;
    dcb.fBinary = TRUE;
    dcb.fParity = FALSE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fDsrSensitivity = FALSE;
    dcb.fTXContinueOnXoff = TRUE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fErrorChar = FALSE;
    dcb.fNull = FALSE;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    dcb.fAbortOnError = FALSE;
    dcb.XonLim = 0;
    dcb.XoffLim = 0;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.XonChar = 17;
    dcb.XoffChar = 19;
    dcb.ErrorChar = 0;
    dcb.EofChar = 0;
    dcb.EvtChar = 0;
    dcb.wReserved = 0;

    //
    // Set the new serial port configuration.
    //
    SetCommState(hSerialPort, &dcb);
#endif
#ifdef __linux__
    struct termios buf;
    int rate;

    //
    // Get the current settings for the serial port.
    //
    if(tcgetattr(lSerialPort, &buf))
    {
        return;
    }

    //
    // Reset to the serial port to raw mode.
    //
    buf.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP |
                     INLCR | IGNCR | ICRNL | IXON);
    buf.c_oflag &= ~OPOST;
    buf.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    buf.c_cflag &= ~(CSIZE | PARENB);
    buf.c_cflag |= CS8;

    //
    // Get the baud rate.
    //
    switch(lRate)
    {
        case 9600:
        {
            rate = B9600;
            break;
        }

        case 19200:
        {
            rate = B19200;
            break;
        }

        case 38400:
        {
            rate = B38400;
            break;
        }

        case 57600:
        {
            rate = B57600;
            break;
        }

        case 115200:
        {
            rate = B115200;
            break;
        }
    }

    //
    // Set the input and output baud rate of the serial port.
    //
    cfsetispeed(&buf, rate);
    cfsetospeed(&buf, rate);

    //
    // Set data bits to 8.
    //
    buf.c_cflag &= ~CSIZE;
    buf.c_cflag |= CS8;

    //
    // Set stop bits to one.
    //
    buf.c_cflag &= ~CSTOPB;

    //
    // Disable parity.
    //
    buf.c_cflag &= ~(PARENB | PARODD);

    //
    // Set the new settings for the serial port.
    //
    if(tcsetattr(lSerialPort, TCSADRAIN, &buf))
    {
      printf("tcsetattr failed\n");
	return;
    }

    
    //
    // Wait until the output buffer is empty.
    //
    tcdrain(lSerialPort);
#endif
}

//****************************************************************************
//
// SendChar sends a character to the serial port.
//
//****************************************************************************
void
SendChar(char cChar)
{
#ifdef _WIN32
    DWORD dwLen;

    //
    // Send this character to the serial port.
    //
    WriteFile(hSerialPort, &cChar, 1, &dwLen, NULL);
#endif
#ifdef __linux__
    //
    // Send this character to the serial port.
    //
    while(write(lSerialPort, &cChar, 1)!=1);

#endif
}

//****************************************************************************
//
// ReceiveChar reads a character from the serial port.
//
//****************************************************************************
char
ReceiveChar(long lTimeout)
{
#ifdef _WIN32
    COMMTIMEOUTS sTimeouts;
    char cChar;
    DWORD dwLen;

    //
    // Fill in the timeout structure based on the timeout requested for this
    // read.
    //
    sTimeouts.ReadIntervalTimeout = 0;
    sTimeouts.ReadTotalTimeoutMultiplier = 0;
    sTimeouts.ReadTotalTimeoutConstant = lTimeout;
    sTimeouts.WriteTotalTimeoutMultiplier = 0;
    sTimeouts.WriteTotalTimeoutConstant = 0;

    //
    // Set the timeout for this read.
    //
    SetCommTimeouts(hSerialPort, &sTimeouts);

    //
    // Read a character.
    //
    if(!ReadFile(hSerialPort, &cChar, 1, &dwLen, NULL))
    {
        //
        // The read failed, so set the read character to a NULL.
        //
        cChar = 0;
    }

    //
    // If we did not read a character, then set the character to NULL.
    //
    if(dwLen != 1)
    {
        cChar = 0;
    }

    //
    // Return the character we read.
    //
    return(cChar);
#endif
#ifdef __linux__
    struct timeb sTime;
    char cChar;
    int res;
    time_t tStart, tNow;

    //
    // Get the current time.
    //
    ftime(&sTime);
    tStart = (sTime.time * 1000) + sTime.millitm;

    //
    // Read the next character from the serial port.
    //
    while(1)
    {
        //
        // Try to read a character from the serial port.
        //
        res = read(lSerialPort, &cChar, 1);
        if(res == 1)
        {
            //
            // We read a character, so break out of the loop.
            //
            break;
        }

        //
        // Get the current time.
        //
        ftime(&sTime);
        tNow = (sTime.time * 1000) + sTime.millitm;

        //
        // See if the timeout has expired.
        //
        if(lTimeout && ((tStart + lTimeout) < tNow))
        {
            cChar = 0;
            break;
        }
    }

    //
    // Return the character read from the serial port.
    //
    return(cChar);
#endif
}

//****************************************************************************
//
// WaitTillEmpty waits until the serial port's output buffer is empty.
//
//****************************************************************************
void
WaitTillEmpty(void)
{
#ifdef _WIN32
    //
    // Wait for 10ms so the output buffer can drain.
    //
    Sleep(10);
#endif
#ifdef __linux__
    //
    // Wait until the output buffer is empty.
    //
    tcdrain(lSerialPort);
#endif
}

//****************************************************************************
//
// WaitFor waits until a specific character is read from the serial port.
//
//****************************************************************************
void
WaitFor(char cWaitChar)
{
    char cChar;

    //
    // Wait until we read a specific character from the serial port.
    //
    while(1)
    {
        //
        // Read a character.
        //
        cChar = ReceiveChar(0);

        //
        // Stop waiting if we received the character.
        //
        if(cChar == cWaitChar)
        {
            break;
        }
    }
}

//****************************************************************************
//
// Prints out a usage message for the program.
//
//****************************************************************************
void
Usage(void)
{
    fprintf(stderr, "\
Usage: download {-b <baud>} {-h} {-n <number>} {-o <offset>} {-p <port>}\n\
                {-s} {-t <type>} {-v} <filename>\n\
\n\
Downloads a program image into the NOR FLASH of a Cirrus Logic EP73xx or\n\
EP93xx board, or into the serial EEPROM of a EP93xx board.  The Intel B3\n\
(sizes 512KB through 4MB), C3 (sizes 1MB through 8MB), and J3(Strata) (sizes\n\
4MB through 16MB) FLASH devices are supported in either 16 or 32 bit wide\n\
configurations.  The Atmel 25F1024 serial EEPROM is supported.\n\
\n\
  -b <baud>     Use the specified baud rate (default is \"115200\").  Valid\n\
                values are 9600, 19200, 38400, 57600, and 115200.\n\
\n\
  -h            Prints this usage message.\n\
\n\
  -n <number>   The board number, which implies the Ethernet MAC address, to\n\
                be programmed into the EEPROM.  This implies -s.\n\
\n\
  -o <offset>   Start programming at the given offset into the FLASH/EEPROM.\n\
\n\
  -p <port>     Use the specified serial port (default is \"1\").  Valid\n");
#ifdef _WIN32
    fprintf(stderr, "\
                values are 1 (COM1), 2 (COM2), 3 (COM3), and 4 (COM4).\n");
#endif
#ifdef __linux__
    fprintf(stderr, "\
                values are 1 (/dev/ttyUSB0), 2 (/dev/ttyUSB1), 3 (/dev/ttyUSB2),\n\
                and 4 (/dev/ttyUSB3).\n");
#endif
    fprintf(stderr, "\
\n\
  -s            Program the EEPROM instead of the NOR FLASH (only valid for\n\
                an EP93xx).\n\
\n\
  -t            Specifies the board type, used in conjunction with -n\n\
                (default is \"9312\").  Valid values are 9301, 9302, 9307,\n\
                9312, or 9315.\n\
\n\
  -v            Prints the version of this program.\n\
\n\
When -n is used to specify the board number, the board type and number are\n\
used together to generate the Ethernet MAC address.  This information is\n\
placed at offset 0x1000 in the EEPROM; the 32 bytes of file data at that\n\
position will be overwritten (or the data will be padded with zeros up to\n\
that offset).  It is valid to not specify a file when the Ethernet MAC\n\
address is being programmed, in which case the first 0x1000 bytes of the\n\
EEPROM will be filled with zeros.\n\
\n");
}

//****************************************************************************
//
// This program waits for the '<' character from the boot ROM, sends the boot
// code, waits for the '>' from the boot ROM, waits for the '?' from the boot
// code, changes the serial port rate (preferably to 115200), downloads the
// user data file, and then prints out progress status as the boot code writes
// the user data file to the NOR FLASH.
//
//****************************************************************************
int
main(int argc, char *argv[])
{
    long lPort = 1, lRate = 115200, lFileSize, lIdx, lLoop, lSum, lOffset = 0;
    long lType = 9312, lNumber;
    char cChar, cFirstChar, cRateChar, cBuffer[1024], *pcString;
    int bError = 0, bEEPROM = 0, bMAC = 0, bHaveFile = 0;
    FILE *pFile;

    //
    // First, set stdout to be unbuffered, so that our status messages are
    // always displayed immediately.
    //
    setbuf(stdout, NULL);

    //
    // Prevent getopt from printing out any errors.
    //
    opterr = 0;

    //
    // See if there are any flags specified on the command line.
    //
    while((cChar = getopt(argc, argv, "b:hn:o:p:st:v")) != -1)
    {
        //
        // Determine which flag was specified.
        //
        switch(cChar)
        {
            //
            // See if this argument is "-b".
            //
            case 'b':
            {
                //
                // Get the baud rate from the command line.
                //
                lRate = atoi(optarg);

                //
                // Make sure that the specified baud rate is valid.
                //
                if((lRate != 9600) && (lRate != 19200) && (lRate != 38400) &&
                   (lRate != 57600) && (lRate != 115200))
                {
                    //
                    // Tell the user that the baud rate is invalid.
                    //
                    fprintf(stderr, "Invalid baud rate '%s'.\n\n", optarg);

                    //
                    // Print the usage message.
                    //
                    Usage();

                    //
                    // We're done.
                    //
                    return(1);
                }

                //
                // We're done handling this argument.
                //
                break;
            }

            //
            // See if this argument is "-h".
            //
            case 'h':
            {
                //
                // Print the usage message.
                //
                Usage();

                //
                // We're done.
                //
                return(1);
            }

            //
            // See if this argument is "-n".
            //
            case 'n':
            {
                //
                // Get the board number from the command line.
                //
                lNumber = atoi(optarg);

                //
                // Make sure that the specified board number is valid.
                //
                if((lNumber < 1) || (lNumber > 65535))
                {
                    //
                    // Tell the user that the board number is invalid.
                    //
                    fprintf(stderr, "Invalid board number '%s'.\n\n", optarg);

                    //
                    // Print the usage message.
                    //
                    Usage();

                    //
                    // We're done.
                    //
                    return(1);
                }

                //
                // Indicate that the EEPROM is to be programmed, and that we
                // have a MAC address.
                //
                bEEPROM = 1;
                bMAC = 1;

                //
                // We're done handling this argument.
                //
                break;
            }

            //
            // See if this argument is "-o".
            //
            case 'o':
            {
                //
                // Get the starting offset.
                //
                lOffset = strtol(optarg, &pcString, 0);

                //
                // If the offset is followed by a "k", then multiply the value
                // by 1K.
                //
                if((*pcString == 'k') || (*pcString == 'K'))
                {
                    lOffset *= 1024;
                }

                //
                // If the offset is followed by a "m", then multiply the value
                // by 1M.
                //
                if((*pcString == 'm') || (*pcString == 'M'))
                {
                    lOffset *= 1048576;
                }

                //
                // We're done handling this argument.
                //
                break;
            }

            //
            // See if this argument is "-p".
            //
            case 'p':
            {
                //
                // Get the port number from the command line.
                //
                lPort = atoi(optarg);

                //
                // Make sure that the specified port number is valid.
                //
                if((lPort != 1) && (lPort != 2) && (lPort != 3) &&
                   (lPort != 4))
                {
                    //
                    // Tell the user that the port number is invalid.
                    //
                    fprintf(stderr, "Invalid serial port '%s'.\n\n", optarg);

                    //
                    // Print the usage message.
                    //
                    Usage();

                    //
                    // We're done.
                    //
                    return(1);
                }

                //
                // We're done handling this argument.
                //
                break;
            }

            //
            // See if this argument is "-s".
            //
            case 's':
            {
                //
                // Indicate that the EEPROM should be programmed.
                //
                bEEPROM = 1;

                //
                // We're done handling this argument.
                //
                break;
            }

            //
            // See if this argument is "-t".
            //
            case 't':
            {
                //
                // Get the board type from the command line.
                //
                lType = atoi(optarg);

                //
                // Make sure that the specified board type is valid.
                //
                if((lType != 9301) && (lType != 9302) && (lType != 9307) &&
                   (lType != 9312) && (lType != 9315))
                {
                    //
                    // Tell the user that the board type is invalid.
                    //
                    fprintf(stderr, "Invalid board type '%s'.\n\n", optarg);

                    //
                    // Print the usage message.
                    //
                    Usage();

                    //
                    // We're done.
                    //
                    return(1);
                }

                //
                // We done handling this argument.
                //
                break;
            }

            //
            // See if this argument is "-v".
            //
            case 'v':
            {
                //
                // Print the version of this program.
                //
                printf("Cirrus Logic FLASH Programmer for ARM Processors, "
                       "Version 3.11 for ");
#ifdef _WIN32
                printf("Win32.\n");
#endif
#ifdef __linux__
                printf("Linux.\n");
#endif
                printf("Copyright (c) 1999,2000,2001,2002,2003,2004 Cirrus "
                       "Logic, Inc.\n\n");
                printf("Report bugs to <epdmkt@cirrus.com>.\n");

                //
                // We're done.
                //
                return(0);
            }

            //
            // An unrecognized flag was specifed.
            //
            default:
            {
                //
                // Print the usage message.
                //
                Usage();

                //
                // We're done.
                //
                return(1);
            }
        }
    }

    //
    // See if a filename was specified.
    //
    bHaveFile = ((optind + 1) == argc);

    //
    // Make sure that a filename was specified or that the MAC address should
    // be programmed.
    //
    if(!bHaveFile && !bMAC)
    {
        //
        // A filename or MAC address was not specified, so print the usage
        // message.
        //
        Usage();

        //
        // We're done.
        //
        return(1);
    }

    //
    // Open the serial port to be used.
    //
    if(OpenPort(lPort) != 1)
    {
        return(1);
    }

    //
    // See if there is a file.
    //
    if(bHaveFile)
    {
        //
        // Open the file to be downloaded.
        //
        pFile = fopen(argv[argc - 1], "rb");
        if(!pFile)
        {
            fprintf(stderr, "Could not open file '%s'.\n", argv[argc - 1]);
            ClosePort(lPort);
            return(1);
        }

        //
        // Get the size of the file.
        //
        fseek(pFile, 0, SEEK_END);
        lFileSize = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
    }
    else
    {
        //
        // Only the MAC address is to be programmed, so set the file size to
        // the size of the board info structure.
        //
        lOffset = 0;
        lFileSize = 0x1020;
    }

    //
    // Round the file size up to the next 1K boundary.
    //
    lFileSize = (lFileSize + 1023) & ~1023;

    //
    // Program the initial baud rate of 9600, 8 data bits, and no parity.
    //
    SetBaud(9600);

    //
    // Tell the user to reset the board.
    //
    printf("Waiting for the board to wakeup...");

    //
    // Wait until we read a '<' from the serial port.
    //
        WaitFor('<');
	//sleep(2);
	//ReceiveChar(0);

    //
    // Tell the user that we are downloading the boot code.
    //
    printf("\r                                  \r");
    printf("Downloading boot code...(  0%%)");

    //
    // Write the boot code to the serial port.
    //
    for(lIdx = 0; lIdx < 2048; lIdx++)
    {
        //
        // Write this character.
        //
        SendChar(pcBoot[lIdx]);
        //
        // Periodically print out our progress.
        //
        if((lIdx & 127) == 127)
        {
            //
            // Wait until the transmit buffer is empty.
            //
            WaitTillEmpty();

            //
            // Print the updated status.
            //
            printf("\b\b\b\b\b%3d%%)", ((lIdx + 1) * 100) / 2048);
        }
    }
    printf("\r                              \r");

    //
    // Wait until we read a '>' from the serial port.
    //
    WaitFor('>');

    //
    // Wait until we read a '?' from the serial port.
    //
    while(1)
    {
        //
        // Read the next character from the serial port.
        //
        cChar = ReceiveChar(0);

        //
        // Quit waiting if this is a '?'.
        //
        if(cChar == '?')
        {
            break;
        }
    }

    //
    // Get the baud rate character for the given baud rate.
    //
    switch(lRate)
    {
        case 9600:
        {
            cRateChar = '0';
            break;
        }

        case 19200:
        {
            cRateChar = '1';
            break;
        }

        case 38400:
        {
            cRateChar = '2';
            break;
        }

        case 57600:
        {
            cRateChar = '3';
            break;
        }

        case 115200:
        {
            cRateChar = '4';
            break;
        }
    }

    //
    // Tell the boot code to switch to the desired baud rate.
    //
    SendChar('B');
    SendChar(cRateChar);

    //
    // Wait until the output buffer is empty.
    //
    WaitTillEmpty();
    usleep(1000);

    //
    // Switch our baud rate to the desired rate.
    //
    SetBaud(lRate);

    //
    // Send a '-' character until we receive back a '?' character.
    //
    while(1)
    {
        //
        // Send a '-' character.
        //
        SendChar('-');

        //
        // Read the character.
        //
        cChar = ReceiveChar(10);

        //
        // Quit waiting if this is a '?'.
        //
        if(cChar == '?')
        {
            break;
        }
    }

    //
    // Empty out the input queue.
    //
    while((cChar = ReceiveChar(10)) != 0)
    {
    }


    //
    // Send the program FLASH command.
    //
    if(bEEPROM)
    {
        SendChar('S');
    }
    else
    {
        SendChar('F');
    }

    //
    // Read the next character from the serial port.
    //
    cChar = ReceiveChar(0);

    //
    // See if this is a 'X'.
    //
    if(cChar == 'X')
    {
        fprintf(stderr, "The board contains an unknown %s.\n",
                bEEPROM ? "EEPROM" : "FLASH");
        ClosePort(lPort);
        return(1);
    }

    //
    // Send the starting offset.
    //
    SendChar((char)(lOffset & 0xFF));
    SendChar((char)((lOffset >> 8) & 0xFF));
    SendChar((char)((lOffset >> 16) & 0xFF));
    SendChar((char)((lOffset >> 24) & 0xFF));

    //
    // Send the length of the data file.
    //
    SendChar((char)(lFileSize & 0xFF));
    SendChar((char)((lFileSize >> 8) & 0xFF));
    SendChar((char)((lFileSize >> 16) & 0xFF));
    SendChar((char)((lFileSize >> 24) & 0xFF));

    //
    // Tell the user that we are erasing the FLASH.
    //
    printf("Erasing the %s...", bEEPROM ? "EEPROM" : "FLASH");

    //
    // Wait until we receive a '!' indicating that the FLASH has been erased.
    //
    while(1)
    {
        //
        // Read the next character from the serial port.
        //
        cChar = ReceiveChar(0);

        //
        // Quit waiting if this is a '!'.
        //
        if(cChar == '!')
        {
            break;
        }

        //
        // See if this is a '&'.
        //
        if(cChar == '&')
        {
            printf("\r                     \r");
            fprintf(stderr, "The image is too large for the %s.\n",
                    bEEPROM ? "EEPROM" : "FLASH");
            ClosePort(lPort);
            return(1);
        }

        //
        // See if this is a '%'.
        //
        if(cChar == '%')
        {
            printf("\r                     \r");
            fprintf(stderr, "The offset '%ld' is not valid for the %s.\n",
                    lOffset, bEEPROM ? "EEPROM" : "FLASH");
            ClosePort(lPort);
            return(1);
        }
    }

    //
    // Tell the user that we are downloading the file data.
    //
    printf("\rProgramming the %s...(  0%%)", bEEPROM ? "EEPROM" : "FLASH");

    //
    // Send the actual data in the file.
    //
    for(lIdx = 0; lIdx < lFileSize; lIdx += 1024)
    {
        //
        // See if the data should come from a file.
        //
        if(bHaveFile)
        {
            //
            // Read the next 1K block from the file.
            //
            lLoop = fread(cBuffer, 1, 1024, pFile);
        }
        else
        {
            //
            // Clear the data buffer.
            //
            memset(cBuffer, 0, 1024);
            lLoop = 1024;
        }

        //
        // See if the Ethernet MAC address is to be programmed, and the correct
        // memory location has been reached.
        //
        if(bMAC && (lIdx == 0x1000))
        {
            //
            // The first four bytes should be "EMAC", the tag to indicate a
            // Ethernet MAC block.
            //
            cBuffer[0] = 'E';
            cBuffer[1] = 'M';
            cBuffer[2] = 'A';
            cBuffer[3] = 'C';

            //
            // The next six bytes are the Ethernet MAC address.
            //
            cBuffer[4] = 0x00;
            cBuffer[5] = 0xdc;
            cBuffer[6] = 0x6c;
            cBuffer[7] = lType - 9300;
            cBuffer[8] = lNumber / 256;
            cBuffer[9] = lNumber & 255;

            //
            // The next six bytes are reserved.
            //
            cBuffer[10] = 0x00;
            cBuffer[11] = 0x00;
            cBuffer[12] = 0x00;
            cBuffer[13] = 0x00;
            cBuffer[14] = 0x00;
            cBuffer[15] = 0x00;

            //
            // The next 16 bytes are the string name for this board.
            //
            cBuffer[16] = 'e';
            cBuffer[17] = 'd';
            cBuffer[18] = 'b';
            cBuffer[19] = '0' + (lType / 1000);
            cBuffer[20] = '0' + ((lType / 100) % 10);
            cBuffer[21] = '0' + ((lType / 10) % 10);
            cBuffer[22] = '0' + (lType % 10);
            cBuffer[23] = '-';
            cBuffer[24] = '0' + (lNumber / 10000);
            cBuffer[25] = '0' + ((lNumber / 1000) % 10);
            cBuffer[26] = '0' + ((lNumber / 100) % 10);
            cBuffer[27] = '0' + ((lNumber / 10) % 10);
            cBuffer[28] = '0' + (lNumber % 10);
            cBuffer[29] = 0x00;
            cBuffer[30] = 0x00;
            cBuffer[31] = 0x00;
        }

        //
        // If we could not read 1K from the file, then fill the remainder of
        // the buffer with zeros.
        //
        for(; lLoop < 1024; lLoop++)
        {
            cBuffer[lLoop] = 0;
        }

        //
        // Send this block of data until it is correctly received and
        // programmed into the FLASH.
        //
        do
        {
            //
            // Send the data for this block.
            //
            for(lLoop = 0; lLoop < 1024; lLoop++)
            {
                SendChar(cBuffer[lLoop]);
            }

            //
            // Compute the checksum for this block.
            //
            for(lLoop = 0, lSum = 0; lLoop < 1024; lLoop++)
            {
                lSum += (long)(unsigned char)cBuffer[lLoop];
            }

            //
            // Send the checksum for this block.
            //
            SendChar((char)(lSum & 0xFF));
            SendChar((char)((lSum >> 8) & 0xFF));
            SendChar((char)((lSum >> 16) & 0xFF));
            SendChar((char)((lSum >> 24) & 0xFF));

            //
            // We now need to wait to see what the target does with this block
            // of data.  Several things could happen:
            //     1) Everything is OK and it sends a '#' character.
            //     2) The block checksum was bad and it sends a '@' character.
            //     3) Some bytes were lost in the transfer and it does nothing.
            // To handle all these cases, we wait for a while to receive a
            // character.  If we never receive a character, we assume that a
            // byte was lost and start sending extra bytes, up to 16 extra
            // bytes.  The extra bytes are always 0xFF, so that the checksum
            // will always fail and the target will request a block resend.  If
            // we send 1028 extra bytes and still do not receive a reply, then
            // assume that the target died and abort the download.
            //
            for(lLoop = 0; lLoop < 16; lLoop++)
            {
                //
                // Read the character from the serial port.
                //
                cChar = ReceiveChar(4000);
                if(cChar != 0)
                {
                    break;
                }

                //
                // Send a 0xFF character.
                //
                SendChar(0xFF);
            }

            //
            // If we could not get a response from the target, then indicate
            // an error and quit trying to send this and further blocks.
            //
            if(lLoop == 16)
            {
                //
                // Indicate that there was an error.
                //
                bError = 1;

                //
                // Stop trying to send this block.
                //
                break;
            }
        }
        while(cChar != '#');

        //
        // If there was an error, then quit sending data.
        //
        if(bError)
        {
            break;
        }

        //
        // Print out our progress.
        //
        printf("\b\b\b\b\b%3d%%)",
               (((lIdx + 1024) / 1024) * 100) / (lFileSize / 1024));
    }
    printf("\r                               \r");

    //
    // If there has not been an error thus far, read the final status from the
    // target.
    //
    if(!bError)
    {
        //
        // Read characters from the serial port until we receive a '*'.
        //
        while(1)
        {
            //
            // Read a character from the serial port.
            //
            cChar = ReceiveChar(0);

            //
            // If the character is a '^', then there was an error programming
            // the FLASH.
            //
            if(cChar == '^')
            {
                bError = 1;
            }

            //
            // If the character is a '*', then we are done.
            //
            if(cChar == '*')
            {
                break;
            }
        }
    }

    //
    // Close the file.
    //
    if(bHaveFile)
    {
        fclose(pFile);
    }

    //
    // Close the serial port.
    //
    ClosePort(lPort);

    //
    // Tell the user we are done.
    //
    if(!bError)
    {
        if(bHaveFile)
        {
            printf("Successfully programmed '%s'.\n", argv[argc - 1]);
        }
        else
        {
            printf("Successfully programmed the Ethernet MAC address.\n");
        }
        return(0);
    }
    else
    {
        if(bHaveFile)
        {
            printf("Failed to program '%s'.\n", argv[argc - 1]);
        }
        else
        {
            printf("Failed to program the Ethernet MAC address.\n");
        }
        return(1);
    }
}
