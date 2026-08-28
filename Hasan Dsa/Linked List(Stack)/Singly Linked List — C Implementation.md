## 1. `linked_list.c`

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

struct node *insertAtBeginning(struct node *head, int value)
{
    struct node *newNode = createNode(value);

    if(newNode == NULL)
        return head;

    if(head == NULL)
        return newNode;

    newNode->next = head;
    head = newNode;

    return head;
}

struct node *insertAtEnd(struct node *head, int value)
{
    struct node *newNode = createNode(value);

    if(newNode == NULL)
        return head;

    if(head == NULL)
        return newNode;

    struct node *curr = head;

    while(curr->next != NULL)
    {
        curr = curr->next;
    }

    curr->next = newNode;

    return head;
}

struct node *insertAtPos(struct node *head, int value, int pos)
{
    if(pos < 1)
        return head;

    if(pos == 1)
    {
        return insertAtBeginning(head, value);
    }

    if(head == NULL)
        return head;

    struct node *newNode = createNode(value);

    if(newNode == NULL)
        return head;

    struct node *curr = head;
    int i = 1;

    while(i < pos - 1 && curr->next != NULL)
    {
        curr = curr->next;
        i++;
    }

    if(i != pos - 1)
    {
        free(newNode);
        return head;
    }

    newNode->next = curr->next;
    curr->next = newNode;

    return head;
}

struct node *insertKey(struct node *head, int key, int value)
{
    if(head == NULL)
        return head;

    if(head->data == key)
        return insertAtBeginning(head, value);

    struct node *newNode = createNode(value);

    if(newNode == NULL)
        return head;

    struct node *curr = head;

    while(curr->next != NULL && curr->next->data != key)
    {
        curr = curr->next;
    }

    if(curr->next == NULL)
    {
        free(newNode);
        return head;
    }

    newNode->next = curr->next;
    curr->next = newNode;

    return head;
}

struct node *insertOrdered(struct node *head, int value)
{
    struct node *newNode = createNode(value);
    if(newNode == NULL)
        return head;

    if(head == NULL || value <= head->data)
    {
        newNode->next = head;
        return newNode;
    }

    struct node *curr = head;
    while(curr->next != NULL && curr->next->data <= value)
    {
        curr = curr->next;
    }

    newNode->next = curr->next;
    curr->next = newNode;

    return head;
    
}

struct node *deleteAtBeginning(struct node *head)
{
    if(head == NULL)
        return NULL;

    struct node *temp = head->next;

    free(head);

    return temp;
}

struct node *deleteAtEnd(struct node *head)
{
    if(head == NULL) return NULL;
    if(head->next == NULL)
    {
        free(head);
        return NULL;
    }

    struct node *curr = head;
    while(curr->next->next != NULL)
    {
        curr = curr->next;
    }

    free(curr->next);
    curr->next = NULL;
    return head;
}

struct node *deleteAtPos(struct node *head, int pos)
{
    if(head == NULL ||pos < 1) return head;
    if(pos == 1) return deleteAtBeginning(head);

    struct node *curr = head;
    int i = 1;
    while(i < pos - 1 && curr->next != NULL)
    {
        curr = curr->next;
        i++;
    }

    if(curr->next == NULL)
        return head;

    struct node *temp = curr->next;
    curr->next = temp->next;
    free(temp);
    return head;
}

struct node *deleteKey(struct node *head, int key)
{
    if(head == NULL)
        return NULL;

    if(head->data == key)
    {
        return deleteAtBeginning(head);
    }

    struct node *curr = head;

    while(curr->next != NULL && curr->next->data != key)
    {
        curr = curr->next;
    }

    if(curr->next == NULL)
        return head;

    struct node *temp = curr->next;
    curr->next = temp->next;
    free(temp);
    return head;
}

struct node *deleteAll(struct node *head)
{
    struct node *temp;

    while (head != NULL)
    {
        temp = head->next;
        free(head);
        head = temp;
    }

    return NULL;
}

void display(struct node *head)
{
    struct node *curr = head;

    while(curr != NULL)
    {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }

    printf("NULL\n");
}
```

## 2. `main.c`

```c
#include <stdio.h>
#include "linked_list.h"

int main()
{
    struct node *head = NULL;

    int choice;
    int value;
    int pos;
    int key;

    printf("Linked List Program\n");

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

                head = insertAtBeginning(head, value);
                break;

            case 2:
                printf("Enter value: ");
                scanf("%d", &value);

                head = insertAtEnd(head, value);
                break;

            case 3:
                printf("Enter value: ");
                scanf("%d", &value);

                printf("Enter position: ");
                scanf("%d", &pos);

                head = insertAtPos(head, value, pos);
                break;

            case 4:
                printf("Enter value: ");
                scanf("%d", &value);

                printf("Enter key before which new node is to be inserted: ");
                scanf("%d", &key);

                head = insertKey(head, key, value);
                break;

            case 5:
                printf("Enter value: ");
                scanf("%d", &value);

                head = insertOrdered(head, value);
                break;

            case 6:
                head = deleteAtBeginning(head);
                break;

            case 7:
                head = deleteAtEnd(head);
                break;

            case 8:
                printf("Enter position: ");
                scanf("%d", &pos);

                head = deleteAtPos(head, pos);
                break;

            case 9:
                printf("Enter key to delete: ");
                scanf("%d", &key);

                head = deleteKey(head, key);
                break;

            case 10:
                head = deleteAll(head);
                break;

            case 11:
                display(head);
                break;

            case 12:
                head = deleteAll(head);
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
```

## 3. `linked_list.h`

```c
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct node
{
    int data;
    struct node *next;
};

struct node *createNode(int value);
struct node *insertAtBeginning(struct node *head, int value);
struct node *insertAtEnd(struct node *head, int value);
struct node *insertAtPos(struct node *head, int value, int pos);
struct node *insertKey(struct node *head, int key, int value);
struct node *insertOrdered(struct node *head, int value);
struct node *deleteAtBeginning(struct node *head);
struct node *deleteAtEnd(struct node *head);
struct node *deleteAtPos(struct node *head, int pos);
struct node *deleteKey(struct node *head, int key);
struct node *deleteAll(struct node * head);
void display(struct node *head);

#endif
```

---

## Key Points

- **`createNode`** centralizes malloc + NULL-check; every insert function reuses it, so a failed allocation never corrupts the list.
- **Beginning/End operations**: `insertAtBeginning`/`deleteAtBeginning` are O(1); `insertAtEnd`/`deleteAtEnd` are O(n) since there's no tail pointer — they must walk to the last node.
- **Position-based ops** (`insertAtPos`, `deleteAtPos`) are 1-indexed; `insertAtPos` walks to `pos - 1` and validates `i == pos - 1` to guard against an out-of-range position (fails safely by freeing `newNode`).
- **Key-based ops** (`insertKey`, `deleteKey`) search by value, not position; `insertKey` inserts **before** the matching key (special-cased when the key is at `head`), and both silently no-op if the key isn't found.
- **`insertOrdered`** assumes the list is already sorted; it finds the first node whose value exceeds the new value and inserts just before it — breaks if called on an unsorted list.
- **Memory safety**: every delete function `free()`s the removed node exactly once; `deleteAll` iterates with a `temp` pointer to avoid a use-after-free while freeing `head`.
- **Return-value convention**: every insert/delete function returns the (possibly new) `head` — callers must always reassign `head = insertX(...)` / `head = deleteX(...)`, since a straight `insertX(head, ...)` silently drops the update.
- **`main.c`** is a menu-driven CLI wrapping every list operation via `scanf` + `switch`; option 12 frees the whole list (`deleteAll`) before exiting to avoid leaks.
- **`linked_list.h`** uses the standard `#ifndef` include guard and exposes only the function prototypes + `struct node`, keeping `main.c` decoupled from the implementation.