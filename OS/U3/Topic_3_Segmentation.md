---
title: "OS — Topic 3: Segmentation"
course: UE25CS242A
tags:
  - operating-systems
  - memory-management
  - segmentation
---

# Topic 3 — Segmentation

## 1. Definition

**Segmentation** divides a program into logical, variable-sized units called **segments**. Examples include code, procedures/functions, global and local data, stack, heap, arrays, objects, and symbol tables.

Each segment represents a meaningful part of a program. Segments can be placed at different physical-memory locations and do not need to be adjacent.

## 2. Segmentation versus contiguous allocation

In simple contiguous allocation, a process occupies one continuous physical-memory region. In segmentation, a process is split into multiple segments, and each segment can occupy a separate physical-memory region. Each individual segment is still stored contiguously.

## 3. Logical address format

A segmented logical address is written as `<s, d>`:

- `s` = segment number.
- `d` = offset within that segment.

Example: `<2, 50>` means offset 50 within segment 2. It is not itself a physical address.

## 4. Segment table

Each segment-table entry contains:

| Field | Meaning |
|---|---|
| Base | Starting physical address of the segment |
| Limit | Length of the segment |

Special registers:
- **STBR (Segment-Table Base Register):** physical starting address of the segment table.
- **STLR (Segment-Table Length Register):** number of segment-table entries.

A legal segment number satisfies `0 <= s < STLR`.

A valid offset satisfies `0 <= d < limit[s]`. The limit is a length, not the highest valid offset. If the limit is 100, valid offsets are 0 through 99.

## 5. Address translation

For logical address `<s, d>`:

1. Check that the segment number is valid: `s < STLR`.
2. Read the segment's base and limit from the segment table.
3. Check that `0 <= d < limit[s]`.
4. If valid, calculate `Physical address = base[s] + d`.
5. If the segment number or offset is invalid, hardware raises an exception/trap. Do not calculate a valid physical address for an invalid offset.

### Example

| Segment | Base | Limit |
|---:|---:|---:|
| 0 | 1000 | 400 |
| 1 | 2400 | 200 |
| 2 | 5000 | 600 |

Logical address `<1, 50>`:
- Segment 1 has base 2400 and limit 200.
- Offset check: `0 <= 50 < 200`, valid.
- Physical address = `2400 + 50 = 2450`.

Logical address `<2, 600>`:
- Segment 2 has limit 600.
- Offset 600 is invalid because valid offsets end at 599.
- Result: trap/invalid access.

## 6. Numerical example from the lecture

| Segment | Base | Limit |
|---:|---:|---:|
| 0 | 219 | 600 |
| 1 | 2300 | 14 |
| 2 | 90 | 100 |
| 3 | 1327 | 580 |
| 4 | 1952 | 96 |

Translate the following logical addresses:

### `<0, 430>`
- `430 < 600`, valid.
- Physical address = `219 + 430 = 649`.

### `<1, 10>`
- `10 < 14`, valid.
- Physical address = `2300 + 10 = 2310`.

### `<2, 500>`
- `500 >= 100`, invalid.
- Result: trap/segmentation fault; no valid physical address.

### `<3, 400>`
- `400 < 580`, valid.
- Physical address = `1327 + 400 = 1727`.

### `<4, 112>`
- `112 >= 96`, invalid.
- Result: trap/segmentation fault; no valid physical address.

| Logical address | Result |
|---|---:|
| `<0, 430>` | 649 |
| `<1, 10>` | 2310 |
| `<2, 500>` | Invalid |
| `<3, 400>` | 1727 |
| `<4, 112>` | Invalid |

**Exam method:** Look up base and limit → validate offset → add base and offset only if valid.

## 7. Protection

Segmentation permits logical parts of a program to be managed separately. A system can assign different permissions to different segments:

- Code segment: read and execute.
- Data segment: read and write.
- Stack segment: read and write.
- Shared library code: often read and execute.

The limit provides bounds protection. Permission checks are a separate mechanism and depend on hardware and OS implementation.

## 8. Sharing segments

Two processes can share a physical segment if their segment-table entries point to the same physical base address and permissions allow it. The segment numbers in the two processes do not need to match.

Example:
- Process A: segment 2 → physical base 8000.
- Process B: segment 4 → physical base 8000.

Both processes can access the same physical segment through their own logical addresses. Uses include shared libraries, shared data, and interprocess communication (IPC). Writable shared data may require synchronization.

## 9. External fragmentation

Because segments have variable sizes, allocation and deallocation can leave scattered free holes.

Example:
- Hole A = 100 KB
- Hole B = 200 KB
- Hole C = 150 KB
- Total free = 450 KB

A request for a 300 KB segment fails if no individual hole is at least 300 KB, despite 450 KB total free memory. This is **external fragmentation**. Compaction can combine holes but requires relocating segments and has a performance cost.

## 10. x86 and the flat memory model

Classic x86 segmentation uses registers such as:
- `CS` — Code Segment
- `DS` — Data Segment
- `SS` — Stack Segment

Modern 64-bit Linux on x86-64 generally uses a flat memory model for ordinary code and data addressing, with segment bases effectively zero for the main address spaces. Some special segmentation mechanisms remain, but ordinary virtual-memory translation relies primarily on paging.

## 11. Advantages and disadvantages

### Advantages
1. Logical organization matching program structure.
2. Segments can have different sizes and grow independently.
3. Supports bounds protection and separate permissions.
4. Can support shared code and data.

### Disadvantages
1. Variable-sized segments cause external fragmentation.
2. Segment-table management adds overhead.
3. Allocation can fail when total free memory is sufficient but no single hole is large enough.
4. Compaction, when used, is expensive.

## 12. Segmentation versus paging

| Feature | Segmentation | Paging |
|---|---|---|
| Unit | Variable-sized logical segment | Fixed-size page |
| Physical allocation unit | Segment occupies a contiguous region | Page occupies one frame |
| Logical address | Segment number + offset | Page number + offset |
| Mapping structure | Segment table | Page table |
| Main fragmentation issue | External fragmentation | Internal fragmentation may occur in final page |
| Programmer's logical view | Reflects program units | Usually transparent |

Paging is the next topic in this course sequence.

## 13. Exam questions

1. Define segmentation.
2. What are the two fields in a segmented logical address?
3. Define segment base and segment limit.
4. Explain STBR and STLR.
5. Translate a logical address using a given segment table.
6. When does segmentation raise a trap?
7. How can segmentation support protection and sharing?
8. Explain external fragmentation with an example.
9. Compare segmentation with paging.
10. What is meant by the flat memory model in modern x86-64 Linux?

## 14. Quick revision checklist

- [ ] Logical address is `<segment number, offset>`.
- [ ] Segment-table entry contains base and limit.
- [ ] STBR points to the segment table.
- [ ] STLR records the number of segment entries.
- [ ] Legal segment number: `s < STLR`.
- [ ] Valid offset: `0 <= d < limit`.
- [ ] Physical address: `base[s] + d`, only after checks pass.
- [ ] Segments can be shared by mapping multiple entries to the same physical segment.
- [ ] Main disadvantage: external fragmentation.
- [ ] Paging uses fixed-size pages and frames to avoid external fragmentation in frame allocation.
