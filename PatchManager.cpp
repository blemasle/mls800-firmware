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

byte PatchManager::load(byte patchNumber)
{
	return load(patchNumber, 0);
}

byte PatchManager::load(byte patchNumber, byte ccNumber)
{
	if(!validatePatchNumber(patchNumber, ccNumber)) return -1;
	return _storage->read(getPatchAddress(patchNumber, 0));
}

byte PatchManager::save(byte patchNumber, byte value)
{
	return save(patchNumber, 0, value);	
}

byte PatchManager::save(byte patchNumber, byte ccNumber, byte value)
{
	if(!validatePatchNumber(patchNumber, ccNumber)) return -1;
	_storage->write(getPatchAddress(patchNumber, ccNumber), value);
}

bool PatchManager::validatePatchNumber(byte patchNumber, byte ccNumber)
{
	return patchNumber >= 0 && patchNumber < _patchesLength &&
		ccNumber >= 0 && ccNumber < _ccLength;
}

unsigned short PatchManager::getPatchAddress(byte patchNumber, byte ccNumber)
{
	return patchNumber * (1 + _ccLength) + ccNumber;
}