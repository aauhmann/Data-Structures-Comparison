#include "trees.h"
#include "chronometer.h"
#include "printResults.h"

#define DELIMITERS ",\n"
#define STEAM_GAMES argv[1]
#define DESIRED_GAMES argv[2]
#define OUTPUT_FILE argv[3]
#define NUM_ARGS 4

float TimedGamesDataToTree(FILE* file, void* tree, Trees type);
float TimedGameTreeSearch(FILE* desiredGamesFile, void* tree, Trees type, float* estimatedTime);

int main(int argc, char* argv[]) {
    
    if (argc != NUM_ARGS) {
        printf("Numero de parametros incorreto. Entrada do programa deve ser:");
        printf(" exemplo <arq_steamgames> <arq_listajogos> <arq_saida>");
        return 1;
    }
    
    BST bst = bstNew();
    AVL avl = avlNew();
    RBT rbt = rbtNew();
    SPT spt = sptNew();

    float generationTime[NUM_TREES];
    float searchTime[NUM_TREES];
    float estimatedTime[NUM_TREES];

    FILE* steamGamesFile = fopen(STEAM_GAMES, "r");
    FILE* desiredGamesFile = fopen(DESIRED_GAMES, "r");
    FILE* outputFile = fopen(OUTPUT_FILE, "w");
    
    if (steamGamesFile == NULL) {       // Checks file opening
        printf("Erro ao abrir o arquivo %s", STEAM_GAMES);
        return 1;
    }
    
    if (desiredGamesFile == NULL) {       
        printf("Erro ao abrir o arquivo %s", DESIRED_GAMES);
        return 1;
    }
    
    if (outputFile == NULL) {
        printf("Erro ao abrir o arquivo %s", OUTPUT_FILE);
        return 1;
    }
    
    
    // BST TREE
    generationTime[BSTT] = TimedGamesDataToTree(steamGamesFile, &bst, BSTT);
    searchTime[BSTT] = TimedGameTreeSearch(desiredGamesFile, &bst, BSTT, &estimatedTime[BSTT]);
    
    rewind(steamGamesFile);
    rewind(desiredGamesFile);
    
    // AVL TREE
    generationTime[AVLT] = TimedGamesDataToTree(steamGamesFile, &avl, AVLT);
    searchTime[AVLT] = TimedGameTreeSearch(desiredGamesFile, &avl, AVLT, &estimatedTime[AVLT]);    

    rewind(steamGamesFile);
    rewind(desiredGamesFile);

    // RB TREE
    generationTime[RBTT] = TimedGamesDataToTree(steamGamesFile, &rbt, RBTT);
    searchTime[RBTT] = TimedGameTreeSearch(desiredGamesFile, &rbt, RBTT, &estimatedTime[RBTT]);
    
    rewind(steamGamesFile);
    rewind(desiredGamesFile);

    // SPLAY TREE
    generationTime[SPLAYT] = TimedGamesDataToTree(steamGamesFile, &spt, SPLAYT);
    postInsertionHeight = sptHeight(spt);
    searchTime[SPLAYT] = TimedGameTreeSearch(desiredGamesFile, &spt, SPLAYT, &estimatedTime[SPLAYT]);
    
    
    printResults(estimatedTime, generationTime, searchTime, bst, avl, rbt, spt);
    fprintResults(outputFile, estimatedTime, generationTime, searchTime, bst, avl, rbt, spt);
    
    bstClear(&bst);
    avlClear(&avl);
    rbtClear(&rbt);
    sptClear(&spt);
    
    fclose(steamGamesFile);
    fclose(desiredGamesFile);
    fclose(outputFile);

    printf("\n\n\nEnd of program...\n");
    
    return 0;
}

float TimedGamesDataToTree(FILE* file, void* tree, Trees type) {

    Chronometer insertionChron = chronNew();
    char line[1000];

    chronStart(&insertionChron);

    // Dataset loop
    while (fgets(line, sizeof(line), file)) {
        char *word = strtok(line, DELIMITERS);      // Gets the game's name(first word)

        Game game = { (char*)malloc(strlen(word) + 1), atof(strtok(NULL, DELIMITERS)) }; // Allocs name's memmory and gets the average gameplay time(second word)
        
        strncpy(game.name, word, strlen(word) + 1);  // Puts the game's name into the struct

        treeInsert(tree, game, type);      // Inserts into the tree   
        
    }
    
    chronStop(&insertionChron);

    return insertionChron.millisec;
}

float TimedGameTreeSearch(FILE* desiredGamesFile, void* tree, Trees type, float* estimatedTime) {

    Chronometer searchChron = chronNew();
    char line[1000];
    *estimatedTime = 0;
    
    chronStart(&searchChron);

    // Search loop
    while (fgets(line, sizeof(line), desiredGamesFile)) {   
       
        void* desiredGame = treeSearch(tree, strtok(line, DELIMITERS), type);   // Gets desired game's name and search it on the tree
        
        if (desiredGame != NULL) {  // Game was found in the dataset
            
            switch (type) {
                case BSTT: {
                    NodeBST* aux = (NodeBST*)desiredGame;
                    *estimatedTime += aux->game.time;
                    break;
                }
                case AVLT: {
                    NodeAVL* aux2 = (NodeAVL*)desiredGame;
                    *estimatedTime += aux2->key.time;
                    break;
                }
                case RBTT: {
                    NodeRBT* aux3 = (NodeRBT*)desiredGame;
                    *estimatedTime += aux3->key.time;
                    break;
                }
                case SPLAYT: {
                    NodeSPT* aux4 = (NodeSPT*)desiredGame;
                    *estimatedTime += aux4->data.time;
                    break;
                }
            }
            
        } 
    }

    chronStop(&searchChron);

    return searchChron.millisec;
}