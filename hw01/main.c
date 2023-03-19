#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t const MASK_40_BITS = 0x10000000000;
uint64_t const MASK_32_BITS = 0x80000000;

bool load_chars(int amount, uint64_t *result)
{
    uint64_t input = 0;
    int c;

    while (amount > 0 && (c = getchar()) != EOF) {
        input <<= 8;
        amount--;
        input |= c;
    }
    if (c == EOF) {
        while (amount > 0) {
            input <<= 8;
            amount--;
        }
        *result = input;
        return false;
    }
    *result = input;
    return true;
}

void print_chars(uint64_t out, int amount)
{
    uint64_t char_mask = 0xff;
    char_mask <<= (8 * (amount - 1));
    uint64_t character;

    for (int i = 0; i < amount; i++) {
        character = out & char_mask;
        character >>= (amount - i - 1) * 8;
        putchar(character);
        char_mask >>= 8;
    }
}

/**
 *
 * @param input number to extend or shorten
 * @param inOutSwitch true if you want to extend input number to 40 bits
 *                    false if you want to shorten input number to 32 bits
 *
 * @return refactored number
 */
uint64_t extend_shorten_number(uint64_t input, bool inOutSwitch)
{
    uint64_t mask = MASK_32_BITS;
    uint64_t plus_mask = MASK_40_BITS;
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
        if (inOutSwitch) {
            if (mask & input) {
                out = out + plus_mask;
            }
        } else {
            if (plus_mask & input) {
                out |= mask;
            }
        }
        mask = mask >> 1;
    }
    return out;
}

uint64_t reverse(uint64_t num)
{
    uint64_t mask = 1;
    uint64_t plus_mask = MASK_40_BITS;
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

bool encode(void)
{
    uint64_t num = 0;
    bool continue_loading;
    while (true) {
        continue_loading = load_chars(4, &num);
        if (continue_loading || (!continue_loading && num != 0)) {
            num = extend_shorten_number(num, true);
            num = reverse(num);
            num = fill_parity(num);
            num = reverse(num);
            print_chars(num, 5);
        }
        if (!continue_loading) {
            break;
        }
    }
    return true;
}

bool decode(void)
{
    uint64_t num = 0;
    bool continue_reading;
    while (true) {
        continue_reading = load_chars(5, &num);
        if (num == 0 && !continue_reading) {
            break;
        }
        if (!continue_reading) {
            fprintf(stderr, "Wrong code word\n");
            return false;
        }
        num = extend_shorten_number(num, false);
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
