#ifndef __LIB_RASTERIZERS__
#define __LIB_RASTERIZERS__
#include <stdio.h>
#include <stdlib.h>

// Define the Vector3 struct
typedef struct {
    float x, y, z;
} Vector3;

// Define the Point3D struct
typedef struct {
    int x, y, z;
} Point3D;

// Function to rasterize a line between two 3D points
Point3D* rasterizeLine(Vector3 p1, Vector3 p2, int* numPoints) {
    // Calculate the differences between points
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float dz = p2.z - p1.z;

    // Determine the number of points needed based on the longest axis
    float length = fmaxf(fmaxf(fabsf(dx), fabsf(dy)), fabsf(dz));
    *numPoints = (int)length + 1;

    // Allocate memory for the points
    Point3D* points = (Point3D*)malloc(*numPoints * sizeof(Point3D));
    if (points == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Bresenham's line algorithm
    float stepX = dx / length;
    float stepY = dy / length;
    float stepZ = dz / length;

    float x = p1.x;
    float y = p1.y;
    float z = p1.z;

    for (int i = 0; i < *numPoints; i++) {
        points[i].x = (int)(x + 0.5f); // Round to nearest integer
        points[i].y = (int)(y + 0.5f);
        points[i].z = (int)(z + 0.5f);
        
        x += stepX;
        y += stepY;
        z += stepZ;
    }

    return points;
}

// Function to rasterize a rectangular plate given three points
Point3D* rasterizeRectangle(Vector3 a, Vector3 b, Vector3 c, int* numPoints) {
    // Rasterize lines between the points to form the rectangle
    int numPointsAB, numPointsBC, numPointsCA;
    Point3D* pointsAB = rasterizeLine(a, b, &numPointsAB);
    Point3D* pointsBC = rasterizeLine(b, c, &numPointsBC);
    Point3D* pointsCA = rasterizeLine(c, a, &numPointsCA);

    // Calculate the total number of points
    *numPoints = numPointsAB + numPointsBC + numPointsCA - 2; // Subtracting duplicates

    // Allocate memory for the points
    Point3D* points = (Point3D*)malloc(*numPoints * sizeof(Point3D));
    if (points == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Copy points from the lines
    int index = 0;
    for (int i = 0; i < numPointsAB - 1; i++) {
        points[index++] = pointsAB[i];
    }
    for (int i = 0; i < numPointsBC - 1; i++) {
        points[index++] = pointsBC[i];
    }
    for (int i = 0; i < numPointsCA - 1; i++) {
        points[index++] = pointsCA[i];
    }

    // Free memory allocated for line points
    free(pointsAB);
    free(pointsBC);
    free(pointsCA);

    return points;
}

int rasterizers_test() {
    // Example usage
    Vector3 a = {1.0f, 1.0f, 1.0f};
    Vector3 b = {4.0f, 1.0f, 1.0f}; // Along x-axis
    Vector3 c = {1.0f, 4.0f, 3.0f}; // Slope towards y and z

    int numPoints;
    Point3D* points = rasterizeRectangle(a, b, c, &numPoints);

    printf("Rasterized Rectangle Points:\n");
    for (int i = 0; i < numPoints; i++) {
        printf("(%d, %d, %d)\n", points[i].x, points[i].y, points[i].z);
    }

    free(points); // Don't forget to free allocated memory
    return 0;
}

#endif