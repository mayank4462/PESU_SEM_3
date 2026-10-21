# OS Unit 3 — Topic 3.7.1: Page Replacement, Frame Allocation, and Thrashing

> Source coverage: 20-slide presentation `3.7.1 Page replacement, Allocation of frames, Thrashing` plus its 7-page companion Lecture 39 notes. These notes cover the slide order and include the additional material from the notes: minimum frames, major/minor faults, kernel page reaper, NUMA context, locality, working-set model, PFF, and thrashing prevention.

## Learning objectives

- Explain why page replacement is needed and how algorithms are evaluated.
- Trace FIFO, Optimal, and LRU page-replacement algorithms.
- Count page faults for a reference string and a specified number of frames.
- Explain Belady's anomaly and stack algorithms.
- Compare counter and stack implementations of LRU.
- Explain frame allocation, minimum frames, equal allocation, and proportional allocation.
- Compare global and local replacement.
- Define thrashing, its causes, symptoms, and feedback loop.
- Explain locality, the working-set model, and page-fault frequency (PFF).
- Describe background memory reclaim and why the OS may reduce multiprogramming.

---

# 1. Why page replacement is needed

Demand paging brings a page into RAM only when it is referenced. But RAM has a finite number of frames. Eventually, a page fault may occur when no free frame is available.

The OS then needs to decide:
1. **Which page/frame should be replaced?** This is the job of the page-replacement algorithm.
2. **How many frames should each process receive?** This is the job of the frame-allocation algorithm.

The primary goal is to minimize page faults, which usually reduces storage I/O and improves performance.

### Basic page-fault path when RAM is full

1. A process references a page that is not resident.
2. A page fault occurs.
3. The OS checks for a free frame.
4. If there is no free frame, it chooses a victim page.
5. If the victim is dirty, its contents may need to be written back.
6. The requested page is loaded into the victim frame.
7. Page-table/frame information is updated and execution resumes.

A **reference string** is the sequence of virtual page numbers referenced by a process. Example:

`7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1`

Important rules:
- Use page numbers, not full memory addresses.
- Re-accessing a page already in a frame is a hit, not a page fault.
- The page-fault count depends on the reference string, the algorithm, and the number of frames.
- Assume frames start empty unless the question states otherwise.

---

# 2. FIFO — First-In, First-Out (slides 6–7)

FIFO replaces the page that has been in memory the longest, i.e. the page that entered RAM earliest.

Implementation idea: maintain a queue.
- On a page fault with a free frame, insert the new page.
- Once frames are full, evict the queue's oldest page and put the new page at the back.
- A hit does not change the arrival order.

## Worked example: 3 frames

Reference string from the slide's main example:

`7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1`

| Ref. | Frame contents after access (oldest → newest) | Result |
|---:|---|---|
| 7 | 7 | Fault |
| 0 | 7, 0 | Fault |
| 1 | 7, 0, 1 | Fault |
| 2 | 0, 1, 2 | Fault; evict 7 |
| 0 | 0, 1, 2 | Hit |
| 3 | 1, 2, 3 | Fault; evict 0 |
| 0 | 2, 3, 0 | Fault; evict 1 |
| 4 | 3, 0, 4 | Fault; evict 2 |
| 2 | 0, 4, 2 | Fault; evict 3 |
| 3 | 4, 2, 3 | Fault; evict 0 |
| 0 | 2, 3, 0 | Fault; evict 4 |
| 3 | 2, 3, 0 | Hit |
| 0 | 2, 3, 0 | Hit |
| 3 | 2, 3, 0 | Hit |
| 2 | 2, 3, 0 | Hit |
| 1 | 3, 0, 1 | Fault; evict 2 |
| 2 | 0, 1, 2 | Fault; evict 3 |
| 0 | 0, 1, 2 | Hit |
| 1 | 0, 1, 2 | Hit |
| 7 | 1, 2, 7 | Fault; evict 0 |
| 0 | 2, 7, 0 | Fault; evict 1 |
| 1 | 7, 0, 1 | Fault; evict 2 |

**Total FIFO page faults for this 22-reference string with 3 frames: 15.**

The deck's FIFO illustration may omit repeated references in its displayed string. When solving, count exactly the reference string printed in the question.

### Advantages
- Simple to implement.
- Queue makes it easy to track which page arrived earliest.

### Disadvantages
- Ignores how frequently or recently a page is used.
- A heavily used page can be evicted simply because it is old.

---

# 3. Belady's anomaly (slide 7)

**Belady's anomaly** is the surprising situation in which increasing the number of frames causes FIFO to produce *more* page faults rather than fewer.

The slide gives the classic reference string:

`1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5`

For this string:
- FIFO with 3 frames produces 9 faults.
- FIFO with 4 frames produces 10 faults.

Thus, adding a frame makes the fault count worse.

Not all page-replacement algorithms exhibit Belady's anomaly. FIFO can; stack algorithms such as LRU and Optimal do not.

---

# 4. Optimal page replacement (slide 8)

The **Optimal (OPT)** algorithm replaces the page whose next use is farthest in the future—or that will never be used again.

This gives the minimum possible number of page faults for a known reference string and fixed number of frames.

### How to solve
1. On a hit, do nothing.
2. On a fault with a free frame, load the page.
3. When all frames are full, look ahead in the remaining reference string.
4. Evict the resident page whose next use is farthest away. If a page is never used again, it is an ideal victim.

### Why is OPT mainly theoretical?
A real OS generally cannot know future references exactly. OPT is useful as a benchmark: it tells us the best fault count an algorithm could achieve for that reference string.

**Exam tip:** look forward from the current reference, not backward. OPT uses future knowledge.

---

# 5. LRU — Least Recently Used (slides 9–12)

LRU replaces the page that has not been used for the longest time in the past.

It uses past behavior as a practical approximation to future behavior:
- OPT asks, “Which page will be used farthest in the future?”
- LRU asks, “Which page was used least recently?”

On every hit, update the page's recency. On a fault when frames are full, evict the least recently used resident page.

## LRU example from the slide

The slide's reference string is a shortened sequence:

`7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1`

With 3 frames, **LRU gives 12 page faults** for this shortened sequence.

Be careful not to combine this count with the longer 22-reference string from slide 4 without checking the exact sequence. Reference-string variants change the total.

### LRU implementations

#### A. Counter/timestamp implementation
- Each page-table entry (or associated metadata) has a timestamp/counter.
- Whenever the page is referenced, copy the current clock value into its timestamp.
- On replacement, inspect resident pages and evict the one with the smallest timestamp.

**Cost:** finding the oldest page may require searching the entries; timestamp updates also need hardware/OS support.

#### B. Stack implementation
- Maintain pages in a doubly linked list ordered by recency.
- The most recently used page is at the top.
- Whenever a page is referenced, move it to the top.
- The least recently used page stays at the bottom and is the victim.

A doubly linked list needs pointer updates when an element is moved; the slide notes that several pointers (often six) must be changed. This avoids searching for the victim, but each reference may make updates more expensive.

### Stack algorithms and Belady's anomaly
LRU and OPT are **stack algorithms**: the set of pages held with \(n\) frames is a subset of the pages held with \(n+1\) frames. Therefore, adding frames cannot increase their page-fault count. They do not suffer Belady's anomaly.

---

# 6. Comparing the main algorithms

| Algorithm | Victim choice | Needs future knowledge? | Belady's anomaly? | Main point |
|---|---|---:|---:|---|
| FIFO | Oldest arrival | No | Yes | Simple queue |
| OPT | Farthest next use | Yes | No | Best possible benchmark |
| LRU | Least recently used | No; uses past | No | Good practical approximation |
| Clock/Second Chance | Approximate LRU using reference bit | No | Depends on variant; standard Clock is not generally treated as a stack algorithm | Efficient approximation; covered in Topic 3.8 |

---

# 7. Allocation of frames (slides 13–16)

Frame allocation decides how physical frames are distributed among processes.

Example from the PPT:
- Total physical memory = 128 KB
- Page/frame size = 1 KB
- Total frames = \(128/1=128\)
- OS uses 35 KB = 35 frames
- Frames remaining for user processes = \(128-35=93\)

Under pure demand paging, initial page faults can be satisfied using the free-frame list. After all 93 frames are used, a page-replacement algorithm must free a frame when another page is needed. When a process terminates, its frames can return to the free-frame list.

## Minimum and maximum frames (slide 14)

Each process must receive at least a minimum number of frames. The minimum is influenced by the computer architecture and by how many distinct pages a single instruction can reference.

Examples from the notes:
- A simple instruction may require a frame for the instruction and another for the data: at least 2 frames.
- Indirect addressing may require an additional page to read an address or pointer: at least 3 frames in the example.

The exact minimum depends on the architecture and instruction set. Too few frames can prevent an instruction from completing properly and can sharply increase page faults.

The maximum number of frames is limited by available physical memory and the OS's allocation policy.

## A. Equal allocation (slide 15)

Give every process the same number of frames, regardless of process size.

\[
\text{Frames per process}=\frac{\text{available frames}}{\text{number of processes}}
\]

Example: 93 frames and 5 processes.

\[
93/5=18.6
\]

Assign 18 frames to each process, using 90 frames. Keep the remaining 3 frames in a free-frame buffer pool.

**Advantages:** simple and equal.

**Disadvantages:** ignores process size. Small processes may receive more frames than needed, while large processes may suffer more page faults.

## B. Proportional allocation (slide 16)

Allocate frames according to process size.

Let:
- \(s_i\) = size of process \(i\)
- \(S=\sum s_i\) = total size of all processes
- \(m\) = available frames
- \(a_i\) = frames allocated to process \(i\)

\[
\boxed{a_i=\frac{s_i}{S}\times m}
\]

Round to suitable integers, respect the minimum-frame requirement, and ensure the total allocation does not exceed \(m\).

### Worked example from the PPT
Two processes:
- Process 1 size = 10 pages
- Process 2 size = 127 pages
- Total frames = 62
- Total size \(S=10+127=137\)

Process 1:

\[
a_1=\frac{10}{137}\times62\approx4.53
\]

Process 2:

\[
a_2=\frac{127}{137}\times62\approx57.47
\]

The slide rounds these approximately to 4 and 57 frames, totaling 61; rounding and minimum-frame constraints must be reconciled with the available frame total in a real allocation. A common allocation convention is to use a rounding method that ensures the final integer allocations sum to the available total. The key exam formula is proportional to process size.

---

# 8. Global versus local replacement (slide 17)

This classification concerns **which frames a process is allowed to choose as victims**.

## Global replacement
A process may select a victim frame from the frames allocated to any process.

- A process can take a frame away from another process.
- The number of frames assigned to a process can change dynamically.
- Often improves overall memory utilization and throughput.
- Performance of one process can be affected by the behavior of other processes.

Example: under a priority-based global scheme, a high-priority process may take a frame from a lower-priority process.

## Local replacement
A process may select a victim only from its own allocated frames.

- The process's frame allocation remains fixed (unless a separate allocation policy changes it).
- Processes interfere less with each other's frame sets.
- Performance is more predictable.
- A process may suffer faults even when another process has frames that could be used more productively.

| Feature | Global replacement | Local replacement |
|---|---|---|
| Victim selection | Any eligible system frame | Only own frames |
| Frame allocation | Can change dynamically | Normally stays fixed |
| Cross-process interference | Higher | Lower |
| Utilization/throughput | Often higher | Can be lower |
| Predictability | Lower | Higher |

The PPT says global replacement is generally associated with higher throughput and is commonly used. However, it can also allow one process's memory pressure to affect others.

---

# 9. Thrashing (slides 18–19 and companion notes)

**Thrashing occurs when a process spends more time handling page faults and moving pages between RAM and storage than executing useful instructions.**

It typically happens when a process does not have enough frames to hold its current working set.

### How thrashing develops
1. A process references a page that is absent.
2. A page fault occurs.
3. The OS evicts another page to make room.
4. The process soon needs the evicted page again.
5. Another page fault occurs.
6. This repeats continuously.

Example from the notes: a process needs 8 active pages but has only 4 frames. It repeatedly loads a needed page by evicting another page that it soon needs again. The system spends its time paging instead of making progress.

## Causes of thrashing
1. **Insufficient frames:** the process cannot hold its working set.
2. **High degree of multiprogramming:** too many processes compete for limited frames.
3. **Global replacement interactions:** a process under pressure can take frames from others, spreading memory pressure.
4. **High page-fault rate:** more time is spent waiting for page I/O, leaving less time for useful execution.

## Symptoms/effects
- Very high page-fault rate
- Heavy storage activity
- Low CPU utilization
- Reduced throughput
- Increased effective memory-access time
- Poor overall system responsiveness

## The dangerous feedback loop (slide 19)

1. More processes are admitted into memory.
2. Each process receives fewer frames.
3. Processes fault more often.
4. CPU utilization falls because processes wait for paging I/O.
5. The OS may interpret low CPU utilization as a sign that it should admit more processes.
6. More processes compete for frames, worsening thrashing.

The key is that low CPU utilization does not always mean the system needs more processes. It may mean memory pressure is so severe that processes cannot execute efficiently.

---

# 10. Locality model

A **locality** is a group of pages a process uses intensively during a particular phase of execution.

Examples:
- A function and its local variables
- A loop and the data it repeatedly accesses
- A program module

A process may move from one locality to another as its execution changes.

If enough frames are available to hold the current locality, page faults become relatively rare. If the process cannot hold its active locality in memory, it can thrash.

Locality connects the behavior of real programs to working-set frame allocation.

---

# 11. Working-set model

The **working set** is the collection of distinct pages referenced during the most recent window of execution, commonly described as the most recent \(\Delta\) page references.

- \(\Delta\) is the window size.
- \(WSS_i\) is the working-set size of process \(i\): the number of distinct pages in that window.

Total demand for frames is:

\[
D=\sum_i WSS_i
\]

If \(D\) is no greater than the available frames, the OS may be able to keep all current working sets resident.

If \(D\) exceeds available frames, the system is overcommitted. To prevent thrashing, the OS can suspend or swap out one or more processes and redistribute frames among the remaining processes.

The model is based on the locality principle: the pages used recently are a useful estimate of the pages likely to be needed soon.

---

# 12. Page-Fault Frequency (PFF) method

PFF controls thrashing by monitoring each process's page-fault rate.

The OS defines an upper and a lower threshold.

- If the page-fault rate rises above the **upper limit**, allocate more frames to that process if possible.
- If the rate falls below the **lower limit**, the process may have more frames than it needs; reclaim some for other processes.
- If the rate is high and no frames are available, the OS may suspend a process to reduce the degree of multiprogramming and free its frames.

The working-set model estimates *which pages* should be resident. PFF uses the observed *fault rate* to adjust frame allocation.

---

# 13. Kernel page reaper and background reclaim (companion notes)

The notes describe free-memory thresholds:
- When free memory falls below a minimum/low threshold, a background reclaim mechanism starts.
- It reclaims pages until free memory reaches a maximum/high threshold.
- If memory becomes critically low, Linux may invoke the Out-of-Memory (OOM) killer to terminate a process and recover memory.

The 3.6.1 presentation introduced `kswapd`; this lecture's notes use the term “kernel page reaper.” The underlying principle is proactive reclaim: maintain a reserve of free frames so that the process needing memory does not always have to wait for reclamation to begin.

---

# 14. Related concept in the notes: NUMA

The companion notes briefly introduce **Non-Uniform Memory Access (NUMA)**:
- Each CPU/socket has local memory.
- Accessing local memory is faster than accessing memory attached to another CPU/socket.
- The OS tries to allocate frames close to the CPU executing the process to improve memory-access performance.

NUMA is the primary topic of the following 3.8.1 presentation, so learn its details there. For this lecture, remember the connection: frame allocation is not only about how many frames a process receives; on NUMA systems, where those frames are physically located also matters.

---

# 15. Quick formula sheet

| Concept | Formula / rule |
|---|---|
| Total physical frames | Physical memory size / frame size |
| Frames available to user processes | Total frames − OS-reserved frames |
| Equal allocation | \(a_i\approx m/n\) |
| Proportional allocation | \(a_i=(s_i/S)\times m\) |
| Total working-set demand | \(D=\sum_i WSS_i\) |
| Thrashing | Paging/page-fault handling dominates useful execution |
| FIFO victim | Oldest page by arrival time |
| OPT victim | Page whose next use is farthest in the future |
| LRU victim | Page least recently used in the past |

---

# 16. Exam-style questions and answers

1. **Why is page replacement needed?** RAM has a finite number of frames; when a fault occurs and no free frame exists, the OS must evict a resident page.
2. **What is a reference string?** A sequence of virtual page numbers accessed by a process.
3. **What does FIFO replace?** The page that entered memory earliest.
4. **What is Belady's anomaly?** FIFO can produce more page faults after the number of frames is increased.
5. **What does OPT replace?** The resident page whose next reference is farthest in the future.
6. **Why is OPT theoretical?** The OS cannot generally know future memory references exactly.
7. **What does LRU replace?** The page that has not been referenced for the longest time in the past.
8. **How can LRU be implemented?** Timestamps/counters or a recency-ordered doubly linked list.
9. **Why do LRU and OPT avoid Belady's anomaly?** They are stack algorithms: the resident-page set with \(n\) frames is contained in the set with \(n+1\) frames.
10. **What is equal allocation?** Every process gets the same number of frames.
11. **What is proportional allocation?** Frames are distributed according to process size.
12. **Global versus local replacement?** Global may select a victim from another process's frames; local may select only from its own.
13. **Define thrashing.** Spending more time paging than executing useful instructions.
14. **Why can adding processes worsen thrashing?** More processes compete for the same frames, increasing fault rates.
15. **What is a locality?** A set of pages used intensively during a phase of execution.
16. **What is the working-set model?** It estimates the pages needed from a recent reference window and allocates enough frames to keep them resident.
17. **How does PFF work?** It increases allocation when fault rate is too high and reclaims frames when the rate is low.
18. **How can the OS respond if total working-set demand exceeds RAM?** Suspend/swap out one or more processes to reduce the degree of multiprogramming.
19. **What is the dirty bit's relevance to replacement?** A dirty victim may need to be written back before its frame is reused.
20. **What does NUMA add to frame allocation?** The physical location of a frame matters because local memory is faster than remote memory.

---

# 17. Slide-by-slide coverage checklist

- [x] Slides 1–3: course title, credits, topic title.
- [x] Slide 4: purpose of frame allocation and page replacement, reference strings, page-fault evaluation.
- [x] Slide 5: graph of page faults versus frame count; general principle that faults usually decrease as frames increase, with FIFO's anomaly as an exception.
- [x] Slides 6–7: FIFO, queue implementation, classic Belady's anomaly example.
- [x] Slide 8: Optimal replacement.
- [x] Slides 9–10: LRU definition, example, 12-fault result for the slide's shorter reference string.
- [x] Slides 11–12: counter and stack implementations, stack-algorithm property.
- [x] Slides 13–14: allocation of frames and minimum frames.
- [x] Slides 15–16: equal and proportional allocation, numerical examples.
- [x] Slide 17: global versus local replacement.
- [x] Slides 18–19: thrashing and its CPU-utilization feedback loop.
- [x] Slide 20: closing slide.
- [x] Companion notes pages 1–3: replacement basics, allocation, advantages/disadvantages.
- [x] Companion notes page 4: major/minor faults, background reclaim, NUMA connection.
- [x] Companion notes pages 5–7: thrashing causes/effects, locality, working set, PFF, prevention.

**End of Topic 3.7.1.** Practise tracing FIFO, OPT, and LRU on a reference string before memorizing the theory.
