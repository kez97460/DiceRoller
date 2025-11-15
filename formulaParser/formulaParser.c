#include "formulaParser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsedElements.h"
#include "simpleRNG.h"

/************************************************************************************************************
 * Macros, enums, structs, variables
 */

#define ELEMENT_BUFFER_SIZE 16 // you overflow your memory with dice elements before this buffer size is a problem

/************************************************************************************************************
 * Private functions
 */

/**
 * Get the first number that can be read in the string. Any non-digit char found ends the function
 * 
 * @param str
 */
uint32_t private_getFirstNumber(char *string)
{
    uint32_t res = 0;
    uint32_t index = 0;

    while ((string[index] >= '0') && (string[index] <= '9'))
    {
        res *= 10;
        res += string[index] - '0'; // add value of digit
        index++;
    }

    return res;
}

/**
 * Parse an element in string form into an element array
 * 
 * @param element_array_ptr 
 * @param buffer 
 */
ParsedElementError_t private_parseElementInBuffer(ParsedElementArray_t *element_array_ptr, char *buffer)
{
    // This handles dice and numbers.
    //TODO: make this return an error if the string is invalid (contains something other than 0-9 and d)
    
    // Check if string is empty
    if (strlen(buffer) == 0)
    {
        return PELEM_ERR_INVALID_INPUT;
    }
    

    char * dice_ptr = strchr(buffer, 'd');

    if (dice_ptr != NULL)
    {
        uint32_t dice_count = private_getFirstNumber(buffer);
        uint32_t dice_sides = private_getFirstNumber(dice_ptr + 1);

        if (dice_count == 0 || dice_sides == 0)
        {
            return PELEM_ERR_INVALID_INPUT;
        }

        parsedElements_arrayAppend(element_array_ptr, (ParsedElement_t) {TYPE_OPERATOR, OPERATOR_OPEN_P});
        
        for (uint32_t i = 0; i < dice_count; i++)
        {
            parsedElements_arrayAppend(element_array_ptr, (ParsedElement_t) {TYPE_DICE, dice_sides});
            parsedElements_arrayAppend(element_array_ptr, (ParsedElement_t) {TYPE_OPERATOR, OPERATOR_PLUS});
        }

        // Replace last "+" with ")"
        ParsedElementError_t status = parsedElements_arraySetElement(element_array_ptr, element_array_ptr->current_length - 1, (ParsedElement_t) {TYPE_OPERATOR, OPERATOR_CLOSE_P});
        if (status)
        {
            return status;
        }
        
    }
    else 
    {
        uint32_t number = private_getFirstNumber(buffer);

        ParsedElement_t number_element = {
            .type = TYPE_NUMBER,
            .subtype = number
        };

        parsedElements_arrayAppend(element_array_ptr, number_element);
    }

    return PELEM_OK;
}

uint32_t private_getOperatorPrecedence(Operator_t op)
{
    switch (op)
    {
    case OPERATOR_MINUS:
    case OPERATOR_PLUS:
        return 1;
        break;
    
    case OPERATOR_TIMES:
        return 2;
        break;

    case OPERATOR_OPEN_P:
        return 3;
        break;
    
    default:
        return 0;
        break;
    }

    return 0;
}

/**
 * Calculates the result of the expression of the given array that contains no dice.
 * Does not modify the given array
 * 
 * @param element_array 
 */
int32_t private_calculateExpression(ParsedElementArray_t element_array)
{
    int32_t retval = 0;
    
    // For the shunting yard algorithm
    ParsedElement_t *elements_postfix = malloc(element_array.current_length * (sizeof *elements_postfix));
    Operator_t operator_stack[128] = {0}; //TODO: dynamic allocation maybe, or document limitations
    uint32_t current_op_stack_size = 0;
    uint32_t current_postfix_size = 0;

    // Init elements_postfix array
    for (uint32_t i = 0; i < element_array.current_length; i++)
    {
        elements_postfix[i].type = TYPE_NONE;
        elements_postfix[i].subtype = 0;
    }
    
    // Transform to postfix (Shunting Yard algorithm)
    for (uint32_t i = 0; i < element_array.current_length; i++)
    {
        ElementType_t current_type = element_array.array[i].type;
        uint32_t current_subtype = element_array.array[i].subtype;

        switch (current_type)
        {
        case TYPE_NUMBER:
            elements_postfix[current_postfix_size].type = current_type;
            elements_postfix[current_postfix_size].subtype = current_subtype;
            current_postfix_size++;
            break;

        case TYPE_OPERATOR:
            if (current_subtype == OPERATOR_CLOSE_P)
            {
                while ((current_op_stack_size != 0) && (operator_stack[current_op_stack_size - 1] != OPERATOR_OPEN_P))
                {
                    elements_postfix[current_postfix_size].type = TYPE_OPERATOR;
                    elements_postfix[current_postfix_size].subtype = operator_stack[current_op_stack_size - 1];
                    current_postfix_size++;
                    current_op_stack_size--;
                }
                // pop the '(' into nothing
                current_op_stack_size--;
            }
            else 
            {
                uint32_t precedence = private_getOperatorPrecedence(current_subtype);
            
                while ((current_op_stack_size != 0) && (precedence <= private_getOperatorPrecedence(operator_stack[current_op_stack_size])))
                {
                    elements_postfix[current_postfix_size].type = TYPE_OPERATOR;
                    elements_postfix[current_postfix_size].subtype = operator_stack[current_op_stack_size - 1];
                    current_postfix_size++;
                    current_op_stack_size--;
                }
                operator_stack[current_op_stack_size] = current_subtype;
                current_op_stack_size++;
            }
            break;
        
        default:
            retval = -9999;
            goto end;
            break;
        }
        
    }

    // Process the remaining operators
    while (current_op_stack_size != 0)
    {
        elements_postfix[current_postfix_size].type = TYPE_OPERATOR;
        elements_postfix[current_postfix_size].subtype = operator_stack[current_op_stack_size - 1];
        current_postfix_size++;
        current_op_stack_size--;
    }

    // Evaluate postfix expression
    int32_t *number_stack = malloc(current_postfix_size * (sizeof *number_stack));
    uint32_t number_stack_size = 0;

    for (uint32_t i = 0; i < current_postfix_size; i++)
    {
        if (elements_postfix[i].type == TYPE_NUMBER)
        {
            number_stack[number_stack_size] = elements_postfix[i].subtype;
            number_stack_size++;
        }
        else 
        {
            if (number_stack_size < 2)
            {
                retval = -8888;
                goto end;
            }

            int32_t num1 = number_stack[number_stack_size - 1];
            number_stack_size--;
            int32_t num2 = number_stack[number_stack_size - 1];
            number_stack_size--;

            // Operator handling
            switch (elements_postfix[i].subtype)
            {
            case OPERATOR_PLUS:
                number_stack[number_stack_size] = num2 + num1;
                number_stack_size++;
                break;

            case OPERATOR_MINUS:
                number_stack[number_stack_size] = num2 - num1;
                number_stack_size++;
                break;

            case OPERATOR_TIMES:
                number_stack[number_stack_size] = num2 * num1;
                number_stack_size++;
                break;                
            
            default:
                break;
            }
        }
        
    }
    
    if (number_stack_size == 0)
    {
        retval = -7777;
        goto end;
    }
    
    retval = number_stack[0];
end:
    free(elements_postfix);
    free(number_stack);
    return retval;
}



/************************************************************************************************************
 * Public functions
 */

/**
 * Calculate the result of the dice formula with randomized rolls.
 * 
 * @param formula 
 * @param is_advantage 
 * @param is_disadvantage 
 * @param print_steps 
 */
int32_t formulaParser_calculateResult(char *formula, bool is_advantage, bool is_disadvantage, bool print_steps)
{
    // ---Transform string into array of ParsedElements

    ParsedElementArray_t parsed_formula;
    char current_element[ELEMENT_BUFFER_SIZE] = {0}; 
    uint32_t element_length = 0; // current index in the element

    parsedElements_arrayInit(&parsed_formula);

    for (uint32_t i = 0; i < strlen(formula); i++)    
    {
        if (parsedElements_charToOperator(formula[i]) == NOT_AN_OPERATOR)
        {
            current_element[element_length] = formula[i];
            element_length++;

            if (element_length >= ELEMENT_BUFFER_SIZE - 1)
            {
                return -6666;
            }
        }
        else
        {
            if (element_length != 0)
            {
                private_parseElementInBuffer(&parsed_formula, current_element);
                memset(current_element, 0, ELEMENT_BUFFER_SIZE);
                element_length = 0;
            }

            parsedElements_arrayAppend(&parsed_formula, (ParsedElement_t) {TYPE_OPERATOR, parsedElements_charToOperator(formula[i])});
        }
    }

    if (element_length != 0)
    {
        private_parseElementInBuffer(&parsed_formula, current_element);
    }

    // Print formula
    if (print_steps) 
    {
        parsedElement_printArray(parsed_formula);
        printf("\n");
    }

    // ---Roll dice to transform into numbers
    if (print_steps) {printf("---Throwing dice---\n");}

    for (uint32_t i = 0; i < parsed_formula.current_length; i++)
    {
        if (parsed_formula.array[i].type == TYPE_DICE)
        {
            uint32_t side_count = parsed_formula.array[i].subtype;
            uint32_t dice_result = 0;

            if (is_advantage && (side_count == 20))
            {
                // d20 with advantage
                uint32_t dice1 = simpleRNG_randomUint32InRange(1, side_count);
                uint32_t dice2 = simpleRNG_randomUint32InRange(1, side_count);

                dice_result = (dice1 > dice2) ? dice1 : dice2;

                if (print_steps) {printf("Throwing d20 with advantage: {%d, %d} -> >%d<\n", dice1, dice2, dice_result);}
                is_advantage = false;
            } 
            else if (is_disadvantage && (side_count == 20))
            {
                // d20 with disadvantage
                uint32_t dice1 = simpleRNG_randomUint32InRange(1, side_count);
                uint32_t dice2 = simpleRNG_randomUint32InRange(1, side_count);

                dice_result = (dice1 < dice2) ? dice1 : dice2;

                if (print_steps) {printf("Throwing d20 with disadvantage: {%d, %d} -> >%d<\n", dice1, dice2, dice_result);}
                is_disadvantage = false;
            }
            else if (side_count == 20)
            {
                // d20 logs its result to make detecting nat 1/ nat 20 easy
                dice_result = simpleRNG_randomUint32InRange(1, side_count);
                if (print_steps) {printf("Throwing d20 : >%d<\n", dice_result);}
            }
            else
            {
                dice_result = simpleRNG_randomUint32InRange(1, side_count);
            }   

            if (parsedElements_arraySetElement(&parsed_formula, i, (ParsedElement_t) {TYPE_NUMBER, dice_result}))
            {
                return -5555;
            }
        }
    }

    // Print formula
    if (print_steps) 
    {
        parsedElement_printArray(parsed_formula);
        printf("\n");
    }
    
    // ---Calculate final result
    return private_calculateExpression(parsed_formula);
}

/**
 * Calculate the result of the formula if all dice hit the max possible value.
 * 
 * @param formula 
 * @param print_steps 
 */
int32_t formulaParser_calculateMaxResult(char *formula, bool print_steps)
{
        // ---Transform string into array of ParsedElements

    ParsedElementArray_t parsed_formula;
    char current_element[ELEMENT_BUFFER_SIZE] = {0}; 
    uint32_t element_length = 0; // current index in the element

    parsedElements_arrayInit(&parsed_formula);

    for (uint32_t i = 0; i < strlen(formula); i++)    
    {
        if (parsedElements_charToOperator(formula[i]) == NOT_AN_OPERATOR)
        {
            current_element[element_length] = formula[i];
            element_length++;

            if (element_length >= ELEMENT_BUFFER_SIZE - 1)
            {
                return -6666;
            }
        }
        else
        {
            if (element_length != 0)
            {
                private_parseElementInBuffer(&parsed_formula, current_element);
                memset(current_element, 0, ELEMENT_BUFFER_SIZE);
                element_length = 0;
            }

            parsedElements_arrayAppend(&parsed_formula, (ParsedElement_t) {TYPE_OPERATOR, parsedElements_charToOperator(formula[i])});
        }
    }

    if (element_length != 0)
    {
        private_parseElementInBuffer(&parsed_formula, current_element);
    }

    // Print formula
    if (print_steps) 
    {
        parsedElement_printArray(parsed_formula);
        printf("\n");
    }

    // ---Roll dice to transform into numbers
    if (print_steps) {printf("---Throwing dice---\n");}

    for (uint32_t i = 0; i < parsed_formula.current_length; i++)
    {
        if (parsed_formula.array[i].type == TYPE_DICE)
        {
            uint32_t side_count = parsed_formula.array[i].subtype;

            if (parsedElements_arraySetElement(&parsed_formula, i, (ParsedElement_t) {TYPE_NUMBER, side_count}))
            {
                return -5555;
            }
        }
    }

    // Print formula
    if (print_steps) 
    {
        parsedElement_printArray(parsed_formula);
        printf("\n");
    }
    
    // ---Calculate final result
    return private_calculateExpression(parsed_formula);
}

/**
 * Calculate the result of the formula if all dice hit 1.
 * 
 * @param formula 
 * @param print_steps 
 */
int32_t formulaParser_calculateMinResult(char *formula, bool print_steps)
{
            // ---Transform string into array of ParsedElements

    ParsedElementArray_t parsed_formula;
    char current_element[ELEMENT_BUFFER_SIZE] = {0}; 
    uint32_t element_length = 0; // current index in the element

    parsedElements_arrayInit(&parsed_formula);

    for (uint32_t i = 0; i < strlen(formula); i++)    
    {
        if (parsedElements_charToOperator(formula[i]) == NOT_AN_OPERATOR)
        {
            current_element[element_length] = formula[i];
            element_length++;

            if (element_length >= ELEMENT_BUFFER_SIZE - 1)
            {
                return -6666;
            }
        }
        else
        {
            if (element_length != 0)
            {
                private_parseElementInBuffer(&parsed_formula, current_element);
                memset(current_element, 0, ELEMENT_BUFFER_SIZE);
                element_length = 0;
            }

            parsedElements_arrayAppend(&parsed_formula, (ParsedElement_t) {TYPE_OPERATOR, parsedElements_charToOperator(formula[i])});
        }
    }

    if (element_length != 0)
    {
        private_parseElementInBuffer(&parsed_formula, current_element);
    }

    // Print formula
    if (print_steps) 
    {
        parsedElement_printArray(parsed_formula);
        printf("\n");
    }

    // ---Roll dice to transform into numbers
    if (print_steps) {printf("---Throwing dice---\n");}

    for (uint32_t i = 0; i < parsed_formula.current_length; i++)
    {
        if (parsed_formula.array[i].type == TYPE_DICE)
        {
            if (parsedElements_arraySetElement(&parsed_formula, i, (ParsedElement_t) {TYPE_NUMBER, 1}))
            {
                return -5555;
            }
        }
    }

    // Print formula
    if (print_steps) 
    {
        parsedElement_printArray(parsed_formula);
        printf("\n");
    }
    
    // ---Calculate final result
    return private_calculateExpression(parsed_formula);
}