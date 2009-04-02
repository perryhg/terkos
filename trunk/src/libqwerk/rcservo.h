//  BEGIN LICENSE BLOCK
//
//  Version: MPL 1.1
//
//  The contents of this file are subject to the Mozilla Public License Version
//  1.1 (the "License"); you may not use this file except in compliance with
//  the License. You may obtain a copy of the License at
//  http://www.mozilla.org/MPL/
//
//  Software distributed under the License is distributed on an "AS IS" basis,
//  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
//  for the specific language governing rights and limitations under the
//  License.
//
//  The Original Code is The Xport Software Distribution.
//
//  The Initial Developer of the Original Code is Charmed Labs LLC.
//  Portions created by the Initial Developer are Copyright (C) 2003
//  the Initial Developer. All Rights Reserved.
//
//  Contributor(s): Rich LeGrand (rich@charmedlabs.com)
//
//  END LICENSE BLOCK

#ifndef _RCSERVO_H
#define _RCSERVO_H

#include <vector>
#include "memmap.h"

#define QES_DEFAULT_SERVOS       16
#define QES_DEFAULT_ADDR         0x400

struct RCServoConfig
   {
   int minBound;
   int maxBound;
   int initialPosition;
   };

class CRCServo
{
public:
  CRCServo(unsigned char num=QES_DEFAULT_SERVOS, unsigned long addr=QES_DEFAULT_ADDR, bool enable=true);
  CRCServo(::std::vector<RCServoConfig> servoConfigs, unsigned char num=QES_DEFAULT_SERVOS, unsigned long addr=QES_DEFAULT_ADDR, bool enable=true);
  ~CRCServo();

  void Disable();	// turn off all servos
  void Enable();  // turn on all servos
  unsigned char GetPosition(unsigned char index);
  void SetPosition(unsigned char index, unsigned char pos);
  void SetBounds(unsigned char index, unsigned char lower, unsigned char upper);

private:
  unsigned char *m_boundUpper;
  unsigned char *m_boundLower;
  volatile unsigned short *m_addr;
  unsigned char *m_shadowPos;  // use shadow registers to save read-back logic
  unsigned char m_num;

  CMemMap m_map;

  void Init(::std::vector<RCServoConfig> servoConfigs, unsigned char num, unsigned long addr, bool enable);

};

#endif
