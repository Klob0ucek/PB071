#include <ctype.h>
#include <stdbool.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int analyze_data(const uint8_t *stats, const int *player, int *util_array, int *win_cards)
{
    uint8_t pair1 = 0;
    uint8_t pair2 = 0;
    uint8_t triple = 0;
    uint8_t four = 0;
    uint8_t straight = 0;
    uint8_t flush = 255;

    for (int i = 0; i < 4; i++) {
        if (stats[i] >= 5) {
            flush = i;
        }
    }
    int in_row = (stats[16] >= 1) ? 1 : 0;
    for (int i = 4; i < 17; i++) {
        uint8_t card = stats[i];
        if (card == 0) {
            in_row = 0;
        } else {
            in_row++;
            if (card == 2) {
                pair2 = pair1;
                pair1 = i;
            }
            if (card == 3) {
                triple = i;
            }
            if (card == 4) {
                four = i;
            }
            if (in_row >= 5) {
                straight = i;
            }
        }
    }

    int util_i = 0;
    if (straight && (flush < 4)) {
        *win_cards = straight;
        return 1;
    }
    if (four) {
        *win_cards = four;
        for (int i = 16; i > 3; i--) {
            if (stats[i] >= 1 && i != four) {
                util_array[util_i] = i;
                util_i++;
            }
            if (util_i == 1) {
                break;
            }
        }
        return 2;
    }
    if (triple && pair1) {
        *win_cards = triple * 100 + pair1;
        return 3;
    }
    if (flush < 4) { // flush fixed
        for (int i = 16; i > 3; i--) {
            if (stats[i] != 0) {
                for (int j = 0; j < 7; j++) {
                    if (player[j] % 10 == flush && player[j] / 10 == i) {
                        util_array[util_i] = i;
                        util_i++;
                    }
                }
            }
            if (util_i == 5) {
                break;
            }
        }
        return 4;
    }
    if (straight) {
        *win_cards = straight;
        return 5;
    }
    if (triple) {
        *win_cards = triple;
        for (int i = 16; i > 3; i--) {
            if (stats[i] == 1) {
                util_array[util_i] = i;
                util_i++;
            }
            if (util_i == 2) {
                break;
            }
        }
        return 6;
    }
    if (pair1 && pair2) {
        *win_cards = pair1 * 100 + pair2;
        for (int i = 16; i > 3; i--) {
            if (stats[i] == 1) {
                util_array[util_i] = i;
                util_i++;
            }
            if (util_i == 1) {
                break;
            }
        }
        return 7;
    }
    if (pair1) {
        *win_cards = pair1;
        for (int i = 16; i > 3; i--) {
            if (stats[i] == 1) {
                util_array[util_i] = i;
                util_i++;
            }
            if (util_i == 3) {
                break;
            }
        }
        return 8;
    }
    for (int i = 16; i > 3; i--) {
        if (stats[i] == 1) {
            util_array[util_i] = i;
            util_i++;
        }
        if (util_i == 5) {
            break;
        }
    }
    return 9;
}


/**
 * Function that evaluates game instance
 *
 * @param player1 player 1 cards
 * @param player2 player 2 cards
 * @return returns 0 if draw, 1 if player 1 is winner else 2 if player 2 is winner
 */
void hand_stats(uint8_t *player_hand_stats, const int player[7])
{
    for (int i = 0; i < 7; ++i) {
        player_hand_stats[(player[i] % 10)] += 1;
        player_hand_stats[(player[i] / 10)] += 1;
    }
}

int evaluate_game(int player1[7], int player2[7])
{
    uint8_t p1_hand_stats[17] = {0};
    int util_p1[5] = {0};
    int p1_win_cards = 0;
    hand_stats(p1_hand_stats, player1);

    uint8_t p2_hand_stats[17] = {0};
    int util_p2[5] = {0};
    int p2_win_cards = 0;
    hand_stats(p2_hand_stats, player2);

    int win_condition_p1 = analyze_data(p1_hand_stats, player1, util_p1, &p1_win_cards);
    int win_condition_p2 = analyze_data(p2_hand_stats, player2, util_p2, &p2_win_cards);

    if (win_condition_p1 == win_condition_p2) {
        switch (win_condition_p1) {
        case 9: // high card
            for (int i = 0; i < 5; i++) {
                if (util_p1[i] == util_p2[i]) {
                    continue;
                }
                return (util_p1[i] > util_p2[i]) ? 1 : 2;
            }
            return 0;
        case 8: // one pair
            if (p1_win_cards == p2_win_cards) {
                for (int i = 0; i < 3; ++i) {
                    if (util_p1[i] == util_p2[i]) {
                        continue;
                    }
                    return (util_p1[i] > util_p2[i]) ? 1 : 2;
                }
                return 0;
            }
            return (p1_win_cards > p2_win_cards) ? 1 : 2;
        case 7: // two pairs
            if (p1_win_cards / 100 == p2_win_cards / 100) {
                if (p1_win_cards % 100 == p2_win_cards % 100) {
                    if (util_p1[0] == util_p2[0]) {
                        return 0;
                    }
                    return (util_p1[0] > util_p2[0]) ? 1 : 2;
                }
                return (p1_win_cards % 100 > p2_win_cards % 100) ? 1 : 2;
            }
            return (p1_win_cards / 100 > p2_win_cards / 100) ? 1 : 2;
        case 6: // three of kind
            if (p1_win_cards == p2_win_cards) {
                for (int i = 0; i < 2; ++i) {
                    if (util_p1[i] == util_p2[i]) {
                        continue;
                    }
                    return (util_p1[i] > util_p2[i]) ? 1 : 2;
                }
                return 0;
            }
            return (p1_win_cards > p2_win_cards) ? 1 : 2;
        case 5: // straight
            if (p1_win_cards == p2_win_cards) {
                return 0;
            }
            return (p1_win_cards > p2_win_cards) ? 1 : 2;
        case 4: // flush
            for (int i = 0; i < 5; i++) {
                if (util_p1[i] == util_p2[i]) {
                    continue;
                }
                return (util_p1[i] > util_p2[i]) ? 1 : 2;
            }
            return 0;
        case 3: // full house
            if (p1_win_cards / 100 == p2_win_cards / 100) {
                if (p1_win_cards % 100 == p2_win_cards % 100) {
                    return 0;
                }
                return (p1_win_cards % 100 > p2_win_cards % 100) ? 1 : 2;
            }
            return (p1_win_cards / 100 > p2_win_cards / 100) ? 1 : 2;
        case 2: // four of kind
            if (p1_win_cards == p2_win_cards) {
                if (util_p1[0] == util_p2[0]) {
                    return 0;
                }
                return (util_p1[0] > util_p2[0]) ? 1 : 2;
            }
            return (p1_win_cards > p2_win_cards) ? 1 : 2;
        case 1:
            if (p1_win_cards == p2_win_cards) {
                return 0;
            }
            return (p1_win_cards > p2_win_cards) ? 1 : 2;
        default:
            fprintf(stderr, "Evaluation error\n");
            return 0;
        }
    }
    return (win_condition_p1 < win_condition_p2) ? 1 : 2;
}

/**
 * Function that loads a card from standard input and converts it into card number
 *
 * Note: I know this function is not pretty but it allowed me to change values of each card
 * to make future handling of the cards easier
 *
 * @param card is pointer at a number where will be the card stored
 * @return false if unknown cards are loaded else true
 */
bool load_card(int *card, bool need_white_space)
{
    int c;
    bool white_space = false;
    *card = 0;
    while (true) {
        c = getchar();
        if (isspace(c)) {
            white_space = true;
            continue;
        }
        if (white_space || !need_white_space) {
            switch (c) {
                case EOF:
                    *card = EOF;
                    return true;
                case '2':
                    *card += 4;
                    break;
                case '3':
                    *card += 5;
                    break;
                case '4':
                    *card += 6;
                    break;
                case '5':
                    *card += 7;
                    break;
                case '6':
                    *card += 8;
                    break;
                case '7':
                    *card += 9;
                    break;
                case '8':
                    *card += 10;
                    break;
                case '9':
                    *card += 11;
                    break;
                case 'T':
                    *card += 12;
                    break;
                case 'J':
                    *card += 13;
                    break;
                case 'Q':
                    *card += 14;
                    break;
                case 'K':
                    *card += 15;
                    break;
                case 'A':
                    *card += 16;
                    break;
                default:
                    return false;
            }
            *card *= 10;
            c = getchar();
            switch (c) {
                case 'h':
                    *card += 0;
                    break;
                case 'd':
                    *card += 1;
                    break;
                case 's':
                    *card += 2;
                    break;
                case 'c':
                    *card += 3;
                    break;
                default:
                    return false;
            }
            return true;
        }
        fprintf(stderr, "White space missing\n");
        exit(1);
    }
}

bool load_table(int *player1, int *player2)
{
    int card = 0;
    int ch;
    bool need_white_space = false;
    for (int i = 2; i < 7; ++i) {
        if (load_card(&card, need_white_space)) {
            player1[i] = card;
            player2[i] = card;
        } else {
            fprintf(stderr, "Invalid card input\n");
            exit(1);
        }
        need_white_space = true;
    }
    ch = getchar();
    if (ch != '\n') {
        fprintf(stderr, "Invalid input format\n");
        exit(1);
    }
    return true;
}
/**
 * Function that will load cards of a player
 *
 * @param player array of numbers each representing a card in player's hand
 */
bool load_player(int *player)
{
    int card = 0;
    if (load_card(&card, false)) {
        if (card == EOF) {
            return true;
        }
        player[0] = card;
    } else {
        fprintf(stderr, "Invalid card input\n");
        exit(1);
    }
    card = 0;
    if (load_card(&card, true)) {
        player[1] = card;
    } else {
        fprintf(stderr, "Invalid card input\n");
        exit(1);
    }
    int ch = getchar();
    if (ch != '\n') {
        fprintf(stderr, "Invalid input format\n");
        exit(1);
    }
    return false;
}

void validate_game(const int player1[7], const int player2[7])
{
    int table[9];
    for (int i = 0; i < 7; i++) {
        table[i] = player1[i];
    }
    for (int i = 0; i < 2; i++) {
        table[7 + i] = player2[i];
    }
    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 9; j++) {
            if (table[i] == table[j]) {
                fprintf(stderr, "Two or more cards are the same\n");
                exit(1);
            }
        }
    }
}

bool load_instance(int player1[7], int player2[7], bool *first_char_EOF)
{
    bool next_instance;
    *first_char_EOF = load_player(player1);
    if (*first_char_EOF) {
        return false;
    }
    load_player(player2);
    next_instance = load_table(player1, player2);
    validate_game(player1, player2);
    return next_instance;
}

/* Parse the number of players at the table for the bonus extension.
 * IMPORTANT: Do not modify this function! */
static int parse_players(int argc, char **argv)
{
    switch (argc) {
    case 1:
        return 2;
    case 2:
        return atoi(argv[1]);
    default:
        return 0;
    }
}

int main(int argc, char **argv)
{
    int players = parse_players(argc, argv);
    /* TODO (BONUS):
     * If you intend to implement the bonus, «REMOVE» the following line and use
     * the ‹players› variable. It either contains the amount of players, or 0 on
     * invalid program arguments. The validation is left up to you. */
    (void) players;

    bool next_game;
    while (true) {
        int player1[7];
        int player2[7];
        bool first_char_EOF;
        next_game = load_instance(player1, player2, &first_char_EOF);
        if (first_char_EOF) {
            return 0;
        }
        switch (evaluate_game(player1, player2)) {
        case 0:
            fprintf(stdout, "Draw\n");
            break;
        case 1:
            fprintf(stdout, "Player 1\n");
            break;
        case 2:
            fprintf(stdout, "Player 2\n");
            break;
        default:
            printf("Game undecided\n");
        }
        if (!next_game) {
            break;
        }
    }
    return 0;
}
