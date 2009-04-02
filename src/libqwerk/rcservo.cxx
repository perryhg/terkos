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

#include <stdio.h>
#include "rcservo.h"

CRCServo::CRCServo(unsigned char num, unsigned long addr, bool enable) :
  //m_map(addr, 0x100)
  m_map(0x20000000, 0x1000)
{
  ::std::vector<RCServoConfig> rcServoConfigVector;
  Init(rcServoConfigVector, num, addr, enable);
}

CRCServo::CRCServo(::std::vector<RCServoConfig> servoConfigs, unsigned char num, unsigned long addr, bool enable)
:
  //m_map(addr, 0x100)
  m_map(0x20000000, 0x1000)
{
  Init(servoConfigs, num, addr, enable);
}

void CRCServo::Init(::std::vector<RCServoConfig> servoConfigs, unsigned char num, unsigned long addr, bool enable)
{
  unsigned int i;

  m_num = num;
  m_addr = (volatile unsigned short *)m_map.GetMap()+(addr>>1);
  m_boundLower = new unsigned char[m_num];
  m_boundUpper = new unsigned char[m_num];
  m_shadowPos = new unsigned char[m_num];
  for (i=0; i<num; i++)
    {
      unsigned char minBound = 0x00;
      unsigned char maxBound = 0xff;
      unsigned char initialPosition = 0x80;

      if (servoConfigs.size() >= i+1)
      {
        minBound = servoConfigs[i].minBound;
        maxBound = servoConfigs[i].maxBound;
        initialPosition = servoConfigs[i].initialPosition;
      }
      m_boundLower[i] = minBound;
      m_boundUpper[i] = maxBound;
      SetPosition(i, initialPosition);
      printf("CRCServo::Init(): DEBUG: Initialized servo %2d with bounds [%3d,%3d] and initial position %3d\n",i,minBound,maxBound,initialPosition);
    }
  if (enable)
    Enable();
}

CRCServo::~CRCServo()
{
  Disable();
  delete [] m_boundLower;
  delete [] m_boundUpper;
  delete [] m_shadowPos;
}

void CRCServo::Enable()
{
  m_addr[0x1f] = 1;
}

void CRCServo::Disable()
{
  m_addr[0x1f] = 0;
}

unsigned char CRCServo::GetPosition(unsigned char index)
{
  if (index>=m_num)
    return 0;

  return m_shadowPos[index];
}

void CRCServo::SetPosition(unsigned char index, unsigned char pos)
{
  unsigned char scaledPos;

  if (index>=m_num)
    return;

  m_shadowPos[index] = pos;

  // scale position such that it lies between bounds, but maintain full input range [0..255]
  scaledPos = (unsigned char)(((unsigned short)(m_boundUpper[index]-m_boundLower[index])*(unsigned short)pos)>>8) + m_boundLower[index];
  if (index&1)
    {
      index >>= 1;
      m_addr[index] = scaledPos<<8 | m_addr[index]&0x00ff;
    }
  else
    {
      index >>= 1;
      m_addr[index] = scaledPos | m_addr[index]&0xff00;
    }
}

void CRCServo::SetBounds(unsigned char index, unsigned char lower, unsigned char upper)
{
  if (index<m_num && lower<upper)
    {
      m_boundUpper[index] = upper;
      m_boundLower[index] = lower;
    }
}
