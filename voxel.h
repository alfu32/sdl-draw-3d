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

void voxel__render(voxel_t* self, camera_t* camera) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    camera__setup_projection(camera);
    
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

// Method to check if a voxel intersects with a ray
intersection_result_t voxel__intersects_ray(voxel_t *voxel, ray_t *ray) {
    intersection_result_t result={false,(vector_t){voxel->x,voxel->y,voxel->z}};

    // Calculate the minimum and maximum bounds of the voxel
    int voxelMinX = voxel->x * 50;
    int voxelMinY = voxel->y * 50;
    int voxelMinZ = voxel->z * 50;
    int voxelMaxX = voxelMinX + 50;
    int voxelMaxY = voxelMinY + 50;
    int voxelMaxZ = voxelMinZ + 50;

    // Calculate the direction of the ray
    int dirX = ray->through_point.x - ray->origin.x;
    int dirY = ray->through_point.y - ray->origin.y;
    int dirZ = ray->through_point.z - ray->origin.z;

    // Calculate the origin of the ray relative to the voxel
    int relOriginX = ray->origin.x * 50 - voxelMinX;
    int relOriginY = ray->origin.y * 50 - voxelMinY;
    int relOriginZ = ray->origin.z * 50 - voxelMinZ;

    // Initialize t values for the ray intersections with the voxel faces
    double tmin, tmax, tymin, tymax, tzmin, tzmax;

    // Calculate t values for the x faces
    double divX = 1.0 / dirX;
    if (divX >= 0) {
        tmin = (voxelMinX - relOriginX) * divX;
        tmax = (voxelMaxX - relOriginX) * divX;
    } else {
        tmin = (voxelMaxX - relOriginX) * divX;
        tmax = (voxelMinX - relOriginX) * divX;
    }

    // Calculate t values for the y faces
    double divY = 1.0 / dirY;
    if (divY >= 0) {
        tymin = (voxelMinY - relOriginY) * divY;
        tymax = (voxelMaxY - relOriginY) * divY;
    } else {
        tymin = (voxelMaxY - relOriginY) * divY;
        tymax = (voxelMinY - relOriginY) * divY;
    }

    // Check if the ray misses the voxel
    if ((tmin > tymax) || (tymin > tmax)) {
        return result;
    }
    if (tymin > tmin) {
        tmin = tymin;
    }
    if (tymax < tmax) {
        tmax = tymax;
    }

    // Calculate t values for the z faces
    double divZ = 1.0 / dirZ;
    if (divZ >= 0) {
        tzmin = (voxelMinZ - relOriginZ) * divZ;
        tzmax = (voxelMaxZ - relOriginZ) * divZ;
    } else {
        tzmin = (voxelMaxZ - relOriginZ) * divZ;
        tzmax = (voxelMinZ - relOriginZ) * divZ;
    }

    // Check if the ray misses the voxel
    if ((tmin > tzmax) || (tzmin > tmax)) {
        return result;
    }
    if (tzmin > tmin) {
        tmin = tzmin;
    }
    if (tzmax < tmax) {
        tmax = tzmax;
    }

    // Check if the ray intersects the voxel
    result.intersects=true;
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