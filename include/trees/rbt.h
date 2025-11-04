#pragma once

#include "game.h"
#include <stdbool.h>
#include <stdio.h>

typedef enum { RED, BLACK } Color;

typedef Game type;

typedef struct nodeRBT {
    Game key;
    Color color;
    struct nodeRBT* parent;
    struct nodeRBT* left;
    struct nodeRBT* right;
} NodeRBT;

typedef struct {
    NodeRBT* root;
} RBT;

/// @brief Creates a new RBT
/// @return The new RBT
RBT rbtNew();

/// @brief Inserts a new game into the RBT
/// @param tree 
/// @param game 
/// @return true(success)/false(error)
bool rbtInsert(RBT* tree, Game game);

/// @brief Counts the number of nodes from a RBT
/// @param tree 
/// @return Number of nodes
int rbtCountNodes(RBT tree);

/// @brief Prints a RBT
/// @param tree 
void rbtPrint(RBT tree);

/// @brief Calculate the height of a RBT
/// @param tree 
/// @return Height of the RBT
int rbtHeight(RBT tree);

/// @brief Clears a RBT
/// @param tree 
/// @return true(success)/false(error)
bool rbtClear(RBT* tree);

/// @brief Prints numNodes, height, numRotations, numComparisons
/// @param tree 
void rbtPrintResults(RBT tree);

/// @brief Prints numNodes, height, numRotations, numComparisons on the file
/// @brief ### FILE MUST BE ALREADY OPEN ON W MODE
/// @param tree 
/// @param file 
void rbtFilePrintResults(RBT tree, FILE* file);

//FUNÇÃO VIVIANE
NodeRBT* rbtSearch(NodeRBT* root, char* key);
