#pragma once

#include "game.h"
#include <stdbool.h>
#include <stdio.h>

typedef Game type;

typedef struct NodeAVL {
    Game key;
    int factor;
    struct NodeAVL* parent;
    struct NodeAVL* left;
    struct NodeAVL* right;
} NodeAVL;

typedef struct AVL {
    NodeAVL* root;
    int totalComparisons;
    int numRotations;
} AVL;

typedef void (*NodeFunctionAVL)(NodeAVL*);

AVL avlInitialize();
bool avlEmpty(AVL tree);
void avlInsert(AVL* tree, type key);
NodeAVL* avlSearchByKey(AVL* tree, type key, NodeFunctionAVL function);
void avlInOrder(NodeAVL* root, NodeFunctionAVL function);
void avlPreOrder(NodeAVL* root, NodeFunctionAVL function);
void avlPostOrder(NodeAVL* root, NodeFunctionAVL function);
void avlHeight(AVL* tree);
void avlPrint(AVL tree);
void avlClear(AVL* tree);
void avlFactor(NodeAVL* root, int* factor);
void avlPrintResults(AVL tree);
void avlFilePrintResults(AVL tree, FILE* file);
int avlCountNodes(AVL tree);
NodeAVL* avlSearch(AVL* tree, char* key);