#include "printResults.h"

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

void bstPrintResults(BST tree) {

    int numNodes = bstCountNodes(tree);
    int height = bstHeight(tree);
    int numRotations = 0;

    printf("\n----- ABP -----");
    printf("\nNum nodos: %d", numNodes);
    printf("\nAltura: %d", height);
    printf("\nNum rotacoes: %d", numRotations);
    printf("\nComparacoes: %d", tree.totalComparisons);
}

void bstFilePrintResults(BST tree, FILE* file) {

    int numNodes = bstCountNodes(tree);
    int height = bstHeight(tree);
    int numRotations = 0;

    fprintf(file, "----- ABP -----");
    fprintf(file, "\nNum nodos: %d", numNodes);
    fprintf(file, "\nAltura: %d", height);
    fprintf(file, "\nNum rotacoes: %d", numRotations);
    fprintf(file, "\nComparacoes: %d", tree.totalComparisons);
}

void rbtPrintResults(RBT tree) {
    printf("\n\n----- RBT -----");
    printf("\nNum nodos: %d", rbtCountNodes(tree));
    printf("\nAltura: %d", rbtHeight(tree));
    printf("\nNum rotacoes: %d", tree.numRotations);
    printf("\nComparacoes: %d", tree.totalComparisons);
}

void rbtFilePrintResults(RBT tree, FILE* file) {
    fprintf(file, "\n\n----- RUBRO-NEGRA -----");
    fprintf(file, "\nNum nodos: %d", rbtCountNodes(tree));
    fprintf(file, "\nAltura: %d", rbtHeight(tree));
    fprintf(file, "\nNum rotacoes: %d", tree.numRotations);
    fprintf(file, "\nComparacoes: %d", tree.totalComparisons);
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

void printResults(float* estimatedTime, float* generationTime, float* searchTime, BST bst, AVL avl, RBT rbt, SPT spt) {
    bstPrintResults(bst);
    printf("\nTempo estimado: %.2f h", estimatedTime[BSTT]);
    printf("\nTempo de criacao: %.2f ms", generationTime[BSTT]);
    printf("\nTempo de busca: %.2f ms", searchTime[BSTT]);
    
    avlPrintResults(avl);
    printf("\nTempo estimado: %.2f h", estimatedTime[AVLT]);
    printf("\nTempo de criacao: %.2f ms", generationTime[AVLT]);
    printf("\nTempo de busca: %.2f ms", searchTime[AVLT]);

    rbtPrintResults(rbt);
    printf("\nTempo estimado: %.2f h", estimatedTime[RBTT]);
    printf("\nTempo de criacao: %.2f ms", generationTime[RBTT]);
    printf("\nTempo de busca: %.2f ms", searchTime[RBTT]);
    
    sptPrintResults(spt);
    printf("\nTempo estimado: %.2f h", estimatedTime[SPLAYT]);
    printf("\nTempo de criacao: %.2f ms", generationTime[SPLAYT]);
    printf("\nTempo de busca: %.2f ms", searchTime[SPLAYT]);
}

void fprintResults(FILE* file, float* estimatedTime, float* generationTime, float* searchTime, BST bst, AVL avl, RBT rbt, SPT spt) {
    bstFilePrintResults(bst, file);
    fprintf(file, "\nTempo estimado: %.2f h", estimatedTime[BSTT]);
    fprintf(file, "\nTempo de criacao: %.2f ms", generationTime[BSTT]);
    fprintf(file, "\nTempo de busca: %.2f ms", searchTime[BSTT]);
    
    avlFilePrintResults(avl, file);
    fprintf(file, "\nTempo estimado: %.2f h", estimatedTime[AVLT]);
    fprintf(file, "\nTempo de criacao: %.2f ms", generationTime[AVLT]);
    fprintf(file, "\nTempo de busca: %.2f ms", searchTime[AVLT]);
    
    rbtFilePrintResults(rbt, file);
    fprintf(file, "\nTempo estimado: %.2f h", estimatedTime[RBTT]);
    fprintf(file, "\nTempo de criacao: %.2f ms", generationTime[RBTT]);
    fprintf(file, "\nTempo de busca: %.2f ms", searchTime[RBTT]);
    
    sptFilePrintResults(spt, file);
    fprintf(file, "\nTempo estimado: %.2f h", estimatedTime[SPLAYT]);
    fprintf(file, "\nTempo de criacao: %.2f ms", generationTime[SPLAYT]);
    fprintf(file, "\nTempo de busca: %.2f ms", searchTime[SPLAYT]);
}