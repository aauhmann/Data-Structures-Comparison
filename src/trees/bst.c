#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include "strcompare.h"
#include <string.h>

#include <ctype.h>

BST bstNew() {
    return (BST){NULL, 0};
}

static NodeBST* bstInsertNode(NodeBST* node, Game game) {

    if (node == NULL) {
        node = (NodeBST*) malloc(sizeof(NodeBST));
        node->game = game;
        node->left = NULL;
        node->right = NULL;
    }

    else if (strcompare(node->game.name, game.name) > 0) {
        node->left = bstInsertNode(node->left, game);
    }
    else {
        node->right = bstInsertNode(node->right, game);
    }

    return node;
}

bool bstInsert(BST* tree, Game game) {

    if (tree == NULL) return false;
    tree->root = bstInsertNode(tree->root, game);
    return tree->root ? true : false;
}

static int bstCountNodesRecur(NodeBST* a) {

    if (a == NULL) return 0;

    return bstCountNodesRecur(a->left) + bstCountNodesRecur(a->right) + 1;
    
}

int bstCountNodes(BST tree) {

    return bstCountNodesRecur(tree.root);
    
}

static void bstPrintRecur(NodeBST* a, int cont) {

    if (a == NULL) return;

    for (int i = 0; i <= cont; i++) printf("=");
    
    printf("%s\n", a->game.name);

    if (a->left != NULL) bstPrintRecur(a->left, cont + 1);

    if (a->right != NULL) bstPrintRecur(a->right, cont + 1);
}

void bstPrint(BST tree) {

    bstPrintRecur(tree.root, 0);
}

static int bstHeightRecur(NodeBST *a){

    if (a == NULL) return 0;

    int leftHeight = bstHeightRecur(a->left);
    int rightHeight = bstHeightRecur(a->right);

    if (leftHeight > rightHeight) return (1 + leftHeight);
    else return (1 + rightHeight);

}

int bstHeight(BST tree) {
    return bstHeightRecur(tree.root);
}

static int bstClearRecur(NodeBST* node) {

    if (node == NULL) return 1;

    int a = bstClearRecur(node->right);
    a += bstClearRecur(node->left);

    free(node->game.name);
    free(node);

    return a;

}

int bstClear(BST* tree) {
    
    if (tree == NULL) return 0;

    NodeBST* removed = tree->root;
    tree->root = NULL;

    return bstClearRecur(removed);

}

void bstPrintResults(BST tree) {

    int numNodes = bstCountNodes(tree);
    int height = bstHeight(tree);
    int numRotations = 0;

    printf("\n----- ABP -----");
    printf("\nNum nodos: %d", numNodes);
    printf("\nAltura: %d", height);
    printf("\nNum rotacoes: %d", numRotations);
    printf("\nComparacoes: %d", tree.totalComparisons);
}

void bstFilePrintResults(BST tree, FILE* file) {

    int numNodes = bstCountNodes(tree);
    int height = bstHeight(tree);
    int numRotations = 0;

    fprintf(file, "----- ABP -----");
    fprintf(file, "\nNum nodos: %d", numNodes);
    fprintf(file, "\nAltura: %d", height);
    fprintf(file, "\nNum rotacoes: %d", numRotations);
    fprintf(file, "\nComparacoes: %d", tree.totalComparisons);
}

NodeBST* bstSearch(BST* tree, char* key) {

    NodeBST* aux = tree->root;

    while (aux != NULL){

        tree->totalComparisons++;

        if (!strcompare(aux->game.name, key))
             return aux;
        else {
            if (strcompare(aux->game.name, key) > 0)
                aux = aux->left;
            else
                aux = aux->right;
        }
    }

    return NULL;
}
