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


// Helper functions for vector operations
void cross_product(const vector_t* a, const vector_t* b, vector_t* out) {
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}

void normalize(vector_t* v) {
    float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

void scale_vector(const vector_t* v, float scale, vector_t* out) {
    out->x = v->x * scale;
    out->y = v->y * scale;
    out->z = v->z * scale;
}

void add_vectors(const vector_t* a, const vector_t* b, vector_t* out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
}

#endif