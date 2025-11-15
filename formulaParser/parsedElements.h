#ifndef INC_PARSEDELEMENTS_H
#define INC_PARSEDELEMENTS_H

#include <stdint.h>

typedef enum
{
    PELEM_OK,
    PELEM_ERR_OOB,
    PELEM_ERR_INVALID_INPUT
} ParsedElementError_t;

typedef enum
{
    TYPE_NONE,
    TYPE_OPERATOR,
    TYPE_NUMBER,
    TYPE_DICE
} ElementType_t;

typedef enum 
{
    NOT_AN_OPERATOR,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_TIMES,
    OPERATOR_OPEN_P,
    OPERATOR_CLOSE_P
} Operator_t;

/*---Structs---*/

typedef struct
{
    ElementType_t type;
    uint32_t subtype; // operator type for operators, dice face count for dice, number for numbers
} ParsedElement_t;

typedef struct
{
    uint32_t max_length;
    uint32_t current_length;
    ParsedElement_t *array;
} ParsedElementArray_t;

Operator_t parsedElements_charToOperator(char c);
char parsedElements_operatorToChar(Operator_t op);

void parsedElements_arrayInit(ParsedElementArray_t *element_array_ptr);
void parsedElements_arrayResize(ParsedElementArray_t *element_array_ptr, uint32_t new_length);
void parsedElements_arrayDeInit(ParsedElementArray_t *element_array_ptr);

void parsedElements_arrayAppend(ParsedElementArray_t *element_array_ptr, ParsedElement_t element);
ParsedElementError_t parsedElements_arraySetElement(ParsedElementArray_t *element_array_ptr, uint32_t index, ParsedElement_t element);
void parsedElements_arrayClear(ParsedElementArray_t *element_array_ptr);

void parsedElements_printElement(ParsedElement_t element);
void parsedElement_printArray(ParsedElementArray_t array);

#endif /* INC_PARSEDELEMENTS_H */
