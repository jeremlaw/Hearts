#include "card.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SPADE "\u2660"
#define CLUB "\u2663"
#define HEART "\u2665"
#define DIAMOND "\u2666"

#define RED "\033[31m"
#define BLACK "\033[30m"
#define RESET "\033[0m"

Card newCard(char *val, char *suit)
{
    Card card = malloc(sizeof(struct Card));
    card->val = strdup(val);
    card->suit = suit;
    if (strcmp(card->suit, HEART) == 0 || strcmp(card->suit, DIAMOND) == 0) {
        card->color = RED;
    } else {
        card->color = BLACK;
    }
    return card;
}

void freeCard(Card card)
{
    free(card->val);
    free(card);
}

void displayCard(Card card)
{
    char *val = card->val;
    char *suit = card->suit;
    char *color = card->color;

    /* adds spacing if necessary */
    char *spacing = strlen(val) == 1 ? " " : "";

    printf("╭────────────╮\n");

    /* value and suit (top left corner) */
    printf("│%s%s%s%s          │\n", color, spacing, val, RESET);
    printf("│%s %s%s          │\n", color, suit, RESET);

    for (int i = 0; i < 5; i++) {
        printf("│            │\n");
    }

    /* value and suit (bottom right corner) */
    printf("│          %s%s %s│\n", color, suit, RESET);
    printf("│          %s%s%s%s│\n", color, val, spacing, RESET);

    printf("╰────────────╯\n");
}

static void printEmptyCard()
{
    printf("╭────────────╮\n");
    for (int i = 0; i < 9; i++) {
        printf("│            │\n");
    }
    printf("╰────────────╯\n");
}

void printNCards(Card *cards, int start, int end)
{
    if (end == 0) {
        return printEmptyCard();
    }

    Card last = cards[end - 1];

    /* top edges */
    for (int i = start; i < end; i++) {
        printf("╭───");
    }
    printf("─────────╮\n");

    /* values and suit (top left corners) */
    for (int i = start; i < end; i++) {
        Card c = cards[i];
        char *spacing = strlen(c->val) == 1 ? " " : "";
        printf("│%s%s%s%s ", c->color, spacing, c->val, RESET);
    }
    printf("         │\n");

    for (int i = start; i < end - 1; i++) {
        Card c = cards[i];
        printf("│%s %s%s ", c->color, c->suit, RESET);
    }
    printf("│%s %s%s          │\n", last->color, last->suit, RESET);

    /* vertical edges */
    for (int i = 0; i < 5; i++) {
        for (int i = start + 1; i < end; i++) {
            printf("│   ");
        }
        printf("│            │\n");
    }
    
    for (int i = start; i < end - 1; i++) {
        printf("│   ");
    }
    /* value and suit (bottom right corner) */
    printf("│          %s%s%s │\n", last->color, last->suit, RESET);

    for (int i = start; i < end; i++) {
        printf("│   ");
    }
    char *spacing = strlen(cards[end - 1]->val) == 1 ? " " : "";
    printf("       %s%s%s%s│\n", last->color, last->val, spacing, RESET);

    /* bottom edges */
    for (int i = start; i < end; i++) {
        printf("╰───");
    }
    printf("─────────╯\n");
}

char getSuit(Card card)
{
    char *suit = card->suit;
    if (strcmp(suit, HEART) == 0) {
        return 'h';
    } else if (strcmp(suit, SPADE) == 0) {
        return 's';
    } else if (strcmp(suit, DIAMOND) == 0) {
        return 'd';
    }
    return 'c';
}

int getVal(Card card, int aceHigh)
{
    char num = card->val[0];

    if ('1' < num && num <= '9') {
        return num - '0';
    }

    switch(num) {
        case '1':
            return 10;
        case 'J':
            return 11;
        case 'Q':
            return 12;
        case 'K':
            return 13;
        case 'A':
            return aceHigh ? 14 : 1;
    }
    return 0;
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

void testCard(Card card)
{
    char *val = card->val;
    char *suit = card->suit;
    char *color = card->color;

    /* add spacing if necessary */
    char *spacing = strlen(val) == 1 ? " " : "";

    printf("╭────────────╮\n");

    /* value and suit (top left corner) */
    printf("│%s%s%s%s          │\n", color, spacing, val, RESET);
    printf("│%s %s%s          │\n", color, suit, RESET);

    for (int i = 0; i < 5; i++) {
        printf("│            │\n");
    }

    /* value and suit (bottom right corner) */
    printf("│          %s", color);
    printUDSuit(suit);
    printf(" %s│\n", RESET);
    printf("│          %s", color);
    if (printUD(val) == 0) {
        spacing = "";
    }
    printf("%s%s│\n", spacing, RESET);

    printf("╰────────────╯\n");
}