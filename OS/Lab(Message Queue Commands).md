#OS #IPC #SystemProgramming #C

# Message Queues (System V IPC)

A **message queue** is a linked list of messages stored in the kernel, identified by a **message queue identifier**. It allows processes to communicate by sending and receiving discrete, typed messages — unlike pipes, messages don't have to be read in FIFO order (they can be picked by type).

Related: [[Shared Memory (IPC)]], [[Pipes (IPC)]], [[Semaphores (IPC)]]

---

## Headers required

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
```

## Message buffer structure

Every message queue program defines a structure like this — the first member **must** be a `long` (the message type):

```c
struct mesg_buffer {
    long mesg_type;
    char mesg_text[BUF_SIZE];
} message;
```

- `mesg_type` — used to filter/select messages on receive
- `mesg_text` — the actual payload

---

## Key generation — `ftok()`

```c
key_t key = ftok("progfile", 65);
```

- Generates a unique **IPC key** from a pathname (`"progfile"` must exist) and a project id (`65`, any non-zero int).
- Both the sender and receiver must use the **same pathname + id** to arrive at the same key → same queue.

---

## `msgget()` — create/access a queue

```c
int msgid = msgget(key, 0666 | IPC_CREAT);
```

- Returns the message queue identifier for the given `key`.
- `0666` — permission bits (rw for owner/group/others)
- `IPC_CREAT` — create the queue if it doesn't already exist

---

## `msgsnd()` — send a message

```c
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
```

|Parameter|Meaning|
|---|---|
|`msqid`|the queue identifier (1st param) — which queue to send to|
|`msgp`|pointer to the message buffer struct containing the message|
|`msgsz`|length in bytes of the message **text** being sent (not including `mesg_type`)|
|`msgflg`|action to take if the message can't be sent immediately (e.g. `0` = block, `IPC_NOWAIT` = return error instead of blocking)|

Usage from `msgque_write.c`:

```c
message.mesg_type = 1;
msgsnd(msgid, &message, sizeof(message), 0);
```

---

## `msgrcv()` — receive a message

```c
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
```

|Parameter|Meaning|
|---|---|
|`msqid`|the queue identifier (1st param) — which queue to receive from|
|`msgp`|pointer to the message buffer struct to fill|
|`msgsz`|max length in bytes of the message text to receive|
|`msgtyp`|selects **which** message to read from the queue (see below)|
|`msgflg`|flags controlling behavior (e.g. blocking)|

### `msgtyp` selection rules

- `msgtyp == 0` → receive the **first** message in the queue (regardless of type)
- `msgtyp > 0` → receive the first message **of that exact type**
- `msgtyp < 0` → receive the message with the **lowest type** that is ≤ `|msgtyp|`

Usage from `msgque_read.c`:

```c
msgrcv(msgid, &message, sizeof(message), 1, 0);
```

→ reads the first message of type `1`.

---

## `msgctl()` — control / remove a queue

```c
msgctl(msgid, IPC_RMID, NULL);
```

- `IPC_RMID` removes the message queue from the kernel. Should be called by (typically) the reader once communication is done, otherwise the queue persists after the program exits.

---

## Full example — Writer (`msgque_write.c`)

```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#define MAX 10
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
    printf("Message being sent :%s\n", message.mesg_text);

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);

    message.mesg_type = 1;
    msgsnd(msgid, &message, sizeof(message), 0);

    sleep(60);   // keeps process alive so queue isn't orphaned instantly
    return 0;
}
```

## Full example — Reader (`msgque_read.c`)

```c
#include <stdio.h>
#include <sys/types.h>
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

    msgrcv(msgid, &message, sizeof(message), 1, 0);
    printf("Message received: %s\n", message.mesg_text);

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
```

---

## Execution flow

1. Compile both: `gcc msgque_write.c -o writer` and `gcc msgque_read.c -o reader`
2. Run **writer** first in one terminal — it creates the queue, sends the message, then sleeps 60s.
3. Run **reader** in another terminal — it attaches to the same queue (via `ftok`), reads the message of type `1`, prints it, then destroys the queue.

> [!note] Ordering `msgget` with `IPC_CREAT` works whether or not the queue exists yet — so either process can technically run first and create the queue, but the reader will block on `msgrcv` until a matching message arrives.

## Quick recap table

|Function|Purpose|
|---|---|
|`ftok()`|generate a unique key from a file path + id|
|`msgget()`|create or get a message queue for a key|
|`msgsnd()`|send a message into the queue|
|`msgrcv()`|receive a message from the queue (filtered by type)|
|`msgctl()`|control the queue (e.g. `IPC_RMID` to delete it)|

