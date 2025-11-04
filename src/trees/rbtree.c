#include "rbtree.h"
#include "global.h"
#include "strcompare.h"
#include <stdlib.h>

RB rbInitialize() {
    return (RB){NULL, 0};
}

bool rbEmpty(RB tree) {
    if (tree.root == NULL) return true;
    else return false;
}

static void rbSubtreeRotateRight(RB* tree, NodeRB* root) {
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

    //root->depth++;
    //aux->depth--;
    //rbInOrder(root->right, rbNodeDepthInc);
    //rbInOrder(aux->left, rbNodeDepthDec);
}

static void rbSubtreeRotateLeft(RB* tree, NodeRB* root) {
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

    // root->depth++;
    // aux->depth--;
    // rbInOrder(root->left, rbNodeDepthInc);
    // rbInOrder(aux->right, rbNodeDepthDec);
}

static void rbSubtreeDoubleRotateRight(RB* tree, NodeRB* root) {
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

    // root->depth++;
    // auxR->depth -= 2;
    // rbInOrder(root->right, rbNodeDepthInc);
    // rbInOrder(root->left, rbNodeDepthDec);
    // rbInOrder(auxL->right, rbNodeDepthDec);
}

static void rbSubtreeDoubleRotateLeft(RB* tree, NodeRB* root) {
    NodeRB* auxR = root->right;
    // printf("var, ");
    // if (auxR == NULL) printf("auxR == NULL, ");
    NodeRB* auxL = auxR->left;
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
    // printf("nodes done, ");

    // root->depth++;
    // auxL->depth -= 2;
    // rbInOrder(root->left, rbNodeDepthInc);
    // rbInOrder(root->right, rbNodeDepthDec);
    // rbInOrder(auxR->left, rbNodeDepthDec);
    // printf("depth done, ");
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

static void rbSubtreeMaxDepth(NodeRB* root, int* maxDepth) {
    if (root != NULL) {
        if (root->depth > *maxDepth) *maxDepth = root->depth;
        rbSubtreeMaxDepth(root->left, maxDepth);
        rbSubtreeMaxDepth(root->right, maxDepth);
    }
}

static void rbHeight(RB* tree) {
    if (rbEmpty(*tree)) {
        tree->height = 0;
    } else {
        tree->height = 0;
        rbSubtreeMaxDepth(tree->root, &tree->height);
    }
}

static bool rbInsertFix(RB* tree, NodeRB* new, NodeRB* parent) {
    NodeRB* grandpa = parent->parent;
    NodeRB* uncle;
    bool needHeightRecount = false;
    
    while (grandpa != NULL && parent->color == RED) {   // Esse parent->color == RED o copilot q disse q era pra por
        // printf("\nw, ");
        // if (new == NULL) printf("new == NULL, ");
        // if (parent == NULL) printf("parent == NULL, ");
        // if (grandpa == NULL) printf("grandpa == NULL, ");
        // if (grandpa->left == NULL) printf("grandpa->left == NULL, ");
        // if (grandpa->right == NULL) printf("grandpa->right == NULL, ");

        if (grandpa->left == parent)
            uncle = grandpa->right;
        else
            uncle = grandpa->left;
        
        if (uncle != NULL && uncle->color == RED) {
            // printf("red, ");
            uncle->color = parent->color = BLACK;
            //grandpa->color = RED;    // Se ativa essa linha a arvore fica com a altura completamente cagada (da muita merda) AAAAAAAAA
            needHeightRecount = true;

            new = parent;
            parent = grandpa;
            grandpa = grandpa->parent;
            // printf("g = g->p, ");
            // if (grandpa == NULL) printf("grandpa == NULL, ");
        }
        else {  // Means uncle's color is BLACK
            // printf("black, ");

            if (new == parent->left) {
                if (parent == grandpa->left) {
                    // printf("\tr, ");
                    rbSubtreeRotateRight(tree, grandpa);
                    rbChangeColor(tree->root, parent);
                    rbChangeColor(tree->root, grandpa);
                }
                else {
                    // printf("\tdl, ");
                    rbSubtreeDoubleRotateLeft(tree, grandpa);
                    rbChangeColor(tree->root, new);
                    rbChangeColor(tree->root, grandpa);
                }
            }
            else {  // new == parent->right
                if (parent == grandpa->right) {
                    // printf("\tl, ");
                    rbSubtreeRotateLeft(tree, grandpa);
                    rbChangeColor(tree->root, parent);
                    rbChangeColor(tree->root, grandpa);
                }
                else {
                    // printf("\tdr, ");
                    rbSubtreeDoubleRotateRight(tree, grandpa);
                    rbChangeColor(tree->root, new);
                    rbChangeColor(tree->root, grandpa);
                }
            }
            // printf("loop out");
            tree->root->color = BLACK;
            return true;
        }
    }
    // printf("loop out");
    tree->root->color = BLACK;
    return needHeightRecount;
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
                new->parent = aux;
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
                new->parent = aux;
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
    
    if (needRotation)
        rbHeight(tree);
    else if (tree->height < new->depth)
        tree->height = new->depth;
}

// static void rbNodePrint(NodeRB* node) {
//     printf("\n");
//     for (int i = 0; i < node->depth; i++)
//         printf("=");
//     printf("%s", node->key.name);
//     if (node->color == RED)
//         printf("(RED)");
//     else
//         printf("(BLACK)");
// }

static void rbtPrintRecur(NodeRB* a, int cont) {

    if (a == NULL) return;

    for (int i = 0; i <= cont; i++) printf("=");
    
    printf("%s(%d)\n", a->key.name, a->color);

    if (a->left != NULL) rbtPrintRecur(a->left, cont + 1);  

    if (a->right != NULL) rbtPrintRecur(a->right, cont + 1);
}

void rbtPrint(RB tree) {

    rbtPrintRecur(tree.root, 0);
}

static void rbNodeClear(NodeRB* node) {
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

// void rbPrint(RB tree) {
//     if (rbEmpty(tree)) printf("\nEmpty tree");
//     else rbPreOrder(tree.root, rbNodePrint);
// }

void rbClear(RB* tree) {
    if (rbEmpty(*tree))
        printf("\nEmpty tree");
    else {
        rbPostOrder(tree->root, rbNodeClear);
        tree->root = NULL;
        tree->height = 0;
        // printf("\nTree cleared");
    }

    rotations[RBT] = 0;
}

static int rbCountNodesRecur(NodeRB* a) {
    if (a == NULL) return 0;

    return rbCountNodesRecur(a->left) + rbCountNodesRecur(a->right) + 1;
}

int rbCountNodes(RB tree) {
    return rbCountNodesRecur(tree.root);
}

static int rbHeightRecursion(NodeRB* root) {
    if (root == NULL)
        return 0;  // Altura de árvore vazia é 0
    
    int leftHeight = rbHeightRecursion(root->left);
    int rightHeight = rbHeightRecursion(root->right);
    
    // Retorna a maior altura entre esquerda e direita + 1 (raiz)
    if (leftHeight > rightHeight)
        return leftHeight + 1;
    else
        return rightHeight + 1;
}

void rbPrintResults(RB tree) {
    printf("\n\n----- RB -----");
    printf("\nNum nodos: %d", rbCountNodes(tree));
    printf("\nAltura: %d", rbHeightRecursion(tree.root));
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

NodeRB* rbConsulta(NodeRB* root, char* key) {
    while (root != NULL){
        comp[RBT]++;
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

// Returns the new subtree root
static NodeRB* rbtHandleRotations(RB* tree, NodeRB* node, NodeRB* father, NodeRB* grandpa) {
    
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

static int rbtBalance(RB* tree, NodeRB* insertedNode) {

    NodeRB* aux = insertedNode;

    while (aux->parent != NULL) {

        if (aux->color == BLACK || aux->parent->color == BLACK) aux = aux->parent;
        else if (aux->parent->color == RED) {

            NodeRB* father = aux->parent;
            NodeRB* grandpa = father->parent;
            NodeRB* uncle;

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
                return 0;
            }

        }
        else {
            printf("\nNodo com cor corrompida!\n");
            return 0;
        }

    }

    tree->root = aux;
    return 1;

}

int leandroInsert2(RB* tree, Game game) {

    NodeRB* newNode = (NodeRB*)malloc(sizeof(NodeRB));
    //bool needRotation = false;
    
    newNode->key = game;
    newNode->left = NULL; 
    newNode->right = NULL;
    newNode->parent = NULL;
    
    if (tree->root == NULL) {   // If the tree is empty
        
        tree->root = newNode;
        tree->root->color = BLACK;
        
        return 1;
    }

    newNode->color = RED;      // Inserting red node
    NodeRB* aux = tree->root;
    NodeRB* father;
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
            return 0;
        }
    }

    if (lastDirection == LEFT) father->left = newNode;
    if (lastDirection == RIGHT) father->right = newNode;
    newNode->parent = father;

    return rbtBalance(tree, newNode);

}