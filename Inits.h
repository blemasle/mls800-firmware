// Inits.h

#ifndef _INITS_h
#define _INITS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class InitsClass
{
 protected:


 public:
	void init();
};

extern InitsClass Inits;

#endif

