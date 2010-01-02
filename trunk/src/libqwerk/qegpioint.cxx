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

#include "qegpioint.h"

SINGLETON_REGISTER(CQEGpioInt);

CQEGpioInt::CQEGpioInt()
{
  m_p9302hw = C9302Hardware::GetPtr();
  if (m_p9302hw->GetBitstreamMajorVersion()!=0xa0)
    throw std::runtime_error("wrong FPGA bitstream version");
  
  m_data = m_p9302hw->m_fpga.Ushort(QEG_DEFAULT_BASE); 
  m_dataDir = m_data + 1;
  m_intMode = m_data + 2;
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

    default:
      return PROP_ERROR_NOT_SUPPORTED;
    }

  return PROP_OK;
}
