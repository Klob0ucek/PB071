#include "errors.h"
#include "structures.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

static int64_t normalize(int64_t number, int upper)
{
    int lower = upper / 10;
    while (number < lower)
        number *= 10;
    while (number > upper)
        number /= 10;
    return number;
}

int decimals_to_base(int decimals)
{
    int base = 1;
    while (decimals-- > 0)
        base *= 10;
    return base;
}

int64_t load_decimal(const char *string, int decimals, enum load_type load)
{
    int base = decimals_to_base(decimals);
    if (!strchr(string, '.')) {
        int64_t result;
        sscanf(string, "%" PRId64, &result);
        return (int64_t) result * base;
    }

    int64_t large;
    int64_t small;
    sscanf(string, "%" PRId64 ".%" PRId64, &large, &small);

    if (load == CURRENCY) {
        OP(large < 100000 && small < 10000, CURRENCY_WRONG_INPUT);
    }
    if (load == RATING) {
        OP(large < 10000000 && small < 100, CURRENCY_WRONG_INPUT);
    }

    return (int64_t) large * base + normalize(small, base);
}
