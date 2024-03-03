
#ifndef VOXEL_H
#define VOXEL_H
#include <float.h>
#define VOXEL_SIZE 1.0f
#define VOXEL_SIZE_2 0.5f



typedef struct voxel_s {
    Vector3 position;       // Position of the left bottom front vertex
    unsigned int material_id; // Material ID for future use
    Color material_color; // Material ID for future use
} voxel_t;

typedef enum collision_hit_ {
    COLLISION_HIT_NONE=0x110,
    COLLISION_HIT_VOXEL,
    COLLISION_HIT_PLANE,
    COLLISION_HIT_GUIDE,
} collision_hit_e;

typedef struct collision_s {
    
    bool hit;               // Did the ray hit something?
    float distance;         // Distance to the nearest hit
    Vector3 point;          // Point of the nearest hit
    Vector3 normal;         // Surface normal of hit
    voxel_t voxel;
    int voxel_index;
    collision_hit_e collision_hit;
    
} collision_t;

const Vector3 kMAX_POINT={FLT_MAX,FLT_MAX,FLT_MAX,};
const Vector3 kZERO_POINT={0,0,0,};
const voxel_t kVOXEL_NONE={0,0,0,-1,BLACK};


BoundingBox voxel__get_bounding_box(voxel_t* self){
    return (BoundingBox){
        (Vector3){self->position.x-0.5f,self->position.y-0.5f,self->position.z-0.5f},
        (Vector3){self->position.x+0.5f,self->position.y+0.5f,self->position.z+0.5f}
    };
}

#endif /* VOXEL_H */