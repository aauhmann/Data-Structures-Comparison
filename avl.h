#pragma once

#include "game.h"
#include <stdbool.h>

extern int rotations;

typedef Game type;

typedef struct NodeAVL {
    Game key;
    int factor;
    int depth;
    struct NodeAVL* parent;
    struct NodeAVL* left;
    struct NodeAVL* right;
} NodeAVL;

typedef struct AVL {
    NodeAVL* root;
    int height;
} AVL;

typedef void (*NodeFunction)(NodeAVL*);

AVL avlInitialize();
bool avlEmpty(AVL tree);
void avlInsert(AVL* tree, type key);
NodeAVL* avlSearchByKey(AVL* tree, type key, NodeFunction function);
void avlInOrder(NodeAVL* root, NodeFunction function);
void avlPreOrder(NodeAVL* root, NodeFunction function);
void avlPostOrder(NodeAVL* root, NodeFunction function);
void avlHeight(AVL* tree);
void avlPrint(AVL tree);
void avlClear(AVL* tree);
void avlFactor(NodeAVL* root, int* factor);
void avlPrintResults(AVL tree);
int avlCountNodes(AVL tree);
NodeAVL* avlConsulta(NodeAVL *a, char *chave);