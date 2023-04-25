#include "libs/cut.h"
#include "libs/utils.h"
#include "libs/mainwrap.h"

#include "../src/decimals.h"

TEST(decimals_to_base) {
    int value;

    SUBTEST(2) {
        value = decimals_to_base(2);
        ASSERT(value == 100);
    }
    SUBTEST(1) {
        value = decimals_to_base(1);
        ASSERT(value == 10);
    }
    SUBTEST(0) {
        value = decimals_to_base(0);
        ASSERT(value == 1);
    }
    SUBTEST(-1) {
        value = decimals_to_base(-1);
        ASSERT(value == 1);
    }
}

TEST(load_decimal) {
    int value;
    SUBTEST("1, 2 -> 100") {
        value = load_decimal("1", 2, CURRENCY);
        ASSERT(value == 100);
    }
    SUBTEST("1.23, 2 -> 123") {
        value = load_decimal("1.23", 2, RATING);
        ASSERT(value == 123);
    }
}