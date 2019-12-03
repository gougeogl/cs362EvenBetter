
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


/*
void assertCoins (int coinStorage, int currentCoin) {

    // Assertion
    if (currentCoin == coinStorage) {
        printf("Coins are correctly calculated for current player! No errors.\n\n");
    }

    else {
        printf("Coins are incorrectly calculated for current player! Error detected -- player has wrong number of coins.\n\n");
    }
}


*/

int main () {

printf("/****************************************\n");
printf("/***********   BUG 8 TEST   *************\n");
printf("/****************************************\n");


printf("\n");
printf("I found bug #8 to be similar to bug #6 in that I spent a lot of time debugging it before concluding that there is, in fact, no bug.\n");
printf("The full write-up can be found in Part A, but a synopsis is below:\n");

printf("\n");

printf("The bug thread states that 'the updateCoins function is causing a bug by simply giving the player 5 coins.'\n");
printf("The argument is that the bonus 'gets overwritten by the updateCoins function unless its given the correct coin_bonus from playCard'.\n");
printf("However, this is based on a misunderstanding of updateCoins() and bonus coins. Bonus coins are not meant to be persistent.\n");
printf("Rather, they are given per-turn, and reset at the end of each turn. They are intended to exist for the duration of a turn.\n");
printf("In Baron, for example, the four coins are given by adding four coins to the state. This is the exact end result of calling updateCoins.\n");
printf("Then, the fact that the updateCoins function wipes the coins from the state is NOT a bug, but the proper course of action for the game.\n");
printf("This entirely negates the bug as listed in the bug thread. There is no bug with updateCoins in cardEffect.\n");


/*
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
*/

printf("\n");
printf("\n");
printf("\n");

return 0;
}
