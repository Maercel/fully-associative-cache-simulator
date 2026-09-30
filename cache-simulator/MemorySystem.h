#pragma once

#include "MainMemory.h"
#include "Cache.h"

class MemorySystem
{
private: 
	MainMemory mainMemory; 
	Cache cache; 
public: 
	MemorySystem(); 

	uint32_t read(const uint32_t address); 
	void write(uint32_t address, uint8_t data); 
	void printMainMemory() const; 

	MemoryStatistics getMemoryStatistics() const; 
	void resetCacheStatistics(); 
};

