# OS Unit 2 — Programs + Quick Revision

> **Code-first revision sheet**
>
> Learn the program pattern first. After every program, only the **2–3 exam-important points** are given.
>
> **Coverage:** every code-bearing `.c` file in the supplied ZIP + the distinct code programs/snippets present in `systemcall.docx`.

---

# 1. Pthreads

## 1.1 Basic Thread Creation — `ptreadex.c`

```c
#include <pthread.h>
#include <stdio.h>

void *thread_function(void *arg) {
    char *message = (char *)arg;
    printf("Thread says: %s\n", message);
    return NULL;
}

int main() {
    pthread_t thread;
    char *message = "Hello from the thread!";

    if (pthread_create(&thread, NULL, thread_function, (void *)message) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    pthread_join(thread, NULL);

    printf("Thread has finished.\n");
    return 0;
}
```

### Remember
- `pthread_create()` creates the thread and starts execution from `thread_function()`.
- The 4th argument of `pthread_create()` is passed to the thread function as `void *arg`.
- `pthread_join()` makes the main thread wait for the created thread.

**Compile:**
```bash
gcc ptreadex.c -o ptreadex -pthread
```

---

## 1.2 Pthread Scheduling Scope — PCS / SCS — `pthreadpcsscs.c`

> **Cleaned exam-ready version:** the supplied source had `NUM THREADS` / forward-declaration issues. The logic below is the intended program.

```c
#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 5

void *runner(void *param);

int main(int argc, char *argv[]) {
    int i, scope;
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;

    /* Get the default attributes */
    pthread_attr_init(&attr);

    /* Inquire about current scheduling scope */
    if (pthread_attr_getscope(&attr, &scope) != 0) {
        fprintf(stderr, "Unable to get scheduling scope\n");
    } else {
        if (scope == PTHREAD_SCOPE_PROCESS)
            printf("PTHREAD_SCOPE_PROCESS\n");
        else if (scope == PTHREAD_SCOPE_SYSTEM)
            printf("PTHREAD_SCOPE_SYSTEM\n");
        else
            fprintf(stderr, "Illegal scope value.\n");
    }

    /* Set scheduling scope to SCS */
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    /* Create threads */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_create(&tid[i], &attr, runner, NULL);

    /* Join every thread */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(tid[i], NULL);

    return 0;
}

/* Each thread begins execution here */
void *runner(void *param) {
    pthread_exit(0);
}
```

### Remember
- `PTHREAD_SCOPE_PROCESS` → **PCS**; `PTHREAD_SCOPE_SYSTEM` → **SCS**.
- `pthread_attr_getscope()` reads the current scope; `pthread_attr_setscope()` changes it.
- `pthread_attr_init()` initializes the thread-attribute object.

**Compile:**
```bash
gcc pthreadpcsscs.c -o pthreadpcsscs -pthread
```

---

# 2. Concurrency and Parallelism

## 2.1 Concurrency vs Parallelism Using CPU Affinity — `concurrency_parallelism.c`

> **Cleaned version:** the supplied file contained stray English text after two `bind_to_core()` calls. The intended logic is preserved below.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sched.h>

#define ITERATIONS 2000000000L

/* CPU-intensive task */
void do_work(const char *name) {
    volatile long long sum = 0;

    printf("%s started (PID: %d)\n", name, getpid());

    for (long i = 0; i < ITERATIONS; i++) {
        sum += i % 10;
    }

    printf("%s finished (PID: %d)\n", name, getpid());
}

/* Bind current process to a particular CPU core */
void bind_to_core(int core) {
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(core, &set);

    if (sched_setaffinity(0, sizeof(set), &set) == -1) {
        perror("sched_setaffinity");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <mode>\n", argv[0]);
        printf("Modes: 1 = One Core, 2 = Two Cores\n");
        return 1;
    }

    int mode = atoi(argv[1]);
    clock_t start = clock();

    pid_t pid1 = fork();

    if (pid1 == 0) {
        if (mode == 1)
            bind_to_core(0);    // Both processes use Core 0
        else
            bind_to_core(0);    // Process 1 uses Core 0

        do_work("Task 1");
        exit(0);
    }

    pid_t pid2 = fork();

    if (pid2 == 0) {
        if (mode == 1)
            bind_to_core(0);    // Both processes use Core 0
        else
            bind_to_core(1);    // Process 2 uses Core 1

        do_work("Task 2");
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    clock_t end = clock();

    printf("\nAll tasks completed.\n");
    printf("Execution time: %.2f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
```

### Remember
- **Mode 1:** both processes are restricted to **Core 0** → concurrent execution.
- **Mode 2:** Task 1 → Core 0 and Task 2 → Core 1 → parallel execution.
- `sched_setaffinity()` restricts a process to selected CPU core(s).

---

## 2.2 Concurrency / Parallelism with `CLOCK_MONOTONIC` — `coredemo.c`

```c
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>
#include <time.h>

#define ITERATIONS 1000000000L

void do_work(const char *task_name) {
    volatile unsigned long long sum = 0;

    printf("%s started (PID = %d)\n", task_name, getpid());

    for (long i = 0; i < ITERATIONS; i++) {
        sum += i % 10;
    }

    printf("%s finished\n", task_name);
}

/* Restrict process to one CPU core */
void bind_to_core(int core) {
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(core, &set);

    if (sched_setaffinity(0, sizeof(set), &set) == -1) {
        perror("CPU affinity error");
        exit(EXIT_FAILURE);
    }
}

/* Calculate elapsed wall-clock time */
double get_elapsed_time(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s 1\n", argv[0]);
        printf("       %s 2\n", argv[0]);
        printf("1 = Concurrent execution on ONE core\n");
        printf("2 = Parallel execution on TWO cores\n");
        return 1;
    }

    int mode = atoi(argv[1]);

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    pid_t p1 = fork();

    if (p1 == 0) {
        bind_to_core(0);
        do_work("Task 1");
        exit(0);
    }

    pid_t p2 = fork();

    if (p2 == 0) {
        if (mode == 1)
            bind_to_core(0);
        else
            bind_to_core(1);

        do_work("Task 2");
        exit(0);
    }

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("\n---------------------------------\n");
    printf("Mode: %s\n",
           mode == 1 ? "Concurrency on ONE core" :
                       "Parallelism on TWO cores");

    printf("Total elapsed time: %.3f seconds\n",
           get_elapsed_time(start, end));

    return 0;
}
```

### Remember
- `CLOCK_MONOTONIC` is suitable for **elapsed-time measurement** because it moves forward independently of wall-clock changes.
- `waitpid()` waits for a **specific child process**.
- `CPU_ZERO()` clears the CPU set; `CPU_SET()` adds a selected CPU.

**Run:**
```bash
gcc -O0 coredemo.c -o coredemo
./coredemo 1
./coredemo 2
```

---

# 3. Pipes

## 3.1 Pipe Between Parent and Child — `demo_pipe.c`

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main() {
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE] = "";
    int fd[2];
    pid_t pid;

    /* Create pipe */
    if (pipe(fd) == -1) {
        printf("Pipe failed\n");
        return 1;
    }

    /* Fork child process */
    pid = fork();

    if (pid > 0) {
        /* Parent */
        close(fd[READ_END]);

        write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);

        close(fd[WRITE_END]);

        wait(NULL);
    }
    else if (pid == 0) {
        /* Child */
        close(fd[WRITE_END]);

        read(fd[READ_END], read_msg, BUFFER_SIZE);

        printf("child process read -%s\n", read_msg);

        close(fd[READ_END]);
    }
    else {
        printf("Fork failed\n");
        return 1;
    }

    return 0;
}
```

### Remember
- `pipe(fd)` creates two descriptors: `fd[0]` = **read end**, `fd[1]` = **write end**.
- Parent writes → child reads through the pipe.
- Close the **unused end** in each process.

---

## 3.2 Same Pipe Program from `systemcall.docx`

> This is the distinct code block supplied in the DOCX; it implements the same parent → child anonymous-pipe pattern.

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main() {
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE] = "";
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1) {
        printf("Pipe failed\n");
        return 1;
    }

    pid = fork();

    if (pid > 0) {
        close(fd[READ_END]);
        write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);
        close(fd[WRITE_END]);
        wait(NULL);
    }
    else if (pid == 0) {
        close(fd[WRITE_END]);
        read(fd[READ_END], read_msg, BUFFER_SIZE);
        printf("child process read -%s\n", read_msg);
        close(fd[READ_END]);
    }
    else {
        printf("Fork failed\n");
        return 1;
    }
}
```

### Remember
- Same fundamental pattern: `pipe()` → `fork()` → parent `write()` → child `read()`.
- The child must close the write descriptor and the parent must close the read descriptor.

---

# 4. FIFO / Named Pipe

## 4.1 FIFO Writer — `fifo_write.c`

```c
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 100

int main() {
    int fd;
    char buff[BUFFER_SIZE] = "";
    char myfifo[BUFFER_SIZE] = "/tmp/myfifo";

    printf("Enter the message to be passed\n");
    read(0, buff, BUFFER_SIZE);

    /* Create named FIFO */
    mkfifo(myfifo, 0666);

    /* Open FIFO for writing */
    fd = open(myfifo, O_WRONLY);

    /* Write input to FIFO */
    write(fd, buff, strlen(buff) + 1);

    close(fd);

    sleep(100);

    return 0;
}
```

### Remember
- `mkfifo()` creates a **named pipe** in the filesystem.
- Writer uses `open(..., O_WRONLY)` and `write()`.
- The reader and writer can be **separate programs/processes**.

---

## 4.2 FIFO Reader — `fifo_read.c`

```c
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 100

int main() {
    int fd1;
    char buff[BUFFER_SIZE] = "";
    char myfifo[BUFFER_SIZE] = "/tmp/myfifo";

    /* Create named FIFO */
    mkfifo(myfifo, 0666);

    /* Open FIFO for reading */
    fd1 = open(myfifo, O_RDONLY);

    read(fd1, buff, BUFFER_SIZE);

    printf("Information read: %s", buff);

    close(fd1);

    return 0;
}
```

### Remember
- Reader uses `open(..., O_RDONLY)` and `read()`.
- FIFO name/path must match on both programs.
- `mkfifo()` creates the special FIFO file.

---

## 4.3 Interactive FIFO Receiver — from `systemcall.docx`

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_NAME "myfifo"
#define SIZE 100

int main() {
    int fd;
    char buffer[SIZE];

    /* Create FIFO if it does not already exist */
    mkfifo(FIFO_NAME, 0666);

    printf("Receiver started. Waiting for messages...\n");

    /* Open FIFO for reading */
    fd = open(FIFO_NAME, O_RDONLY);

    while (1) {
        int bytesRead = read(fd, buffer, SIZE - 1);

        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            printf("Received: %s", buffer);
        }
    }

    close(fd);

    return 0;
}
```

### Remember
- `read()` returns the **number of bytes actually read**.
- `buffer[bytesRead] = '\0'` converts the received bytes into a C string.
- The receiver waits for incoming FIFO data.

---

## 4.4 Interactive FIFO Sender — from `systemcall.docx`

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_NAME "myfifo"
#define SIZE 100

int main() {
    int fd;
    char message[SIZE];

    printf("Sender started. Type messages below.\n");

    /* Open FIFO for writing */
    fd = open(FIFO_NAME, O_WRONLY);

    while (1) {
        printf("Enter message: ");
        fgets(message, SIZE, stdin);

        /* Send message */
        write(fd, message, strlen(message));

        /* Type exit to stop */
        if (strcmp(message, "exit\n") == 0)
            break;
    }

    close(fd);

    return 0;
}
```

### Remember
- `fgets()` reads the user's message safely into the buffer.
- `write()` sends it to the FIFO.
- `"exit\n"` is used as the termination condition because `fgets()` keeps the newline.

---

# 5. System V Message Queues

## 5.1 Basic Message Queue Writer — `msgq_write.c`

```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define BUF_SIZE 100

struct mesg_buffer {
    long mesg_type;
    char mesg_text[BUF_SIZE];
} message;

int main() {
    key_t key;
    int msgid;

    printf("Enter message to be passed :\n");

    read(0, message.mesg_text, BUF_SIZE);

    printf("Message being sent: %s\n", message.mesg_text);

    key = ftok("progfile", 65);

    msgid = msgget(key, 0666 | IPC_CREAT);

    message.mesg_type = 1;

    msgsnd(msgid, &message, sizeof(message), 0);

    sleep(60);

    return 0;
}
```

### Remember
- `ftok()` generates a key; `msgget()` creates/accesses the queue.
- `mesg_type` must be a **positive long integer**.
- `msgsnd()` sends the message; the supplied version keeps the process alive using `sleep(60)`.

---

## 5.2 Basic Message Queue Reader — `msgq_read.c`

### Exam-ready version

```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUF_SIZE 100

struct mesg_buffer {
    long mesg_type;
    char mesg_text[BUF_SIZE];
} message;

int main() {
    key_t key;
    int msgid;

    key = ftok("progfile", 65);

    msgid = msgget(key, 0666 | IPC_CREAT);

    msgrcv(msgid, &message, sizeof(message.mesg_text), 1, 0);

    printf("Message received : %s\n", message.mesg_text);

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
```

### Remember
- `msgrcv()` receives a message of the requested type; here the type is `1`.
- The message-size argument should represent the **message data size**, excluding `long mesg_type`.
- `msgctl(..., IPC_RMID, NULL)` removes the queue.

---

## 5.3 Sender + Receiver Using Fixed Key — from `systemcall.docx`

### Sender — `sender.c`

```c
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long msg_type;
    char msg_text[100];
};

int main() {
    int msqid;
    struct message msg;

    /* Create or access queue */
    msqid = msgget(1234, IPC_CREAT | 0666);

    if (msqid == -1) {
        perror("msgget failed");
        return 1;
    }

    msg.msg_type = 1;
    strcpy(msg.msg_text, "Hello from Sender Process!");

    if (msgsnd(msqid, &msg, sizeof(msg.msg_text), 0) == -1) {
        perror("msgsnd failed");
        return 1;
    }

    printf("Message sent successfully: %s\n", msg.msg_text);

    return 0;
}
```

### Receiver — `receiver.c`

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long msg_type;
    char msg_text[100];
};

int main() {
    int msqid;
    struct message msg;

    /* Access same queue using same key */
    msqid = msgget(1234, 0666);

    if (msqid == -1) {
        perror("msgget failed");
        return 1;
    }

    printf("Waiting for a message...\n");

    if (msgrcv(msqid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv failed");
        return 1;
    }

    printf("Message received: %s\n", msg.msg_text);

    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        return 1;
    }

    printf("Message queue removed successfully.\n");

    return 0;
}
```

### Remember
- Both programs must use the **same key** to refer to the same queue.
- Sender uses `msgsnd()`; receiver uses `msgrcv()`.
- Receiver can remove the queue using `msgctl(..., IPC_RMID, ...)`.

---

## 5.4 Multiple Message Types — from `systemcall.docx`

### Sender — `sender.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msgid;
    struct message msg;

    /* Generate key */
    key = ftok("progfile", 65);

    if (key == -1) {
        perror("ftok failed");
        return 1;
    }

    /* Create message queue */
    msgid = msgget(key, 0666 | IPC_CREAT);

    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    /* Type 1 */
    msg.msg_type = 1;
    strcpy(msg.msg_text, "Hello Student");
    msgsnd(msgid, &msg, sizeof(msg.msg_text), 0);

    /* Type 2 */
    msg.msg_type = 2;
    strcpy(msg.msg_text, "OS Lab at 2 PM");
    msgsnd(msgid, &msg, sizeof(msg.msg_text), 0);

    /* Type 3 */
    msg.msg_type = 3;
    strcpy(msg.msg_text, "Assignment Due Tomorrow");
    msgsnd(msgid, &msg, sizeof(msg.msg_text), 0);

    printf("Three messages sent successfully.\n");

    return 0;
}
```

### Receiver — `receiver.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msgid;
    struct message msg;

    /* Generate same key */
    key = ftok("progfile", 65);

    if (key == -1) {
        perror("ftok failed");
        return 1;
    }

    /* Access existing queue */
    msgid = msgget(key, 0666);

    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    /* Receive Type 2 */
    msgrcv(msgid, &msg, sizeof(msg.msg_text), 2, 0);
    printf("Received Type %ld message: %s\n",
           msg.msg_type, msg.msg_text);

    /* Receive Type 1 */
    msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0);
    printf("Received Type %ld message: %s\n",
           msg.msg_type, msg.msg_text);

    /* Receive Type 3 */
    msgrcv(msgid, &msg, sizeof(msg.msg_text), 3, 0);
    printf("Received Type %ld message: %s\n",
           msg.msg_type, msg.msg_text);

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
```

### Remember
- `msg_type` lets one queue hold **different logical message categories**.
- `msgrcv(..., 2, ...)` specifically asks for type `2`.
- Receiving order can therefore differ from sending order.

---

# 6. Semaphores

## 6.1 P / V Semaphore Operations — `semop.c`

```c
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    key_t key = ftok("semfile", 65);

    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    int semid = semget(key, 1, IPC_CREAT | 0666);

    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    /* Initialize semaphore to 1 */
    semctl(semid, 0, SETVAL, 1);

    struct sembuf sops[1];

    /* P / wait operation */
    sops[0].sem_num = 0;
    sops[0].sem_op = -1;
    sops[0].sem_flg = 0;

    printf("Trying to enter critical section...\n");

    if (semop(semid, sops, 1) == -1) {
        perror("semop");
        exit(1);
    }

    printf("In critical section!\n");
    sleep(5);

    /* V / signal operation */
    sops[0].sem_op = 1;

    if (semop(semid, sops, 1) == -1) {
        perror("semop");
        exit(1);
    }

    printf("Exiting critical section.\n");

    return 0;
}
```

### Remember
- `semget()` → create/access semaphore set.
- `semctl(..., SETVAL, 1)` initializes the semaphore to `1`.
- `sem_op = -1` → **P / wait / down**; `sem_op = +1` → **V / signal / up**.

**Compile:**
```bash
gcc semop.c -o semop
```

---

## 6.2 Semaphore API Snippets — from `systemcall.docx`

### `semget()` — Create / Access

```c
int semget(key_t key, int nsems, int semflg);

int semid;

semid = semget(key, 1, IPC_CREAT | 0666);
```

### `semctl()` — Initialize / Read / Remove

```c
int semctl(int semid, int semnum, int cmd, ...);

/* Set value */
semctl(semid, 0, SETVAL, 1);

/* Get value */
int value;
value = semctl(semid, 0, GETVAL);

/* Remove */
semctl(semid, 0, IPC_RMID);
```

### `semop()` — P / V

```c
int semop(int semid, struct sembuf *sops, size_t nsops);

struct sembuf {
    unsigned short sem_num;
    short sem_op;
    short sem_flg;
};
```

### P / Wait

```c
struct sembuf op;

op.sem_num = 0;
op.sem_op = -1;
op.sem_flg = 0;

semop(semid, &op, 1);
```

### V / Signal

```c
struct sembuf op;

op.sem_num = 0;
op.sem_op = 1;
op.sem_flg = 0;

semop(semid, &op, 1);
```

### Remember
- `sem_num` = semaphore index.
- `sem_op` = operation (`-1`, `+1`, or `0`).
- `sem_flg = 0` = normal blocking behavior.

---

# 7. `popen()` / `pclose()`

## 7.1 `popen()` with `"r"` — Read Command Output — `popenwithR.c`

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char buffer[100];

    /* Execute command and read its output */
    fp = popen("ls -l", "r");

    if (fp == NULL) {
        printf("Error opening pipe\n");
        return 1;
    }

    printf("Output received from the command:\n\n");

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    /* Close pipe and wait for command */
    pclose(fp);

    return 0;
}
```

### Remember
- `popen("command", "r")` executes a shell command and lets the program **read its output**.
- `"r"` means the program reads from the command's output stream.
- `pclose()` closes the stream and waits for the command process.

---

## 7.2 `popen()` with `"w"` — Send Input to a Command

> The DOCX states the required `"w"` concept but does not provide a complete implementation. This is the corresponding complete lab-style program.

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char message[100];

    fp = popen("cat > output.txt", "w");

    if (fp == NULL) {
        perror("popen");
        return 1;
    }

    printf("Enter text: ");
    fgets(message, sizeof(message), stdin);

    fprintf(fp, "%s", message);

    pclose(fp);

    printf("Data sent to command successfully.\n");

    return 0;
}
```

### Remember
- `"w"` means the program **writes to the command's standard input**.
- `fprintf(fp, ...)` sends data through the pipe.
- Always use `pclose()` after `popen()`.

---

# 8. UNIX Domain Sockets

## 8.1 UNIX Domain Socket Server — `server.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/real_ipc_socket.sock"
#define BUFFER_SIZE 256

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    /* Create UNIX domain socket */
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Server socket creation failed");
        exit(EXIT_FAILURE);
    }

    /* Remove old socket file */
    unlink(SOCKET_PATH);

    /* Configure address */
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path,
            SOCKET_PATH,
            sizeof(server_addr.sun_path) - 1);

    /* Bind socket to path */
    if (bind(server_fd,
             (struct sockaddr *)&server_addr,
             sizeof(struct sockaddr_un)) < 0) {
        perror("Server bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    /* Listen */
    if (listen(server_fd, 5) < 0) {
        perror("Server listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("==========================================\n");
    printf(" SERVER STARTED: Waiting for a client...\n");
    printf("==========================================\n");

    /* Accept connection */
    client_fd = accept(server_fd, NULL, NULL);

    if (client_fd < 0) {
        perror("Server accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("[Connected] A client has connected!\n");

    /* Receive data */
    memset(buffer, 0, BUFFER_SIZE);

    bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

    if (bytes_received > 0) {
        printf("[Received] Client says: \"%s\"\n", buffer);
    }
    else if (bytes_received == 0) {
        printf("[Disconnected] Client closed connection.\n");
    }
    else {
        perror("Server recv failed");
    }

    /* Clean up */
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

    printf("[Shutdown] Socket cleaned up and server closed.\n");

    return 0;
}
```

### Remember
- Server flow: **`socket()` → `bind()` → `listen()` → `accept()` → `recv()`**.
- `AF_UNIX` is for UNIX local IPC; `SOCK_STREAM` provides stream-based communication.
- `unlink()` removes the socket file from the filesystem.

---

## 8.2 UNIX Domain Socket Client — `client.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/real_ipc_socket.sock"

int main() {
    int client_fd;
    struct sockaddr_un server_addr;
    char message[256];

    /* Create socket */
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (client_fd < 0) {
        perror("Client socket creation failed");
        exit(EXIT_FAILURE);
    }

    /* Configure server address */
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path,
            SOCKET_PATH,
            sizeof(server_addr.sun_path) - 1);

    /* Connect */
    printf("Connecting to server at %s...\n", SOCKET_PATH);

    if (connect(client_fd,
                (struct sockaddr *)&server_addr,
                sizeof(struct sockaddr_un)) < 0) {
        perror("Connection failed! Is the server running?");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected successfully!\n\n");

    /* Read message from user */
    printf("Enter message to send to server: ");

    if (fgets(message, sizeof(message), stdin) != NULL) {
        message[strcspn(message, "\n")] = 0;

        /* Send */
        if (send(client_fd, message, strlen(message), 0) < 0) {
            perror("Send failed");
        } else {
            printf("[Sent] Message dispatched to server.\n");
        }
    }

    close(client_fd);

    printf("Client disconnected and closed.\n");

    return 0;
}
```

### Remember
- Client flow: **`socket()` → `connect()` → `send()` → `close()`**.
- Client and server must use the **same `SOCKET_PATH`**.
- Start the **server first**, then run the client.

---

# 9. Quick Comparison — IPC Programs

| IPC | Main idea | Important calls |
|---|---|---|
| Anonymous Pipe | Related processes communicate through pipe descriptors | `pipe()`, `read()`, `write()`, `close()` |
| FIFO | Named pipe visible in filesystem | `mkfifo()`, `open()`, `read()`, `write()` |
| Message Queue | Messages stored with message types | `msgget()`, `msgsnd()`, `msgrcv()`, `msgctl()` |
| Semaphore | Synchronization / mutual exclusion | `semget()`, `semctl()`, `semop()` |
| `popen()` | Execute command and communicate through a stream | `popen()`, `pclose()` |
| UNIX Socket | Local client-server IPC | `socket()`, `bind()`, `listen()`, `accept()`, `connect()`, `send()`, `recv()` |

---

# 10. Important Syntaxes — LAST-MINUTE CHEAT SHEET

## Pthreads

### Create thread
```c
pthread_create(&thread, NULL, thread_function, arg);
```
**Used for:** creating a new thread.

### Join thread
```c
pthread_join(thread, NULL);
```
**Used for:** waiting for a thread to finish.

### Initialize attributes
```c
pthread_attr_init(&attr);
```

### Get scheduling scope
```c
pthread_attr_getscope(&attr, &scope);
```

### Set scheduling scope
```c
pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
```

---

## Process / Waiting

### Create process
```c
pid_t pid = fork();
```

### Wait for any child
```c
wait(NULL);
```

### Wait for specific child
```c
waitpid(pid, NULL, 0);
```

---

## Pipe

### Create anonymous pipe
```c
int fd[2];
pipe(fd);
```

```text
fd[0] → READ
fd[1] → WRITE
```

### Read
```c
read(fd[0], buffer, size);
```

### Write
```c
write(fd[1], buffer, size);
```

### Close
```c
close(fd);
```

---

## FIFO

### Create named pipe
```c
mkfifo("/tmp/myfifo", 0666);
```

### Open for reading
```c
open("/tmp/myfifo", O_RDONLY);
```

### Open for writing
```c
open("/tmp/myfifo", O_WRONLY);
```

---

## System V Message Queue

### Generate key
```c
key_t key = ftok("progfile", 65);
```

### Create / access queue
```c
msgget(key, 0666 | IPC_CREAT);
```

### Send message
```c
msgsnd(msgid, &msg, sizeof(msg.msg_text), 0);
```

### Receive message
```c
msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0);
```

### Remove queue
```c
msgctl(msgid, IPC_RMID, NULL);
```

### Message structure
```c
struct message {
    long msg_type;
    char msg_text[100];
};
```

> **MCQ trap:** `msg_type` is the first `long`; the `msgsz` argument for `msgsnd()` / `msgrcv()` is the message-data size, **excluding** that `long`.

---

## Semaphores

### Create / access semaphore set
```c
semget(key, 1, IPC_CREAT | 0666);
```

### Initialize
```c
semctl(semid, 0, SETVAL, 1);
```

### Get current value
```c
semctl(semid, 0, GETVAL);
```

### Remove
```c
semctl(semid, 0, IPC_RMID);
```

### Semaphore operation structure
```c
struct sembuf {
    unsigned short sem_num;
    short sem_op;
    short sem_flg;
};
```

### P / Wait
```c
op.sem_num = 0;
op.sem_op = -1;
op.sem_flg = 0;

semop(semid, &op, 1);
```

### V / Signal
```c
op.sem_num = 0;
op.sem_op = 1;
op.sem_flg = 0;

semop(semid, &op, 1);
```

---

## `popen()`

### Open command pipe
```c
FILE *fp = popen("ls -l", "r");
```

### `"r"`
**Program reads command output.**

### `"w"`
**Program writes to command input.**

### Close
```c
pclose(fp);
```

---

## UNIX Domain Socket

### Create socket
```c
socket(AF_UNIX, SOCK_STREAM, 0);
```

### Server bind
```c
bind(server_fd,
     (struct sockaddr *)&server_addr,
     sizeof(server_addr));
```

### Listen
```c
listen(server_fd, 5);
```

### Accept client
```c
accept(server_fd, NULL, NULL);
```

### Client connect
```c
connect(client_fd,
        (struct sockaddr *)&server_addr,
        sizeof(server_addr));
```

### Send
```c
send(client_fd, message, strlen(message), 0);
```

### Receive
```c
recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
```

---

## CPU Affinity / Timing

### Clear CPU set
```c
CPU_ZERO(&set);
```

### Add CPU
```c
CPU_SET(core, &set);
```

### Restrict process to CPU set
```c
sched_setaffinity(0, sizeof(set), &set);
```

### Get monotonic time
```c
clock_gettime(CLOCK_MONOTONIC, &start);
```

### Difference in `timespec`
```c
(end.tv_sec - start.tv_sec) +
(end.tv_nsec - start.tv_nsec) / 1000000000.0
```

---

# 11. Ultra-Fast Function → Purpose

| Function | Meaning |
|---|---|
| `pthread_create()` | Create thread |
| `pthread_join()` | Wait for thread |
| `fork()` | Create child process |
| `wait()` | Wait for child |
| `waitpid()` | Wait for specific child |
| `pipe()` | Create anonymous pipe |
| `mkfifo()` | Create named pipe |
| `ftok()` | Generate IPC key |
| `msgget()` | Create/access message queue |
| `msgsnd()` | Send message |
| `msgrcv()` | Receive message |
| `msgctl()` | Control/remove message queue |
| `semget()` | Create/access semaphore set |
| `semctl()` | Control semaphore |
| `semop()` | Perform P/V operation |
| `popen()` | Execute command + open stream |
| `pclose()` | Close `popen()` stream |
| `socket()` | Create socket |
| `bind()` | Attach server socket to address/path |
| `listen()` | Wait for connections |
| `accept()` | Accept client |
| `connect()` | Connect client to server |
| `send()` | Send socket data |
| `recv()` | Receive socket data |
| `unlink()` | Remove filesystem name/socket file |
| `sched_setaffinity()` | Bind process to CPU core(s) |
| `clock_gettime()` | Get clock/time |
