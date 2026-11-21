#include <stdio.h>
#include "tree_array.h"

int main()
{
    // create and insert
    struct tree t1;
    init(&t1);

    int a1[] = {40, 20, 60, 10, 70, 30, 50};
    int n1 = 7;
    for(int i = 0; i < n1; ++i)
    {
        insert(&t1, a1[i]);
    }

    disp(&t1);

    // number of nodes and leaves
    printf("# of nodes : %d\n", number_of_nodes(&t1));
    printf("# of leaves : %d\n", number_of_leaves(&t1));

    // traversals
    disp_preorder(&t1);
    disp_postorder(&t1);
    disp_inorder(&t1);

    // copy and compare trees
    struct tree t2;
    init(&t2);
    copy(&t1, &t2);
    disp(&t2);
    printf("same : %d\n", are_same(&t1, &t2));

    // create another tree and compare
    struct tree t3;
    init(&t3);

    int a3[] = {10, 20, 30, 40, 50, 60, 70};
    int n3 = 7;
    for(int i = 0; i < n3; ++i)
    {
        insert(&t3, a3[i]);
    }

    disp(&t3);
    printf("same : %d\n", are_same(&t1, &t3));

    // empty tree
    struct tree t4;
    init(&t4);
    disp(&t4);

    printf("# of nodes : %d\n", number_of_nodes(&t4));
    printf("# of leaves : %d\n", number_of_leaves(&t4));

    deinit(&t1);
    deinit(&t2);
    deinit(&t3);
    deinit(&t4);

    return 0;
}