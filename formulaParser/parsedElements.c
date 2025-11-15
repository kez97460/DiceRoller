#include "parsedElements.h"

#include <stdlib.h>
#include <stdio.h>

/************************************************************************************************************
 * Macros, enums, structs, variables
 */

#define DEFAULT_ELEMENT_ARRAY_SIZE 8

/************************************************************************************************************
 * Private functions
 */


/************************************************************************************************************
 * Public functions
 */

/**
 * Get the operator that corresponds to the given char
 * 
 * @param c 
 */
Operator_t parsedElements_charToOperator(char c)
{
    switch (c)
    {
    case '+':
        return OPERATOR_PLUS;
        break;
    
    case '-':
        return OPERATOR_MINUS;
        break;

    case '*':
        return OPERATOR_TIMES;
        break;

    case '(':
        return OPERATOR_OPEN_P;
        break;

    case ')':
        return OPERATOR_CLOSE_P;
        break;

    default:
        return NOT_AN_OPERATOR;
        break;
    }

    return NOT_AN_OPERATOR;
}

/**
 * Get the char that corresponds to the given operator
 * 
 * @param op 
 */
char parsedElements_operatorToChar(Operator_t op)
{
    switch (op)
    {
    case OPERATOR_PLUS:
        return '+';
        break;
    
    case OPERATOR_MINUS:
        return '-';
        break;

    case OPERATOR_TIMES:
        return '*';
        break;

    case OPERATOR_OPEN_P:
        return '(';
        break;

    case OPERATOR_CLOSE_P:
        return ')';
        break;

    default:
        return '\0';
        break;
    }

    return '\0';
}

/**
 * Initialize a parsed element array (allocate memory)
 * 
 * @param element_array_ptr
 */
void parsedElements_arrayInit(ParsedElementArray_t *element_array_ptr)
{
    element_array_ptr->array = malloc(DEFAULT_ELEMENT_ARRAY_SIZE * (sizeof *(element_array_ptr->array)));
    element_array_ptr->max_length = DEFAULT_ELEMENT_ARRAY_SIZE;
    element_array_ptr->current_length = 0;
    
    for (uint32_t i = 0; i < DEFAULT_ELEMENT_ARRAY_SIZE; i++)
    {
        element_array_ptr->array[i].type = TYPE_NONE;
        element_array_ptr->array[i].subtype = 0;
    }
}

/**
 * Modify the max number of elements that can be stored in an array (re-allocate memory)
 * 
 * @param element_array_ptr 
 * @param new_length 
 */
void parsedElements_arrayResize(ParsedElementArray_t *element_array_ptr, uint32_t new_length)
{
    uint32_t old_length = element_array_ptr->max_length;

    element_array_ptr->array = realloc(element_array_ptr->array, new_length * (sizeof *element_array_ptr->array));
    element_array_ptr->max_length = new_length;
    
    if (old_length < new_length)
    {
        for (uint32_t i = old_length; i < new_length; i++)
        {
            element_array_ptr->array[i].type = TYPE_NONE;
            element_array_ptr->array[i].subtype = 0;
        }
    }

    element_array_ptr->max_length = new_length;
}

/**
 * De-initialize an element array (free the memory)
 * 
 * @param element_array_ptr 
 */
void parsedElements_arrayDeInit(ParsedElementArray_t *element_array_ptr)
{
    free(element_array_ptr->array);
    element_array_ptr->max_length = 0;
    element_array_ptr->current_length = 0;
}

/**
 * Append a new element at the end of the array, extending its max length if needed
 * 
 * @param element_array_ptr 
 * @param element 
 */
void parsedElements_arrayAppend(ParsedElementArray_t *element_array_ptr, ParsedElement_t element)
{
    if (element_array_ptr->current_length >= element_array_ptr->max_length - 1)
    {
        parsedElements_arrayResize(element_array_ptr, element_array_ptr->max_length + DEFAULT_ELEMENT_ARRAY_SIZE);
    }

    element_array_ptr->array[element_array_ptr->current_length] = element;
    element_array_ptr->current_length++;
}


/**
 * Set the element at the given index in the array with the given one.
 * 
 * 
 * @param element_array_ptr 
 * @param index (must be lower than the current length)
 * @param element 
 */
ParsedElementError_t parsedElements_arraySetElement(ParsedElementArray_t *element_array_ptr, uint32_t index, ParsedElement_t element)
{
    if (index >= element_array_ptr->current_length)
    {
        return PELEM_ERR_OOB;
    }

    element_array_ptr->array[index] = element;
}

/**
 * Remove every element in the array
 * 
 * @param element_array_ptr 
 */
void parsedElements_arrayClear(ParsedElementArray_t *element_array_ptr)
{
    for (uint32_t i = 0; i < element_array_ptr->current_length; i++)
    {
        static ParsedElement_t empty_element = {
            .type = TYPE_NONE,
            .subtype = 0
        };

        parsedElements_arraySetElement(element_array_ptr, i, empty_element);
    }

    element_array_ptr->current_length = 0;
}

/**
 * Print an element
 * 
 * @param element 
 */
void parsedElements_printElement(ParsedElement_t element)
{
    switch (element.type)
    {
    case TYPE_DICE:
        printf("d%d ", element.subtype);
        break;
    
    case TYPE_NUMBER:
        printf("%d ", element.subtype);
        break;

    case TYPE_OPERATOR:
        printf("%c ", parsedElements_operatorToChar(element.subtype));
        break;
    
    default:
        break;
    }
}

/**
 * Print an element array
 * 
 * @param array 
 */
void parsedElement_printArray(ParsedElementArray_t array)
{
    for (uint32_t i = 0; i < array.current_length; i++)
    {
        parsedElements_printElement(array.array[i]);
    }
}