# Write Experiments (`test2.csim`)

All runs: 4 bytes per line, 8 lines, 8-bit addresses, replacement `LRU`.

## 1. WT + WA (cache 50 ns, memory 500 ns)

```bash
./main 4 8 8 50 500 LRU WT WA test2.csim
```

- Instructions executed: 1034
- Hits: 114
- Execution time: 759200

## 2. WB + WA (cache 100 ns, memory 200 ns)

```bash
./main 4 8 8 100 200 LRU WB WA test2.csim
```

- Instructions executed: 1034
- Hits: 114
- Execution time: 287400

## 3. WB + WR (cache 90 ns, memory 100 ns)

```bash
./main 4 8 8 90 100 LRU WB WR test2.csim
```

- Instructions executed: 1034
- Hits: 117
- Execution time: 184760

## 4. Write-back (WB)

- The data is updated only in the cache, and `dirty = true`.
- The line is written to memory only when necessary (in the `replace` function).
- On replacement we check whether the line is dirty and, if so, write the whole line back to main memory at the block's own address.
- Higher efficiency, but main memory is not always up to date.

From run 3. Instruction `P(78, 70)` hits a block that is already in the cache:

```
Writing to cache (tag: 30 address: 0x78, value: 0x70)
```

Main memory is not changed yet. Later, when this line is replaced, the whole dirty block is written back:

```
Writing to main memory (address: 0x78, value: 0x70 0x00 0x00 0x00)
```

## 5. Write-through (WT)

- With write-through we write to both the cache and main memory.
- First to the cache, then to memory.
- Main memory and the cache are always up to date.
- The downside is lower efficiency with frequent writes, because every write also pays the memory time.

From run 1. The same instruction `P(78, 70)`:

```
Writing to cache (tag: 30 address: 0x78, value: 0x70)
Writing to main memory (address: 0x78, value: 0x70)
```

## 6. Write-around (WR)

- On a write miss, the data is written to main memory only. The cache is not updated.
- The block is loaded into the cache later, only if it is read.
- This avoids loading blocks that are written but never read, but repeated writes to the same block always go to memory.

From run 3. Instruction `P(30, B4)`, the block is not in the cache:

```
Writing to main memory (address: 0x30, value: 0xB4)
```

There is no `Writing to cache` line: the cache is skipped.
