#ifndef CAL_H
#define CAL_H

#include "fstack.h"
#include <stdbool.h>


// This calculator wont process single user input above 10,000.
#define MAX_MIN_VAL 10000
#define MAX_MIN_VAL_STR "10000"
#define MAX_MIN_VAL_STR_LEN 6

#define MAX_TOKEN_LEN 30

enum CState { // Calculator State
    STATE_OK,                               // 0
    STATE_DIVISION_BY_ZERO,                 // 1
    STATE_UNREACHABLE_OPERATOR,             // 2
    STATE_MISSING_VALUE,                    // 3
    STATE_MISSING_OPERATOR,                 // 4
    STATE_PRESENT_VALUE,                    // 5
    STATE_PRESENT_OPERATOR,                 // 6
    STATE_INVALID_CHAR_ERROR,               // 7
    STATE_TOKEN_PARSE_ERROR,                // 8
    STATE_TOO_MANY_ELEMENTS_IN_STACK_ERROR, // 9
    STATE_VERY_FEW_ELEMENTS_IN_STACK_ERROR, // 10
};

enum Operator {  // each Operator is of type `int`
    OP_PLUS,     // 0
    OP_MINUS,    // 1
    OP_PRODUCT,  // 2
    OP_DIVISION, // 3
    OP_INVALID,  // 4
};

bool parse_str_to_int(char *str, long *dest);
enum CState operate(FStack *numbers, FStack *operators, long *result);
enum CState evaluate(char *s, long *val);

#endif
