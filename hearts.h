/* 
 * This file contains the interface for the Hearts game.
 * All Hearts rules are implemented except for card passing,
 * which is in progress.
 */

#ifndef HEARTS
#define HEARTS

#include "deck.h"
#include "hand.h"
#include <stdbool.h>

typedef struct Player *Player;

Player *setPlayers();
void freePlayers(Player *players);
int deal(Player *players, Deck deck);
int playTrick(Player *players, int leader, bool *introduced, bool *firstTrick);
void finishGame(Player *players);
void playRound(Player *players, int startingPlayer);
void clearPreviousLines(int lines);

#endif