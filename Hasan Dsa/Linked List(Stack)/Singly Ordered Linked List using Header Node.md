## 1. `linked_list.h`

```c
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct node
{
    int data;
    struct node *next;
};

struct linked_list
{
    struct node *head;
};

struct node *createNode(int value);

void initList(struct linked_list *list);
void insertAtBeginning(struct linked_list *list, int value);
void insertAtEnd(struct linked_list *list, int value);
void insertAtPos(struct linked_list *list, int value, int pos);
void insertKey(struct linked_list *list, int key, int value);
void insertOrdered(struct linked_list *list, int value);
void deleteAtBeginning(struct linked_list *list);
void deleteAtEnd(struct linked_list *list);
void deleteAtPos(struct linked_list *list, int pos);
void deleteKey(struct linked_list *list, int key);
void deleteAll(struct linked_list *list);
void display(struct linked_list *list);

#endif
```

## 2. `linked_list.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

struct node *createNode(int value)
{
    struct node *newNode = (struct node *)malloc(sizeof(struct node));

    if(newNode == NULL)
    {
        printf("Malloc Failed!");
        return NULL;
    }

    newNode->data = value;
    newNode->next = NULL;

    return newNode;
}

void initList(struct linked_list *list)
{
    list->head = (struct node *)malloc(sizeof(struct node));

    if(list->head == NULL)
    {
        printf("Malloc Failed!");
        return;
    }

    list->head->data = 0;
    list->head->next = NULL;
}

void insertAtBeginning(struct linked_list *list, int value)
{
    struct node *newNode = createNode(value);

    if(newNode == NULL)
        return;

    newNode->next = list->head->next;
    list->head->next = newNode;
}

void insertAtEnd(struct linked_list *list, int value)
{
    struct node *newNode = createNode(value);

    if(newNode == NULL)
        return;

    struct node *curr = list->head;

    while(curr->next != NULL)
    {
        curr = curr->next;
    }

    curr->next = newNode;
}

void insertAtPos(struct linked_list *list, int value, int pos)
{
    if(pos < 1)
        return;

    struct node *curr = list->head;
    int i = 0;

    while(i < pos - 1 && curr->next != NULL)
    {
        curr = curr->next;
        i++;
    }

    if(i != pos - 1)
        return;

    struct node *newNode = createNode(value);

    if(newNode == NULL)
        return;

    newNode->next = curr->next;
    curr->next = newNode;
}

void insertKey(struct linked_list *list, int key, int value)
{
    struct node *curr = list->head;

    while(curr->next != NULL && curr->next->data != key)
    {
        curr = curr->next;
    }

    if(curr->next == NULL)
        return;

    struct node *newNode = createNode(value);

    if(newNode == NULL)
        return;

    newNode->next = curr->next;
    curr->next = newNode;
}

void insertOrdered(struct linked_list *list, int value)
{
    struct node *newNode = createNode(value);

    if(newNode == NULL)
        return;

    struct node *curr = list->head;

    while(curr->next != NULL && curr->next->data <= value)
    {
        curr = curr->next;
    }

    newNode->next = curr->next;
    curr->next = newNode;
}

void deleteAtBeginning(struct linked_list *list)
{
    if(list->head->next == NULL)
        return;

    struct node *temp = list->head->next;
    list->head->next = temp->next;
    free(temp);
}

void deleteAtEnd(struct linked_list *list)
{
    if(list->head->next == NULL)
        return;

    struct node *curr = list->head;

    while(curr->next->next != NULL)
    {
        curr = curr->next;
    }

    free(curr->next);
    curr->next = NULL;
}

void deleteAtPos(struct linked_list *list, int pos)
{
    if(pos < 1)
        return;

    struct node *curr = list->head;
    int i = 0;

    while(i < pos - 1 && curr->next != NULL)
    {
        curr = curr->next;
        i++;
    }

    if(i != pos - 1 || curr->next == NULL)
        return;

    struct node *temp = curr->next;
    curr->next = temp->next;
    free(temp);
}

void deleteKey(struct linked_list *list, int key)
{
    struct node *curr = list->head;

    while(curr->next != NULL && curr->next->data != key)
    {
        curr = curr->next;
    }

    if(curr->next == NULL)
        return;

    struct node *temp = curr->next;
    curr->next = temp->next;
    free(temp);
}

void deleteAll(struct linked_list *list)
{
    struct node *curr = list->head;
    struct node *temp;

    while(curr != NULL)
    {
        temp = curr->next;
        free(curr);
        curr = temp;
    }

    list->head = NULL;
}

void display(struct linked_list *list)
{
    struct node *curr = list->head->next;

    while(curr != NULL)
    {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }

    printf("NULL\n");
}
```

## 3. `main.c`

```c
#include <stdio.h>
#include "linked_list.h"

int main()
{
    struct linked_list list;

    initList(&list);

    int choice;
    int value;
    int pos;
    int key;

    printf("Linked List Program (Header Node)\n");

    while(1)
    {
        printf("\n-----------------------------");
        printf("\n1. Insert at beginning");
        printf("\n2. Insert at end");
        printf("\n3. Insert at position");
        printf("\n4. Insert before key");
        printf("\n5. Insert in order");

        printf("\n6. Delete at beginning");
        printf("\n7. Delete at end");
        printf("\n8. Delete at position");
        printf("\n9. Delete key");
        printf("\n10. Delete all");

        printf("\n11. Display linked list");
        printf("\n12. Exit");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("Enter value: ");
                scanf("%d", &value);

                insertAtBeginning(&list, value);
                break;

            case 2:
                printf("Enter value: ");
                scanf("%d", &value);

                insertAtEnd(&list, value);
                break;

            case 3:
                printf("Enter value: ");
                scanf("%d", &value);

                printf("Enter position: ");
                scanf("%d", &pos);

                insertAtPos(&list, value, pos);
                break;

            case 4:
                printf("Enter value: ");
                scanf("%d", &value);

                printf("Enter key before which new node is to be inserted: ");
                scanf("%d", &key);

                insertKey(&list, key, value);
                break;

            case 5:
                printf("Enter value: ");
                scanf("%d", &value);

                insertOrdered(&list, value);
                break;

            case 6:
                deleteAtBeginning(&list);
                break;

            case 7:
                deleteAtEnd(&list);
                break;

            case 8:
                printf("Enter position: ");
                scanf("%d", &pos);

                deleteAtPos(&list, pos);
                break;

            case 9:
                printf("Enter key to delete: ");
                scanf("%d", &key);

                deleteKey(&list, key);
                break;

            case 10:
                deleteAll(&list);
                initList(&list);
                break;

            case 11:
                display(&list);
                break;

            case 12:
                deleteAll(&list);
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
```

---

## Key Points

- **Sentinel/dummy head design**: `list->head` is always a real, allocated node (`data = 0`, unused) that never holds actual list data — it exists purely to simplify insertion/deletion logic. The real data starts at `list->head->next`.
- **No head-special-casing**: because of the sentinel, `insertAtPos`/`insertKey` never need an "is this the first node" branch (unlike the plain `head`-pointer version) — `curr` always starts at a valid non-NULL node (`list->head`), so the loop and pointer-relinking logic is uniform for every insert position, including the very front.
- **Position-based ops** are 1-indexed relative to real data; `curr` starts at the sentinel (treated as position 0) and walks `pos - 1` steps, so `pos == 1` naturally lands right after the sentinel with no extra branch.
- **Key-based ops** (`insertKey`, `deleteKey`) search by value, not position; both walk from the sentinel and silently no-op if the key isn't found — inserting before a key at the very front works the same as inserting before any other key.
- **`insertOrdered`** assumes the list is already sorted; walks while `curr->next->data <= value`, so new duplicates land after existing equal values (stable ordering) — breaks if called on an unsorted list.
- **Memory safety**: every delete function `free()`s the removed node exactly once; `deleteAll` frees starting from `list->head` itself (not `->next`), since the sentinel was heap-allocated in `initList` and must be freed too.
- **`deleteAll` leaves the struct unusable**: it sets `list->head = NULL` afterward, so calling `insertX`/`display` again would crash — `initList` must be called again first (see `main.c` option 10, which re-inits right after clearing).
- **Struct-based API / return-value convention flip**: instead of passing/returning `struct node *head` everywhere and requiring the caller to reassign (`head = insertX(...)`), the whole list is wrapped in `struct linked_list` and passed by pointer — every function returns `void` and mutates `list->head`/nodes directly. This removes the "forgot to reassign head" footgun from the plain-pointer version.
- **`createNode`** is still centralized and reused by every insert function (including `insertOrdered`, refactored to use it) — a failed allocation never corrupts the list.
- **Trade-off vs. the non-sentinel version**: simpler, more uniform code (no special-casing for position 1 / key-at-front), at the cost of one extra always-allocated node and slightly less obvious "empty list" semantics (empty means `head->next == NULL`, not `head == NULL`).
- **Coverage**: this version implements the full op set — `insertAtBeginning/End/Pos`, `insertKey`, `insertOrdered`, `deleteAtBeginning/End/Pos`, `deleteKey`, `deleteAll`, `display` — matching the original non-sentinel implementation one-for-one, just restructured around the header node.