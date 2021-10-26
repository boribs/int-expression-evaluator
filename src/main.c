#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cal.h"


int main() {
    int repeat;
    char again;
    char expression[100];

    printf("Ingresa una expresión:\n ");
    printf("Escribe 'q' para salir.\n\n");

    do {
        printf("> ");
        memset(expression, 0, 100);
        for (size_t i = 0; i < 100; ++i) {
            scanf("%c", &expression[strlen(expression)]);

            if (expression[strlen(expression) - 1] == '\n') {
                expression[strlen(expression) - 1] = 0;
                break;
            }
        }

        size_t len = strlen(expression);
        if (len != 0) {
            for (size_t i = 0; i < len; ++i) {
                if (expression[i] == 'q') return 0;
            }

            printf("\n");
            get_result(expression);
            printf("\n");
        }

        fflush(stdin);
    } while(true);

    return 0;
}
