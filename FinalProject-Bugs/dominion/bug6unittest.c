
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


G.supplyCount[15] = 3;
G.supplyCount[7] = 3;
G.supplyCount[6] = 3;

G.hand[currentPlayer][0] = feast;
G.hand[currentPlayer][1] = feast;

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
G.hand[currentPlayer][2] = 6;
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

printf("\n");
printf("\n");
printf("\n");

return 0;
};
