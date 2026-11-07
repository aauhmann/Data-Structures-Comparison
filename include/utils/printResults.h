#pragma once

#include "trees.h"

void avlPrintResults(AVL tree);
void avlFilePrintResults(AVL tree, FILE* file);

/// @brief Prints numNodes, height, numRotations, numComparisons
/// @param tree 
void bstPrintResults(BST tree);

/// @brief Prints numNodes, height, numRotations, numComparisons on the file
/// @brief ### FILE MUST BE ALREADY OPEN ON W MODE
/// @param tree 
/// @param file 
void bstFilePrintResults(BST tree, FILE* file);

/// @brief Prints numNodes, height, numRotations, numComparisons
/// @param tree 
void rbtPrintResults(RBT tree);

/// @brief Prints numNodes, height, numRotations, numComparisons on the file
/// @brief ### FILE MUST BE ALREADY OPEN ON W MODE
/// @param tree 
/// @param file 
void rbtFilePrintResults(RBT tree, FILE* file);

void sptPrintResults(SPT tree);
void sptFilePrintResults(SPT tree, FILE* file);

void printResults(float* estimatedTime, float* generationTime, float* searchTime, BST bst, AVL avl, RBT rbt, SPT spt);
void fprintResults(FILE* file, float* estimatedTime, float* generationTime, float* searchTime, BST bst, AVL avl, RBT rbt, SPT spt);