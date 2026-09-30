#pragma once
#include <cstdint>
#include <memory>
#include <array>

constexpr uint32_t MAIN_MEMORY_SIZE = 4 * 1024 * 1024; // 4MB main memory

class MainMemory
{
private: 
	std::unique_ptr<std::array<uint8_t, MAIN_MEMORY_SIZE>> memory; 

public: 
	MainMemory(); 

	void read(uint32_t startAddress, uint8_t size, uint8_t* destination) const;
	void write(uint32_t startAddress, uint8_t size, uint8_t* source); 
	void print() const; 
};

