#ifndef __SCENE_H__
#define __SCENE_H__

#include <stdbool.h>
#include <stdlib.h>
#include <float.h>
#include "lib.h"
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


int scene__voxel_at_position(const scene_t *scene, Vector3 position) {
    for (int i = 0; i < scene->numCubes; i++) {
        if (Vector3DistanceSqr(scene->voxels[i].position,position)<0.75) {
            return 1; // Voxel exists
        }
    }
    return 0; // Voxel does not exist
}

error_id scene__add_voxel(scene_t *scene, Vector3 position, unsigned int material) {
    if (scene->numCubes >= MAX_VOXELS) {
        return -1; // Error: Scene is full
    }
    if(!scene__voxel_at_position(scene,position)){
        voxel_t newVoxel = {position.x, position.y, position.z, material};
        scene->voxels[scene->numCubes++] = newVoxel;
    }
    return 0; // Success
}

error_id scene__remove_voxel(scene_t *scene, Vector3 position) {
    for (int i = 0; i < scene->numCubes; i++) {
        float dist = Vector3DistanceSqr(scene->voxels[i].position,position);
        printf("voxel %d is at %3.2f\n",i,dist);
        if ( dist<0.75) {
            // Move the last voxel to the current position and decrease count
            scene->voxels[i] = scene->voxels[--scene->numCubes];
            return 0; // Success
        }
    }
    return -1; // Error: Voxel not found
}



int scene__render_voxel(scene_t *scene,voxel_t* voxel) {
    return 0; 
}

int scene__render(scene_t *scene) {
    // Simplified: Iterate through all voxels and render them
    for (int i = 0; i < scene->numCubes; i++) {
        //Vector3 pos = Vector3Add(scene->voxels[i].position,(Vector3){-0.5f, -0.5f, -0.5f});
        Vector3 pos = scene->voxels[i].position;
        DrawCube(pos, 1.0f, 1.0f, 1.0f, scene->colormap[scene->voxels[i].material_id % MAX_MAT_ID]);
        DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, Fade(DARKGRAY, 0.5f));
    }
    return 0;
}


// Method to check if a cube exists in the scene
RayCollision scene__ray_intersect_point(scene_t *scene,Ray* ray) {
    RayCollision result={false,FLT_MAX,(Vector3){0},(Vector3){0}};
    for (int i = 0; i < scene->numCubes; ++i) {
        voxel_t cube=scene->voxels[i];
        RayCollision r = GetRayCollisionBox(*ray,voxel__get_bounding_box(&cube));
        if( r.hit ) {
            if( r.distance < result.distance){
                result=r;
            }
        }
    }
    return result;
}

#endif