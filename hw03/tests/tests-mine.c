/*
 * You can use this file for your own tests
 */

#include "libs/cut.h"
#include "libs/mainwrap.h"
#include "libs/utils.h"

#include <stdlib.h>

/* The following “extentions” to CUT are available in this test file:
 *
 * • ‹CHECK_IS_EMPTY(file)› — test whether the file is empty.
 * • ‹CHECK_NOT_EMPTY(file)› — inverse of the above.
 *
 * • ‹app_main_args(ARG…)› — call your ‹main()› with given arguments.
 * • ‹app_main()› — call your ‹main()› without any arguments. */

TEST(my_test_1)
{
    int rv = 0; /* return value of main()*/
    CHECK(app_main_args("-t" "APBGC" "tests/data/example-containers.csv" "tests/data/example-paths.csv") != rv);

    /* TIP: Use ‹app_main()› to test the program without arguments. */

    const char *correct_output =
            "ID: 1, Type: Colored glass, Capacity: 1550, Address: Drozdi 55, Neighbors: 4\n"
            "ID: 2, Type: Clear glass, Capacity: 1550, Address: Drozdi 55, Neighbors: 4\n"
            "ID: 3, Type: Plastics and Aluminium, Capacity: 1100, Address: Drozdi 55, Neighbors: 4\n"
            "ID: 4, Type: Colored glass, Capacity: 900, Address: Drozdi 55, Neighbors: 1 2 3 5 8\n"
            "ID: 5, Type: Paper, Capacity: 5000, Address: Klimesova 60, Neighbors: 4 8\n"
            "ID: 6, Type: Colored glass, Capacity: 3000, Address: Klimesova 60, Neighbors: 8\n"
            "ID: 7, Type: Plastics and Aluminium, Capacity: 5000, Address: Klimesova 60, Neighbors: 8\n"
            "ID: 8, Type: Biodegradable waste, Capacity: 3000, Address: Na Buble 5, Neighbors: 4 5 6 7 11\n"
            "ID: 10, Type: Plastics and Aluminium, Capacity: 900, Address: Odlehla 70, Neighbors: 9\n"
            "ID: 11, Type: Paper, Capacity: 2000, Address: Odlehla 70, Neighbors: 8\n"
            ;

    ASSERT_FILE(stderr, "Failed to load parser!\n");
}

TEST(my_test_2)
{
    int rv = 0; /* return value of main()*/
    CHECK(app_main_args("-c" "5000-5000" "tests/data/example-containers.csv" "tests/data/example-paths.csv") == rv);

    const char *correct_output =
            "ID: 5, Type: Paper, Capacity: 5000, Address: Klimesova 60, Neighbors: 4 8\n"
            "ID: 7, Type: Plastics and Aluminium, Capacity: 5000, Address: Klimesova 60, Neighbors: 8\n"
    ;

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(my_test_3)
{
    int rv = 0; /* return value of main()*/
    CHECK(app_main_args("-t" "APBGC" "-c" "500-1600" "tests/data/example-containers.csv" "tests/data/example-paths.csv") == rv);

    const char *correct_output =
            "ID: 1, Type: Colored glass, Capacity: 1550, Address: Drozdi 55, Neighbors: 4\n"
            "ID: 2, Type: Clear glass, Capacity: 1550, Address: Drozdi 55, Neighbors: 4\n"
            "ID: 3, Type: Plastics and Aluminium, Capacity: 1100, Address: Drozdi 55, Neighbors: 4\n"
            "ID: 4, Type: Colored glass, Capacity: 900, Address: Drozdi 55, Neighbors: 1 2 3 5 8\n"
            "ID: 10, Type: Plastics and Aluminium, Capacity: 900, Address: Odlehla 70, Neighbors: 9\n"
    ;

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(my_test_4)
{
    int rv = 0; /* return value of main()*/
    CHECK(app_main_args("-p" "Y" "tests/data/example-containers.csv" "tests/data/example-paths.csv") == rv);

    const char *correct_output =
            "ID: 1, Type: Colored glass, Capacity: 1550, Address: Drozdi 55, Neighbors: 4\n"
            "ID: 2, Type: Clear glass, Capacity: 1550, Address: Drozdi 55, Neighbors: 4\n"
            "ID: 3, Type: Plastics and Aluminium, Capacity: 1100, Address: Drozdi 55, Neighbors: 4\n"
            "ID: 4, Type: Colored glass, Capacity: 900, Address: Drozdi 55, Neighbors: 1 2 3 5 8\n"
            "ID: 8, Type: Biodegradable waste, Capacity: 3000, Address: Na Buble 5, Neighbors: 4 5 6 7 11\n"
            "ID: 9, Type: Textile, Capacity: 500, Address: Na Buble 5, Neighbors: 10\n"
            "ID: 10, Type: Plastics and Aluminium, Capacity: 900, Address: Odlehla 70, Neighbors: 9\n"
            "ID: 11, Type: Paper, Capacity: 2000, Address: Odlehla 70, Neighbors: 8\n"
    ;

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(my_test_5)
{
    int rv = 0; /* return value of main()*/
    CHECK(app_main_args("-p" "Y" "tests/data/example-containers.csv" "tests/data/example-paths.csv") == rv);

    const char *correct_output =
            "ID: 5, Type: Paper, Capacity: 5000, Address: Klimesova 60, Neighbors: 4 8\n"
            "ID: 6, Type: Colored glass, Capacity: 3000, Address: Klimesova 60, Neighbors: 8\n"
            "ID: 7, Type: Plastics and Aluminium, Capacity: 5000, Address: Klimesova 60, Neighbors: 8\n"
    ;

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(my_test_6)
{
    int rv = 0; /* return value of main()*/
    CHECK(app_main_args("-t" "C" "-p" "Y" "tests/data/example-containers.csv" "tests/data/example-paths.csv") == rv);

    const char *correct_output =
            "ID: 6, Type: Colored glass, Capacity: 3000, Address: Klimesova 60, Neighbors: 8\n"
    ;

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(my_test_empty)
{
    CHECK(app_main_args("") != 0);

    CHECK_NOT_EMPTY(stderr);
}
