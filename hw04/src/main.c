#include "currency.h"
#include "decimals.h"
#include "errors.h"
#include "load.h"
#include "persons.h"
#include "structures.h"

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

struct person *find_extreme(struct persons *persons, int sign)
{
    if (!persons->size)
        return NULL;
    struct person *extreme = &persons->persons[0];
    for (int i = 1; i < persons->size; ++i) {
        if (sign == -1 && (persons->persons[i].amount) < extreme->amount) {
            extreme = &persons->persons[i];
        }
        if (sign == 1 && (persons->persons[i].amount) > extreme->amount) {
            extreme = &persons->persons[i];
        }
    }
    return extreme;
}

void print_amount(int64_t amount)
{
    int64_t pre_amount = amount / decimals_to_base(RATING_DECIMALS);
    int64_t post_amount = amount % decimals_to_base(RATING_DECIMALS);

    if (post_amount == 0) {
        printf("%" PRId64, pre_amount);
    } else {
        while (post_amount % 10 == 0 || post_amount > 100) {
            post_amount /= 10;
        }
        printf("%" PRId64 ".%" PRId64, pre_amount, post_amount);
    }
}

bool all_payments_covered(struct persons *persons)
{
    for (int i = 0; i < persons->size; i++) {
        if (persons->persons[i].amount != 0) {
            return false;
        }
    }
    return true;
}


void settle_debt(struct persons *persons, struct currency_table *currency_table)
{
    while (!all_payments_covered(persons)) {
        struct person *debtor = find_extreme(persons, -1);
        struct person *creditor = find_extreme(persons, 1);

        if (debtor->amount == 0 || creditor->amount == 0) {
            fprintf(stderr, "Rounding error\n");
            break;
        }


        int64_t amount = -debtor->amount;
        if (amount > creditor->amount)
            amount = creditor->amount;

        debtor->amount += amount;
        creditor->amount -= amount;

        printf("%s (%s) -> %s (%s): ", debtor->name, debtor->id, creditor->name, creditor->id);
        print_amount(amount);
        printf(" %s\n", currency_table->main_currency);
    }
}

int main(int argc, char **argv)
{
    stack_top_frame();
    enum error_codes error_code;
    struct currency_table currency_table;
    struct persons persons;
    FILE *person_file = NULL;
    FILE *currency_file = NULL;
    FILE *payment_file = NULL;
    memset(&currency_table, 0, sizeof(currency_table));
    memset(&persons, 0, sizeof(persons));

    if ((error_code = read_error_point())) {
        object_destroy(&currency_table);
        object_destroy(&persons);
        if (person_file != NULL) {
            fclose(person_file);
        }
        if (currency_file != NULL) {
            fclose(currency_file);
        }
        if (payment_file != NULL) {
            fclose(payment_file);
        }
        if (error_code != SUCCESS)
            print_error_message(error_code);

        return return_code(error_code);
    }
    OP(argc == 4, INVALID_ARGUMENTS);

    init_currency_table(&currency_table);
    init_persons(&persons);

    if ((person_file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Unable to open persons file!\n");
        return 1;
    }
    load_persons(&persons, person_file);
    fclose(person_file);
    person_file = NULL;

    if ((currency_file = fopen(argv[2], "r")) == NULL) {
        fprintf(stderr, "Unable to open file with currency!\n");
        return 1;
    }
    load_currency_table(&currency_table, currency_file);
    fclose(currency_file);
    currency_file = NULL;

    if ((payment_file = fopen(argv[3], "r")) == NULL) {
        fprintf(stderr, "Unable to open file with payments!\n");
        return 1;
    }
    load_payments(&persons, &currency_table, payment_file);
    fclose(payment_file);
    payment_file = NULL;

    settle_debt(&persons, &currency_table);

    exit_success();
}
