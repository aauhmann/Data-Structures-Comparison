#include "strcompare.h"
#include "bst.h"
#include "avl.h"
#include "global.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define DELIMITERS ",\n"
#define STEAM_GAMES argv[1]
#define DESIRED_GAMES argv[2]
#define OUTPUT_FILE argv[3]

void printResults(float* estimatedTime, float* creationTime, float* searchTime, BST bst, AVL avl);
void fprintResults(FILE* file, float* estimatedTime, float* creationTime, float* searchTime, BST bst, AVL avl);

/// a files\teste.csv files\lista_teste.txt files\saida.txt
/// a files\dataset.csv files\lista_jogador1.txt files\saida.txt

int comp[4] = {0, 0, 0, 0};

int main(int argc, char* argv[]) {
    
    if (argc != 4) {
        printf("Numero de parametros incorreto. Entrada do programa deve ser:");
        printf(" exemplo <arq_steamgames> <arq_listajogos> <arq_saida>");
        return 1;
    }
    
    BST bst = bstNew();
    AVL avl = avlInitialize();
    char line[1000];
    clock_t begin, end;
    float generationTime[4];
    float searchTime[4];
    float estimatedTime[4] = {0, 0, 0, 0};
    Game game = {NULL, 0};

    FILE* steamGamesFile = fopen(STEAM_GAMES, "r");
    FILE* desiredGamesFile = fopen(DESIRED_GAMES, "r");
    FILE* outputFile = fopen(OUTPUT_FILE, "w");

    if (steamGamesFile == NULL) {       // Checks file opening
        printf("Erro ao abrir o arquivo %s", STEAM_GAMES);
        return 1;
    }

    if (desiredGamesFile == NULL) {       // Checks file opening
        printf("Erro ao abrir o arquivo %s", DESIRED_GAMES);
        return 1;
    }

    if (outputFile == NULL) {
        printf("Erro ao abrir o arquivo %s", OUTPUT_FILE);
        return 1;
    }


    // BST TREE
    begin = clock();       // Starts the BST generation timer

    // Dataset loop
    while (fgets(line, sizeof(line), steamGamesFile)) {
        char *word = strtok(line, DELIMITERS);      // Gets the game's name(first word)

        game.name = (char*)malloc(strlen(word) + 1);
        
        strncpy(game.name, word, strlen(word) + 1);
        
        game.time = atof(strtok(NULL, DELIMITERS));     // Gets the average gameplay time(second word)

        bstInsert(&bst, game);      // Inserts into the tree       
    }

    end = clock();      // Stops the BST generation timer
    generationTime[BSTT] = (float)(end - begin);
    
    // Search loop
    begin = clock();     // Starts the BST search timer

    while (fgets(line, sizeof(line), desiredGamesFile)) {
        // char* word = strtok(line, DELIMITERS);
        // printf("\n%s", word);
        //NodeBST* desiredGame = bstConsulta(bst.root, strtok(line, DELIMITERS));    // Gets desired game's name
        NodeBST* desiredGame = bstConsulta(bst.root, strtok(line, DELIMITERS));
        if (desiredGame != NULL) { 
            //printf("\t esta no arquivo");
            //estimatedTime += desiredGame->game.time;
            estimatedTime[BSTT] += desiredGame->game.time;
        }
        //else printf("NAO ACHOU");
    }

    end = clock();      // Stops the BST search timer
    searchTime[BSTT] = (float)(end - begin);


    // AVL TREE
    rewind(steamGamesFile);
    rewind(desiredGamesFile);

    // Dataset loop
    begin = clock();       // Starts the AVL tree generation timer

    while (fgets(line, sizeof(line), steamGamesFile)) {
        char *word = strtok(line, DELIMITERS);      // Gets the game's name(first word)

        game.name = (char*)malloc(strlen(word) + 1);
        
        strncpy(game.name, word, strlen(word) + 1);
        
        game.time = atof(strtok(NULL, DELIMITERS));     // Gets the average gameplay time(second word)

        avlInsert(&avl, game);      // Inserts into the tree      
    }

    end = clock();      // Stops the AVL tree generation timer
    generationTime[AVLT] = (float)(end - begin);

    // Search loop
    begin = clock();

    while (fgets(line, sizeof(line), desiredGamesFile)) {
        // char* word = strtok(line, DELIMITERS);
        // printf("\n%s", word);
        //NodeBST* desiredGame = bstConsulta(bst.root, strtok(line, DELIMITERS));    // Gets desired game's name
        NodeAVL* desiredGame = avlConsulta(avl.root, strtok(line, DELIMITERS));
        if (desiredGame != NULL) { 
            //printf("\t esta no arquivo");
            //estimatedTime += desiredGame->game.time;
            estimatedTime[AVLT] += desiredGame->key.time;
        }
        //else printf("NAO ACHOU");
    }

    end = clock();
    searchTime[AVLT] = (float)(end - begin);      // Stops the AVL search timer


    printResults(estimatedTime, generationTime, searchTime, bst, avl);
    fprintResults(outputFile, estimatedTime, generationTime, searchTime, bst, avl);
    
    bstClear(&bst);
    avlClear(&avl);
    
    fclose(steamGamesFile);
    fclose(desiredGamesFile);
    fclose(outputFile);

    printf("\n\n\nEnd of program...\n");
    
    return 0;
}

void printResults(float* estimatedTime, float* generationTime, float* searchTime, BST bst, AVL avl) {
    bstPrintResults(bst);
    printf("\nTempo estimado: %.2f h", estimatedTime[BSTT]);
    printf("\nTempo de criacao: %.2f ms", generationTime[BSTT]);
    printf("\nTempo de busca: %.2f ms", searchTime[BSTT]);
    
    avlPrintResults(avl);
    printf("\nTempo estimado: %.2f h", estimatedTime[AVLT]);
    printf("\nTempo de criacao: %.2f ms", generationTime[AVLT]);
    printf("\nTempo de busca: %.2f ms", searchTime[AVLT]);
    
    // printf("\nTempo estimado (SPLAY): %.2f", estimatedTime[SPLAYT]);
    // printf("\nTempo estimado (RBT): %.2f", estimatedTime[RBT]);

    // printf("\nTempo de criacao (SPLAY): %.2f", generationTime[SPLAYT]);
    // printf("\nTempo de criacao (RBT): %.2f", generationTime[RBT]);

    // printf("\nTempo de busca (SPLAY): %.2f", searchTime[SPLAYT]);
    // printf("\nTempo de busca (RBT): %.2f", searchTime[RBT]);

}

void fprintResults(FILE* file, float* estimatedTime, float* generationTime, float* searchTime, BST bst, AVL avl) {
    bstFilePrintResults(bst, file);
    fprintf(file, "\nTempo estimado: %.2f h", estimatedTime[BSTT]);
    fprintf(file, "\nTempo de criacao: %.2f ms", generationTime[BSTT]);
    fprintf(file, "\nTempo de busca: %.2f ms", searchTime[BSTT]);
    
    avlFilePrintResults(avl, file);
    fprintf(file, "\nTempo estimado: %.2f h", estimatedTime[AVLT]);
    fprintf(file, "\nTempo de criacao: %.2f ms", generationTime[AVLT]);
    fprintf(file, "\nTempo de busca: %.2f ms", searchTime[AVLT]);
    
    // fprintf(file, "\nTempo estimado (SPLAY): %.2f", estimatedTime[SPLAYT]);
    // fprintf(file, "\nTempo estimado (RB): %.2f", estimatedTime[RBT]);
    
    // fprintf(file, "\nTempo de criacao (SPLAY): %.2f", generationTime[SPLAYT]);
    // fprintf(file, "\nTempo de criacao (RBT): %.2f", generationTime[RBT]);

    // fprintf(file, "\nTempo de busca (SPLAY): %.2f", searchTime[SPLAYT]);
    // fprintf(file, "\nTempo de busca (RBT): %.2f", searchTime[RBT]);
}