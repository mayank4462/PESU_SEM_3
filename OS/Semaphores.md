# System V Semaphores

## `semget()`

`semget()` is used to **create or access a semaphore set**.

```
semget(key, nsems, semflg);
```

### Return value

- Returns a **semaphore set ID (`semid`)** on success.
- Returns `-1` on failure.

### Flags

- `flag = 0`
    - Accesses an **existing semaphore set**.
    - If the semaphore set does not exist, the call fails.
- `flag = IPC_CREAT`
    - Creates a **new semaphore set** if one does not already exist.
    - If the set already exists, it returns the ID of the existing set.
- `flag = IPC_CREAT | IPC_EXCL`
    - Creates a **new semaphore set only if it does not already exist**.
    - Fails if the semaphore set already exists.

> **Note:** `IPC_CREAT` does not always mean "newly created." It creates the set **only if it doesn't already exist**.

---

# `semctl()`

`semctl()` is used to perform **control operations** on a semaphore set.

```
semctl(semid, semnum, cmd, ...);
```

### Parameters

- `semid` → ID of the semaphore set
- `semnum` → Semaphore number within the set
- `cmd` → Operation to perform
- `...` → Optional argument depending on the command

---

## `SETVAL`

Used to **set the value of a particular semaphore**.

```
semctl(semid, 0, SETVAL, 1);
```

Here:

- `semid` → Semaphore set ID
- `0` → Semaphore number 0
- `SETVAL` → Set its value
- `1` → Set semaphore value to `1`

So, this sets **semaphore 0 = 1**.

If we use:

```
semctl(semid, 0, SETVAL, 2);
```

then:

```
semaphore 0 = 2
```

It **does not increment** the value. It **sets** the value to the specified number.

---

# `GETVAL`

Used to **get the current value of a particular semaphore**.

```
semctl(semid, 0, GETVAL);
```

For example, if semaphore 0 currently has value `2`:

```
GETVAL → 2
```

---

# `IPC_RMID`

Used to **remove the entire semaphore set**.

```
semctl(semid, 0, IPC_RMID);
```

This completely removes the semaphore set identified by `semid`.

> The `semnum` argument is ignored for `IPC_RMID`, so `0` is conventionally used.

### Important

```
IPC_RMID → Remove the entire semaphore set
```

It is **not** `PC_RMID`; the correct constant is:

```
IPC_RMID
```

---

# `GETALL`

Used to **get the values of all semaphores** in a semaphore set.

```
semctl(semid, 0, GETALL, array);
```

The values are stored in an array.

For example:

```
Semaphore set:

sem[0] = 1
sem[1] = 0
sem[2] = 2
```

`GETALL` retrieves:

```
[1, 0, 2]
```

---

# `SETALL`

Used to **set the values of all semaphores** in a semaphore set.

```
semctl(semid, 0, SETALL, array);
```

For example:

```
array = [1, 0, 2]
```

After `SETALL`:

```
sem[0] = 1
sem[1] = 0
sem[2] = 2
```

---

# `semop()`

`semop()` is used to **perform operations on semaphores**, mainly for synchronization.

```
semop(semid, &sops, nsops);
```

The operation is specified using:

```
struct sembuf
```

The important field is:

```
sem_op
```

---

## `sem_op = -1` → Wait / Lock

```
sem_op = -1;
```

This is commonly called the **wait** or **P operation**.

It attempts to decrease the semaphore value by `1`.

### Example

```
Semaphore value = 1
sem_op = -1

1 - 1 = 0
```

The process can enter the critical section.

If:

```
Semaphore value = 0
```

then the process **blocks/waits** until the semaphore can be decremented.

---

## `sem_op = +1` → Signal / Unlock

```
sem_op = +1;
```

This is commonly called the **signal** or **V operation**.

It increases the semaphore value by `1`.

### Example

```
Semaphore value = 0
sem_op = +1

0 + 1 = 1
```

This can allow a waiting process to proceed.

---

# Quick Revision

|Function / Operation|Purpose|
|---|---|
|`semget()`|Create/access a semaphore set|
|`SETVAL`|Set value of one semaphore|
|`GETVAL`|Get value of one semaphore|
|`SETALL`|Set values of all semaphores|
|`GETALL`|Get values of all semaphores|
|`IPC_RMID`|Remove entire semaphore set|
|`semop()`|Perform semaphore operations|
|`sem_op = -1`|Wait / Lock / P|
|`sem_op = +1`|Signal / Unlock / V|

### Most important distinction

```
SETVAL  → SET one semaphore's value
SETALL  → SET all semaphore values

GETVAL  → GET one semaphore's value
GETALL  → GET all semaphore values

IPC_RMID → REMOVE the entire semaphore set

sem_op = -1 → WAIT / LOCK
sem_op = +1 → SIGNAL / UNLOCK
```