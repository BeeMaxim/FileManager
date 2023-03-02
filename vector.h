#pragma once

struct vector {
    void *buf;
    int elem_sizeof;
    int capacity;
    int size;
};

struct vector * vector_init(int elem_sizeof);

int vector_push_back(struct vector *vec, void *new_elem);

int vector_get(struct vector *vec, int index, void **elem);

int vector_clear(struct vector *vec);

void vector_free(struct vector *vec);
