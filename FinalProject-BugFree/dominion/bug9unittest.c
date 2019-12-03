/*************************************************
* FileName: bug7unittest.c
* Author:	Glen Gougeon
* Class:	CS362 Software Engineering II
* Created:	11-30-2019
* Last Mod:	12-2-2019
*
* Assignement: Final Project Part B/
*
* Description:	Unit Test for Bug 9:
*				TributeRevealedCards Illegal Action Gain
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

// TEST PROTO-TYPES
void initTestGame(int numPlayers, int* kDeck, int mySeed, struct gameState* game);

// sets variable states and calls 'cardEffect' w/ 'tribute' as card
void subTestTribute(
	int testNo,
	char* message,
	struct gameState* oldState,
	struct gameState* newState,
	int newHandSize,
	int tribute_index,
	int cardInDeck
);

// must be called after subTestTribute
void runAsserts(
	struct gameState* before, 
	struct gameState* after 
);

// HELPER PROTOS-TYPES
void emptyDeck(int player, struct gameState* state);
void emptyPlayedCards(struct gameState* state);
void emptyDiscard(int player, struct gameState* state);

// HAND RELATED PROTO-TYPES
void emptyHand(int player, struct gameState* dState);
void setNewHandCount(int player, struct gameState* state, int newHandSize);
void setAtHandPos(int player, struct gameState* state, int card, int handPos);

int main()
{
	printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	printf("*** START Bug9 Unit Test: TributeRevealedCards Illegal Action Gain ***\n");
	printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\n");

	int kingdomCards[10] = { adventurer, ambassador, baron, curse, estate, tribute, minion, mine, gardens, remodel };

	struct gameState G;
	struct gameState backup;

	int seed = 1;
	int numPlayers = 2;
	//int currentPlayer = 0;
	int newHandSize = 1;
	int tribute_index = 0; // handPos tribute .. not really used
	int cardInDeck = -1;
	char* test_1_message = "Place 2 treasures at top of next player's deck, then call tribute.";
	char* test_2_message = "Place 2 Victory cards at top of next player's deck, then call tribute.";
	char* test_3_message = "Place 2 non-coin & non-victory (curses) at top of next player's deck, then call tribute.";

	// SUBTEST 1 -- 2 adventurer cards
	initTestGame(numPlayers, kingdomCards, seed, &G);
	G.numActions = 0;
	cardInDeck = adventurer;
	subTestTribute(1, test_1_message, &backup, &G, newHandSize, tribute_index, cardInDeck);
	runAsserts(&backup,&G);
	printf("------------------------------------------------------------------------------------\n");

	// SUBTEST 2 -- 2 curse cards
	initTestGame(numPlayers, kingdomCards, seed, &G);
	G.numActions = 0;
	cardInDeck = curse;
	subTestTribute(2, test_2_message, &backup, &G, newHandSize, tribute_index, cardInDeck);
	runAsserts(&backup, &G);
	printf("------------------------------------------------------------------------------------\n");

	return 0;
}

/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
/* *** FUNCTION SPECIFICATIONS ***																	*/
/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */

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

// set player to remove all cards from current player's deck  
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

// set player to remove all cards from current player's deck   
void emptyPlayedCards(struct gameState* state)
{
	int i = 0;
	while (i < state->playedCardCount)
	{
		state->playedCards[i] = -1;
		i++;
	}
	state->playedCardCount = 0;
}

// eliminate all cards from discard.   
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

// Use for running subtest on 'tribute' case of 'cardEffect' function
void subTestTribute(
	// parameters
	int testNo,
	char* message,
	struct gameState* oldState, 
	struct gameState* newState, 
	int newHandSize, 
	int tribute_index, 
	int cardInDeck)
	// end paramaters
{
	printf(" SUB-TEST %d: %s\n\n", testNo, message);

	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
	/* ** SETUP TEST SECTION ** */
	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */

	int idxOfChoice1 = 0; // choice1 tribute
	int blank = -1;
	int coinBonus = 0;
	int currentPlayer = newState->whoseTurn;
	int nextPlayer = currentPlayer + 1;

	/* MMMMMMMMM SET UP CURRENT PLAYER *** MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

	// empty every deck currentPlayer has
	emptyDeck(currentPlayer, newState);
	emptyPlayedCards(newState); // req'd. b/c 'discardCard' is broken !!
	emptyDiscard(currentPlayer, newState);
	emptyHand(currentPlayer, newState);

	// set up new hand size after empty
	setNewHandCount(currentPlayer, newState, newHandSize);

	// place tribute card in hand
	setAtHandPos(currentPlayer, newState, tribute, tribute_index);

	// set coins appropriately
	updateCoins(currentPlayer, newState, coinBonus);

	/* MMMMMMMMM SET UP NEXT PLAYER *** MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

	// empty every deck currentPlayer has
	emptyDeck(nextPlayer, newState);
	emptyPlayedCards(newState); // req'd. b/c 'discardCard' is broken !!
	emptyDiscard(nextPlayer, newState);
	emptyHand(nextPlayer, newState);
	
	// subtest 1 = ensure only 2 golds are in next player's hand
	// subtest 2 = ensure only 2 victory cards are in next player's hand
	// subtest 3 = ensure only 2 action cards are in next player's hand
	// subtest 4 = ensure only 2..
	//			   non-treasure, non-victory, non-action cards are in next player's hand

	// empty supply of cardInDeck to ensure the ones 
	// going into deck are the only ones possible
	newState->supplyCount[cardInDeck] = 0;

	// place 2 cards in nextPlayer's deck then increment
	newState->deck[nextPlayer][newState->deckCount[nextPlayer]] = cardInDeck;
	newState->deckCount[nextPlayer]++;
	newState->deck[nextPlayer][newState->deckCount[nextPlayer]] = cardInDeck;
	newState->deckCount[nextPlayer]++;

	/* BACK UP STATE BEFORE CALL */
	memset(oldState, '\0', sizeof(*oldState));
	oldState = newState;

	/* CALL TO TRIBUTE <-----------------------------------------------------*/
	cardEffect(tribute, idxOfChoice1, blank, blank, newState, tribute_index, &coinBonus);

}

/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
/* ** ASSERTS SECTION ** */
/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
void runAsserts(struct gameState* before, struct gameState* after)
{
	int currentPlayer = after->whoseTurn;
	//int currentPlayer = 0;
	int nextPlayer = currentPlayer + 1;

	char name[MAX_STRING_LENGTH];
	char nombre[MAX_STRING_LENGTH];

	/*Assert that the top 2 cards from the next player's previous deck
	are exactly the ones found in their discard (playedCards) after the use of tribute.*/
	if (before->deck[nextPlayer][before->deckCount[nextPlayer]] != after->discard[nextPlayer][after->discardCount[nextPlayer]])
	{
		if (before->deck[nextPlayer][before->deckCount[nextPlayer] - 2] != after->discard[nextPlayer][after->discardCount[nextPlayer] - 2])
		{
			memset(name, '\0', sizeof name);
			cardNumToName(after->discard[nextPlayer][after->discardCount[nextPlayer] - 1], name);

			memset(nombre, '\0', sizeof nombre);
			cardNumToName(after->discard[nextPlayer][after->discardCount[nextPlayer] - 1], nombre);

			printf("Tribute Error: previous top 2 deck cards next player..\n");
			printf("               not found in next player's discard.\n\n");
			printf("               Discard top 2:\n");
			printf("               G.discard[%d]: %s\n", after->discardCount[nextPlayer] - 1, name);
			printf("               G.discard[%d]: %s\n\n", after->discardCount[nextPlayer] - 2, nombre);
		}
	}

	/*Assert that the counts of the next player's discard and deck counts
	are correct, such that their deckCount is 2 less than previously, and
	their discardCount is 2 greater than previously.*/
	if (before->deckCount[nextPlayer] - 2 != after->deckCount[nextPlayer])
	{
		if (before->discardCount[nextPlayer] + 2 != after->discardCount[nextPlayer])
		{
			printf("Tribute Error: next player's deckCount not -2 previous.\n");
			printf("             : next player's discardCount not +2 previous.\n\n");

		}
	}

	int total_non_treasure_non_victories = 0;
	if (after->discard[nextPlayer][after->discardCount[nextPlayer] - 1] == adventurer ||
		after->discard[nextPlayer][after->discardCount[nextPlayer] - 2] == adventurer) {

		totalActions += 2;
	}

	if (totalActions == 2)
	{
		if (before->numActions + 2 != after->numActions)
		{
			printf("TRACE: totalActions %d\n", totalActions);
			printf("Tribute Error: You had 1 action card in discard, but didn't gain 2 actions.\n\n");
		}
	}
	else if (totalActions == 0 && (before->numActions != after->numActions))
	{
		printf("TRACE: totalActions %d\n", totalActions);
		printf("TRACE: before->numActions %d, after->numActions %d\n", before->numActions, after->numActions);
		printf("Tribute Error: You gained actions, but didn't find any action cards in discard.\n\n");
	}
}