#include "strcompare.h"
#include "bst.h"
#include "avl.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define DELIMITERS ",\n"
#define STEAM_GAMES argv[1]
#define DESIRED_GAMES argv[2]
#define OUTPUT_FILE argv[3]

void printResults(float estimatedTime, BST bst, AVL avl);
void fprintResults(FILE* file, float estimatedTime, BST bst);

/// a files\teste.csv files\lista_teste.txt files\saida.txt

int comp = 0;

int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("Numero de parametros incorreto. Entrada do programa deve ser:");
        printf(" exemplo <arq_steamgames> <arq_listajogos> <arq_saida>");
        return 1;
    }

    BST bst = bstNew();
    AVL avl = avlInitialize();
    char line[1000];
    Game game;
    FILE* steamGamesFile = fopen(STEAM_GAMES, "r");

    if (steamGamesFile == NULL) {       // Checks file opening
        printf("Erro ao abrir o arquivo %s", STEAM_GAMES);
        return 1;
    }

    while (fgets(line, 1000, steamGamesFile)) {
        char *word = strtok(line, DELIMITERS);      // Gets the game's name(first word)
        strncpy(game.name, word, MAX_NAME_SIZE - 1);
        game.name[MAX_NAME_SIZE - 1] = '\0';        // Ensures null termination
        
        game.time = atof(strtok(NULL, DELIMITERS));     // Gets the average gameplay time(second word)

        //bstInsert(&bst, game);
        avlInsert(&avl, game);        // Inserts into the tree
    }

    fclose(steamGamesFile);

    float estimatedTime = 0;
    FILE* desiredGamesFile = fopen(argv[2], "r");

    if (desiredGamesFile == NULL) {       // Checks file opening
        printf("Erro ao abrir o arquivo %s", DESIRED_GAMES);
        return 1;
    }

    while (fgets(line, 1000, desiredGamesFile)) {

        // char* word = strtok(line, DELIMITERS);
        // printf("\n%s", word);
        //NodeBST* desiredGame = bstConsulta(bst.root, strtok(line, DELIMITERS));    // Gets desired game's name
        NodeAVL* desiredGame = avlConsulta(avl.root, strtok(line, DELIMITERS));
        if (desiredGame != NULL) { 
            //printf("\t esta no arquivo");
            //estimatedTime += desiredGame->game.time;
            estimatedTime += desiredGame->key.time;

        }
        //else printf("NAO ACHOU");
    }

    fclose(desiredGamesFile);

    printResults(estimatedTime, bst, avl);


    FILE* outputFile = fopen(OUTPUT_FILE, "w");
    
    //fprintResults(outputFile, estimatedTime, bst);

    fclose(outputFile);

    bstClear(&bst);

    return 0;
}

void printResults(float estimatedTime, BST bst, AVL avl) {
    printf("\nTempo estimado: %.2f", estimatedTime);
    bstPrintResults(bst);
    avlPrintResults(avl);
}

void fprintResults(FILE* file, float estimatedTime, BST bst) {

    fprintf(file, "\nTempo estimado: %.2f", estimatedTime);
    bstFilePrintResults(bst, file);

}


