
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


/*
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


void assertMore (int costs, int newCost) {

    // Assertion
    if (costs == newCost) {
        printf("Purchase over 5 coins blocked. No errors.\n");
    }

    else if (newCost <= (costs+5)) {
      printf("Purchase was acceptable (less than 5 coins). No errors.\n");
    }

    else {
        printf("Player was able to buy card worth more than 5. Error detected.\n");
    }
}
*/


int main () {

printf("/****************************************\n");
printf("/***********   BUG 6 TEST   *************\n");
printf("/****************************************\n");

printf("\n");
printf("I actually spent a lot of time debugging this section before concluding that there is, in fact, no bug.\n");
printf("The full write-up can be found in Part A, but a synopsis is below:\n");

printf("\n");

printf("The bug thread states that the updateCoins function is causing a bug by simply giving the player 5 coins.\n");
printf("The argument is that this allows the player to spend more than 5 coins or keep the extras that are unspent.\n");
printf("However, this is based on a misunderstanding of updateCoins(). This function doesn't simply give coins.\n");
printf("Rather, it resets the coin count, and then gives fresh coins according to the player's hand and any passed-in bonus.\n");
printf("In Feast, the hand is set to -1, and then 5 is passed into the updateCoins as a bonus, effectively resetting the coin count to 5, regardless of a player's treasure cards or existing coins.\n");
printf("Then, the hand is restored, and updateCoins is called again with a bonus of 0, clearing any leftover coins from Feast.\n");
printf("Together, these negate both of the bugs listed in the bug thread. There is no bug with updateCoins in Feast.\n");


/*
int currentPlayer = 0;

struct gameState G;

int * k = kingdomCards(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

initializeGame (2, k, 8, &G);

G.whoseTurn = currentPlayer;

//feast == 9;


G.supplyCount[15] = 3;
G.supplyCount[7] = 3;
G.supplyCount[6] = 3;

G.hand[currentPlayer][0] = feast;
G.hand[currentPlayer][1] = feast;
G.hand[currentPlayer][2] = gold;

updateCoins(currentPlayer, &G, 0);

int coinStorage = G.coins;


printf("\n");
printf("Testing buying card worth less than 5....\n");
//int playCard(int handPos, int choice1, int choice2, int choice3, struct gameState *state)
playCard(0, 15, 0, 0, &G);
//cardEffect(9, 15, 0, 0, &G, 0, 0);

int currentCoin = G.coins;

assertLess(coinStorage, currentCoin);


G.whoseTurn = currentPlayer;
G.phase = 0;
G.numActions = 1;

int choice1 = 7;
G.hand[currentPlayer][0] = feast;
G.hand[currentPlayer][1] = feast;
G.hand[currentPlayer][2] = gold;
//G.handCount[currentPlayer] = 3;

printf("\n");
printf("Testing buying card worth more than 5....\n");

int costs = 0;
//cardEffect(9, choice1, 0, 0, &G, 1, 0);
for (int e = 0; e < G.handCount[currentPlayer]; e++) {
  costs = costs + getCost(G.hand[currentPlayer][e]);
  //printf("adding Card: %d, Cost: %d\n", G.hand[currentPlayer][e], getCost(G.hand[currentPlayer][e]));
}

playCard(0, choice1, 0, 0, &G);


int newCost = 0;
//cardEffect(9, choice1, 0, 0, &G, 1, 0);
for (int e = 0; e < G.handCount[currentPlayer]; e++) {
  newCost = newCost + getCost(G.hand[currentPlayer][e]);
  //printf("adding Card: %d, Cost: %d\n", G.hand[currentPlayer][e], getCost(G.hand[currentPlayer][e]));

}

//printf("cost, %d, new cost %d\n", costs, newCost);


assertMore(costs, newCost);
*/

printf("\n");
printf("\n");
printf("\n");

return 0;
}
