/* 
 * This module defines the Card object, a playing card, and
 * provides functions for displaying and using the Card.
 */

#ifndef CARD
#define CARD

typedef struct Card {
    char *val;
    char *suit;
    char *color;
} *Card;

Card newCard(char *num, char *suit);
void freeCard(Card card);
void displayCard(Card card);
void printNCards(Card *cards, int start, int end);
char getSuit(Card card);
int getVal(Card card, int aceHigh);

void testCard(Card card);

#endif
