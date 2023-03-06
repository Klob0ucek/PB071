/*
 * You can write your own tests in this file
 */

#define CUT
#include "libs/cut.h"

#include "libs/mainwrap.h"

#include <stdlib.h>

// Test template just for you ;)
/*
TEST(TODO_NAME_MUST_BE_UNIQUE)
{
    const unsigned char input[] = {
        // TODO
    };
    const unsigned char output[] = {
        // TODO
    };
    INPUT_BYTES(input);
    CHECK(app_main(TODO) == EXIT_SUCCESS);
    CHECK_FILE(stderr, TODO);
    CHECK_BINARY_FILE(stdout, output);
}
*/

TEST(mine_encode_test_one)
{
    const unsigned char input[] = {
            0x36,0xa6,0x69,0xdd
    };
    const unsigned char output[] = {
            0x6b, 0xb5, 0x19, 0xa7, 0x3a
    };
    INPUT_BYTES(input);
    CHECK(app_main("-e") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

TEST(mine_encode_test_two)
{
    const unsigned char input[] = {
            0x36,0xa6,0x69
    };
    const unsigned char output[] = {
            0x4b, 0xb5, 0x19, 0xa4, 0x00
    };
    INPUT_BYTES(input);
    CHECK(app_main("-e") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

TEST(mine_encode_test_three)
{
    const unsigned char input[] = {
            0xfd, 0x81, 0xc1, 0xdb
    };
    const unsigned char output[] = {
            0x7f, 0xec, 0x07, 0x07, 0x36
    };
    INPUT_BYTES(input);
    CHECK(app_main("-e") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

TEST(mine_encode_test_four)
{
    const unsigned char input[] = {
            0xfd, 0x81, 0xc1, 0xdb, 0xab
    };
    const unsigned char output[] = {
            0x7f, 0xec, 0x07, 0x07, 0x36, 0x72, 0xd8, 0x00, 0x00, 0x00
    };
    INPUT_BYTES(input);
    CHECK(app_main("-e") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

TEST(mine_encode_test_five)
{
    const unsigned char input[] = {
            0x03, 0xa9, 0x63, 0x88, 0x1b, 0xcc, 0x69, 0xb2
    };
    const unsigned char output[] = {
            0x40, 0x1d, 0xa5, 0x8e, 0x90, 0x61, 0xde, 0xb1, 0xa6, 0xe4
    };
    INPUT_BYTES(input);
    CHECK(app_main("-e") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

TEST(mine_encode_test_six)
{
    const unsigned char input[] = {
            0x03, 0xa9, 0x63, 0x88, 0x1b, 0xcc, 0x69, 0xb2, 0xbd
    };
    const unsigned char output[] = {
            0x40, 0x1d, 0xa5, 0x8e, 0x90, 0x61, 0xde, 0xb1, 0xa6, 0xe4, 0x5b, 0xe8, 0x00, 0x00, 0x00
    };
    INPUT_BYTES(input);
    CHECK(app_main("-e") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

TEST(mine_encode_test_seven)
{
    const unsigned char input[] = {
            0x03
    };
    const unsigned char output[] = {
            0x68, 0x18, 0x00, 0x00, 0x00
    };
    INPUT_BYTES(input);
    CHECK(app_main("-e") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

TEST(mine_encode_test_eight)
{
    const unsigned char input[] = {
            0xff, 0xff, 0xff, 0xff
    };
    const unsigned char output[] = {
            0x17, 0xff, 0xff, 0xff, 0x7e
    };
    INPUT_BYTES(input);
    CHECK(app_main("-e") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

TEST(mine_decode_test_one)
{
    const unsigned char input[] = {
            0x7f, 0xec, 0x07, 0x07, 0x36, 0x72, 0xd8, 0x00, 0x00, 0x00
    };
    const unsigned char output[] = {
            0xfd, 0x81, 0xc1, 0xdb, 0xab, 0x00, 0x00, 0x00
    };
    INPUT_BYTES(input);
    CHECK(app_main("-d") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

TEST(mine_decode_test_two)
{
    const unsigned char input[] = {
            0x7f, 0xec, 0x07, 0x07, 0x36, 0x72, 0xd8, 0x00, 0x00
    };
    const unsigned char output[] = {
            0xfd, 0x81, 0xc1, 0xdb
    };
    INPUT_BYTES(input);
    CHECK(app_main("-d") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "Wrong code word\n");
    CHECK_BINARY_FILE(stdout, output);
}

TEST(mine_decode_test_three)
{
    const unsigned char input[] = {
            0x66, 0x8b, 0x21, 0xbd, 0xd4
    };
    const unsigned char output[] = {
            0x61, 0x68, 0x6f, 0x6a
    };
    INPUT_BYTES(input);
    CHECK(app_main("-d") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

TEST(mine_decode_test_four)
{
    const unsigned char input[] = {
            0x01, 0x02, 0x03, 0x04, 0x05
    };
    const unsigned char output[] = {
            0x10, 0x40, 0xc1, 0x02
    };
    INPUT_BYTES(input);
    CHECK(app_main("-d") == EXIT_SUCCESS);
    CHECK_FILE(stderr, "");
    CHECK_BINARY_FILE(stdout, output);
}

