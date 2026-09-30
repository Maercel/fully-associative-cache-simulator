#include "WriteStrategy.h"

WriteStrategy::WriteStrategy(const std::string& writeHitPolicy, const std::string& writeMissPolicy)
{
	this->writeHitPolicy = writeHitPolicy; 
	this->writeMissPolicy = writeMissPolicy; 
}


void WriteStrategy::writeThrough(CacheLine* line, const FullyAssociateAddressParts& addressParts,  MemoryStatistics& memoryStats)
{

}

void WriteStrategy::writeAround()
{

}
