# OS Unit 3 — Topic 3.6.1: Virtual Memory, Demand Paging, Page-Fault Handling, Copy-on-Write

> Companion materials: the 37-slide presentation `3.6.1 Virtual Memory – Demand Paging, Page-Fault Handling, Copy-on-Write` and its matching Lecture 6 notes PDF. These notes follow the presentation's sequence and combine it with the notes' explanations. They also include worked calculations and exam questions.

## Learning outcomes

You should be able to:
- Define virtual memory and explain why it is useful.
- Describe a process's virtual address space, holes, stack, heap, and shared libraries.
- Explain demand paging and pure demand paging.
- Distinguish a legal but nonresident page from an illegal reference.
- Describe the full page-fault handling sequence.
- Explain locality of reference and instruction restart.
- Calculate effective access time (EAT) when page faults occur.
- Explain file-backed versus anonymous memory and compressed memory.
- Explain Copy-on-Write (COW), including its hardware mechanism.
- Explain why page replacement is required, the role of the dirty bit, and Linux's background reclaim concept.

---

# 1. Virtual memory: the central idea

**Virtual memory** is a memory-management technique that lets a process execute without requiring its entire program to be loaded into physical RAM at once. The process sees a large logical/virtual address space; the operating system and MMU map the portions currently needed to physical memory.

### Why do we need it?

Programs often contain code or data that is not used during every run:
- Error-handling routines
- Debugging functions
- Rarely used modules
- Large data structures that are only occasionally accessed

Loading all of this into RAM at startup wastes memory and I/O. Virtual memory loads only the parts needed now.

### Main advantages
1. **Programs can be larger than RAM.** Only the currently required pages need to be resident.
2. **Lower memory use per process.** Unused portions do not occupy physical frames.
3. **Less I/O and faster startup.** The OS avoids loading pages that are never used.
4. **More concurrent processes.** More processes can fit in RAM, potentially improving CPU utilization and throughput.
5. **Sharing.** Processes can map the same physical pages for libraries or interprocess communication.
6. **Protection and isolation.** Each process has its own virtual address space and access permissions.

Virtual memory is an abstraction; it does not mean the computer has unlimited physical RAM. If working sets exceed RAM badly, paging can become expensive.

## 2. Virtual and physical memory (slides 4–6)

- **Virtual/logical address space:** the memory view seen by a process. It often appears to begin at address 0 and contain a continuous range of addresses.
- **Physical memory:** actual RAM, divided into page frames when paging is used.
- **MMU (Memory Management Unit):** hardware that translates virtual addresses to physical addresses using page tables and related structures.

Virtual memory can be implemented using demand paging or demand segmentation. Most general-purpose modern systems rely heavily on paging.

The essential separation is that a process uses virtual addresses; the OS/MMU determines where the corresponding data currently resides in RAM.

## 3. Virtual address-space layout (slide 7)

A typical process address space may include:
- Program code/text
- Global/static data
- Heap
- Memory-mapped regions and shared libraries
- Stack

The **heap usually grows upward** as dynamically allocated memory is requested. The **stack usually grows downward** as function calls, local variables, and saved execution state require space. The unused region between them is a **hole**.

Why is this useful? The heap and stack can grow into the unused region without the OS reserving physical RAM for the entire gap in advance. This supports a sparse address space: large virtual ranges may be reserved or available even though only some pages are physically backed.

Other benefits of a sparse virtual address space:
- Dynamic heap growth
- Stack growth
- Dynamically linked libraries
- Memory-mapped files
- Shared memory

A hole is unused virtual address space; it is not automatically a block of physical RAM sitting unused for that process.

## 4. Shared libraries and shared memory (slides 8 and 7)

If many processes use the same library, the OS can map the same physical code pages into each process's virtual address space. There need not be one separate physical copy per process.

For safe sharing, common library code is typically mapped read-only. Each process still has its own private data and execution state. Writable shared memory can also be mapped into multiple processes for interprocess communication, but processes must coordinate access.

**Key distinction:** each process can have a different virtual address that maps to the same physical frame.

---

# 5. Demand paging (slides 9–11)

**Demand paging** loads a page into RAM only when the process actually references it. It avoids loading the entire process at startup.

A component that manages moving pages into memory is called a **pager**. A “lazy swapper” is a useful textbook phrase for the idea that a page is not brought into RAM until it is needed.

### Two possible outcomes when a page is referenced

**Case A — The page is resident in RAM**
- The page-table entry says it is present.
- Address translation proceeds.
- The instruction continues normally.

**Case B — The page is legal but not resident**
- The MMU detects that the page is not present.
- Hardware raises a page-fault exception/trap.
- The OS brings the page into RAM, updates the mapping, and restarts the instruction.

An illegal reference is different: if the address is outside the process's permitted address space or violates access permissions, the OS should not simply load a page for it; the process may be terminated or receive a fault signal.

### Advantages of demand paging
- Less disk I/O at startup
- Lower RAM consumption
- Faster initial response
- More processes can coexist in memory
- Pages that are never referenced are never loaded

### Demand paging versus loading the whole process

| Load everything at startup | Demand paging |
|---|---|
| Reads all program pages whether needed or not | Loads pages as they are referenced |
| More initial I/O and memory use | Lower initial I/O and memory use |
| Startup may be slower | Startup can be faster |
| Simple mental model | Page faults add runtime overhead |

---

# 6. Valid/invalid or present-status bit (slides 12–13)

The presentation uses a valid–invalid bit and describes:
- `v` = in memory / resident
- `i` = not in memory / nonresident

With that convention, an entry marked `i` causes a page fault when referenced. The OS checks another data structure to determine whether the reference is legal and where the page is stored.

**Terminology caution for exams:** textbook terminology varies. Some books use valid/invalid to mean “inside/outside the process's legal address space”; demand-paging implementations also need to represent whether a legal page is currently present in RAM. In practical systems, legality and residency are conceptually separate questions. Follow the convention explicitly stated in your lecture slides, and explain the distinction if the question asks about it.

The page table and supporting OS data structures together let the system answer:
1. Is this virtual address legal for this process?
2. Is the page currently resident?
3. If it is not resident, where can its contents be obtained?
4. Does the process have permission to perform this access?

---

# 7. Steps in handling a page fault (slides 14–15)

This is one of the most important exam sequences.

1. **CPU references a page.** The MMU discovers that the page is not resident and raises a page-fault trap.
2. **Save process state.** The OS preserves the registers and execution state needed to resume the process.
3. **Check whether the reference is legal.** The OS consults the page table and other memory-management information.
4. **If illegal:** reject the access—often by terminating the process or delivering a fault signal.
5. **If legal but nonresident:** locate the page on backing storage (or another valid source).
6. **Find a free frame.** If a free frame exists, use it. If not, page replacement may be required (Topic 3.7).
7. **Read the page into RAM.** The I/O operation can take a long time, so the faulting process may be blocked while the CPU runs another process.
8. **Handle I/O completion.** The device signals completion; the OS resumes the relevant kernel work.
9. **Update the page table/frame information.** Mark the page resident/present and record the physical frame; update related status bits and TLB state as required.
10. **Restart the faulting instruction.** The instruction is re-executed so the process behaves as though the page had been available.

### Flow in one line

`Reference → page fault → check legality → locate page → obtain frame → read page → update tables → restart instruction`

Do not confuse a page fault with an illegal access. A legal nonresident page is recoverable; an illegal reference generally is not.

---

# 8. Major and minor page faults (slide 16)

The slide distinguishes two broad categories:

- **Major (hard) page fault:** the required page must be fetched from disk/backing storage. This is expensive because storage I/O is much slower than RAM.
- **Minor (soft) page fault:** the fault can be resolved without reading the page from disk—for example, by establishing a mapping to a page already in RAM or resolving certain memory-management conditions.

The precise definition varies across operating systems and monitoring tools. The important performance distinction is whether expensive storage I/O is needed. A minor fault still costs CPU/OS work, but is generally much faster than a major fault.

---

# 9. Pure demand paging and locality of reference (slide 17)

**Pure demand paging** is the extreme case where a process starts with no pages loaded into RAM. The first instruction fetch causes a page fault; after that page is loaded, execution proceeds until another nonresident page is referenced.

One instruction may touch multiple pages. For example, an instruction that reads two operands and writes a result can require the instruction page, operand pages, and destination page to be resident. It may cause more than one fault as these pages are encountered.

Why does pure demand paging not necessarily cause a fault on every access forever? Because of **locality of reference**.

### Locality of reference

Programs tend to access a limited group of pages repeatedly for a period of time.

- **Temporal locality:** a recently used location is likely to be used again soon.
- **Spatial locality:** locations near a recently used location are likely to be accessed soon.

A program may move between localities during execution. Once the pages in its current locality are loaded, the page-fault rate often decreases. If the process's actively used pages cannot fit in RAM, however, faults may remain frequent.

### Hardware/OS support for demand paging
- Page tables with status information to identify resident/nonresident pages
- Secondary storage or another backing source for pages not in RAM
- A page-fault trap mechanism
- Ability to restart the faulting instruction safely

---

# 10. Instruction restart (slide 18)

After the OS resolves a page fault, it normally restarts the instruction that caused the fault. This is straightforward only if the instruction has not partially changed visible state.

### Why it can be difficult

Imagine a complex instruction that copies a block of bytes from one memory region to another. The source and destination may overlap or cross a page boundary. If the instruction modifies part of the destination and then faults on a later page, blindly restarting the entire instruction might copy data incorrectly because some source bytes have already been overwritten.

The slide describes two hardware strategies:
1. **Check the ends of both blocks before modifying memory.** The microcode attempts to access both ends of source and destination first, so a fault can be detected before any changes occur.
2. **Use temporary registers to save overwritten values.** If a fault occurs, restore old values before trapping to the OS.

The architectural requirement is that the interrupted operation can be resumed without corrupting program-visible state.

---

# 11. Page-fault service time and EAT (slides 19–22)

Page-fault handling is expensive because it may involve a storage read, scheduling, and context switches.

### Worst-case stages in the slide

1. Trap to the OS.
2. Save registers and process state.
3. Determine that the trap is a page fault.
4. Check that the reference is legal and locate the page.
5. Queue for storage access if necessary.
6. Wait for device service, seek/latency, and transfer.
7. While I/O is pending, run another process.
8. Receive the I/O-completion interrupt.
9. Update page tables and related structures.
10. Wait until the faulting process is scheduled again.
11. Restore its state and restart the instruction.

The slide groups these into three major activities:
- **Service the interrupt:** relatively small CPU overhead
- **Read the page:** usually the dominant cost
- **Restart the process:** relatively small CPU overhead

### Page-fault rate

Let \(p\) be the probability that a memory reference causes a page fault.

- \(p=0\): no page faults
- \(p=1\): every reference causes a page fault

Let:
- \(T_M\) = ordinary memory access time
- \(T_P\) = average page-fault service time

Then:

\[
EAT=(1-p)T_M+pT_P
\]

This assumes the stated page-fault service time includes the relevant fault overhead. Always use consistent units.

### Worked example from the PPT

Given:
- Memory access time = 200 ns
- Average page-fault service time = 8 ms

Convert milliseconds to nanoseconds:

\[
8\text{ ms}=8,000,000\text{ ns}
\]

Therefore:

\[
EAT=(1-p)(200)+p(8,000,000)
\]

Expand:

\[
EAT=200-200p+8,000,000p
\]

\[
\boxed{EAT=200+7,999,800p\text{ ns}}
\]

If one access out of 1,000 faults, \(p=0.001\):

\[
EAT=200+7,999,800(0.001)
\]

\[
EAT=200+7,999.8=8,199.8\text{ ns}
\]

Approximately:

\[
\boxed{EAT\approx8,200\text{ ns}=8.2\ \mu s}
\]

Slowdown relative to 200 ns:

\[
\frac{8,200}{200}\approx\boxed{41\times}
\]

The slide rounds this to about a 40-fold slowdown. The lesson is that even a seemingly small page-fault rate can severely affect performance.

### Keep degradation below 10%

A 10% slowdown means EAT must be below:

\[
1.10(200)=220\text{ ns}
\]

Set:

\[
200+7,999,800p<220
\]

\[
7,999,800p<20
\]

\[
p<\frac{20}{7,999,800}\approx2.5\times10^{-6}
\]

That is approximately fewer than **one page fault per 400,000 memory references**.

**Exam tip:** page-fault service time is often measured in milliseconds, while RAM access is measured in nanoseconds. Convert units before calculating.

---

# 12. Demand-paging optimizations and backing storage (slide 23 and notes)

Pages not currently in RAM may come from different sources.

### File-backed pages
These are associated with a file, such as executable code or a memory-mapped file. Clean file-backed pages can often be discarded and read from the file again when needed, rather than being written to swap.

### Anonymous memory
Anonymous pages are not backed by a regular file. Examples include much of a process's heap and stack. If such a page has been modified and must be evicted while preserving its contents, the OS generally needs swap space or another backing mechanism.

### Dirty/modified pages
A page is **dirty** if it has been modified since it was read from its backing store or otherwise made clean. A dirty page may need to be written back before its frame can be reused.

### Swap space
Swap space is storage reserved for holding some pages that cannot remain in RAM. Its speed and configuration vary by OS and device. Do not memorize “always equal to double RAM” as a universal rule; that is not a fixed law.

### Compressed memory in mobile systems
The slide notes that modern Android and Apple systems may compress inactive pages in RAM instead of immediately writing them to disk. Android systems commonly use zRAM. Compression spends CPU cycles but may be faster than storage I/O and can increase the amount of effective memory available.

---

# 13. Copy-on-Write (COW) (slides 24–25)

**Copy-on-Write** lets two processes initially share the same physical pages and creates a separate copy only when one process attempts to modify a shared page.

It is commonly used when a process creates a child with `fork()`.

### Without COW
A naïve process-creation strategy would copy all the parent's pages into the child's address space immediately. This takes time and memory—even if the child immediately replaces its address space or never modifies most of the inherited pages.

### With COW
1. Parent and child initially map the same physical pages.
2. The OS marks the shared pages read-only in both processes' page tables.
3. If either process only reads a shared page, no copy is needed.
4. If one process writes to a COW page, the MMU raises a protection fault.
5. The OS recognizes that this is an intentional COW event, allocates a new physical frame, and copies the old page's contents into it.
6. The writing process's page-table entry is changed to point to the new frame and is made writable.
7. The faulting write instruction is restarted.
8. The other process continues to use the original frame.

### Before and after Process 1 modifies Page C

**Before the write:**

`Process 1 ─┐`
`          ├──> Shared physical Page C (read-only mapping)`
`Process 2 ─┘`

**After Process 1 writes:**

`Process 1 ───> New private copy of Page C (read-write)`
`Process 2 ───> Original Page C`

Only the modified page is copied; other pages remain shared.

### Why COW improves performance
- Faster process creation
- Less memory copied
- Avoids copying pages the child never changes
- Useful when a child process quickly calls `exec()` to load a different program

### Zero-fill-on-demand pages
The slide also mentions a pool of free zero-fill-on-demand frames. When a new page must be allocated, the OS can provide a frame cleared to zeros, preventing a new process from seeing data left by another process. Maintaining a pool of free frames reduces delay during faults. The OS should not have to perform every possible cleanup operation synchronously at the exact moment a process faults.

---

# 14. What if there is no free frame? Page replacement (slides 26–31)

Demand paging assumes that a needed page can be brought into RAM. But physical memory is finite. Frames are used by process pages, the kernel, I/O buffers, and other system needs.

If no free frame is available, the OS may need **page replacement**:
- Select a page currently in RAM that can be evicted.
- Free its frame.
- Load the demanded page into that frame.
- Update the relevant tables and restart the instruction.

The selected page is called the **victim page**; the physical frame it occupied is the **victim frame**.

The page-replacement algorithm should minimize the number of page faults, because each fault can be expensive. A page may be loaded, evicted, and later loaded again if the process references it again.

### Basic page-replacement sequence

1. Locate the required page on backing storage.
2. Check whether a free frame exists.
3. If there is a free frame, use it.
4. If not, choose a victim frame.
5. If the victim page is dirty, write it back to its backing store.
6. Load the required page into the freed frame.
7. Update the page table and frame-table information.
8. Invalidate or update any stale TLB translation as needed.
9. Restart the instruction that caused the fault.

### Dirty bit / modify bit

- **Clean page:** has not been modified; it may be possible to discard it without writing it back.
- **Dirty page:** has been modified; its new contents must be preserved, usually by writing it to backing storage before reuse.

The dirty bit helps avoid unnecessary writes. If the victim page is dirty, a fault can require two transfers: write the victim out and read the requested page in. That can significantly increase page-fault service time.

### Why replacement completes the virtual-memory model

Virtual memory lets the process's logical address space be larger than the available physical memory. Page replacement lets the OS reuse physical frames as the process's active pages change over time. The virtual address space remains stable while the physical frames backing its pages can change.

The slide includes two “need for page replacement” diagrams. Their conceptual point is that when all frames are occupied and a fault demands a new page, the OS must evict a resident page unless it can otherwise obtain a frame.

---

# 15. Linux angle: proactive frame reclaim (slides 32–33)

A system should not wait until there are literally zero free frames before beginning to reclaim memory. That would force a faulting process to wait while the OS finds a victim and potentially writes it out.

The slides describe Linux's background reclaim thread, **`kswapd`**:
- The OS maintains free-memory watermarks, including low and high thresholds.
- When free memory falls below a low watermark, `kswapd` can wake in the background.
- It reclaims suitable inactive pages and attempts to build up free frames.
- This makes it more likely that a later memory request can be serviced promptly.

The practical idea is **proactive memory reclamation**, rather than waiting for every allocation to fail before responding. Exact Linux reclaim details depend on kernel version and memory conditions, but the slide's core principle is that background reclaim reduces synchronous pressure on the process that needs memory.

---

# 16. Questions from the PPT — answer key

## Slide 35: Virtual memory and demand paging

1. **What is virtual memory?** A technique that lets a process execute without all of its pages being resident in RAM at once.
2. **Why do stack and heap grow from opposite ends?** To use the virtual address space flexibly and allow both to grow into the unused region between them.
3. **What is pure demand paging?** Starting a process with no pages loaded and bringing pages in only when first referenced.
4. **How does the MMU know whether a page is resident?** It checks page-table status/presence information, with OS metadata used to determine where a nonresident page can be obtained.
5. **What happens when a page is marked `i` in the slide's convention?** The MMU raises a page-fault trap; the OS checks whether the reference is legal and resolves it if appropriate.
6. **Why is a minor fault faster than a major fault?** A minor fault can be resolved without a disk read; a major fault requires backing-storage I/O.
7. **Why is instruction restart difficult?** An instruction may have partially modified memory before the fault, so blindly re-executing it could corrupt state.
8. **File-backed versus anonymous memory?** File-backed pages correspond to a file; anonymous pages, such as typical heap/stack pages, do not have a regular file as their backing source.
9. **Why use compressed memory on mobile systems?** Compression in RAM can be cheaper/faster than writing inactive pages to storage.

## Slide 36: COW and page replacement

1. **What permission is applied to shared COW pages after `fork()`?** Read-only, so a write triggers a protection fault that the OS handles by copying the page.
2. **Why is page replacement needed?** A page fault may occur when no free physical frame is available.
3. **What is a victim frame?** The frame selected to be reused for the demanded page.
4. **What is the dirty bit for?** It records whether the page was modified and may need to be written back.
5. **Why can a dirty victim increase fault time?** The OS may need to write the victim page out and read the demanded page in.
6. **What happens to the evicted page's page-table entry?** It must no longer claim the page is resident in that frame; the OS records its nonresident/backing status.
7. **Does Linux wait for zero free frames?** No. The slide describes background reclaim using `kswapd` and watermarks.

---

# 17. Quick formula sheet

| Concept | Formula / rule |
|---|---|
| Page-fault probability | \(0\le p\le1\) |
| Effective access time | \(EAT=(1-p)T_M+pT_P\) |
| Page-fault service time conversion | \(1\text{ ms}=1,000,000\text{ ns}\) |
| 10% maximum degradation target | \(EAT<1.10T_M\) |
| Dirty victim | Usually must be written back before frame reuse |
| Clean file-backed victim | May often be discarded and reread from file |
| COW write | Protection fault → allocate/copy page → update mapping → restart |

---

# 18. Common exam mistakes

- A page fault is not always an illegal memory access. Legal nonresident pages are loaded; illegal references are rejected.
- Do not say virtual memory makes physical RAM unlimited. It provides a larger address-space abstraction and relies on backing storage.
- Do not confuse the page-fault rate \(p\) with the TLB miss ratio.
- Convert ms to ns before substituting into the EAT formula.
- A minor page fault can still require OS work; it is “minor” because disk I/O is not required.
- COW does not copy every page immediately. It copies only a page that is written.
- In COW, the initial read-only protection is deliberate; it causes the OS to intercept a write.
- A clean page can often be discarded; a dirty page's modified contents must be preserved.
- If there is no free frame, a page fault can require both a victim write and a requested-page read.
- `kswapd` is a background-reclaim mechanism; the core concept is proactive maintenance of free frames.

---

## Completion checklist

- [x] Slides 1–3: topic title and source credits
- [x] Slides 4–6: virtual memory background and logical/physical separation
- [x] Slides 7–8: address-space layout, stack/heap, holes, shared libraries
- [x] Slides 9–11: demand paging and motivation
- [x] Slides 12–13: valid/invalid or residency status
- [x] Slides 14–15: page-fault handling
- [x] Slide 16: major and minor faults
- [x] Slide 17: pure demand paging, locality, hardware requirements
- [x] Slide 18: instruction restart and partially completed instructions
- [x] Slides 19–22: page-fault service stages, EAT, numerical example, performance target
- [x] Slide 23 and notes: swap, anonymous/file-backed pages, compressed memory
- [x] Slides 24–25: COW mechanism and before/after behavior
- [x] Slides 26–31: no-free-frame problem, page replacement, dirty bit, victim frame
- [x] Slides 32–33: Linux `kswapd` and watermarks
- [x] Slides 35–36: all listed review questions answered
- [x] Slide 37: closing slide

**End of Topic 3.6.1.** Learn the page-fault sequence and the EAT calculation first; then practise explaining COW and page replacement in your own words.
