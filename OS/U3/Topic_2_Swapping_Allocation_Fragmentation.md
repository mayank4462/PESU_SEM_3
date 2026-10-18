---
title: "OS — Topic 2: Swapping, Allocation and Fragmentation"
course: UE25CS242A
tags:
  - operating-systems
  - memory-management
  - swapping
  - fragmentation
---

# Topic 2 — Swapping, Allocation and Fragmentation

## 1. Swapping

**Swapping** temporarily moves a process's memory image from RAM to secondary storage, then brings it back when needed.

- **Backing store:** Secondary storage used to hold swapped-out process images.
- Swapping can free RAM for other processes and support memory oversubscription.
- A process cannot execute until the memory it needs is resident in RAM.
- Classic swapping moves a whole process image; modern systems more commonly move individual pages.

### Roll-out and roll-in
- **Roll-out:** Swap a lower-priority process out of RAM.
- **Roll-in:** Load a higher-priority process into the freed space.
- Used in classic priority-based scheduling examples.

### Swapping time

`Transfer time = Amount of data / Transfer rate`

Example: process size = 100 MB, transfer rate = 50 MB/s.
- Swap-out = 100 / 50 = 2 seconds.
- Swap-in of another 100 MB process = 100 / 50 = 2 seconds.
- Total transfer overhead = 4 seconds, excluding other overhead.

For 3000 MB at 50 MB/s:
- One transfer = 3000 / 50 = 60 seconds.
- Swap-out + equal-size swap-in = about 120 seconds.

### Limitations
- Disk/SSD transfer is much slower than RAM access.
- Moving a whole process can be expensive.
- **Pending I/O problem:** If a device is writing directly into a process buffer and that process's memory is moved/reused, data may be written to the wrong location.
- The OS can avoid swapping a process with pending direct I/O, or use kernel buffers and copy data into the correct process buffer.

## 2. Contiguous memory allocation

Each process occupies one continuous physical-memory region.

- Simple to manage: record the region's base and size.
- The OS may occupy a low-memory region; user processes occupy other regions (exact layout varies).
- The **interrupt vector** identifies addresses of interrupt service routines. The classic notes example uses 256 entries × 4 bytes = 1024 bytes, addresses `000000H–0003FFH`; this is a textbook example, not universal hardware behavior.

### Base and limit registers

- **Base/relocation register:** Starting physical address of the allocated region.
- **Limit register:** Size of the legal logical-address range.
- Valid logical address condition: `0 <= logical address < limit`.
- Physical address: `base + logical address`.
- Hardware checks each relevant access; an invalid access raises a protection trap.
- The OS loads protection registers using privileged instructions during process dispatch/context switching.

Example: base = 20000, limit = 5000.
- Valid logical addresses: 0–4999.
- Physical addresses: 20000–24999.
- Logical address 5000 is invalid.

## 3. Fixed versus variable partitioning

### Fixed partitioning
Memory is split into predetermined partitions; one process occupies each partition.

Advantages:
- Simple allocation and tracking.

Disadvantages:
- Number of resident processes limited by partition count.
- A process may not fit any partition even when some memory is free.
- Internal fragmentation occurs if the process is smaller than its partition.

Example: request 400 KB, partition 512 KB → internal fragmentation = 512 − 400 = 112 KB.

### Variable partitioning
Partitions are created dynamically according to process requirements.

- Free contiguous regions are called **holes**.
- When a process terminates, its region becomes free; adjacent holes can be merged.
- Reduces internal fragmentation when allocations match requests closely.
- External fragmentation can develop as free memory becomes scattered.
- Requires tracking and searching free regions.

| Property | Fixed | Variable |
|---|---|---|
| Partition sizes | Predetermined | Created dynamically |
| Internal fragmentation | Common | Reduced with exact-size allocation |
| External fragmentation | Not the main issue in the basic model | Common |
| Main limitation | Partition count and wasted space within partitions | Scattered free holes |

## 4. Dynamic storage-allocation algorithms

For variable partitioning, the OS chooses a hole for each request.

- **First-fit:** First hole large enough. Often fast.
- **Best-fit:** Smallest hole large enough. Can create many tiny holes; often requires searching all holes.
- **Worst-fit:** Largest hole. Intended to leave a large remainder, but can use large blocks inefficiently.

Example holes: `100, 500, 250 KB`; request = 200 KB.
- First-fit → 500 KB, remainder 300 KB (assuming the list order shown).
- Best-fit → 250 KB, remainder 50 KB.
- Worst-fit → 500 KB, remainder 300 KB.

### Full slide problem

Initial holes: `300, 600, 350, 200, 750, 125 KB`.
Process requests in order: `P1=115, P2=500, P3=358, P4=200, P5=375 KB`.
Assume a selected hole is split and the remainder stays in the same list position.

#### First-fit

| Process | Selected hole | Remainder |
|---|---:|---:|
| P1=115 | 300 | 185 |
| P2=500 | 600 | 100 |
| P3=358 | 750 | 392 |
| P4=200 | 350 | 150 |
| P5=375 | 392 | 17 |

All five allocate. Final holes: `185, 100, 150, 200, 17, 125 KB`.

#### Best-fit

| Process | Selected hole | Remainder |
|---|---:|---:|
| P1=115 | 125 | 10 |
| P2=500 | 600 | 100 |
| P3=358 | 750 | 392 |
| P4=200 | 200 | 0 |
| P5=375 | 392 | 17 |

All five allocate. Final nonzero holes: `300, 100, 350, 17, 10 KB`.

#### Worst-fit

| Process | Selected hole | Remainder |
|---|---:|---:|
| P1=115 | 750 | 635 |
| P2=500 | 635 | 135 |
| P3=358 | 600 | 242 |
| P4=200 | 350 | 150 |
| P5=375 | No sufficient hole | Not allocated |

P5 waits despite 1152 KB total free space, because the largest hole is smaller than 375 KB. This demonstrates external fragmentation.

## 5. Fragmentation

### Internal fragmentation
Unused space **inside** an allocated block.

`Internal fragmentation = allocated size − requested size`

Example: allocated partition 512 KB; request 400 KB → 112 KB internal fragmentation.

### External fragmentation
Free memory exists but is divided among separate holes, so no single hole can satisfy a contiguous request.

Example: holes = 100 KB, 150 KB, 200 KB.
- Total free = 450 KB.
- Request = 300 KB.
- Largest hole = 200 KB, so allocation fails under contiguous allocation.

**Quick distinction:** wasted space inside an allocated block = internal; free space scattered between allocated blocks = external.

## 6. 50-percent rule

A simplified statistical rule of thumb for first-fit allocation estimates that about `0.5N` additional blocks may be lost to fragmentation for `N` allocated blocks.

- Allocated blocks = `N`.
- Fragmented blocks (model estimate) = `0.5N`.
- Total = `1.5N`.
- Fragmented fraction = `0.5N / 1.5N = 1/3`.

This is an approximation under model assumptions, not a universal guarantee.

## 7. Compaction

**Compaction** relocates allocated blocks so scattered free holes combine into one large contiguous hole.

Example:
- Before: free holes of 100 KB, 200 KB and 150 KB, separated by processes.
- Total free = 450 KB, but largest hole = 200 KB.
- After compaction: one 450 KB hole may be available.

Requirements and limitations:
- Dynamic relocation support is needed.
- Moving memory consumes time and bandwidth.
- I/O must be coordinated so devices do not write into reused/moved memory.
- Compaction is expensive for large allocations.

Compaction reduces external fragmentation; it does not automatically eliminate internal fragmentation.

## 8. Buddy system

The buddy system allocates blocks in power-of-two sizes, e.g. 4, 8, 16, 32, 64, 128, 256 KB.

- Split a larger free block into two equal buddies until a suitable size is reached.
- On free, merge a block with its buddy when both are free and correctly aligned.
- Efficient splitting/merging and tracking.
- Can cause internal fragmentation because the allocation is rounded up.

Example: request 200 KB from a 1024 KB block.
- Split 1024 → 512 + 512.
- Split one 512 → 256 + 256.
- Allocate 256 KB; internal waste = 256 − 200 = 56 KB.

Linux uses a buddy allocator for physical page-frame allocation, alongside other allocation mechanisms.

## 9. High-yield comparison table

| Concept | Key point |
|---|---|
| Swapping | Move process image between RAM and backing storage |
| Roll-out/roll-in | Swap out lower-priority process to load higher-priority process |
| First-fit | First sufficient hole |
| Best-fit | Smallest sufficient hole |
| Worst-fit | Largest hole |
| Internal fragmentation | Waste inside allocated block |
| External fragmentation | Free memory scattered into separate holes |
| Compaction | Relocate allocations to combine free holes |
| Buddy system | Power-of-two splitting and merging |

## 10. Exam questions

1. Define swapping and backing store.
2. Explain roll-out and roll-in.
3. A 200 MB process is swapped out and another 200 MB process swapped in at 100 MB/s. Calculate transfer time.
4. Explain the pending I/O problem.
5. Compare fixed and variable partitioning.
6. Define a hole.
7. Solve first-fit, best-fit and worst-fit allocation for a given sequence.
8. Differentiate internal and external fragmentation with examples.
9. State the 50-percent rule and explain its assumptions.
10. Explain compaction and its limitations.
11. Explain the buddy system with a numerical example.
12. Why can a process fail to allocate memory even when total free memory exceeds its request?

## 11. Quick revision

- Swap time = data size ÷ transfer rate.
- Include both swap-out and swap-in if the question asks for both.
- First-fit = first sufficient; best-fit = smallest sufficient; worst-fit = largest.
- Internal fragmentation is inside allocated blocks.
- External fragmentation is scattered free space between allocations.
- Compaction combines free holes but requires relocation.
- Buddy allocation splits/merges power-of-two blocks.
