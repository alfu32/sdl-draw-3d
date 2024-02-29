#ifndef VOXEL_H
#define VOXEL_H

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "liblog.h"
#include "camera.h"
#include "ray.h"

#define VOXEL_SIZE 50

typedef struct voxel_s {
    float x, y, z;       // Position of the left bottom front vertex
    unsigned int material_id; // Material ID for future use
} voxel_t;


// Method to allocate memory for a voxel
voxel_t * voxel__alloc() {
    voxel_t* voxel = (voxel_t*)malloc(sizeof(voxel_t));
    if (voxel == NULL) {
        return NULL;
    }
    return voxel;
}

// Method to initialize a voxel
error_id voxel__init(voxel_t *voxel, int x, int y, int z, unsigned int material_id) {
    if (voxel == NULL) {
        return -1;
    }
    voxel->x = x;
    voxel->y = y;
    voxel->z = z;
    voxel->material_id = material_id;
    return 0;
}

void voxel__render0(voxel_t* self) {
    
    // Calculate cube size
    float size = 1.0f; // Assuming unit cube size for simplicity

    // Calculate other vertices based on the size
    float x2 = self->x + size;
    float y2 = self->y + size;
    float z2 = self->z - size; // Assuming OpenGL's right-hand coordinate system

    // Draw the cube
    glBegin(GL_QUADS);
    
    // Front face
    glColor3f(1.0f, 0.0f, 0.0f); // Red, material_id can be used to change color
    glVertex3f(self->x, self->y, self->z);
    glVertex3f(x2, self->y, self->z);
    glVertex3f(x2, y2, self->z);
    glVertex3f(self->x, y2, self->z);

    // Right face
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(x2, self->y, self->z);
    glVertex3f(x2, self->y, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y2, self->z);

    // Back face
    glColor3f(0.5f, 0.0f, 0.0f); // dark red
    glVertex3f(x2, self->y, z2);
    glVertex3f(self->x, self->y, z2);
    glVertex3f(self->x, y2, z2);
    glVertex3f(x2, y2, z2);

    // Left face
    glColor3f(0.0f, 0.5f, 0.0f); // dark Green
    glVertex3f(self->x, self->y, z2);
    glVertex3f(self->x, self->y, self->z);
    glVertex3f(self->x, y2, self->z);
    glVertex3f(self->x, y2, z2);

    // Top face
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(self->x, y2, self->z);
    glVertex3f(x2, y2, self->z);
    glVertex3f(x2, y2, z2);
    glVertex3f(self->x, y2, z2);

    // Bottom face
    glColor3f(0.0f, 0.0f, 0.5f); // dark Blue
    glVertex3f(self->x, self->y, self->z);
    glVertex3f(x2, self->y, self->z);
    glVertex3f(x2, self->y, z2);
    glVertex3f(self->x, self->y, z2);

    glEnd();
}
void voxel__render(voxel_t* self, camera_t* camera) {
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    camera__setup_projection(camera);
    voxel__render0(self);
}

// Method to check if a voxel intersects with a ray
intersection_result_t voxel__intersects_ray(voxel_t *voxel, ray_t *ray) {
    intersection_result_t result={false,(vector_t){voxel->x,voxel->y,voxel->z}};
    // Direction vector of the ray
    vector_t ray_direction = {
        ray->through_point.x - ray->origin.x,
        ray->through_point.y - ray->origin.y,
        ray->through_point.z - ray->origin.z
    };
    
    // Normalize the direction
    float dir_magnitude = sqrt(ray_direction.x * ray_direction.x + ray_direction.y * ray_direction.y + ray_direction.z * ray_direction.z);
    ray_direction.x /= dir_magnitude;
    ray_direction.y /= dir_magnitude;
    ray_direction.z /= dir_magnitude;
    
    // Calculate the vector from the ray's origin to the voxel's anchor
    vector_t to_voxel = {
        voxel->x - ray->origin.x,
        voxel->y - ray->origin.y,
        voxel->z - ray->origin.z
    };
    
    // Project this vector onto the ray's direction vector
    float projection_length = to_voxel.x * ray_direction.x + to_voxel.y * ray_direction.y + to_voxel.z * ray_direction.z;
    vector_t closest_point_on_ray = {
        ray->origin.x + ray_direction.x * projection_length,
        ray->origin.y + ray_direction.y * projection_length,
        ray->origin.z + ray_direction.z * projection_length
    };
    
    // Check if the closest point is within the voxel's bounds
    float halfSize = 0.5; // Assuming the voxel is a unit cube for simplicity
    result.intersects = closest_point_on_ray.x >= (voxel->x - halfSize) && closest_point_on_ray.x <= (voxel->x + halfSize) &&
                      closest_point_on_ray.y >= (voxel->y - halfSize) && closest_point_on_ray.y <= (voxel->y + halfSize) &&
                      closest_point_on_ray.z >= (voxel->z - halfSize) && closest_point_on_ray.z <= (voxel->z + halfSize);
    
    return result;
}
// Method to free memory allocated for a voxel
int voxel__free(voxel_t *voxel) {
    if (voxel == NULL) {
        return -1;
    }
    free(voxel);
    return 0;
}


#endif /* VOXEL_H */