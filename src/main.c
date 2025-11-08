#include "trees.h"
#include <string.h>
#include <stdlib.h>
#include "cronometer.h"

#define DELIMITERS ",\n"
#define STEAM_GAMES argv[1]
#define DESIRED_GAMES argv[2]
#define OUTPUT_FILE argv[3]

void printResults(float* estimatedTime, float* creationTime, float* searchTime, BST bst, AVL avl, RBT rbt);
void fprintResults(FILE* file, float* estimatedTime, float* creationTime, float* searchTime, BST bst, AVL avl, RBT rbt);
typedef bool (*TreeInsertFunction)(void* tree, Game game);
float TimeGamesDataToTree(FILE* file, void* tree, Trees type);
float TimedGameTreeSearch(FILE* desiredGamesFile, void* tree, Trees type, float* estimatedTime);

int main(int argc, char* argv[]) {
    
    if (argc != 4) {
        printf("Numero de parametros incorreto. Entrada do programa deve ser:");
        printf(" exemplo <arq_steamgames> <arq_listajogos> <arq_saida>");
        return 1;
    }
    
    BST bst = bstNew();
    AVL avl = avlInitialize();
    RBT rbt = rbtNew();

    float generationTime[4];
    float searchTime[4];
    float estimatedTime[4] = {0, 0, 0, 0};

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
    generationTime[BSTT] = TimeGamesDataToTree(steamGamesFile, &bst, BSTT);
    
    searchTime[BSTT] = TimedGameTreeSearch(desiredGamesFile, &bst, BSTT, &estimatedTime[BSTT]);

    rewind(steamGamesFile);
    rewind(desiredGamesFile);

    // AVL TREE
    generationTime[AVLT] = TimeGamesDataToTree(steamGamesFile, &avl, AVLT);

    searchTime[AVLT] = TimedGameTreeSearch(desiredGamesFile, &avl, AVLT, &estimatedTime[AVLT]);    

    rewind(steamGamesFile);
    rewind(desiredGamesFile);

    // RBT TREE
    generationTime[RBTT] = TimeGamesDataToTree(steamGamesFile, &rbt, RBTT);

    searchTime[RBTT] = TimedGameTreeSearch(desiredGamesFile, &rbt, RBTT, &estimatedTime[RBTT]);  
     

    printResults(estimatedTime, generationTime, searchTime, bst, avl, rbt);
    fprintResults(outputFile, estimatedTime, generationTime, searchTime, bst, avl, rbt);
    
    bstClear(&bst);
    avlClear(&avl);
    rbtClear(&rbt);
    
    fclose(steamGamesFile);
    fclose(desiredGamesFile);
    fclose(outputFile);

    printf("\n\n\nEnd of program...\n");
    
    return 0;
}

void printResults(float* estimatedTime, float* generationTime, float* searchTime, BST bst, AVL avl, RBT rbt) {
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
    
    // printf("\nTempo estimado (SPLAY): %.2f h", estimatedTime[SPLAYT]);
    // printf("\nTempo de criacao (SPLAY): %.2f ms", generationTime[SPLAYT]);
    // printf("\nTempo de busca (SPLAY): %.2f ms", searchTime[SPLAYT]);
}

void fprintResults(FILE* file, float* estimatedTime, float* generationTime, float* searchTime, BST bst, AVL avl, RBT rbt) {
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
    
    // fprintf(file, "\nTempo estimado (SPLAY): %.2f h", estimatedTime[SPLAYT]);
    // fprintf(file, "\nTempo de criacao (SPLAY): %.2f ms", generationTime[SPLAYT]);
    // fprintf(file, "\nTempo de busca (SPLAY): %.2f ms", searchTime[SPLAYT]);

}

float TimeGamesDataToTree(FILE* file, void* tree, Trees type) {

    Cronometer insertionCron = cronNew();
    char line[1000];

    // Dataset loop
    while (fgets(line, sizeof(line), file)) {
        char *word = strtok(line, DELIMITERS);      // Gets the game's name(first word)

        Game game = { (char*)malloc(strlen(word) + 1), atof(strtok(NULL, DELIMITERS)) }; // Allocs name's memmory and gets the average gameplay time(second word)

        strncpy(game.name, word, strlen(word) + 1);  // Puts the game's name into the struct

        cronStart(&insertionCron);
        treeInsert(tree, game, type);      // Inserts into the tree   
        cronStop(&insertionCron);    
    }

    return insertionCron.millisec;
}

float TimedGameTreeSearch(FILE* desiredGamesFile, void* tree, Trees type, float* estimatedTime) {

    Cronometer searchCron = cronNew();
    char line[1000];
    *estimatedTime = 0;
    
    // Search loop
    while (fgets(line, sizeof(line), desiredGamesFile)) {   
       
        cronStart(&searchCron);
        void* desiredGame = treeSearch(tree, strtok(line, DELIMITERS), type);   // Gets desired game's name and search it on the tree
        cronStop(&searchCron);

        if (desiredGame != NULL) {  // Game was found in the dataset
            
            switch (type) {
                case BSTT:
                    NodeBST* aux = (NodeBST*)desiredGame;
                    *estimatedTime += aux->game.time;
                    break;
                case AVLT:
                    NodeAVL* aux2 = (NodeAVL*)desiredGame;
                    *estimatedTime += aux2->key.time;
                    break;
                case RBTT:
                    NodeRBT* aux3 = (NodeRBT*)desiredGame;
                    *estimatedTime += aux3->key.time;
                    break;
                case SPLAYT:
                    break;    
            }

        } 
    }
    return searchCron.millisec;
}