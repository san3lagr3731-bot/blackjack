#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "blackjack.h"

int main(void)
{
    srand(time(NULL)); // ★ここで乱数の種を設定

    int i, a, round;

    struct player p[PLAYER_LIMIT];
    struct player dealer;

    printf("write your name\n");
    for (i = 0; i < PLAYER_LIMIT; i++) {
        printf("Player%d:", i + 1);
        scanf("%49s", p[i].name);
        if(strcmp(p[i].name, "npc") == 0) {
            p[i].npc = 1; // Mark as NPC
        } else {
            p[i].npc = 0; // Mark as human player
        }
    }
    strcpy(dealer.name, "Dealer");
    round = 1;
    while (1) {
        init_cards();
        init_players(p);
        dealer.score = 0;
        dealer.hand_size = 0;
        for (int j = 0; j < CARD_LIMIT; j++) {
            dealer.hand[j] = (struct Card){0, 0, -1, 0};
        }
        printf("********** Round %d **********\n", round);
        round++;
        for (i = 0; i < PLAYER_LIMIT; i++) {
            // Deal initial two cards to player p[i]
            p[i].hand[p[i].hand_size] = draw_card();
            p[i].hand_size++;
            score_calc(&p[i]);
            p[i].hand[p[i].hand_size] = draw_card();
            p[i].hand_size++;
            score_calc(&p[i]);
        }
        dealer.hand[dealer.hand_size] = draw_card();
        dealer.hand_size++;
        score_calc(&dealer);
        dealer.hand[dealer.hand_size] = draw_card();
        dealer.hand_size++;
        printf("Dealer's visible card:\n");
        show_card(dealer.hand[0].number);
        printf("Dealer's score: %d\n", dealer.score);
        for (i = 0; i < PLAYER_LIMIT; i++) {
            if(p[i].npc) {
                npc_turn(&p[i]);
            } else {
                turn(&p[i]);
            }
        }
        dturn(&dealer);
        for (i = 0; i < PLAYER_LIMIT; i++) {
            // Determine winner between p[i] and dealer
            if (p[i].score > 21) {
                printf("%s You lose.\n", p[i].name);
            } else if (dealer.score > 21 || p[i].score > dealer.score) {
                printf("%s You win!\n", p[i].name);
            } else if (p[i].score < dealer.score) {
                printf("%s You lose.\n", p[i].name);
            } else {
                printf("%s Draw.\n", p[i].name);
            }
        }
        printf("Do you want to continue? (1: Yes, 2: No): ");
        scanf("%d", &a);
        if (a == 2) {
            break;
        } else if (a != 1) {
            printf("Invalid input. Exiting the game.\n");
            break;
        }
    }

    return 0;
}

void init_players(struct player *p) {
    for (int i = 0; i < PLAYER_LIMIT; i++) {
        p[i].score = 0;
        p[i].hand_size = 0;
        for (int j = 0; j < CARD_LIMIT; j++) {
            p[i].hand[j] = (struct Card){0, 0, -1, 0}; // Initialize with invalid card
        }
    }
}

void turn(struct player *p) {
    int a;
    while (p->score <= 21) {
        printf("%s's turn.\n", p->name);
        printf("***Your cards***\n");
        for (int i = 0; i < p->hand_size; i++) {
            show_card(p->hand[i].number);
        }
        printf("****************\n");
        printf("Score: %d\n", p->score);
        printf("Do you want to draw a card? (1: hit, 2: stand): ");
        scanf("%d", &a);
        if (a == 1) {
            struct Card drawn_card = draw_card();
            p->hand[p->hand_size] = drawn_card;
            show_card(drawn_card.number);
            // Add card to player's hand and update score
            p->hand_size++;
            score_calc(p);
            if (p->score > 21) {
                printf("***********\n");
                printf("* BUSTED! *\n");
                printf("***********\n");
                break;
            }
        } else if (a == 2) {
            break;
        } else {
            printf("Invalid input. Please enter 1 or 2.\n");
        }
    }
}

void dturn(struct player *dealer) {
    printf("Dealer's turn.\n");
    score_calc(dealer);
    while (dealer->score < 17) {
        struct Card drawn_card = draw_card();
        dealer->hand[dealer->hand_size] = drawn_card;
        dealer->hand_size++;
        score_calc(dealer);
    }
    printf("Dealer's cards:\n");
    for (int i = 0; i < dealer->hand_size; i++) {
        show_card(dealer->hand[i].number);
    }
    printf("Dealer's final score: %d\n", dealer->score);
}

void npc_turn(struct player *p) {
    printf("%s's turn.\n", p->name);
    score_calc(p);
    while (p->score < 17) {
        struct Card drawn_card = draw_card();
        p->hand[p->hand_size] = drawn_card;
        p->hand_size++;
        score_calc(p);
    }
    printf("%s's cards:\n", p->name);
    for (int i = 0; i < p->hand_size; i++) {
        show_card(p->hand[i].number);
    }
    printf("%s's final score: %d\n", p->name, p->score);
}

void score_calc(struct player *p) {
    int score = 0;
    int ace_count = 0;
    for (int i = 0; i < p->hand_size; i++) {
        int rank = p->hand[i].rank;
        if (rank >= 10) {
            score += 10;
        } else if (rank == 1) { // Ace
            score += 11;
            ace_count++;
        } else {
            score += rank;
        }
    }
    while (score > 21 && ace_count > 0) {
        score -= 10; // Count Ace as 1 instead of 11
        ace_count--;
    }
    p->score = score;
}