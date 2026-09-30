# Read Experiments (`test1.csim`)

All runs: 4 bytes per line, 8 lines, 8-bit addresses, write-hit `WT`, write-miss `WR`.

## 1. FIFO (cache 50 ns, memory 400 ns)

```bash
./main 4 8 8 50 400 FIFO WT WR test1.csim
```

- Instructions executed: 1365
- Hits: 154
- Execution time: 552650

## 2. LRU (cache 100 ns, memory 200 ns)

```bash
./main 4 8 8 100 200 LRU WT WR test1.csim
```

- Instructions executed: 1365
- Hits: 156
- Execution time: 378300

## 3. Random (cache 20 ns, memory 100 ns)

```bash
./main 4 8 8 20 100 Random WT WR test1.csim
```

- Instructions executed: 1365
- Hits: 75
- Execution time: 156300

## 4. Hit rate

- Run 1: 11.28%, FIFO – acceptable, slightly worse than LRU.
- Run 2: 11.43%, LRU – best of the three, best cache utilization.
- Run 3: 5.49%, Random – clearly the worst. The strategy is not suitable.

## 5. Read miss (data is not in the cache)

- Increases the total time (cache time + memory time).
- First checks whether a block with that tag exists in the cache.
- It doesn't: compute the start address of the block, read the whole block from main memory and add it to the cache.

Instruction `B(78)` from run 2:

```
Reading from main memory (address: 0x78)
```

The block `0x78`–`0x7B` (4 bytes) is loaded into the cache.

## 6. Read hit (data is in the cache)

- Hit.
- Increases the total time (cache time only).
- Increments the hit counter by 1.
- Returns the data from the cache, without accessing main memory.

The next instruction, `B(7B)`, is in the block that was just loaded:

```
Reading from cache (tag: 30 address: 0x7B)
```

The tag is `0x7B / 4 = 30`, the same block as `0x78`, so it is a hit.
(The tag is printed in decimal, the address in hex.)
