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
// The 0th object is for intrinsic properties -- reserved
// 64K of properties should be plenty for each object
#define PROP_MULTIPLIER                   0x10000
#define PROP_ID(object, index)            ((object+1)*PROP_MULTIPLIER + index)

// string related stuff-- some properties can be returned as null- 
// terminated strings-- commented out for now, food for thought....
//#define PROP_STRING                       0xc000
//#define PROP_IS_STRING(id)                (id & PROP_STRING)
//#define PROP_STRING_ID(object, index)     ((object+1)*PROP_MULTIPLIER + PROP_STRING + index)


class IProperty
{
public:
  virtual int GetProperty(int property, long *value) = 0;
  virtual int SetProperty(int property, long value) = 0;
};

#endif
