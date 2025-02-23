#include "hand.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPADE "\u2660"
#define CLUB "\u2663"
#define HEART "\u2665"
#define DIAMOND "\u2666"

#define RED "\033[31m"
#define BLACK "\033[30m"
#define RESET "\033[0m"

#define TOP 0
#define BOTTOM 1

#define VAL 0
#define SUIT 1

/********************************************
 * Represents a dynamic collection of Cards *
 ********************************************/
struct Hand {
    int capacity; /* Amount of memory currently allocated */
    int size;     /* Number of accessable cards */
    Card *cards;  /* Array of Card objects */
};

/*************************************************
 * Allocate space on the heap for a Hand.        *
 * Note: terminates program if allocation fails. *
 *************************************************/
Hand newHand(int size)
{
    if (size < 1) {
        printf("Invalid size\n");
        exit(EXIT_FAILURE);
    }
    Hand hand = malloc(sizeof(struct Hand));
    hand->size = 0;
    hand->capacity = size;
    hand->cards = malloc(size * sizeof(struct Card));
    return hand;
}

/**************************************************
 * Frees space allocated for a Hand.              *
 * Note: does not attempt to free a null pointer. *
 **************************************************/
void freeHand(Hand hand)
{
    if (hand == NULL) {
        printf("Error: Invalid free\n");
        return;
    }
    int size = hand->size;
    Card *cards = hand->cards;
    for (int i = 0; i < size; i++) {
        if (cards[i] != NULL) {
            freeCard(cards[i]);
        }
    }
}

/*************************************************************
 * Adds a Card to the Hand. Allocates more memory if needed. *
 * Note: terminates program if allocation fails.             *
 *************************************************************/
void pick(Hand hand, Card card)
{
    if (hand->size >= hand->capacity) {
        int newcap = hand->capacity * 2;
        Card *newcards = realloc(hand->cards, newcap * sizeof(struct Card));
        if (newcards == NULL) {
            printf("Error\n");
            exit(EXIT_FAILURE);
        }
        hand->cards = newcards;
        hand->capacity = newcap;
    }

    hand->cards[hand->size++] = newCard(card->val, card->suit);
}

/**********************************
 * Displays all cards in the Hand *
 **********************************/
void showHand(Hand hand)
{
    printNCards(hand->cards, 0, hand->size);
}

/* 
 * Helper function for highlightCard
 *
 * Prints the horizontal edges of all cards in a Hand,
 * given its size and the side (top or bottom edge).
 * Also outlines the card at the given index in red.
 */
static void printHorizEdges(int size, int index, int side)
{
    char *leftCorner = (side == TOP) ? "╭" : "╰";
    char *rightCorner = (side == TOP) ? "╮" : "╯";

    for (int i = 0; i < index; i++) {
        printf("%s───", leftCorner);
    }

    printf("%s%s────────────%s%s", RED, leftCorner, rightCorner, RESET);

    for (int i = index; i < size - 1; i++) {
        printf("%s───", leftCorner);
    }
    if (index < size - 1) {
        printf("─────────%s", rightCorner);
    }
    printf("\n");
}

/*
 * Helper function for highlightCard
 *
 * Utility function used to print the value or suit of a card
 * and the area around it. Parameters specify what to print on
 * either side (s1 and s2), the type (VAL or SUIT), edge color
 * (RED or RESET), the side (TOP or BOTTOM), and the Card.
 */
static void printValOrSuit(char *s1, char *s2, int type, char *edgeColor,
                           int side, Card c)
{
    char *spacing = (strlen(c->val) == 1 || type == SUIT) ? " " : "";
    char *symbol = (type == VAL) ? c->val : c->suit;
    char *arg1 = (side == TOP) ? spacing : symbol;
    char *arg2 = (side == TOP) ? symbol : spacing;
    
    printf("%s%s%s%s%s%s%s%s", edgeColor, s1, c->color, arg1, arg2, edgeColor,
            s2, RESET);
}

/*
 * Helper function for highlightCard
 *
 * Prints the top left corners of the specified cards, given
 * the type (VAL or SUIT), index of card to highlight in red,
 * number of cards, and an array of Cards.
 */
static void printTopLeft(int index, int numCards, int type, Card *cards)
{
    for (int i = 0; i < numCards; i++) {
        Card c = cards[i];

        printValOrSuit("│", " ", type, (i == index) ? RED : RESET, TOP, c);

        if (i == index) {
            printf("         %s│%s", RED, RESET);
        }
    }
    if (index < numCards - 1) {
        printf("         │");
    }
    printf("\n");
}

/*
 * Helper function for highlightCard
 * Utility function that prints the left edge of a card numCards times. 
 */
static void printLeftEdge(int numCards)
{
    for (int i = 0; i < numCards; i++) {
        printf("│   ");
    }
}

/* 
 * Helper function for highlightCard
 *
 * Prints the vertical edges of each card in a Hand, given the
 * number of cards and index of the card to highlight in red.
 */
static void printVerticalEdges(int index, int numCards)
{
    for (int i = 0; i < 5; i++) {
        printLeftEdge(index);
        printf("%s│            │%s", RED, RESET);
        printLeftEdge(numCards - index - 2);
        
        if (index < numCards - 1) {
            printf("│            │");
        }
        printf("\n");
    }
    printLeftEdge(index);
}

/*
 * Helper function for highlightCard
 *
 * Prints the bottom right corners of applicable cards,
 * given the type (VAL or SUIT), index of card to highlight
 * in red, number of cards, and an array of Cards.
 */
static void printBottomRight(int index, int numCards, int type, Card *cards)
{
    Card selected = cards[index];
    Card last = cards[numCards - 1];
    
    printValOrSuit("│          ", "│", type, RED, BOTTOM, selected);
    printLeftEdge(numCards - index - 2);

    if (index < numCards - 1) {
        printValOrSuit("│          ", "│", type, RESET, BOTTOM, last);
    }
    printf("\n");
}

/******************************************************
 * Prints every card in the given Hand and highlights *
 * the card at the specified index in red.            *
 ******************************************************/
void highlightCard(Hand hand, int index)
{
    Card *cards = hand->cards;
    int size = hand->size;

    printHorizEdges(size, index, TOP);
    printTopLeft(index, size, VAL, cards);
    printTopLeft(index, size, SUIT, cards);
    printVerticalEdges(index, size);
    printBottomRight(index, size, SUIT, cards);
    printLeftEdge(index);
    printBottomRight(index, size, VAL, cards);
    printHorizEdges(size, index, BOTTOM);
}

/**************************************
 * Returns the size of the given Hand *
 **************************************/
int handSize(Hand hand)
{
    return hand->size;
}

/**************************************************************
 * Returns the Card at the specified index of the given Hand. *
 * Note: terminates program if index is out of range.         *
 **************************************************************/
Card cardAt(Hand hand, int index)
{
    if (index >= hand->size || index < 0) {
        printf("Invalid read\n");
        exit(EXIT_FAILURE);
    }
    return hand->cards[index];
}

/**************************************************************
 * Removes the Card at the specified index of the given Hand. *
 * Note: terminates program if index is out of range.         *
 **************************************************************/
Card removeCard(Hand hand, int index)
{
    if (index >= hand->size || index < 0) {
        printf("Invalid read\n");
        return NULL;
    }

    Card card = hand->cards[index];

    for (int i = index; i < hand->size; i++) {
        hand->cards[i] = hand->cards[i + 1];
    }

    hand->size--;
    return card;
}

/*
 * Helper function for compCards
 * Returns the index of a Card in an ordered deck.
 */
static int cardIndex(Card card)
{
    char suit = getSuit(card);
    int val = getVal(card, 1);

    switch (suit) {
        case 'c':
            return val;
        case 'd':
            return val + 13;
        case 's':
            return val + 26;
        default:
            return val + 39;
    }
}

/* 
 * Helper function for sortHand
 * Compares the position of two cards in an ordered deck. 
 */
static int compCards(const void *card1, const void *card2)
{
    Card c1 = *(Card *)card1;
    Card c2 = *(Card *)card2;

    return (cardIndex(c1) - cardIndex(c2));
}

/************************************************************
 * Sorts the given hand, from least to greatest (ace high)  *
 * and in this order of suits: Club, Diamond, Spade, Heart. *
 ************************************************************/
void sortHand(Hand hand)
{
    qsort(hand->cards, hand->size, sizeof(Card), compCards);
}
