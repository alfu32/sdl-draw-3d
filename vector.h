#ifndef VECTOR_H_
#define VECTOR_H_
#include "lib.h"

typedef struct vector_s {
    GLfloat x, y, z;
} vector_t;

typedef struct intersection_result_s {
    BOOL intersects;
    vector_t point;
} intersection_result_t;

#endif