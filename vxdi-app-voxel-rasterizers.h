#ifndef __VXDI_APP_VOXEL_RASTERIZERS_H__
#define __VXDI_APP_VOXEL_RASTERIZERS_H__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "vxdi-app-voxel.h"
#include "vxdi-rl-math-extras.h"

// Function to rasterize a line between two 3D points
/*Point3D*/int rasterizeLine(Vector3 p1, Vector3 p2, scene_t* scene, Color material, unsigned int mat_id) {
    // Calculate the differences between points
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float dz = p2.z - p1.z;

    // Determine the number of points needed based on the longest axis
    int length = max(max(abs((int)dx), abs((int)dy)), abs((int)dz));
    int numPoints = length + 1;

    // Bresenham's line algorithm
    float stepX = dx / length;
    float stepY = dy / length;
    float stepZ = dz / length;

    float x = p1.x;
    float y = p1.y;
    float z = p1.z;

    for (int i = 0; i < numPoints; i++) {
        
        x += stepX;
        y += stepY;
        z += stepZ;
        scene__add_voxel(scene,(Vector3){x,y,z},material,mat_id);
    }

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