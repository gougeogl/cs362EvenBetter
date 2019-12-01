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

	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine, gardens, remodel, smithy };

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

	printDeck(nextPlayer, &G);
	printDiscard(nextPlayer, &G);

	/* CALL TO TRIBUTE <-----------------------------------------------------*/
	cardEffect(tribute, idxOfChoice1, blank, blank, &G, tribute_index, &coinBonus);

	printDeck(nextPlayer, &G);
	printDiscard(nextPlayer, &G);
	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
	/* ** ASSERTS SECTION ** */
	/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
	char name[MAX_STRING_LENGTH];
	char nombre[MAX_STRING_LENGTH];

	/*Assert that the top 2 cards from the next player's previous deck
	are exactly the ones found in their discard (playedCards) after the use of tribute.*/
	if (backup.deck[nextPlayer][backup.deckCount[nextPlayer] - 1] != G.discard[nextPlayer][G.discardCount[nextPlayer] - 1])
	{
		if (backup.deck[nextPlayer][backup.deckCount[nextPlayer] - 2] != G.discard[nextPlayer][G.discardCount[nextPlayer] - 2])
		{
			memset(name, '\0', sizeof name);
			cardNumToName(G.discard[nextPlayer][G.discardCount[nextPlayer] - 1], name);

			memset(nombre, '\0', sizeof nombre);
			cardNumToName(G.discard[nextPlayer][G.discardCount[nextPlayer] - 1], nombre);

			printf("Tribute Error: previous top 2 deck cards next player..\n");
			printf("               not found in next player's discard.\n\n");
			printf("               Discard top 2:\n");
			printf("               G.discard[%d]: %s\n", G.discardCount[nextPlayer] - 1, name);
			printf("               G.discard[%d]: %s\n\n", G.discardCount[nextPlayer] - 2, nombre);
		}
	}

	/*Assert that the counts of the next player's discard and deck counts
	are correct, such that their deckCount is 2 less than previously, and
	their discardCount is 2 greater than previously.*/
	if (backup.deckCount[nextPlayer] - 2 != G.deckCount[nextPlayer])
	{
		if (backup.discardCount[nextPlayer] + 2 != G.discardCount[nextPlayer])
		{
			printf("Tribute Error: next player's deckCount not -2 previous.\n");
			printf("             : next player's discardCount not +2 previous.\n");

		}
	}	  

	/*Assert if one of the top 2 cards in the next player's discard
	are a coin, that the current player's coin is exactly 2 greater than previously.
	for each treasure in discard*/
	int totalCoin = 0;
	if (G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == copper ||
		G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == silver ||
		G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == gold) {

		// 1 treasure found
		totalCoin += 2;

			if (G.discard[nextPlayer][G.discardCount[nextPlayer] - 2] == copper ||
				G.discard[nextPlayer][G.discardCount[nextPlayer] - 2] == silver ||
				G.discard[nextPlayer][G.discardCount[nextPlayer] - 2] == gold){

				// 2nd treasure found
				totalCoin += 2;
			}
	}

	if (totalCoin == 2)
	{
		if (backup.coins + 2 != G.coins)
		{
			printf("Tribute Error: You had 1 treasure in discard, but didn't gain 2 coin.\n");
		}
	}
	else if (totalCoin == 4)
	{
		if (backup.coins + 4 != G.coins)
		{
			printf("Tribute Error: You had 2 treasures in discard, but didn't gain 4 coin.\n");
		}
	}
	/*Assert that if the current player's coin went up by 2 from previous, that
	the next player's top 2 discarded cards are a coin, otherwise print a message*/
	else if (totalCoin == 0 && (backup.coins != G.coins))
	{
		printf("Tribute Error: You gained coin, but didn't find a treasure in discard.\n");
	}

	/*Assert if one of the top 2 cards in the next player's discard
	are a victory card, that the current player's handCount is 2 greater than previously.*/
	int totalVictory = 0;
	if (G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == estate ||
		G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == gardens) {

		// 1 treasure found
		totalVictory += 2;

		if (G.discard[nextPlayer][G.discardCount[nextPlayer] - 2] == estate ||
			G.discard[nextPlayer][G.discardCount[nextPlayer] - 2] == gardens) {

			// 2nd treasure found
			totalVictory += 2;
		}
	}

	if (totalVictory == 2)
	{
		if (backup.handCount[nextPlayer] + 2 != G.handCount[nextPlayer])
		{
			printf("Tribute Error: You had 1 Victory card in discard, but didn't gain 2 card.\n");
		}
	}
	else if (totalVictory == 4)
	{
		if (backup.handCount[nextPlayer] + 4 != G.handCount[nextPlayer])
		{
			printf("Tribute Error: You had 2 Victory card in discard, but didn't gain 4 cards.\n");
		}
	}
	/*Assert that if the current player's hand count went up by 2 from previous, that
	the next player's top 2 discarded cards are a victory card.*/
	else if (totalVictory == 0 && (backup.handCount[nextPlayer]!= G.handCount[nextPlayer]))
	{
		printf("Tribute Error: You gained cards in hand, but didn't find a Victory card in discard.\n");
	}

	// This will catch the error.
	/*Assert that if the current player's numActions went up by 2 from previous, that 
	the next player's top 2 discarded cards are an action card, otherwise print a message*/

	adventurer, ambassador, baron, estate, tribute, minion, mine, gardens, remodel, smithy
	int totalActions = 0;
	if (G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == adventurer ||
		G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == baron ||
		G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == tribute ||
		G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == minion ||
		G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == mine ||
		G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == remodel ||
		G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == smithy ){

		// 1 action found
		totalActions += 2;

		if (G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == adventurer ||
			G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == baron ||
			G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == tribute ||
			G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == minion ||
			G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == mine ||
			G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == remodel ||
			G.discard[nextPlayer][G.discardCount[nextPlayer] - 1] == smithy) {

			// 2nd actions found
			totalActions += 4;
		}
	}

	if (totalActions == 2)
	{
		if (backup.numActions + 2 != G.numActions)
		{
			printf("Tribute Error: You had 1 action card in discard, but didn't gain 2 actions.\n");
		}
	}
	else if (totalActions == 4)
	{
		if (backup.numActions + 4 != G.numActions)
		{
			printf("Tribute Error: You had 2 action cards in discard, but didn't gain 4 actions.\n");
		}
	}
	else if (totalActions == 0 && (backup.numActions != G.numActions))
	{
		printf("Tribute Error: You gained actions, but didn't find any action cards in discard.\n");
	}

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
