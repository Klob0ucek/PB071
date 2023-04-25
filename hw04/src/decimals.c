#include <stdio.h>
#include <string.h>
#include <inttypes.h>


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
    while (decimals --> 0)
        base *= 10;
    return base;
}

int64_t load_decimal(const char *string, int decimals)
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

    return (int64_t) large * base + normalize(small, base);
}
