#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

void assertMineCard(int card, int choice1, int choice2, int choice3, struct gameState *G, int handPos, int *bonus) {
    
    // Assertion
    if (cardEffect(card, choice1, choice2, choice3, G, handPos, bonus) == 0) {
        printf("Choice of card to gain evaluated as valid and was accepted. No errors.\n\n");
    }
    else {
        printf("Choice of card to gain evaluated as invalid and was not accepted. Error encountered.\n\n");
    }
}
 
int main () {
    int numPlayers = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine
           , remodel, smithy, village, baron, great_hall};   
    int seed = 10; 
    struct gameState G;
    int initResult;
    int handCount = 2;
    int cards[handCount];
    int currentPlayer = 0;

    // Argument variables for cardEffect()
    int card;
    int choice1;
    int choice2;
    int choice3;
    int handPos;
    int *bonus;

    printf("****************************************\n");
    printf("***********   BUG 2 TEST   *************\n");
    printf("****************************************\n");

    printf("\nBegin Testing Mine cardEffect():\n");

    memset(&G, 23, sizeof(struct gameState)); 

    initResult = initializeGame(numPlayers, k, seed, &G);

    // Proceed if game initialization successful
    if (initResult == 0) {    

        // Set hand and hand count for all players
        for (int i = 0; i < numPlayers; i++) {
            G.handCount[i] = handCount;
            memcpy(G.hand[i], cards, sizeof(int) * handCount);  
        }

        // Manually set the cards in the first player's hand
        G.hand[currentPlayer][0] = mine;
        G.hand[currentPlayer][1] = gold;

        // Set turn to first player
        G.whoseTurn = currentPlayer;

        // Set arguments so that current player will choose 
        // to trash a gold card to gain a copper card
        card = mine;
        choice1 = 1;        // Index of gold card
        choice2 = copper;   // Copper card selected to be gained
        choice3 = 0;        // Unused
        handPos = 0;        // Unused
        bonus = NULL;       // Unused

        assertMineCard(card, choice1, choice2, choice3, &G, handPos, bonus);

        printf("\n\n");
    }          
                                        
    return 0;
}
