#include <stdio.h>
#include "heap.h"
#include "queue.h"

int main()
{
    struct heap h;
    struct queue q;

    init(&h);
    init_queue(&q);

    int opt;
    int key;
    int n;
    int a[MAXSIZE + 1];

    printf("\n1 : Heap insert");
    printf("\n2 : Heap delete max");
    printf("\n3 : Heap peek max");
    printf("\n4 : Display heap");
    printf("\n5 : Build heap bottom-up");
    printf("\n6 : Build heap top-down");
    printf("\n7 : Queue enqueue");
    printf("\n8 : Queue dequeue");
    printf("\n9 : Display queue");
    printf("\n0 : Exit");

    printf("\n\nEnter option : ");
    scanf("%d", &opt);

    while(opt != 0)
    {
        switch(opt)
        {
            case 1:
                printf("Enter key : ");
                scanf("%d", &key);

                insert(&h, key);
                disp(&h);
                break;

            case 2:
                if(h.n == 0)
                    printf("heap empty\n");
                else
                {
                    key = delete_max(&h);
                    printf("deleted : %d\n", key);
                    disp(&h);
                }
                break;

            case 3:
                if(h.n == 0)
                    printf("heap empty\n");
                else
                    printf("maximum : %d\n", peek_max(&h));
                break;

            case 4:
                disp(&h);
                break;

            case 5:
            case 6:
                printf("Enter number of elements : ");
                scanf("%d", &n);

                if(n < 0 || n > MAXSIZE)
                {
                    printf("invalid heap size\n");
                    break;
                }

                printf("Enter elements : ");

                for(int i = 1; i <= n; ++i)
                    scanf("%d", &a[i]);

                if(opt == 5)
                {
                    build_heap(&h, a, n);
                    printf("Bottom-up heap constructed\n");
                }
                else
                {
                    build_heap_top_down(&h, a, n);
                    printf("Top-down heap constructed\n");
                }

                disp(&h);
                break;

            case 7:
                if(is_full(&q))
                    printf("queue full\n");
                else
                {
                    printf("Enter key : ");
                    scanf("%d", &key);

                    enqueue(&q, key);
                    disp_queue(&q);
                }
                break;

            case 8:
                if(is_empty(&q))
                    printf("queue empty\n");
                else
                {
                    key = dequeue(&q);
                    printf("dequeue : %d\n", key);
                    disp_queue(&q);
                }
                break;

            case 9:
                disp_queue(&q);
                break;

            default:
                printf("invalid option\n");
        }

        printf("\nEnter option : ");
        scanf("%d", &opt);
    }

    deinit(&h);
    deinit_queue(&q);

    return 0;
}