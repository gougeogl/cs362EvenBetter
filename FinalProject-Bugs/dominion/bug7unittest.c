/*************************************************
* FileName: bug7unittest.c
* Author:	Glen Gougeon
* Class:	CS362 Software Engineering II
* Created:	11-30-2019
* Last Mod:	11-30-2019
*
* Assignement: Final Project Part B/
*
* Description:	Unit Test for Bug 7:
*				TributeRevealedCards Buffer-OverRun
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
	printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	printf("*** START Bug7 Unit Test: TributeRevealedCards Buffer-OverRun ***\n");
	printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
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
	int newHandSize = 1;
	int tribute_index = 0; // handPos tribute .. not really used
	int idxOfChoice1 = 0; // choice1 tribute
	int blank = -1;
	int coinBonus = 0;

	// initialize and set currentPlayer
	initTestGame(numPlayers, kingdomCards, seed, &G);
	currentPlayer = G.whoseTurn;

	/* MMMMMMMMM SET UP CURRENT PLAYER *** MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

	// empty every deck currentPlayer has
	emptyDeck(currentPlayer, &G);
	emptyPlayedCards(&G); // req'd. b/c 'discardCard' is broken !!
	emptyDiscard(currentPlayer, &G);
	emptyHand(currentPlayer, &G);

	// set up new hand size after empty
	setNewHandCount(currentPlayer, &G, newHandSize);

	// place tribute card in hand
	setAtHandPos(currentPlayer, &G, tribute, tribute_index);

	/* MMMMMMMMM SET UP NEXT PLAYER *** MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
	int nextPlayer = currentPlayer + 1;

	// empty every deck currentPlayer has
	emptyDeck(nextPlayer, &G);
	emptyPlayedCards(&G); // req'd. b/c 'discardCard' is broken !!
	emptyDiscard(nextPlayer, &G);
	emptyHand(nextPlayer, &G);

	// ensure only 2 golds are in next player's hand
	G.supplyCount[gold] = 0;

	// place 2 cards in nextPlayer's deck then increment
	int cardInDeck = gold;

	G.deck[nextPlayer][G.deckCount[nextPlayer]] = cardInDeck;
	G.deckCount[nextPlayer]++;
	G.deck[nextPlayer][G.deckCount[nextPlayer]] = cardInDeck;
	G.deckCount[nextPlayer]++;

	/* BACK UP STATE BEFORE CALL */
	memset(&backup, '\0', sizeof(backup));
	backup = G;

	/* CALL TO TRIBUTE <-----------------------------------------------------*/
	cardEffect(tribute, idxOfChoice1, blank, blank, &G, tribute_index, &coinBonus);

	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
	/* ** ASSERTS SECTION ** */
	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
	char name[MAX_STRING_LENGTH];
	char nombre[MAX_STRING_LENGTH];

	/*Assert that the top 2 cards from the next player's previous deck
	are exactly the ones found in their discard (playedCards) after the use of tribute.*/
	/*if (backup.deck[nextPlayer][backup.deckCount[nextPlayer] - 1] != G.playedCards[G.playedCardCount - 1])
	{
		if (backup.deck[nextPlayer][backup.deckCount[nextPlayer] - 2] != G.playedCards[G.playedCardCount - 2])
		{			
			memset(name, '\0', sizeof name);
			cardNumToName(G.playedCards[G.playedCards[G.playedCardCount - 1], name);
		
			memset(nombre, '\0', sizeof name);
			cardNumToName(G.playedCards[G.playedCards[G.playedCardCount - 2], nombre);

			printf("Tribute Error: previous top 2 deck cards next player..\n");
			printf("               not found in next player's discard.\n");
			printf("               Found Location: playedCards\n");
			printf("  G.playedCards[%d]: %s\n", G.playedCardCount - 1, name);
			printf("  G.playedCards[%d]: %s\n", G.playedCardCount - 2, nombre);

		}
	}*/

	if (backup.deck[nextPlayer][backup.deckCount[nextPlayer] - 1] != G.discard[nextPlayer][G.discardCount[nextPlayer] - 1])
	{
		if (backup.deck[nextPlayer][backup.deckCount[nextPlayer] - 2] != G.discard[nextPlayer][G.discardCount[nextPlayer] - 2])
		{
			memset(name, '\0', sizeof name);
			cardNumToName(G.playedCards[G.playedCards[G.playedCardCount - 1], name);

			memset(nombre, '\0', sizeof nombre);
			cardNumToName(G.playedCards[G.playedCards[G.playedCardCount - 2], nombre);

			printf("Tribute Error: previous top 2 deck cards next player..\n");
			printf("               not found in next player's discard.\n");
			printf("               Found Location: playedCards\n");
			printf("               G.playedCards[%d]: %s\n", G.playedCardCount - 1, name);
			printf("               G.playedCards[%d]: %s\n\n", G.playedCardCount - 2, nombre);

			memset(name, '\0', sizeof name);
			cardNumToName(G.discard[nextPlayer][G.discardCount[nextPlayer] - 1], name);

			memset(nombre, '\0', sizeof nombre);
			cardNumToName(G.discard[nextPlayer][G.discardCount[nextPlayer] - 1], nombre);

			printf("                Discard top 2:")
			printf("                G.discard[%d]: %s\n", G.discardCount[nextPlayer] - 1, name);
			printf("                G.discard[%d]: %s\n\n", G.discardCount[nextPlayer] - 2, nombre);
		}
	}

	/*Assert that the counts of the next player's discard and deck counts
	are correct, such that their deckCount is 2 less than previously, and
	their discardCount is 2 greater than previously.*/

	/*Assert if one of the top 2 cards in the next player's discard
	are a coin, that the current player's coin is exactly 2 greater than previously.*/

	/*Assert if one of the top 2 cards in the next player's discard
	are a victory card, that the current player's handCount is 2 greater than previously.*/

	/*Assert that if the current player's coin went up by 2 from previous, that 
	the next player's top 2 discarded cards are a coin, otherwise print a message*/

	// This will catch the error.
	/*Assert that if the current player's numActions went up by 2 from previous, that 
	the next player's top 2 discarded cards are an action card, otherwise print a message*/

	/*Assert that if the current player's hand count went up by 2 from previous, that 
	the next player's top 2 discarded cards are a victory card.
		If so compare the hands to see that the contents of the current player's hand
		is no the same as before.Use a for loop for i in G.handCount[currentPlayer],
		and compare contents.Any difference should break out of loop, set a print flag,
		and print the error.*/

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
