/*************************************************
* FileName: bug1unittest.c
* Author:	Glen Gougeon
* Class:	CS362 Software Engineering II
* Created:	11-29-2019
* Last Mod:	11-29-2019
* 
* Assignement: Final Project Part B/
*
* Description: Unit Test for Bug 1 
*
**************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"

enum TEST_FLAGS
{
	SAME_HAND = 800,
	DIFFERENT_HAND,
	SAME_DECK,
	DIFFERENT_DECK,
	SAME_DISCARD,
	DIFFERENT_DISCARD,
	PLUS_2_COINS,
	SAME_COINS,
	/* used by prep functions */
	FILL_DIFF,
	FILL_SAME
};


// TEST PROTO-TYPES
void initTestGame(int numPlayers, int* kDeck, int mySeed, struct gameState* game);

// HELPER PROTOS-TYPES
void emptyDeck(int player, struct gameState* state);
void emptyDiscard(int player, struct gameState* state);

// HAND RELATED PROTO-TYPES
void emptyHand(int player, struct gameState* dState);
void setNewHandCount(int player, struct gameState* state, int newHandSize);
void setAtHandPos(int player, struct gameState* state, int card, int handPos);

// COMPARISION PROTO-TYPES
int compareCoins(int player, struct gameState* before, struct gameState* after, int flag);
int compareNumActionsTribute(int player, struct gameState* before, struct gameState* after);
int compareHand(int player, struct gameState* before, struct gameState* after, int flag);
int compareDeck(int player, struct gameState* before, struct gameState* after, int limit, int flag);
int compareTopsAfter(int player, int* deckTops, int* discardTops);
void setTypesFoundTribute(int* oldDeckTops, int* coin_count, int* draw_card_count, int* num_actions_count);

// SAVE VALUES PROTO-TYPES
void savePreviousHandCounts(int* container, struct gameState* state);
void savePreviousDeckCounts(int* container, struct gameState* state);
void savePreviousDiscardCounts(int* container, struct gameState* state);

void saveTop2Deck(int player, struct gameState* state, int* topTwo);
void saveTop2Discard(int player, struct gameState* state, int* topTwo);

/* selects a random card from kingdomCards deck */
int _rand_of_kingdomCards();

/* generate an int in range [min..max] */
int _genRandRange(int min, int max);

int main()
{
	printf("*** START Bug1 Unit Test *** .\n");

	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
	/* ** SETUP TEST SECTION ** /
	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */

	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	struct gameState G;
	struct gameState backup;

	int seed = 1;
	int numPlayers = 2;
	int currentPlayer = 0;
	int newHandSize = 1;
	int idxOfChoice1 = 0; // choice1 mine
	int moneyToGet = silver; // choice2 mine
	int blank = -1;
	int coinBonus = 0;

	// initialize and set currentPlayer
	initTestGame(numPlayers, kingdomCards, seed, &G);
	currentPlayer = G.whoseTurn;

	// empty every deck currentPlayer has
	emptyDeck(currentPlayer, &G);
	emptyDiscard(currentPlayer, &G);
	emptyHand(currentPlayer, &G);

	// you will only have a copper in your hand
	setNewHandCount(currentPlayer, &G, newHandSize);

	// The only copper in the game is in currentPlayer's hand
	G.supplyCount[copper] = 0;

	// The only silver in the game is THIS in supply
	G.supplyCount[silver] = 0;
	G.supplyCount[silver] = 1;

	setAtHandPos(currentPlayer, &G, copper, idxOfChoice1);
	updateCoins(currentPlayer, &G);

	/* BACK UP STATE BEFORE CALL */
	memset(&backup, '\0', sizeof(backup));
	backup = G;

	/* CALL TO MINE <-----------------------------------------------------*/
	cardEffect(mine, idxOfChoice1, moneyToGet, blank, &G, blank, coinBonus);

	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
	/* ** ASSERTS SECTION ** /
	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */

	/* Assert that the choice1 card is found at the top of the trashPile,
		otherwise print that it is missing. */
	if(G.trashPile[G.trashCount -1] != backup.hand[currentPlayer][idxOfChoice1])
	{
		char name[MAX_STRING_LENGTH];
		memset(name, '\0', sizeof(name));
		cardNumToName(backup.hand[currentPlayer][idxOfChoice1], name);

		char nombre[MAX_STRING_LENGTH];
		memset(nombre, '\0', sizeof(nombre));
		cardNumToName(G.trashPile[G.trashCount -1], nombre);

		printf("Error Mine: previous choice1 not found at top of discard pile.\n");
		printf("BEFORE: choice1 at hand[%d]: %s\n", idxOfChoice1, name);
		printf("AFTER: trashPile[%d]: %s\n", G.trashCount - 1, nombre);
	}
	/*	Assert that choice1(an index) is a different card, otherwise print that it
		is the same. */
	if(backup.hand[currentPlayer][idxOfChoice1] == backup.hand[currentPlayer][idxOfChoice1])
	{
		printf("Error Mine: choice1 is still the same but shouldn't be.\n");
	}

	/*	Assert that the trashCount is 1 greater than the previous trashCount,
		otherwise print that "trashCount is not +1 previous." */
	if (backup.trashCount + 1 != G.trashCount);
	{
		printf("Error Mine: trashCount is still the same but shouldn't be.\n");
	}
	/*	Assert that the discardCount has not changed, otherwise print
		'discardCount changed but shouldn't have'. */
	if (backup.discardCount[currentPlayer] != G.discardCount[currentPlayer])
	{
		printf("Error Mine: discardCount changed but shouldn't have.\n");
	}

	/*	Assert if top of previous discard was not the same as choice1, and
		choice1 is found at the top of discard, print 'choice1 discarded
		not trashed'. */
	if (backup.discard[currentPlayer][backup.discardCount[currentPlayer]] != copper)
	{
		if (G.discard[currentPlayer][G.discardCount[currentPlayer] - 1] == copper)
		{
			printf("Error Mine: choice1 found at top of discard, but should at top of trashPile.\n");
		}
	}
	/*	Assert if top of previous discard was the same as choice1, and the
		top 2 cards in discard are both choice1, print 'choice1 discarded
		not trashed'. */
	if (backup.discard[currentPlayer][backup.discardCount[currentPlayer]] == copper)
	{
		if ((G.discard[currentPlayer][G.discardCount[currentPlayer] - 1] == copper) &&
			(G.discard[currentPlayer][G.discardCount[currentPlayer] - 2] == copper)
		{
			printf("Error Mine: choice1 discarded not trashed.\n");
		}
	}

	/*	Assert if backup.supplyCount[choice2] > 0 and choice2 was not in
		previous hand, and is not in current hand, print
		'choice2 not found in hand after mine'. */
	if (backup.supplyCount[moneyToGet] > 0)
	{
		int prevHandCount = backup.handCount[currentPlayer];
		int was_in_hand = 0;
		int not_in_hand = 1;

		int idx;
		for (idx = 0; idx < prevHandCount; idx++)
		{
			if (backup.hand[currentPlayer][idx] == moneyToGet)
			{
				was_in_hand = 1;
			}
		}
		if (was_in_hand == 0)
		{
			int iter;
			for (iter = 0; iter < G.handCount[currentPlayer]; iter++)
			{
				if (G.hand[currentPlayer][iter] == moneyToGet)
				{
					not_in_hand == 0;
				}
			}
		}
		if (not_in_hand == 1)
		{
			printf("Error Mine: choice1 not found in hand after the call.\n");
		}
	}
	/*	Compare the supplyCount[silver] before and after the call.If the
		current count is not 1 less than previous, then print
		'choice2 silver not 1 less than previous' */
	if (backup.supplyCount[silver] - 1 != G.supplyCount[silver])
	{
		printf("Error Mine: choice2 silver is not -1 previous supplyCount.\n");
	}

	printf("*** END Bug1 Unit Test *** .\n");
	return 0;
}

/* This function adds memset initialization, and prints error if fail  */
void initTestGame(int numPlayers, int* kDeck, int mySeed, struct gameState* game)
{
	memset(game, '\0', sizeof(struct gameState));   // clear mem of 

	int checkInit = initializeGame(numPlayers, kDeck, mySeed, game); // initialize 2 player game 

	if (checkInit == -1)
	{
		printf("Bad game initialization.\n");
	}

}

// set player to remove all estates from current player's deck  
void emptyDeck(int player, struct gameState* state)
{
	int i = 0;
	while (i < state->deckCount[player])
	{
		state->deck[player][i] = -1;
		i++;
	}
	state->deckCount[player] = 0;
}

// eliminate all estates from discard.   
void emptyDiscard(int player, struct gameState* state)
{
	int i = 0;
	while (i < state->discardCount[player])
	{
		state->discard[player][i] = -1;
		i++;
	}
	state->discardCount[player] = 0;
}

/* Sets current player's handCount to newHandSize, then
	overwrites everything in hand with -1 */
void emptyHand(int player, struct gameState* state)
{
	int i;
	for (i = 0; i < state->handCount[player]; i++)
	{
		state->hand[player][i] = -1;
	}
	state->handCount[player] = 0;
}

// sets handCount of player to newHandSize
void setNewHandCount(int player, struct gameState* state, int newHandSize)
{
	state->handCount[player] = newHandSize;
}

// adds indicated card in current player's hand at handPos 
void setAtHandPos(int player, struct gameState* state, int card, int handPos)
{
	state->hand[player][handPos] = card;
}
