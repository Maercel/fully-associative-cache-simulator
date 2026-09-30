#include "MemorySystem.h"

MemorySystem::MemorySystem()
{
	cache.initialize(&mainMemory); 
}

uint32_t MemorySystem::read(const uint32_t address)
{
	return cache.read(address); 
}

void MemorySystem::write(uint32_t address, uint8_t data)
{
	cache.write(address, data); 
}

void MemorySystem::printMainMemory() const
{
	mainMemory.print(); 
}

MemoryStatistics MemorySystem::getMemoryStatistics() const
{
	return cache.getMemoryStatistics(); 
}

void MemorySystem::resetCacheStatistics()
{
	return cache.resetStatistics(); 
}
