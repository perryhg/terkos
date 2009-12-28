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
    case QEG_PROP_WIDTH:
      *value = QEG_NUM_IO;
      break;

    case QEG_PROP_DATA_REG:
      *value = *m_data;
      break;

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
    case QEG_PROP_DATA_REG:
      *m_data = value;
      break;

    case QEG_PROP_DATA_DIR_REG:
      *m_dataDir = value;
      break;

    default:
      return PROP_ERROR_NOT_SUPPORTED;
    }

  return PROP_OK;
}
