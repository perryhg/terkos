#include "qegpioint.h"

SINGLETON_REGISTER(CQEGpioInt);

CQEGpioInt::CQEGpioInt()
{
  m_p9302hw = C9302Hardware::GetObject();
  m_base = QEG_DEFAULT_BASE;
  
}
  
CQEGpioInt::~CQEGpioInt()
{
  C9302Hardware::ReleaseObject();
}
