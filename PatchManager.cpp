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

}

byte PatchManager::load(byte patchNumber, byte ccNumber)
{

}

byte PatchManager::save(byte patchNumber, byte value)
{

}

byte PatchManager::save(byte patchNumber, byte ccNumber, byte value)
{

}