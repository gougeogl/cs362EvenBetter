
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>



void assertLess (int coinStorage, int currentCoin) {

    // Assertion
    if (currentCoin == coinStorage) {
        printf("Coins are correctly calculated for current player when buying card worth less than 5. No errors.\n\n");
    }
    else if (currentCoin > coinStorage) {
        printf("Coins are incorrectly calculated for current player when buying card worth less than 5. Error detected -- player has too many coins.\n\n");
    }

    else {
        printf("Coins are incorrectly calculated for current player when buying card worth less than 5. Error detected -- player had too few coins.\n\n");
    }
}

void assertMore (int choiceCost) {

    // Assertion
    if (choiceCost <= 5) {
        printf("Purchase was acceptable (less than 5 coins). No errors.\n");
    }

    else {
        printf("Player was able to buy card worth more than 5. Error detected.\n");
    }
}



int main () {

printf("/****************************************\n");
printf("/***********   BUG 6 TEST   *************\n");
printf("/****************************************\n");

int currentPlayer = 0;

struct gameState G;

int * k = kingdomCards(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

initializeGame (2, k, 8, &G);

G.whoseTurn = currentPlayer;

//feast == 9;

G.coins = 0;

int coinStorage = G.coins;

G.supplyCount[15] = 3;
G.supplyCount[7] = 3;

G.hand[currentPlayer][0] = feast;
G.hand[currentPlayer][1] = feast;

printf("\n");
printf("Testing buying card worth less than 5....\n");
//int playCard(int handPos, int choice1, int choice2, int choice3, struct gameState *state)
playCard(0, 15, 0, 0, &G);
//cardEffect(9, 15, 0, 0, &G, 0, 0);

int currentCoin = G.coins;

assertLess(coinStorage, currentCoin);

int choice1 = 7;
G.hand[currentPlayer][0] = 9;
G.hand[currentPlayer][1] = 9;
G.hand[currentPlayer][2] = 6;
//G.handCount[currentPlayer] = 3;

printf("\n");
printf("Testing buying card worth more than 5....\n");

//cardEffect(9, choice1, 0, 0, &G, 1, 0);
playCard(0, choice1, 0, 0, &G);

int choiceCost = getCost(choice1);

assertMore(choiceCost);

printf("\n");
printf("\n");
printf("\n");

return 0;
}
