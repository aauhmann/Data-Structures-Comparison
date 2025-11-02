#include "rbtree.h"

int comp[4];
int rotations[4];

int main() {

    RB tree = rbInitialize();

    if(!leandroInsert2(&tree, (Game) {"20", 1})) printf("erro");
    if(!leandroInsert2(&tree, (Game) {"25", 1})) printf("erro2");
    if(!leandroInsert2(&tree, (Game) {"30", 1})) printf("erro2");
    if(!leandroInsert2(&tree, (Game) {"35", 1})) printf("erro2");
    if(!leandroInsert2(&tree, (Game) {"40", 1})) printf("erro2");
    if(!leandroInsert2(&tree, (Game) {"45", 1})) printf("erro2");
    if(!leandroInsert2(&tree, (Game) {"21", 1})) printf("erro2");
    if(!leandroInsert2(&tree, (Game) {"50", 1})) printf("erro2");
    if(!leandroInsert2(&tree, (Game) {"42", 1})) printf("erro2");

    //printf("\n%s", tree.root->right->right->key.name);

    rbtPrint(tree);



    return 0;
}