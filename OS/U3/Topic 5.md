# OS Unit 3 — Topic 3.5.1: Structure of Page Tables, TLB, EAT, Protection, and Page-Table Designs

> **Scope:** Notes based on the complete 39-slide presentation `3.5.1 Structure of page tables, Translation Lookaside Buffer (TLB), TLB Hits and Misses, Effective Access Time` and its matching Lecture 5 PDF notes. These notes combine the presentation and its companion notes, with explanations and worked calculations.

## Learning objectives

By the end, you should be able to:
- Explain why page tables are needed and where they are stored.
- Explain PTBR, PTLR, TLB, TLB hit/miss, and ASID.
- Trace address translation with and without a TLB.
- Calculate Effective Access Time (EAT).
- Explain page protection, valid/invalid bits, demand paging, and shared pages.
- Calculate page-table sizes and compare single-level, hierarchical, hashed, and inverted page tables.
- Explain huge pages and why they help TLB performance.

---

## 1. Quick recap: pages, frames, and address translation

Paging divides a process's **logical/virtual address space** into fixed-size **pages** and physical memory into same-sized **frames**.

- **Page:** fixed-size block in virtual memory.
- **Frame:** fixed-size block in physical RAM.
- **Page table:** maps each virtual page number (VPN) to a physical frame number (PFN).
- **Logical address:** consists of a page number and an offset.
- **Physical address:** consists of a frame number and the same offset.

For page size \(2^d\) bytes, the offset is \(d\) bits. The offset is not translated: it tells us the position *inside* the page/frame. The MMU translates the page number to a frame number and joins the frame number with the unchanged offset.

Example: page size = 4 KB = \(2^{12}\) bytes, so the offset is 12 bits.

## 2. Hardware support for page tables (slides 4–5)

### 2.1 Page table in CPU registers

One possible design stores page-table entries in dedicated CPU registers. Register access is very fast, but real page tables may contain thousands or millions of entries. Keeping the entire table in registers is therefore practical only for very small page tables.

### 2.2 Page table in main memory

Most systems keep page tables in RAM. Two special registers help the hardware find and bound the table:

- **PTBR — Page-Table Base Register:** contains the physical starting address of the current page table.
- **PTLR — Page-Table Length Register:** gives the size/number of entries in the page table, so the system can check whether a page-table index is in range.

### 2.3 The two-memory-access problem

Without a TLB, each ordinary memory reference can require two RAM accesses:

1. Access the page table in RAM to discover the physical frame number.
2. Access the actual instruction/data in that frame.

If one RAM access takes \(T_M\), then the basic cost is approximately \(2T_M\), ignoring other overheads. For example, if RAM takes 100 ns, a paged reference without a TLB takes about 200 ns.

**Why this matters:** address translation happens for every memory reference, so the extra access can substantially slow the system.

## 3. Translation Lookaside Buffer (TLB) (slides 4–8)

### 3.1 Definition

A **Translation Lookaside Buffer (TLB)** is a small, very fast hardware cache that stores recently used page-table mappings (typically VPN → PFN, plus protection/status information and sometimes an ASID).

It is usually implemented using associative lookup: hardware compares the requested page number against the entries in parallel. The presentation's companion notes give a typical range of about **64–1024 entries**; actual sizes vary by processor and may be split across levels.

### 3.2 Address translation with a TLB

1. CPU generates a virtual address: `(page number, offset)`.
2. MMU searches the TLB for the page number (and the matching process identifier, if ASIDs are used).
3. **TLB hit:** the frame number is found in the TLB.
4. **TLB miss:** the page-table entry must be fetched from the page table in RAM (or through a hardware/software page-table walk, depending on the architecture).
5. Once the frame number is known, combine it with the original offset to form the physical address.
6. Perform the requested memory access, subject to permissions and page-presence checks.

A **TLB miss is not automatically a page fault**:
- TLB miss = translation was not cached in the TLB.
- Page fault = the requested page is not currently resident, or the access violates a condition requiring OS handling.
A TLB miss can be resolved by reading a valid page-table entry without disk I/O.

### 3.3 TLB hit and miss

- **Hit:** requested mapping is in the TLB; translation is fast.
- **Miss:** mapping is absent; translation takes extra work to consult the page table and usually insert the mapping into the TLB for future references.

When the TLB is full, entries need replacement. Policies can include LRU-like or FIFO-like choices, although real hardware TLB replacement policies are architecture-specific. Some systems can pin/wire selected entries so they are not evicted.

### 3.4 Address-Space Identifiers (ASIDs)

An **ASID** identifies which process/address space owns a TLB entry. Two processes can use the same virtual page number but map it to different physical frames. The ASID prevents those mappings from being confused.

- **Without ASIDs:** the OS may need to flush TLB entries on a context switch to prevent one process using another process's translation.
- **With ASIDs:** entries for multiple processes can remain in the TLB, provided the ASID is checked during lookup. This reduces unnecessary flushes.

### 3.5 TLB versus CPU cache (slide 8)

| TLB | CPU cache |
|---|---|
| Caches address translations (VPN → PFN) | Caches data and/or instructions |
| Speeds up virtual-to-physical address translation | Reduces the latency of fetching data/instructions from RAM |
| Consulted as part of translation by the MMU | Consulted in the memory hierarchy for the requested data/instruction |

They solve different bottlenecks. A TLB hit does not mean the requested data is in the CPU cache; the data access can still miss in the CPU cache.

## 4. Effective Access Time (EAT) (slide 7 and problem slide 32)

EAT is the **average memory-access time**, weighted by how often each outcome occurs.

Let:
- \(h\) = TLB hit ratio, as a decimal
- \(1-h\) = TLB miss ratio
- \(T_T\) = TLB lookup time
- \(T_M\) = one main-memory access time

### 4.1 Simplified formula used in the slide's first examples

The slide assumes the hit path costs one memory access and the miss path costs two, and omits TLB lookup overhead:

\[
EAT = hT_M + (1-h)(2T_M)
\]

**Example A:** \(h=0.80\), \(T_M=10\text{ ns}\)

- Hit cost = 10 ns
- Miss cost = 20 ns
- \(EAT = 0.80(10)+0.20(20)=8+4=12\text{ ns}\)
- Compared with 10 ns, this is a 20% slowdown.

**Example B:** \(h=0.99\), \(T_M=10\text{ ns}\)

\[
EAT=0.99(10)+0.01(20)=9.9+0.2=10.1\text{ ns}
\]

This is only 1% slower than 10 ns. A high hit ratio is therefore important.

### 4.2 Formula when TLB lookup time is explicitly included

If every reference first spends \(T_T\) searching the TLB:

- Hit cost = \(T_T+T_M\)
- Miss cost = \(T_T+2T_M\), assuming a miss needs one page-table memory access and then one data memory access.

\[
EAT=h(T_T+T_M)+(1-h)(T_T+2T_M)
\]

Equivalent form:

\[
EAT=T_T+(2-h)T_M
\]

This formula assumes a single-level page-table lookup that takes one RAM access, no extra page-fault handling, and no additional cache or page-walk effects.

**Problem from slide 32:** \(T_M=80\text{ ns}\), \(T_T=5\text{ ns}\), hit ratio \(h=0.90\).

(a) Without TLB: two RAM accesses = \(2(80)=160\text{ ns}\).

(b) With TLB:
- Hit cost = \(5+80=85\text{ ns}\)
- Miss cost = \(5+80+80=165\text{ ns}\)
- \(EAT=0.90(85)+0.10(165)=76.5+16.5=93\text{ ns}\).

**Important exam habit:** check whether TLB lookup time is included. Use the formula matching the question's assumptions.

## 5. Memory protection and valid/invalid bits (slides 9–11)

### 5.1 Protection bits

Page-table entries can contain permissions such as:
- Read-only
- Read-write
- Execute-only (where supported)
- User/supervisor access and other architecture-specific controls

If a process tries an illegal operation—for example, writing to a read-only code page—the hardware raises a **trap/exception** so the operating system can handle it. Protection prevents processes from corrupting each other's memory and protects the OS.

### 5.2 Valid/invalid bit

A valid/invalid bit indicates whether a virtual page is part of the process's legal address space (as presented in the slides):

- **Valid (v):** the page belongs to the process's logical address space.
- **Invalid (i):** the page is not a legal page for that process.

An invalid access triggers a trap. The OS determines what happened and may terminate the process or handle the event.

**Terminology caution:** in practical systems, a “not-present” page and an “illegal address” are not always the same thing. Some textbook diagrams use valid/invalid for address-space legality; demand paging often uses a separate present/resident status. Do not assume every invalid bit simply means “load this page from disk.”

### 5.3 Connection to demand paging (slide 11)

Demand paging loads a page only when it is actually needed. The OS can arrange for a legal page not currently resident to cause a page fault, then load it into RAM, update the page table, and restart the instruction. This saves RAM when the program never touches all of its possible pages. The full page-fault procedure is covered in Topic 3.6.

## 6. Shared pages and reentrant code (slides 12–15)

Paging allows multiple processes to map their virtual pages to the **same physical frames**.

### 6.1 Worked example: 40 text-editor users

Each editor has:
- Code = 150 KB
- Private data = 50 KB

Without sharing, each user needs \(150+50=200\) KB:

\[
40(150+50)=40(200)=8000\text{ KB}
\]

With one shared copy of code and a private 50 KB data area per user:

\[
150+40(50)=150+2000=2150\text{ KB}
\]

Memory saved:

\[
8000-2150=5850\text{ KB}
\]

The shared code is used by all processes; each process keeps its own private data.

### 6.2 Reentrant code

**Reentrant code** can safely be executed by multiple processes/threads at the same time. For shared code, it should not modify itself during execution; it is normally mapped read-only. Each execution's changing state must be held separately (for example, in registers, stack, or private data).

Examples: shared libraries, text-editor code, compilers, and common system libraries.

Shared read-write pages can also support interprocess communication, but they require coordination and synchronization to avoid races and corruption.

## 7. Why page-table structure becomes a problem (slides 16–17)

A straightforward single-level page table has one entry for every virtual page. For large virtual address spaces, this can consume substantial memory—even if the process uses only a small fraction of its virtual address space.

### 7.1 Example: 32-bit virtual address, 4 KB pages

- Virtual address space: \(2^{32}\) bytes
- Page size: \(4\text{ KB}=2^{12}\) bytes
- Number of virtual pages/table entries:

\[
\frac{2^{32}}{2^{12}}=2^{20}=1,048,576\text{ entries}
\]

If each page-table entry is 4 bytes:

\[
2^{20}\times4=2^{22}\text{ bytes}=4\text{ MiB}
\]

So one process's conventional page table needs about **4 MiB**, assuming 4-byte entries. Multiple processes can multiply the overhead. The table may also be wasteful for sparse address spaces and can be awkward to allocate as one contiguous block.

The presentation covers three alternatives:
1. Hierarchical (multi-level) page tables
2. Hashed page tables
3. Inverted page tables

## 8. Hierarchical / multi-level page tables (slides 18–22)

### 8.1 Core idea

Instead of one huge table, split the page table into smaller tables. The outer table points to inner page tables. An inner page table is allocated only for a region of virtual address space that needs one. This is especially helpful for sparse address spaces.

The logical address becomes:

`[outer index p1 | inner index p2 | page offset d]`

- \(p_1\): selects an entry in the outer page table.
- \(p_2\): selects an entry in the selected inner page table.
- \(d\): byte position within the page.

This is called a **forward-mapped page table** in the slides because translation proceeds from the outer level inward.

### 8.2 Two-level example from slide 19

A 32-bit logical address with a 1 KB page size:

- Page size \(=2^{10}\) bytes → offset = 10 bits.
- Virtual page number bits \(=32-10=22\).
- The slide divides the 22-bit page number into:
  - \(p_1=12\) bits
  - \(p_2=10\) bits
- Full address: `[p1: 12 bits | p2: 10 bits | offset: 10 bits]`.

Why can the inner index be 10 bits? A 1 KB page table page with 1-byte entries would hold 1024 entries, but with common 4-byte entries, a 1 KB table page holds 256 entries (8 index bits). Therefore, treat the slide's split as its stated illustrative convention; when solving a numerical problem, use the entry size and table-page size explicitly rather than blindly copying the split.

### 8.3 How translation works

1. Use \(p_1\) to index the outer table.
2. The outer entry identifies the appropriate inner page table.
3. Use \(p_2\) to index that inner table.
4. Obtain the physical frame number.
5. Append the unchanged offset \(d\).

Without a TLB, a two-level page table can require two memory accesses for the page-table walk plus one for the actual data, i.e. three memory accesses. A TLB hit can avoid the page-table walk.

### 8.4 Why 64-bit systems need multiple levels (slides 21–22)

A naïve 64-bit virtual address space with 4 KB pages has:

\[
2^{64}/2^{12}=2^{52}\text{ virtual pages}
\]

A single-level table would have \(2^{52}\) entries, which is impractically large. Multi-level paging allows only the required lower-level tables to be allocated. Systems can add further levels (three, four, or five levels) as needed. The exact number of usable virtual-address bits and page-table levels depends on the CPU architecture and OS configuration.

**Trade-off:** multi-level tables save memory for sparse address spaces but can increase translation-walk memory accesses on a TLB miss.

## 9. Hashed page tables (slide 23 and notes)

Hashed page tables are useful for very large, often sparse address spaces.

General process:
1. Hash the virtual page number (often combined with a process/address-space identifier).
2. Use the hash result to find a bucket.
3. Compare entries in that bucket, which may form a chain.
4. Find the matching virtual page and obtain its physical frame number.

An entry commonly records the virtual page number, mapped frame number, and a pointer to the next entry in the chain.

**Advantages**
- Avoids maintaining a huge flat table for every possible virtual page.
- Can work well for sparse, large address spaces.

**Disadvantages**
- Hash collisions require checking additional entries.
- Performance depends on the hash function, table size, and collision-chain length.
- It does not eliminate the need for a TLB.

## 10. Inverted page tables (slides 24–26)

### 10.1 Core idea

A conventional page table has one entry per virtual page **for each process**. An **inverted page table** has one entry per physical frame in the system.

Each entry records which process/address space and virtual page currently occupy that physical frame.

| Conventional page table | Inverted page table |
|---|---|
| Entry count tied to virtual pages (per process) | Entry count tied to physical frames (system-wide) |
| Directly indexed using virtual page number | Search/hash lookup needed to find a matching `(process ID, virtual page)` |
| Can consume much memory for huge virtual spaces | Reduces page-table storage when virtual address spaces are huge |
| Naturally represents each process's virtual-page mappings | Shared mappings and lookup complexity need special handling |

### 10.2 Lookup

The system searches for the pair `(process ID, virtual page number)` in the inverted table, often using a hash table to narrow the search. A TLB caches recent translations to make common references fast.

**Trade-off:** memory use is reduced, but lookup is more complex. Without hashing or a TLB, searching can be slow.

### 10.3 Shared-memory difficulty

One physical frame can be mapped at multiple virtual addresses (possibly in different processes). But an inverted table normally has one entry for each physical frame and therefore only one primary record for that frame. This makes shared mappings more complicated than in conventional page tables. Systems need additional mapping structures or conventions to represent aliases correctly.

## 11. Huge pages and the TLB bottleneck (slide 27)

A TLB has a finite number of entries. With 4 KB pages, 1024 TLB entries cover:

\[
1024\times4\text{ KB}=4096\text{ KB}=4\text{ MiB}
\]

If a workload repeatedly accesses a much larger memory region, many translations may miss in the TLB.

**Huge pages** use larger page sizes, commonly 2 MiB or 1 GiB on supported Linux/x86-64 configurations. A single TLB entry can then map a much larger region.

For 1024 entries:
- 4 KB pages → 4 MiB of coverage
- 2 MB pages → 2 GiB of coverage
- 1 GB pages → 1 TiB of coverage

Benefits: fewer TLB misses and potentially less page-table overhead.
Costs/trade-offs: allocating contiguous large frames can be harder; internal fragmentation can increase; large-page use needs appropriate OS and hardware support.

## 12. Comparison of page-table designs (slide 28)

| Design | Main idea | Advantages | Limitations |
|---|---|---|---|
| Single-level page table | One entry per virtual page | Simple lookup/indexing | Can be very large |
| Hierarchical paging | Table divided into levels | Allocates lower tables only where needed; good for sparse spaces | Multiple memory references on a TLB miss |
| Hashed page table | Hash virtual page number to entry/bucket | Useful for large sparse address spaces | Collisions and hash-chain overhead |
| Inverted page table | One entry per physical frame | Table size depends on physical RAM | More complex lookup and shared-memory handling |
| Huge pages | Increase page size | More memory coverage per TLB entry; fewer translations | Potential fragmentation and allocation constraints |

The slide's comparison table also mentions segmentation and paged segmentation from earlier topics. Segmentation can suffer external fragmentation; paging avoids external fragmentation but may have internal fragmentation. Multi-level paging is fast to allocate in smaller pieces but may require several memory references on a TLB miss.

## 13. Worked numerical problems

### Problem 1 — EAT with TLB lookup time (slide 32)

Given:
- RAM access = 80 ns
- TLB lookup = 5 ns
- Hit ratio = 90%

(a) Without TLB: page-table access + data access:

\[
2(80)=160\text{ ns}
\]

(b) With TLB:
- Hit time \(=5+80=85\) ns
- Miss time \(=5+80+80=165\) ns

\[
EAT=0.90(85)+0.10(165)=76.5+16.5=\boxed{93\text{ ns}}
\]

### Problem 2 — Address space, frames, and page-table levels (slide 36)

Given:
- Physical memory = 512 MB \(=2^{29}\) bytes
- Logical address space = 2 GB \(=2^{31}\) bytes
- Page/frame size = 4 KB \(=2^{12}\) bytes

1. **Offset bits:** \(\log_2(4096)=\boxed{12}\) bits.
2. **Virtual page-number bits:** \(31-12=\boxed{19}\) bits.
3. **Physical frame-number bits:** \(29-12=\boxed{17}\) bits.
4. **Number of physical frames:** \(2^{29}/2^{12}=2^{17}=\boxed{131,072}\).
5. **Number of virtual pages:** \(2^{31}/2^{12}=2^{19}=\boxed{524,288}\).
6. **Number of paging levels:** the address-space/page size alone does not uniquely determine this; it depends on page-table entry size and how many entries fit in a page. If entries are 4 bytes, a 4 KB page holds \(4096/4=1024=2^{10}\) entries. One inner table maps \(2^{10}\) virtual pages; an outer table needs \(2^{19}/2^{10}=2^9=512\) entries. Therefore a **two-level** arrangement is sufficient under these assumptions.

### Problem 3 — Conventional versus inverted page table (slide 37)

Given:
- 32-bit logical address
- Page size = 4 KB \(=2^{12}\) bytes
- Physical memory = 512 MB \(=2^{29}\) bytes

(a) Conventional single-level page table entries:

\[
2^{32}/2^{12}=2^{20}=\boxed{1,048,576\text{ entries}}
\]

(b) Inverted page table entries = number of physical frames:

\[
2^{29}/2^{12}=2^{17}=\boxed{131,072\text{ entries}}
\]

If each conventional entry is 4 bytes, its size is \(2^{20}\times4=4\) MiB. If each inverted entry were also 4 bytes, its size would be 512 KiB, but real inverted entries typically need process IDs and virtual-page information, so they are larger than 4 bytes. The entry counts—not an assumed entry size—are what the question asks for.

### Problem 4 — Single-level table size

Given a 32-bit virtual address, 4 KB pages, and 4-byte entries:
- Offset bits = 12
- Virtual page-number bits = 20
- Entries = \(2^{20}\)
- Table size = \(2^{20}\times4=2^{22}\) bytes = **4 MiB**

### Problem 5 — Shared editor memory

40 users; each editor has 150 KB code and 50 KB private data.
- No sharing: \(40(150+50)=8000\) KB
- Shared code: \(150+40(50)=2150\) KB
- Saved: \(8000-2150=\boxed{5850\text{ KB}}\)

## 14. Slide-by-slide coverage checklist

- [x] Slides 1–3: course/topic introduction and credits (context only).
- [x] Slides 4–6: hardware support, PTBR/PTLR, two-access problem, TLB hardware translation.
- [x] Slide 7: hit ratio and EAT examples (80% and 99%).
- [x] Slide 8: TLB versus CPU cache.
- [x] Slides 9–11: protection bits, valid/invalid bits, relation to demand paging.
- [x] Slides 12–15: shared pages, reentrant code, 40-user example.
- [x] Slides 16–22: large page tables, hierarchical paging, two-level example, 64-bit address spaces and further levels.
- [x] Slides 23–26: hashed and inverted page tables, shared-memory issue.
- [x] Slide 27: Linux huge pages.
- [x] Slide 28: comparison of page-table designs.
- [x] Slides 30–32: conceptual questions and TLB EAT numerical.
- [x] Slides 33–35: problem slides (the text layer contains no readable problem statement; use the visible slide content in the original deck if these slides contain image-only problems).
- [x] Slides 36–37: address/frame/level calculation and conventional/inverted table entries.
- [x] Slides 29, 38–39: blank/closing slides.

## 15. Exam questions with short answers

1. **Page vs frame?** A page is a block of virtual memory; a frame is a same-sized block of physical RAM.
2. **Why is the offset unchanged?** It identifies a byte inside a page; the mapped frame has the same size, so the byte's within-page position remains the same.
3. **What is the two-memory-access problem?** Without a TLB, each reference may need one RAM access for the page table and another for the actual data/instruction.
4. **What does PTBR do?** Holds the starting physical address of the page table.
5. **What does PTLR do?** Gives the table's length/entry count and supports bounds checking.
6. **What is a TLB hit?** The requested translation is found in the TLB.
7. **Is a TLB miss a page fault?** Not necessarily; a miss can be resolved from a resident page-table entry.
8. **Why use an ASID?** To distinguish mappings from different processes and reduce the need to flush TLB entries on context switches.
9. **TLB versus CPU cache?** TLB caches translations; CPU cache stores data/instructions.
10. **What is EAT?** The weighted average of hit and miss access costs.
11. **What do protection bits do?** Restrict operations such as reading, writing, and executing a page.
12. **What do valid/invalid bits indicate in the lecture model?** Whether a virtual page belongs to the process's legal address space.
13. **Why share read-only code?** Many processes can use one physical copy while keeping separate private data.
14. **What is reentrant code?** Code safe to execute concurrently and that does not modify itself during execution.
15. **Why are multi-level tables useful?** They avoid allocating a giant flat table for unused regions of sparse address spaces.
16. **What is a hashed page table?** A hash-based lookup structure mapping virtual page identifiers to frames.
17. **What is an inverted page table?** A table with one entry per physical frame, recording which process and virtual page occupy it.
18. **Why use huge pages?** One TLB entry covers more memory, reducing TLB misses.
19. **What is the main cost of multi-level paging?** More page-table-walk accesses on a TLB miss.
20. **What is the main cost of inverted tables?** More complex lookup and shared-mapping handling.

## 16. Common mistakes to avoid

- Do not confuse a **TLB miss** with a **page fault**.
- Do not change the page offset during translation.
- Do not forget the TLB lookup time when the problem explicitly provides it.
- Convert hit percentages to decimals before substituting into EAT.
- A conventional page table's entry count is based on the number of **virtual pages**; an inverted table's entry count is based on **physical frames**.
- For multi-level paging, do not guess the number of levels without considering page size, entry size, and the number of virtual-page bits.
- Valid/invalid terminology varies by textbook and architecture. Distinguish address-space legality from whether a legal page is currently resident.
- Huge pages reduce translation pressure but can increase internal fragmentation or make physical allocation harder.

## 17. Formula sheet

| Quantity | Formula |
|---|---|
| Offset bits | \(\log_2(\text{page size in bytes})\) |
| Virtual page-number bits | Virtual-address bits − offset bits |
| Physical frame-number bits | Physical-address bits − offset bits |
| Number of virtual pages | Virtual address-space size / page size |
| Number of physical frames | Physical-memory size / frame size |
| Single-level table size | Number of virtual pages × bytes per PTE |
| Entries per page-table page | Page-table page size / bytes per PTE |
| Simplified EAT | \(hT_M+(1-h)(2T_M)\) |
| EAT including TLB lookup | \(h(T_T+T_M)+(1-h)(T_T+2T_M)\) |
| Shared-code memory | One code copy + private data for every process |

---

**End of Topic 3.5.1 notes.** Study the concepts first, then practise the EAT and page-table-size calculations without looking at the worked answers.
