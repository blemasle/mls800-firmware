#include "PatchManager.h"

PatchManager::PatchManager(E24LC256* storage)
{
	_storage = storage;
}

PatchManager::~PatchManager() {}

void PatchManager::init(int startAddr, byte patchesLength, byte ccLength)
{
	_startAddr = startAddr;
	_patchesLength = patchesLength;
	_ccLength = ccLength;
}

bool PatchManager::load(byte patchNumber, byte& value)
{
	return load(patchNumber, CC_UNDEFINED, value);
}

bool PatchManager::load(byte patchNumber, byte ccNumber, byte& value)
{
	if(!validatePatchNumber(patchNumber, ccNumber)) return false;
	value = _storage->read(getPatchAddress(patchNumber, ccNumber));

	return true;
}

bool PatchManager::save(byte patchNumber, byte value)
{
	return save(patchNumber, CC_UNDEFINED, value);	
}

bool PatchManager::save(byte patchNumber, byte ccNumber, byte value)
{
	if(!validatePatchNumber(patchNumber, ccNumber)) return false;
	_storage->write(getPatchAddress(patchNumber, ccNumber), value);

	return true;
}

bool PatchManager::validatePatchNumber(byte patchNumber, byte ccNumber)
{
	return patchNumber >= 0 && patchNumber < _patchesLength &&
		((ccNumber >= 0 && ccNumber < _ccLength) || ccNumber == CC_UNDEFINED);
}

unsigned short PatchManager::getPatchAddress(byte patchNumber, byte ccNumber)
{
	return _startAddr + patchNumber * (1 + _ccLength) + (ccNumber == CC_UNDEFINED ? 0 : ccNumber + 1);
}