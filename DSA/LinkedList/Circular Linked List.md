```c
#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node *next;
};

typedef struct node node_t;

struct clist
{
    node_t *head;
};

typedef struct clist clist_t;


// Create a new node
node_t *create_node(int data)
{
    node_t *temp = (node_t *)malloc(sizeof(node_t));

    temp->data = data;

    return temp;
}


// Initialize the circular list
void init_list(clist_t *ptr_clist)
{
    ptr_clist->head = NULL;
}


// Insert a node
void insert(clist_t *ptr_clist, int data)
{
    node_t *temp = create_node(data);

    if (ptr_clist->head == NULL)
    {
        temp->next = temp;
        ptr_clist->head = temp;
    }
    else
    {
        temp->next = ptr_clist->head->next;
        ptr_clist->head->next = temp;
    }
}


// Display the circular list
void display(clist_t *ptr_clist)
{
    node_t *pres = ptr_clist->head;

    if (pres == NULL)
    {
        printf("List is empty\n");
        return;
    }

    node_t *temp = pres;

    printf("Circular List: ");

    do
    {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    while (temp != pres);

    printf("\n");
}


// Main
int main()
{
    clist_t list;

    init_list(&list);

    insert(&list, 10);
    insert(&list, 20);
    insert(&list, 30);
    insert(&list, 40);

    display(&list);

    return 0;
}
```