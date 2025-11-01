#pragma once

#include "game.h"
#include <stdbool.h>
#include <stdio.h>

typedef enum {RED, BLACK} Color;

typedef Game type;

typedef struct NodeRB {
    Game key;
    short int color;
    int depth;
    struct NodeRB* parent;
    struct NodeRB* left;
    struct NodeRB* right;
} NodeRB;

typedef struct RB {
    NodeRB* root;
    int height;
} RB;

typedef void (*NodeFunctionRB)(NodeRB*);

RB rbInitialize();
bool rbEmpty(RB tree);
void rbInsert(RB* tree, type key);
NodeRB* rbSearchByKey(RB* tree, type key, NodeFunctionRB function);
void rbInOrder(NodeRB* root, NodeFunctionRB function);
void rbPreOrder(NodeRB* root, NodeFunctionRB function);
void rbPostOrder(NodeRB* root, NodeFunctionRB function);
void rbHeight(RB* tree);
void rbPrint(RB tree);
void rbClear(RB* tree);
void rbPrintResults(RB tree);
void rbFilePrintResults(RB tree, FILE* file);
int rbCountNodes(RB tree);
NodeRB* rbConsulta(NodeRB *a, char *chave);