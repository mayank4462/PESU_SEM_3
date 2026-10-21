# OS Unit 3 — Topic 3.8.1: NUMA, Memory Locality, and Clock Page Replacement

> These notes combine the presentation `3.8.1 NUMA (Non-Uniform Memory Access) Basics and Memory Locality, Clock Page Replacement` with its companion notes. They include core theory, algorithm steps, worked examples, and exam-oriented comparisons.

## Learning objectives
- Define NUMA and compare UMA and NUMA.
- Explain local and remote memory and NUMA-aware allocation.
- Explain temporal and spatial locality.
- Describe reference/use and dirty/modified bits.
- Trace Clock (second-chance) page replacement.

---

# 1. NUMA: Non-Uniform Memory Access

## 1.1 Definition

**NUMA (Non-Uniform Memory Access)** is a multiprocessor memory architecture in which memory-access time depends on where the requested memory is physically located.

A NUMA system is divided into nodes. A node typically contains one or more CPU cores or a processor socket, memory attached to that socket, and an interconnect to other nodes.

- **Local memory:** memory attached to the node running the CPU; generally faster to access.
- **Remote memory:** memory attached to another node; generally slower because the request travels over an interconnect.

All memory can still be accessible in a shared address space. Remote memory is not invalid; it simply has a different access cost.

## 1.2 Why NUMA is used

Large multiprocessor systems need high memory bandwidth. A single centralized memory controller can become a bottleneck as more CPUs are added. NUMA distributes memory controllers and memory among nodes, improving scalability and aggregate bandwidth.

The trade-off is non-uniform latency: local memory is usually faster than remote memory.

## 1.3 UMA versus NUMA

| Feature | UMA (Uniform Memory Access) | NUMA |
|---|---|---|
| Access time | Roughly uniform across CPUs | Depends on memory location |
| Memory organization | Shared memory with uniform access characteristics | Memory distributed among nodes |
| Local/remote distinction | Not central to the model | Important |
| Scalability | Centralized paths may become bottlenecks | Distributed controllers can scale better |
| OS placement | Less location-sensitive | Placement affects performance |

UMA is a simplified model; caches and other effects can still cause latency differences in real systems.

## 1.4 NUMA-aware OS decisions

A NUMA-aware OS tries to place data near the CPU that uses it. Techniques include:
- Allocating from the local node when possible.
- **First-touch placement:** a page may be allocated on the node of the CPU that first touches it, depending on OS policy.
- Keeping a thread and its frequently used pages close together.
- Migrating threads or pages when workload patterns change.
- Balancing local capacity against remote-access costs.

“Always allocate locally” is not a universal solution: local memory is finite, and balancing workloads can require remote memory.

## 1.5 Advantages and disadvantages

**Advantages**
- Better scalability for multiprocessor systems.
- Higher aggregate memory bandwidth through distributed controllers.
- Potentially faster local accesses.
- Less dependence on one centralized memory controller.

**Disadvantages**
- Remote accesses can be slower.
- Thread/page placement affects performance.
- OS scheduling and memory allocation are more complex.
- Poor placement can increase latency and interconnect traffic.

### Example

A thread runs on Node A and repeatedly reads an array:
- If the array is in Node A's memory, most accesses may be local.
- If the array is in Node B's memory, the thread may make many remote accesses.
- Moving the thread or migrating the pages may improve performance, depending on the rest of the workload.

**Exam definition:** NUMA is a multiprocessor architecture in which memory-access latency is non-uniform and depends on whether the requested memory is local or remote to the accessing processor.

---

# 2. Memory locality

Memory locality is the tendency of a program to access the same or nearby memory locations repeatedly. It helps explain why caches and page-replacement algorithms are effective.

## 2.1 Temporal locality

**Temporal locality:** a location used recently is likely to be used again soon.

Examples:
- A loop counter accessed on every iteration.
- Instructions inside a loop.
- A frequently used variable.
- A function called repeatedly.

## 2.2 Spatial locality

**Spatial locality:** locations near a referenced location are likely to be accessed soon.

Examples:
- Traversing an array sequentially.
- Reading adjacent instructions.
- Iterating over neighboring elements in a matrix row.

## 2.3 Why locality matters

A program often works within a small group of pages for a while. If these pages remain in RAM, later accesses avoid page faults. If they are repeatedly evicted and needed again, the process can suffer high fault rates or thrashing.

Locality explains:
- Why caches improve performance.
- Why recently used pages may be worth retaining.
- Why the working-set model can estimate the pages a process needs.

**Important distinction:** temporal/spatial locality describes a program's access pattern. NUMA locality describes the physical location of memory relative to a processor. They are related to performance but are not the same concept.

---

# 3. Page-table bits used by Clock

## 3.1 Reference/use bit

A **reference bit** (also called a use bit) indicates that a page has been referenced. Hardware may set it when the page is accessed.

- `R = 1`: the page has been referenced since the bit was last cleared.
- `R = 0`: the page has not been referenced during that interval.

The replacement mechanism can clear the bit during scanning so it can detect subsequent references.

## 3.2 Dirty/modified bit

A **dirty bit** indicates whether a page has been modified since it was loaded or last made clean.

- **Clean page:** may often be discarded or reloaded from its backing source without first saving modified contents.
- **Dirty page:** its modified contents generally need to be written back before the frame is reused.

The reference bit estimates recent activity; the dirty bit helps estimate write-back cost. They serve different purposes.

---

# 4. Clock page-replacement algorithm

## 4.1 Core idea

The **Clock algorithm**, also called the **second-chance algorithm**, is a practical approximation to LRU.

Frames are arranged conceptually in a circle. A pointer, or clock hand, indicates the next frame to examine. Each frame has a reference bit \(R\).

When a page fault requires replacement:

1. Examine the frame at the clock hand.
2. If its reference bit is `0`, select it as the victim.
3. If its reference bit is `1`, clear the bit to `0` and give the page a second chance.
4. Advance the hand and continue until a frame with bit `0` is found.
5. Replace that page with the demanded page.
6. Set the newly loaded page's reference bit to `1`.
7. Advance the hand according to the convention used by the problem.

When a resident page is referenced, its reference bit is set to `1`.

### Why “second chance”?

A page with `R=1` is not evicted immediately. Its bit is cleared and the page is skipped for that pass. If the page is referenced again, the bit becomes `1` again and it may receive another second chance.

## 4.2 Clock is not exact LRU

LRU tracks the precise recency order of page references. Clock uses only a reference bit and a moving hand. It is usually cheaper to implement, but it may choose a different victim from LRU.

## 4.3 Pointer conventions

Worked Clock examples can differ depending on whether the hand advances before or after replacement. For exams:
- State the initial hand position.
- Follow the diagram or convention in the question.
- Show each examined frame and reference bit.
- Mark when a `1` bit is cleared.
- State where the hand ends after replacement.

This makes the trace unambiguous.

---

# 5. Worked Clock example from the companion notes

Reference string:

`7, 0, 1, 2, 0, 3, 0, 4`

Number of frames: **3**.

Assumptions for this trace:
- Frames start empty.
- A newly loaded/referenced page gets `R=1`.
- On a fault with full frames, scan from the hand: clear `R=1` and advance; replace the first `R=0`.
- After replacement, advance the hand to the next frame.

| Reference | Frame contents after access | Result |
|---:|---|---|
| 7 | 7, –, – | Fault |
| 0 | 7, 0, – | Fault |
| 1 | 7, 0, 1 | Fault |
| 2 | A victim is selected after the hand clears reference bits | Fault |
| 0 | Depends on the exact victim from the prior scan | Hit or fault |
| 3 | Update frames and bits using the same clock-hand procedure | Depends on prior state |
| 0 | Check whether 0 remains resident | Hit or fault |
| 4 | Scan for the first frame with `R=0` | Fault |

**Counting note:** The companion notes report **6 page faults** for this reference string under their pointer convention. The exact frame-by-frame trace depends on the starting hand position and whether the hand advances before or after replacement. Use the source slide's diagram if it specifies that convention; do not mix pointer conventions within one solution.

## 5.1 Reliable method for solving any Clock problem

For every reference:
1. Check whether the page is already resident.
2. If yes, mark a hit and set its reference bit to 1.
3. If a frame is empty, load the page and set its bit to 1.
4. Otherwise scan from the clock hand.
5. For each frame with `R=1`, clear it and move forward.
6. Replace the first frame with `R=0`.
7. Set the new page's bit to 1 and update the hand.
8. Record the page fault.

Never count a hit as a fault.

---

# 6. Clock example from the slide deck

Reference string:

`1, 2, 3, 2, 4, 1`

Number of frames: **3**.

Method:
1. Start with three empty frames and the specified initial hand position.
2. Load 1, 2, and 3 into the empty frames; set each reference bit to 1.
3. Reference 2 again: it is a hit, so its bit remains/gets set to 1.
4. Reference 4: it faults. Scan from the clock hand, clearing bits of pages set to 1 until a frame with bit 0 is found. Replace that page with 4 and set its bit to 1.
5. Reference 1: check whether it remains in memory. If evicted, this is a fault; otherwise, it is a hit.
6. Count faults and show the frame contents, bits, and hand after each access.

The first three references fault, the second reference to page 2 hits, and reference 4 faults. The final access to page 1 depends on the victim chosen and the initial hand convention. Show the full trace rather than guessing the result.

---

# 7. Clock versus FIFO and LRU

| Algorithm | Replacement rule | Uses reference bit? | Main property |
|---|---|---:|---|
| FIFO | Oldest page loaded | No | Simple, but can show Belady's anomaly |
| LRU | Least recently used page | Tracks recency explicitly | Uses recent history, with tracking overhead |
| Clock / second chance | Scan circularly; clear `R=1`, replace first `R=0` | Yes | Efficient approximation to LRU |

Clock avoids maintaining an exact recency order. It is a compromise between FIFO's simplicity and LRU's more detailed access-history tracking.

---

# 8. Exam questions and answers

1. **What is NUMA?** A multiprocessor architecture where memory-access time depends on whether memory is local or remote to the accessing CPU.
2. **What is local memory?** Memory physically attached to the accessing processor's NUMA node.
3. **Why is remote memory slower?** Requests travel through an interconnect to another node's memory controller.
4. **What is NUMA's main advantage?** Better scalability and aggregate memory bandwidth in large multiprocessor systems.
5. **What is the main OS challenge in NUMA?** Keeping threads and frequently used pages close while balancing capacity and workload.
6. **Define temporal locality.** Recently used locations are likely to be used again soon.
7. **Define spatial locality.** Locations near a referenced location are likely to be referenced soon.
8. **What does the reference bit indicate?** Whether a page has been referenced since the bit was last cleared.
9. **What does the dirty bit indicate?** Whether a page has been modified and may need to be written back.
10. **Why is Clock called second chance?** A page with `R=1` has its bit cleared and is skipped instead of being immediately evicted.
11. **How does Clock select a victim?** It scans circularly until it finds a frame with reference bit 0.
12. **Is Clock identical to LRU?** No. It approximates LRU with a reference bit and a moving pointer.
13. **Why state the initial hand position?** The scan order can change the victim and final fault count.

---

# 9. Common mistakes

- Remote NUMA memory is accessible; it is usually slower, not invalid.
- Do not confuse NUMA locality with temporal or spatial locality.
- Do not confuse the reference bit with the dirty bit.
- A reference bit of 1 means the page gets a second chance; it is not immediately evicted.
- Clock does not always choose the least recently used page exactly.
- Track the hand explicitly; pointer conventions can change a trace.
- A newly loaded Clock page normally gets reference bit 1.
- Do not count a hit as a page fault.

---

# 10. Quick revision sheet

- **NUMA:** local and remote memory have different access times.
- **UMA:** memory access is modeled as uniform.
- **Temporal locality:** reuse of recently accessed locations.
- **Spatial locality:** access to nearby locations.
- **Reference bit:** approximates recent page activity.
- **Dirty bit:** tracks modifications and potential write-back cost.
- **Clock:** circular second-chance algorithm; clear reference bits until a zero bit is found.
- **Goal:** reduce page faults without maintaining exact LRU history.

**End of Topic 3.8.1 notes.**
