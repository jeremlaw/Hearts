/* 
 * This module defines the Hand object, a collection of playing cards,
 * and provides various functions for a user interfacing with a Hand.
 */

#ifndef HAND
#define HAND

#include "card.h"

typedef struct Hand *Hand;

Hand newHand(int size);
void freeHand(Hand hand);
void pick(Hand hand, Card card);
void showHand(Hand hand);
void highlightCard(Hand hand, int index);
int handSize(Hand hand);
Card cardAt(Hand hand, int index);
Card removeCard(Hand hand, int index);
void sortHand(Hand hand);

#endif