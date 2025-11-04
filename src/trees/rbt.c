#include "rbt.h"
#include "global.h"
#include "strcompare.h"
#include <stdlib.h>

RBT rbtNew() {
    return (RBT){NULL};
}

static void rbSubtreeRotateRight(RBT* tree, NodeRBT* root) {
    NodeRBT* aux = root->left;
    NodeRBT* parent = root->parent;

    rotations[RBTT]++;

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

}

static void rbSubtreeRotateLeft(RBT* tree, NodeRBT* root) {
    NodeRBT* aux = root->right;
    NodeRBT* parent = root->parent;

    rotations[RBTT]++;

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

}

static void rbSubtreeDoubleRotateRight(RBT* tree, NodeRBT* root) {
    NodeRBT* auxL = root->left;
    NodeRBT* auxR = auxL->right;
    NodeRBT* parent = root->parent;

    rotations[RBTT] += 2;

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

}

static void rbSubtreeDoubleRotateLeft(RBT* tree, NodeRBT* root) {
    NodeRBT* auxR = root->right;
    NodeRBT* auxL = auxR->left;
    NodeRBT* parent = root->parent;

    rotations[RBTT] += 2;
    
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

}

static void rbtPrintRecur(NodeRBT* a, int cont) {

    if (a == NULL) return;

    for (int i = 0; i <= cont; i++) printf("=");
    
    printf("%s(%s)\n", a->key.name, (a->color == RED) ? "RED" : "BLACK");

    if (a->left != NULL) rbtPrintRecur(a->left, cont + 1);  

    if (a->right != NULL) rbtPrintRecur(a->right, cont + 1);
}

void rbtPrint(RBT tree) {

    rbtPrintRecur(tree.root, 0);
}

static bool rbtClearRecur(NodeRBT* node) {

    if (node == NULL) return true;

    int a = rbtClearRecur(node->right);
    a += rbtClearRecur(node->left);

    free(node->key.name);
    free(node);

    return a;

}

bool rbtClear(RBT* tree) {
    
    if (tree == NULL) return false;

    NodeRBT* removed = tree->root;
    tree->root = NULL;

    return rbtClearRecur(removed);

}

static int rbCountNodesRecur(NodeRBT* a) {
    if (a == NULL) return 0;

    return rbCountNodesRecur(a->left) + rbCountNodesRecur(a->right) + 1;
}

int rbtCountNodes(RBT tree) {
    return rbCountNodesRecur(tree.root);
}

void rbtPrintResults(RBT tree) {
    printf("\n\n----- RBT -----");
    printf("\nNum nodos: %d", rbtCountNodes(tree));
    printf("\nAltura: %d", rbtHeight(tree));
    printf("\nNum rotacoes: %d", rotations[RBTT]);
    printf("\nComparacoes: %d", comp[RBTT]);
}

void rbtFilePrintResults(RBT tree, FILE* file) {
    fprintf(file, "\n\n----- RBT -----");
    fprintf(file, "\nNum nodos: %d", rbtCountNodes(tree));
    fprintf(file, "\nAltura: %d", rbtHeight(tree));
    fprintf(file, "\nNum rotacoes: %d", rotations[RBTT]);
    fprintf(file, "\nComparacoes: %d", comp[RBTT]);
}

NodeRBT* rbtSearch(NodeRBT* root, char* key) {
    while (root != NULL){
        comp[RBTT]++;
        if (!strcompare(root->key.name, key)){
            return root;
        } else {
            if (strcompare(root->key.name, key) > 0)
                root = root->left;
            else
                root = root->right;
        }
    }
    return NULL;
}

static void rbChangeColor(NodeRBT* root, NodeRBT* node) {
    if (root == node)
        node->color = BLACK;
    else {
        if (node->color == RED)
            node->color = BLACK;
        else
            node->color = RED;
    }
}

// Returns the new subtree root
static NodeRBT* rbtHandleRotations(RBT* tree, NodeRBT* node, NodeRBT* father, NodeRBT* grandpa) {
    
    if (strcompare(grandpa->key.name, father->key.name) > 0) {
        
        if (strcompare(father->key.name, node->key.name) > 0) {
            rbSubtreeRotateRight(tree, grandpa);
            rbChangeColor(tree->root, father);
            rbChangeColor(tree->root, grandpa);
            return father;
        }
        else if (strcompare(father->key.name, node->key.name) < 0) { 
            rbSubtreeDoubleRotateRight(tree, grandpa);
            rbChangeColor(tree->root, node);
            rbChangeColor(tree->root, grandpa);
            return node;
        }
          
    }
    else if (strcompare(grandpa->key.name, father->key.name) < 0) {
        
        if (strcompare(father->key.name, node->key.name) < 0) {
            rbSubtreeRotateLeft(tree, grandpa);
            rbChangeColor(tree->root, father);
            rbChangeColor(tree->root, grandpa);
            return father;
        }
        else if (strcompare(father->key.name, node->key.name) > 0) {
            rbSubtreeDoubleRotateLeft(tree, grandpa);
            rbChangeColor(tree->root, node);
            rbChangeColor(tree->root, grandpa);
            return node;
        }
        
    }

    return NULL;
}

static bool rbtBalance(RBT* tree, NodeRBT* insertedNode) {

    NodeRBT* aux = insertedNode;

    while (aux->parent != NULL) {

        if (aux->color == BLACK || aux->parent->color == BLACK) aux = aux->parent;
        else if (aux->parent->color == RED) {

            NodeRBT* father = aux->parent;
            NodeRBT* grandpa = father->parent;
            NodeRBT* uncle;

            if (strcompare(grandpa->key.name, father->key.name) < 0) uncle = grandpa->left;
            else uncle = grandpa->right;

            if (uncle == NULL || uncle->color == BLACK) {

                aux = rbtHandleRotations(tree, aux, father, grandpa);

            }
            else if (uncle->color == RED) {
                father->color = BLACK;
                uncle->color = BLACK;
                if (tree->root != grandpa) grandpa->color = RED;

                aux = grandpa;
            }
            else {
                printf("\nNodo com cor corrompida!\n");
                return false;
            }

        }
        else {
            printf("\nNodo com cor corrompida!\n");
            return false;
        }

    }

    tree->root = aux;
    return true;

}

bool rbtInsert(RBT* tree, Game game) {

    NodeRBT* newNode = (NodeRBT*)malloc(sizeof(NodeRBT));
    //bool needRotation = false;
    
    newNode->key = game;
    newNode->left = NULL; 
    newNode->right = NULL;
    newNode->parent = NULL;
    
    if (tree->root == NULL) {   // If the tree is empty
        
        tree->root = newNode;
        tree->root->color = BLACK;
        
        return true;
    }

    newNode->color = RED;      // Inserting red node
    NodeRBT* aux = tree->root;
    NodeRBT* father;
    Direction lastDirection;
    while (aux != NULL) {   // Searching the newNode's spot

        if (strcompare(aux->key.name, game.name) < 0) {
            father = aux;
            aux = aux->right;
            lastDirection = RIGHT;
        }
        else if(strcompare(aux->key.name, game.name) > 0) {
            father = aux;
            aux = aux->left;
            lastDirection = LEFT;
        }
        else {
            printf("\nErro, nodo já existente!\n");
            return false;
        }
    }

    if (lastDirection == LEFT) father->left = newNode;
    if (lastDirection == RIGHT) father->right = newNode;
    newNode->parent = father;

    return rbtBalance(tree, newNode);

}

static int rbtHeightRecur(NodeRBT *a){

    if (a == NULL) return 0;

    int leftHeight = rbtHeightRecur(a->left);
    int rightHeight = rbtHeightRecur(a->right);

    if (leftHeight > rightHeight) return (1 + leftHeight);
    else return (1 + rightHeight);

}

int rbtHeight(RBT tree) {
    return rbtHeightRecur(tree.root);
}