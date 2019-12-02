/*************************************************
* FileName: bug1unittest.c
* Author:	Glen Gougeon
* Class:	CS362 Software Engineering II
* Created:	11-29-2019
* Last Mod:	11-29-2019
* 
* Assignement: Final Project Part B/
*
* Description:	Unit Test for Bug 1: 
*				Mine Money to Trash only discards 
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
	printf("*** START Bug1 Unit Test: Mine Money to Trash Only Discards ***\n");
	printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	printf("Note* discardCard() broken. Where would test contents of discard..\n");
	printf("      I am testing against contents of playedCards instead.\n\n");

	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
	/* ** SETUP TEST SECTION ** */
	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */

	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	struct gameState G;
	struct gameState backup;

	int seed = 1;
	int numPlayers = 2;
	int currentPlayer = 0;
	int newHandSize = 2;
	int mine_index = 0; // handPos remodel
	int idxOfChoice1 = 1; // choice1 mine
	int moneyToGet = silver; // choice2 mine
	int blank = -1;
	int coinBonus = 0;

	// initialize and set currentPlayer
	initTestGame(numPlayers, kingdomCards, seed, &G);
	currentPlayer = G.whoseTurn;

	// empty every deck currentPlayer has
	emptyDeck(currentPlayer, &G);
	emptyPlayedCards(&G); // req'd. b/c 'discardCard' is broken !!
	emptyDiscard(currentPlayer, &G);
	emptyHand(currentPlayer, &G);

	// you will only have a copper in your hand
	setNewHandCount(currentPlayer, &G, newHandSize);

	// place mine card in hand
	setAtHandPos(currentPlayer, &G, mine, mine_index);

	// The only copper in the game is in currentPlayer's hand
	G.supplyCount[copper] = 0;

	// The only silver in the game is THIS in supply
	G.supplyCount[silver] = 0;
	G.supplyCount[silver] = 1;

	setAtHandPos(currentPlayer, &G, copper, idxOfChoice1);
	updateCoins(currentPlayer, &G, coinBonus);

	/* BACK UP STATE BEFORE CALL */
	memset(&backup, '\0', sizeof(backup));
	backup = G;

	/* CALL TO MINE <-----------------------------------------------------*/
	cardEffect(mine, idxOfChoice1, moneyToGet, blank, &G, mine_index, &coinBonus);

	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
	/* ** ASSERTS SECTION ** */
	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */

	/*	Assert that choice1(an index) is a different card, otherwise print that it
		is the same. */
	if(backup.hand[currentPlayer][idxOfChoice1] == G.hand[currentPlayer][idxOfChoice1])
	{
		printf("Error Mine: choice1 is still the same but shouldn't be.\n\n");
	}

	/*	Assert if top of previous discard was not the same as choice1, and
	choice1 is found at the top of discard, print 'choice1 discarded
	not trashed'. */

	//if (backup.discard[currentPlayer][backup.discardCount[currentPlayer] - 1] != copper)
	if (backup.playedCards[backup.playedCardCount - 1] != copper)
	{
		if (G.playedCards[G.playedCardCount - 1] == copper)
		{
			//printf("Error 'discardCard': should be discard .. but is playedCards array\n");
			printf("Error Mine: choice1 found in playedCards, but should be at top of trashPile.\n\n");
		}
	}

	/*	Assert if top of previous discard was the same as choice1, and the
	top 2 cards in discard are both choice1, print 'choice1 discarded
	not trashed'. */
	//if (backup.discard[currentPlayer][backup.discardCount[currentPlayer] -1] == copper)
	//{
	//	if ((G.discard[currentPlayer][G.discardCount[currentPlayer] - 1] == copper) &&
	//	    (G.discard[currentPlayer][G.discardCount[currentPlayer] - 2] == copper))
	//	{
	//		printf("Error Mine: choice1 discarded not trashed.\n\n");
	//	}
	//}

	if (backup.playedCards[backup.playedCardCount - 1] == copper)
	{
		if ((G.playedCards[G.playedCardCount - 1] == copper) &&
			(G.playedCards[G.playedCardCount - 2] == copper))
		{
			//printf("Error 'discardCard': should be discard .. but is playedCards array\n");
			printf("Error Mine: choice1 found in playedCards NOT in trash.\n\n");
		}
	}

	/*	Assert that the discardCount has not changed, otherwise print
		'discardCount changed but shouldn't have'. */

	//if (backup.discardCount[currentPlayer] != G.discardCount[currentPlayer])
	if (backup.playedCardCount -1 != G.playedCardCount -1)
	{
		//printf("Error 'discardCard': should be discardCount .. but is playedCardCount array\n");
		printf("Error Mine: playedCardCount changed.\n\n");
	}

	/* The handCount should change.
	   Assert if backup.handCount[currentPlayer] == G.handCount[currentPlayer]
	   print 'Error Mine: handCount didn't change.\n'	*/
	if (backup.handCount[currentPlayer] == G.handCount[currentPlayer])
	{
		printf("Error Mine: handCount didn't change.\n");
	}

	/*	Assert if backup.supplyCount[choice2] > 0 and choice2 was not in
		previous hand, and is not in current hand, print
		'choice2 not found in hand after mine'. */
	if (backup.supplyCount[moneyToGet] > 0)
	{
		int found = 0;
		int iter;
		for (iter = 0; iter < G.handCount[currentPlayer]; iter++)
		{
			// is it in hand now ??
			if (G.hand[currentPlayer][iter] == moneyToGet)
			{
				found = 1;
				break;
			}

		}

		if (!found)
		{
			printf("Error Mine: choice2 not found in hand after the call.\n\n");
		}

	}

	/*	Compare the supplyCount[silver] before and after the call.If the
		current count is not 1 less than previous, then print
		'choice2 silver not 1 less than previous' */
	if (backup.supplyCount[silver] - 1 != G.supplyCount[silver])
	{
		printf("Error Mine: choice2 silver is not -1 previous supplyCount.\n\n");
	}

	printf("*** END Bug1 Unit Test ***\n\n");
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
