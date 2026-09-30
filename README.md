<h1 align="center">Cache Simulator</h1>

<p align="center">
  <img alt="C++" src="https://img.shields.io/badge/C++17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white">
  <img alt="CMake" src="https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white">
</p>

<br>

<div align="center">
  <b>Read And Write</b><br>
  <sub>Every read and write to the cache or main memory.</sub><br>
  <img src="docs/screenshots/read-write-output.png" alt="Read and write log" width="50%"><br><br>
  <b>Main Memory</b><br>
  <sub>Main memory contents after all instructions. </sub><br>
  <img src="docs/screenshots/memory-output.png" alt="Main memory dump" width="50%"><br><br>
  <b>Statistics</b><br>
  <sub>Accesses, hits, hit rate and total access time (ns)</sub><br>
  <img src="docs/screenshots/stats.png" alt="Statistics" width="50%">
</div>

---

## 📖 About

A **C++ simulator** of a **Fully Associative CPU cache**.
It **reads memory instructions** from a **file** and prints:
- Cache accesses
- Cache hits
- Cache hit rate
- Total cache access time

## 🔨 Build

```bash
cd cache-simulator
g++ -std=c++17 -o main main.cpp MemorySystem.cpp Cache.cpp MainMemory.cpp Processor.cpp ReplacementAlgorithms.cpp MemoryConfig.cpp WriteStrategy.cpp
```

## ▶️ Run

```bash
./main 4 8 8 90 100 LRU WB WR test2.csim
```

Without parameters it uses the defaults: `4 8 8 90 100 LRU WB WR test2.csim` (same as the example).

### ⚙️ Parameters (in order):
- **line size** - one cache line size in bytes (4)
- **line count**- number of lines in the cache (8)
- **address bits** - memory address size (8)
- **cache time** - cache access time in ns (90)
- **memory time** - memory access time in ns (100)
- **replacement** - LRU, FIFO or Random
- **write hit**- WT or WB
- **write miss** - WR or WA
- **file** - instruction file (test1.csim or test2.csim)

## 📄 Instruction file

```
B(F5)       read address 0xF5
P(67, 54)   write 0x54 to address 0x67
```

`B` = read, `P` = write (from Slovene *branje* / *pisanje*).

## 📖 Terms

- **Fully associative**: a memory block can be stored in any cache line. The cache searches all lines to find it (by cache tag).
- **LRU**: replace the least recently used line.
- **FIFO**: replace the line that has been in the cache the longest.
- **Random**: replace a random line.
- **WT** (write-through): write to cache and memory.
- **WB** (write-back): on a write hit, update only the cache and mark the line as dirty. The line is written to memory when it is replaced.
- **WR** (write-around): on a miss, write to memory only.
- **WA** (write-allocate): on a miss, load the block into the cache, then write it as s hit (with WT or WB).

## 📊 Experiments

- [test1-results.md](test1-results.md): read tests (LRU, FIFO, Random)
- [test2-results.md](test2-results.md): write tests (WT, WB, WR, WA)
