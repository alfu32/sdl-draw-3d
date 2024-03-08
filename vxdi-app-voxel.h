
#ifndef __VXDI_APP_VOXEL_H__
#define __VXDI_APP_VOXEL_H__
#include <float.h>
#include <raylib.h>
#define VOXEL_SIZE 1.0f
#define VOXEL_SIZE_2 0.5f



typedef struct voxel_s {
    Vector3 position;       // Position of the left bottom front vertex
    unsigned int material_id; // Material ID for future use
    Color material_color; // Material ID for future use
} voxel_t;

typedef enum collision_hit_ {
    COLLISION_HIT_NONE=0x110,
    COLLISION_HIT_VOXEL,
    COLLISION_HIT_PLANE,
    COLLISION_HIT_GUIDE,
} collision_hit_e;

typedef struct collision_s {
    
    bool hit;               // Did the ray hit something?
    float distance;         // Distance to the nearest hit
    Vector3 point;          // Point of the nearest hit
    Vector3 normal;         // Surface normal of hit
    voxel_t voxel;
    int voxel_index;
    collision_hit_e collision_hit;
    
} collision_t;

const Vector3 kMAX_POINT={FLT_MAX,FLT_MAX,FLT_MAX,};
const Vector3 kZERO_POINT={0,0,0,};
const voxel_t kVOXEL_NONE={0,0,0,-1,BLACK};


BoundingBox voxel__get_bounding_box(voxel_t* self){
    return (BoundingBox){
        (Vector3){self->position.x-0.5f,self->position.y-0.5f,self->position.z-0.5f},
        (Vector3){self->position.x+0.5f,self->position.y+0.5f,self->position.z+0.5f}
    };
}


// Function to shade a color based on its brightness and the light angle
Color ShadeColor(Color color, Vector3 lightDir) {
    // Compute the angle between the light direction and the normal of the face (assuming up is the normal direction)
    float angle = acosf(Vector3DotProduct(lightDir, (Vector3){0.0f, 1.0f, 0.0f}));

    // Compute the shading factor based on the angle
    float shadingFactor = (1.0f - (angle / PI)) * 0.5f; // Range from 0.0f to 0.5f

    // Adjust the brightness of the color based on the shading factor
    float brightness = (float)color.r / 255.0f; // Assuming the color components are in the range [0, 255]
    brightness += shadingFactor;

    // Clamp brightness value to [0.0, 1.0]
    brightness = Clamp(brightness, 0.0f, 1.0f);

    // Compute the shaded color
    return ColorBrightness(color,brightness);
}

// Function to draw a shaded voxel based on directional light
void voxel__draw_shaded(voxel_t *voxel, Vector3 light_direction) {
    // Define cube size
    float size = 0.5f; // Half of the total size to make the cube size 1 in all directions

    // Define vertices for each face of the cube
    // Define vertices for each face of the cube
    Vector3 vertices[] = {
        // Front face
        { voxel->position.x - size, voxel->position.y - size, voxel->position.z + size },
        { voxel->position.x + size, voxel->position.y - size, voxel->position.z + size },
        { voxel->position.x - size, voxel->position.y + size, voxel->position.z + size },
        { voxel->position.x + size, voxel->position.y + size, voxel->position.z + size },
        // Back face
        { voxel->position.x - size, voxel->position.y - size, voxel->position.z - size },
        { voxel->position.x - size, voxel->position.y + size, voxel->position.z - size },
        { voxel->position.x + size, voxel->position.y - size, voxel->position.z - size },
        { voxel->position.x + size, voxel->position.y + size, voxel->position.z - size },
        // Top face
        { voxel->position.x - size, voxel->position.y + size, voxel->position.z + size },
        { voxel->position.x + size, voxel->position.y + size, voxel->position.z + size },
        { voxel->position.x - size, voxel->position.y + size, voxel->position.z - size },
        { voxel->position.x + size, voxel->position.y + size, voxel->position.z - size },
        // Bottom face
        { voxel->position.x - size, voxel->position.y - size, voxel->position.z - size },
        { voxel->position.x + size, voxel->position.y - size, voxel->position.z - size },
        { voxel->position.x - size, voxel->position.y - size, voxel->position.z + size },
        { voxel->position.x + size, voxel->position.y - size, voxel->position.z + size },
        // Left face
        { voxel->position.x - size, voxel->position.y - size, voxel->position.z - size },
        { voxel->position.x - size, voxel->position.y - size, voxel->position.z + size },
        { voxel->position.x - size, voxel->position.y + size, voxel->position.z - size },
        { voxel->position.x - size, voxel->position.y + size, voxel->position.z + size },
        // Right face
        { voxel->position.x + size, voxel->position.y - size, voxel->position.z - size },
        { voxel->position.x + size, voxel->position.y + size, voxel->position.z - size },
        { voxel->position.x + size, voxel->position.y - size, voxel->position.z + size },
        { voxel->position.x + size, voxel->position.y + size, voxel->position.z + size }
   };


    // Draw the cube using triangle strips with shaded colors
        DrawTriangleStrip3D(&vertices[0], 4, voxel->material_color);// Front face
        DrawTriangleStrip3D(&vertices[4], 4, voxel->material_color);// Back face
        DrawTriangleStrip3D(&vertices[8], 4, voxel->material_color);// Top face
        DrawTriangleStrip3D(&vertices[12], 4, voxel->material_color);// Bottom face
        DrawTriangleStrip3D(&vertices[16], 4, voxel->material_color);// Left face
        DrawTriangleStrip3D(&vertices[20], 4, voxel->material_color);// Right face
    // Draw each face of the cube with shaded colors
}
// Function to draw a plane with specified vertices and color
void voxel__draw_plane(Vector3 vertices[], Color color) {
    // Draw the plane using a triangle strip
    DrawTriangleStrip3D(vertices, 4, color);
}

#endif /* VOXEL_H */