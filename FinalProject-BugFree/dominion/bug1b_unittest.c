/*************************************************
* FileName: bug1b_unittest.c
* Author:	Glen Gougeon
* Class:	CS362 Software Engineering II
* Created:	11-29-2019
* Last Mod:	11-29-2019
*
* Assignement: Final Project Part B/
*
* Description:	Unit Test for Bug 1b:
*				Remodel Trash failure
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
void emptyDiscard(int player, struct gameState* state);

// HAND RELATED PROTO-TYPES
void emptyHand(int player, struct gameState* dState);
void setNewHandCount(int player, struct gameState* state, int newHandSize);
void setAtHandPos(int player, struct gameState* state, int card, int handPos);

int main()
{
	printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	printf("*** START Bug1-B Unit Test: Remodel Trash Failure ***\n");
	printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");

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
	int remodel_index = 0; // handPos remodel
	int idxOfChoice1 = 1; // choice1 remodel
	int cardToGet = estate; // choice2 remodel
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

	// place remodel card in hand
	setAtHandPos(currentPlayer, &G, remodel, remodel_index);

	// The only copper in the game is in currentPlayer's hand
	G.supplyCount[copper] = 0;

	// The only silver in the game is THIS in supply
	G.supplyCount[estate] = 0;
	G.supplyCount[estate] = 1;

	setAtHandPos(currentPlayer, &G, copper, idxOfChoice1);
	updateCoins(currentPlayer, &G, coinBonus);

	/* BACK UP STATE BEFORE CALL */
	memset(&backup, '\0', sizeof(backup));
	backup = G;

	/* CALL TO REMODEL <-----------------------------------------------------*/
	cardEffect(remodel, idxOfChoice1, cardToGet, blank, &G, remodel_index, &coinBonus);

	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
	/* ** ASSERTS SECTION ** */
	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
	
	/* a) Assert that choice1(an index) is a different card, otherwise print that it
	   is the same. */
	if (backup.hand[currentPlayer][idxOfChoice1] == backup.hand[currentPlayer][idxOfChoice1])
	{
		printf("Error Remodel: choice1 is still the same but shouldn't be.\n\n");
	}

	/* b) Assert if top of previous discard was not the same as choice1, and
	   choice1 is found at the top of discard, print 'choice1 discarded
	   not trashed'. */

	// b) IF DISCARD WAS REFACTORED - VERSION COMMENTED OUT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (backup.discard[currentPlayer][backup.discardCount[currentPlayer] -1] != copper)
	{
		if (G.discard[currentPlayer][G.discardCount[currentPlayer] - 1] == copper)
		{
			printf("Error Remodel: choice1 found at top of discard, but should be at top of trashPile.\n\n");
		}
	}
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (backup.playedCards[backup.playedCardCount - 1] != copper)
	{
		if (G.playedCards[G.playedCardCount - 1] == copper)
		{
			//printf("Error 'discardCard': should be discard .. but is playedCards array\n");
			printf("Error Remodel: choice1 found in playedCards, but should be at top of trashPile.\n\n");
		}
	} */

	/* Assert if top of previous discard was the same as choice1, and the
	   top 2 cards in discard are both choice1, print 'choice1 discarded
	   not trashed'. */

	/* IF DISCARD CARD WAS REFACTORED - VERSION COMMENTED OUT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	if (backup.discard[currentPlayer][backup.discardCount[currentPlayer] -1] == copper)
	{
		if ((G.discard[currentPlayer][G.discardCount[currentPlayer] - 1] == copper) &&
			(G.discard[currentPlayer][G.discardCount[currentPlayer] - 2] == copper))
		{
			printf("Error Remodel: choice1 discarded not trashed.\n\n");
		}
	}
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (backup.playedCards[backup.playedCardCount - 1] == copper)
	{
		if ((G.playedCards[G.playedCardCount - 1] == copper) &&
			(G.playedCards[G.playedCardCount - 2] == copper))
		{
			//printf("Error 'discardCard': should be discard .. but is playedCards array\n");
			printf("Error Remodel: choice1 found in playedCards NOT in trash.\n\n");
		}
	}*/

	/* Assert if backup.supplyCount[choice2] > 0 and choice2 was not in
   previous top of discard, and is not in current top of discard, print
   'choice2 not found in top of discard after remodel'. */

	/* IF DISCARD CARD WAS REFACTORED - VERSION COMMENTED OUT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	if (backup.supplyCount[cardToGet] > 0)
	{
		if ((backup.discard[currentPlayer][backup.discardCount[currentPlayer] - 1] != cardToGet) &&
			(G.discard[currentPlayer][G.discardCount[currentPlayer] - 1] != cardToGet))
		{
			printf("Error Remodel. choice2 card to gain not found in top of discard after remodel.\n\n");
		}
	}
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (backup.supplyCount[cardToGet] > 0)
	{
		if ((backup.playedCards[backup.playedCardCount - 1] != cardToGet) &&
			(G.playedCards[G.playedCardCount - 1] != cardToGet))
		{
			printf("Error Remodel. choice2 card to gain not found in top of discard after remodel.\n");
			printf("               *actually not in playedCards .. b/c that is where 'discardCard' places cards.\n\n");
		}
	}*/

	/* The handCount should change.
   Assert if backup.handCount[currentPlayer] == G.handCount[currentPlayer]
   print 'Error Remodel: handCount didn't change.\n'	*/
	if (backup.handCount[currentPlayer] == G.handCount[currentPlayer])
	{
		printf("Error Remodel: handCount didn't change.\n");
	}

	/* Compare the supplyCount[estate] before and after the call.If the
	   current count is not 1 less than previous, then print
	   'choice2 estate not 1 less than previous' */
	if (backup.supplyCount[estate] - 1 != G.supplyCount[estate])
	{
		printf("Error Remodel: choice2 estate is not -1 previous supplyCount.\n\n");
	}

	printf("*** END Bug1-B Unit Test ***\n\n");
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
