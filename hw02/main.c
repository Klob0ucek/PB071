#include <stdbool.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool load_card(int *card);
void load_player(int player[7]);
bool load_table(int player1[7], int player2[7]);
bool load_instance(int player1[7], int player2[7]);
void validate_game(const int player1[7], const int player2[7]);

int evaluate_game(int player1[7], int player2[7]);
void hand_stats(uint8_t *player_hand_stats, const int player[7]);

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
        next_game = load_instance(player1, player2);
        switch (evaluate_game(player1, player2)) {
        case 0:
            printf("Draw\n");
            break;
        case 1:
            printf("Player 1\n");
            break;
        case 2:
            printf("Player 2\n");
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

int analyze_data(uint8_t *stats, int *util_array, int *win_cards)
{
    uint8_t pair1 = 0;
    uint8_t pair2 = 0;
    uint8_t triple = 0;
    uint8_t four = 0;
    uint8_t straight = 0;
    uint8_t flush = -1;

    for (int i = 0; i < 4; i++) {
        if (stats[i] >= 5) {
            flush = i;
        }
    }
    int in_row = stats[16];
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
            if (stats[i] == 1) {
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
    if (flush < 4) {
        for (int i = 16; i > 3; i--) {
            if (stats[i] % 10 == flush) {
                util_array[util_i] = i;
                util_i++;
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

int evaluate_game(int player1[7], int player2[7])
{
    uint8_t p1_hand_stats[17];
    memset(p1_hand_stats, 0x00, 17);
    int util_p1[5];
    int p1_win_cards;
    hand_stats(p1_hand_stats, player1);

    uint8_t p2_hand_stats[17];
    memset(p2_hand_stats, 0x00, 17);
    int util_p2[5];
    int p2_win_cards;
    hand_stats(p2_hand_stats, player2);

    int win_condition_p1 = analyze_data(p1_hand_stats, util_p1, &p1_win_cards);
    int win_condition_p2 = analyze_data(p2_hand_stats, util_p2, &p2_win_cards);

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
                return (util_p1[0] == util_p2[0]) ? 1 : 2;
            }
            return (p1_win_cards > p2_win_cards) ? 1 : 2;
        case 1:
            if (p1_win_cards == p2_win_cards) {
                return 0;
            }
            return (p1_win_cards > p2_win_cards) ? 1 : 2;
        default:
            fprintf(stderr, "Evaluation error");
            return 0;
        }
    }
    return (win_condition_p1 < win_condition_p2) ? 1 : 2;
}

/**
 * This function will process 7 cards (2 hand cards and 5 table cards) into array of 1 byte integers.
 *
 * First four integers (index 0 to 3) will store data about card signs (hearts, diamonds, spades or clubs)
 * Next 14 integers wills store data about of "card numbers" (index 4 to 16)
 *
 * For example if player has 3 heart cards, then player_hand_stats will be set to 3
 *             if player has a Queen, a King and two Aces, then last 3 integers will be set to 1, 1, 2
 *
 * @param player_hand_stats array with length 17 of small integers
 * @param player array of numbers representing players cards
 */
void hand_stats(uint8_t *player_hand_stats, const int player[7])
{
    for (int i = 0; i < 7; ++i) {
        player_hand_stats[(player[i] % 10)] += 1;
        player_hand_stats[(player[i] / 10)] += 1;
    }
}

bool load_instance(int player1[7], int player2[7])
{
    bool next_instance;
    load_player(player1);
    load_player(player2);
    next_instance = load_table(player1, player2);
    validate_game(player1, player2);
    return next_instance;
}

/**
 * This function will validate if game instance is legal - mainly check if all cards are different
 * @param player1 First players cards
 * @param player2 second players cards
 */
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
                fprintf(stderr, "Two or more cards are the same");
                exit(1);
            }
        }
    }
}

/**
 * This function will load table cards from standard input
 *
 * @param table is array of 5 numbers each representing a card on the table
 * @return true if there is another instance of game to evaluate, else false
 */
bool load_table(int *player1, int *player2)
{
    int card = 0;
    int ch;
    for (int i = 0; i < 4; ++i) {
        if (load_card(&card)) {
            player1[i + 2] = card;
            player2[i + 2] = card;
        } else {
            fprintf(stderr, "Invalid card input");
            exit(1);
        }
        ch = getchar();
        if (!(ch == ' ' || ch == '\t')) {
            fprintf(stderr, "Invalid input format");
            exit(1);
        }
    }
    if (load_card(&card)) {
        player1[6] = card;
        player2[6] = card;
    } else {
        fprintf(stderr, "Invalid card input");
        exit(1);
    }
    ch = getchar();
    switch (ch) {
    case EOF:
        return false;
    case '\n':
        return true;
    default:
        fprintf(stderr, "Invalid input format");
        exit(1);
    }
}
/**
 * Function that will load cards of a player
 *
 * @param player array of numbers each representing a card in player's hand
 */
void load_player(int *player)
{
    int card = 0;
    if (load_card(&card)) {
        player[0] = card;
    } else {
        fprintf(stderr, "Invalid card input");
        exit(1);
    }
    int ch = getchar();
    if (!(ch == ' ' || ch == '\t')) {
        fprintf(stderr, "Invalid input format");
        exit(1);
    }
    card = 0;
    if (load_card(&card)) {
        player[1] = card;
    } else {
        fprintf(stderr, "Invalid card input");
        exit(1);
    }
    ch = getchar();
    if (ch != '\n') {
        fprintf(stderr, "Invalid input format");
        exit(1);
    }
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
bool load_card(int *card)
{
    int c;
    *card = 0;
    c = getchar();
    switch (c) {
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
