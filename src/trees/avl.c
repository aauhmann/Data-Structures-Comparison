#include "avl.h"
#include "strcompare.h"
#include "global.h"
#include <stdlib.h>

bool avlNodeFixFactor(NodeAVL* node);
void avlNodePrint(NodeAVL* node);
void avlNodeDepthAndFactor(NodeAVL* node);
void avlNodeDepthDec(NodeAVL* node);
void avlNodeDepthInc(NodeAVL* node);
void avlNodeClear(NodeAVL* node);
bool avlTreeFactorCheck(NodeAVL* root, int* currentDepth, type* key, bool* needRotation);
void avlSubtreeMaxDepth(NodeAVL* root, int* maxDepth);
void avlSubtreeRotate(AVL* tree, NodeAVL* root);
void avlSubtreeRotateRight(AVL* tree, NodeAVL* root);
void avlSubtreeRotateLeft(AVL* tree, NodeAVL* root);
void avlSubtreeDoubleRotateRight(AVL* tree, NodeAVL* root);
void avlSubtreeDoubleRotateLeft(AVL* tree, NodeAVL* root);
int avlNodeFactor(NodeAVL* node);

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
                    needRotation = avlNodeFixFactor(aux);
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
                    needRotation = avlNodeFixFactor(aux);
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
        avlSubtreeRotate(tree, aux);
        avlHeight(tree);
    }
    else if (tree->height < new->depth)
        tree->height = new->depth;
}

bool avlNodeFixFactor(NodeAVL* node) {
    bool needRotation = false;

    while (node != NULL) {
        node->factor = avlNodeFactor(node);

        if (abs(node->factor) > 1)
            needRotation = true;

        node = node->parent;
    }

    return needRotation;
}

void avlNodePrint(NodeAVL* node) {
    printf("\n");
    for (int i = 0; i < node->depth; i++)
        printf("=");
    printf("%s", node->key.name);
}

void avlNodeDepthAndFactor(NodeAVL* node) {
    printf("\nDepth: %d", node->depth);
    printf("\nFactor: %d", node->factor);
}

void avlNodeDepthDec(NodeAVL* node) {
    node->depth--;
}

void avlNodeDepthInc(NodeAVL* node) {
    node->depth++;
}

void avlNodeClear(NodeAVL* node) {
    NodeAVL* parent = node->parent;

    if (parent != NULL) {
        if (parent->left == node) parent->left = NULL;
        else parent->right = NULL;
    }

    free(node->key.name);
    free(node);
}

bool avlTreeFactorCheck(NodeAVL* root, int* currentDepth, type* key, bool* needRotation) {
    if (abs(root->factor) > 1 && root->depth > *currentDepth) {
        *currentDepth = root->depth;
        *key = root->key;
        *needRotation = true;
    }
    avlTreeFactorCheck(root->left, currentDepth, key, needRotation);
    avlTreeFactorCheck(root->right, currentDepth, key, needRotation);
    
    return *needRotation;
}

NodeAVL* avlSearchByKey(AVL* tree, type key, NodeFunctionAVL function) {
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

void avlInOrder(NodeAVL* root, NodeFunctionAVL function) {
    if (root != NULL && function != NULL) {
        avlInOrder(root->left, function);
        function(root);
        avlInOrder(root->right, function);
    }
}

void avlPreOrder(NodeAVL* root, NodeFunctionAVL function) {
    if (root != NULL && function != NULL) {
        function(root);
        avlPreOrder(root->left, function);
        avlPreOrder(root->right, function);
    }
}

void avlPostOrder(NodeAVL* root, NodeFunctionAVL function) {
    if (root != NULL && function != NULL) {
        avlPostOrder(root->left, function);
        avlPostOrder(root->right, function);
        function(root);
    }
}

void avlPrint(AVL tree) {
    if (avlEmpty(tree)) printf("\nEmpty tree");
    else avlPreOrder(tree.root, avlNodePrint);
}

void avlHeight(AVL* tree) {
    if (avlEmpty(*tree)) {
        tree->height = 0;
    } else {
        tree->height = 0;
        avlSubtreeMaxDepth(tree->root, &tree->height);
    }
}

void avlSubtreeMaxDepth(NodeAVL* root, int* maxDepth) {
    if (root != NULL) {
        if (root->depth > *maxDepth) *maxDepth = root->depth;
        avlSubtreeMaxDepth(root->left, maxDepth);
        avlSubtreeMaxDepth(root->right, maxDepth);
    }
}

void avlClear(AVL* tree) {
    if (avlEmpty(*tree))
        printf("\nEmpty tree");
    else {
        avlPostOrder(tree->root, avlNodeClear);
        tree->root = NULL;
        tree->height = 0;
        // printf("\nTree cleared");
    }

    rotations[AVLT] = 0;
}

void avlSubtreeRotate(AVL* tree, NodeAVL* root) {
    NodeAVL* parent;

    while (root != NULL) {
        parent = root->parent;

        if (root->factor > 1) {
            if (root->left->factor > 0)
                avlSubtreeRotateRight(tree, root);
            else
                avlSubtreeDoubleRotateRight(tree, root);

            break;
        }
        else if (root->factor < -1) {
            if (root->right->factor < 0)
                avlSubtreeRotateLeft(tree, root);
            else
                avlSubtreeDoubleRotateLeft(tree, root);

            break;
        }

        root = parent;
    }
}

void avlSubtreeRotateRight(AVL* tree, NodeAVL* root) {
    NodeAVL* aux = root->left;
    NodeAVL* parent = root->parent;

    rotations[AVLT]++;

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
    avlInOrder(root->right, avlNodeDepthInc);
    avlInOrder(aux->left, avlNodeDepthDec);

    root->factor = 0;
    aux->factor = avlNodeFactor(aux);
}

void avlSubtreeRotateLeft(AVL* tree, NodeAVL* root) {
    NodeAVL* aux = root->right;
    NodeAVL* parent = root->parent;

    rotations[AVLT]++;

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
    avlInOrder(root->left, avlNodeDepthInc);
    avlInOrder(aux->right, avlNodeDepthDec);

    root->factor = 0;
    aux->factor = avlNodeFactor(aux);
}

void avlSubtreeDoubleRotateRight(AVL* tree, NodeAVL* root) {
    NodeAVL* auxL = root->left;
    NodeAVL* auxR = auxL->right;
    NodeAVL* parent = root->parent;

    rotations[AVLT] += 2;

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
    auxR->depth -= 2;
    avlInOrder(root->right, avlNodeDepthInc);
    avlInOrder(root->left, avlNodeDepthDec);
    avlInOrder(auxL->right, avlNodeDepthDec);

    if (auxR->factor == 1) root->factor = -1;
    else root->factor = 0;

    if (auxR->factor == -1) auxL->factor = 1;
    else auxL->factor = 0;

    auxR->factor = avlNodeFactor(auxR);
}

void avlSubtreeDoubleRotateLeft(AVL* tree, NodeAVL* root) {
    NodeAVL* auxR = root->right;
    NodeAVL* auxL = auxR->left;
    NodeAVL* parent = root->parent;

    rotations[AVLT] += 2;

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
    avlInOrder(root->left, avlNodeDepthInc);
    avlInOrder(root->right, avlNodeDepthDec);
    avlInOrder(auxR->left, avlNodeDepthDec);

    if (auxL->factor == -1) root->factor = 1;
    else root->factor = 0;

    if (auxL->factor == 1) auxR->factor = -1;
    else auxR->factor = 0;

    auxL->factor = avlNodeFactor(auxL);
}

int avlNodeFactor(NodeAVL* node) {
    int hl = node->depth, hr = node->depth;

    avlSubtreeMaxDepth(node->left, &hl);

    avlSubtreeMaxDepth(node->right, &hr);

    return (hl - hr);
}

void avlFactor(NodeAVL* root, int* factor) {
    int aux = avlNodeFactor(root);

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
    printf("\n\n----- AVL -----");
    printf("\nNum nodos: %d", avlCountNodes(tree));
    printf("\nAltura: %d", tree.height);
    printf("\nNum rotacoes: %d", rotations[AVLT]);
    printf("\nComparacoes: %d", comp[AVLT]);
}

void avlFilePrintResults(AVL tree, FILE* file) {
    fprintf(file, "\n\n----- AVL -----");
    fprintf(file, "\nNum nodos: %d", avlCountNodes(tree));
    fprintf(file, "\nAltura: %d", tree.height);
    fprintf(file, "\nNum rotacoes: %d", rotations[AVLT]);
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