#ifndef FSTACK_H
#define FSTACK_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct { // fixed length stack
    long * elements;
    size_t len;
    size_t max_len;
} FStack;

static FStack new_fstack(size_t max_len) {
    return (FStack) {
        .elements = (long *)malloc(sizeof(long) * max_len),
        .len = 0,
        .max_len = max_len,
    };
}

static bool push(FStack *s, long value) {
    if (s->len >= s->max_len) return false;

    s->elements[s->len] = value;
    s->len++;
    return true;
}

static bool pop(FStack *s, long *dest) {
    if (s->len == 0) return false;

    *dest = s->elements[--(s->len)];
    return true;
}

static void free_stack(FStack *s) {
    free(s->elements);
    s->len = 0;
}

static void show(FStack *s, char *a) {
    printf("%s:", a);
    for (size_t i = 0; i < s->len; ++i) {
        printf("%ld ", s->elements[i]);
    }
    printf("\n");
}

#endif
