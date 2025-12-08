#include "splay.h"
#include "strcompare.h"
#include <stdlib.h>

int postInsertionHeight = 0;

SPT sptNew() {
    return (SPT){NULL, 0, 0}; // Returns an empty splay tree with zeroed rotations and comparisons counters
}

static bool sptEmpty(SPT tree) {
    return tree.root == NULL; // Returns true in case the tree is empty and false in case it isn't
}

static int sptHeightRecursive(NodeSPT* node) {
    if (node == NULL)
        return 0;
    
    int leftHeight = sptHeightRecursive(node->left);
    int rightHeight = sptHeightRecursive(node->right);
    
    if (leftHeight > rightHeight)
        return leftHeight + 1;
    else // rightHeight > leftHeight
        return rightHeight + 1;
}

int sptHeight(SPT tree) {
    return sptHeightRecursive(tree.root);
}

static void sptZig(NodeSPT* aux, NodeSPT* node, NodeSPT* father) {
    node->left = aux->right;
    aux->right = node;
    if (father != NULL) { 
        if (node == father->left)
            father->left = aux;
        else // If (node == father->right)
            father->right = aux;
    }
}

static void sptZag(NodeSPT* aux, NodeSPT* node, NodeSPT* father) {
    node->right = aux->left;
    aux->left = node;
    if (father != NULL) { 
        if (node == father->left)
            father->left = aux;
        else // If (node == father->right)
            father->right = aux;
    }
}

static void sptSplaying(NodeSPT* aux, NodeSPT* node, NodeSPT* father) {
    if (aux == node->left)
        sptZig(aux, node, father);
    else // If (aux == node->right)
        sptZag(aux, node, father);
}

static void sptSplit(SPT* tree, SPT auxTrees[]) {
    auxTrees[0] = (SPT){tree->root, tree->numRotations, tree->totalComparisons};
    auxTrees[1] = (SPT){tree->root->right, 0, 0};

    // Cuts the connection between the new trees
    auxTrees[0].root->right = NULL;
}

static SPT sptJoin(SPT auxTrees[], NodeSPT* root) {
    SPT tree = (SPT){root, auxTrees[0].numRotations, auxTrees[0].totalComparisons};

    if (root != NULL)
        // Connects right tree to the new root
        root->right = auxTrees[1].root;
    else
        tree.root = auxTrees[1].root;

    return tree;
}

static NodeSPT* sptAccessSearch(NodeSPT* node, NodeSPT* father, char* key, int* rotations, int* comparisons) {
    if (node == NULL) // Node with data not found
        return father;
        
    if (comparisons != NULL) // Just increments 'comparisons' when sptAccessSearch is called by sptSearch
        (*comparisons)++;

    if (strcompare(key, node->data.name) == 0) // Node with data found
        return node;
    
    NodeSPT* aux;


    if (strcompare(key, node->data.name) < 0) 
        aux = sptAccessSearch(node->left, node, key, rotations, comparisons);
    else // If (data > node->data)
        aux = sptAccessSearch(node->right, node, key, rotations, comparisons);

    if (aux != node) {
        sptSplaying(aux, node, father);
        (*rotations)++;
    }

    return aux;
}

NodeSPT* sptSearch(SPT* tree, char* key) {
    if (sptEmpty(*tree)) {
        // Checks if the splay tree is empty
        printf("\nEmpty splay tree");
        return NULL;
    }

    int rotation = 0;
    NodeSPT* aux = sptAccessSearch(tree->root, NULL, key, &rotation, &tree->totalComparisons);

    // Fixes the Splay rotations number
    if (rotation%2 == 0)
        rotation = rotation/2;
    else
        rotation = (rotation - 1)/2 + 1;

    tree->numRotations += rotation;

    tree->root = aux;

    if (strcompare(key, aux->data.name) != 0) // Node with data not found
        return NULL;

    // Node with the data is found
    return aux;
}

void sptInsert(SPT* tree, type data) {
    NodeSPT* new = (NodeSPT*)malloc(sizeof(NodeSPT));

    new->left = new->right = NULL;
    new->data = data;

    if (sptEmpty(*tree)) {
        // Empty tree case
        tree->root = new;
        return;
    }

    int rotation = 0;
    NodeSPT* aux = sptAccessSearch(tree->root, NULL, data.name, &rotation, NULL);

    // Fixes the Splay rotations number
    if (rotation%2 == 0)
        rotation = rotation/2;
    else
        rotation = (rotation - 1)/2 + 1;

    tree->numRotations += rotation;

    tree->root = aux;

    if (strcompare(data.name, aux->data.name) == 0)
        return;

    SPT auxTrees[2]; // Array of the auxiliar trees
    sptSplit(tree, auxTrees);
    // printf("\nSuccesfull split");

    // Case when the new node data would be inserted as the left child of what would be his father (now the left tree's root)
    if (strcompare(data.name, auxTrees[0].root->data.name) < 0) {
        auxTrees[0].root->right = auxTrees[1].root;
        auxTrees[1].root = auxTrees[0].root;
        auxTrees[0].root = auxTrees[0].root->left;
        auxTrees[1].root->left = NULL;
    }

    new->left = auxTrees[0].root;
    auxTrees[0].root = new;

    *tree = sptJoin(auxTrees, new); // Reconnects the auxiliar trees
    // printf("\nSuccessfull join");
}

static int sptNodeDepth(SPT tree, type data) {
    int depth = 1;

    NodeSPT* aux = tree.root;

    while (aux != NULL && strcompare(data.name, aux->data.name) != 0) {
        if (strcompare(data.name, aux->data.name) < 0)
            aux = aux->left;
        else // If data > aux->data
            aux = aux->right;

        depth++;
    }

    if (aux == NULL) // Node not found
        return -1;
    else // Node found
        return depth;
}

void sptNodePrintDepth(SPT tree, type data) {
    if (sptEmpty(tree)) {
        // Checks if the splay tree is empty
        printf("\nEmpty splay tree");
        return;
    }

    int depth = sptNodeDepth(tree, data);

    if (depth == -1) {
        printf("\nNode named %s not in the tree", data.name);
        return;
    }

    printf("Depth: %d", depth);
}

void sptNodePrint(SPT tree, type data) {
    if (sptEmpty(tree)) {
        // Checks if the splay tree is empty
        printf("\nEmpty splay tree");
        return;
    }

    NodeSPT* aux = tree.root;

    while (aux != NULL && strcompare(data.name, aux->data.name) != 0) {
        if (strcompare(data.name, aux->data.name) < 0)
            aux = aux->left;
        else // If data > aux->data
            aux = aux->right;
    }

    if (aux == NULL) {
        printf("\nNode named %s not in the tree", data.name);
        return;
    }

    printf("Name: %s", aux->data.name);
    printf("Time: %.2f", aux->data.time);
}

void sptNodeDelete(SPT* tree, type data) {
    if (sptEmpty(*tree)) {
        // Checks if the splay tree is empty
        printf("\nEmpty splay tree");
        return;
    }

    NodeSPT* aux = sptSearch(tree, data.name); // The 'soon to be' deleted node
    
    // Ends function in case the accessed node isn't the one with the desired data
    if (strcompare(data.name, aux->data.name) != 0)
        return;

    // Case there's only one node on the tree
    if (aux->right == NULL && aux->left == NULL) {
        free(aux);
        printf("\nNode with name %s (tree root) deleted", data.name);
        tree->root = NULL;
        return;
    }

    SPT auxTrees[2]; // Array of the auxiliar trees
    sptSplit(tree, auxTrees);
    // printf("\nSuccesfull split");

    auxTrees[0].root = aux->left;
    free(aux); // Deletes node

    NodeSPT* newRoot = auxTrees[0].root;
    if (newRoot != NULL) {
        // Stops when finds the rightmost node in the left tree
        while (newRoot->right != NULL)
            // Searches for node with max data (the rightmost node)
            newRoot = newRoot->right;
        // printf("\nOut of rightmost node while");

        sptAccessSearch(auxTrees[0].root, NULL, newRoot->data.name, &tree->numRotations, NULL); // newRoot becomes the left tree's root
        // printf("\nOut of join splay");
    }

    *tree = sptJoin(auxTrees, newRoot); // Reconnects the auxiliar trees
    // printf("\nSuccessfull join");

    printf("\nNode with name %s deleted", data.name);
}

static void sptPrintRecursive(NodeSPT* node, int depth) {
    if (node != NULL) {
        for (int i = 0; i < depth; i++) // Prints '=' equal times to the node's depth count
            printf("=");

        printf("%s", node->data.name);
        if (node->left != NULL) printf(" (%s", node->left->data.name);
        else printf(" (-");
        if (node->right != NULL) printf(", %s)", node->right->data.name);
        else printf(", -)");
        printf("\n");

        sptPrintRecursive(node->left, depth + 1);
        sptPrintRecursive(node->right, depth + 1);
    }
}

void sptPrint(SPT tree) {
    if (sptEmpty(tree))
        printf("\n\nEmpty splay tree");
    else {
        printf("\n\n----- PRINTING SPLAY TREE -----\n");
        sptPrintRecursive(tree.root, 1);
    }
}

static void sptClearRecursive(NodeSPT* node) {
    if (node != NULL) {
        sptClearRecursive(node->left);
        sptClearRecursive(node->right);

        free(node);
    }
}

void sptClear(SPT* tree) {
    if (sptEmpty(*tree)) {
        // Checks if the tree is empty
        printf("\nEmpty splay tree");
        return;
    }

    sptClearRecursive(tree->root);

    tree->root = NULL;
}

static int sptCountNodesRecursive(NodeSPT* a) {
    if (a == NULL)
        return 0;

    return sptCountNodesRecursive(a->left) + sptCountNodesRecursive(a->right) + 1;
    
}

int sptCountNodes(SPT tree) {
    return sptCountNodesRecursive(tree.root);
}

void sptPrintResults(SPT tree) {
    printf("\n\n----- SPLAY -----");
    printf("\nNum nodos: %d", sptCountNodes(tree));
    printf("\nAltura pos insercao: %d", postInsertionHeight);
    printf("\nAltura final: %d", sptHeight(tree));
    printf("\nNum rotacoes: %d", tree.numRotations);
    printf("\nComparacoes: %d", tree.totalComparisons);
}

void sptFilePrintResults(SPT tree, FILE* file) {
    fprintf(file, "\n\n----- SPLAY -----");
    fprintf(file, "\nNum nodos: %d", sptCountNodes(tree));
    fprintf(file, "\nAltura: %d", sptHeight(tree));
    fprintf(file, "\nNum rotacoes: %d", tree.numRotations);
    fprintf(file, "\nComparacoes: %d", tree.totalComparisons);
}