
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>



void assertScoreFor (int testScore, int calcScore) {

    // Assertion
    if (testScore == calcScore) {
        printf("Score is correctly calculated for current player. No errors.\n\n");
    }
    else {
        printf("Score is incorrectly calculated for current player. Error detected.\n\n");
    }
}



int calculateScore (int player, struct gameState *state) {

    int i;
    int score = 0;
    //score from hand
    for (i = 0; i < state->handCount[player]; i++)
    {
        if (state->hand[player][i] == curse) {
            score = score - 1;
        };
        if (state->hand[player][i] == estate) {
            score = score + 1;
        };
        if (state->hand[player][i] == duchy) {
            score = score + 3;
        };
        if (state->hand[player][i] == province) {
            score = score + 6;
        };
        if (state->hand[player][i] == great_hall) {
            score = score + 1;
        };
        if (state->hand[player][i] == gardens) {
            score = score + ( fullDeckCount(player, 0, state) / 10 );
        };
    }

    //score from discard
    for (i = 0; i < state->discardCount[player]; i++)
    {
        if (state->discard[player][i] == curse) {
            score = score - 1;
        };
        if (state->discard[player][i] == estate) {
            score = score + 1;
        };
        if (state->discard[player][i] == duchy) {
            score = score + 3;
        };
        if (state->discard[player][i] == province) {
            score = score + 6;
        };
        if (state->discard[player][i] == great_hall) {
            score = score + 1;
        };
        if (state->discard[player][i] == gardens) {
            score = score + ( fullDeckCount(player, 0, state) / 10 );
        };
    }

    //score from deck
    for (i = 0; i < state->deckCount[player]; i++)
    {
        if (state->deck[player][i] == curse) {
            score = score - 1;
        };
        if (state->deck[player][i] == estate) {
            score = score + 1;
        };
        if (state->deck[player][i] == duchy) {
            score = score + 3;
        };
        if (state->deck[player][i] == province) {
            score = score + 6;
        };
        if (state->deck[player][i] == great_hall) {
            score = score + 1;
        };
        if (state->deck[player][i] == gardens) {
            score = score + ( fullDeckCount(player, 0, state) / 10 );
        };
    }

    return score;
}



int main () {

printf("/****************************************\n");
printf("/***********   BUG 5 TEST   *************\n");
printf("/****************************************\n");

int currentPlayer = 0;

struct gameState G;

int * k = kingdomCards(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

initializeGame (2, k, 8, &G);

int testScore = calculateScore(currentPlayer, &G);

int calcScore = scoreFor (currentPlayer, &G);

assertScoreFor(testScore, calcScore);

printf("\n");
printf("\n");
printf("\n");

return 0;
};
