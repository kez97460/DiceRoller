/**
 * @file simpleRandom.h
 * @author Kezia Marcou
 * @brief Simple implementation of a pseudo-random number generator.
 * Uses a linear congruential generator (mod 2^64)
 * 
 * Dependencies :
 * - stdint.h (8, 32 and 64 bit types, both signed and unsigned)
 * 
 */

#ifndef INC_SIMPLERNG_H
#define INC_SIMPLERNG_H

#include <stdint.h>

void simpleRNG_init(uint64_t seed);

uint64_t simpleRNG_randomUint64();
uint32_t simpleRNG_randomUint32();
uint8_t simpleRNG_randomUint8();
int64_t simpleRNG_randomInt64();
int32_t simpleRNG_randomInt32();
int8_t simpleRNG_randomInt8();

uint64_t simpleRNG_randomUint64InRange(uint64_t min_value, uint64_t max_value);
uint32_t simpleRNG_randomUint32InRange(uint32_t min_value, uint32_t max_value);
uint8_t simpleRNG_randomUint8InRange(uint8_t min_value, uint8_t max_value);
int64_t simpleRNG_randomInt64InRange(int64_t min_value, int64_t max_value);
int32_t simpleRNG_randomInt32InRange(int32_t min_value, int32_t max_value);
int8_t simpleRNG_randomInt8InRange(int8_t min_value, int8_t max_value);

float simpleRNG_randomFloat();
double simpleRNG_randomDouble();


#endif /* INC_SIMPLERNG_H */
