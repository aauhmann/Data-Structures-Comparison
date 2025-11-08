#pragma once
#include "game.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct nodeBST {
    Game game;
    struct nodeBST *left;
    struct nodeBST *right;
} NodeBST;

typedef struct {
    NodeBST* root;
    int totalComparisons; 
} BST;

/// @brief Creates a new BST
/// @return The new BST
BST bstNew();

/// @brief Inserts a new game into the BST
/// @param tree 
/// @param game 
/// @return 0 == ERROR, 1 == SUCCESS
bool bstInsert(BST* tree, Game game);

/// @brief Counts the number of nodes from a BST
/// @param tree 
/// @return Number of nodes
int bstCountNodes(BST tree);

/// @brief Prints a BST
/// @param tree 
void bstPrint(BST tree);

/// @brief Calculate the height of a BST
/// @param tree 
/// @return Height of the BST
int bstHeight(BST tree);

/// @brief Clears a BST
/// @param tree 
/// @return 0 == ERROR, 1 == SUCCESS
int bstClear(BST* tree);

/// @brief Prints numNodes, height, numRotations, numComparisons
/// @param tree 
void bstPrintResults(BST tree);

/// @brief Prints numNodes, height, numRotations, numComparisons on the file
/// @brief ### FILE MUST BE ALREADY OPEN ON W MODE
/// @param tree 
/// @param file 
void bstFilePrintResults(BST tree, FILE* file);

//FUNÇÃO VIVIANE
NodeBST* bstSearch(BST* tree, char* key);