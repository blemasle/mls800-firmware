#pragma once

#include <Arduino.h>
#include <E24.h>

#define PATCH_DEFAULT_VALUE 0
#define PATCH_CC_DEFAULT_VALUE 0
#define CC_UNDEFINED 255

class PatchManager {
private:
	E24* _storage;
	uint8_t _startAddr;
	uint8_t _patchesLength;
	uint8_t _ccLength;

	bool validatePatchNumber(uint8_t patchNumber, uint8_t ccNumber);
	uint16_t getPatchAddress(uint8_t patchNumber, uint8_t ccNumber);
public:
	PatchManager(E24* storage);
	~PatchManager();
	void init(int startAddr, uint8_t patchesLength, uint8_t ccLength);
	
	bool load(uint8_t patchNumber, uint8_t& value);
	bool load(uint8_t patchNumber, uint8_t ccNumber, uint8_t& value);
	
	bool save(uint8_t patchNumber, uint8_t value);
	bool save(uint8_t patchNumber, uint8_t ccNumber, uint8_t value);
};