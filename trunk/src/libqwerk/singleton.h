//
// begin license header
//  
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the 
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).  
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at 
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//
// end license header
//

#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdexcept>
#include "singleton.h"

#define SINGLETON_TIMEOUT 1000000000

// put this in your class declaration
#define SINGLETON(T)				\
  friend class TSingleton<T>;			\
  static TSingleton<T> m_singleton;		\
  static T *GetPtr(bool suppressError=false)    \
  {						\
    return m_singleton.GetPtr(suppressError);	\
  }						\
  static T &GetRef()				\
  {						\
    return m_singleton.GetRef();		\
  }						\
  static void Release()				\
  {						\
    m_singleton.Release();			\
  }                                             \
  static bool Requested()                       \
  {                                             \
    return m_singleton.Requested();             \
  }

#define SINGLETON_REGISTER(T)			\
  TSingleton<T> T::m_singleton( #T )

  
template <class T>
class TSingleton
{
public:
  static T *GetPtr(bool suppressError=false);
  static T &GetRef();
  static bool Requested();
  static void Release();

  TSingleton();
  TSingleton(const char *identifier);
  ~TSingleton();
 
private:
  static void Cleanup();
  static unsigned int Hash(const char *str);

  static int m_refCount;
  static T *m_pInstance;  
  static const char *m_identifier;
  static int m_id;
  static unsigned int m_hash;
};

template <class T> int TSingleton<T>::m_refCount = 0;
template <class T> T *TSingleton<T>::m_pInstance = NULL;
template <class T> const char *TSingleton<T>::m_identifier = NULL;
template <class T> int TSingleton<T>::m_id = -1;
template <class T> unsigned int TSingleton<T>::m_hash = 0;

template <class T> T *TSingleton<T>::GetPtr(bool suppressError)
{
  if (m_pInstance==NULL)
    {
      if (m_identifier!=NULL)
	{
	  int ret;
	  struct sembuf ops;
	  struct timespec ts;
	  union semun 
	  {
	    int              val;    
	    struct semid_ds *buf;    
	    unsigned short  *array;  
	    struct seminfo  *__buf;  
	  } sn;

	  // hash it
	  m_hash = Hash(m_identifier);
	  
	  // does it exist already?
	  m_id = semget(m_hash, 1, 0666);
	  if (m_id<0)
	    { // create it...
	      m_id = semget(m_hash, 1, IPC_CREAT | 0666);
	      if (m_id<0)
		{
		  if (!suppressError)
		    fprintf(stderr, "ERROR: cannot create semaphore.\n");
		  return NULL;
		}

	      // ...and initialize it...
	      sn.val = 1;
	      semctl(m_id, 0, SETVAL, sn);   
	    }
	  
	  // grab it, wait if necessary
	  ops.sem_num = 0;
	  ops.sem_op = -1;
	  ops.sem_flg = SEM_UNDO;
	  
	  ts.tv_sec = SINGLETON_TIMEOUT/1000000000;
	  ts.tv_nsec = SINGLETON_TIMEOUT%1000000000; 
	  ret = semtimedop(m_id, &ops, 1, &ts);
	  
	  if (ret<0)
	    {
	      fprintf(stderr, "ERROR: %s object already exists in another process.\n", m_identifier);
	      m_id = -1;
	      return NULL;
	    }
	}
      m_pInstance = new T();
    }
  m_refCount++;  
  
  return m_pInstance;
}

template <class T> T &TSingleton<T>::GetRef()
{
  T *p;
 
  p = GetPtr();

  if (p==NULL)
    throw std::runtime_error("cannot create object");
  else 
    return *p;
}

template <class T> unsigned int TSingleton<T>::Hash(const char *str)
{
  unsigned int hash = 5381;
  int c;

  while ((c=*str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

template <class T> bool TSingleton<T>::Requested()
{
  // get number of processes waiting
  if (semctl(m_id, 0, GETNCNT)>0)
    return true;
  else
    return false;
}

template <class T> void TSingleton<T>::Release()
{
  if (m_refCount)
    m_refCount--;
  
  if (m_refCount==0)
    Cleanup();
}

template <class T> TSingleton<T>::TSingleton()
{
}

template <class T> void TSingleton<T>::Cleanup()
{
  if (m_id>=0)
    {
      struct sembuf ops;
      // release (increment) semaphore
      ops.sem_num = 0;
      ops.sem_op = 1;
      ops.sem_flg = SEM_UNDO;
      
      semop(m_id, &ops, 1);
      m_id = -1;
    }
  if (m_pInstance)
    {
      delete m_pInstance;
      m_pInstance = NULL;
    }
}

template <class T> TSingleton<T>::TSingleton(const char *identifier)
{
  m_identifier = identifier;
}

template <class T> TSingleton<T>::~TSingleton()
{
  Cleanup();
}


#endif

