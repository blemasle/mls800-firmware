#ifndef _PatchManager_h
#define _PatchManager_h

#include <E24LC256.h>

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#define PATCH_DEFAULT_VALUE 0
#define PATCH_CC_DEFAULT_VALUE 0
#define CC_UNDEFINED 255

class PatchManager {
private:
	E24LC256* _storage;
	byte _startAddr;
	byte _patchesLength;
	byte _ccLength;

	bool validatePatchNumber(byte patchNumber, byte ccNumber);
	unsigned short getPatchAddress(byte patchNumber, byte ccNumber);
public:
	PatchManager(E24LC256* storage);
	~PatchManager();
	void init(int startAddr, byte patchesLength, byte ccLength);
	
	bool load(byte patchNumber, byte& value);
	bool load(byte patchNumber, byte ccNumber, byte& value);
	
	bool save(byte patchNumber, byte value);
	bool save(byte patchNumber, byte ccNumber, byte value);
};

#endif