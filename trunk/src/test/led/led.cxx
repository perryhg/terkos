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

#include <stdio.h>
#include "qeleds.h"

int main()
{
  CQELEDController &led = CQELEDController::GetRef();

  led.SetLED(LED_ROBOT, LED_COLOR_YELLOW);
  led.SetLED(LED_VEXNET, LED_COLOR_YELLOW);
  led.SetLED(LED_GAME, LED_COLOR_YELLOW);

  CQELEDController::Release();
}
 
  
