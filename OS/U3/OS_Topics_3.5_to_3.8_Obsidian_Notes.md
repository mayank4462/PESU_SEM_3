---
title: "OS Unit 3 — Topics After Paging (3.5–3.8)"
course: UE25CS242A
tags:
  - operating-systems
  - memory-management
  - paging
  - virtual-memory
  - page-replacement
  - numa
---

# OS Unit 3 — Topics After Paging (3.5–3.8)

These notes cover the remaining material after Topic 3.4 (Paging), based on the uploaded PESU Unit 3 slide decks and notes.

## Contents

- [[#Topic 3.5 — Structure of Page Tables, TLB and EAT]]
- [[#Topic 3.6 — Virtual Memory, Demand Paging and Copy-on-Write]]
- [[#Topic 3.7 — Page Replacement, Frame Allocation and Thrashing]]
- [[#Topic 3.8 — NUMA, Memory Locality and Clock Page Replacement]]
- [[#Final revision sheet]]
- [[#Exam practice]]

---

# Topic 3.5 — Structure of Page Tables, TLB and EAT

## 1. Why page-table design matters

Paging divides virtual memory into fixed-size pages and physical memory into equal-size frames. The page table maps virtual page numbers to physical frame numbers.

Two problems arise:
1. **Translation overhead:** looking up the page table in RAM for every memory reference takes time.
2. **Page-table size:** a large virtual address space can require a very large page table.

The TLB addresses translation speed. Hierarchical, hashed, and inverted page tables address page-table storage and lookup concerns.

## 2. Hardware support: PTBR and PTLR

- **PTBR — Page-Table Base Register:** points to the start of the page table.
- **PTLR — Page-Table Length Register:** indicates the page-table size/number of entries in the model used in the lecture.

If the page table is stored in RAM, a simple translation may require:
1. One memory access to read the page-table entry.
2. One memory access to read/write the requested data.

If each RAM access takes 100 ns, this simplified process takes about 200 ns, excluding other overhead.

## 3. Translation Lookaside Buffer (TLB)

A **TLB** is a small, fast hardware cache that stores recently used virtual-page-to-physical-frame translations.

Typical entry information can include:
- Virtual page number
- Physical frame number
- Valid/present bit
- Access permissions
- Address-space identifier (ASID), on systems that support it

### TLB hit
The translation is found in the TLB. The frame number is available without a separate page-table lookup in RAM.

### TLB miss
The translation is not in the TLB. The system looks up the page table. If the page is resident and the entry is valid, the translation can be placed in the TLB.

**A TLB miss is not necessarily a page fault.** A page can be in RAM even when its translation is absent from the TLB.

### TLB versus CPU cache

| TLB | CPU cache |
|---|---|
| Caches address translations | Caches instruction/data contents |
| Maps virtual page to physical frame | Holds copies of memory contents |
| Speeds up address translation | Speeds up data/instruction access |

A TLB hit does not guarantee a CPU-cache hit; these are separate mechanisms.

## 4. Effective Access Time (EAT)

Let:
- \(h\) = TLB hit ratio
- \(1-h\) = TLB miss ratio
- \(T_M\) = main-memory access time
- \(T_T\) = TLB lookup time

### Simplified model: ignore TLB lookup time

On a hit, one RAM access is needed. On a miss, a page-table access and a data access are needed:

\[
EAT=hT_M+(1-h)(2T_M)
\]

**Example:** \(T_M=10\) ns, \(h=0.8\).

\[
EAT=0.8(10)+0.2(20)=8+4=\boxed{12\text{ ns}}
\]

At a 99% hit ratio:

\[
EAT=0.99(10)+0.01(20)=\boxed{10.1\text{ ns}}
\]

### Model including TLB lookup time

\[
EAT=h(T_T+T_M)+(1-h)(T_T+2T_M)
\]

Example: RAM = 100 ns, TLB = 5 ns, hit ratio = 90%.

- Hit time = \(5+100=105\) ns
- Miss time = \(5+100+100=205\) ns

\[
EAT=0.9(105)+0.1(205)=\boxed{115\text{ ns}}
\]

**Exam rule:** Use the formula that matches the assumptions in the question. If TLB time is given, include it unless the question explicitly says to ignore it.

## 5. Address-space identifiers (ASIDs)

The same virtual page number can map to different physical frames in different processes. An **ASID** tags a TLB entry with its owning address space.

This helps prevent a translation from one process being mistaken for a translation belonging to another process. It can also reduce the need to flush the entire TLB on every context switch.

## 6. Protection and valid-invalid bits

Page-table entries can contain:
- **Protection bits:** permit or deny operations such as read, write, and execute.
- **Present/valid information:** indicates whether a translation is currently usable or whether the page is part of the permitted address space, depending on the architecture's encoding.

Writing to a read-only page or accessing an illegal address causes a protection exception/trap.

Do not assume that every non-present page is an illegal address: virtual-memory systems can use non-present entries to represent valid pages that are not currently resident in RAM.

## 7. Shared pages

Multiple processes can map the same physical frame. Read-only, reentrant code such as a shared library is a common example. Each process can still have separate private data pages.

Example from the lecture:
- 40 users
- Code per user = 150 KB
- Private data per user = 50 KB

Without sharing:

\[
40(150+50)=8000\text{ KB}
\]

With one shared code copy:

\[
150+40(50)=2150\text{ KB}
\]

Memory saved:

\[
8000-2150=\boxed{5850\text{ KB}}
\]

Shared writable data requires appropriate permissions and often synchronization.

## 8. Page-table size example

A 32-bit virtual address space has \(2^{32}\) byte addresses. With 4 KB pages:

\[
4\text{ KB}=2^{12}\text{ bytes}
\]

Number of virtual pages:

\[
\frac{2^{32}}{2^{12}}=2^{20}=1,048,576
\]

If every entry is 4 bytes:

\[
2^{20}\times4=2^{22}\text{ bytes}=\boxed{4\text{ MiB}}
\]

This is the storage for one flat page table under these assumptions.

## 9. Hierarchical (multi-level) page tables

A multi-level page table splits the page number into fields. For a two-level table, a logical address can be represented as:

\[
\langle p_1,p_2,d\rangle
\]

- \(p_1\): index into the outer page table
- \(p_2\): index into an inner page table
- \(d\): offset within the page

Translation:
1. Use \(p_1\) to locate the relevant inner page table.
2. Use \(p_2\) to locate the page-table entry.
3. Obtain the frame number.
4. Combine frame number with offset \(d\).

**Advantage:** inner page tables need only be allocated for portions of the address space that are used.

**Disadvantage:** a page-table walk may involve several memory accesses. TLBs reduce the average cost.

The number of levels and bit fields depends on the architecture and page size.

## 10. Hashed page tables

A hashed page table uses a hash function on the virtual page number to find a bucket.

1. Hash the virtual page number.
2. Find the corresponding bucket.
3. Search the bucket for the matching virtual page and address-space identity.
4. Obtain the physical frame number.

Hash collisions may cause multiple entries to share a bucket, so entries must be checked for a match. Hashed page tables can be useful for large, sparse address spaces.

## 11. Inverted page tables

A conventional page table has entries corresponding to virtual pages. An **inverted page table** has one main entry per physical frame.

| Conventional page table | Inverted page table |
|---|---|
| Organized by virtual pages | Organized by physical frames |
| Size relates to virtual address space | Size relates to physical memory |
| Direct virtual-page indexing is straightforward | Mapping lookup can be more complex |
| Can be large for huge virtual spaces | Can reduce table-storage requirements |

An inverted entry identifies the virtual page and address space currently occupying that physical frame. Hashing can help locate mappings.

Shared mappings and aliases require additional care because multiple virtual addresses can refer to one physical frame.

## 12. Huge pages

Common illustrative sizes include:
- Standard page: 4 KB
- Huge page: 2 MB
- Larger huge page: 1 GB

A 2 MB page covers:

\[
\frac{2\text{ MiB}}{4\text{ KiB}}=\boxed{512}
\]

times as much memory as a 4 KB page. Larger pages can improve **TLB reach** (the amount of memory addressable through cached translations).

Trade-off:
- Benefits: fewer page mappings and potentially fewer TLB misses.
- Costs: more internal waste in some workloads and stricter allocation/management constraints.

---

# Topic 3.6 — Virtual Memory, Demand Paging and Copy-on-Write

## 1. Virtual memory

**Virtual memory** allows a process to execute without requiring its entire logical address space to be resident in physical RAM at once.

The OS keeps only the currently needed portions in RAM and stores or reconstructs other portions when required.

Benefits:
- Programs can have address spaces larger than available RAM.
- More processes can be kept active.
- Each process receives a private virtual address space.
- Memory can be used more efficiently when not all program pages are needed at once.

Virtual memory is commonly implemented using paging and demand paging.

## 2. Demand paging

**Demand paging** loads a page into RAM only when it is first needed.

A page-table entry or related OS metadata indicates whether the page is currently resident. If a process references a valid page that is not resident, a **page fault** occurs.

Page fault does not automatically mean a program has an error. It may be a normal part of demand paging.

## 3. What happens during a page fault?

A typical page-fault handling sequence:

1. The CPU references a virtual address.
2. The MMU detects that the page is not currently resident or that the access needs OS handling.
3. Hardware traps to the operating system.
4. The OS checks whether the reference is legal.
5. If illegal, the OS terminates the process or reports an access violation.
6. If legal, the OS locates the page on backing storage or creates/initializes the page as appropriate.
7. The OS finds a free frame. If none is available, it may select a victim page for replacement.
8. If the victim is dirty, its contents may need to be written to storage.
9. The required page is read or created in the selected frame.
10. The OS updates the page table and related translation metadata.
11. The faulting instruction is restarted.

### Page-fault flow to remember

`Reference → fault trap → validate address → obtain frame → load page → update tables → restart instruction`

The page-fault service time is much longer than a normal memory access because it may include storage I/O.

## 4. Valid page fault versus invalid access

| Situation | Meaning |
|---|---|
| Legal virtual page, currently not resident | Normal demand page fault; bring the page into RAM |
| Address is outside the process's legal space | Invalid access; signal/terminate or handle according to OS policy |
| Write to a read-only page | Protection fault, unless a special mechanism such as copy-on-write applies |
| TLB does not contain a mapping but page table does | TLB miss, not necessarily a page fault |

## 5. Restarting the instruction

After the page is loaded and the page table is updated, the CPU must retry the instruction that caused the fault. The OS and hardware must preserve enough state to resume execution correctly.

This matters because a memory reference can occur in the middle of an instruction. The architecture defines how faults are precise and restartable.

## 6. Effective Access Time with page faults

Let:
- \(p\) = probability/frequency of a page fault per memory reference
- \(T_M\) = normal memory access time
- \(T_F\) = page-fault service time

A simplified model is:

\[
EAT=(1-p)T_M+pT_F
\]

If \(T_M=100\) ns and \(T_F=8\) ms:

\[
8\text{ ms}=8,000,000\text{ ns}
\]

Even a small fault rate can dominate EAT. For example, if \(p=0.001\):

\[
EAT=0.999(100)+0.001(8,000,000)
\]

\[
EAT\approx100+8000=\boxed{8100\text{ ns (approximately)}}
\]

This simplified formula ignores details such as page-out time, dirty victims, and other overheads, but demonstrates why page faults are expensive.

## 7. Copy-on-Write (COW)

**Copy-on-Write** allows processes to share the same physical pages until one process tries to modify a shared page.

It is commonly useful after a `fork()` operation:
1. Parent and child initially refer to the same physical pages.
2. Shared pages are marked so a write triggers special handling.
3. If either process writes to a shared page, a protection fault occurs.
4. The OS allocates a separate frame and copies the page for the writer.
5. The writer's page table is updated to point to the private copy.
6. The write instruction is restarted.
7. The other process continues using the original page.

COW avoids copying all pages immediately when a process is created.

### COW example

Suppose a parent process has 100 pages and creates a child:
- Without COW, a simple eager-copy model could copy all 100 pages immediately.
- With COW, the two processes initially share the pages.
- If the child modifies only 3 pages, only those pages need private copies (assuming each modified page is copied separately and no other complexities apply).

**Important:** COW is not the same as swapping. COW copies a shared page when a write requires separation; swapping moves pages between RAM and backing storage.

## 8. Demand paging versus pre-paging

- **Demand paging:** bring a page in when it is referenced.
- **Pre-paging:** bring in pages in advance based on a prediction of likely future use.

Demand paging avoids loading pages that are never used. Pre-paging can reduce later faults if the prediction is good, but wastes I/O and RAM if the pages are not needed.

## 9. Locality and virtual memory

Programs tend to exhibit:
- **Temporal locality:** recently used locations are likely to be used again.
- **Spatial locality:** locations near a recently used location are likely to be used soon.

Demand paging works well when the active pages fit in memory because a process tends to reuse a relatively small working set over a period of time.

---

# Topic 3.7 — Page Replacement, Frame Allocation and Thrashing

## 1. Why page replacement is needed

A page fault may occur when no free frame is available. The OS must choose a page currently in RAM to remove.

This is **page replacement**.

Typical sequence:
1. Faulting page is identified as valid but non-resident.
2. OS selects a victim frame/page.
3. If the victim is dirty, write it back to storage.
4. Load the needed page into the frame.
5. Update page tables and TLB-related state.
6. Restart the instruction.

A clean page may be discarded without writing it back if its contents can be reloaded or reconstructed. A dirty/modified page generally needs to be written back before its frame is reused.

## 2. Page replacement algorithm goals

A good algorithm aims to reduce page faults while keeping overhead reasonable.

- **Reference string:** sequence of page numbers accessed by a process.
- **Page hit:** requested page is already resident.
- **Page fault:** requested page is not resident.
- **Frames:** physical slots available to hold pages.

When solving a numerical question, always state the number of frames, the algorithm, the initial state, each hit/fault, and the total faults.

## 3. FIFO — First-In, First-Out

FIFO replaces the page that has been in memory the longest.

How to apply:
1. Treat the frames as a queue.
2. If a reference is already in a frame, it is a hit and the queue order does not change.
3. If the reference is absent and a free frame exists, load it.
4. If all frames are occupied, replace the oldest loaded page.

Advantages:
- Simple to understand and implement.

Disadvantages:
- Ignores how recently or frequently a page is used.
- Can exhibit **Belady's anomaly**: increasing the number of frames can increase page faults.

## 4. Optimal (OPT) page replacement

OPT replaces the page whose next use is farthest in the future.

Advantages:
- Produces the minimum possible number of page faults for a known reference string and fixed frame count.
- Useful as a benchmark for evaluating other algorithms.

Disadvantage:
- The OS cannot know future references exactly in a real workload, so OPT is mainly theoretical.

To solve: for each replacement, look forward in the reference string and remove the page that will not be used for the longest time (or never again).

## 5. LRU — Least Recently Used

LRU replaces the page that has not been used for the longest time in the past.

Rationale: recent past use is used to estimate near-future use.

Advantages:
- Usually better than FIFO for workloads with locality.
- Does not exhibit Belady's anomaly under the standard stack-algorithm model.

Disadvantages:
- Exact LRU requires tracking recency, which can be costly in hardware/software.
- Systems often use approximations rather than exact LRU.

## 6. Clock / Second-Chance algorithm

The Clock algorithm approximates LRU with a circular arrangement of frames and a pointer called the **clock hand**.

Each frame has a reference/use bit \(R\):
- Hardware sets \(R=1\) when the page is referenced.
- On a replacement search:
  - If \(R=0\), select that page as the victim.
  - If \(R=1\), clear it to 0 and advance the hand, giving the page a second chance.
- Continue until a frame with \(R=0\) is found.
- Replace that page; the newly loaded page's reference bit is normally set to 1.

If all bits are 1, one full pass can clear them; a later check can select a victim.

Advantages:
- Simple and low overhead.
- Approximates LRU without maintaining exact access order.

Disadvantages:
- Does not always choose the true least-recently-used page.
- May scan multiple frames before finding a victim.

The separate detailed worked example is included in Topic 3.8 because the uploaded NUMA/Clock lecture contains the Clock algorithm example.

## 7. Belady's anomaly

Belady's anomaly is the surprising case where **increasing the number of frames causes more page faults**.

- It can occur with FIFO.
- It does not occur with OPT or standard LRU because they are stack algorithms.
- It is not a statement that adding memory usually hurts; it is a property of particular replacement behavior and reference strings.

## 8. Comparing algorithms

| Algorithm | Victim choice | Knows future? | Key property |
|---|---|---|---|
| FIFO | Oldest loaded page | No | Simple; may show Belady's anomaly |
| OPT | Page used farthest in future | Yes, idealized | Minimum faults; theoretical benchmark |
| LRU | Least recently used in past | No | Good locality behavior; stack algorithm |
| Clock | First page encountered with \(R=0\), clearing \(R=1\) along the way | No | Low-overhead LRU approximation |

## 9. Frame allocation

The OS must decide how many frames to give each process.

### Equal allocation

If there are \(m\) frames and \(n\) processes, each gets approximately \(m/n\) frames, with any remainder handled by policy.

Simple, but it ignores differences in process size and memory demand.

### Proportional allocation

Allocate frames in proportion to process size.

If process \(i\) has size \(s_i\), total size is \(S=\sum s_i\), and there are \(m\) available frames, an idealized proportional share is:

\[
a_i=\frac{s_i}{S}m
\]

The OS must round to whole frames and handle minimum/maximum constraints.

### Priority allocation

Higher-priority processes may receive more frames or be favored when memory is scarce. Exact policy is OS-dependent.

## 10. Global versus local replacement

- **Global replacement:** a process can take a frame from the pool of frames allocated to other processes.
- **Local replacement:** a process replaces pages only within its own assigned frames.

| Global replacement | Local replacement |
|---|---|
| Can allocate frames where demand is highest | Isolates each process's replacement behavior |
| One process can affect another's fault rate | More predictable per-process allocation |
| Can improve total utilization | May leave frames underused in one process while another is short |

## 11. Thrashing

**Thrashing** occurs when a system spends so much time handling page faults and moving pages that useful CPU execution drops sharply.

Typical cause:
- The combined active working sets of processes exceed available physical frames.
- Processes continually evict pages that they soon need again.
- Page-fault rate rises; storage I/O increases; CPU utilization may fall.

Symptoms:
- Very high page-fault rate.
- Heavy paging/swapping I/O.
- Low useful CPU utilization despite heavy system activity.
- Poor response time and throughput.

Adding more multiprogrammed processes can make thrashing worse because each process gets too few frames.

## 12. Working-set model

The **working set** estimates the pages a process has referenced in a recent window of \(\Delta\) memory references.

- \(WSS_i\) = working-set size of process \(i\).
- \(D=\sum WSS_i\) = total estimated demand for frames.
- \(m\) = number of available frames.

Decision:
- If \(D\le m\), the system may be able to keep the working sets resident.
- If \(D>m\), there is not enough memory for all estimated working sets; the OS may reduce the degree of multiprogramming by suspending a process.

The window size \(\Delta\) is important: too small may miss relevant pages; too large may include pages no longer active.

## 13. Page-Fault Frequency (PFF) control

PFF adjusts a process's frame allocation based on its page-fault rate.

- If fault rate rises above an upper threshold, allocate more frames if possible.
- If fault rate falls below a lower threshold, reclaim some frames for other processes.
- If fault rate is high and no free frames exist, the OS may suspend a process and reallocate its frames.

This creates a feedback loop that aims to keep page faults within a tolerable range.

## 14. Ways to prevent or reduce thrashing

1. Give each process enough frames for its active working set.
2. Estimate locality and working-set size.
3. Use working-set or PFF control to adjust allocations.
4. Reduce the degree of multiprogramming when memory demand exceeds supply.
5. Consider local replacement to prevent one process from destabilizing others.
6. Add physical memory if possible and appropriate.

---

# Topic 3.8 — NUMA, Memory Locality and Clock Page Replacement

## 1. NUMA definition

**NUMA (Non-Uniform Memory Access)** is a shared-memory multiprocessor architecture in which memory access time depends on where the memory is physically located relative to the CPU.

- A CPU accesses its own node's **local memory** with lower latency.
- It can access another node's **remote memory** through an interconnect, usually with higher latency.
- All CPUs can still address the shared system memory space.

NUMA improves scalability compared with a design where all CPUs compete for the same uniform memory connection, but software must account for locality.

## 2. Components of a NUMA system

Each NUMA node typically contains:
- One or more CPUs/cores
- Local RAM
- A memory controller
- Links to a high-speed interconnect

The interconnect allows CPUs to reach memory attached to other nodes.

The OS is responsible for scheduling processes and allocating pages in ways that improve locality.

## 3. Local versus remote memory

| Local memory | Remote memory |
|---|---|
| Attached to the CPU's node | Attached to another node |
| Lower latency, often higher effective bandwidth | Additional interconnect traversal, typically higher latency |
| Preferred for frequently accessed data | Needed when data is placed on another node or shared |

A NUMA-aware OS tries to place a process's pages near the CPU where it runs. If a process frequently moves between nodes or uses data located remotely, performance can suffer.

## 4. Advantages and disadvantages of NUMA

### Advantages
- Scales to larger multiprocessor systems.
- Can increase aggregate memory bandwidth.
- Reduces contention by distributing memory among nodes.
- Provides fast local memory access.

### Disadvantages
- Hardware and OS management are more complex.
- Remote memory accesses are slower.
- Performance varies with process placement and memory placement.
- Workloads that frequently share remote data may not benefit as much.

## 5. Memory locality

**Memory locality** is the tendency of programs to access the same or nearby memory locations repeatedly over a short period.

It improves performance by:
- Increasing cache hit rates.
- Reducing cache misses.
- Reducing average memory access time.
- Making demand paging more effective.
- Reducing unnecessary page faults when the active working set fits in RAM.

### Temporal locality (locality in time)

If a location is accessed now, it is likely to be accessed again soon.

Examples:
- A loop counter used repeatedly.
- Instructions in a frequently executed loop.
- A frequently accessed variable.

### Spatial locality (locality in space)

If a location is accessed, nearby locations are likely to be accessed soon.

Examples:
- Sequential array traversal.
- Executing adjacent instructions.
- Reading a file sequentially.

Caches exploit spatial locality by transferring a cache line rather than just one byte.

## 6. Page-table bits related to replacement

The NUMA/Clock slides review common page-table-entry bits:

- **Present/valid bit:** indicates whether a translation is currently present/usable under the architecture's semantics.
- **Accessed/use/reference bit:** hardware sets it when the page is accessed; OS can clear it to track subsequent references.
- **Dirty/modified bit:** indicates that the page has been written since it was loaded or since the bit was last cleared, according to the architecture.
- **Writable/read-only bit:** controls whether writes are permitted.
- **Page frame number:** identifies the physical frame.

The accessed/reference bit is particularly useful for approximating LRU. The dirty bit helps decide whether a victim must be written back before replacement.

## 7. Clock algorithm: exact procedure

Clock is also called **Second-Chance** replacement.

Each resident page has a reference bit \(R\). A circular pointer checks frames when a replacement is needed.

Algorithm:
1. On each page reference, hardware sets the page's \(R\) bit to 1.
2. When a page fault needs a victim and all frames are full, inspect the frame at the clock hand.
3. If \(R=1\), clear it to 0 and advance the hand.
4. If \(R=0\), choose that frame as the victim.
5. If the victim is dirty, save it to backing storage as required.
6. Load the requested page into that frame and update page-table/TLB state.
7. Set the new page's reference bit to 1 and advance/update the pointer according to the algorithm's convention.

Different teaching examples may initialize or advance the pointer at slightly different moments. Follow the convention stated in the question and apply it consistently.

## 8. Clock algorithm worked example from the notes

Reference string:

`7, 0, 1, 2, 0, 3, 0, 4`

Number of frames: 3.

The notes' frame order and reference-bit state can be summarized as follows. The pointer convention is the one used in the supplied notes.

| Step | Reference | Frame contents (clock order) | Reference bits | Result |
|---:|---:|---|---|---|
| 1 | 7 | 7, –, – | 1, –, – | Fault |
| 2 | 0 | 7, 0, – | 1, 1, – | Fault |
| 3 | 1 | 7, 0, 1 | 1, 1, 1 | Fault |
| 4 | 2 | 2, 0, 1 | 1, 0, 0 | Fault; 7 gets a second chance and is replaced |
| 5 | 0 | 2, 0, 1 | 1, 1, 0 | Hit |
| 6 | 3 | 2, 0, 3 | 1, 0, 1 | Fault; 1 is replaced |
| 7 | 0 | 2, 0, 3 | 1, 1, 1 | Hit |
| 8 | 4 | 4, 0, 3 | 1, 0, 0 | Fault; 2 is replaced after second chances |

Total page faults in this example:

\[
\boxed{6}
\]

Hits occur for references 0 at step 5 and 0 at step 7. All other references fault.

**How to solve any Clock problem:**
- Draw the frames in a circle.
- Write the reference bit next to each page.
- On a hit, set that page's bit to 1.
- On a fault with no free frame, follow the hand: clear 1s and advance; replace the first 0.
- Count faults carefully.

## 9. Reading the Clock hand's behavior

The slides make these observations:
- If the hand finds a victim quickly, few pages may have their reference bits set.
- If the hand moves through many frames, many pages have recently been referenced and may receive a second chance.
- If all reference bits are 1, the hand clears them on a pass and eventually finds a candidate.

The hand does not move with wall-clock time. It advances as the algorithm searches for a victim.

---

# Final revision sheet

## Core formulas

| Concept | Formula |
|---|---|
| TLB EAT, ignoring TLB time | \(EAT=hT_M+(1-h)(2T_M)\) |
| TLB EAT, including TLB time | \(EAT=h(T_T+T_M)+(1-h)(T_T+2T_M)\) |
| Number of virtual pages | Virtual address-space size ÷ page size |
| Flat page-table storage | Number of entries × bytes per entry |
| Simplified page-fault EAT | \(EAT=(1-p)T_M+pT_F\) |
| Proportional frame allocation | \(a_i=(s_i/\sum s_i)m\) |
| Total working-set demand | \(D=\sum WSS_i\) |

## Common distinctions

| Do not confuse | Correct distinction |
|---|---|
| TLB miss vs page fault | TLB miss means translation is uncached; page fault requires OS handling for the memory reference |
| Page vs frame | Page belongs to virtual memory; frame is a physical-RAM block |
| Clean vs dirty page | Dirty page has been modified and may need write-back |
| Demand paging vs COW | Demand paging loads absent pages; COW copies shared pages when a write requires separation |
| FIFO vs LRU | FIFO uses load age; LRU uses past access recency |
| Local vs global replacement | Local stays within a process's frames; global can take frames from the system-wide pool |
| Thrashing vs a normal page fault | Thrashing is sustained excessive paging that harms useful execution |
| Temporal vs spatial locality | Reuse over time vs access to nearby locations |
| NUMA local vs remote | Memory near the CPU node vs memory attached to another node |

## Page replacement: algorithm reminders

- **FIFO:** replace oldest loaded page.
- **OPT:** replace the page whose next use is farthest in the future.
- **LRU:** replace the page unused for the longest time in the past.
- **Clock:** clear reference bits of recently used pages and replace the first page found with bit 0.
- **Belady's anomaly:** possible with FIFO; not with OPT/LRU under their standard stack-algorithm definitions.

---

# Exam practice

## Short-answer questions

1. Define TLB, TLB hit, and TLB miss.
2. Explain the two-memory-access problem.
3. Calculate EAT with a given TLB hit ratio and access times.
4. What is an ASID and why is it useful?
5. Explain hierarchical, hashed, and inverted page tables.
6. What is virtual memory? Explain demand paging.
7. List the steps in page-fault handling.
8. Explain Copy-on-Write with a `fork()` example.
9. Compare FIFO, OPT, LRU, and Clock.
10. Define Belady's anomaly.
11. Compare equal and proportional frame allocation.
12. Differentiate local and global page replacement.
13. Define thrashing and explain the working-set model.
14. Explain Page-Fault Frequency (PFF) control.
15. Define NUMA and distinguish local from remote memory.
16. Explain temporal and spatial locality.
17. Describe Clock page replacement and the use/reference bit.

## Numerical questions to practise

1. TLB: RAM access = 80 ns, TLB lookup = 4 ns, hit ratio = 95%. Calculate EAT using the two-access miss model.
2. Page table: 36-bit virtual address, 4 KB pages, 8-byte entries. Find the number of virtual pages and flat page-table size.
3. Demand paging: normal memory access = 100 ns, page-fault service time = 6 ms, page-fault probability = \(10^{-5}\). Calculate simplified EAT.
4. Proportional allocation: 60 frames available; process sizes are 10, 20, and 30 units. Find ideal proportional shares before rounding.
5. Clock: use three frames and reference string `1, 2, 3, 2, 4, 1`. Show each frame state, reference bits, hits/faults, and final hand position. State your initial pointer convention.

## Final checklist

- [ ] Explain why a TLB is needed and distinguish TLB misses from page faults.
- [ ] Calculate EAT under the assumptions given.
- [ ] Explain page-table storage approaches.
- [ ] Describe demand-paging fault handling in the correct order.
- [ ] Explain how COW avoids unnecessary copies.
- [ ] Apply FIFO, OPT, LRU, and Clock to a reference string.
- [ ] Explain frame allocation, working sets, PFF, and thrashing.
- [ ] Explain NUMA, memory locality, and why local memory is faster.

