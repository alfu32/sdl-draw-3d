#ifndef __VXDI_APP_VOXEL_RASTERIZERS_H__
#define __VXDI_APP_VOXEL_RASTERIZERS_H__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "vxdi-app-voxel.h"
#include "vxdi-rl-math-extras.h"
typedef int (*voxel_operator_fn)(scene_t*,Vector3,Color,unsigned int);
// Function to rasterize a line between two 3D points
/*Point3D*/int rasterizeLine(Vector3 a, Vector3 b, scene_t* scene, Color material, unsigned int material_id,voxel_operator_fn operator) {
    // Calculate the differences between points
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;
    Vector3 ab=(Vector3){dx,dy,dz};

    int cx0=min(a.x,b.x);
    int cy0=min(a.y,b.y);
    int cz0=min(a.z,b.z);
    int cx1=max(a.x,b.x);
    int cy1=max(a.y,b.y);
    int cz1=max(a.z,b.z);
    for(int x=cx0;x<=cx1;x++){
        for(int y=cy0;y<=cy1;y++){
            for(int z=cz0;z<=cz1;z++){
                Vector3 p=(Vector3){x,y,z};

                // Calculate vector from a to p
                Vector3 ap = {p.x - a.x, p.y - a.y, p.z - a.z};

                // Calculate dot product of ab and ap
                float dotABAP = ab.x * ap.x + ab.y * ap.y + ab.z * ap.z;

                // Calculate dot product of ab with itself
                float dotABAB = ab.x * ab.x + ab.y * ab.y + ab.z * ab.z;

                // Calculate t value (projection factor)
                float t = dotABAP / dotABAB;

                // Calculate the point on the line
                Vector3 p0;
                p0.x = a.x + t * ab.x;
                p0.y = a.y + t * ab.y;
                p0.z = a.z + t * ab.z;
                if(Vector3Distance(p0,p)<=0.630f){
                    operator(scene,p,material,material_id);
                }
            }
        }
    }

    return 0;
}

// Function to rasterize a rectangular plate given three points
Vector3* rasterizeSolidCube(Vector3 a, Vector3 b, scene_t* scene, Color material, unsigned int material_id,voxel_operator_fn operator) {
    // Calculate the differences between points
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;
    Vector3 ab=(Vector3){dx,dy,dz};

    int cx0=min(a.x,b.x);
    int cy0=min(a.y,b.y);
    int cz0=min(a.z,b.z);
    int cx1=max(a.x,b.x);
    int cy1=max(a.y,b.y);
    int cz1=max(a.z,b.z);
    for(int x=cx0;x<=cx1;x++){
        for(int y=cy0;y<=cy1;y++){
            for(int z=cz0;z<=cz1;z++){
                Vector3 p=(Vector3){x,y,z};
                operator(scene,p,material,material_id);
            }
        }
    }
    return 0;
}
Vector3* rasterizeHollowCube(Vector3 a, Vector3 b, scene_t* scene, Color material, unsigned int material_id,voxel_operator_fn operator) {
    // Calculate the differences between points
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;
    Vector3 ab=(Vector3){dx,dy,dz};

    int cx0=min(a.x,b.x);
    int cy0=min(a.y,b.y);
    int cz0=min(a.z,b.z);
    int cx1=max(a.x,b.x);
    int cy1=max(a.y,b.y);
    int cz1=max(a.z,b.z);
    rasterizeSolidCube((Vector3){a.x,a.y,a.z},(Vector3){a.x,b.y,b.z},scene,material,material_id,operator);
    rasterizeSolidCube((Vector3){b.x,a.y,a.z},(Vector3){b.x,b.y,b.z},scene,material,material_id,operator);

    rasterizeSolidCube((Vector3){a.x,a.y,a.z},(Vector3){b.x,a.y,b.z},scene,material,material_id,operator);
    rasterizeSolidCube((Vector3){a.x,b.y,a.z},(Vector3){b.x,b.y,b.z},scene,material,material_id,operator);

    rasterizeSolidCube((Vector3){a.x,a.y,a.z},(Vector3){b.x,b.y,a.z},scene,material,material_id,operator);
    rasterizeSolidCube((Vector3){a.x,a.y,b.z},(Vector3){b.x,b.y,b.z},scene,material,material_id,operator);
    
    return 0;
}
Vector3* rasterizeStructureCube(Vector3 a, Vector3 b, scene_t* scene, Color material, unsigned int material_id,voxel_operator_fn operator) {
    // Calculate the differences between points
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;
    Vector3 ab=(Vector3){dx,dy,dz};

    int cx0=min(a.x,b.x);
    int cy0=min(a.y,b.y);
    int cz0=min(a.z,b.z);
    int cx1=max(a.x,b.x);
    int cy1=max(a.y,b.y);
    int cz1=max(a.z,b.z);
    rasterizeSolidCube(a, (Vector3){b.x, a.y, a.z},scene,material,material_id,operator);
    rasterizeSolidCube(a, (Vector3){a.x, b.y, a.z},scene,material,material_id,operator);
    rasterizeSolidCube((Vector3){b.x, a.y, a.z}, (Vector3){b.x, b.y, a.z},scene,material,material_id,operator);
    rasterizeSolidCube((Vector3){a.x, b.y, a.z}, (Vector3){b.x, b.y, a.z},scene,material,material_id,operator);
    
    // Generate edges for opposite face
    rasterizeSolidCube((Vector3){a.x, a.y, b.z}, (Vector3){b.x, a.y, b.z},scene,material,material_id,operator);
    rasterizeSolidCube((Vector3){a.x, a.y, b.z}, (Vector3){a.x, b.y, b.z},scene,material,material_id,operator);
    rasterizeSolidCube((Vector3){b.x, a.y, b.z}, b,scene,material,material_id,operator);
    rasterizeSolidCube((Vector3){a.x, b.y, b.z}, b,scene,material,material_id,operator);
    
    // Generate edges connecting both faces
    rasterizeSolidCube(a, (Vector3){a.x, a.y, b.z},scene,material,material_id,operator);
    rasterizeSolidCube((Vector3){b.x, a.y, a.z}, (Vector3){b.x, a.y, b.z},scene,material,material_id,operator);
    rasterizeSolidCube((Vector3){a.x, b.y, a.z}, (Vector3){a.x, b.y, b.z},scene,material,material_id,operator);
    rasterizeSolidCube((Vector3){b.x, b.y, a.z}, b,scene,material,material_id,operator);
    
    return 0;
}
int add_to_map(scene_t * scene,Vector3 p,Color c,unsigned int id){
    scene__add_voxel(scene,p,c,id);
    return 0;
}
// Function to rasterize a rectangular plate given three points
Vector3* rasterizePlane(Vector3 a, Vector3 b,  Vector3 c, scene_t* scene, Color material, unsigned int material_id,voxel_operator_fn operator) {
    // Calculate the differences between points
    Vector3 ab=(Vector3){b.x-a.x,b.y-a.y,b.z-a.z};
    Vector3 ac=(Vector3){c.x-a.x,c.y-a.y,c.z-a.z};
    scene_t *s1 = (scene_t*)malloc(sizeof(scene_t));
    scene__init(s1,false,(Vector3){1,1,1});
    scene_t *s2 = (scene_t*)malloc(sizeof(scene_t));
    scene__init(s2,false,(Vector3){1,1,1});
    rasterizeLine(a,b,s1,material,material_id,add_to_map);
    for(int i=0;i<s1->numVoxels;i++){
        voxel_t vx=s1->voxels[i];
        Vector3 v=vx.position;
        Vector3 vac=(Vector3){v.x+ac.x,v.y+ac.y,v.z+ac.z};
        rasterizeLine(v,vac,s2,material,material_id,add_to_map);
    }
    for(int i=0;i<s2->numVoxels;i++){
        voxel_t vx=s2->voxels[i];
        operator(scene,vx.position,material,material_id);
    }
    free(s1);
    free(s2);

    return 0;
}
//////////////    // Function to rasterize a rectangular plate given three points
//////////////    /*Point3D*/Vector3* rasterizeRectangle(Vector3 a, Vector3 b, Vector3 c, int* numPoints) {
//////////////        // Rasterize lines between the points to form the rectangle
//////////////        int numPointsAB, numPointsBC, numPointsCA;
//////////////        /*Point3D*/Vector3* pointsAB = rasterizeLine(a, b, &numPointsAB);
//////////////        /*Point3D*/Vector3* pointsBC = rasterizeLine(b, c, &numPointsBC);
//////////////        /*Point3D*/Vector3* pointsCA = rasterizeLine(c, a, &numPointsCA);
//////////////    
//////////////        // Calculate the total number of points
//////////////        *numPoints = numPointsAB + numPointsBC + numPointsCA - 2; // Subtracting duplicates
//////////////    
//////////////        // Allocate memory for the points
//////////////        /*Point3D*/Vector3* points = (/*Point3D*/Vector3*)malloc(*numPoints * sizeof(/*Point3D*/Vector3));
//////////////        if (points == NULL) {
//////////////            fprintf(stderr, "Memory allocation failed\n");
//////////////            exit(EXIT_FAILURE);
//////////////        }
//////////////    
//////////////        // Copy points from the lines
//////////////        int index = 0;
//////////////        for (int i = 0; i < numPointsAB - 1; i++) {
//////////////            points[index++] = pointsAB[i];
//////////////        }
//////////////        for (int i = 0; i < numPointsBC - 1; i++) {
//////////////            points[index++] = pointsBC[i];
//////////////        }
//////////////        for (int i = 0; i < numPointsCA - 1; i++) {
//////////////            points[index++] = pointsCA[i];
//////////////        }
//////////////    
//////////////        // Free memory allocated for line points
//////////////        free(pointsAB);
//////////////        free(pointsBC);
//////////////        free(pointsCA);
//////////////    
//////////////        return points;
//////////////    }

/// //////////    int test_rasterizers() {
/// //////////        // Example usage
/// //////////        Vector3 a = {1.0f, 1.0f, 1.0f};
/// //////////        Vector3 b = {4.0f, 1.0f, 1.0f}; // Along x-axis
/// //////////        Vector3 c = {1.0f, 4.0f, 3.0f}; // Slope towards y and z
/// //////////    
/// //////////        int numPoints;
/// //////////        /*Point3D*/Vector3* points = rasterizeRectangle(a, b, c, &numPoints);
/// //////////    
/// //////////        printf("Rasterized Rectangle Points:\n");
/// //////////        for (int i = 0; i < numPoints; i++) {
/// //////////            printf("(%d, %d, %d)\n", points[i].x, points[i].y, points[i].z);
/// //////////        }
/// //////////    
/// //////////        free(points); // Don't forget to free allocated memory
/// //////////        return 0;
/// //////////    }


#endif