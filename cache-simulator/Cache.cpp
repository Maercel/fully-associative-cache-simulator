#include "Cache.h"

#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstring>

CacheLine::CacheLine() {
	data.resize(MemoryConfig::getMemoryConfig()->getCacheLineSize());
}

FullyAssociateAddressParts::FullyAssociateAddressParts(const uint32_t address)
{
	byteOffSet = address & (MemoryConfig::getMemoryConfig()->getCacheLineSize() - 1);
	tag = address / MemoryConfig::getMemoryConfig()->getCacheLineSize();
}

float_t CacheStatistics::getHitRate() const
{
	return static_cast<float_t>(cacheHits) / cacheAccesses;
}

AccessTimeStatistics::AccessTimeStatistics()
{
	cacheAccessTime = MemoryConfig::getMemoryConfig()->getCacheAccessTime();
	memoryAccessTime = MemoryConfig::getMemoryConfig()->getMemoryAccessTime();
}

CacheSet::CacheSet()
{
	lines.resize(MemoryConfig::getMemoryConfig()->getCacheLineSetSize());
	replacement.initialize(MemoryConfig::getMemoryConfig()->getReadStrategy(),
		MemoryConfig::getMemoryConfig()->getCacheLineSetSize());
}

CacheLine* CacheSet::find(const uint32_t tag) 
{
	for (uint32_t line = 0; line < MemoryConfig::getMemoryConfig()->getCacheLineSetSize(); ++line) {
		if (lines.at(line).valid && lines.at(line).tag == tag) {
			replacement.updateLRUList(line); 
			return &lines.at(line);
		}
	}
	return nullptr;
}

CacheLine* CacheSet::replace(uint32_t lineStart, uint32_t tag, uint8_t* sourceData, MainMemory* mainMemory)
{
	uint8_t victim = replacement.getVictim(); 

	
	if (lines.at(victim).valid && lines.at(victim).dirty) {
		mainMemory->write(lines.at(victim).tag * MemoryConfig::getMemoryConfig()->getCacheLineSize(),
			MemoryConfig::getMemoryConfig()->getCacheLineSize(), &lines.at(victim).data[0]);
		lines.at(victim).dirty = false; 
	}
	
	lines.at(victim).valid = true;
	lines.at(victim).tag = tag;
	lines.at(victim).data.resize(MemoryConfig::getMemoryConfig()->getCacheLineSize());

	replacement.updateLRUList(victim);

	std::memcpy(&lines.at(victim).data[0], sourceData,
		MemoryConfig::getMemoryConfig()->getCacheLineSize());


	return &lines.at(victim); 
}

void Cache::initialize(MainMemory* memory)
{
	mainMemory = memory; 
}

uint32_t Cache::read(const uint32_t address)
{
	FullyAssociateAddressParts addressParts(address); 

	CacheLine* line = set.find(addressParts.tag); 

	memoryStats.cacheStats.cacheAccesses += 1;

	// Hit
	if (line) {
		std::stringstream ss;
		ss << "Reading from cache (tag: " << addressParts.tag << " "
			<< "address: 0x" << std::hex << std::uppercase << address << ")\n";
		std::cout << ss.str();

		memoryStats.cacheStats.cacheHits += 1; 
		memoryStats.accessTimeStats.totalAccessTime += memoryStats.accessTimeStats.cacheAccessTime; 
		return line->data.at(addressParts.byteOffSet);
	}
	else 
	{
		memoryStats.accessTimeStats.totalAccessTime +=
			memoryStats.accessTimeStats.cacheAccessTime + memoryStats.accessTimeStats.memoryAccessTime; 

		uint32_t lineStart = address & 
			~(MemoryConfig::getMemoryConfig()->getCacheLineSize() - 1);

		std::vector<uint8_t> buffer(MemoryConfig::getMemoryConfig()->getCacheLineSize());

		mainMemory->read(lineStart, MemoryConfig::getMemoryConfig()->getCacheLineSize(), buffer.data());

		CacheLine* newLine = set.replace(lineStart, addressParts.tag, buffer.data(), mainMemory); 
		
		return newLine->data.at(addressParts.byteOffSet);
	}
	return 0;
}

void Cache::write(const uint32_t address, uint8_t data)
{
	FullyAssociateAddressParts addressParts(address); 

	CacheLine* line = set.find(addressParts.tag); 

	memoryStats.cacheStats.cacheAccesses += 1; 

	// Hit
	if (line) {
		// switch (strategy) case Strategy::WriteThrough: WriteThrough(info);
		std::stringstream ss;
		ss << "Writing to cache (tag: " << addressParts.tag << " "
			<< "address: 0x" << std::hex << std::uppercase << address << ", value: 0x"
			<< std::setw(2) << std::setfill('0') << static_cast<uint32_t>(data) << ")\n";
		std::cout << ss.str();

		
		memoryStats.cacheStats.cacheHits += 1; 
		memoryStats.accessTimeStats.totalAccessTime += memoryStats.accessTimeStats.cacheAccessTime;

		if (MemoryConfig::getMemoryConfig()->getWriteHitStrategy() == "WT") {
			if (addressParts.byteOffSet < line->data.size()) {
				line->data.at(addressParts.byteOffSet) = data;
			}
			else {
				std::cerr << " write out of bounds at offset "
					<< addressParts.byteOffSet << " (line size = " << line->data.size() << ")\n";
				return;
			}

			mainMemory->write(address, 1, &data);
			memoryStats.accessTimeStats.totalAccessTime += memoryStats.accessTimeStats.memoryAccessTime;
		}
		else if (MemoryConfig::getMemoryConfig()->getWriteHitStrategy() == "WB") {
			line->data.at(addressParts.byteOffSet) = data;
			line->dirty = true;
		}
	}
	else 
	{
		if (MemoryConfig::getMemoryConfig()->getWriteMissStrategy() == "WR") {
			memoryStats.accessTimeStats.totalAccessTime += memoryStats.accessTimeStats.cacheAccessTime;
			mainMemory->write(address, 1, &data);
			memoryStats.accessTimeStats.totalAccessTime += memoryStats.accessTimeStats.memoryAccessTime;
		}
		else if (MemoryConfig::getMemoryConfig()->getWriteMissStrategy() == "WA") {
			memoryStats.accessTimeStats.totalAccessTime += memoryStats.accessTimeStats.memoryAccessTime + memoryStats.accessTimeStats.cacheAccessTime;

			int32_t lineStart = address &
				~(MemoryConfig::getMemoryConfig()->getCacheLineSize() - 1);

			std::vector<uint8_t> buffer(MemoryConfig::getMemoryConfig()->getCacheLineSize());

			mainMemory->read(lineStart, MemoryConfig::getMemoryConfig()->getCacheLineSize(), buffer.data());

			CacheLine* newLine = set.replace(lineStart, addressParts.tag, buffer.data(), mainMemory);

			if (newLine) {
				if (addressParts.byteOffSet < newLine->data.size()) {
					newLine->data.at(addressParts.byteOffSet) = data;

					if (MemoryConfig::getMemoryConfig()->getWriteHitStrategy() == "WB") {
						newLine->dirty = true;
					}
					else if (MemoryConfig::getMemoryConfig()->getWriteHitStrategy() == "WT") {

						mainMemory->write(address, 1, &data);
						memoryStats.accessTimeStats.totalAccessTime += memoryStats.accessTimeStats.memoryAccessTime;
					}
				}
				else {
					std::cerr << " write out of bounds at offset "
						<< addressParts.byteOffSet << " (line size = " << newLine->data.size() << ")\n";
					return;
				}
			}
		}
	}
}

MemoryStatistics Cache::getMemoryStatistics() const
{
	return memoryStats; 
}

void Cache::resetStatistics()
{
	memoryStats.cacheStats.cacheAccesses = 0; 
	memoryStats.cacheStats.cacheHits = 0; 

	memoryStats.accessTimeStats.totalAccessTime = 0; 
}
