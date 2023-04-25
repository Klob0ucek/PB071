#include "libs/cut.h"
#include "libs/utils.h"
#include "libs/mainwrap.h"

/*
 * You can use this file for your tests.
 * All tests will compile together into one binary.
 * You can also delete your colleagues test files,
 * if you decide not to use them.
 */

TEST(my_test)
{
    int rv = 0;
    CHECK(app_main_args("tests/data/extra/persons.txt",
                        "tests/data/extra/simple/currencies.txt",
                        "tests/data/extra/simple/payments-01.txt") == rv);
    /* use app_main() if you don't want to give any args */

    const char *correct_output = "Lydia van Bredevoort (LB) -> Assire var Anahid (AA): 10 BASE\n"
                                 "Triss Merigold (TM) -> Assire var Anahid (AA): 10 BASE\n";

    CHECK_NOT_EMPTY(stdout);
    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(my_test_2)
{
    int rv = 0;
    CHECK(app_main_args("tests/data/extra/persons.txt",
                        "tests/data/extra/simple/currencies.txt",
                        "tests/data/extra/simple/payments-02.txt") == rv);
    /* use app_main() if you don't want to give any args */

    const char *correct_output = "Francesca Findabair (FF) -> Assire var Anahid (AA): 88.37 BASE\n"
                                 "Francesca Findabair (FF) -> Galadriel (G): 85.12 BASE\n"
                                 "Margarita Laux-Antille (MLA) -> Galadriel (G): 1.62 BASE\n"
                                 "Queenie Goldstein (QG) -> Galadriel (G): 1.62 BASE\n";

    CHECK_NOT_EMPTY(stdout);
    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(my_test_3)
{
    int rv = 0;
    CHECK(app_main_args("tests/data/extra/persons.txt",
                        "tests/data/extra/advanced/currencies.txt",
                        "tests/data/extra/advanced/payments-01.txt") == rv);
    /* use app_main() if you don't want to give any args */
    CHECK_NOT_EMPTY(stderr);
}

TEST(my_test_4)
{
    int rv = 0;
    CHECK(app_main_args("tests/data/extra/persons.txt",
                        "tests/data/extra/advanced/currencies.txt",
                        "tests/data/extra/advanced/payments-02.txt") == rv);
    /* use app_main() if you don't want to give any args */
    CHECK_NOT_EMPTY(stderr);

}
