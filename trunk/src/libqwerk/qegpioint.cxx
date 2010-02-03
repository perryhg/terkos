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

#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "../module/qeinterrupt/qeinterrupt.h"
#include "qegpioint.h"


SINGLETON_REGISTER(CQEGpioInt);

int CQEGpioInt::m_fd[QEG_NUM_IO];
void (*CQEGpioInt::m_callback[QEG_NUM_IO])(unsigned int, struct timeval *);

CQEGpioInt::CQEGpioInt()
{
  int i;

  m_p9302hw = C9302Hardware::GetPtr();
  if (m_p9302hw->GetBitstreamMajorVersion()!=0xa0)
    throw std::runtime_error("wrong FPGA bitstream version");
  
  m_data = m_p9302hw->m_fpga.Ushort(QEG_DEFAULT_BASE); 
  m_dataDir = m_data + 1;
  m_intMode = m_data + 2;

  for (i=0; i<QEG_NUM_IO; i++)
    {
      m_fd[i] = -1;
      m_callback[i] = NULL;
    }

  // register callback
  signal(SIGIO, SigHandler);  
}
  
CQEGpioInt::~CQEGpioInt()
{
  C9302Hardware::Release();
}

int CQEGpioInt::GetProperty(int property, long *value)
{
  if (value==NULL)
    return PROP_ERROR;

  switch(property)
    {
      /**
       * - QEG_PROP_WIDTH=number of digital I/O signals 
       */
    case QEG_PROP_WIDTH:
      *value = QEG_NUM_IO;
      break;

      /**
       * - QEG_PROP_DATA_REG=returns the state of 
       * the external digital I/O signals as a bitmap with bit 0 (LSB)
       * corresponding to digital signal 1 and bit 15 corresponding 
       * to digital signal 16 -- typically used to read signals
       * configured as inputs, although the state of output signals is also
       * returned.
       */
    case QEG_PROP_DATA_REG:
      *value = *m_data;
      break;

      /**
       * - QEG_PROP_DATA_DIR_REG=returns the contents of the data
       * direction register -- only the least significant 16 bits are used.
       */
    case QEG_PROP_DATA_DIR_REG:
      *value = *m_dataDir;
      break;

      /**
       * - QEG_PROP_INTERRUPT_MODE=returns the contents of the interrupt
       * mode register -- only the least significant 16 bits are used.
       */
    case QEG_PROP_INTERRUPT_MODE:
      *value = *m_intMode;
      break;

    default:
      return PROP_ERROR_NOT_SUPPORTED;
    }

  return PROP_OK;
}

int CQEGpioInt::SetProperty(int property, long value)
{
  switch (property)
    {
      /**
       * - QEG_PROP_DATA_REG=changes the state of 
       * the external digital I/O signals as a bitmap with bit 0 (LSB)
       * corresponding to digital signal 1 and bit 15 corresponding 
       * to digital signal 16 -- typically used to write signals
       * configured as outputs.
       */
    case QEG_PROP_DATA_REG:
      *m_data = value;
      break;

      /**
       * - QEG_PROP_DATA_DIR_REG=sets the data direction of the corresponding
       * data signal.  A 0 (zero) configures the corresponding data signal 
       * to be input.  A 1 (one) configures the corresponding data signal 
       * to be an output.
       */
    case QEG_PROP_DATA_DIR_REG:
      *m_dataDir = value;
      break;

      /**
       * - QEG_PROP_INTERRUPT_MODE=sets the interrupt mode of the corresponding
       * data signal.  A 0 (zero) configures the corresponding data signal to
       * be positive edge triggered.  A 1 (one) configures the corresponding
       * data signal to be negative edge triggered.
       */ 
    case QEG_PROP_INTERRUPT_MODE:
      *m_intMode = value;
      break;

    default:
      return PROP_ERROR_NOT_SUPPORTED;
    }

  return PROP_OK;
}

int CQEGpioInt::RegisterCallback(unsigned int io, void (*callback)(unsigned int, struct timeval *))
{
  int oflags;
  char devbuf[32];
  const char *dev = "/dev/qeint";

  if (io>=QEG_NUM_IO)
    return -1;

  // open interrupt device
  sprintf(devbuf, "%s%d", dev, io+16);
  m_fd[io] = open(devbuf, O_RDWR); 
  if (m_fd[io]<0)
    return -1;

  if (fcntl(m_fd[io], F_SETOWN, getpid())<0)
    return -1;
  oflags = fcntl(m_fd[io], F_GETFL);
  fcntl(m_fd[io], F_SETFL, oflags | FASYNC);
  
  // clear status
  ioctl(m_fd[io], QEINT_IOC_RESET_STATUS); 

  // assuming the following assignment is atomic...
  m_callback[io] = callback;

  return 0;
}

int CQEGpioInt::UnregisterCallback(unsigned int io)
{
  if (io>=QEG_NUM_IO)
    return -1;

  // assuming the following assignment is atomic...
  m_callback[io] = NULL;
  // close device
  close(m_fd[io]);
  m_fd[io] = -1;

  return 0;
}

void CQEGpioInt::SigHandler(int signum)
{
  int i;
  char status;
  struct timeval tv;

  for (i=0; i<QEG_NUM_IO; i++)
    {
      if (m_callback[i]!=NULL && m_fd[i]!=-1)
	{
	  // check to see if interrupt has occurred on this I/O 
	  ioctl(m_fd[i], QEINT_IOC_READ_STATUS, &status);
	  if (status)
	    {
	      // clear status
	      ioctl(m_fd[i],  QEINT_IOC_RESET_STATUS);
	      // read time
	      read(m_fd[i], (void *)&tv, sizeof(tv));
	      // callback 
	      (*m_callback[i])(i, &tv);
	    }
	}
    }
}

