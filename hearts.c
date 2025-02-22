#include "hearts.h"
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include <string.h>

#define RIGHT 'C'
#define LEFT 'D'

#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"

typedef struct Player {
    Hand hand;
    Hand points;
    int score;
} *Player;


Player *setPlayers()
{
    Player *players = malloc(4 * sizeof(struct Player));

    for (int i = 0; i < 4; i++) {
        Player p = malloc(sizeof(struct Player));
        p->hand = newHand(13);
        p->points = newHand(4);
        p->score = 0;
        players[i] = p;
    }

    return players;
}

void freePlayers(Player *players)
{
    for (int i = 0; i < 4; i++) {
        Player p = players[i];
        freeHand(p->hand);
        freeHand(p->points);
        free(p);
    }
}

static bool isTwoOfClubs(Card card)
{
    return (getVal(card, 1) == 2 && getSuit(card) == 'c');
}

static bool isQueenOfSpades(Card card)
{
    return (getVal(card, 1) == 12 && getSuit(card) == 's');
}

int deal(Player *players, Deck deck)
{
    int startingPlayer = 0;
    for (int i = 0; i < 52; i++) {
        Card card = draw(deck);
        if (isTwoOfClubs(card)) {
            startingPlayer = i;
        }
        pick(players[i % 4]->hand, card);
    }
    destroyDeck(deck);

    return startingPlayer;
}

static void enableRawMode()
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

static void disableRawMode(struct termios *orig)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig);
}

void clearPreviousLines(int lines)
{
    for (int i = 0; i < lines; i++) {
        printf("\033[F");
        printf("\033[2K");
    }
    printf("\r");
}

static bool voidOfSuit(Hand hand, char suit)
{
    int size = handSize(hand);
    for (int i = 0; i < size; i++) {
        Card card = cardAt(hand, i);
        if (getSuit(card) == suit) {
            return false;
        }
    }
    return true;
}

Card selection(Hand hand, bool *introduced, char leadSuit, bool firstTrick)
{
    struct termios orig_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    enableRawMode();

    int index = 0;
    int numCards = handSize(hand);
    showHand(hand);

    selecting:
        while (1) {
            clearPreviousLines(11);
            highlightCard(hand, index);
            char c = getchar();
            if (c == '\033') {
                getchar();
                char arrow = getchar();
                switch (arrow) {
                    case RIGHT:
                        index = (index + 1) % numCards;
                        break;
                    case LEFT:
                        index = (index - 1 + numCards) % numCards;
                        break;
                }
                fflush(stdout);
            }
            else if (c == '\n') {
                Card card = cardAt(hand, index);
                char suit = getSuit(card);
                if (!(*introduced) && suit == 'h') {
                    if (leadSuit && voidOfSuit(hand, leadSuit)) {
                        *introduced = true;
                        break;
                    }
                    printf("\a");
                    goto selecting;
                }
                if (suit && suit != leadSuit && !voidOfSuit(hand, leadSuit)) {
                    printf("\a");
                    goto selecting;
                }
                if (firstTrick && !isTwoOfClubs(card)) {
                    printf("\a");
                    goto selecting;
                }
                if (isQueenOfSpades(card)) {
                        *introduced = true;
                }
                break;
            }
        }

    clearPreviousLines(12);
    disableRawMode(&orig_termios);
    return removeCard(hand, index);
}

void passing(Player *players, int order[])
{   
    for (int i = 0; i < 4; i++) {
        Player player = players[order[i]];
        printf("Choose 3 cards to pass\n");
    }
}

char turn(Hand trick, Player player, int id, bool *introduced, char leadSuit,
          bool *firstTrick)
{
    printf(HIDE_CURSOR);
    printf("Pile:\n");
    showHand(trick);
    printf("Player %d\n", id + 1);

    sortHand(player->hand);
    Card played = selection(player->hand, introduced, leadSuit, *firstTrick);
    if (*firstTrick) {
        *firstTrick = false;
    }
    pick(trick, played);

    clearPreviousLines(12);
    printf("Pile:\n");
    showHand(trick);
    printf("Next player's turn\n");

    char c = 'a';
    while (c != '\n') {
        c = getchar();
    }

    clearPreviousLines(14);
    printf(SHOW_CURSOR);
    return getSuit(played);
}

static int endTrick(Player *players, Hand trick, int leader)
{
    Card lead = cardAt(trick, 0);
    char suit = getSuit(lead);

    int vals[4] = { 0 };
    char suits[4] = { 0 };

    int points = 0;

    for (int i = 0; i < 4; i++) {
        Card card = cardAt(trick, i);
        vals[i] = getVal(card, 1);
        suits[i] = getSuit(card);
        if (suits[i] == 'h') {
            points++;
        } else if (isQueenOfSpades(card)) {
            points += 13;
        }
    }
    
    char lead_suit = suits[0];
    int high_val = vals[0];
    int winner_id = 0;

    for (int i = 1; i < 4; i++) {
        if (vals[i] >= high_val && suits[i] == lead_suit) {
            high_val = vals[0];
            winner_id = (i + leader) % 4;
        }
    }

    Player winner = players[winner_id];
    winner->score += points;
    
    for (int i = 0; i < 4; i++) {
        if (suits[i] == 'h' || (suits[i] == 's' && vals[i] == 12)) {
            pick(winner->points, cardAt(trick, i));
        }
    }

    return winner_id;
}

int playTrick(Player *players, int leader, bool *introduced, bool *firstTrick)
{
    Hand trick = newHand(4);
    int id = leader % 4;
    char leadSuit = turn(trick, players[id], id, introduced, '\0', firstTrick);
    for (int i = 1; i < 4; i++) {
        id = (leader + i) % 4;
        turn(trick, players[id], id, introduced, leadSuit, firstTrick);
    }

    return endTrick(players, trick, leader);
}

void finishGame(Player *players)
{
    int scores[4] = { 0 };
    int winners[4] = { 0 }, winner_count = 0;
    int winner_id = -1;
    int min_id = -1, min_score = 27;

    for (int i = 0; i < 4; i++) {
        int score = players[i]->score;
        scores[i] = score;

        if (score < min_score) {
            min_score = score;
            min_id = i;
            winner_count = 1;
            winners[0] = i + 1;
        } else if (score == min_score) {
            winners[winner_count++] = i + 1;
        } else if (score == 26) {
            min_score = 26;
            winner_id = i;
        }
    }

    if (min_score == 26) {
        printf("Player %d shoots the moon!\n", min_id + 1);
        return;
    } else if (winner_count > 1) {
        printf("Players ");
        for (int i = 0; i < winner_count; i++) {
            printf("%d ", winners[i]);
            if (i == winner_count - 2) {
                printf("and ");
            }
        }
        printf("win!\n\n");
    } else {
        printf("Player %d wins!\n\n", min_id + 1);
    }

    char *points = "points";
    for (int i = 0; i < 4; i++) {
        Player player = players[i];
        if (player->score == 1) {
            points = "point";
        }
        printf("Player %d scored %d %s\n", i + 1, player->score, points);
        if (handSize(player->points) > 0) {
            showHand(player->points);
        } else {
            printf("\n");
        }
    }
}

void playRound(Player *players, int startingPlayer)
{
    bool introduced = false;
    bool firstTrick = true;
    int player_id = playTrick(players, startingPlayer, &introduced, &firstTrick);
    for (int i = 1; i < 13; i++) {
        player_id = playTrick(players, player_id, &introduced, &firstTrick);
    }

    finishGame(players);
}


//test
void getCursorPosition(int *row, int *col)
{
    struct termios oldt, newt;
    char buf[32];
    int i = 0;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    write(STDOUT_FILENO, "\033[6n", 4);

    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, buf + i, 1) != 1)
            break;
        if (buf[i] == 'R')
            break;
        i++;
    }
    buf[i] = '\0';

    if (sscanf(buf, "\033[%d;%dR", row, col) != 2) {
        *row = *col = -1;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

Hand getHand(Player player)
{
    return player->hand;
}
