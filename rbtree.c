#include "rbtree.h"
#include "global.h"
#include "strcompare.h"
#include <stdlib.h>

void node_print(NodeRB* node);
void node_depthAndColor(NodeRB* node);
void node_depthDec(NodeRB* node);
void node_depthInc(NodeRB* node);
void node_clear(NodeRB* node);
void subtree_maxDepth(NodeRB* root, int* maxDepth);
void subtree_rotateRight(RB* tree, NodeRB* root);
void subtree_rotateLeft(RB* tree, NodeRB* root);
void subtree_doubleRotateRight(RB* tree, NodeRB* root);
void subtree_doubleRotateLeft(RB* tree, NodeRB* root);

RB rbInitialize() {
    return (RB){NULL, 0};
}

bool rbEmpty(RB tree) {
    if (tree.root == NULL) return true;
    else return false;
}

static void rbChangeColor(NodeRB* root, NodeRB* node) {
    if (root == node)
        node->color = BLACK;
    else {
        if (node->color == RED)
            node->color = BLACK;
        else
            node->color = RED;
    }
}

static bool rbInsertFix(RB* tree, NodeRB* new, NodeRB* parent) {
    NodeRB* grandpa;
    NodeRB* uncle;
    
    grandpa = parent->parent;

    if (grandpa->left == parent)
        uncle = grandpa->right;
    else
        uncle = grandpa->left;
    
    if (uncle != NULL && uncle->color == RED) {
        uncle->color = parent->color = new->color = BLACK;
        
        return true;
    }
    else {  // Means uncle's color is BLACK
        printf("\n\n%s, ", new->key.name);
        if (new == parent->left) {
            if (parent == grandpa->left) {
                printf("r\n");
                subtree_rotateRight(tree, parent);
                rbChangeColor(tree->root, grandpa);
                rbChangeColor(tree->root, parent);
            }
            else {
                printf("dl\n");
                subtree_doubleRotateLeft(tree, grandpa);
                printf("\ns");
                rbChangeColor(tree->root, grandpa);
                rbChangeColor(tree->root, new);
            }
        }
        else {  // new == parent->right
            if (parent == grandpa->right) {
                printf("l\n");
                subtree_rotateLeft(tree, parent);
                rbChangeColor(tree->root, grandpa);
                rbChangeColor(tree->root, parent);
            }
            else {
                printf("dr\n");
                subtree_doubleRotateRight(tree, grandpa);
                rbChangeColor(tree->root, grandpa);
                rbChangeColor(tree->root, new);
            }
        }
        
        return true;
    }
}

void rbHeight(RB* tree) {
    if (rbEmpty(*tree)) {
        tree->height = 0;
    } else {
        tree->height = 0;
        subtree_maxDepth(tree->root, &tree->height);
    }
}

void rbInsert(RB* tree, type key) {
    NodeRB* new = (NodeRB*)malloc(sizeof(NodeRB));
    NodeRB* aux = tree->root;
    bool needRotation = false;

    new->key = key;
    new->left = new->right = NULL;

    if (rbEmpty(*tree)) {
        tree->root = new;
        tree->height = 1;
        new->parent = NULL;
        new->depth = 1;
        new->color = BLACK;
        return;
    }

    new->color = RED;
    
    while (true) {
        if (strcompare(key.name, aux->key.name) < 0) {
            if (aux->left == NULL) {
                aux->left = new;
                new->depth = aux->depth + 1;
                if (aux->color == RED)    // If the parent is BLACK
                    needRotation = rbInsertFix(tree, new, aux);
                break;
            }
            else aux = aux->left;
        }
        else if (strcompare(key.name, aux->key.name) > 0) {
            if (aux->right == NULL) {
                aux->right = new;
                new->depth = aux->depth + 1;
                if (aux->color == RED)
                    needRotation = rbInsertFix(tree, new, aux);
                break;
            }
            else aux = aux->right;
        }
        else {
            return;
        }
    }

    new->parent = aux;
    
    if (needRotation)
        rbHeight(tree);
    else if (tree->height < new->depth)
        tree->height = new->depth;
}

void node_print(NodeRB* node) {
    printf("\n");
    for (int i = 0; i < node->depth; i++)
        printf("=");
    printf("%s", node->key.name);
    if (node->color == RED)
        printf("(RED)");
    else
        printf("(BLACK)");
}

void node_depthAndColor(NodeRB* node) {
    printf("\nDepth: %d", node->depth);
    if (node->color == RED)
        printf("\nColor: RED");
    else
        printf("\nColor: BLACK");
}

void node_depthDec(NodeRB* node) {
    node->depth--;
}

void node_depthInc(NodeRB* node) {
    node->depth++;
}

void node_clear(NodeRB* node) {
    NodeRB* parent = node->parent;

    if (parent != NULL) {
        if (parent->left == node) parent->left = NULL;
        else parent->right = NULL;
    }

    free(node->key.name);
    free(node);
}

NodeRB* rbSearchByKey(RB* tree, type key, NodeFunctionRB function) {
    if (rbEmpty(*tree)) {
        printf("\nEmpty tree");
        return NULL;
    }

    NodeRB* aux = tree->root;

    while (aux != NULL) {
        if (strcompare(key.name, aux->key.name) < 0)
            aux = aux->left;
        else if (strcompare(key.name, aux->key.name) > 0)
            aux = aux->right;
        else {
            NodeRB* node = aux;
            if (function != NULL)
                function(aux);
            return node;
        }
    }

    printf("Key not found");
    return NULL;
}

void rbInOrder(NodeRB* root, NodeFunctionRB function) {
    if (root != NULL && function != NULL) {
        rbInOrder(root->left, function);
        function(root);
        rbInOrder(root->right, function);
    }
}

void rbPreOrder(NodeRB* root, NodeFunctionRB function) {
    if (root != NULL && function != NULL) {
        function(root);
        rbPreOrder(root->left, function);
        rbPreOrder(root->right, function);
    }
}

void rbPostOrder(NodeRB* root, NodeFunctionRB function) {
    if (root != NULL && function != NULL) {
        rbPostOrder(root->left, function);
        rbPostOrder(root->right, function);
        function(root);
    }
}

void rbPrint(RB tree) {
    if (rbEmpty(tree)) printf("\nEmpty tree");
    else rbPreOrder(tree.root, node_print);
}

void subtree_maxDepth(NodeRB* root, int* maxDepth) {
    if (root != NULL) {
        if (root->depth > *maxDepth) *maxDepth = root->depth;
        subtree_maxDepth(root->left, maxDepth);
        subtree_maxDepth(root->right, maxDepth);
    }
}

void rbClear(RB* tree) {
    if (rbEmpty(*tree))
        printf("\nEmpty tree");
    else {
        rbPostOrder(tree->root, node_clear);
        tree->root = NULL;
        tree->height = 0;
        // printf("\nTree cleared");
    }

    rotations[RBT] = 0;
}

void subtree_rotateRight(RB* tree, NodeRB* root) {
    NodeRB* aux = root->left;
    NodeRB* parent = root->parent;

    rotations[RBT]++;

    if (root == tree->root)
        tree->root = aux;

    root->left = aux->right;
    if (root->left != NULL)
        root->left->parent = root;
    aux->parent = parent;
    if (parent != NULL) {
        if (parent->left == root) parent->left = aux;
        else parent->right = aux;
    }
    aux->right = root;
    root->parent = aux;

    root->depth++;
    aux->depth--;
    rbInOrder(root->right, node_depthInc);
    rbInOrder(aux->left, node_depthDec);
}

void subtree_rotateLeft(RB* tree, NodeRB* root) {
    NodeRB* aux = root->right;
    NodeRB* parent = root->parent;

    rotations[RBT]++;

    if (root == tree->root)
        tree->root = aux;

    root->right = aux->left;
    if (root->right != NULL)
        root->right->parent = root;
    aux->parent = parent;
    if (parent != NULL) {
        if (parent->left == root) parent->left = aux;
        else parent->right = aux;
    }
    aux->left = root;
    root->parent = aux;

    root->depth++;
    aux->depth--;
    rbInOrder(root->left, node_depthInc);
    rbInOrder(aux->right, node_depthDec);
}

void subtree_doubleRotateRight(RB* tree, NodeRB* root) {
    NodeRB* auxL = root->left;
    NodeRB* auxR = auxL->right;
    NodeRB* parent = root->parent;

    rotations[RBT] += 2;

    if (root == tree->root)
        tree->root = auxR;

    auxL->right = auxR->left;
    if (auxL->right != NULL)
        auxL->right->parent = auxL;
    root->left = auxR->right;
    if (root->left != NULL)
        root->left->parent = root;
    auxR->parent = root->parent;
    if (parent != NULL) {
        if (parent->left == root) parent->left = auxR;
        else parent->right = auxR;
    }
    root->parent = auxR;
    auxL->parent = auxR;
    auxR->left = auxL;
    auxR->right = root;

    root->depth++;
    auxL->depth -= 2;
    rbInOrder(root->right, node_depthInc);
    rbInOrder(root->left, node_depthDec);
    rbInOrder(auxL->right, node_depthDec);
}

void subtree_doubleRotateLeft(RB* tree, NodeRB* root) {
    NodeRB* auxR = root->right;
    NodeRB* auxL = auxR->left;
    printf("\n j");
    NodeRB* parent = root->parent;

    rotations[RBT] += 2;
    
    if (root == tree->root)
        tree->root = auxL;

    root->right = auxL->left;
    if (root->right != NULL)
        root->right->parent = root;
    auxR->left = auxL->right;
    if (auxR->left != NULL) 
        auxR->left->parent = auxR;
    auxL->parent = root->parent;
    if (parent != NULL) {
        if (parent->left == root) parent->left = auxL;
        else parent->right = auxL;
    }
    root->parent = auxL;
    auxR->parent = auxL;
    auxL->left = root;
    auxL->right = auxR;

    root->depth++;
    auxL->depth -= 2;
    rbInOrder(root->left, node_depthInc);
    rbInOrder(root->right, node_depthDec);
    rbInOrder(auxR->left, node_depthDec);
}

static int RBCountNodesRecur(NodeRB* a) {

    if (a == NULL) return 0;

    return RBCountNodesRecur(a->left) + RBCountNodesRecur(a->right) + 1;
    
}

int rbCountNodes(RB tree) {

    return RBCountNodesRecur(tree.root);
    
}

void rbPrintResults(RB tree) {
    printf("\n\n----- RB -----");
    printf("\nNum nodos: %d", rbCountNodes(tree));
    printf("\nAltura: %d", tree.height);
    printf("\nNum rotacoes: %d", rotations[RBT]);
    printf("\nComparacoes: %d", comp[RBT]);
}

void rbFilePrintResults(RB tree, FILE* file) {
    fprintf(file, "\n\n----- RB -----");
    fprintf(file, "\nNum nodos: %d", rbCountNodes(tree));
    fprintf(file, "\nAltura: %d", tree.height);
    fprintf(file, "\nNum rotacoes: %d", rotations[RBT]);
    fprintf(file, "\nComparacoes: %d", comp[RBT]);
}

NodeRB* rbConsulta(NodeRB *a, char *chave) {
    while (a != NULL){
        comp[RBT]++;
        if (!strcompare(a->key.name, chave)){
            return a;
        } else {
            if (strcompare(a->key.name, chave) > 0)
                a = a->left;
            else
                a = a->right;
        }
    }
    return NULL;
}