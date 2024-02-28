#ifndef SCENE_H
#define SCENE_H

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "lib.h"
#include "liblog.h"
#include "camera.h"
#include "voxel.h"

// Define maximum number of voxels in the scene
#define MAX_CUBES 100

// Structure to represent the scene
typedef struct {
    voxel_t voxels[MAX_CUBES];
    int numCubes;
} scene_t;


// implementation scene
void scene__init(scene_t *scene) {
    scene->numCubes = 0;
}

// Method to add a cube to the scene
error_id scene__add_voxel(scene_t *scene, int x, int y, int z, unsigned int material) {
    if (scene->numCubes < MAX_CUBES) {
        error_id result = voxel__init(&(scene->voxels[scene->numCubes]), x, y, z, material);
        if (result == 0) {
            scene->numCubes++;
        }
        return result;
    }
    return -1; // or any other appropriate error code
}

// Method to remove a cube from the scene
error_id scene__remove_voxel(scene_t *scene, int x, int y, int z) {
    for (int i = 0; i < scene->numCubes; ++i) {
        if (scene->voxels[i].x == x && scene->voxels[i].y == y && scene->voxels[i].z == z) {
            // Shift remaining voxels to fill the gap
            for (int j = i; j < scene->numCubes - 1; ++j) {
                scene->voxels[j] = scene->voxels[j + 1];
            }
            scene->numCubes--;
            return 0;
        }
    }
    return -1; // or any other appropriate error code
}

// Method to check if a cube exists in the scene
int scene__voxel_exists(const scene_t *scene, int x, int y, int z) {
    for (int i = 0; i < scene->numCubes; ++i) {
        if (scene->voxels[i].x == x && scene->voxels[i].y == y && scene->voxels[i].z == z) {
            return 1;
        }
    }
    return 0;
}
// Method to check if a cube exists in the scene
int scene__render(scene_t *scene,camera_t* camera) {
    for (int i = 0; i < scene->numCubes; ++i) {
        voxel__render(&(scene->voxels[i]),camera);
    }
    return 0;
}
// Method to check if a cube exists in the scene
intersection_result_t scene__intersects_ray(const scene_t *scene,ray_t* ray) {
    intersection_result_t result={false,(vector_t){0,0,0}};
    for (int i = 0; i < scene->numCubes; ++i) {
        result = voxel__intersects_ray((voxel_t*)&(scene->voxels[i]),ray);
        if( result.intersects ) {
            // TODO find the closes voxel
            // use ray__distance_to_point
            return result;
        }
    }
    return result;
}
#endif