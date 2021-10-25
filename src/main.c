#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cal.h"


int main() {
    int repeat;
    char again;
    char expression[100];

    printf("Escribe 'q' para salir.\n\n");

    do {
        memset(expression, 0, 100);
        printf("Ingresa una expresión:\n> ");
        for (size_t i = 0; i < 100; ++i) {
            scanf("%c", &expression[strlen(expression)]);

            if (expression[strlen(expression) - 1] == '\n') {
                expression[strlen(expression) - 1] = 0;
                break;
            }
        }

        if (strlen(expression) != 0) {
            for (size_t i = 0; i < strlen(expression); ++i) {
                if (expression[i] == 'q') return 0;
            }

            get_result(expression);
        }

        fflush(stdin);
    } while(true);

    return 0;
}
