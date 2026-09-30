#include "MainMemory.h"

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cstring>

MainMemory::MainMemory() {
	memory = std::make_unique<std::array<uint8_t, MAIN_MEMORY_SIZE >>();

	memory.get()->fill(0x00); // fill with zero's
}

void MainMemory::read(uint32_t startAddress, uint8_t size, uint8_t* destination) const {
	std::stringstream ss; 
	ss << "Reading from main memory (address: 0x" << std::hex << std::uppercase << startAddress << ")\n"; 
	std::cout << ss.str(); 

	std::memcpy(destination, &memory.get()->at(startAddress), size);
}

void MainMemory::write(uint32_t startAddress, uint8_t size, uint8_t* source) {
	std::stringstream ss;
	ss << "Writing to main memory (address: 0x" << std::hex << std::uppercase << startAddress << ", value:";
	for (uint8_t i = 0; i < size; ++i) {
		ss << " 0x" << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(source[i]);
	}
	ss << ")\n";
	std::cout << ss.str();

	std::memcpy(&memory.get()->at(startAddress), source, size);
}

void MainMemory::print() const {
	constexpr uint32_t ROWS = 24; 
	constexpr uint8_t COLS = 12; 

	for (uint32_t row = 0; row < ROWS; ++row) {
		for (uint32_t col = 0; col < COLS; ++col) {
			std::stringstream ss; 
			ss << "0x" << std::setw(2) << std::setfill('0') << std::hex << std::uppercase
				<< static_cast<uint32_t>(memory.get()->at(row * COLS + col));
			std::cout << ss.str() << " ";
		}
		std::cout << "\n"; 
	}
	std::cout << "\n";

	std::string line(120, '-');
	std::cout << line << "\n";
}