#include "PatchManager.h"

PatchManager::PatchManager(E24* storage)
{
	_storage = storage;
}

PatchManager::~PatchManager() {}

void PatchManager::init(int startAddr, uint8_t patchesLength, uint8_t ccLength)
{
	_startAddr = startAddr;
	_patchesLength = patchesLength;
	_ccLength = ccLength;
}

bool PatchManager::load(uint8_t patchNumber, uint8_t& value)
{
	return load(patchNumber, CC_UNDEFINED, value);
}

bool PatchManager::load(uint8_t patchNumber, uint8_t ccNumber, uint8_t& value)
{
	if(!validatePatchNumber(patchNumber, ccNumber)) return false;
	value = _storage->read(getPatchAddress(patchNumber, ccNumber));

	return true;
}

bool PatchManager::save(uint8_t patchNumber, uint8_t value)
{
	return save(patchNumber, CC_UNDEFINED, value);	
}

bool PatchManager::save(uint8_t patchNumber, uint8_t ccNumber, uint8_t value)
{
	if(!validatePatchNumber(patchNumber, ccNumber)) return false;
	_storage->write(getPatchAddress(patchNumber, ccNumber), value);

	return true;
}

bool PatchManager::validatePatchNumber(uint8_t patchNumber, uint8_t ccNumber)
{
	return patchNumber >= 0 && patchNumber < _patchesLength &&
		((ccNumber >= 0 && ccNumber < _ccLength) || ccNumber == CC_UNDEFINED);
}

uint16_t PatchManager::getPatchAddress(uint8_t patchNumber, uint8_t ccNumber)
{
	return _startAddr + patchNumber * (1 + _ccLength) + (ccNumber == CC_UNDEFINED ? 0 : ccNumber + 1);
}