#include "ReplacementAlgorithms.h"

void ReplacementAlgorithms::initialize(const std::string& readStrategy, const uint32_t lineSetSize)
{
	this->lineSetSize = lineSetSize;
	this->readStrategy = readStrategy; 

	FIFOCounter = 0; 
	lruList.clear(); 
	for (uint8_t address = 0; address < lineSetSize; ++address) {
		lruList.push_back(address);
	}
}

uint8_t ReplacementAlgorithms::getVictim()
{
	if (readStrategy == "Random") return randomVictim(randomSeed);
	else if (readStrategy == "LRU") return LRUAlgorithm();
	else if (readStrategy == "FIFO") return FIFOAlgoritm();

	return randomVictim(randomSeed);
}

uint8_t ReplacementAlgorithms::LRUAlgorithm()
{
	uint8_t victim = lruList.front(); 
	lruList.pop_front(); 
	return victim; 
}

void ReplacementAlgorithms::updateLRUList(const uint8_t lineIndex)
{
	if (readStrategy != "LRU") return; 

	lruList.remove(lineIndex);
	lruList.push_back(lineIndex);
}

uint8_t ReplacementAlgorithms::FIFOAlgoritm()
{
	return static_cast<uint8_t>(FIFOCounter++ % lineSetSize); 
}

uint8_t ReplacementAlgorithms::randomVictim(uint8_t& seed) {
	constexpr uint16_t m = 256;
	constexpr uint8_t a = 3;
	constexpr uint8_t c = 3;
	seed = (seed * a + c) % m;

	return static_cast<uint8_t>(seed % lineSetSize);
}

