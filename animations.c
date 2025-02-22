#include "animations.h"
#include "card.h"
#include <stdio.h>
#include <unistd.h>
#include "hearts.h"
#include <sys/ioctl.h>
#include <string.h>

#define RED "\033[31m"
#define RESET "\033[0m"
#define WIN_HEIGHT 66
#define WIN_WIDTH 212
#define CARD_HEIGHT 11
#define CARD_WIDTH 14
#define PADDING 3

#define ROW1  "╭╥╥─╥╥──╥╥─╥╥╮"
#define ROW2  "│╚═╗╚║╗╔║╝╔═╝│"
#define ROW3  "│╔║╝╔═╝╚═╗╚║╗│"
#define ROW4  "│╚═╗╚║╗╔║╝╔═╝│"
#define ROW5  "│╔║╝╔║╣╠║╗╚║╗│"
#define ROW6  "╞║╣ ╠═╣╠═╣ ╠║╡"
#define ROW7  "│╚║╗╚║╣╠║╝╔║╝│"
#define ROW8  "│╔═╝╔║╝╚║╗╚═╗│"
#define ROW9  "│╚║╗╚═╗╔═╝╔║╝│"
#define ROW10 "│╔═╝╔║╝╚║╗╚═╗│"
#define ROW11 "╰╨╨─╨╨──╨╨─╨╨╯"

const char *ROWS[] = {
    ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7, ROW8, ROW9, ROW10, ROW11
};

static void vertSpace(int rows)
{
    for (int i = 0; i < rows; i++) {
        printf("\n");
    }
}

static void horSpace(int cols)
{
    for (int i = 0; i < cols; i++) {
        printf(" ");
    }
}

static void initialScreen()
{
    vertSpace((WIN_HEIGHT - CARD_HEIGHT) / 2);
    for (int i = 0; i < CARD_HEIGHT; i++) {
        horSpace(WIN_WIDTH / 2 - CARD_WIDTH);
        printf("%s\n", ROWS[i]);
    }
}

static char *lastN(const char *s, int n)
{
    size_t len = strlen(s);
    if (strcmp(s, ROW6) == 0) {
        if (n > 3) {
            len += 2;
        }
        if (n > 10) {
            len += 2;
        }
    }
    return (char *)(s + len - 3 * n);
}

static void printFirstN(const char *s, int n)
{
    const char *curr = s;
    int len, i = 0;
    unsigned char c;
    while (*curr && i < n) {
        c = (unsigned char)*curr;
        if ((c & 0x80) == 0) {
            len = 1;
        } else if ((c & 0xE0) == 0xC0) {
            len = 2;
        } else if ((c & 0xF0) == 0xE0) {
            len = 3;
        } else if ((c & 0xF8) == 0xF0) {
            len = 4;
        } else {
            break;
        }
        if (strlen(curr) < len) {
            break;
        }
        printf("%.*s", len, curr);
        curr += len;
        i++;
    }
}

static void dealP1()
{
    for (int i = 0; i < (WIN_WIDTH - CARD_WIDTH) / 4; i++) {
        vertSpace((WIN_HEIGHT - CARD_HEIGHT) / 2);
        for (int j = 0; j < CARD_HEIGHT; j++) {
            horSpace(WIN_WIDTH / 2 - CARD_WIDTH);
            if (i < CARD_WIDTH) {
                printf("%s%s\n", ROWS[j], lastN(ROWS[j], i));
            } else {
                printf("%s", ROWS[j]);
                horSpace(i - CARD_WIDTH);
                printf("%s\n", ROWS[j]);
            }
        }
        usleep(10000);
        if (i < (WIN_WIDTH - CARD_WIDTH) / 4 - 1) {
            clearPreviousLines(WIN_HEIGHT);
        }
    }
}

static void printP2Card()
{
    vertSpace(PADDING + 1);
    for (int j = 0; j < CARD_HEIGHT; j++) {
        horSpace(WIN_WIDTH / 2 - CARD_WIDTH);
        printf("%s\n", ROWS[j]);
    }
    vertSpace((WIN_HEIGHT - CARD_HEIGHT) / 2 - PADDING - CARD_HEIGHT - 1);
}

static void dealP1Again()
{
    printP2Card();
    
    for (int i = 0; i < (WIN_WIDTH - CARD_WIDTH) / 4; i++) {
        for (int j = 0; j < CARD_HEIGHT; j++) {
            horSpace((WIN_WIDTH - 2 * CARD_WIDTH) / 4 - PADDING);
            printf("%s", ROWS[i]);
            horSpace((WIN_WIDTH - CARD_WIDTH) / 4 - CARD_WIDTH);
            printf("%s", ROWS[i]);
            if (i < CARD_WIDTH) {
                printf("%s", lastN(ROWS[j], i));
            }
        }
    }
}

void twoCards()
{   
    for (int j = 0; j < CARD_HEIGHT; j++) {
        horSpace(WIN_WIDTH / 2 - CARD_WIDTH);
        printf("%s", ROWS[j]);
        horSpace((WIN_WIDTH - CARD_WIDTH) / 4 - CARD_WIDTH);
        printf("%s\n", ROWS[j]);
    }
}

static void dealP2()
{
    for (int i = 0; i < (WIN_HEIGHT - CARD_HEIGHT) / 2 - PADDING; i++) {
        vertSpace((WIN_HEIGHT - CARD_HEIGHT) / 2 - i);
        if (i < CARD_HEIGHT) {
            for (int j = 0; j < i; j++) {
                horSpace(WIN_WIDTH / 2 - CARD_WIDTH);
                printf("%s\n", ROWS[j]);
            }
        } else {
            for (int j = 0; j < CARD_HEIGHT; j++) {
                horSpace(WIN_WIDTH / 2 - CARD_WIDTH);
                printf("%s\n", ROWS[j]);
            }
            vertSpace(i - CARD_HEIGHT);
        }
        twoCards();
        usleep(20000);
        if (i < (WIN_HEIGHT - CARD_HEIGHT) / 2 - PADDING - 1) {
            clearPreviousLines(WIN_HEIGHT);
        }
    }
}

static void dealP3()
{
    for (int i = 0; i < (WIN_WIDTH - CARD_WIDTH) / 4; i++) {
        printP2Card();
        for (int j = 0; j < CARD_HEIGHT; j++) {
            horSpace((WIN_WIDTH + CARD_WIDTH) / 4 - i);
            if (i < CARD_WIDTH) {
                horSpace((WIN_WIDTH - CARD_WIDTH) / 4 - CARD_WIDTH);
                printFirstN(ROWS[j], i + 1);
                printf("%s", ROWS[j]);
                horSpace((WIN_WIDTH - CARD_WIDTH) / 4 - CARD_WIDTH);
                printf("%s\n", ROWS[j]);
            } else {
                horSpace((WIN_WIDTH - CARD_WIDTH) / 4 - CARD_WIDTH);
                printf("%s", ROWS[j]);
                horSpace(i + 1 - CARD_WIDTH);
                printf("%s", ROWS[j]);
                horSpace((WIN_WIDTH - CARD_WIDTH) / 4 - CARD_WIDTH);
                printf("%s\n", ROWS[j]);
            }
        }
        usleep(10000);
        if (i < (WIN_WIDTH - CARD_WIDTH) / 4 - 1) {
            clearPreviousLines(WIN_HEIGHT);
        }
    }
}

static void threeCard()
{
    printP2Card();
    for (int i = 0; i < CARD_HEIGHT; i++) {
        horSpace((WIN_WIDTH - 2 * CARD_WIDTH) / 4 - PADDING);
        printf("%s", ROWS[i]);
        horSpace((WIN_WIDTH - CARD_WIDTH) / 4 - CARD_WIDTH);
        printf("%s", ROWS[i]);
        horSpace((WIN_WIDTH - CARD_WIDTH) / 4 - CARD_WIDTH);
        printf("%s\n", ROWS[i]);
    }
}

static void dealP4()
{
    for (int i = 0; i < (WIN_HEIGHT - CARD_HEIGHT) / 2 - PADDING; i++) {
        threeCard();
        if (i < CARD_HEIGHT) {
            for (int j = CARD_HEIGHT - i - 1; j < CARD_HEIGHT; j++) {
                horSpace(WIN_WIDTH / 2 - CARD_WIDTH);
                printf("%s\n", ROWS[j]);
            }
        } else {
            vertSpace(i - CARD_HEIGHT + 1);
            for (int j = 0; j < CARD_HEIGHT; j++) {
                horSpace(WIN_WIDTH / 2 - CARD_WIDTH);
                printf("%s\n", ROWS[j]);
            }
        }
        usleep(20000);
        if (i < (WIN_HEIGHT - CARD_HEIGHT) / 2 - PADDING - 1) {
            clearPreviousLines(WIN_HEIGHT);
        }
    }
}

void dealAnim()
{
    clearPreviousLines(WIN_HEIGHT);
    dealP1();
    clearPreviousLines(WIN_HEIGHT);
    dealP2();
    clearPreviousLines(WIN_HEIGHT);
    dealP3();
    clearPreviousLines(WIN_HEIGHT);
    dealP4();
    clearPreviousLines(WIN_HEIGHT);
}