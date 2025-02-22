/* 
 * This file contains the program for playing Hearts.
 */

#include "deck.h"
#include "hand.h"
#include "hearts.h"
#include "animations.h"
#include <stdio.h>

int main()
{
    //dealAnim();
    
    Player *players = setPlayers();

    Deck deck = newDeck();
    shuffle(deck);
    shuffle(deck);

    int startingPlayer = deal(players, deck);

    playRound(players, startingPlayer);

    freePlayers(players);

    return 0;
}