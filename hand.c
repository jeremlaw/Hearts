#include "hand.h"
#include "deck.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hearts.h"

#define SPADE "\u2660"
#define CLUB "\u2663"
#define HEART "\u2665"
#define DIAMOND "\u2666"

#define RED "\033[31m"
#define BLACK "\033[30m"
#define RESET "\033[0m"

struct Hand {
    int capacity;
    int size;
    Card *cards;
};

Hand newHand(int size)
{
    if (size < 1) {
        printf("Invalid size\n");
        return NULL;
    }
    Hand hand = malloc(sizeof(struct Hand));
    hand->size = 0;
    hand->capacity = size;
    hand->cards = malloc(size * sizeof(struct Card));
    return hand;
}

void freeHand(Hand hand)
{
    int size = hand->size;
    Card *cards = hand->cards;
    for (int i = 0; i < size; i++) {
        if (cards[i] != NULL) {
            freeCard(cards[i]);
        }
    }
}

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

void showHand(Hand hand)
{
    printNCards(hand->cards, 0, hand->size);
}

void showCard(Hand hand, int index)
{
    if (index >= hand->size || index < 0) {
        printf("Invalid card\n");
        return;
    }
    displayCard(hand->cards[index]);
}

/* unused */
static int printUD(char *val)
{
    char c = val[0];
    int spaces = 1;

    switch (c) {
        case 'A':
            printf("∀");
            break;
        case 'J':
            printf("ᒋ");
            break;
        case '9':
            printf("6");
            break;
        case '6':
            printf("9");
            break;
        case '8':
            printf("8");
            break;
        case '3':
            printf("Ɛ");
            break;
        case '7':
            printf("ㄥ");
            spaces = 0;
            break;
        case 'Q':
            printf("Ό");
            break;
        case '1':
            printf("0Ɩ");
            spaces = 0;
            break;
        default:
            printf("%s", val);
            break;
    }
    return spaces;
}

static void printUDSuit(char *suit)
{
    if (strcmp(suit, SPADE) == 0) {
        printf(HEART);
    } else if (strcmp(suit, HEART) == 0) {
        printf(SPADE);
    } else {
        printf("%s", suit);
    }
}

void highlightCard(Hand hand, int index)
{
    Card *cards = hand->cards;
    Card selected = cards[index];

    /* top edges (before highlight) */
    for (int i = 0; i < index; i++) {
        printf("╭───");
    }
    printf("%s╭────────────╮%s", RED, RESET);

    int size = hand->size;
    Card last = cards[size - 1];

    /* top edges (after highlight) */
    for (int i = index; i < size - 1; i++) {
        printf("╭───");
    }
    if (index < hand->size - 1) {
        printf("─────────╮");
    }
    printf("\n");

    /* values (top left corners, before highlight) */
    for (int i = 0; i < index; i++) {
        Card c = cards[i];
        char *spacing = strlen(c->val) == 1 ? " " : "";
        printf("│%s%s%s%s ", c->color, spacing, c->val, RESET);
    }
    printf("%s│%s", RED, RESET);
    char *spacing = strlen(selected->val) == 1 ? " " : "";
    printf("%s%s%s%s", selected->color, spacing, selected->val, RESET);
    printf("          %s│%s", RED, RESET);

    /* values (top left corners, after highlight) */
    for (int i = index + 1; i < size; i++) {
        Card c = cards[i];
        char *spacing = strlen(c->val) == 1 ? " " : "";
        printf("│%s%s%s%s ", c->color, spacing, c->val, RESET);
    }

    if (index < hand->size - 1) {
        printf("         │");
    }
    printf("\n");


    for (int i = 0; i < index; i++) {
        Card c = cards[i];
        printf("│%s %s %s", c->color, c->suit, RESET);
    }
    printf("%s│%s %s%s          │%s", RED, selected->color, selected->suit, RED, RESET);

    for (int i = index + 1; i < size - 1; i++) {
        Card c = cards[i];
        printf("│%s %s%s ", c->color, c->suit, RESET);
    }

    if (index < hand->size - 1) {
        printf("│%s %s%s          │", last->color, last->suit, RESET);
    }
    printf("\n");

    /* vertical edges */
    for (int i = 0; i < 5; i++) {
        for (int i = 0; i < index; i++) {
            printf("│   ");
        }
        printf("%s│            │%s", RED, RESET);

        for (int i = index + 1; i < size - 1; i++) {
            printf("│   ");
        }
        
        if (index < hand->size - 1) {
            printf("│            │");
        }
        printf("\n");
        
    }
    
    for (int i = 0; i < index; i++) {
        printf("│   ");
    }
    
    /* value and suit (bottom right corner) */
    printf("%s│          %s", RED, selected->color);
    printUDSuit(selected->suit);
    printf("%s │%s", RED, RESET);

    for (int i = index + 1; i < size - 1; i++) {
        printf("│   ");
    }

    if (index < hand->size - 1) {
        printf("│          %s", last->color);
        printUDSuit(last->suit);
        printf("%s │", RESET);
    }
    printf("\n");

    for (int i = 0; i < index; i++) {
        printf("│   ");
    }

    spacing = strlen(selected->val) == 1 ? " " : "";
    printf("%s│          %s%s%s%s│%s", RED, selected->color, selected->val, spacing, RED, RESET);


    for (int i = index + 1; i < size - 1; i++) {
        printf("│   ");
    }
    if (index < hand->size - 1) {
        spacing = strlen(last->val) == 1 ? " " : "";
        printf("│          %s%s%s%s│", last->color, last->val, spacing, RESET);
    }
    printf("\n");

    /* bottom edges */
    for (int i = 0; i < index; i++) {
        printf("╰───");
    }
    printf("%s╰────────────╯%s", RED, RESET);

    /* bottom edges */
    for (int i = index + 1; i < size - 1; i++) {
        printf("╰───");
    }

    if (index < hand->size - 1) {
        printf("╰────────────╯");
    }
    printf("\n");
}

int handSize(Hand hand)
{
    return hand->size;
}

Card cardAt(Hand hand, int index)
{
    if (index >= hand->size || index < 0) {
        printf("Invalid card\n");
        return NULL;
    }
    return hand->cards[index];
}

Card removeCard(Hand hand, int index)
{
    if (index >= hand->size || index < 0) {
        printf("Invalid card\n");
        return NULL;
    }

    Card card = hand->cards[index];

    for (int i = index; i < hand->size; i++) {
        hand->cards[i] = hand->cards[i + 1];
    }

    hand->size--;
    return card;
}

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

static int compCards(const void *card1, const void *card2)
{
    Card c1 = *(Card *)card1;
    Card c2 = *(Card *)card2;

    return (cardIndex(c1) - cardIndex(c2));
}

void sortHand(Hand hand)
{
    qsort(hand->cards, hand->size, sizeof(Card), compCards);
}