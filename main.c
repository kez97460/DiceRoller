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
        BOOLEAN_ARG(result_only, "-r", "Only print the final result") \
        BOOLEAN_ARG(statistics, "-s", "Get statistics instead of throwing dice once") \

#define OPTIONAL_ARGS \
        OPTIONAL_UINT_ARG(samples, 100000, "--samples", "sample_count", "Amount of samples for statistics, if active") \

#include "easyargs.h"
#include <stdio.h>
#include "simpleRNG.h"
#include "time.h"
#include "formulaParser.h"
#include <sys/random.h> // For getting good RNG seeds
#include <math.h>

/*******************************************
 * Function prototypes
 */

uint64_t getSeed();
float meanOfPercentArray(float *array, uint32_t array_length, int32_t min_result);
float stdDeviationOfPercentArray(float *array, uint32_t array_length, int32_t min_result);

/********************************************
 * Main
 */

int main(int argc, char *argv[])
{
    args_t args = make_default_args();
    // Parse arguments
    if (!parse_args(argc, argv, &args) || args.help) {
        print_help(argv[0]);
        return 1;
    }

    simpleRNG_init(getSeed());

    // Handle statistics here, then return
    if (args.statistics)
    {
        if (args.result_only == false)
        {
            printf("Processing formula : %s \n", args.dice_formula);
        }

        int32_t min_result = formulaParser_calculateMinResult(args.dice_formula, false);
        int32_t max_result = formulaParser_calculateMaxResult(args.dice_formula, false);
        if (min_result >= max_result)
        {
            printf("Min result higher or equal to max result for this formula. \nStatistics will not run.\n");
            return 1;
        }

        uint32_t result_array_size = max_result - min_result + 1;
        uint32_t *result_array = malloc(result_array_size * (sizeof *result_array));
        for (uint32_t i = 0; i < result_array_size; i++)
        {
            result_array[i] = 0;
        }

        for (uint32_t i = 0; i < args.samples; i++)
        {
            int32_t res = formulaParser_calculateResult(args.dice_formula, args.advantage, args.disadvantage, false);
            int32_t index = res - min_result;

            result_array[index]++;
        }

        float *percent_array = malloc(result_array_size * (sizeof *percent_array));

        // Calculate % based on results
        for (uint32_t i = 0; i < result_array_size; i++)
        {
            percent_array[i] = ((float) result_array[i]) / ((float) args.samples);
        }

        // Get stats
        float mean = meanOfPercentArray(percent_array, result_array_size, min_result);
        float std_deviation = stdDeviationOfPercentArray(percent_array, result_array_size, min_result);

        // Print results and statistics
        if (args.result_only == false)
        {
            printf("--- Raw results ---\n");
            for (uint32_t i = 0; i < result_array_size; i++)
            {
                printf("[%d: %.0f%%] ", (i + min_result), (100 * percent_array[i]));
            }
            printf("\n\n");

            printf("--- Statistics ---\n");
            printf("min: %d, max: %d\n", min_result, max_result);
            printf("mean: %.2f\n", mean);
            printf("std deviation: %.2f\n\n", std_deviation);
        }



        return 0;
    }
    

    if (args.result_only == false)
    {
        printf("Processing formula : %s \n", args.dice_formula);
    }
    int32_t result = formulaParser_calculateResult(args.dice_formula, args.advantage, args.disadvantage, !args.result_only);

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

float meanOfPercentArray(float *array, uint32_t array_length, int32_t min_result)
{
    float res = 0;
    for (uint32_t i = 0; i < array_length; i++)
    {
        res = res + array[i] * (min_result + i);
    }
    return res;
}

float stdDeviationOfPercentArray(float *array, uint32_t array_length, int32_t min_result)
{
    float mean = meanOfPercentArray(array, array_length, min_result);
    float res = 0;

    for (uint32_t i = 0; i < array_length; i++)
    {
        res = res + array[i] * ((i + min_result) - mean) * ((i + min_result) - mean); // difference squared
    }
    
    res = sqrtf(res);
    return res;
}