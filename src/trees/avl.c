#include "avl.h"
#include "strcompare.h"
#include <stdlib.h>

AVL avlNew() {
    return (AVL){NULL, 0, 0}; // Returns an empty splay tree with zeroed counters
}

static bool avlEmpty(AVL tree) {
    return (tree.root == NULL); // Returns true in case the tree is empty and false in case it isn't
}

static int avlNodeHeight(NodeAVL* node) {
    if (node == NULL)
        return 0;
    
    int leftHeight = avlNodeHeight(node->left);
    int rightHeight = avlNodeHeight(node->right);
    
    if (leftHeight > rightHeight)
        return leftHeight + 1;
    else // rightHeight > leftHeight
        return rightHeight + 1;
}

int avlTreeHeight(AVL tree) {
    return avlNodeHeight(tree.root);
}

static int avlNodeFactorBalance(NodeAVL* node) {
    if (node == NULL) return 0;

    int leftHeight = avlNodeHeight(node->left);
    int rightHeight = avlNodeHeight(node->right);

    node->factor = leftHeight - rightHeight;

    return node->factor;
}

static void avlTreeFactorRecursion(NodeAVL* node, int* maxFactor) {
    if (node == NULL) return;

    if (abs(node->factor) > abs(*maxFactor))
        *maxFactor = node->factor;
        
    avlTreeFactorRecursion(node->left, maxFactor);
    avlTreeFactorRecursion(node->right, maxFactor);
}

int avlTreeFactor(AVL tree) {
    if (avlEmpty(tree)) {
        printf("\nEmpty AVL tree");
        return 0;
    }

    int maxFactor = 0;
    avlTreeFactorRecursion(tree.root, &maxFactor);

    return maxFactor;
}

static void avlRotateRight(NodeAVL* child, NodeAVL* node, NodeAVL* father) {
    node->left = child->right;
    child->right = node;
    child->factor = 0;

    if (father != NULL) {
        // Parent conncetion
        if (node == father->left)
            father->left = child;
        else // If (node == father->right)
            father->right = child;
    }
}

static void avlRotateLeft(NodeAVL* child, NodeAVL* node, NodeAVL* father) {
    node->right = child->left;
    child->left = node;
    child->factor = 0;

    if (father != NULL) {
        // Parent connection
        if (node == father->left)
            father->left = child;
        else // If (node == father->right)
            father->right = child;
    }
}

static void avlRotation(NodeAVL* node, NodeAVL* father, AVL* tree) {
    if (node->factor > 1) {
        if (node->left->factor > 0) {
            // Right rotation case
            if (node == tree->root) tree->root = node->left; // Updates tree's root
            avlRotateRight(node->left, node, father);
            tree->numRotations++;
        }
        else { // If node->left->factor < 0
            // Double right rotation case
            if (node == tree->root) tree->root = node->left->right; // Updates tree's root
            avlRotateLeft(node->left->right, node->left, node);
            avlRotateRight(node->left, node, father);
            tree->numRotations += 2;
        }
    }
    else { // node->factor < -1
        if (node->right->factor < 0) {
            // Left rotation case
            if (node == tree->root) tree->root = node->right; // Updates tree's root
            avlRotateLeft(node->right, node, father);
            tree->numRotations++;
        }
        else { // If node->right->factor > 0
            // Double left rotation case
            if (node == tree->root) tree->root = node->right->left; // Updates tree's root
            avlRotateRight(node->right->left, node->right, node);
            avlRotateLeft(node->right, node, father);
            tree->numRotations += 2;
        }
    }
}

static NodeAVL* avlInsertRecursion(NodeAVL* node, NodeAVL* father, type key, AVL* tree) {
    if (node == NULL) {
        // Empty space to insertion
        NodeAVL* new = (NodeAVL*)malloc(sizeof(NodeAVL));
        new->left = new->right = NULL;
        new->key = key;
        new->factor = 0;

        // Father connection
        if (strcompare(key.name, father->key.name) < 0)
            father->left = new;
        else // If key > father->key
            father->right = new;

        return new;
    }
    
    if (strcompare(key.name, node->key.name) == 0) // Node with key found
        return NULL;
    
    NodeAVL* aux;

    if (strcompare(key.name, node->key.name) < 0)
        aux = avlInsertRecursion(node->left, node, key, tree);
    else // If (key > node->key)
        aux = avlInsertRecursion(node->right, node, key, tree);

    // If a node has been inserted
    if (aux != NULL) {
        avlNodeFactorBalance(node);

        // Case the node's unbalanced
        if (abs(node->factor) > 1)
            avlRotation(node, father, tree);
    }

    return aux;
}

void avlInsert(AVL* tree, type key) {    
    if (avlEmpty(*tree)) {
        // Empty tree case
        NodeAVL* new = (NodeAVL*)malloc(sizeof(NodeAVL));
        new->left = new->right = NULL;
        new->key = key;
        tree->root = new;
        return;
    }

    NodeAVL* aux = avlInsertRecursion(tree->root, NULL, key, tree);

    if (aux == NULL)
        return;
}

NodeAVL* avlNodeGet(AVL tree, type key) {
    if (avlEmpty(tree)) {
        // Checks if the avl tree is empty
        printf("\nEmpty AVL tree");
        return NULL;
    }

    NodeAVL* aux = tree.root;

    // Stops if the node isn't on the tree or if it's found
    while (aux != NULL && strcompare(key.name, aux->key.name) != 0) {
        if (strcompare(key.name, aux->key.name) < 0)
            aux = aux->left;
        else // If key > aux->right
            aux = aux->right;
    }

    if (aux == NULL) // Node with key not found
        return NULL;

    // Node with the key is found
    return aux;
}

static int avlNodeDepth(AVL tree, type key) {
    int depth = 1;

    NodeAVL* aux = tree.root;

    while (aux != NULL && strcompare(key.name, aux->key.name) != 0) {
        if (strcompare(key.name, aux->key.name) < 0)
            aux = aux->left;
        else
            aux = aux->right;

        depth++;
    }

    if (aux == NULL) // Node not found
        return -1;
    else // Node found
        return depth;
}

void avlNodePrintDepth(AVL tree, type key) {
    if (avlEmpty(tree)) {
        // Checks if the avl tree is empty
        printf("\nEmpty AVL tree");
        return;
    }

    int depth = avlNodeDepth(tree, key);

    if (depth == -1)
        return;

    printf("Depth: %d", depth);
}

void avlNodePrint(AVL tree, type key) {
    if (avlEmpty(tree)) {
        // Checks if the avl tree is empty
        printf("\nEmpty AVL tree");
        return;
    }

    NodeAVL* aux = tree.root;

    while (aux != NULL && strcompare(key.name, aux->key.name) != 0) {
        if (strcompare(key.name, aux->key.name) < 0)
            aux = aux->left;
        else // If key > aux->key
            aux = aux->right;
    }

    if (aux == NULL) {
        printf("\nKey %s not in the tree", key.name);
        return;
    }

    printf("Key: %s", aux->key.name);
}

static void avlPrintRecursive(NodeAVL* node, int depth) {
    if (node != NULL) {
        for (int i = 0; i < depth; i++) // Prints '=' equal times to the node's depth count
            printf("=");

        avlNodeFactorBalance(node);

        printf("%s", node->key.name);
        printf(" - FB: %d", node->factor);
        if (node->left != NULL) printf(" (%s", node->left->key.name);
        else printf(" (Nan");
        if (node->right != NULL) printf(", %s)", node->right->key.name);
        else printf(", Nan)");
        printf("\n");

        avlPrintRecursive(node->left, depth + 1);
        avlPrintRecursive(node->right, depth + 1);
    }
}

void avlPrint(AVL tree) {
    if (avlEmpty(tree))
        printf("\n\nEmpty AVL tree");
    else {
        printf("\n\n----- PRINTING AVL TREE -----\n");
        avlPrintRecursive(tree.root, 1);
    }
}

static void avlClearRecursion(NodeAVL* node) {
    if (node != NULL) {
        avlClearRecursion(node->left);
        avlClearRecursion(node->right);

        free(node);
    }
}

void avlClear(AVL* tree) {
    if (avlEmpty(*tree)) {
        // Checks if the tree is empty
        printf("\nEmpty AVL tree");
        return;
    }

    avlClearRecursion(tree->root);

    tree->root = NULL;
    tree->totalComparisons = tree->numRotations = 0;
}

static int avlCountNodesRecur(NodeAVL* a) {

    if (a == NULL) return 0;

    return avlCountNodesRecur(a->left) + avlCountNodesRecur(a->right) + 1;
    
}

int avlCountNodes(AVL tree) {

    return avlCountNodesRecur(tree.root);
    
}

void avlPrintResults(AVL tree) {
    printf("\n\n----- AVL -----");
    printf("\nNum nodos: %d", avlCountNodes(tree));
    printf("\nAltura: %d", avlTreeHeight(tree));
    printf("\nNum rotacoes: %d", tree.numRotations);
    printf("\nComparacoes: %d", tree.totalComparisons);
}

void avlFilePrintResults(AVL tree, FILE* file) {
    fprintf(file, "\n\n----- AVL -----");
    fprintf(file, "\nNum nodos: %d", avlCountNodes(tree));
    fprintf(file, "\nAltura: %d", avlTreeHeight(tree));
    fprintf(file, "\nNum rotacoes: %d", tree.numRotations);
    fprintf(file, "\nComparacoes: %d", tree.totalComparisons);
}

NodeAVL* avlSearch(AVL* tree, char* key) {

    NodeAVL* aux = tree->root;

    while (aux != NULL){

        tree->totalComparisons++;

        if (!strcompare(aux->key.name, key))
            return aux;
        else {
            if (strcompare(aux->key.name, key) > 0)
                aux = aux->left;
            else
                aux = aux->right;
        }

    }
    return NULL;
}