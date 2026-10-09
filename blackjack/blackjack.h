#include "cards.h"

#ifndef BLACKJACK_H
#define BLACKJACK_H

#define CARD_LIMIT 15
#define PLAYER_LIMIT 4

struct player {
    char name[50];
    int score;
    struct Card hand[CARD_LIMIT];
    int hand_size;
    int npc; // 0 for player, 1 for dealer
};

void init_players(struct player *p);
void turn(struct player *p);
void dturn(struct player *dealer);
void npc_turn(struct player *p);
void score_calc(struct player *p);

#endif