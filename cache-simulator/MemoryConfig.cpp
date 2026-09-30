#include "MemoryConfig.h"

MemoryConfig* MemoryConfig::memoryConfig = nullptr;
std::mutex MemoryConfig::mtx;

MemoryConfig* MemoryConfig::getMemoryConfig()
{
	if (!memoryConfig) {
		std::lock_guard<std::mutex> lock(mtx);
		if (!memoryConfig) {
			memoryConfig = new MemoryConfig();
		}
	}
	return memoryConfig;
}

void MemoryConfig::setMemoryConfig(uint8_t lineSize, uint32_t lineSetSize,
	uint8_t addressSize, float_t cacheTime, float_t memTime,
	const std::string& readStrat,
	const std::string& writeHitStrat,
	const std::string& writeMissStrat)
{
	cacheLineSize = lineSize;
	cacheLineSetSize = lineSetSize;
	memoryAddressSize = addressSize;
	cacheAccessTime = cacheTime;
	memoryAccessTime = memTime;
	readStrategy = readStrat;
	writeHitStrategy = writeHitStrat;
	writeMissStrategy = writeMissStrat;
}

uint8_t MemoryConfig::getCacheLineSize() const { return cacheLineSize; }
uint32_t MemoryConfig::getCacheLineSetSize() const { return cacheLineSetSize; }
uint8_t MemoryConfig::getMemoryAddressSize() const { return memoryAddressSize; }
float_t MemoryConfig::getCacheAccessTime() const { return cacheAccessTime; }
float_t MemoryConfig::getMemoryAccessTime() const { return memoryAccessTime; }
const std::string& MemoryConfig::getReadStrategy() const { return readStrategy; }
const std::string& MemoryConfig::getWriteHitStrategy() const { return writeHitStrategy; }
const std::string& MemoryConfig::getWriteMissStrategy() const { return writeMissStrategy; }
