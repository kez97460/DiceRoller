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
#include <sys/random.h> // For getting good RNG seeds

/*******************************************
 * Function prototypes
 */

uint64_t getSeed();

/********************************************
 * Main
 */

int main(int argc, char *argv[])
{
    simpleRNG_init(getSeed());

    args_t args = make_default_args();

    // Parse arguments
    if (!parse_args(argc, argv, &args) || args.help) {
        print_help(argv[0]);
        return 1;
    }

    if (args.result_only == false)
    {
        printf("Processing formula : %s \n", args.dice_formula);
    }

    int32_t result = formulaParser_calculateFormula(args.dice_formula, args.advantage, args.disadvantage, !args.result_only);

    if (args.result_only == false)
    {
        printf("Final result: %d\n", result);
    }
    else
    {
        printf("%d", result);
    }

    return 0;
}

/********************************************************
 * Functions
 */

uint64_t getSeed()
{
    uint64_t seed;
    if (getrandom(&seed, sizeof(seed), 0) != sizeof(seed)) {
        perror("getrandom");
        return 0;
    }
    return seed;
}