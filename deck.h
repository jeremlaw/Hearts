/* 
 * This module defines the Deck object, a deck of playing cards,
 * and provides functions for shuffling, dealing, displaying,
 * and drawing from the deck.
 */

#ifndef DECK
#define DECK

#include "card.h"

typedef struct Deck *Deck;

Deck newDeck();
void destroyDeck(Deck deck);
void shuffle(Deck deck);

Card draw(Deck deck);

void printDeck(Deck deck);
void topNCards(Deck deck, int n);

#endif