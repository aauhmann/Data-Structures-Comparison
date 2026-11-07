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
    int totalComparisons;
    int numRotations;
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

//FUNÇÃO VIVIANE
NodeRBT* rbtSearch(RBT* tree, char* key);
