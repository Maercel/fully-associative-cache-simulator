#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>
#include <iomanip>
#include <fstream>

#include "MemorySystem.h"
#include "MemoryConfig.h"

constexpr char READ = 'B';
constexpr char WRITE = 'P';

void loadTestMemoryConfig() {
    MemoryConfig::getMemoryConfig()->setMemoryConfig(
        static_cast<uint8_t>(4),
        static_cast<uint32_t>(8),
        static_cast<uint8_t>(8),
        static_cast<float_t>(90),
        static_cast<float_t>(100),
        "LRU",
        "WB",
        "WR"
    );
}

void loadInstructions(const std::string filePath, MemorySystem& memorySystem) {
    std::ifstream in(filePath);
    if (!in.is_open()) return; 

    int counter = 0; 
    std::string line; 
    while (std::getline(in, line)) {
        std::size_t start = line.find('(');
        std::size_t end = line.find(')');

        if (line[0] == READ) {
            std::string addressStr = line.substr(start + 1, end - start - 1);
            uint32_t addressUint = static_cast<uint32_t>(std::stoul(addressStr, nullptr, 16));
            memorySystem.read(addressUint);
        }
        else if (line[0] == WRITE)
        {
            std::size_t comma = line.find(',', start);
            std::string addressStr = line.substr(start + 1, comma - start - 1); 
            std::string dataStr = line.substr(comma + 1, end - comma - 1); 

            uint32_t addressUint = static_cast<uint32_t>(std::stoul(addressStr, nullptr, 16));
            uint8_t dataUint = static_cast<uint8_t>(std::stoul(dataStr, nullptr, 16));

            memorySystem.write(addressUint, dataUint);
        }
        ++counter; 
    }
    std::cout << "Counter: " << counter << "\n";
    in.close(); 
}

int main(int argc, const char* argv[])
{
    std::string fileName;
    if (argc < 9) { 
        loadTestMemoryConfig();
        fileName = "test2.csim";
    }
    else {
        MemoryConfig::getMemoryConfig()->setMemoryConfig(
            static_cast<uint8_t>(atoi(argv[1])),
            static_cast<uint32_t>(atoi(argv[2])),
            static_cast<uint8_t>(atoi(argv[3])),
            static_cast<float_t>(atof(argv[4])),
            static_cast<float_t>(atof(argv[5])),
            argv[6],
            argv[7],
            argv[8]
        );
        fileName = argv[9];
    }

   


    MemorySystem memorySystem; 

    loadInstructions(fileName, memorySystem);
    /*
    uint32_t data1 = memorySystem.read(0x20);

    data1 = memorySystem.read(0x20);
    memorySystem.write(0x20, 0x39);  

  
    uint32_t data2 = memorySystem.read(0x20); 
    std::cout << "0x" << std::hex << data2 << std::endl;

    memorySystem.write(0x21, 0x40);
    memorySystem.write(0x10, 0x12);
    memorySystem.write(0x11, 0x11);
    memorySystem.write(0x10, 0x77);
    memorySystem.write(0xC1, 0x11);
    
    std::cout << "0x" << std::hex << memorySystem.read(0x10) << "\n";
    
    */
    memorySystem.printMainMemory();

    MemoryStatistics memoryStats = memorySystem.getMemoryStatistics(); 
    std::cout << "Total cache accesses: " << std::dec << memoryStats.cacheStats.cacheAccesses << "\t";
    std::cout << "Total cache hits: " << memoryStats.cacheStats.cacheHits << "\t";
    std::cout << "Cache hit rate: " << memoryStats.cacheStats.getHitRate() * 100 << "%\t";
    std::cout << "Total access time: " << std::fixed << std::setprecision(2)
        << memoryStats.accessTimeStats.totalAccessTime << "\n";
       
    return 0;
}
