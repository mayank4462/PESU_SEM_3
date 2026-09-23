# OS Unit 1 — Programs + Shell + Syntax Cheat Sheet

> Code-first revision. Learn the code, then the 2–3 points immediately below it.

---

# 1. CPU Scheduling Programs

## 1.1 FCFS — First Come First Serve

```c
#include <stdio.h>
#include <string.h>

int main()
{
    char pn[10][10], t[10];
    int arr[10], bur[10], star[10], finish[10];
    int tat[10], wt[10], i, j, n, temp;
    int totwt = 0, tottat = 0, time = 0;

    printf("Enter the number of processes to schedule:");
    scanf("%d", &n);

    for (i = 0; i < n; i++)
    {
        printf("Enter the Process Name, Arrival Time and Burst Time of the processes:");
        scanf("%s %d %d", pn[i], &arr[i], &bur[i]);
    }

    /* Sort according to arrival time */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (arr[i] < arr[j])
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;

                temp = bur[i];
                bur[i] = bur[j];
                bur[j] = temp;

                strcpy(t, pn[i]);
                strcpy(pn[i], pn[j]);
                strcpy(pn[j], t);
            }
        }
    }

    printf("\nProcessName\tArrivalTime\tBurstTime\tWaitTime\tStart\tTurnAroundTime\tFinish");
    printf("\n-----------\t-----------\t---------\t--------\t-----\t--------------\t------");

    for (i = 0; i < n; i++)
    {
        star[i] = (time < arr[i]) ? arr[i] : time;
        wt[i] = star[i] - arr[i];
        finish[i] = star[i] + bur[i];
        tat[i] = finish[i] - arr[i];
        time = finish[i];

        printf("\n%s\t\t%3d\t\t%3d\t\t%3d\t\t%3d\t%6d\t\t%6d",
               pn[i], arr[i], bur[i], wt[i], star[i], tat[i], finish[i]);

        totwt += wt[i];
        tottat += tat[i];
    }

    printf("\n\nAverage Waiting time:%f", (float)totwt / n);
    printf("\nAverage Turn Around Time:%f\n", (float)tottat / n);

    return 0;
}
```

**Remember**
- Sort by `Arrival Time`; then execute in that order.
- `WT = Start - Arrival`, `TAT = Finish - Arrival`.
- `star = max(time, arrival)` handles CPU idle time.

---

## 1.2 SJF — Shortest Job First (Non-Preemptive)

```c
#include <stdio.h>
#include <string.h>

void main()
{
    int et[20], at[10], n, i, j, temp;
    int st[10], ft[10], wt[10], ta[10];
    int totwt = 0, totta = 0;
    char pn[10][10], t[10];

    printf("Enter the number of processes to schedule:");
    scanf("%d", &n);

    for (i = 0; i < n; i++)
    {
        printf("Enter the process name, arrival time and execution time of each process:");
        scanf("%s %d %d", pn[i], &at[i], &et[i]);
    }

    /* Sort according to execution/burst time */
    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (et[i] > et[j])
            {
                temp = at[i];
                at[i] = at[j];
                at[j] = temp;

                temp = et[i];
                et[i] = et[j];
                et[j] = temp;

                strcpy(t, pn[i]);
                strcpy(pn[i], pn[j]);
                strcpy(pn[j], t);
            }
        }
    }

    printf("\nProcess\tArrivalTime\tExecutionTime\tWaitingTime\tTurnAroundTime");
    printf("\n-------\t-----------\t-------------\t-----------\t--------------");

    for (i = 0; i < n; i++)
    {
        st[i] = (i == 0) ? at[i] : (ft[i - 1] > at[i] ? ft[i - 1] : at[i]);
        wt[i] = st[i] - at[i];
        ft[i] = st[i] + et[i];
        ta[i] = ft[i] - at[i];

        totwt += wt[i];
        totta += ta[i];

        printf("\n%s\t%5d\t\t%5d\t\t%5d\t\t%5d",
               pn[i], at[i], et[i], wt[i], ta[i]);
    }

    printf("\nAverage waiting time is: %f", (float)totwt / n);
    printf("\nAverage turnaround time is: %f\n", (float)totta / n);
}
```

**Remember**
- Sort by smallest execution/burst time.
- `ST = max(previous FT, AT)`; then `FT = ST + BT`.
- ⚠️ The supplied program sorts the entire list by burst time; it does **not** dynamically select the shortest job only from the currently arrived ready queue.

---

## 1.3 SRTF — Shortest Remaining Time First

```c
#include <stdio.h>

int main()
{
    int a[10], b[10], x[10];
    int i, smallest, count = 0, time, n;
    int wait = 0, tat = 0, end;
    char pn[10][10];

    printf("Enter the number of processes to schedule:\n");
    scanf("%d", &n);

    for (i = 0; i < n; i++)
    {
        printf("Enter the process name, arrival time and burst time for each process:\n");
        scanf("%s %d %d", pn[i], &a[i], &b[i]);
        x[i] = b[i];
    }

    b[9] = 9999;

    printf("Process\tburst-time\tarrival-time\twaiting-time\tturnaround-time\tcompletion-time\n");
    printf("-------\t----------\t------------\t------------\t---------------\t---------------\n");

    for (time = 0; count != n; time++)
    {
        smallest = 9;

        for (i = 0; i < n; i++)
        {
            if (a[i] <= time && b[i] < b[smallest] && b[i] > 0)
                smallest = i;
        }

        if (--b[smallest] == 0)
        {
            count++;
            end = time + 1;

            wait += end - a[smallest] - x[smallest];
            tat += end - a[smallest];

            printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
                   pn[smallest], x[smallest], a[smallest],
                   end - a[smallest] - x[smallest],
                   end - a[smallest], end);
        }
    }

    printf("\n\nAverage waiting time = %lf\n", (float)wait / n);
    printf("Average Turnaround time = %lf\n", (float)tat / n);

    return 0;
}
```

**Remember**
- At every time unit, choose the process with the **smallest remaining burst** among arrived processes.
- `x[]` stores the original burst time; `b[]` is reduced every time unit.
- A process completes when `--b[smallest] == 0`.

---

## 1.4 Priority Scheduling — Non-Preemptive

### Supplied `Priority.txt`

```c
#include <stdio.h>
#include <string.h>

int main()
{
    char pn[10][10], t[10];
    int arr[10], bur[10], star[10], finish[10];
    int tat[10], wt[10], i, j, n, temp;
    int totwt = 0, tottat = 0, time = 0;

    printf("Enter the number of processes to schedule:");
    scanf("%d", &n);

    for (i = 0; i < n; i++)
    {
        printf("Enter the Process Name, Arrival Time and Burst Time of the processes:");
        scanf("%s %d %d", pn[i], &arr[i], &bur[i]);
    }

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (arr[i] < arr[j])
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;

                temp = bur[i];
                bur[i] = bur[j];
                bur[j] = temp;

                strcpy(t, pn[i]);
                strcpy(pn[i], pn[j]);
                strcpy(pn[j], t);
            }
        }
    }

    printf("\nProcessName\tArrivalTime\tBurstTime\tWaitTime\tStart\tTurnAroundTime\tFinish");
    printf("\n-----------\t-----------\t---------\t--------\t-----\t--------------\t------");

    for (i = 0; i < n; i++)
    {
        star[i] = (time < arr[i]) ? arr[i] : time;
        wt[i] = star[i] - arr[i];
        finish[i] = star[i] + bur[i];
        tat[i] = finish[i] - arr[i];
        time = finish[i];

        printf("\n%s\t\t%3d\t\t%3d\t\t%3d\t\t%3d\t%6d\t\t%6d",
               pn[i], arr[i], bur[i], wt[i], star[i], tat[i], finish[i]);

        totwt += wt[i];
        tottat += tat[i];
    }

    printf("\n\nAverage Waiting time:%f", (float)totwt / n);
    printf("\nAverage Turn Around Time:%f\n", (float)tottat / n);

    return 0;
}
```

**Remember**
- ⚠️ The supplied `Priority.txt` is identical to the FCFS program and has **no priority input or priority comparison**.
- Do **not** memorize this file as a genuine Priority Scheduling implementation.
- For an actual Priority program, the code must store a priority value and select the appropriate arrived process according to the stated priority rule.

---

## 1.5 Round Robin

```c
#include <stdio.h>

void main()
{
    int i, NOP, sum = 0, count = 0, y, quant;
    int wt = 0, tat = 0, at[10], bt[10], temp[10];
    float avg_wt, avg_tat;

    printf("Enter the total number of process to schedule: ");
    scanf("%d", &NOP);

    y = NOP;

    for (i = 0; i < NOP; i++)
    {
        printf("\nEnter the Arrival and Burst time of the Process[%d]: ", i + 1);
        scanf("%d %d", &at[i], &bt[i]);
        temp[i] = bt[i];
    }

    printf("Enter the Time Quantum for the process: ");
    scanf("%d", &quant);

    printf("\nProcess No\t\tBurst Time\t\tTAT\t\tWaiting Time ");

    for (sum = 0, i = 0; y != 0; )
    {
        if (temp[i] <= quant && temp[i] > 0)
        {
            sum = sum + temp[i];
            temp[i] = 0;
            count = 1;
        }
        else if (temp[i] > 0)
        {
            temp[i] = temp[i] - quant;
            sum = sum + quant;
        }

        if (temp[i] == 0 && count == 1)
        {
            y--;

            printf("\nProcess No[%d]\t\t%d\t\t\t%d\t\t\t%d",
                   i + 1, bt[i], sum - at[i], sum - at[i] - bt[i]);

            wt = wt + sum - at[i] - bt[i];
            tat = tat + sum - at[i];
            count = 0;
        }

        if (i == NOP - 1)
            i = 0;
        else if (at[i + 1] <= sum)
            i++;
        else
            i = 0;
    }

    avg_tat = (float)tat / NOP;
    avg_wt = (float)wt / NOP;

    printf("\nAverage Turn Around Time: %f", avg_tat);
    printf("\nAverage Waiting Time: %f\n", avg_wt);
}
```

**Remember**
- `temp[]` = remaining burst time; `bt[]` = original burst time.
- `y` = number of unfinished processes; loop continues while `y != 0`.
- If `temp[i] <= quant`, process finishes; otherwise subtract the quantum and move on.

### RR Main Loop — The Part to Memorize

```c
for (sum = 0, i = 0; y != 0; )
{
    if (temp[i] <= quant && temp[i] > 0)
    {
        sum = sum + temp[i];
        temp[i] = 0;
        count = 1;
    }
    else if (temp[i] > 0)
    {
        temp[i] = temp[i] - quant;
        sum = sum + quant;
    }

    if (temp[i] == 0 && count == 1)
    {
        y--;
        wt = wt + sum - at[i] - bt[i];
        tat = tat + sum - at[i];
        count = 0;
    }

    if (i == NOP - 1)
        i = 0;
    else if (at[i + 1] <= sum)
        i++;
    else
        i = 0;
}
```

**Remember**
- `sum` = current CPU time.
- Completion: `temp[i] == 0 && count == 1`.
- `TAT = sum - at[i]`, `WT = TAT - bt[i]`.

---

# 2. Process Management Programs

## 2.1 `wait()` + `SIGCHLD`

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void handler(int sig)
{
    printf("Child terminated.\n");
    wait(NULL);     // Collect child's exit status
}

int main()
{
    signal(SIGCHLD, handler);

    if (fork() == 0)
    {
        printf("Child Running\n");
        sleep(2);
    }
    else
    {
        while (1);   // Parent keeps running
    }

    return 0;
}
```

**Remember**
- `SIGCHLD` is generated when a child changes state/terminates; the handler calls `wait(NULL)` to collect it.
- `wait(NULL)` waits for any child and ignores the exit-status details.
- `fork() == 0` identifies the child process branch.

---

## 2.2 Zombie Process

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        printf("Child: terminating...\n");
        exit(0);
    }
    else
    {
        printf("Parent: not calling wait()\n");
        sleep(30);
    }

    return 0;
}
```

**Remember**
- Child finishes, but parent does **not** call `wait()` immediately.
- The terminated child remains as a zombie entry until its exit status is collected.
- `ps` can be used to observe process state while testing.

---

## 2.3 Orphan Process

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid = fork();

    if (pid > 0)
    {
        printf("Parent: exiting...\n");
        exit(0);
    }
    else
    {
        sleep(30);
        printf("Child: still running\n");
    }

    return 0;
}
```

**Remember**
- Parent terminates **before** the child.
- Child continues running after its original parent exits.
- The child is adopted by the system's reaper process (commonly `systemd`, PID 1, on modern Linux).

---

## 2.4 `exec()` Family — All Important Forms

### `execl()`

```c
int execl(const char *pathname, const char *arg0, ..., (char *)0);

execl("/bin/ls", "ls", "-l", NULL);
```

### `execv()`

```c
int execv(const char *pathname, char *const argv[]);

char *args[] = {"ls", "-l", NULL};
execv("/bin/ls", args);
```

### `execle()`

```c
int execle(const char *pathname,
           const char *arg0, ...,
           (char *)0,
           char *const envp[]);

execle("/bin/ls", "ls", "-l", NULL, envp);
```

### `execve()`

```c
int execve(const char *pathname,
           char *const argv[],
           char *const envp[]);

char *args[] = {"ls", "-l", NULL};
execve("/bin/ls", args, envp);
```

### `execlp()`

```c
int execlp(const char *filename,
           const char *arg0,
           ...,
           NULL);

execlp("ls", "ls", "-l", NULL);
```

### `execvp()`

```c
int execvp(const char *filename, char *const argv[]);

char *args[] = {"ls", "-l", NULL};
execvp("ls", args);
```

### `fexecve()`

```c
int fexecve(int fd,
            char *const argv[],
            char *const envp[]);

int fd = open("/bin/ls", O_RDONLY);
fexecve(fd, args, envp);
```

**Remember**
- `l` = arguments supplied as a **list**; `v` = arguments supplied as a **vector/array**.
- `p` = search executable using **PATH**; without `p`, use the executable path.
- `e` = explicitly pass a custom **environment**; `execve()` is the standard low-level form with path + array + environment.

---

# 3. Shell Scripting

## 3.1 Basic Shell Script

```bash
#!/bin/bash

echo "Hello World"
echo "Welcome to Shell Scripting"
```

**Remember**
- `#!/bin/bash` = shebang; tells the system to use Bash.
- Give permission with `chmod +x hello.sh` and run using `./hello.sh`.
- `bash hello.sh` also executes the script and does not require `chmod +x`.

### Create → Write → Save → Execute

```bash
vi hello.sh
chmod +x hello.sh
./hello.sh
```

---

## 3.2 `for` Loop

```bash
#!/bin/bash

for i in {1..5}
do
    echo "i = $i"
done
```

**Remember**
- `{1..5}` expands to `1 2 3 4 5`.
- `$i` accesses the current value of the variable.
- Loop body is between `do` and `done`.

---

## 3.3 Nested `for` Loop

```bash
#!/bin/bash

for i in {1..5}
do
    for j in {1..3}
    do
        echo "i = $i and j = $j"
    done
done
```

**Remember**
- Inner loop completes fully for every value of the outer loop.
- Total iterations = `5 × 3 = 15`.
- Each loop has its own `do ... done` block.

---

## 3.4 `if` Condition — File Exists

```bash
#!/bin/bash

if [ -f welcome.txt ]; then
    echo "File exists"
else
    echo "File does not exist"
fi
```

**Remember**
- `-f` checks whether the path is a **regular file**.
- Shell condition uses `[ condition ]`; spacing around `[` and `]` matters.
- `if ... then ... else ... fi` is the basic structure.

---

## 3.5 `break`

```bash
#!/bin/bash

for i in {1..5}
do
    if [ $i -eq 3 ]
    then
        break
    fi

    for j in {1..3}
    do
        echo "i = $i and j = $j"
    done
done
```

**Remember**
- `break` immediately terminates the current loop.
- Here, when `i == 3`, the outer loop ends.
- Therefore only `i = 1` and `i = 2` complete their inner loops.

---

## 3.6 `continue`

```bash
#!/bin/bash

for i in {1..5}
do
    if [ $i -eq 3 ]
    then
        continue
    fi

    for j in {1..3}
    do
        echo "i = $i and j = $j"
    done
done
```

**Remember**
- `continue` skips the rest of the current iteration.
- Here, `i = 3` is skipped, but the loop continues with `i = 4` and `i = 5`.
- `break` = exit loop; `continue` = skip iteration.

---

## 3.7 Cron

```text
* * * * * command
│ │ │ │ │
│ │ │ │ └── Day of week
│ │ │ └──── Month
│ │ └────── Day of month
│ └──────── Hour
└────────── Minute
```

### Example: Every day at 10 PM

```bash
0 22 * * * /home/user/backup.sh
```

### Useful Cron Examples

```bash
*/5 * * * * /home/user/test.sh
0 * * * * /home/user/test.sh
0 9 * * 1 /home/user/test.sh
30 18 * * * /home/user/test.sh
0 22 * * 1-5 /home/user/test.sh
```

### Test Cron Every Minute

```bash
crontab -e
```

Add:

```bash
* * * * * date >> /tmp/cron_test.txt
```

Check:

```bash
cat /tmp/cron_test.txt
crontab -l
```

**Remember**
- Cron order = **Minute → Hour → Day of Month → Month → Day of Week**.
- `*` = every, `-` = range, `,` = specific values, `/` = interval.
- Example: `*/5 * * * *` = every 5 minutes; `0 22 * * 1-5` = 10 PM Monday–Friday.

---

# 4. Linux Commands — Final Command Section

## Navigation

| Command | Use | Example |
|---|---|---|
| `pwd` | Show current directory | `pwd` |
| `cd` | Change directory / return home | `cd Documents` |
| `cd ~` | Go to home directory | `cd ~` |
| `cd ..` | Go to parent directory | `cd ..` |
| `cd -` | Go to previous directory | `cd -` |
| `ls` | List files/directories | `ls` |
| `ls -l` | Long listing | `ls -l` |
| `ls -a` | Include hidden files | `ls -a` |
| `ls -ld` | Details of directory itself | `ls -ld Documents` |

## Files & Directories

| Command | Use | Example |
|---|---|---|
| `touch` | Create empty file | `touch notes.txt` |
| `cat` | Display file contents | `cat notes.txt` |
| `head` | First lines of file | `head file.txt` |
| `tail` | Last lines of file | `tail file.txt` |
| `tail -f` | Follow appended file output | `tail -f log.txt` |
| `cp` | Copy | `cp a.txt b.txt` |
| `mv` | Move / rename | `mv old.txt new.txt` |
| `rm` | Delete file | `rm notes.txt` |
| `mkdir` | Create directory | `mkdir Project` |
| `rmdir` | Remove empty directory | `rmdir Project` |
| `file` | Show file type | `file program` |

## Search & Information

| Command | Use | Example |
|---|---|---|
| `find` | Search for files | `find . -name "report.txt"` |
| `grep` | Search text/pattern | `grep "error" log.txt` |
| `which` | Show executable path from PATH | `which grep` |
| `whereis` | Locate binary/source/man page | `whereis ls` |
| `locate` | Fast filename search | `locate mozilla` |
| `ps` | Show running processes | `ps` |
| `w` | Show logged-in users/activity | `w` |
| `id` | Show user/group IDs | `id` |
| `df -h` | Filesystem disk usage | `df -h` |
| `du -sh` | Directory size summary | `du -sh Documents` |
| `top` | Live process monitor | `top` |
| `free -h` | Memory usage | `free -h` |
| `uname -a` | System information | `uname -a` |

## Utilities

| Command | Use | Example |
|---|---|---|
| `clear` | Clear terminal | `clear` |
| `echo` | Print text | `echo "Hello"` |
| `more` | View output page-by-page | `more file.txt` |
| `less` | Scrollable file/output viewer | `less file.txt` |
| `sort` | Sort lines | `sort file.txt` |
| `man` | Manual page | `man ls` |
| `man -k` | Search man-page descriptions | `man -k permission` |
| `info` | Info documentation | `info df` |
| `history` | Show command history | `history` |
| `su` | Switch user | `su` |
| `wget` | Download from URL | `wget https://example.com/file.zip` |
| `curl` | Transfer data to/from server | `curl https://example.com` |
| `tar` | Create/extract archive | `tar -czf backup.tar.gz folder/` |
| `zip` | Create ZIP archive | `zip archive.zip file.txt` |
| `unzip` | Extract ZIP | `unzip archive.zip` |
| `chmod` | Change permissions | `chmod 755 script.sh` |
| `chown` | Change ownership | `sudo chown user:user file.txt` |
| `sudo` | Run with elevated privileges | `sudo apt update` |
| `apt` | Package management | `sudo apt install git` |

## Pipe & Redirection

```bash
ls -la /usr/bin | less
ls -l /home/user/*.mp3 > mp3files.txt
ls -l /home/user/*.mp3 >> mp3files.txt
```

| Symbol | Meaning |
|---|---|
| `|` | Pipe output of one command into another |
| `>` | Redirect output; overwrite/create file |
| `>>` | Redirect output; append to file |
| `<` | Take input from a file |
| `;` | Run multiple commands on one line |
| `&&` | Run next command only if previous succeeds |
| `&` | Run command in background |

## Shell Special Characters

| Symbol | Meaning |
|---|---|
| `\` | Escape character |
| `/` | Directory separator |
| `.` | Current directory |
| `..` | Parent directory |
| `~` | Home directory |
| `*` | Zero or more characters |
| `?` | Exactly one character |
| `[ ]` | Character/range matching |

---

# 5. Important C / UNIX Syntax — Last-Minute Cheat Sheet

## Process Creation / IDs

| Syntax | Meaning |
|---|---|
| `fork()` | Create a new child process |
| `getpid()` | Get current process PID |
| `getppid()` | Get parent process PID |
| `exit(0)` | Terminate the current process successfully |
| `sleep(n)` | Pause the process for `n` seconds |

## Waiting

| Syntax | Meaning |
|---|---|
| `wait(NULL)` | Wait for any child; ignore exit status |
| `wait(&status)` | Wait for any child and store status |
| `waitpid(pid, NULL, 0)` | Wait for a specific child |
| `waitpid(-1, NULL, 0)` | Wait for any child |
| `waitpid(pid, NULL, WNOHANG)` | Do not block if child has not exited |
| `waitid(P_PID, pid, &info, WEXITED)` | Wait for specific child with detailed status |

## `exec()` Family — Pattern

| Syntax | Meaning |
|---|---|
| `execl(path, arg0, ..., NULL)` | Full path + argument list |
| `execv(path, argv)` | Full path + argument array |
| `execle(path, ..., NULL, envp)` | Full path + list + custom environment |
| `execve(path, argv, envp)` | Full path + array + custom environment |
| `execlp(file, arg0, ..., NULL)` | PATH search + argument list |
| `execvp(file, argv)` | PATH search + argument array |
| `fexecve(fd, argv, envp)` | Execute using an open file descriptor |

### `exec()` Memory Trick

```text
l  → list
v  → vector / array
p  → PATH search
e  → environment
```

## Signal

```c
signal(SIGCHLD, handler);
```

`SIGCHLD` → parent is notified when a child changes state/terminates.

## Common Headers

| Header | Important functions/types |
|---|---|
| `<stdio.h>` | `printf()`, `scanf()` |
| `<unistd.h>` | `fork()`, `sleep()`, `exec*()`, `getpid()` |
| `<sys/wait.h>` | `wait()`, `waitpid()`, `waitid()` |
| `<signal.h>` | `signal()`, `SIGCHLD` |
| `<stdlib.h>` | `exit()` |
| `<string.h>` | `strcpy()` |

## Shell Execution Syntax

```bash
chmod +x script.sh
./script.sh
bash script.sh
```

```bash
crontab -e   # edit cron jobs
crontab -l   # list cron jobs
```

---

# 6. Ultra-Fast Code Triggers

```text
FCFS      → sort by Arrival Time → execute sequentially
SJF       → shortest Burst Time first
SRTF      → shortest Remaining Time at every time unit
RR        → Time Quantum + remaining burst array
Zombie    → child ends + parent does NOT wait
Orphan    → parent ends + child continues
wait()    → wait for child
waitpid() → wait for specific child
exec()    → replace current process image
Shell     → #!/bin/bash + commands
Cron      → * * * * * command
```
