#include "avl.h"
#include "strcompare.h"
#include "global.h"
#include <stdlib.h>

int rotations = 0;

bool node_fixFactor(NodeAVL* node);
void node_print(NodeAVL* node);
void node_depthAndFactor(NodeAVL* node);
void node_depthDec(NodeAVL* node);
void node_depthInc(NodeAVL* node);
void node_clear(NodeAVL* node);
bool tree_factorCheck(NodeAVL* root, int* currentDepth, type* key, bool* needRotation);
void subtree_maxDepth(NodeAVL* root, int* maxDepth);
void subtree_rotate(AVL* tree, NodeAVL* root);
void subtree_rotateRight(AVL* tree, NodeAVL* root);
void subtree_rotateLeft(AVL* tree, NodeAVL* root);
void subtree_doubleRotateRight(AVL* tree, NodeAVL* root);
void subtree_doubleRotateLeft(AVL* tree, NodeAVL* root);
int node_factor(NodeAVL* node);

AVL avlInitialize() {
    return (AVL){NULL, 0};
}

bool avlEmpty(AVL tree) {
    if (tree.root == NULL) return true;
    else return false;
}

void avlInsert(AVL* tree, type key) {
    NodeAVL* new = (NodeAVL*)malloc(sizeof(NodeAVL));
    NodeAVL* aux = tree->root;
    bool needRotation = false;

    new->key = key;
    new->left = new->right = NULL;
    new->factor = 0;

    if (avlEmpty(*tree)) {
        tree->root = new;
        tree->height = 1;
        new->parent = NULL;
        new->depth = 1;
        return;
    }
    
    while (true) {
        if (strcompare(key.name, aux->key.name) < 0) {
            if (aux->left == NULL) {
                aux->left = new;
                new->depth = aux->depth + 1;
                if (aux->right == NULL)
                    needRotation = node_fixFactor(aux);
                else aux->factor++;
                break;
            }
            else aux = aux->left;
        }
        else if (strcompare(key.name, aux->key.name) > 0) {
            if (aux->right == NULL) {
                aux->right = new;
                new->depth = aux->depth + 1;
                if (aux->left == NULL)
                    needRotation = node_fixFactor(aux);
                else aux->factor--;
                break;
            }
            else aux = aux->right;
        }
        else {
            return;
        }
    }

    new->parent = aux;
    
    if (needRotation) {
        subtree_rotate(tree, aux);
        avlHeight(tree);
    }
    else if (tree->height < new->depth)
        tree->height = new->depth;
}

bool node_fixFactor(NodeAVL* node) {
    bool needRotation = false;

    while (node != NULL) {
        node->factor = node_factor(node);

        if (abs(node->factor) > 1)
            needRotation = true;

        node = node->parent;
    }

    return needRotation;
}

void node_print(NodeAVL* node) {
    printf("\n");
    for (int i = 0; i < node->depth; i++)
        printf("=");
    printf("%s", node->key.name);
}

void node_depthAndFactor(NodeAVL* node) {
    printf("\nDepth: %d", node->depth);
    printf("\nFactor: %d", node->factor);
}

void node_depthDec(NodeAVL* node) {
    node->depth--;
}

void node_depthInc(NodeAVL* node) {
    node->depth++;
}

void node_clear(NodeAVL* node) {
    NodeAVL* parent = node->parent;

    if (parent != NULL) {
        if (parent->left == node) parent->left = NULL;
        else parent->right = NULL;
    }

    free(node->key.name);
    free(node);
}

bool tree_factorCheck(NodeAVL* root, int* currentDepth, type* key, bool* needRotation) {
    if (abs(root->factor) > 1 && root->depth > *currentDepth) {
        *currentDepth = root->depth;
        *key = root->key;
        *needRotation = true;
    }
    tree_factorCheck(root->left, currentDepth, key, needRotation);
    tree_factorCheck(root->right, currentDepth, key, needRotation);
    
    return *needRotation;
}

NodeAVL* avlSearchByKey(AVL* tree, type key, NodeFunction function) {
    if (avlEmpty(*tree)) {
        printf("\nEmpty tree");
        return NULL;
    }

    NodeAVL* aux = tree->root;

    while (aux != NULL) {
        if (strcompare(key.name, aux->key.name) < 0)
            aux = aux->left;
        else if (strcompare(key.name, aux->key.name) > 0)
            aux = aux->right;
        else {
            NodeAVL* node = aux;
            if (function != NULL)
                function(aux);
            return node;
        }
    }

    printf("Key not found");
    return NULL;
}

void avlInOrder(NodeAVL* root, NodeFunction function) {
    if (root != NULL && function != NULL) {
        avlInOrder(root->left, function);
        function(root);
        avlInOrder(root->right, function);
    }
}

void avlPreOrder(NodeAVL* root, NodeFunction function) {
    if (root != NULL && function != NULL) {
        function(root);
        avlPreOrder(root->left, function);
        avlPreOrder(root->right, function);
    }
}

void avlPostOrder(NodeAVL* root, NodeFunction function) {
    if (root != NULL && function != NULL) {
        avlPostOrder(root->left, function);
        avlPostOrder(root->right, function);
        function(root);
    }
}

void avlPrint(AVL tree) {
    if (avlEmpty(tree)) printf("\nEmpty tree");
    else avlPreOrder(tree.root, node_print);
}

void avlHeight(AVL* tree) {
    if (avlEmpty(*tree)) {
        tree->height = 0;
    } else {
        tree->height = 0;
        subtree_maxDepth(tree->root, &tree->height);
    }
}

void subtree_maxDepth(NodeAVL* root, int* maxDepth) {
    if (root != NULL) {
        if (root->depth > *maxDepth) *maxDepth = root->depth;
        subtree_maxDepth(root->left, maxDepth);
        subtree_maxDepth(root->right, maxDepth);
    }
}

void avlClear(AVL* tree) {
    if (avlEmpty(*tree))
        printf("\nEmpty tree");
    else {
        avlPostOrder(tree->root, node_clear);
        tree->root = NULL;
        tree->height = 0;
        // printf("\nTree cleared");
    }

    rotations = 0;
}

void subtree_rotate(AVL* tree, NodeAVL* root) {
    NodeAVL* parent;

    while (root != NULL) {
        parent = root->parent;

        if (root->factor > 1) {
            if (root->left->factor > 0)
                subtree_rotateRight(tree, root);
            else
                subtree_doubleRotateRight(tree, root);

            break;
        }
        else if (root->factor < -1) {
            if (root->right->factor < 0)
                subtree_rotateLeft(tree, root);
            else
                subtree_doubleRotateLeft(tree, root);

            break;
        }

        root = parent;
    }
}

void subtree_rotateRight(AVL* tree, NodeAVL* root) {
    NodeAVL* aux = root->left;
    NodeAVL* parent = root->parent;

    rotations++;

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
    avlInOrder(root->right, node_depthInc);
    avlInOrder(aux->left, node_depthDec);

    root->factor = 0;
    aux->factor = node_factor(aux);
}

void subtree_rotateLeft(AVL* tree, NodeAVL* root) {
    NodeAVL* aux = root->right;
    NodeAVL* parent = root->parent;

    rotations++;

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
    avlInOrder(root->left, node_depthInc);
    avlInOrder(aux->right, node_depthDec);

    root->factor = 0;
    aux->factor = node_factor(aux);
}

void subtree_doubleRotateRight(AVL* tree, NodeAVL* root) {
    NodeAVL* auxL = root->left;
    NodeAVL* auxR = auxL->right;
    NodeAVL* parent = root->parent;

    rotations += 2;

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
    avlInOrder(root->right, node_depthInc);
    avlInOrder(root->left, node_depthDec);
    avlInOrder(auxL->right, node_depthDec);

    if (auxR->factor == 1) root->factor = -1;
    else root->factor = 0;

    if (auxR->factor == -1) auxL->factor = 1;
    else auxL->factor = 0;

    auxR->factor = node_factor(auxR);
}

void subtree_doubleRotateLeft(AVL* tree, NodeAVL* root) {
    NodeAVL* auxR = root->right;
    NodeAVL* auxL = auxR->left;
    NodeAVL* parent = root->parent;

    rotations += 2;

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
    avlInOrder(root->left, node_depthInc);
    avlInOrder(root->right, node_depthDec);
    avlInOrder(auxR->left, node_depthDec);

    if (auxL->factor == -1) root->factor = 1;
    else root->factor = 0;

    if (auxL->factor == 1) auxR->factor = -1;
    else auxR->factor = 0;

    auxL->factor = node_factor(auxL);
}

int node_factor(NodeAVL* node) {
    int hl = node->depth, hr = node->depth;

    subtree_maxDepth(node->left, &hl);

    subtree_maxDepth(node->right, &hr);

    return (hl - hr);
}

void avlFactor(NodeAVL* root, int* factor) {
    int aux = node_factor(root);

    if (aux > *factor) *factor = aux;

    if (root->left != NULL) avlFactor(root->left, factor);
    if (root->right != NULL) avlFactor(root->right, factor);
}

static int avlCountNodesRecur(NodeAVL* a) {

    if (a == NULL) return 0;

    return avlCountNodesRecur(a->left) + avlCountNodesRecur(a->right) + 1;
    
}

int avlCountNodes(AVL tree) {

    return avlCountNodesRecur(tree.root);
    
}

void avlPrintResults(AVL tree) {
    int numNodes = avlCountNodes(tree);
    int height = tree.height;
    int numRotations = rotations;

    printf("\n\n----- AVL -----");
    printf("\nNum nodos: %d", numNodes);
    printf("\nAltura: %d", height);
    printf("\nNum rotacoes: %d", numRotations);
    printf("\nComparacoes: %d", comp[AVLT]);
}

void avlFilePrintResults(AVL tree, FILE* file) {
    int numNodes = avlCountNodes(tree);
    int height = tree.height;
    int numRotations = rotations;

    fprintf(file, "\n\n----- AVL -----");
    fprintf(file, "\nNum nodos: %d", numNodes);
    fprintf(file, "\nAltura: %d", height);
    fprintf(file, "\nNum rotacoes: %d", numRotations);
    fprintf(file, "\nComparacoes: %d", comp[AVLT]);
}

NodeAVL* avlConsulta(NodeAVL *a, char *chave) {
    while (a != NULL){
        comp[AVLT]++;
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