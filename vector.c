#include <malloc.h>
#include <memory.h>
#include "vector.h"

struct vector* vector_init(int elem_sizeof) {
    struct vector *vec = calloc(sizeof(*vec), 1);
    vec->elem_sizeof = elem_sizeof;

    return vec;
}

int vector_push_back(struct vector *vec, void *new_elem) {
    if (vec->size == vec->capacity) {
        void *tmp = realloc(vec->buf, vec->elem_sizeof * (2 * vec->capacity + 1));
        if (!tmp) {
            return 1;
        }
        vec->capacity = 2 * vec->capacity + 1;
        vec->buf = tmp;
    }
    memcpy(vec->buf + vec->elem_sizeof * vec->size++, new_elem, vec->elem_sizeof);
    return 0;
}

int vector_get(struct vector *vec, int index, void **elem) {
    *elem = vec->buf + vec->elem_sizeof * index;
    return 0;
}

int vector_clear(struct vector *vec) {
    vec->size = 0;
    return 0;
}

void vector_free(struct vector *vec) {
    if (vec) {
        free(vec->buf);
        vec->buf = NULL;
    }
    free(vec);
    vec = NULL;
}
