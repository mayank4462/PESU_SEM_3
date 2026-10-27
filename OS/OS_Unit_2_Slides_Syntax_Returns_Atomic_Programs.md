# OS Unit 2 — Slide Syntax + Return Values + Atomic Instruction Programs

> **Purpose:** Separate sheet for the things in the Unit 2 slides that are easy to forget in MCQs/program questions: **function syntax, return values, argument meanings, small code snippets, and Test-and-Set / Compare-and-Swap programs**.
>
> **Source rule:** This sheet is extracted from the supplied Unit 2 slides. Where the slides give a syntax but **do not state a return value**, that is explicitly marked.

---

# 1. PIPE — `pipe()`

## Syntax

```c
#include <unistd.h>

int pipe(int fd[2]);
```

## Return value

```text
 0  → success
-1  → error
```

## File descriptors

```text
fd[0] → read end
fd[1] → write end
```

### Core fact

```text
output of fd[1] → input for fd[0]
```

### Parent → Child

```c
pipe(fd);
fork();

/* parent */
close(fd[0]);      // close read end
write(fd[1], ...); // write

/* child */
close(fd[1]);      // close write end
read(fd[0], ...);  // read
```

### Child → Parent

```c
pipe(fd);
fork();

/* parent */
close(fd[1]);
read(fd[0], ...);

/* child */
close(fd[0]);
write(fd[1], ...);
```

### MCQ traps

- Pipes are historically **half-duplex**.
- For maximum portability, do **not** assume full-duplex behavior.
- Unnamed pipes are normally used between processes with a **common ancestor**.

---

# 2. `popen()` / `pclose()`

## `popen()`

### Syntax

```c
#include <stdio.h>

FILE *popen(const char *cmdstring, const char *type);
```

### Meaning of `type`

```text
"r" → FILE * connected to standard output of cmdstring
"w" → FILE * connected to standard input of cmdstring
```

### Return value

```text
FILE * → success
NULL   → error
```

### Important

`popen()` internally handles:

```text
pipe → fork → close unused ends → execute shell command → wait/management
```

The slides specifically state that `popen()` does a **fork + exec** to execute `cmdstring`.

---

## `pclose()`

### Syntax

```c
int pclose(FILE *fp);
```

### Return value

```text
termination status of the shell → normal return
-1                              → error
```

### Important

`pclose()`:

- closes the standard I/O stream
- waits for the command to terminate

---

# 3. FIFO / Named Pipe

## `mkfifo()`

### Syntax

```c
#include <sys/stat.h>

int mkfifo(const char *path, mode_t mode);
```

## `mkfifoat()`

### Syntax

```c
int mkfifoat(int dirfd, const char *path, mode_t mode);
```

## Return value

For **both**:

```text
 0  → success
-1  → error
```

### Important

- FIFO = **named pipe**.
- Unlike unnamed pipes, FIFOs can be used by **unrelated processes**.
- After creation, use ordinary file I/O such as `open()`, `close()`, `read()`, `write()`, `unlink()`.

---

# 4. System V MESSAGE QUEUES

## `msgget()`

### Syntax

```c
#include <sys/msg.h>

int msgget(key_t key, int flag);
```

## Return value

```text
message queue ID → success
-1                 → error
```

### Purpose

Create a new queue or open an existing queue.

---

## `msgsnd()`

### Syntax

```c
#include <sys/msg.h>

int msgsnd(int msqid, const void *ptr, size_t nbytes, int flag);
```

### Important from slides

Each message contains:

```text
1. positive long integer type
2. non-negative length
3. actual data bytes
```

### Return value

> **Not stated in the supplied slides.**

---

## `msgrcv()`

### Syntax

```c
#include <sys/msg.h>

ssize_t msgrcv(int msqid, void *ptr, size_t nbytes,
               long type, int flag);
```

### Important

Messages do **not** have to be fetched FIFO-style.

They can be selected using their:

```c
type
```

### Return value

> **Not stated in the supplied slides.**

---

## Message Structure

```c
struct message {
    long msg_type;
    char msg_text[100];
};
```

### MCQ trap

```text
msg_type → positive long integer
```

---

# 5. System V SEMAPHORES

## `semget()`

### Syntax

```c
#include <sys/sem.h>

int semget(key_t key, int nsems, int flag);
```

## Return value

```text
semaphore ID → success
-1             → error
```

---

## `semctl()`

### Syntax

```c
#include <sys/sem.h>

int semctl(int semid, int semnum, int cmd, ...);
```

### Purpose

Catch-all function for **various semaphore operations**.

### Return value

> **Not explicitly stated in the supplied slides.**

---

## `semop()`

### Syntax

```c
#include <sys/sem.h>

int semop(int semid, struct sembuf semoparray[], size_t nops);
```

### Important

- `semoparray` points to an **array of semaphore operations**.
- Operations are performed **atomically** on the semaphore set.

## Return value

```text
 0  → success
-1  → error
```

---

## `struct sembuf`

```c
struct sembuf {
    unsigned short sem_num;
    short sem_op;
    short sem_flg;
};
```

---

# 6. SEMAPHORE `wait()` / `signal()` DEFINITIONS

## `wait(S)` / P / Down

```c
wait(S) {
    while (S <= 0)
        ; // busy wait
    S--;
}
```

## `signal(S)` / V / Up

```c
signal(S) {
    S++;
}
```

### Remember

```text
wait   → decrement / acquire
signal → increment / release
```

### Semaphore types

```text
Counting semaphore → unrestricted integer range
Binary semaphore   → only 0 or 1
```

---

# 7. Semaphore — No Busy Waiting

## Structure

```c
typedef struct {
    int value;
    struct process *list;
} semaphore;
```

## `wait()`

```c
wait(semaphore *S) {
    S->value--;

    if (S->value < 0) {
        add this process to S->list;
        block();
    }
}
```

## `signal()`

```c
signal(semaphore *S) {
    S->value++;

    if (S->value >= 0) {
        remove a process P from S->list;
        wakeup(P);
    }
}
```

### Key points

- `block()` places the process on the appropriate waiting queue.
- `wakeup(P)` moves a waiting process to the ready queue.

---

# 8. POSIX THREAD APIs

## `pthread_create()`

### Syntax

```c
int pthread_create(
    pthread_t *thread,
    const pthread_attr_t *attr,
    void *(*start_routine)(void *),
    void *arg
);
```

### Arguments

```text
thread         → stores thread ID / handle
attr           → thread attributes
start_routine  → function executed by new thread
arg            → argument passed to that function
```

### Return value

> **Not stated in the supplied slides.**

---

## `pthread_join()`

### Syntax

```c
int pthread_join(pthread_t thread, void **retval);
```

### Important

- `retval` lets the caller receive the return value of the terminated thread.
- The call is **blocking**.
- If target thread is still running, caller waits.

### Return value

> **Not stated in the supplied slides.**

---

## `pthread_exit()`

### Syntax

```c
void pthread_exit(void *retval);
```

### Important

- Terminates the calling thread.
- `retval` is the thread's return value.
- Calling `pthread_exit()` in `main()` keeps the process alive until the other threads finish.
- Calling `exit()` or returning from `main()` kills the **whole process and all its threads**.

---

# 9. PTHREAD SCHEDULING SCOPE

## Scope constants

```c
PTHREAD_SCOPE_PROCESS
```

→ **PCS** scheduling

```c
PTHREAD_SCOPE_SYSTEM
```

→ **SCS** scheduling

---

## `pthread_attr_init()`

```c
pthread_attr_init(&attr);
```

Used to initialize the thread-attribute object.

---

## `pthread_attr_getscope()`

```c
pthread_attr_getscope(&attr, &scope);
```

Used to inquire about the current scheduling scope.

The supplied slide checks:

```c
if (pthread_attr_getscope(&attr, &scope) != 0)
    fprintf(stderr, "Unable to get scheduling scope\n");
```

> The slides do not explicitly state the return-value table beyond the nonzero error check shown above.

---

## `pthread_attr_setscope()`

```c
pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
```

Used to set PCS or SCS scheduling scope.

---

# 10. LINUX DOMAIN SOCKETS — SLIDE SYNTAX

## Socket creation

```c
socket(AF_UNIX, SOCK_STREAM, 0);
```

### Address family

```text
AF_UNIX
```

or

```text
AF_LOCAL
```

### `sockaddr_un`

```c
struct sockaddr_un {
    sa_family_t sun_family;
    char sun_path[108];
};
```

---

## Server-side sequence

### 1. Create socket

```c
server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
```

### 2. Configure address

```c
addr.sun_family = AF_UNIX;
strcpy(addr.sun_path, "/tmp/mysocket");
```

### 3. Remove old socket file

```c
unlink("/tmp/mysocket");
```

### 4. Bind

```c
bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
```

### 5. Listen

```c
listen(server_fd, 5);
```

### 6. Accept

```c
client_fd = accept(server_fd, NULL, NULL);
```

### 7. Receive

```c
read(client_fd, buffer, sizeof(buffer));
```

### 8. Send reply

```c
write(client_fd, "Hello Client", 13);
```

### 9. Close

```c
close(client_fd);
close(server_fd);
unlink("/tmp/mysocket");
```

### Return values

> The supplied slides show the syntax/workflow but **do not state explicit return-value tables** for these socket calls.

---

## Client-side sequence

### 1. Create socket

```c
sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
```

### 2. Specify server address

```c
addr.sun_family = AF_UNIX;
strcpy(addr.sun_path, "/tmp/mysocket");
```

### 3. Connect

```c
connect(sockfd,
        (struct sockaddr *)&addr,
        sizeof(addr));
```

### 4. Send

```c
write(sockfd, "Hello Server", 13);
```

### 5. Receive reply

```c
read(sockfd, buffer, sizeof(buffer));
```

### 6. Close

```c
close(sockfd);
```

---

# 11. THREAD SCHEDULING PROGRAM FROM SLIDES

```c
#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 5

int main(int argc, char *argv[]) {
    int i, scope;
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* inquire on current scope */
    if (pthread_attr_getscope(&attr, &scope) != 0)
        fprintf(stderr, "Unable to get scheduling scope\n");
    else {
        if (scope == PTHREAD_SCOPE_PROCESS)
            printf("PTHREAD_SCOPE_PROCESS");
        else if (scope == PTHREAD_SCOPE_SYSTEM)
            printf("PTHREAD_SCOPE_SYSTEM");
        else
            fprintf(stderr, "Illegal scope value.\n");
    }

    /* set scheduling algorithm to PCS or SCS */
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    /* create threads */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_create(&tid[i], &attr, runner, NULL);

    /* join each thread */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(tid[i], NULL);

    return 0;
}

/* each thread begins control here */
void *runner(void *param) {
    /* do some work ... */
    pthread_exit(0);
}
```

### Remember

- `PTHREAD_SCOPE_PROCESS` → PCS.
- `PTHREAD_SCOPE_SYSTEM` → SCS.
- `pthread_join()` is used after creation to synchronize the main thread with workers.

---

# 12. TEST-AND-SET (TSL)

## What the instruction does

The slide definition says `test_and_set`:

1. Executes **atomically**.
2. Returns the **original value** of the parameter.
3. Sets the parameter to **TRUE**.

---

## Definition from slide

```c
boolean test_and_set(boolean *target)
{
    boolean rv = *target;
    *target = TRUE;
    return rv;
}
```

> **Slide typo:** the slide displays `return rv:`; the valid C statement is `return rv;`.

### Ultra-fast memory

```text
old = *target
*target = TRUE
return old
```

---

# 13. MUTUAL EXCLUSION USING TEST-AND-SET

## Slide program

```c
do {
    while (test_and_set(&lock))
        ; /* do nothing */

    /* critical section */

    lock = false;

    /* remainder section */

} while (true);
```

## Initial condition

```c
lock = false;
```

### How to remember

```text
lock = 0 → free
TAS returns 0 → enter
TAS returns 1 → keep spinning
leave CS → lock = 0
```

### Key point

`test_and_set()` combines:

```text
test old value + set TRUE
```

as **one atomic operation**.

---

# 14. COMPARE-AND-SWAP (CAS)

## Definition from slide

```c
int compare_and_swap(int *value, int expected, int new_value)
{
    int temp = *value;

    if (*value == expected)
        *value = new_value;

    return temp;
}
```

### What CAS does

1. Executes **atomically**.
2. Returns the **original value** of `*value`.
3. Changes `*value` to `new_value` **only if** `*value == expected`.

### Ultra-fast memory

```text
read old value
if old == expected
    replace with new_value
return old
```

---

# 15. MUTUAL EXCLUSION USING COMPARE-AND-SWAP

## Slide program

```c
do {
    while (compare_and_swap(&lock, 0, 1) != 0)
        ; /* do nothing */

    /* critical section */

    lock = 0;

    /* remainder section */

} while (true);
```

## Initial condition

```c
lock = 0;
```

### Key point

For:

```c
compare_and_swap(&lock, 0, 1)
```

```text
lock = 0 → changes to 1 → returns 0 → enter CS
lock = 1 → remains 1   → returns 1 → keep spinning
```

### Important slide statement

- Mutual exclusion is satisfied.
- **Bounded waiting is NOT satisfied.**

---

# 16. TEST-AND-SET — BOUNDED WAITING MUTUAL EXCLUSION

## Common data structures

```c
boolean waiting[n];
boolean lock;
```

## Algorithm from slide

```c
do {
    waiting[i] = true;
    key = true;

    while (waiting[i] && key)
        key = test_and_set(&lock);

    waiting[i] = false;

    /* critical section */

    j = (i + 1) % n;

    while ((j != i) && !waiting[j])
        j = (j + 1) % n;

    if (j == i)
        lock = false;
    else
        waiting[j] = false;

    /* remainder section */

} while (true);
```

### Remember

This version adds:

```c
boolean waiting[n];
```

to provide a bounded-waiting mechanism.

The slides state that this algorithm satisfies the critical-section requirements.

---

# 17. QUICK ATOMIC-INSTRUCTION COMPARISON

| Property | Test-and-Set | Compare-and-Swap |
|---|---|---|
| Atomic | Yes | Yes |
| Returns | Original target value | Original `*value` |
| Main condition | Test + set TRUE | Replace only if `*value == expected` |
| Typical lock call | `test_and_set(&lock)` | `compare_and_swap(&lock, 0, 1)` |
| Basic mutual exclusion | Yes | Yes |
| Bounded waiting | Basic version: not guaranteed | Slide says not guaranteed |
| Lock release | `lock = false` | `lock = 0` |

---

# 18. RETURN-VALUE MASTER TABLE — FROM THE SLIDES

| Function | Success return | Error return | Slide explicitly gives it? |
|---|---|---|---|
| `pipe()` | `0` | `-1` | ✅ |
| `popen()` | `FILE *` | `NULL` | ✅ |
| `pclose()` | shell termination status | `-1` | ✅ |
| `mkfifo()` | `0` | `-1` | ✅ |
| `mkfifoat()` | `0` | `-1` | ✅ |
| `msgget()` | message queue ID | `-1` | ✅ |
| `msgsnd()` | — | — | ❌ not stated |
| `msgrcv()` | — | — | ❌ not stated |
| `semget()` | semaphore ID | `-1` | ✅ |
| `semctl()` | — | — | ❌ not stated |
| `semop()` | `0` | `-1` | ✅ |
| `pthread_create()` | — | — | ❌ not stated |
| `pthread_join()` | — | — | ❌ not stated |
| `pthread_exit()` | `void` function | — | — |
| socket calls | — | — | ❌ not stated in these slides |
| `test_and_set()` | original target value | — | ✅ |
| `compare_and_swap()` | original `*value` | — | ✅ |

---

# 19. ONE-MINUTE MCQ RECALL

```text
pipe()
    0 success, -1 error
    fd[0] = READ
    fd[1] = WRITE

popen()
    FILE * success
    NULL error
    "r" → read command output
    "w" → write to command input

pclose()
    termination status
    -1 error

mkfifo(), mkfifoat()
    0 success
    -1 error

msgget()
    queue ID success
    -1 error

semget()
    semaphore ID success
    -1 error

semop()
    0 success
    -1 error

test_and_set()
    returns OLD value
    sets target = TRUE atomically

compare_and_swap()
    returns OLD value
    changes value ONLY if old == expected

pthread_join()
    blocking wait for target thread

pthread_exit()
    terminates calling thread

PCS
    PTHREAD_SCOPE_PROCESS

SCS
    PTHREAD_SCOPE_SYSTEM
```
