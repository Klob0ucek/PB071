#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint-gcc.h>
#include <strings.h>

bool load_card(int *card);
void load_player(int player[7]);
bool load_table(int player1[7], int player2[7]);
bool load_instance(int player1[7], int player2[7]);
void validate_game(const int player1[7],const int player2[7]);


int evaluate_game(int player1[7],int player2[7]);
void hand_stats(uint8_t *player_hand_stats,const int player[7]);


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
    uint8_t p1_hand_stats[17];
    memset(p1_hand_stats, 0, sizeof(p1_hand_stats));
    hand_stats(p1_hand_stats, player1);
    uint8_t p2_hand_stats[17];
    memset(p2_hand_stats, 0, sizeof(p2_hand_stats));
    hand_stats(p2_hand_stats, player2);


    printf("Player1 stats: ");
    for (int i = 0; i < 17; ++i) {
        printf("%d ", p1_hand_stats[i]);
    }
    printf("\n");

    printf("Player2 stats: ");
    for (int i = 0; i < 17; ++i) {
        printf("%d ", p2_hand_stats[i]);
    }
    return 0;
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
void hand_stats(uint8_t *player_hand_stats, const int player[7]) {
    for (int i = 0; i < 7; ++i) {
        player_hand_stats[(player[i] % 10)] += 1;
        player_hand_stats[(player[i] / 10)] += 1;
    }
}

bool load_instance(int player1[7], int player2[7]) {
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
void validate_game(const int player1[7], const int player2[7]){
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

int four_of_kind (uint8_t *stats, int *high) {
    for (int i = 4; i < 17; i++){
        if (stats[i] == 4) {
            return 2;
            // TODO set high
        }
    }
    return 0;
}

int full_house (uint8_t *stats, int *high) {
    int pair;
    int triple;
    for (int i = 4; i < 17; i++){
        if (stats[i] == 2) {
            pair = i;
        }
        if (stats[i] == 3) {
            triple = i;
        }
    }
    if (pair && triple) {
        // TODO set high
        return 3;
    }
    return 0;
}

int flush(uint8_t *stats, int *high){
    for (int i = 0; i < 4; ++i) {
        if (stats[i] >= 5){
            return 4; // TODO find best card in flush
        }
    }
}

int straight(uint8_t *stats, int *high){
    int in_row = (stats[16]) ? 1 : 0;
    for (int i = 4; i < 17; ++i) {
        if (stats[i] > 0) {
            in_row++;
            if (in_row >= 5) {
                if (i + 1 < 17 && stats[i + 1] > 0){
                    continue;
                }
                else {
                    //TODO set high
                    return 5;
                }
            }
        } else {
            in_row = 0;
        }
    }
    return 0;
}

/**
 * Finds triple in players hand
 *
 * @param stats stats about players cards
 * @param high pointer at best triple
 * @return
 */
int three_of_kind(uint8_t *stats, int *high) {
    int triple;
    for (int i = 4; i < 17; ++i) {
        if (stats[i] == 3) {
            triple = i;
        }
    }
    if (triple){
        // TODO set high
        return 6;
    }
    return 0;
}

/**
 * Finds one or two pairs in players cards
 *
 * @param stats player stats
 * @param high pointer at highest card of pair
 *             if two pairs found return higher pair * 100 + lower pair
 * @return return win condition No.7 if two pairs No.8, else 0
 */
int pairs(uint8_t *stats, int *high){
    int pairs = 0;
    int pair1 = 0;
    int pair2 = 0;
    for (int i = 4; i < 17; ++i) {
        if (stats[i] == 2) {
            pairs++;
            if (pairs % 2 == 1){
                pair1 = i;
            } else {
                pair2 = i;
            }
        }
    }
    if (pairs >= 2) {
        // TODO set high
        return 7;
    }
    if (pairs == 1) {
        //TODO set high
        return 8;
    }
    return 0;
}

/**
 * Finds highest card
 *
 * @param stats stats about players cards
 * @param high pinter at highest card
 * @return win condition No.9
 */
int high_card(uint8_t *stats, int *high) {
    for (int i = 4; i < 17; i++) {
        if (stats[i] > 0) {
            *high = i;
        }
    }
    return 9;
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
 * Note: I know this function is not pretty but it allowed me to change values of each card
 * to make future handling of the cards easier
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
        case '1':
            *card += 12;
            c = getchar();
            if (c != '0') {
                return false;
            }
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
