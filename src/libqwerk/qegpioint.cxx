#include "qegpioint.h"

SINGLETON_REGISTER(CQEGpioInt);

CQEGpioInt::CQEGpioInt()
{
  m_p9302hw = C9302Hardware::GetPtr();
  if (m_p9302hw->GetBitstreamMajorVersion()!=0xa0)
    throw std::runtime_error("wrong FPGA bitstream version");
  m_base = QEG_DEFAULT_BASE;
  
}
  
CQEGpioInt::~CQEGpioInt()
{
  C9302Hardware::Release();
}
