#pragma once

#include <cstdint>
#include <string>
#include <list>

// Random, LRU, FIFO

constexpr uint32_t SEED = 123;

class ReplacementAlgorithms
{
private:
	uint32_t lineSetSize = 0;

	std::string readStrategy; 

	uint32_t FIFOCounter = 0;
	uint8_t randomSeed = SEED;
	std::list<uint8_t> lruList; 
public: 
	ReplacementAlgorithms() = default; 

	void initialize(const std::string& readStrategy, const uint32_t lineSetSize);

	uint8_t getVictim(); 
	
	uint8_t LRUAlgorithm();
	void updateLRUList(const uint8_t lineIndex); 
	uint8_t FIFOAlgoritm(); 
	uint8_t randomVictim(uint8_t& seed);

};

