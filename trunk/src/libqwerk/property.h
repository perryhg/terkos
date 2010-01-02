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

#ifndef _PROPERTY_H
#define _PROPERTY_H

// result codes
#define PROP_OK                    0   // returned when set or get is successful
#define PROP_ERROR                 -1
#define PROP_ERROR_NOT_SUPPORTED   -2  // property id not supported
#define PROP_ERROR_READ_ONLY       -3  // property is read only
#define PROP_ERROR_ILLEGAL_VALUE   -4  // property value is out 
                                       //   of range or illegal

// Globally unique property ID's are useful because we can merge all
// property interfaces into one.....
// The 0th object is reserved
// 64K of properties should be plenty for each object
#define PROP_MULTIPLIER                   0x10000
#define PROP_ID(object, index)            ((object+1)*PROP_MULTIPLIER + index)

// string related stuff-- some properties can be returned as null- 
// terminated strings-- commented out for now, food for thought....
//#define PROP_STRING                       0xc000
//#define PROP_IS_STRING(id)                (id & PROP_STRING)
//#define PROP_STRING_ID(object, index)     ((object+1)*PROP_MULTIPLIER + PROP_STRING + index)

/**
 * IProperty is an interface class for setting and getting properties of a
 * derived class. 
 */
class IProperty
{
public:
  /**
   * Get the property value.
   * @param property the numerical identifier of the property
   * @param value the value of the property is written into this long 
   * pointer
   * @return  
   * - PROP_OK=success (0)
   * - PROP_ERROR=general error (-1)    
   * - PROP_ERROR_NOT_SUPPORTED=property doesn't exist or isn't supported (-2)
   */
  virtual int GetProperty(int property, long *value) = 0;

  /**
   * Set the property value.
   * @param property the numerical identifier of the property
   * @param value the value of the property 
   * @return  
   * - PROP_OK=success (0)
   * - PROP_ERROR=general error (-1)    
   * - PROP_ERROR_NOT_SUPPORTED=property doesn't exist or isn't supported (-2)
   * - PROP_ERROR_READ_ONLY=property can only be read (-3)
   * - PROP_ERROR_ILLEGAL_VALUE=the value is not permitted (-4)
   */
  virtual int SetProperty(int property, long value) = 0;
};

#endif
