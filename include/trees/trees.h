#pragma once 

#include "bst.h"
#include "avl.h"
#include "rbt.h"
#include "splay.h"

#define NUM_TREES 4

typedef enum {BSTT, AVLT, RBTT, SPLAYT} Trees;

typedef void (*InsertFunction)(void*, Game);
typedef void* (*SearchFunction)(void*, char*);

typedef struct {
    InsertFunction insert;
    SearchFunction search;
} TreeOps;

void bstInsertWrapper(void* tree, Game game);
void avlInsertWrapper(void* tree, Game game);
void rbtInsertWrapper(void* tree, Game game);
void treeInsert(void* tree, Game game, Trees type);

void* bstSearchWrapper(void* tree, char* key);
void* avlSearchWrapper(void* tree, char* key);
void* rbtSearchWrapper(void* tree, char* key);
void* treeSearch(void* tree, char* key, Trees type);


