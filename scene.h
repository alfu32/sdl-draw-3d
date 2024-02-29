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
#include "raylib.h"

// Define maximum number of voxels in the scene
#define MAX_VOXELS 10000
#define MAX_MAT_ID 10000

// Structure to represent the scene
typedef struct {
    voxel_t voxels[MAX_VOXELS];
    int numCubes;
    Color colormap[MAX_MAT_ID];
} scene_t;


void scene__init(scene_t *scene) {
    scene->numCubes = 0;
    scene->colormap[0]=WHITE;
    scene->colormap[1]=RED;
    scene->colormap[2]=ORANGE;
    scene->colormap[3]=YELLOW;
    scene->colormap[4]=GREEN;
    scene->colormap[5]=BLUE;
    scene->colormap[6]=MAGENTA;
    scene->colormap[7]=PINK;
    scene->colormap[8]=BLACK;
}

error_id scene__add_voxel(scene_t *scene, int x, int y, int z, unsigned int material) {
    if (scene->numCubes >= MAX_VOXELS) {
        return -1; // Error: Scene is full
    }
    voxel_t newVoxel = {x, y, z, material};
    scene->voxels[scene->numCubes++] = newVoxel;
    return 0; // Success
}

error_id scene__remove_voxel(scene_t *scene, int x, int y, int z) {
    for (int i = 0; i < scene->numCubes; i++) {
        if (scene->voxels[i].x == x && scene->voxels[i].y == y && scene->voxels[i].z == z) {
            // Move the last voxel to the current position and decrease count
            scene->voxels[i] = scene->voxels[--scene->numCubes];
            return 0; // Success
        }
    }
    return -1; // Error: Voxel not found
}

int scene__voxel_exists(const scene_t *scene, int x, int y, int z) {
    for (int i = 0; i < scene->numCubes; i++) {
        if (scene->voxels[i].x == x && scene->voxels[i].y == y && scene->voxels[i].z == z) {
            return 1; // Voxel exists
        }
    }
    return 0; // Voxel does not exist
}

int scene__render0(scene_t *scene) {
    // Simplified: Iterate through all voxels and render them
    for (int i = 0; i < scene->numCubes; i++) {
        scene__render_voxel(scene,&(scene->voxels[i]));
    }
    return 0; // Success
}
int scene__render_voxel(scene_t *scene,voxel_t* voxel) {
        Vector3 pos = {voxel->x,voxel->y,voxel->z};
        DrawCube(pos, 1.0f, 1.0f, 1.0f, scene->colormap[voxel->material_id % MAX_MAT_ID]);
        DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, Fade(DARKGRAY, 0.5f));
    return 0; // Success
}
int scene__render(scene_t *scene, camera_t* camera) {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Simplified: Iterate through all voxels and render them
    for (int i = 0; i < scene->numCubes; i++) {
        voxel__render(&scene->voxels[i], camera);
    }
    return 0; // Success
}


// Method to check if a cube exists in the scene
intersection_result_t scene__intersects_ray(scene_t *scene,ray_t* ray) {
    intersection_result_t result={false,(vector_t){0,0,0}};
    for (int i = 0; i < scene->numCubes; ++i) {
        result = voxel__intersects_ray(&(scene->voxels[i]),ray);
        if( result.intersects ) {
            LOG_W("found intersection with voxel number %d/%d",i,scene->numCubes)
            // TODO find the closes voxel
            // use ray__distance_to_point
            return result;
        }
    }
    return result;
}

#endif