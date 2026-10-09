#ifndef CARDS_H
#define CARDS_H

#define CARDS_PIECE 52
#define USED 1
#define NOT_USED 0

struct Card {
    int rank; // 1-13
    int suit; // 0-3
    int number; // 0-51
    int is_used; // 0 or 1
};

extern struct Card cards[CARDS_PIECE];

void init_cards();
struct Card draw_card();
void show_card(int card_number);

#endif