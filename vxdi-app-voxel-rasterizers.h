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
/*Point3D*/
/**
 * @brief generates a stream of 3d points on a segment given the start and the end of the segment
 * @param Vector3 a start of the segment
 * @param Vector3 b end of the segment
 * @param scene_t scene* target scene pointer
 * @param Color material current color
 * @param int  material_id ( unsigned ) current material id 
 * @param voxel_operator_fn calback function for each stream generated point
 */
int rasterizeLine(Vector3 a, Vector3 b, scene_t* scene, Color material, unsigned int material_id,voxel_operator_fn operator) {
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
/**
 * @brief Function to rasterize a rectangular plate given three points
 *        generates a stream of points and calls the callback function with
 *        each of them
 */
Vector3* rasterizeSolidCube(Vector3 a, Vector3 b, scene_t* scene, Color material, unsigned int material_id,voxel_operator_fn operator) {

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
/**
 * @brief Generates a stream of 3D points on a circle given the center point and a second point on the circle.
 * @param Vector3 a Center point
 * @param Vector3 b Second point (on the circle)
 * @param scene_t* scene Target scene pointer
 * @param Color material Current color
 * @param unsigned int material_id Current material ID
 * @param voxel_operator_fn operator Callback function for each generated point
 * @return int Status code (0 on success)
 */
int rasterizeCircle(Vector3 a, Vector3 b, scene_t* scene, Color material, unsigned int material_id, voxel_operator_fn operator) {
    
    int cx0 = a.x;
    //int cy0 = a.y;
    int cz0 = a.z;
    int cx1 = b.x;
    //int cy1 = b.y;
    int cz1 = b.z;

    // Calculate the radius using the 3D distance between points a and b
    double r = sqrt((double)((cx0 - cx1) * (cx0 - cx1) + (cz0 - cz1) * (cz0 - cz1)));
    
    // Check if the radius is valid to avoid division by zero
    if (r == 0) {
        r = 1.0; // Invalid radius
    }

    // Resolution based on arc length increment
    double resolution = asin(1.0 / r);

    // Loop through angles and generate points
    for (double omega = 0; omega < 2 * PI; omega += resolution) {
        // Calculate relative x and z based on circle parametric equation
        int x = (int)(r * cos(omega)) + cx0;
        int z = (int)(r * sin(omega)) + cz0;
        int y = a.y; // Assuming y stays constant, adjust if necessary

        Vector3 p = (Vector3){x, y, z};  // Generated point
        operator(scene, p, material, material_id);  // Apply operator callback
    }
    
    return 0; // Success
}

Vector3* rasterizeHollowCube(Vector3 a, Vector3 b, scene_t* scene, Color material, unsigned int material_id,voxel_operator_fn operator) {

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

/**
 * @brief Generates a stream of 3D points on a hollow sphere given the center point and a second point on the sphere.
 * The function will generate points along the surface of the sphere.
 * 
 * @param Vector3 a Center point
 * @param Vector3 b Second point (on the sphere)
 * @param scene_t* scene Target scene pointer
 * @param Color material Current color
 * @param unsigned int material_id Current material ID
 * @param voxel_operator_fn operator Callback function for each generated point
 * @return int Status code (0 on success)
 */
int rasterizeSphere(Vector3 a, Vector3 b, scene_t* scene, Color material, unsigned int material_id, voxel_operator_fn operator) {
    
    // Calculate radius based on the distance between the center (a) and point b
    double r = sqrt((double)((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z)));

    // Step angles for spherical coordinates
    double theta_step = PI / 60;   // Increment for polar angle
    double phi_step = 2 * PI / 60; // Increment for azimuthal angle
    
    // Generate points on the sphere surface using spherical coordinates
    for (double theta = 0; theta < PI; theta += theta_step) { // Polar angle [0, π]
        for (double phi = 0; phi < 2 * PI; phi += phi_step) {  // Azimuthal angle [0, 2π]
            int x = (int)(r * sin(theta) * cos(phi)) + a.x;
            int y = (int)(r * cos(theta)) + a.y;
            int z = (int)(r * sin(theta) * sin(phi)) + a.z;

            Vector3 p = (Vector3){x, y, z};  // Generated point
            operator(scene, p, material, material_id);  // Apply operator callback
        }
    }
    
    return 0; // Success
}


/**
 * @brief Generates a stream of 3D points to represent a spherical cloud, 
 * where the points are randomly distributed on the surface of the sphere and occupy about 30% of the surface area.
 * 
 * @param Vector3 a Center point
 * @param Vector3 b Second point (on the sphere)
 * @param scene_t* scene Target scene pointer
 * @param Color material Current color
 * @param unsigned int material_id Current material ID
 * @param voxel_operator_fn operator Callback function for each generated point
 * @return int Status code (0 on success)
 */
int rasterizeSphereCloud_random(Vector3 a, Vector3 b, scene_t* scene, Color material, unsigned int material_id, voxel_operator_fn operator) {

    // Calculate radius based on the distance between the center (a) and point b
    double r = sqrt((double)((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z)));

    // Surface area of the sphere
    double surface_area = 4 * PI * r * r;

    // Define the percentage of the total points to generate (30%)
    double density = 0.1;

    // Calculate total number of points based on surface area divided by the density
    int total_points = (int)(surface_area * density);
    if(total_points>10000) {
        total_points = 10000;
    }

    // Use a random seed to generate random points
    srand(time(NULL));  // Seed the random number generator

    for (int i = 0; i < total_points; ++i) {
        // Random spherical coordinates using a constant radius
        double theta = ((double)rand() / RAND_MAX) * PI;          // Random polar angle [0, π]
        double phi = ((double)rand() / RAND_MAX) * 2 * PI;        // Random azimuthal angle [0, 2π]

        // Calculate the Cartesian coordinates
        int x = (int)(r * sin(theta) * cos(phi)) + a.x;
        int y = (int)(r * cos(theta)) + a.y;
        int z = (int)(r * sin(theta) * sin(phi)) + a.z;

        Vector3 p = (Vector3){x, y, z};  // Generated point
        operator(scene, p, material, material_id);  // Apply operator callback
    }

    return 0; // Success
}

/**
 * @brief Generates a stream of 3D points to represent a deterministic spherical cloud using Fibonacci lattice distribution.
 * The points are deterministically distributed on the surface of the sphere at about 30% density.
 * 
 * @param Vector3 a Center point
 * @param Vector3 b Second point (on the sphere)
 * @param scene_t* scene Target scene pointer
 * @param Color material Current color
 * @param unsigned int material_id Current material ID
 * @param voxel_operator_fn operator Callback function for each generated point
 * @return int Status code (0 on success)
 */
int rasterizeSphereCloud_fibbonacci(Vector3 a, Vector3 b, scene_t* scene, Color material, unsigned int material_id, voxel_operator_fn operator) {

    // Calculate radius based on the distance between the center (a) and point b
    double r = sqrt((double)((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z)));

    // Surface area of the sphere
    double surface_area = 4 * PI * r * r;

    // Define the percentage of the total points to generate (30%)
    double density = 0.3;

    // Calculate total number of points based on surface area and density
    int total_points = (int)(surface_area * density);

    // The golden ratio, commonly used for even distribution
    const double golden_ratio = (1.0 + sqrt(5.0)) / 2.0;

    // Loop through and generate points using Fibonacci lattice formula
    for (int i = 0; i < total_points; ++i) {
        double theta = acos(1 - 2 * (double)i / total_points);  // Polar angle
        double phi = 2 * PI * i / golden_ratio;  // Azimuthal angle

        // Calculate Cartesian coordinates from spherical coordinates
        int x = (int)(r * sin(theta) * cos(phi)) + a.x;
        int y = (int)(r * cos(theta)) + a.y;
        int z = (int)(r * sin(theta) * sin(phi)) + a.z;

        Vector3 p = (Vector3){x, y, z};  // Generated point
        operator(scene, p, material, material_id);  // Apply operator callback
    }

    return 0; // Success
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