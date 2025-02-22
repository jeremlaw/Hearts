#include "deck.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define SPADE "\u2660"
#define CLUB "\u2663"
#define HEART "\u2665"
#define DIAMOND "\u2666"

#define RED "\033[31m"
#define BLACK "\033[30m"
#define RESET "\033[0m"

struct Deck {
    Card cards[52];
    Card *top;
    int topIndex;
};

static void initSuit(Deck deck, char *suit, int offset)
{
    deck->cards[offset] = newCard("A", suit);
    char *val;
    for (int i = 1; i < 9; i++) {
        val = (char[]){ (char)(i + '1'), '\0' };
        deck->cards[offset + i] = newCard(val, suit);
    }
    deck->cards[offset + 9] = newCard("10", suit);
    deck->cards[offset + 10] = newCard("J", suit);
    deck->cards[offset + 11] = newCard("Q", suit);
    deck->cards[offset + 12] = newCard("K", suit);
}

Deck newDeck()
{
    Deck deck = malloc(sizeof(struct Deck));
    initSuit(deck, SPADE, 0);
    initSuit(deck, HEART, 13);
    initSuit(deck, CLUB, 26);
    initSuit(deck, DIAMOND, 39);
    deck->top = &(deck->cards[0]);
    deck->topIndex = 0;
    return deck;
}

void destroyDeck(Deck deck)
{
    Card *cards = deck->cards;
    for (int i = 0; i < 52; i++) {
        freeCard(cards[i]);
    }
    free(deck);
}

void shuffle(Deck deck)
{
    srand(time(NULL));
    for (int i = 51; i > 0; i--) {
        int j = rand() % 52;
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
    deck->top = &(deck->cards[0]);
}

Card draw(Deck deck)
{
    deck->topIndex++;
    return *(deck->top++);
}

void printDeck(Deck deck)
{
    Card *cards = deck->cards;
    for (int i = 0; i < 52; i++) {
        displayCard(cards[i]);
    }
}

void topNCards(Deck deck, int n)
{
    int topIndex = deck->topIndex;
    printNCards(deck->cards, topIndex, topIndex + n);
}
