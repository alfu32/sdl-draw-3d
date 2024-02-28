#ifndef RAY_H
#define RAY_H
#include <math.h>
#include "vector.h"

typedef struct ray_s {
    vector_t origin;
    vector_t through_point;
} ray_t;


GLfloat ray__distance_to_point(ray_t *self,vector_t *point){
    // Vector from the eye to the target
    vector_t direction = {
        self->origin.x - point->x,
        self->origin.y - point->y,
        self->origin.z - point->z
    };
    return sqrtf(direction.x * direction.x + direction.z * direction.z + direction.y * direction.y);

}
#endif