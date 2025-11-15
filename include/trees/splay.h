#pragma once

#include "game.h"
#include <stdbool.h>
#include <stdio.h>

typedef Game type;

typedef struct s_SplayNode {
    type data;
    struct s_SplayNode* left;
    struct s_SplayNode* right;
} NodeSPT;

typedef struct s_SplayTree {
    NodeSPT* root;
    int totalComparisons;
    int numRotations;
} SPT;

SPT sptNew();
void sptInsert(SPT* tree, type data);
NodeSPT* sptSearch(SPT* tree, char* key);
void sptNodePrintDepth(SPT tree, type data);
void sptNodePrint(SPT tree, type data);
void sptPrint(SPT tree);
int sptHeight(SPT tree);
void sptNodeDelete(SPT* tree, type data);
void sptClear(SPT* tree);
void sptPrintResults(SPT tree);
void sptFilePrintResults(SPT tree, FILE* file);