#include "simpleRNG.h"

/************************************************************************************************************
 * Private macros, typedefs and variables
 */

#define RNG_MULT_CONSTANT 2862933555777941757UL
// Other options are 3202034522624059733UL and 3935559000370003845UL 
// according to https://www.ams.org/journals/mcom/1999-68-225/S0025-5718-99-00996-5/S0025-5718-99-00996-5.pdf

#define RNG_ADD_CONSTANT 696969696969UL

/// Current RNG number, used to generate the next one. Internal to lib.
static uint64_t current_number = 0;

/************************************************************************************************************
 * Private functions
 */

static uint64_t private_getNextNumber()
{
    current_number = current_number * RNG_MULT_CONSTANT + RNG_ADD_CONSTANT;
    return current_number;
}

/************************************************************************************************************
 * Public functions
 */

 /**
  * Initialize the RNG.
  * 
  * @param seed seed used for generating numbers
  */
void simpleRNG_init(uint64_t seed)
{
    current_number = seed;
}

/**
 * Get a random 64 bit unsigned int
 */
uint64_t simpleRNG_randomUint64()
{
    return private_getNextNumber();
}

/**
 * Get a random 32 bit unsigned int
 */
uint32_t simpleRNG_randomUint32()
{
    return (uint32_t) (private_getNextNumber() >> 32);
}

/**
 * Get a random 8 bit unsigned int
 */
uint8_t simpleRNG_randomUint8()
{
    return (uint8_t) (private_getNextNumber() >> 56);
}

/**
 * Get a random 64 bit signed int
 */
int64_t simpleRNG_randomInt64()
{
    return (int64_t) (private_getNextNumber());
}

/**
 * Get a random 32 bit signed int
 */
int32_t simpleRNG_randomInt32()
{
    // The & operation is there to make sure the number fits and avoids any implementation-specific behavior (C17 standard).
    return (int32_t) ((private_getNextNumber() >> 32) & 0xFFFFFFFFUL);
}

/**
 * Get a random 8 bit signed int
 */
int8_t simpleRNG_randomInt8()
{
    // The & operation is there to make sure the number fits and avoids any implementation-specific behavior (C17 standard).
    return (int8_t) ((private_getNextNumber() >> 56) & 0xFFUL);
}

/**
 * Get a 64 bit unsigned int that is within given bounds
 * 
 * @param min_value Minimum possible return value 
 * @param max_value Maximum possible return value
 */
uint64_t simpleRNG_randomUint64InRange(uint64_t min_value, uint64_t max_value)
{
    if (max_value <= min_value)
    {
        return 0;
    }

    // Adds 1 to the % operation in order to make the max_value a possible result
    return (min_value + private_getNextNumber() % (max_value - min_value + 1));
}

/**
 * Get a 32 bit unsigned int that is within given bounds
 * 
 * @param min_value Minimum possible return value 
 * @param max_value Maximum possible return value
 */
uint32_t simpleRNG_randomUint32InRange(uint32_t min_value, uint32_t max_value)
{
    if (max_value <= min_value)
    {
        return 0;
    }

    // Adds 1 to the % operation in order to make the max_value a possible result
    return (min_value + simpleRNG_randomUint32() % (max_value - min_value + 1));
}

/**
 * Get a 8 bit unsigned int that is within given bounds
 * 
 * @param min_value Minimum possible return value 
 * @param max_value Maximum possible return value
 */
uint8_t simpleRNG_randomUint8InRange(uint8_t min_value, uint8_t max_value)
{
    if (max_value <= min_value)
    {
        return 0;
    }

    // Adds 1 to the % operation in order to make the max_value a possible result
    return (min_value + simpleRNG_randomUint8() % (max_value - min_value + 1));
}

/**
 * Get a 64 bit signed int that is within given bounds
 * 
 * @param min_value Minimum possible return value 
 * @param max_value Maximum possible return value
 */
int64_t simpleRNG_randomInt64InRange(int64_t min_value, int64_t max_value)
{
    if (max_value <= min_value)
    {
        return 0;
    }

    // Adds 1 to the % operation in order to make the max_value a possible result
    return (min_value + simpleRNG_randomInt64() % (max_value - min_value + 1));
}

/**
 * Get a 32 bit signed int that is within given bounds
 * 
 * @param min_value Minimum possible return value 
 * @param max_value Maximum possible return value
 */
int32_t simpleRNG_randomInt32InRange(int32_t min_value, int32_t max_value)
{
    if (max_value <= min_value)
    {
        return 0;
    }

    // Adds 1 to the % operation in order to make the max_value a possible result
    return (min_value + simpleRNG_randomInt32() % (max_value - min_value + 1));
}

/**
 * Get a 8 bit signed int that is within given bounds
 * 
 * @param min_value Minimum possible return value 
 * @param max_value Maximum possible return value
 */
int8_t simpleRNG_randomInt8InRange(int8_t min_value, int8_t max_value)
{
    if (max_value <= min_value)
    {
        return 0;
    }

    // Adds 1 to the % operation in order to make the max_value a possible result
    return (min_value + simpleRNG_randomInt8() % (max_value - min_value + 1));
}

/**
 * Get a random float between 0 and 1
 */
float simpleRNG_randomFloat()
{
    return ((float) simpleRNG_randomUint32() / (float) 0xFFFFFFFFU);
}

/**
 * Get a random double between 0 and 1
 */
double simpleRNG_randomDouble()
{
    return ((double) simpleRNG_randomUint64() / (double) 0xFFFFFFFFFFFFFFFFUL);
}
