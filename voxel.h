#ifndef VOXEL_H
#define VOXEL_H

#define VOXEL_SIZE 1.0f
#define VOXEL_SIZE_2 0.5f

typedef struct voxel_s {
    Vector3 position;       // Position of the left bottom front vertex
    unsigned int material_id; // Material ID for future use
    Color material_color; // Material ID for future use
} voxel_t;

BoundingBox voxel__get_bounding_box(voxel_t* self){
    return (BoundingBox){
        (Vector3){self->position.x-0.5f,self->position.y-0.5f,self->position.z-0.5f},
        (Vector3){self->position.x+0.5f,self->position.y+0.5f,self->position.z+0.5f}
    };
}

#endif /* VOXEL_H */