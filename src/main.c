#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cal.h"

static int is_valid_yes(char c) {
    return c == 'y' || c == 'Y' || c == 's' || c == 'S';
}
static int is_valid_no(char c) {
    return c == 'n' || c == 'N';
}

static int is_valid_char(char c) {
    return is_valid_yes(c) || is_valid_no(c);
}

int main() {
    int repeat;
    char again;
    char expression[100];

    do {
        repeat = 0;
        memset(expression, 0, 100);
        printf("Ingresa una expresión:\n> ");
        for (size_t i = 0; i < 100; ++i) {
            scanf("%c", &expression[strlen(expression)]);

            if (expression[strlen(expression) - 1] == '\n') {
                expression[strlen(expression) - 1] = 0;
                break;
            }
        }

        get_result(expression);
        printf("¿Desea ingresar otra? (s/n): ");
        do {
            scanf("%c", &again);
        } while(!is_valid_char(again));

        if (is_valid_yes(again)) repeat = 1;
        fflush(stdin);
    } while(repeat == 1);
}
