#ifndef INC_FORMULAPARSER_H
#define INC_FORMULAPARSER_H

#include <stdint.h>
#include <stdbool.h>

int32_t formulaParser_calculateResult(char *formula, bool is_advantage, bool is_disadvantage, bool print_steps);
int32_t formulaParser_calculateMaxResult(char *formula, bool print_steps);
int32_t formulaParser_calculateMinResult(char *formula, bool print_steps);

#endif /* INC_FORMULAPARSER_H */
