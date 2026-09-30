#pragma once

#include <cstdint>
#include "Cache.h"

#include <string>

class WriteStrategy
{
private: 
	std::string writeHitPolicy, writeMissPolicy; 

public: 
	WriteStrategy(const std::string& writeHitPolicy, const std::string& writeMissPolicy); 

	void writeThrough(CacheLine* line, const FullyAssociateAddressParts& addressParts, MemoryStatistics& memoryStats); 
	void writeAround(); 

};

