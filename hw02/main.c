#include <stdbool.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

const char CARD_VALUES[] = {EOF, '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
const char CARD_SUITS[] = {'h', 'd', 's', 'c'};

enum win_condition {
    Straight_flush = 1, Four_of_kind, Full_house, Flush, Straight, Three_oof_kind,
    Two_pair, Pair, High_card
};

void fill_straight_flush(const int highest, int util_array[5]) {
    for (int i = 0; i < 5; i++) {
        util_array[i] = highest + 2 - i;
    }
}

void fill_four(const int first, const int second, const int value_stats[13], int util_array[5]){
    for (int i = 0; i < 2; ++i) {
        util_array[i] = first + 2;
    }
    for (int i = 2; i < 4; ++i) {
        util_array[i] = second + 2;
    }
    int index = 12;
    while (value_stats[index] == 0 || index == first || index == second){
        index--;
    }
    util_array[4] = index + 2;
}

void fill_full_house(const int triple, const int pair, int util_array[5]){
    for (int i = 0; i < 3; i ++) {
        util_array[i] = triple + 2;
    }
    for (int i = 3; i < 5; i ++) {
        util_array[i] = pair + 2;
    }
}

void fill_flush(const int flush, const int value_stats[13], const int player[7], int util_array[5]){
    int util_i = 0;
    for (int i = 12; i >= 0; i--) {
        if (value_stats[i] != 0) {
            for (int j = 0; j < 7; j++) {
                if (player[j] % 10 == flush - 1 && player[j] / 10 == i + 1) {
                    util_array[util_i] = i + 2;
                    util_i++;
                }
            }
        }
        if (util_i == 5) {
            break;
        }
    }
}

void fill_high_card(const int value_stats[13], int util_array[5]){
    int util_i = 0;
    for (int i = 12; i >= 0; i--) {
        if (value_stats[i] == 1) {
            util_array[util_i] = i + 2;
            util_i++;
        }
        if (util_i == 5) {
            break;
        }
    }
}

void fill_pair_or_three(const int first, int left, const int value_stats[13], int util_array[5]) {
    int util_i;
    for (util_i = 0; util_i < 5 - left; util_i++) {
        util_array[util_i] = first + 2;
    }
    util_i = 5 - left;
    int index = 12;
    while (util_i < 5 || index >= 0) {
        if (value_stats[index] != 0 && index != first) {
            util_array[util_i] = index + 2;
            util_i++;
        }
        index--;
    }
}

enum win_condition analyze_data(const int suit_stats[4], const int value_stats[13],
        int players_cards[7], int util_array[5])
{
    uint8_t pair1 = 0;
    uint8_t pair2 = 0;
    uint8_t triple = 0;
    uint8_t four = 0;
    uint8_t straight = 0;
    uint8_t flush = 0;

    for (int i = 0; i < 4; i++) {
        if (suit_stats[i] >= 5) {
            flush = i + 1;
        }
    }
    int in_row = (value_stats[12] >= 1) ? 1 : 0;
    for (int i = 0; i < 13; i++) {
        int card = value_stats[i];
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

    if (straight && (flush > 0)) {
        fill_straight_flush(straight, util_array);
        return Straight_flush;
    }
    if (four) {
        fill_four(four, four, value_stats, util_array);
        return Four_of_kind;
    }
    if (triple && pair1) {
        fill_full_house(triple, pair1, util_array);
        return Full_house;
    }
    if (flush > 0) { // flush fixed
        fill_flush(flush, value_stats, players_cards, util_array);
        return Flush;
    }
    if (straight) {
        fill_straight_flush(straight, util_array);
        return Straight;
    }
    if (triple) {
        fill_pair_or_three(triple, 2, value_stats, util_array);
        return Three_oof_kind;
    }
    if (pair1 && pair2) {
        fill_four(pair1, pair2, value_stats, util_array);
        return Two_pair;
    }
    if (pair1) {
        fill_pair_or_three(pair1, 3, value_stats, util_array);
        return Pair;
    }
    fill_high_card(value_stats, util_array);
    return High_card;
}

void hand_stats(int suits_stats[4], int value_stats[13], const int player[7])
{
    for (int i = 0; i < 7; ++i) {
        suits_stats[(player[i] % 10) - 1] += 1;
        value_stats[(player[i] / 10) - 1] += 1;
    }
}

int evaluate_game(int players_cards[2][7])
{
    // if there are more players, change the size of 2d array
    int suit_stats[2][4] = {{0}, {0}};
    int value_stats[2][13] = {{0}, {0}};
    int util_array[2][5] = {{0}, {0}};
    enum win_condition win_conditions[2] = {0};

    enum win_condition condition;
    for (int i = 0; i < 2; i++){
        hand_stats(suit_stats[i], value_stats[i], players_cards[i]);
        condition = analyze_data(suit_stats[i], value_stats[i], players_cards[i], util_array[i]);
        win_conditions[i] = condition;
    }

    // this would have to be more complicated for more players
    if (win_conditions[0] == win_conditions[1]) {
        for (int i = 0; i < 5; i++) {
            if (util_array[0][i] != util_array[1][i]){
                return (util_array[0][i] > util_array[1][i]) ? 1 : 2;
            }
        }
        return 0;
    }
    return (win_conditions[0] < win_conditions[1]) ? 1 : 2;
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
            for (int i = 0; i < 14; ++i) {
                if (c == CARD_VALUES[i]){
                    if (i == 0) {
                        *card = EOF;
                        return true;
                    }
                    *card = i;
                    break;
                }
            }
            if (*card == 0) {
                return false;
            }

            *card *= 10;
            c = getchar();
            for (int i = 0; i < 4; ++i) {
                if (c == CARD_SUITS[i]){
                    *card += i + 1;
                    break;
                }
            }
            if (*card % 10 == 0) {
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
    bool first_card = false;
    for (int i = 0; i < 2; i++) {
        if (load_card(&card, first_card)) {
            if (card == EOF && !first_card) {
                return true;
            }
            player[i] = card;
            first_card = true;
        } else {
            fprintf(stderr, "Invalid card input\n");
            exit(1);
        }
    }
    int ch = getchar();
    if (ch != '\n') {
        fprintf(stderr, "Invalid input format\n");
        exit(1);
    }
    return false;
}

void validate_game(int players_cards[2][7])
{
    int table[9];
    for (int i = 0; i < 7; i++) {
        table[i] = players_cards[0][i];
    }
    for (int i = 0; i < 2; i++) {
        table[7 + i] = players_cards[1][i];
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

bool load_instance(int players_cards[2][7], bool *first_char_EOF)
{
    bool next_instance;
    *first_char_EOF = load_player(players_cards[0]);
    if (*first_char_EOF) {
        return false;
    }
    load_player(players_cards[1]);
    next_instance = load_table(players_cards[0], players_cards[1]);
    validate_game(players_cards);
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
        int players_cards[2][7]; // 2 is number of player in the game
        bool first_char_EOF;
        next_game = load_instance(players_cards, &first_char_EOF);
        if (first_char_EOF) {
            return 0;
        }
        switch (evaluate_game(players_cards)) {
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
