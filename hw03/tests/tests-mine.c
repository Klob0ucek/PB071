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

// TEST(my_test_1)
// {
//     int rv = 0; /* return value of main()*/
//     CHECK(app_main_args("-s" "data/Brno-BosonohyContainers.csv" "data/Brno-BosonohyPaths.csv") == rv);

//     /* TIP: Use ‹app_main()› to test the program without arguments. */

//     const char *correct_output =
//             "1;APBGCT;2,3,4,5,6,7,8,9,10,11\n"
//             "2;APGC;1,3,4,5,6,7,8,9,10,11\n"
//             "3;AGCT;1,2,5,7,8,9,10\n"
//             "4;APBGC;1,2,6,7,8,11\n"
//             "5;APBGC;1,2,3,8,9,10\n"
//             "6;APBGCT;1,2,4,7,11\n"
//             "7;APBGC;1,2,3,4,6,8,11\n"
//             "8;APBGC;1,2,3,4,5,7\n"
//             "9;APGC;1,2,3,5,10\n"
//             "10;APGCT;1,2,3,5,9\n"
//             "11;B;1,2,4,6,7"
//             ;

//     ASSERT_FILE(stdout, correct_output);
//     CHECK_IS_EMPTY(stderr);
// }

// TEST(my_test_2)
// {
//     int rv = 0; /* return value of main()*/
//     CHECK(app_main_args("-t" "BT" "-c" "500-2000" "-p" "Y" "data/Brno-BosonohyContainers.csv" "data/Brno-BosonohyPaths.csv") == rv);

//     const char *correct_output =
//             "ID: 22164, Type: Textile, Capacity: 1000, Address: Bosonozske namesti 669, Neighbors: 4135 4477 22119 22698 23952\n"
//             "ID: 22119, Type: Textile, Capacity: 1000, Address: Prazska 702, Neighbors: 4135 4136 4481 4482 6198 6199 6297 6298 6719 6720 6723 6724 8300 8301 10440 10660 10879 10882 10897 10974 11119 11612 11613 11844 12090 12094 12095 12096 12109 12189 12346 13657 14376 17490 18509 22164 22212 22698 23200 23201 23947 23948 23949 23951 23952 23953 23954 24045 24048 24053 24054 24055 26115 26242\n"
//             "ID: 22211, Type: Textile, Capacity: 1000, Address: K Berce 721, Neighbors: 4477 22698 23947 23952 24049\n"
//             "ID: 22212, Type: Textile, Capacity: 1000, Address: Pracata 783, Neighbors: 4135 13657 22119 23947 23952"
//     ;

//     ASSERT_FILE(stdout, correct_output);
//     CHECK_IS_EMPTY(stderr);
// }

TEST(my_test_empty)
{
    CHECK(app_main_args("") != 0);

    CHECK_NOT_EMPTY(stderr);
}
