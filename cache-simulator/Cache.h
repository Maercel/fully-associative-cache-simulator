#pragma once

#include "MainMemory.h"
#include "ReplacementAlgorithms.h"
#include "MemoryConfig.h"

#include <vector>
#include <cstdint>
#include <cmath>


struct CacheLine {
	uint32_t tag = 0;
	std::vector<uint8_t> data; 
	bool valid = false; 
	bool dirty = false; 

	CacheLine();
};

struct FullyAssociateAddressParts {
	int32_t tag;
	uint8_t byteOffSet;

	FullyAssociateAddressParts(const uint32_t address);
};


class CacheSet {
private:
	std::vector<CacheLine> lines;
	ReplacementAlgorithms replacement; // stores for each specific set
public: 
	CacheSet(); 

	CacheLine* find(const uint32_t tag); 
	CacheLine* replace(uint32_t lineStart, uint32_t tag, uint8_t* sourceData, MainMemory* mainMemory);
};

struct CacheStatistics {
	uint32_t cacheAccesses = 0;
	uint32_t cacheHits = 0;

	float_t getHitRate() const;
};

struct AccessTimeStatistics {
	float_t cacheAccessTime = 0;
	float_t memoryAccessTime = 0;
	float_t totalAccessTime = 0;

	AccessTimeStatistics();
};

struct MemoryStatistics {
	CacheStatistics cacheStats;
	AccessTimeStatistics accessTimeStats; 
};

class Cache {
private:
	MainMemory* mainMemory;
	CacheSet set; 
	MemoryStatistics memoryStats;
public: 
	Cache() = default; 

	void initialize(MainMemory* memory); 
	uint32_t read(const uint32_t address); 
	void write(const uint32_t address, uint8_t data);

	MemoryStatistics getMemoryStatistics() const; 
	void resetStatistics(); 

};
