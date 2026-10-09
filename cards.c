#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cards.h"

struct Card cards[CARDS_PIECE];

void init_cards() {
    for (int i = 0; i < CARDS_PIECE; i++) {
        cards[i].number = i;
        cards[i].rank = (i % 13) + 1;
        cards[i].suit = i / 13;
        cards[i].is_used = NOT_USED;
    }
}

struct Card draw_card() {
    int random_index;
    int unused_count = 0;

    for (int i = 0; i < CARDS_PIECE; i++) {
        if (cards[i].is_used == NOT_USED) {
            unused_count++;
        }
    }
    if (unused_count == 0) {
        fprintf(stderr, "No more cards to draw.\n");
        exit(1);
    }
    do {
        random_index = rand() % CARDS_PIECE;
    } while (cards[random_index].is_used == USED);

    cards[random_index].is_used = USED;

    return cards[random_index];
}

void show_card(int card_number) {
    const char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    const char *ranks[] = {
        "Invalid", "Ace", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "Jack", "Queen", "King"
    };
    struct Card card = cards[card_number];

    if (card.rank < 1 || card.rank > 13 || card.suit < 0 || card.suit > 3) {
        printf("Invalid card\n");
        return;
    }

    printf("%s of %s\n", ranks[card.rank], suits[card.suit]);
}