#include "trees.h"

TreeOps ops[] = { 
    {bstInsertWrapper, bstSearchWrapper},
    {avlInsertWrapper, avlSearchWrapper},
    {rbtInsertWrapper, rbtSearchWrapper} 
};

void bstInsertWrapper(void* tree, Game game) {
    bstInsert((BST*)tree, game);
}

void avlInsertWrapper(void* tree, Game game) {
    avlInsert((AVL*)tree, game);
}

void rbtInsertWrapper(void* tree, Game game) {
    rbtInsert((RBT*)tree, game);
}

void treeInsert(void* tree, Game game, Trees type) {
    ops[type].insert(tree, game);
}

void* bstSearchWrapper(void* tree, char* key) {
    return bstSearch((BST*)tree, key);
}

void* avlSearchWrapper(void* tree, char* key) {
    return avlSearch((AVL*)tree, key);
}

void* rbtSearchWrapper(void* tree, char* key) {
    return rbtSearch((RBT*)tree, key);
}

void* treeSearch(void* tree, char* key, Trees type) {
    return ops[type].search(tree, key);
}