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

error_id scene__add_voxel(scene_t *scene, Vector3 position, Color material,unsigned int mat_id) {
    if (scene->numCubes >= MAX_VOXELS) {
        return -1; // Error: Scene is full
    }
    if(!scene__voxel_at_position(scene,position)){
        voxel_t newVoxel = {position.x, position.y, position.z, mat_id,material};
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
        voxel_t vox = scene->voxels[i];
        // DrawCube(pos, 1.0f, 1.0f, 1.0f, scene->colormap[vox.material_id % MAX_MAT_ID]);
        DrawCube(vox.position, 1.0f, 1.0f, 1.0f, vox.material_color);
        DrawCubeWires(vox.position, 1.0f, 1.0f, 1.0f, Fade(DARKGRAY, 0.5f));
    }
    return 0;
}


// Method to check if a cube exists in the scene
collision_t scene__ray_intersect_point(scene_t *scene,Ray* ray) {
    collision_t result={false,FLT_MAX,kMAX_POINT,kZERO_POINT,kVOXEL_NONE,__UINT32_MAX__,"none"};
    for (int i = 0; i < scene->numCubes; ++i) {
        voxel_t cube=scene->voxels[i];
        RayCollision collision = GetRayCollisionBox(*ray,voxel__get_bounding_box(&cube));
        if( collision.hit ) {
            if( collision.distance < result.distance){
                result=(collision_t){collision.hit,collision.distance,collision.point,collision.normal,cube,i,"voxel"};;
            }
        }
    }
    return result;
}

#endif