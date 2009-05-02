#ifndef FILTER_H_
#define FILTER_H_

#include <DynamicObject.h>

class Filter : public DynamicObject
   {
   public:
      Filter(void(*delObj)(void*));
      virtual ~Filter(void);
      virtual void processAudio(int size, unsigned char* buffer) = 0;
   };

#endif /* FILTER_H_ */
