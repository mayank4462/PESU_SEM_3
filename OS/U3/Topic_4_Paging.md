---
title: "OS — Topic 4: Paging"
course: UE25CS242A
tags:
  - operating-systems
  - memory-management
  - paging
  - page-tables
---

# Topic 3 — Paging

## 1. Why paging is needed

Contiguous allocation and segmentation can suffer from **external fragmentation**: enough free memory may exist in total, but it is split into separate holes and no single hole is large enough for a request.

Paging allows a process to occupy non-adjacent physical-memory locations by dividing logical memory and physical memory into fixed-size blocks.

## 2. Pages and frames

- **Page:** fixed-size block of logical/virtual memory.
- **Frame:** fixed-size block of physical RAM.
- Page size = frame size.
- A process's pages can be loaded into any available frames; they need not be contiguous.

The page size is defined by the hardware/architecture. The OS tracks free frames and allocates enough frames for the process's pages.

## 3. Page table and frame table

Each process has a **page table** mapping page numbers to physical frame numbers.

Example:

| Page number | Frame number |
|---:|---:|
| 0 | 5 |
| 1 | 6 |
| 2 | 1 |
| 3 | 2 |

A **frame table** tracks which physical frames are free or allocated. The OS maintains per-process page-table information and selects the appropriate translation state when dispatching a process.

## 4. Logical and physical address format

A logical address is split into:

`<p, d>`

- `p` = page number, used as an index into the page table.
- `d` = page offset, position inside that page.

The page-table entry gives frame number `f`. The offset is unchanged during translation.

`Physical address = (frame number × page size) + offset`

Equivalently, physical address = physical base address of the selected frame + offset.

## 5. Page-number and offset bits

If:
- Logical address space is `2^m` bytes.
- Page size is `2^n` bytes.

Then:
- The high-order `m − n` bits represent the page number.
- The low-order `n` bits represent the offset.

Number of pages in the logical address space = `2^(m−n)`.

Example: `m = 4`, `n = 2`.
- Logical address space = `2^4 = 16` bytes.
- Page size = `2^2 = 4` bytes.
- Page-number bits = `4 − 2 = 2`.
- Offset bits = `2`.
- Number of pages = `16/4 = 4`.

## 6. Numerical example from the lecture slides

Given:
- Logical address size `m = 4` bits.
- `n = 2`, so page size = `2^2 = 4` bytes.
- Logical address space = `2^4 = 16` bytes.
- Physical memory = 32 bytes = 8 frames.

Page table:

| Page `p` | Frame `f` |
|---:|---:|
| 0 | 5 |
| 1 | 6 |
| 2 | 1 |
| 3 | 2 |

Logical page ranges:

| Page | Logical addresses |
|---:|---|
| 0 | 0–3 |
| 1 | 4–7 |
| 2 | 8–11 |
| 3 | 12–15 |

### Example A: logical address 13

1. Page number: `p = floor(13/4) = 3`.
2. Offset: `d = 13 mod 4 = 1`.
3. Page 3 maps to frame 2.
4. Physical address = `2 × 4 + 1 = 9`.

**Answer: 9.**

### Example B: logical address 6

1. Page number: `p = floor(6/4) = 1`.
2. Offset: `d = 6 mod 4 = 2`.
3. Page 1 maps to frame 6.
4. Physical address = `6 × 4 + 2 = 26`.

**Answer: 26.**

### Bit method for address 13

13 in four-bit binary is `1101`. Split into two page bits and two offset bits:

`11 | 01` → page 3, offset 1.

Page 3 maps to frame 2 (`010` in three bits). Append the unchanged offset `01`:

`010 | 01` = binary `01001` = decimal 9.

## 7. Paging hardware and address translation steps

1. CPU generates logical address `<p, d>`.
2. Hardware uses `p` to index the process's page table.
3. The page-table entry supplies frame number `f`.
4. Hardware combines frame `f` with offset `d`.
5. Physical address = `f × page size + d`.
6. Memory is accessed at that physical address.

**Important:** the offset is not changed. The page number is translated into a frame number.

## 8. Free frames and allocation

The OS tracks physical frames in a frame table. When loading a process, it finds enough free frames, places the process's pages in them, and updates the page table. The pages may occupy non-adjacent frames.

Example: pages 0, 1, 2, 3 can map to frames 5, 6, 1, 2 respectively. The logical order remains the same because the page table records the mapping.

## 9. Context switching and TLB

A process has its own page table. On a process switch, the OS/hardware must use the correct translation information for the new process; managing that information contributes to context-switch overhead.

**TLB (Translation Lookaside Buffer):** a small, fast cache of recently used page-number-to-frame-number translations.

- **TLB hit:** translation is found in the TLB.
- **TLB miss:** system consults the page table/page-walk mechanism.
- The TLB speeds up translation but does not replace the page table.

## 10. Internal fragmentation calculation from the slides

Given:
- Page size = 2048 bytes.
- Process size = 72,766 bytes.

Divide:

`72,766 = 35 × 2048 + 1,086`

So the process needs 35 full pages plus one partially used page = **36 pages**.

Unused space in the final page:

`2048 − 1086 = 962 bytes`

**Internal fragmentation = 962 bytes.**

### Worst case

If a process requires one byte more than a whole number of pages, it needs an additional page that is almost entirely unused.

For page size `S`, maximum unused space in the final page is `S − 1` bytes.

## 11. Page-size trade-off

| Smaller pages | Larger pages |
|---|---|
| Lower potential internal fragmentation in the final page | Higher potential internal fragmentation in the final page |
| More pages per process | Fewer pages per process |
| More page-table entries and mapping overhead | Fewer page-table entries and mapping overhead |

Some systems support multiple page sizes. On Linux, `getconf PAGESIZE` can display the base page size used by the system.

## 12. Paging and fragmentation

- **External fragmentation:** Paging avoids external fragmentation in physical-frame allocation because frames are fixed-size.
- **Internal fragmentation:** Possible when a process does not completely fill its last page/frame.

Do not say that paging eliminates all fragmentation; it eliminates external fragmentation in the standard paging allocation model, but internal fragmentation can remain.

## 13. Paging versus segmentation

| Feature | Paging | Segmentation |
|---|---|---|
| Block size | Fixed | Variable |
| Logical unit | Page | Logical segment such as code/data/stack |
| Logical address | Page number + offset | Segment number + offset |
| Mapping structure | Page table | Segment table |
| Main fragmentation concern | Internal fragmentation | External fragmentation |
| Physical placement | Pages can occupy any frames | Segments can be placed separately but each segment needs a contiguous region |

## 14. Demand paging distinction

Paging describes the fixed-size mapping of pages to frames. **Demand paging** is a virtual-memory strategy in which a page is brought into RAM when it is needed, rather than loading every page in advance. The concepts are related but not identical.

## 15. Advantages and limitations

### Advantages
1. Avoids external fragmentation in physical-frame allocation.
2. Supports non-contiguous placement of a process's pages.
3. Fixed-size frames simplify physical-memory allocation.
4. Provides the foundation for virtual memory and demand paging.

### Limitations
1. Internal fragmentation can occur in the last page.
2. Page tables consume memory.
3. Translation requires page-table lookup; TLBs reduce this overhead.
4. The OS must track free frames and per-process page tables.

## 16. Exam questions

1. Define page and frame. Why must their sizes be equal?
2. Explain the purpose of a page table and a frame table.
3. Explain paging address translation with a diagram.
4. A system has 16-bit logical addresses and page size 1 KB. How many bits are used for the offset and page number? *(Assume byte-addressable memory.)*
5. Given a page table and a logical address, calculate the physical address.
6. Why does paging avoid external fragmentation but still cause internal fragmentation?
7. Calculate internal fragmentation for a process of 72,766 bytes with page size 2048 bytes.
8. Explain the page-size trade-off.
9. What is a TLB? Explain TLB hit and miss.
10. Why can page-table management add context-switch overhead?
11. Differentiate paging and segmentation.
12. Distinguish paging from demand paging.

## 17. Quick revision checklist

- [ ] Page = logical-memory block; frame = physical-memory block.
- [ ] Page size equals frame size.
- [ ] Logical address = `<page number, offset>`.
- [ ] Physical address = `frame number × page size + offset`.
- [ ] For `2^m` logical address space and `2^n` page size, page bits = `m − n`; offset bits = `n`.
- [ ] Page table maps pages to frames; frame table tracks free/allocated frames.
- [ ] Paging avoids external fragmentation but may cause internal fragmentation.
- [ ] TLB caches recent translations.
- [ ] Demand paging loads pages when needed.
