#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool load_chars(int amount, uint64_t *result)
{
    uint64_t num = 0;
    int c;

    while (amount > 0 && (c = getchar()) != EOF) {
        num <<= 8;
        amount--;
        num |= c;
    }
    if (c == EOF) {
        while (amount > 0) {
            num <<= 8;
            amount--;
        }
        *result = num;
        return false;
    }
    *result = num;
    return true;
}

void print_chars(uint64_t out, int amount)
{
    uint64_t mask = 1;
    mask <<= ((8 * amount) - 1);

    int character;
    int character_mask;

    for (int i = 0; i < amount; i++) {
        character = 0;
        character_mask = 0x80;
        for (int j = 0; j < 8; j++) {
            if (mask & out) {
                character |= character_mask;
            }
            mask >>= 1;
            character_mask >>= 1;
        }
        putchar(character);
    }
}

uint64_t input32to40(uint64_t input)
{
    uint64_t mask = 0x80000000;
    uint64_t plus_mask = 0x10000000000;
    int skip = 1;
    uint64_t out = 0;

    for (int i = 0; i < 40; i++) {
        plus_mask = plus_mask >> 1;
        if (i == 0 || i == 39) {
            continue;
        }
        if (i == skip) {
            skip = skip << 1;
            continue;
        }
        if (mask & input) {
            out = out + plus_mask;
        }
        mask = mask >> 1;
    }
    return out;
}

uint64_t reverse(uint64_t num)
{
    uint64_t mask = 0x1;
    uint64_t plus_mask = 0x10000000000;
    uint64_t result = 0;
    while (plus_mask > 0) {
        if (num & mask) {
            result += plus_mask;
        }
        mask = mask << 1;
        plus_mask = plus_mask >> 1;
    }
    result = result >> 1;
    return result;
}

int parity(int bit, uint64_t num)
{
    int count_ones = 0;
    uint64_t mask = 1;
    mask = mask << 1;
    for (int i = 1; i < 40; i++) {
        if (bit & i) {
            if (num & mask) {
                count_ones += 1;
            }
        }
        mask = mask << 1;
    }
    return count_ones % 2;
}

uint64_t fill_parity(uint64_t num)
{
    for (int i = 1; i < 40; i = i << 1) {
        if (parity(i, num)) {
            uint64_t parity_bit = 1;
            parity_bit <<= i;
            num |= parity_bit;
        }
    }
    return num;
}

uint64_t input40to32(uint64_t num)
{
    uint64_t mask = 0x80000000;
    uint64_t plus_mask = 0x10000000000;
    int skip = 1;
    uint64_t result = 0;

    for (int i = 0; i < 40; i++) {
        plus_mask = plus_mask >> 1;
        if (i == 0 || i == 39) {
            continue;
        }
        if (i == skip) {
            skip = skip << 1;
            continue;
        }
        if (plus_mask & num) {
            result |= mask;
        }
        mask = mask >> 1;
    }
    return result;
}

bool encode(void)
{
    uint64_t num = 0;
    while (load_chars(4, &num) == true) {
        num = input32to40(num);
        num = reverse(num);
        num = fill_parity(num);
        num = reverse(num);
        print_chars(num, 5);
    }
    if (num != 0) {
        num = input32to40(num);
        num = reverse(num);
        num = fill_parity(num);
        num = reverse(num);
        print_chars(num, 5);
    }
    return true;
}

bool decode(void)
{
    uint64_t num = 0;
    bool error;
    while (true) {
        error = load_chars(5, &num);
        if (num == 0 && !error) {
            break;
        }
        if (!error) {
            fprintf(stderr, "Wrong code word\n");
            return false;
        }
        num = input40to32(num);
        print_chars(num, 4);
    }
    return true;
}

/*************************************
 * DO NOT MODIFY THE FUNCTION BELLOW *
 *************************************/
int main(int argc, char *argv[])
{
    if (argc > 2) {
        fprintf(stderr, "Usage: %s (-e|-d)\n", argv[0]);
        return EXIT_FAILURE;
    }
    bool is_success = false;
    if (argc == 1 || (strcmp(argv[1], "-e") == 0)) {
        is_success = encode();
    } else if (strcmp(argv[1], "-d") == 0) {
        is_success = decode();
    } else {
        fprintf(stderr, "Unknown argument: %s\n", argv[1]);
    }
    return is_success ? EXIT_SUCCESS : EXIT_FAILURE;
}
