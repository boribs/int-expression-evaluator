#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cal.h"
#include "fstack.h"

// macros for error handling
#define CHECK_STATE(expr)   {enum CState state = expr; \
                            if (state != STATE_OK) return state;}

#define PUSH(s, val)   if (!push(s, val)) return STATE_TOO_MANY_ELEMENTS_IN_STACK_ERROR;
#define PUSH_N(n, o, val_ptr)    CHECK_STATE(handle_negative(n, o, val_ptr)); \
                                 PUSH(n, *val_ptr);

#define HANDLE_TOKEN(token, n, o)   CHECK_STATE(handle_token(token, n, o))
#define HANDLE_OPERATOR(n, o)       CHECK_STATE(handle_operator(n, o))


static long to_digit(char c) {
    return (long)(c - '0');
}

bool parse_str_to_int(char *str, long *dest) {
    if (str == NULL) return false;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_MIN_VAL_STR_LEN || (len == MAX_MIN_VAL_STR_LEN && strcmp(MAX_MIN_VAL_STR, str) != 0))
        return false;

    len--;
    long m = 10;
    *dest = str[len] - '0'; // set first number

    for (long i = 1; i < (long)len + 1; ++i) {
        *dest += to_digit(str[len - i]) * m;
        m *= 10;
    }

    return true;
}

int find_matching_parenthesis(char *str) {
    // str should point to first (opening) parenthesis

    if (str == NULL || str[0] != '(') return -1;
    int count = 1;

    for (size_t i = 1; i < strlen(str); ++i) {
        if (str[i] == '(') count++;
        if (str[i] == ')') count--;

        if (count == 0) return i;
    }

    return (count == 0) ? 1 : -1;
}

enum CState operate(FStack *numbers, FStack *operators, long *result) {
    long first, second, op;

    if (!pop(numbers, &second)) return STATE_MISSING_VALUE;
    if (!pop(numbers, &first)) return STATE_MISSING_VALUE;
    if(!pop(operators, &op)) return STATE_MISSING_OPERATOR;

    switch((enum Operator)op) {
        case OP_PLUS:
            *result = first + second;
            break;
        case OP_MINUS:
            *result = first - second;
            break;
        case OP_PRODUCT:
            *result = first * second;
            break;
        case OP_DIVISION:
            if (second == 0) return STATE_DIVISION_BY_ZERO;
            *result = (long)(first / second);
            break;
        default:
            return STATE_UNREACHABLE_OPERATOR;
    }

    return STATE_OK;
}

static int get_precedence(enum Operator o) {
    return (o == OP_PLUS || o == OP_MINUS) ? 1 : 2;
}

static bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

static enum Operator parse_operator(char c) {
    switch (c) {
        case '+': return OP_PLUS;
        case '-': return OP_MINUS;
        case '*': return OP_PRODUCT;
        case '/': return OP_DIVISION;
        default:  return OP_INVALID;
    }
}

static enum CState handle_operator(FStack *n, FStack *o) {
    long d;
    CHECK_STATE(operate(n, o, &d));
    push(n, d);

    return STATE_OK;
}

static enum CState handle_negative(FStack *n, FStack *o, long *d) {
    // This should be called before pushing to the number stack

    if (o->len != 0 && o->elements[o->len - 1] == OP_MINUS) {
        *d *= -1;
        o->len--;

        if (n->len > o->len) {
            if (get_precedence(o->elements[o->len - 1]) > get_precedence(OP_PLUS)) {
                HANDLE_OPERATOR(n, o);
            }

            PUSH(o, OP_PLUS);
        }
    }

    return STATE_OK;
}

static enum CState handle_token(char *token, FStack *n, FStack *o) {
    long d, op;

    if (strlen(token) != 0) {
        if (!parse_str_to_int(token, &d)) return STATE_TOKEN_PARSE_ERROR;

        PUSH_N(n, o, &d);
        memset(token, 0, MAX_TOKEN_LEN);
    }

    return STATE_OK;
}

enum CState evaluate(char *s, size_t len, long *result) {
    FStack operator_stack = new_fstack(3);
    FStack number_stack = new_fstack(3);

    char token[MAX_TOKEN_LEN] = {0};

    for (size_t i = 0; i < len; ++i) {
        char current = s[i];

        if (isdigit(current)) {
            token[strlen(token)] = current;

        } else if (is_operator(current)) {
            HANDLE_TOKEN(token, &number_stack, &operator_stack);

            enum Operator op = parse_operator(current);
            long current_p = get_precedence(op);

            if (operator_stack.len == 0) {
                PUSH(&operator_stack, op);
            } else if (operator_stack.len == 1) {
                long last_op = operator_stack.elements[operator_stack.len - 1];
                long last_p = get_precedence(last_op);

                if (last_p >= current_p && op != OP_MINUS) {
                    HANDLE_OPERATOR(&number_stack, &operator_stack);
                }
                PUSH(&operator_stack, op);
            } else {
                if (operator_stack.len < number_stack.len) {
                    HANDLE_OPERATOR(&number_stack, &operator_stack);
                }
                PUSH(&operator_stack, op);
            }

        } else if (current == ' ') {
            HANDLE_TOKEN(token, &number_stack, &operator_stack);

        } else if (current == '(') {
            HANDLE_TOKEN(token, &number_stack, &operator_stack);

            int m_index = find_matching_parenthesis(&s[i]);
            long d;

            CHECK_STATE(evaluate(&s[i + 1], m_index - 1, &d));

            if (number_stack.len != 0 && operator_stack.len == 0) {
                PUSH(&operator_stack, (long)OP_PRODUCT);
            }

            PUSH_N(&number_stack, &operator_stack, &d);

            if (operator_stack.len < number_stack.len && number_stack.len > 1) {
                HANDLE_OPERATOR(&number_stack, &operator_stack);
            }

            i += m_index;

        } else {
            *result = (long)i;
            return STATE_INVALID_CHAR_ERROR;
        }
    }

    HANDLE_TOKEN(token, &number_stack, &operator_stack);

    while (operator_stack.len != 0) {
        HANDLE_OPERATOR(&number_stack, &operator_stack);
    }

    if (number_stack.len != 1) {
        return STATE_PRESENT_VALUE;
    }

    *result = number_stack.elements[0];
    free_stack(&number_stack);
    free_stack(&operator_stack);

    return STATE_OK;
}

void get_result(char *expression) {
    long result;
    enum CState s = evaluate(expression, strlen(expression), &result);

    switch (s) {
        case STATE_OK:
            printf("%s = %ld\n", expression, result);
            break;
        case STATE_DIVISION_BY_ZERO:
            printf("Error: Hay una división entre 0.\n");break;
        case STATE_MISSING_VALUE:
        case STATE_PRESENT_OPERATOR:
            printf("Error: Tienes muchos operadores y pocos números.\n");break;
        case STATE_MISSING_OPERATOR:
        case STATE_PRESENT_VALUE:
            printf("Error: Tienes muchos números y pocos operadores.\n");break;
        case STATE_INVALID_CHAR_ERROR:
            printf("\nCaracter inválido `%c` en la posición %ld.\n\n", expression[result], result);
            printf("%s\n", expression);
            for (long i = 0; i < result; ++i) printf(" ");
            printf("^\n");
            break;
        default:
            printf("Hubo un error fatal. Informa al creador.\n");break;
    }
}
