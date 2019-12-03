
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>



void assertCoins (int coinStorage, int currentCoin) {

    // Assertion
    if (currentCoin == coinStorage) {
        printf("Coins are correctly calculated for current player! No errors.\n\n");
    }

    else {
        printf("Coins are incorrectly calculated for current player! Error detected -- player has wrong number of coins.\n\n");
    }
}




int main () {

printf("/****************************************\n");
printf("/***********   BUG 8 TEST   *************\n");
printf("/****************************************\n");

int currentPlayer = 0;

struct gameState G;

int * k = kingdomCards(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

initializeGame (2, k, 8, &G);

G.whoseTurn = currentPlayer;

//feast == 9;

G.hand[currentPlayer][0] = baron;

updateCoins(currentPlayer, &G, 0);

int coinStorage = G.coins;


printf("\n");
printf("Testing gaining bonus coins....\n");
//int playCard(int handPos, int choice1, int choice2, int choice3, struct gameState *state)
playCard(0, 0, 0, 0, &G);
//cardEffect(9, 15, 0, 0, &G, 0, 0);

int currentCoin = G.coins;

assertCoins(coinStorage, currentCoin);

printf("\n");
printf("\n");
printf("\n");

return 0;
};
