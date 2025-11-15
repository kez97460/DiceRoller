/**
 * @file main.c
 * @author Kezia Marcou
 * @brief Main file of the diceRoller
 * 
 */

/*---Arguments---*/
#define REQUIRED_ARGS \
        REQUIRED_STRING_ARG(dice_formula, "dice", "Dice formula")

#define BOOLEAN_ARGS \
        BOOLEAN_ARG(help, "-h", "Show help") \
        BOOLEAN_ARG(advantage, "-a", "Throw first d20 with advantage") \
        BOOLEAN_ARG(disadvantage, "-d", "Throw first d20 with disadvantage") \
        BOOLEAN_ARG(result_only, "-r", "Only print the final result")

#include "easyargs.h"
#include <stdio.h>
#include "simpleRNG.h"
#include "time.h"
#include "formulaParser.h"

int main(int argc, char *argv[])
{
    simpleRNG_init(time(NULL));

    args_t args = make_default_args();

    // Parse arguments
    if (!parse_args(argc, argv, &args) || args.help) {
        print_help(argv[0]);
        return 1;
    }

    // Use your arguments
    printf("Processing formula : %s \n", args.dice_formula);


    int32_t result = formulaParser_calculateFormula(args.dice_formula, args.advantage, args.disadvantage, args.result_only);

    printf("Final result: %d\n", result);

    return 0;
}

