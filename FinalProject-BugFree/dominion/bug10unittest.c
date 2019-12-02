#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

void assertAmbassadorCard(int card, int choice1, int choice2, int choice3, struct gameState *G, int handPos, int *bonus) {
    
    // Assertion
    if (cardEffect(card, choice1, choice2, choice3, G, handPos, bonus) == 0) {
        printf("Choice and # of cards to return to supply determined as valid. No errors.\n\n");
    }
    else {
        printf("Choice and # of cards to return to supply determined as invalid. Error encountered.\n\n");
    }
}
 
int main () {
    int numPlayers = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine
           , remodel, smithy, village, baron, great_hall};   
    int seed = 10; 
    struct gameState G;
    int initResult;
    int handCount = 4;
    int cards[handCount];
    int currentPlayer = 0;

    // Argument variables for cardEffect()
    int card;
    int choice1;
    int choice2;
    int choice3;
    int handPos;
    int *bonus;

    printf("\n\nBegin Testing Ambassador cardEffect:\n");

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
        G.hand[currentPlayer][0] = ambassador;
        G.hand[currentPlayer][1] = baron;
        G.hand[currentPlayer][2] = gold;
        G.hand[currentPlayer][3] = baron;

        // Set turn to first player
        G.whoseTurn = currentPlayer;

        // Set arguments so that current player will choose 
        // to return 2 baron cards to the supply
        card = ambassador;
        choice1 = 1;        // Index of card to reveal
        choice2 = 2;        // Number (up to 2) to return to supply
        choice3 = 0;        // Unused
        handPos = 0;        // Position of ambassador card
        bonus = NULL;       // Unused

        assertAmbassadorCard(card, choice1, choice2, choice3, &G, handPos, bonus);
    }          
                                        
    return 0;
}
