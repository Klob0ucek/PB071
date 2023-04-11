/*
 * You can use this file for your own tests
 */

#include "libs/cut.h"
#include "libs/mainwrap.h"
#include "libs/utils.h"

#define FIRST_CONT_FILE "tests/data/Brno-BosonohyContainers.csv"
#define FIRST_PATH_FILE "tests/data/Brno-BosonohyPaths.csv"

#define SECOND_CONT_FILE "tests/data/Brno-IvanoviceContainers.csv"
#define SECOND_PATH_FILE "tests/data/Brno-IvanovicePaths.csv"

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
    CHECK(app_main_args("-t", "APBGC", "tests/data/example-containers.csv", "tests/data/example-paths.csv") == rv);

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

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(my_test_2)
{
    int rv = 0; /* return value of main()*/
    CHECK(app_main_args("-c", "5000-5000", "tests/data/example-containers.csv", "tests/data/example-paths.csv") == rv);

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
    CHECK(app_main_args("-t", "APBGC", "-c", "500-1600", "tests/data/example-containers.csv", "tests/data/example-paths.csv") == rv);

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
    CHECK(app_main_args("-p", "Y", "tests/data/example-containers.csv", "tests/data/example-paths.csv") == rv);

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
    CHECK(app_main_args("-p", "N", "tests/data/example-containers.csv", "tests/data/example-paths.csv") == rv);

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
    CHECK(app_main_args("-t", "C", "-p", "N", "tests/data/example-containers.csv", "tests/data/example-paths.csv") == rv);

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

TEST(my_test_empty_2)
{
    CHECK(app_main() != 0);

    CHECK_NOT_EMPTY(stderr);
}



TEST(file_one_test_1)
{
   int rv = 0;
   CHECK(app_main_args("-t", "B", FIRST_CONT_FILE, FIRST_PATH_FILE) == rv);

   const char *correct_output =
           "ID: 24045, Type: Biodegradable waste, Capacity: 240, Address: Bosonozske namesti 669, Neighbors: 4135 4477 22119 22698 23952\n"
           "ID: 24051, Type: Biodegradable waste, Capacity: 240, Address: Prazska 174, Neighbors: 6716 14376 22698 23947 23952\n"
           "ID: 24055, Type: Biodegradable waste, Capacity: 240, Address: Vzhledna 108, Neighbors: 13657 14376 22119 23947 23952\n"
           "ID: 24047, Type: Biodegradable waste, Capacity: 240, Address: K Berce 721, Neighbors: 4477 22698 23947 23952 24049\n"
           "ID: 26221, Type: Biodegradable waste, Capacity: 240, Address: K Berce 721, Neighbors: 4477 22698 23947 23952 24049\n"
           "ID: 24046, Type: Biodegradable waste, Capacity: 240, Address: K Berce 721, Neighbors: 4477 22698 23947 23952 24049\n"
           "ID: 26115, Type: Biodegradable waste, Capacity: 240, Address: 466, Neighbors: 4477 14376 22119 23947 23952\n"
           "ID: 24048, Type: Biodegradable waste, Capacity: 240, Address: 466, Neighbors: 4477 14376 22119 23947 23952\n"
           "ID: 26242, Type: Biodegradable waste, Capacity: 240, Address: Sevcenkova 572, Neighbors: 4135 22119 22698 23947 23952\n"
           "ID: 24053, Type: Biodegradable waste, Capacity: 240, Address: Sevcenkova 572, Neighbors: 4135 22119 22698 23947 23952\n"
           "ID: 24054, Type: Biodegradable waste, Capacity: 240, Address: Sevcenkova 572, Neighbors: 4135 22119 22698 23947 23952\n"
           "ID: 24049, Type: Biodegradable waste, Capacity: 240, Address: 313, Neighbors: 4477 6715 6716 10614 11800 19762 22211 22698 23947 23952 24046 24047 25733 26221\n"
           ;

   ASSERT_FILE(stdout, correct_output);
   CHECK_IS_EMPTY(stderr);
}

TEST(file_one_test_2)
{
   int rv = 0;
   CHECK(app_main_args("-s", FIRST_CONT_FILE, FIRST_PATH_FILE) == rv);

   const char *correct_output =
           "1;APBGCT;2,3,4,5,6,7,8,9,10,11\n"
           "2;APGC;1,3,4,5,6,7,8,9,10,11\n"
           "3;AGCT;1,2,5,7,8,9,10\n"
           "4;APBGC;1,2,6,7,8,11\n"
           "5;APBGC;1,2,3,8,9,10\n"
           "6;APBGCT;1,2,4,7,11\n"
           "7;APBGC;1,2,3,4,6,8,11\n"
           "8;APBGC;1,2,3,4,5,7\n"
           "9;APGC;1,2,3,5,10\n"
           "10;APGCT;1,2,3,5,9\n"
           "11;B;1,2,4,6,7\n"
   ;

   ASSERT_FILE(stdout, correct_output);
   CHECK_IS_EMPTY(stderr);
}
TEST(file_one_test_3)
{
   int rv = 0;
   CHECK(app_main_args("-t", "GC", FIRST_CONT_FILE, FIRST_PATH_FILE) == rv);

   const char *correct_output =
           "ID: 8300, Type: Clear glass, Capacity: 1550, Address: Bosonozske namesti 669, Neighbors: 4135 4477 22119 22698 23952\n"
           "ID: 8301, Type: Colored glass, Capacity: 1550, Address: Bosonozske namesti 669, Neighbors: 4135 4477 22119 22698 23952\n"
           "ID: 6298, Type: Clear glass, Capacity: 1125, Address: Prazska 79, Neighbors: 4135 14376 22119 22698 23947\n"
           "ID: 6297, Type: Colored glass, Capacity: 1125, Address: Prazska 79, Neighbors: 4135 14376 22119 22698 23947\n"
           "ID: 7678, Type: Clear glass, Capacity: 2500, Address: Prazska 702, Neighbors: 4135 14376 22698 23947 23952\n"
           "ID: 7677, Type: Colored glass, Capacity: 2500, Address: Prazska 702, Neighbors: 4135 14376 22698 23947 23952\n"
           "ID: 4477, Type: Colored glass, Capacity: 1125, Address: Prazska 174, Neighbors: 6198 6199 6715 6716 8300 8301 10440 10614 10660 11612 11613 11800 11844 14376 17490 19762 22164 22211 22698 23947 23948 23949 23952 24045 24046 24047 24048 24049 25733 26115 26221\n"
           "ID: 4478, Type: Clear glass, Capacity: 1125, Address: Prazska 174, Neighbors: 6716 14376 22698 23947 23952\n"
           "ID: 4135, Type: Colored glass, Capacity: 1125, Address: Vzhledna 108, Neighbors: 4481 4482 6297 6298 6719 6720 6723 6724 7677 7678 8300 8301 10440 10879 10882 10883 10897 10974 11612 11613 12090 12094 12095 12096 12109 12189 13657 14376 17490 18509 22119 22164 22212 23201 23947 23948 23949 23951 23952 23953 24045 24053 24054 26242\n"
           "ID: 4136, Type: Clear glass, Capacity: 1125, Address: Vzhledna 108, Neighbors: 13657 14376 22119 23947 23952\n"
           "ID: 6716, Type: Clear glass, Capacity: 1125, Address: K Berce 721, Neighbors: 4477 4478 10881 12092 12093 22698 23947 23950 23952 24049 24051\n"
           "ID: 6715, Type: Colored glass, Capacity: 1125, Address: K Berce 721, Neighbors: 4477 22698 23947 23952 24049\n"
           "ID: 6198, Type: Colored glass, Capacity: 1125, Address: 466, Neighbors: 4477 14376 22119 23947 23952\n"
           "ID: 6199, Type: Clear glass, Capacity: 1125, Address: 466, Neighbors: 4477 14376 22119 23947 23952\n"
           "ID: 4481, Type: Colored glass, Capacity: 1125, Address: Sevcenkova 572, Neighbors: 4135 22119 22698 23947 23952\n"
           "ID: 4482, Type: Clear glass, Capacity: 1125, Address: Sevcenkova 572, Neighbors: 4135 22119 22698 23947 23952\n"
           "ID: 6720, Type: Clear glass, Capacity: 1125, Address: Prima 331, Neighbors: 4135 6724 22119 23947 23952\n"
           "ID: 6719, Type: Colored glass, Capacity: 1125, Address: Prima 331, Neighbors: 4135 6724 22119 23947 23952\n"
           "ID: 6724, Type: Clear glass, Capacity: 1125, Address: Pracata 783, Neighbors: 4135 6719 6720 10897 12109 13657 18509 22119 23947 23952\n"
           "ID: 6723, Type: Colored glass, Capacity: 1125, Address: Pracata 783, Neighbors: 4135 13657 22119 23947 23952\n"
           ;

   ASSERT_FILE(stdout, correct_output);
   CHECK_IS_EMPTY(stderr);
}
TEST(file_one_test_4)
{
   int rv = 0;
   CHECK(app_main_args("-t", "BT", FIRST_CONT_FILE, FIRST_PATH_FILE) == rv);

   const char *correct_output =
           "ID: 24045, Type: Biodegradable waste, Capacity: 240, Address: Bosonozske namesti 669, Neighbors: 4135 4477 22119 22698 23952\n"
           "ID: 22164, Type: Textile, Capacity: 1000, Address: Bosonozske namesti 669, Neighbors: 4135 4477 22119 22698 23952\n"
           "ID: 22119, Type: Textile, Capacity: 1000, Address: Prazska 702, Neighbors: 4135 4136 4481 4482 6198 6199 6297 6298 6719 6720 6723 6724 8300 8301 10440 10660 10879 10882 10897 10974 11119 11612 11613 11844 12090 12094 12095 12096 12109 12189 12346 13657 14376 17490 18509 22164 22212 22698 23200 23201 23947 23948 23949 23951 23952 23953 23954 24045 24048 24053 24054 24055 26115 26242\n"
           "ID: 24051, Type: Biodegradable waste, Capacity: 240, Address: Prazska 174, Neighbors: 6716 14376 22698 23947 23952\n"
           "ID: 24055, Type: Biodegradable waste, Capacity: 240, Address: Vzhledna 108, Neighbors: 13657 14376 22119 23947 23952\n"
           "ID: 24047, Type: Biodegradable waste, Capacity: 240, Address: K Berce 721, Neighbors: 4477 22698 23947 23952 24049\n"
           "ID: 22211, Type: Textile, Capacity: 1000, Address: K Berce 721, Neighbors: 4477 22698 23947 23952 24049\n"
           "ID: 26221, Type: Biodegradable waste, Capacity: 240, Address: K Berce 721, Neighbors: 4477 22698 23947 23952 24049\n"
           "ID: 24046, Type: Biodegradable waste, Capacity: 240, Address: K Berce 721, Neighbors: 4477 22698 23947 23952 24049\n"
           "ID: 26115, Type: Biodegradable waste, Capacity: 240, Address: 466, Neighbors: 4477 14376 22119 23947 23952\n"
           "ID: 24048, Type: Biodegradable waste, Capacity: 240, Address: 466, Neighbors: 4477 14376 22119 23947 23952\n"
           "ID: 26242, Type: Biodegradable waste, Capacity: 240, Address: Sevcenkova 572, Neighbors: 4135 22119 22698 23947 23952\n"
           "ID: 24053, Type: Biodegradable waste, Capacity: 240, Address: Sevcenkova 572, Neighbors: 4135 22119 22698 23947 23952\n"
           "ID: 24054, Type: Biodegradable waste, Capacity: 240, Address: Sevcenkova 572, Neighbors: 4135 22119 22698 23947 23952\n"
           "ID: 22212, Type: Textile, Capacity: 1000, Address: Pracata 783, Neighbors: 4135 13657 22119 23947 23952\n"
           "ID: 24049, Type: Biodegradable waste, Capacity: 240, Address: 313, Neighbors: 4477 6715 6716 10614 11800 19762 22211 22698 23947 23952 24046 24047 25733 26221\n"
           ;

   ASSERT_FILE(stdout, correct_output);
   CHECK_IS_EMPTY(stderr);
}
TEST(file_one_test_5)
{
   int rv = 0;
   CHECK(app_main_args("-p", "Y", "-c", "2000-3000", FIRST_CONT_FILE, FIRST_PATH_FILE) == rv);

   const char *correct_output =
           "ID: 7678, Type: Clear glass, Capacity: 2500, Address: Prazska 702, Neighbors: 4135 14376 22698 23947 23952\n"
           "ID: 7677, Type: Colored glass, Capacity: 2500, Address: Prazska 702, Neighbors: 4135 14376 22698 23947 23952\n"
   ;

   ASSERT_FILE(stdout, correct_output);
   CHECK_IS_EMPTY(stderr);
}

TEST(file_one_test_7)
{
   int rv = 0;
   CHECK(app_main_args("-c", "1000-1099", FIRST_CONT_FILE, FIRST_PATH_FILE) == rv);

   const char *correct_output =
           "ID: 22164, Type: Textile, Capacity: 1000, Address: Bosonozske namesti 669, Neighbors: 4135 4477 22119 22698 23952\n"
           "ID: 22119, Type: Textile, Capacity: 1000, Address: Prazska 702, Neighbors: 4135 4136 4481 4482 6198 6199 6297 6298 6719 6720 6723 6724 8300 8301 10440 10660 10879 10882 10897 10974 11119 11612 11613 11844 12090 12094 12095 12096 12109 12189 12346 13657 14376 17490 18509 22164 22212 22698 23200 23201 23947 23948 23949 23951 23952 23953 23954 24045 24048 24053 24054 24055 26115 26242\n"
           "ID: 22211, Type: Textile, Capacity: 1000, Address: K Berce 721, Neighbors: 4477 22698 23947 23952 24049\n"
           "ID: 22212, Type: Textile, Capacity: 1000, Address: Pracata 783, Neighbors: 4135 13657 22119 23947 23952\n"
   ;

   ASSERT_FILE(stdout, correct_output);
   CHECK_IS_EMPTY(stderr);
}

TEST(file_one_test_8)
{
   int rv = 0;
   CHECK(app_main_args("-c", "400-999", FIRST_CONT_FILE, FIRST_PATH_FILE) == rv);

   CHECK_IS_EMPTY(stdout);
}

TEST(file_one_test_9)
{
   int rv = 0;
   CHECK(app_main_args("-t", "P", "-c", "1000-1100", "-p", "Y", FIRST_CONT_FILE, FIRST_PATH_FILE) == rv);

   const char *correct_output =
           "ID: 11613, Type: Paper, Capacity: 1100, Address: Bosonozske namesti 669, Neighbors: 4135 4477 22119 22698 23952\n"
           "ID: 17490, Type: Paper, Capacity: 1100, Address: Bosonozske namesti 669, Neighbors: 4135 4477 22119 22698 23952\n"
           "ID: 11612, Type: Paper, Capacity: 1100, Address: Bosonozske namesti 669, Neighbors: 4135 4477 22119 22698 23952\n"
           "ID: 23201, Type: Paper, Capacity: 1100, Address: Prazska 79, Neighbors: 4135 14376 22119 22698 23947\n"
           "ID: 12095, Type: Paper, Capacity: 1100, Address: Prazska 79, Neighbors: 4135 14376 22119 22698 23947\n"
           "ID: 12094, Type: Paper, Capacity: 1100, Address: Prazska 79, Neighbors: 4135 14376 22119 22698 23947\n"
           "ID: 12096, Type: Paper, Capacity: 1100, Address: Prazska 79, Neighbors: 4135 14376 22119 22698 23947\n"
           "ID: 12093, Type: Paper, Capacity: 1100, Address: Prazska 174, Neighbors: 6716 14376 22698 23947 23952\n"
           "ID: 12092, Type: Paper, Capacity: 1100, Address: Prazska 174, Neighbors: 6716 14376 22698 23947 23952\n"
           "ID: 12346, Type: Paper, Capacity: 1100, Address: Vzhledna 108, Neighbors: 13657 14376 22119 23947 23952\n"
           "ID: 23200, Type: Paper, Capacity: 1100, Address: Vzhledna 108, Neighbors: 13657 14376 22119 23947 23952\n"
           "ID: 11800, Type: Paper, Capacity: 1100, Address: K Berce 721, Neighbors: 4477 22698 23947 23952 24049\n"
           "ID: 25733, Type: Paper, Capacity: 1100, Address: K Berce 721, Neighbors: 4477 22698 23947 23952 24049\n"
           "ID: 11844, Type: Paper, Capacity: 1100, Address: 466, Neighbors: 4477 14376 22119 23947 23952\n"
           "ID: 14376, Type: Paper, Capacity: 1100, Address: Sevcenkova 572, Neighbors: 4135 4136 4477 4478 6198 6199 6297 6298 7677 7678 10660 10881 10882 10883 11119 11844 12092 12093 12094 12095 12096 12346 22119 22698 23200 23201 23947 23950 23951 23952 23954 24048 24051 24055 26115\n"
           "ID: 12189, Type: Paper, Capacity: 1100, Address: Sevcenkova 572, Neighbors: 4135 22119 22698 23947 23952\n"
           "ID: 13657, Type: Paper, Capacity: 1100, Address: Prima 331, Neighbors: 4135 4136 6723 6724 10879 11119 12090 12346 22119 22212 23200 23947 23952 23954 24055\n"
           "ID: 12109, Type: Paper, Capacity: 1100, Address: Prima 331, Neighbors: 4135 6724 22119 23947 23952\n"
           "ID: 12090, Type: Paper, Capacity: 1100, Address: Pracata 783, Neighbors: 4135 13657 22119 23947 23952\n"
           ;

   ASSERT_FILE(stdout, correct_output);
   CHECK_IS_EMPTY(stderr);
}




TEST(file_two_test_1)
{
    int rv = 0;
    CHECK(app_main_args("-t", "B", SECOND_CONT_FILE, SECOND_PATH_FILE) == rv);

    const char *correct_output =
            "ID: 24169, Type: Biodegradable waste, Capacity: 240, Address: Cerna 161, Neighbors: 8043 8044 8716 8717 10407 10546 11569 11570 11571 13376 13377 13418 21702 21703 21704 21705 22131 24013 24014 24027 24028 24029 24030 24031 24032 24033 24034 24925 24926 25098 25867 25868 25869 25870 25871 25872 25994\n"
            "ID: 25873, Type: Biodegradable waste, Capacity: 240, Address: Cerna 161, Neighbors: 8043 8717 24925 25868 25872\n"
            "ID: 25874, Type: Biodegradable waste, Capacity: 240, Address: Cerna 161, Neighbors: 8043 8717 24925 25868 25872\n"
            "ID: 25872, Type: Biodegradable waste, Capacity: 240, Address: Atriova 559, Neighbors: 8043 8044 8716 8717 10546 13376 13377 22131 24027 24028 24029 24030 24031 24032 24033 24034 24169 24925 24926 25098 25867 25868 25869 25870 25873 25874 25994 26183 26184 26185\n"
            "ID: 25871, Type: Biodegradable waste, Capacity: 240, Address: Atriova 559, Neighbors: 8043 8717 24169 24925 25868\n"
            "ID: 25868, Type: Biodegradable waste, Capacity: 240, Address: Macova 37, Neighbors: 8043 8044 8716 8717 10407 11569 11570 11571 13418 21702 21703 21704 21705 22131 24013 24014 24169 24925 24926 25869 25870 25871 25872 25873 25874 26183 26184 26185\n"
            "ID: 25867, Type: Biodegradable waste, Capacity: 240, Address: Macova 37, Neighbors: 8043 8717 24169 24925 25872\n"
            "ID: 25869, Type: Biodegradable waste, Capacity: 240, Address: Na Nivach 333, Neighbors: 8043 8717 24169 25868 25872\n"
            "ID: 25870, Type: Biodegradable waste, Capacity: 240, Address: Na Nivach 333, Neighbors: 8043 8717 24169 25868 25872\n"
    ;

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(file_two_test_2)
{
    int rv = 0;
    CHECK(app_main_args("-s", SECOND_CONT_FILE, SECOND_PATH_FILE) == rv);

    const char *correct_output =
            "1;APBC;2,3,4,5,6,7,8\n"
            "2;AP;1,3,4,7,8\n"
            "3;APBGC;1,2,4,5,6,7,8\n"
            "4;GCT;1,2,3,5,6,7,8\n"
            "5;APB;1,3,4,6,7\n"
            "6;GC;1,3,4,5,7,8\n"
            "7;APB;1,2,3,4,5,6\n"
            "8;A;1,2,3,4,6\n"
    ;

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}
TEST(file_two_test_3)
{
    int rv = 0;
    CHECK(app_main_args("-t", "GC", SECOND_CONT_FILE, SECOND_PATH_FILE) == rv);

    const char *correct_output =
            "ID: 26185, Type: Colored glass, Capacity: 1660, Address: Cerna 161, Neighbors: 8043 8717 24925 25868 25872\n"
            "ID: 21704, Type: Clear glass, Capacity: 1125, Address: Atriova 559, Neighbors: 8043 8717 24169 24925 25868\n"
            "ID: 21703, Type: Colored glass, Capacity: 1125, Address: Atriova 559, Neighbors: 8043 8717 24169 24925 25868\n"
            "ID: 21705, Type: Colored glass, Capacity: 1125, Address: Atriova 559, Neighbors: 8043 8717 24169 24925 25868\n"
            "ID: 21702, Type: Clear glass, Capacity: 1125, Address: Atriova 559, Neighbors: 8043 8717 24169 24925 25868\n"
            "ID: 8717, Type: Colored glass, Capacity: 1125, Address: Atriova 559, Neighbors: 8043 8044 10407 10546 11569 11570 11571 13376 13377 13418 21702 21703 21704 21705 24013 24014 24027 24028 24029 24030 24031 24032 24033 24034 24169 24925 24926 25098 25867 25868 25869 25870 25871 25872 25873 25874 25994 26183 26184 26185\n"
            "ID: 8716, Type: Clear glass, Capacity: 1125, Address: Atriova 559, Neighbors: 8043 24169 24925 25868 25872\n"
            "ID: 8043, Type: Colored glass, Capacity: 2500, Address: Macova 4, Neighbors: 8716 8717 10407 10546 11569 11570 11571 13418 21702 21703 21704 21705 22131 24013 24014 24027 24028 24029 24030 24031 24032 24033 24034 24169 24925 24926 25098 25867 25868 25869 25870 25871 25872 25873 25874 25994 26183 26184 26185\n"
            "ID: 8044, Type: Clear glass, Capacity: 2500, Address: Macova 4, Neighbors: 8717 24169 24925 25868 25872\n"
    ;

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}
TEST(file_two_test_4)
{
    int rv = 0;
    CHECK(app_main_args("-t", "BT", SECOND_CONT_FILE, SECOND_PATH_FILE) == rv);

    const char *correct_output =
            "ID: 24169, Type: Biodegradable waste, Capacity: 240, Address: Cerna 161, Neighbors: 8043 8044 8716 8717 10407 10546 11569 11570 11571 13376 13377 13418 21702 21703 21704 21705 22131 24013 24014 24027 24028 24029 24030 24031 24032 24033 24034 24925 24926 25098 25867 25868 25869 25870 25871 25872 25994\n"
            "ID: 25873, Type: Biodegradable waste, Capacity: 240, Address: Cerna 161, Neighbors: 8043 8717 24925 25868 25872\n"
            "ID: 25874, Type: Biodegradable waste, Capacity: 240, Address: Cerna 161, Neighbors: 8043 8717 24925 25868 25872\n"
            "ID: 25872, Type: Biodegradable waste, Capacity: 240, Address: Atriova 559, Neighbors: 8043 8044 8716 8717 10546 13376 13377 22131 24027 24028 24029 24030 24031 24032 24033 24034 24169 24925 24926 25098 25867 25868 25869 25870 25873 25874 25994 26183 26184 26185\n"
            "ID: 25871, Type: Biodegradable waste, Capacity: 240, Address: Atriova 559, Neighbors: 8043 8717 24169 24925 25868\n"
            "ID: 22131, Type: Textile, Capacity: 1000, Address: Atriova 559, Neighbors: 8043 24169 24925 25868 25872\n"
            "ID: 25868, Type: Biodegradable waste, Capacity: 240, Address: Macova 37, Neighbors: 8043 8044 8716 8717 10407 11569 11570 11571 13418 21702 21703 21704 21705 22131 24013 24014 24169 24925 24926 25869 25870 25871 25872 25873 25874 26183 26184 26185\n"
            "ID: 25867, Type: Biodegradable waste, Capacity: 240, Address: Macova 37, Neighbors: 8043 8717 24169 24925 25872\n"
            "ID: 25869, Type: Biodegradable waste, Capacity: 240, Address: Na Nivach 333, Neighbors: 8043 8717 24169 25868 25872\n"
            "ID: 25870, Type: Biodegradable waste, Capacity: 240, Address: Na Nivach 333, Neighbors: 8043 8717 24169 25868 25872\n"
            ;

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(file_two_test_6)
{
    int rv = 0;
    CHECK(app_main_args("-p", "N", SECOND_CONT_FILE, SECOND_PATH_FILE) == rv);

    CHECK_IS_EMPTY(stdout);
}

TEST(file_two_test_7)
{
    int rv = 0;
    CHECK(app_main_args("-c", "1000-2000", "-t", "T", SECOND_CONT_FILE, SECOND_PATH_FILE) == rv);

    const char *correct_output =
            "ID: 22131, Type: Textile, Capacity: 1000, Address: Atriova 559, Neighbors: 8043 24169 24925 25868 25872\n"
    ;

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(file_two_test_8)
{
    int rv = 0;
    CHECK(app_main_args("-c", "400-1000", SECOND_CONT_FILE, SECOND_PATH_FILE) == rv);

    const char *correct_output =
            "ID: 22131, Type: Textile, Capacity: 1000, Address: Atriova 559, Neighbors: 8043 24169 24925 25868 25872\n"
    ;

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(file_two_test_9)
{
    int rv = 0;
    CHECK(app_main_args("-t", "P", "-c", "1000-1100", "-p", "Y", SECOND_CONT_FILE, SECOND_PATH_FILE) == rv);

    const char *correct_output =
            "ID: 13377, Type: Paper, Capacity: 1100, Address: 454, Neighbors: 8717 24032 24169 24925 25872\n"
            "ID: 13418, Type: Paper, Capacity: 1100, Address: Atriova 559, Neighbors: 8043 8717 24169 24925 25868\n"
            "ID: 11570, Type: Paper, Capacity: 1100, Address: Atriova 559, Neighbors: 8043 8717 24169 24925 25868\n"
            "ID: 11569, Type: Paper, Capacity: 1100, Address: Atriova 559, Neighbors: 8043 8717 24169 24925 25868\n"
            "ID: 11571, Type: Paper, Capacity: 1100, Address: Atriova 559, Neighbors: 8043 8717 24169 24925 25868\n"
            "ID: 25994, Type: Paper, Capacity: 1100, Address: Macova 37, Neighbors: 8043 8717 24169 24925 25872\n"
            "ID: 24925, Type: Paper, Capacity: 1100, Address: Na Nivach 333, Neighbors: 8043 8044 8716 8717 10407 11569 11570 11571 13376 13377 13418 21702 21703 21704 21705 22131 24013 24014 24169 25098 25867 25868 25871 25872 25873 25874 25994 26183 26184 26185\n"
    ;

    ASSERT_FILE(stdout, correct_output);
    CHECK_IS_EMPTY(stderr);
}

TEST(test_error)
{
    CHECK(app_main_args(FIRST_PATH_FILE, FIRST_CONT_FILE) != 0);

    CHECK_NOT_EMPTY(stderr);
}


TEST(test_error_2)
{
    CHECK(app_main_args("-t", FIRST_CONT_FILE, FIRST_PATH_FILE) != 0);

    CHECK_NOT_EMPTY(stderr);
}

TEST(test_error_3)
{
    CHECK(app_main_args("-t", "-p", "APB", "Y", FIRST_CONT_FILE, FIRST_PATH_FILE) != 0);

    CHECK_NOT_EMPTY(stderr);
}

TEST(test_error_4)
{
    CHECK(app_main_args("-t", "AP", "-s", FIRST_CONT_FILE, FIRST_PATH_FILE) != 0);

    CHECK_NOT_EMPTY(stderr);
}

TEST(test_error_5)
{
    CHECK(app_main_args("-t", "AP", "-t", "B", FIRST_CONT_FILE, FIRST_PATH_FILE) != 0);

    CHECK_NOT_EMPTY(stderr);
}

TEST(test_error_6)
{
    CHECK(app_main_args("-t", "apT", FIRST_CONT_FILE, FIRST_PATH_FILE) != 0);

    CHECK_NOT_EMPTY(stderr);
}

TEST(test_error_7)
{
    CHECK(app_main_args("-t", "AP", FIRST_CONT_FILE, "-c", "500-1000", FIRST_PATH_FILE) != 0);

    CHECK_NOT_EMPTY(stderr);
}
TEST(test_error_8)
{
    CHECK(app_main_args("-c", "ahoj-500", FIRST_CONT_FILE, FIRST_PATH_FILE) != 0);

    CHECK_NOT_EMPTY(stderr);
}
