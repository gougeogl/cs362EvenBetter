#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

void assertGameOver(struct gameState *G) {
    
    // Assertion
    if (isGameOver(G) == 1) {
        printf("Game over status is correctly evaluated for 3 empty piles. No errors.\n\n");
    }
    else {
        printf("Game over status is incorrectly evaluated for 3 empty piles. Error encountered.\n\n");
    }  
}
 
int main () {
    int numPlayers = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine
           , remodel, smithy, village, baron, great_hall};   
    int seed = 10; 
    struct gameState G;
    int initResult;

    printf("****************************************\n");
    printf("***********   BUG 4 TEST   *************\n");
    printf("****************************************\n");

    printf("\nBegin Testing isGameOver():\n");

    memset(&G, 23, sizeof(struct gameState)); 

    initResult = initializeGame(numPlayers, k, seed, &G);

    // Proceed if game initialization successful
    if (initResult == 0) {    
        // Set supply counts
        for (int i = 0; i < 24; i++) {
            G.supplyCount[i] = i + 1;
        }

        // Manually set supply count for last 3 cards
        G.supplyCount[24] = 0;
        G.supplyCount[25] = 0;
        G.supplyCount[26] = 0;
        
        assertGameOver(&G);

        printf("\n\n");        
    }          
                                        
    return 0;
}