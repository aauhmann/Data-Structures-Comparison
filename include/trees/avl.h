#pragma once

#include "game.h"
#include <stdbool.h>
#include <stdio.h>

typedef Game type;

typedef struct s_NodeAVL {
    type key;
    int factor;
    struct s_NodeAVL* left;
    struct s_NodeAVL* right;
} NodeAVL;

typedef struct AVL {
    NodeAVL* root;
    int totalComparisons;
    int numRotations;
} AVL;

typedef void (*NodeFunction)(NodeAVL*);

AVL avlNew();
void avlInsert(AVL* tree, type key);
NodeAVL* avlNodeGet(AVL tree, type key);
void avlNodePrintDepth(AVL tree, type key);
void avlNodePrint(AVL tree, type key);
void avlPrint(AVL tree);
int avlTreeHeight(AVL tree);
int avlTreeFactor(AVL tree);
void avlClear(AVL* tree);
void avlPrintResults(AVL tree);
void avlFilePrintResults(AVL tree, FILE* file);
NodeAVL* avlSearch(AVL* tree, char* key);