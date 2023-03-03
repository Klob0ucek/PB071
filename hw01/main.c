#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/*void print_bin(uint64_t num) {
    uint64_t mask = 0x8000000000;
    for (int i = 0; i < 40; i ++) {
        printf(num & mask ? "1" : "0");
        mask = mask >> 1;
    }
    printf("\n");
    return;
}*/

uint64_t load_chars(int amount) {
    uint64_t num = 0;
    int c;

    while ((c = getchar()) != EOF && amount > 0) {
        num <<= 8;
        amount--;
        num |= c;
    }
    while (amount > 0) {
        num <<= 8;
        amount--;
    }
    return num;
}

void print_chars(uint64_t out, int amount) {
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

uint64_t input32to40(uint64_t input){
    uint64_t mask = 0x80000000;
    uint64_t plus_mask = 0x10000000000;
    int skip = 1;
    uint64_t out = 0;

    for (int i = 0; i < 40; i++) {
        plus_mask = plus_mask >> 1 ;
        if (i == 0 || i == 39) {
            continue;
        } else if (i == skip) {
            skip = skip << 1;
            continue;
        } else {
            if (mask & input) {
                out = out + plus_mask;
            }
            mask = mask >> 1;
        }
    }
    return out;
}

uint64_t reverse(uint64_t num) {
    uint64_t mask = 0x1;
    uint64_t plus_mask = 0x10000000000;
    uint64_t result = 0;
    while (plus_mask > 0) {
        if (num & mask) {
            result += plus_mask;
        }
        mask = mask << 1; plus_mask = plus_mask >> 1;
    }
    result = result >> 1;
    return result;
}

int parity(int bit, uint64_t num) {
    int count_ones = 0;
    uint64_t mask = 1;
    mask = mask << 1;
    for (int i = 1; i < 40; i++) {
//        printf("%d : ", i); print_bin(mask);
        if (bit & i) {
            if (num & mask) {
                count_ones += 1;
//                printf("%d : %d, %lld\n", bit, i, mask);
            }
        }
        mask = mask << 1;
    }
//    printf("%d : %d\n", bit, count_ones);
    return count_ones % 2;
}

bool encode(void)
{
    uint64_t input = load_chars(4);
    uint64_t out = input32to40(input);
    out = reverse(out);

    for (int i = 1; i < 40; i = i << 1) {
        if (parity(i, out)) {
            uint64_t parity_bit = 1;
            parity_bit <<= i;
            out |= parity_bit;
        }
    }

    out = reverse(out);
    print_chars(out, 5);
    return true;
}

bool decode(void)
{
    printf("Input five characters: ");

    uint64_t input = 0;
    int c;
    int counter = 0;

    while ((c = getchar()) != EOF) {
        input<<= 8;
        input |= c;
        counter++;
    }

    if (counter % 5 != 0) {
        fprintf(stderr, "Wrong code word\n");
        return false;
    }

    uint64_t mask = 0x80000000;
    uint64_t plus_mask = 0x10000000000;
    int skip = 1;
    unsigned int result = 0;

    for (int i = 0; i < 40; i++){
        plus_mask = plus_mask >> 1 ;
        if (i == 0 || i == 39) {
            continue;
        }
        if (i == skip) {
            skip = skip << 1;
            continue;
        }
        if (plus_mask & input) {
            result += mask;
        }
        mask = mask >> 1;
    }
    print_chars(result, 4);
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
