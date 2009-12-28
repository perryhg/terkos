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
#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdexcept>
#include "singleton.h"

// put this in your class declaration
#define SINGLETON(T)\
  friend class TSingleton<T>;\
  static TSingleton<T> m_singleton;\
  static T *GetPtr()\
  {\
    return m_singleton.GetPtr();\
  }\
  static T &GetRef()\
  {\
    return m_singleton.GetRef();\
  }\
  static void Release()\
  {\
    m_singleton.Release();\
  }

#define SINGLETON_REGISTER(T)\
  TSingleton<T> T::m_singleton( #T )

  
template <class T>
class TSingleton
{
public:
  static T *GetPtr();
  static T &GetRef();
  static void Release();

  TSingleton();
  TSingleton(const char *identifier);
  ~TSingleton();
 
private:
  static void Cleanup();

  static int m_refCount;
  static T *m_pInstance;  
  static const char *m_identifier;
  static int m_fd;
};

template <class T> int TSingleton<T>::m_refCount = 0;
template <class T> T *TSingleton<T>::m_pInstance = NULL;
template <class T> const char *TSingleton<T>::m_identifier = NULL;
template <class T> int TSingleton<T>::m_fd = -1;

template <class T> T *TSingleton<T>::GetPtr()
{
  if (m_pInstance==NULL)
    {
      if (m_identifier!=NULL)
	{
	  char fn[64];
	  strcpy(fn, "/tmp/");
	  strcat(fn, m_identifier);
	  m_fd = open(fn, O_RDWR | O_CREAT);
	  if (m_fd<0)
	    {
	      fprintf(stderr, "ERROR: cannot create lockfile %s.\n", fn);
	      return NULL;
	    }
	  if (flock(m_fd, LOCK_EX | LOCK_NB)<0)
	    {
	      fprintf(stderr, "ERROR: %s object already exists in another process.\n", m_identifier);
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
  if (m_fd>=0)
    {
      flock(m_fd, LOCK_UN | LOCK_NB);
      close(m_fd);
      m_fd = -1;
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

