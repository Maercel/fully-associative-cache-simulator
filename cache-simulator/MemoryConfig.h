#pragma once

#include <string>
#include <cstdint>
#include <cmath>
#include <mutex>

class MemoryConfig {
private:
	uint8_t cacheLineSize = 64;
	uint32_t cacheLineSetSize = 64;
	uint8_t memoryAddressSize = 32;
	float_t cacheAccessTime = 0.0f;
	float_t memoryAccessTime = 0.0f;
	std::string readStrategy = "LRU";
	std::string writeHitStrategy = "WT";
	std::string writeMissStrategy = "WR";

	static MemoryConfig* memoryConfig;
	static std::mutex mtx;

	MemoryConfig() = default;
public:
	MemoryConfig(const MemoryConfig&) = delete;
	MemoryConfig& operator=(const MemoryConfig&) = delete;

	static MemoryConfig* getMemoryConfig();

	void setMemoryConfig(uint8_t lineSize, uint32_t lineSetSize,
		uint8_t addressSize, float_t cacheTime, float_t memTime,
		const std::string& readStrat,
		const std::string& writeHitStrat,
		const std::string& writeMissStrat);

	uint8_t getCacheLineSize() const;
	uint32_t getCacheLineSetSize() const;
	uint8_t getMemoryAddressSize() const;
	float_t getCacheAccessTime() const;
	float_t getMemoryAccessTime() const;
	const std::string& getReadStrategy() const;
	const std::string& getWriteHitStrategy() const;
	const std::string& getWriteMissStrategy() const;
};
