#define CUT

#include "libs/cut.h"
#include "libs/mainwrap.h"

#include <stdlib.h>

/* Here, you can write your own tests
 *
 * Suggestions:
 *    - more complex combinations, such as triples
 *    - more incorrect inputs
 *
 * For bonus tests with arguments, use ‹app_main_args(ARGUMENTS…)›
 * instead of ‹app_main()›. */



/* A test template: */


TEST(first_card_err) {
    INPUT_STRING(
        "as Ks\n"
        "2s 3s\n"
        "4s 5s 6s 7s 8s\n"
    );

    app_main();
    ASSERT_FILE(stderr, "Invalid card input\n");
}

TEST(second_card_err) {
    INPUT_STRING(
            "As ks\n"
            "2s 3s\n"
            "4s 5s 6s 7s 8s\n"
    );

    app_main();
    ASSERT_FILE(stderr, "Invalid card input\n");
}

TEST(format_err) {
    INPUT_STRING(
            "As+Ks\n"
            "2s 3s\n"
            "4s 5s 6s 7s 8s\n"
    );

    app_main();
    ASSERT_FILE(stderr, "Invalid input format\n");
}

TEST(flush_str_flush_win) {
    INPUT_STRING(
            "As Th\n"
            "9d Ac\n"
            "2s 3s 4s 5s 2d\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 1\n");
}

TEST(four_of_kind_win) {
    INPUT_STRING(
            "5s 5d\n"
            "9d Ac\n"
            "5c 5h 9s 9c 9h\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 2\n");
}
TEST(full_house_win) {
    INPUT_STRING(
            "Ts 5d\n"
            "Tc Ac\n"
            "Th Td 5s Ad Jh\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 2\n");
}
TEST(flush_n_straight_win) {
    INPUT_STRING(
            "6d 5d\n"
            "4h 3c\n"
            "2d Ad Jd 5s Qh\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 1\n");
}

TEST(three_of_kind_win) {
    INPUT_STRING(
            "7h Ad\n"
            "7d Kc\n"
            "7c 7s Th 2c 3h\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 1\n");
}
TEST(three_of_kind_draw) {
    INPUT_STRING(
            "7h Ad\n"
            "7d Ac\n"
            "7c 7s Th 2c 3h\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Draw\n");
}
TEST(two_pair_draw) {
    INPUT_STRING(
            "8d 8h\n"
            "8s 8c\n"
            "5c 5s Th 2c Qh\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Draw\n");
}
TEST(two_pair_win) {
    INPUT_STRING(
            "8d 8h\n"
            "Js Jc\n"
            "5c 5s Th 2c Qh\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 2\n");
}
TEST(two_pair_high_card_win) {
    INPUT_STRING(
            "9d Kh\n"
            "9h Qd\n"
            "Ac As 9c 2c 4h\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 1\n");
}

TEST(pair_high_card_win) {
    INPUT_STRING(
            "9d Kh\n"
            "9h Qd\n"
            "Tc 8d 9c 2c 4h\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 1\n");
}
TEST(pair_win) {
    INPUT_STRING(
            "Ac 9h\n"
            "6d 6h\n"
            "Ad 8d 7c 2c 4h\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 1\n");
}
TEST(high_card_draw) {
        INPUT_STRING(
                "2c 3d\n"
                "2s 3h\n"
                "Ad Kd Jd Ts 7c\n"
        );

        app_main();
        ASSERT_FILE(stdout, "Draw\n");
}

TEST(high_card_win) {
    INPUT_STRING(
            "2c 9c\n"
            "2s 3h\n"
            "Ad Kd Jd Ts 7c\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 1\n");
}

TEST(high_straight_win) {
    INPUT_STRING(
            "2c 3d\n"
            "8h 6s\n"
            "Ad 4s 5d Ts 7c\n"
    );

    app_main();
    ASSERT_FILE(stdout, "Player 2\n");
}
