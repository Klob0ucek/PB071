#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool load_card(int *card);
void load_player(int player[7]);
bool load_table(int player1[7], int player2[7]);
bool load_instance(int player1[7], int player2[7]);
void validate_game(int player1[7],int player2[7]);
void merge_arrays(int const player1[7], int const player2[7], int table[9]);

int evaluate_game(int player1[7],int player2[7]);

int four_of_kind_win(int player1[7], int player2[7]);
int check_four_of_kind(int player[7]);

/* Parse the number of players at the table for the bonus extension.
 * IMPORTANT: Do not modify this function! */
static int parse_players (int argc, char **argv)
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
        evaluate_game(player1, player2);


        if (!next_game) {
            break;
        }
    }
    return 0;
}

int evaluate_game(int player1[7],int player2[7]) {
    int win = four_of_kind_win(player1, player2);
    if (win) {
        printf("Winner is Player%d", win);
        return win;
    }

}

int four_of_kind_win(int player1[7], int player2[7]) {
    int p1w = check_four_of_kind(player1);
    int p2w = check_four_of_kind(player2);
    if (p1w == p2w) {
        return 0;
    }
    return (p1w > p2w) ? 1 : 2;
}

int check_four_of_kind(int *player) {
    int kind_counter, card_kind;
    for (int i = 0; i < 4; ++i) {
        kind_counter = 1;
        card_kind = player[i] / 10;
        for (int j = 0; j < 7; ++j) {
            if (i == j) {
                continue;
            }
            if (card_kind == (player[j] / 10)) {
                kind_counter += 1;
            }
            if (kind_counter == 4) {
                return player[i];
            }
        }
    }
    return 0;
}

bool load_instance(int player1[7], int player2[7]) {
    bool next_instance;
    load_player(player1);
    load_player(player2);
    next_instance = load_table(player1, player2);
    validate_game(player1, player2);
    return next_instance;
}


void validate_game(int player1[7],int player2[7]){
    int table[9];
    merge_arrays(player1, player2, table);
    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 9; j++) {
            if (table[i] == table[j]) {
                fprintf(stderr, "Two or more cards are the same");
                exit(1);
            }
        }
    }
}

void merge_arrays(int const player1[7], int const player2[7], int table[9]) {
    for (int i = 0; i < 7; i++) {
        table[i] = player1[i];
    }
    for (int i = 0; i < 2; i++) {
        table[7 + i] = player2[i];
    }
}

/**
 * This function will load table cards from standard input
 *
 * @param table is array of 5 numbers each representing a card on the table
 * @return true if there is another instance of game to evaluate, else false
 */
bool load_table(int *player1, int *player2) {
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
void load_player(int *player){
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
 * @param card is pointer at a number where will be the card stored
 * @return false if unknown cards are loaded else true
 */
bool load_card(int *card) {
    int c;
    *card = 0;
    c = getchar();
    switch (c) {
        case '2':
            *card += 2;
            break;
        case '3':
            *card += 3;
            break;
        case '4':
            *card += 4;
            break;
        case '5':
            *card += 5;
            break;
        case '6':
            *card += 6;
            break;
        case '7':
            *card += 7;
            break;
        case '8':
            *card += 8;
            break;
        case '9':
            *card += 9;
            break;
        case '1':
            *card += 10;
            c = getchar();
            if (c != '0') {
                return false;
            }
            break;
        case 'J':
            *card += 11;
            break;
        case 'Q':
            *card += 12;
            break;
        case 'K':
            *card += 13;
            break;
        case 'A':
            *card += 14;
            break;
        default:
            return false;
    }
    *card *= 10;
    c = getchar();
    switch (c) {
        case 'h':
            *card += 1;
            break;
        case 'd':
            *card += 2;
            break;
        case 's':
            *card += 3;
            break;
        case 'c':
            *card += 4;
            break;
        default:
            return false;
    }
    return true;
}


/* my notation:
 * hearts, diamonds, spades, clubs = 1, 2, 3, 4
 * 2,3,4,5,6,7,8,9, = 2,3,4,5,6,7,8,9, 10
 * J, Q, K, A = 10, 11, 12, 13*/
