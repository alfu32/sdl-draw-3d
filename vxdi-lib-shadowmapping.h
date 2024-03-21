#ifndef __VXDI_LIB_SHAODWMAPPING_H__
#define __VXDI_LIB_SHAODWMAPPING_H__

#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>


typedef struct shadow_mapper_s {
    // Global variables
    Camera3D shadowCamera; // Camera from the light's perspective
    RenderTexture2D shadowMap;
    Shader depthShader, shadowShader;
    Matrix lightProjection, lightView;
    int shadowMapWidth;
    int shadowMapHeight;
    float size;
    Vector3 direction;
} shadow_mapper_t;


// Function declarations
int shadowmapping_test_main();
void InitShadowMapping(shadow_mapper_t* m);
void SetLight(shadow_mapper_t* m,Camera3D light,BoundingBox bounds);
void BeginShadowPass(shadow_mapper_t* m);
void EndShadowPass(shadow_mapper_t* m);
void BeginScenePass(shadow_mapper_t* m);
void EndScenePass(shadow_mapper_t* m);
void RenderModelWithShadows(shadow_mapper_t* m,Camera3D* camera, Shader* shader) ;


// Function to draw a rectangle plane with position and normal
void draw_rectangle_plane(Vector3 position, Vector3 normal, float width, float height, Color color) {
    // Calculate the tangent vector perpendicular to the normal and the up vector (y-axis)
    Vector3 tangent = Vector3RotateByAxisAngle(Vector3CrossProduct(normal, (Vector3){0, 1, 0}),normal,45*DEG2RAD);

    // Normalize tangent vector
    tangent = Vector3Normalize(tangent);

    // Calculate the bitangent vector perpendicular to the normal and tangent
    Vector3 bitangent = Vector3CrossProduct(normal, tangent);

    // Normalize bitangent vector
    bitangent = Vector3Normalize(bitangent);
    // Calculate the four vertices of the rectangle plane
    Vector3 vertices[] = {
        Vector3Add(position, Vector3Scale(tangent, width / 2)),
        Vector3Add(position, Vector3Scale(bitangent, height / 2)),
        Vector3Add(position, Vector3Scale(tangent, -width / 2)),
        Vector3Add(position, Vector3Scale(bitangent, -height / 2)),
        Vector3Add(position, Vector3Scale(tangent, width / 2)),
        Vector3Add(position, Vector3Scale(bitangent, height / 2)),
    };

    // Draw the rectangle using a triangle fan
    DrawTriangleStrip3D(vertices, 6, color);
}


void InitShadowMapping(shadow_mapper_t* m) {
    m->shadowMapWidth = 256;
    m->shadowMapHeight = 256;
    m->shadowMap = LoadRenderTexture(m->shadowMapWidth, m->shadowMapHeight);
    m->depthShader = LoadShader("assets/shaders/depth.vs", "assets/shaders/depth.fs");
    m->shadowShader = LoadShader("assets/shaders/shadow.vs", "assets/shaders/shadow.fs");
    m->shadowCamera=(Camera3D){};
    m->shadowCamera.position = Vector3Scale((Vector3){1.0f,1.0f,1.0f}, -30); // Camera position
    m->shadowCamera.target = (Vector3){0.0f,0.0f,0.0f};      // Camera looking at point
    m->shadowCamera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    m->shadowCamera.fovy = 45.0f;                                // Camera field-of-view Y
    m->shadowCamera.projection = CAMERA_FREE | CAMERA_ORTHOGRAPHIC;                   // Camera mode type
    // Setup orthogonal projection matrix for the shadow map (light's perspective)
    float left = -50.0f, right = 50.0f, bottom = -50.0f, top = 50.0f;
    float nearVal = 1.0f, farVal = 100.0f;
    m->lightProjection = MatrixOrtho(left, right, bottom, top, nearVal, farVal);

    m->lightView = MatrixLookAt(m->shadowCamera.position, m->shadowCamera.target, (Vector3){ 0.0f, 1.0f, 0.0f });

    // Set shader uniforms that won't change
    SetShaderValueMatrix(m->depthShader, GetShaderLocation(m->depthShader, "lightSpaceMatrix"), MatrixMultiply(m->lightView, m->lightProjection));
    SetShaderValueTexture(m->shadowShader, GetShaderLocation(m->shadowShader, "shadowMap"), m->shadowMap.texture);
}

void SetLight(shadow_mapper_t* m,Camera3D light,BoundingBox bounds){
    Vector3 bb_center=Vector3Scale(Vector3Add(bounds.max,bounds.min),0.5f);
    Vector3 size= Vector3Subtract(bounds.max,bounds.min);
    float radius=Vector3Length(size)/2;
    float left = -radius, right = radius, bottom = -radius, top = radius;
    float nearVal = 1.0f, farVal = radius+1;
    Vector3 direction=Vector3Normalize(Vector3Subtract(light.target,light.position));
    m->direction=direction;
    m->size=radius;
    m->shadowCamera=(Camera3D){};
    m->shadowCamera.position = Vector3Add(bb_center,Vector3Scale(direction, -farVal)); // Camera position
    m->shadowCamera.target = Vector3Add(bb_center,Vector3Scale(direction, farVal ));      // Camera looking at point
    m->shadowCamera.up = light.up;          // Camera up vector (rotation towards target)
    m->shadowCamera.fovy = light.fovy;                                // Camera field-of-view Y
    m->shadowCamera.projection = light.projection;                   // Camera mode type
    // Setup orthogonal projection matrix for the shadow map (light's perspective)
    m->lightProjection = MatrixOrtho(left, right, bottom, top, nearVal, farVal);

    m->lightView = MatrixLookAt(m->shadowCamera.position, m->shadowCamera.target, (Vector3){ 0.0f, 1.0f, 0.0f });

    // Set shader uniforms that won't change
    SetShaderValueMatrix(m->depthShader, GetShaderLocation(m->depthShader, "lightSpaceMatrix"), MatrixMultiply(m->lightView, m->lightProjection));
    SetShaderValueTexture(m->shadowShader, GetShaderLocation(m->shadowShader, "shadowMap"), m->shadowMap.texture);
}

void BeginShadowPass(shadow_mapper_t* m) {
    BeginTextureMode(m->shadowMap);
    ClearBackground(DARKGRAY); // Important to clear to BLACK (0, 0, 0, 255)
    BeginShaderMode(m->depthShader);
}

void EndShadowPass(shadow_mapper_t* m) {
    EndShaderMode();
    EndTextureMode();
}

void BeginScenePass(shadow_mapper_t* m) {
    BeginShaderMode(m->shadowShader);
    // Additional scene setup can go here
}

void EndScenePass(shadow_mapper_t* m) {
    EndShaderMode();
    DrawCube(m->shadowCamera.position,2,2,2,YELLOW);
    DrawLine3D(m->shadowCamera.position,m->shadowCamera.target,GREEN);
    DrawCube(Vector3Scale(m->direction,15.0f),0.5,0.5,0.5,YELLOW);
    draw_rectangle_plane(m->shadowCamera.position,m->direction,2*m->size,2*m->size,Fade(GREEN,0.2f));
    draw_rectangle_plane(m->shadowCamera.target,m->direction,2*m->size,2*m->size,Fade(GREEN,0.2f));
}

void RenderModelWithShadows(shadow_mapper_t* m,Camera3D* camera, Shader* shader) {
    
    if (shader->id == m->shadowShader.id) {
        // Setup model matrix
        Matrix modelMatrix = MatrixTranslate(camera->position.x, camera->position.y, camera->position.z);
        SetShaderValueMatrix(*shader, GetShaderLocation(*shader, "matModel"), modelMatrix);
    }
    if (shader->id == m->shadowShader.id) {
        // Pass additional uniforms if needed (for shadowShader)
        Matrix modelMatrix = MatrixTranslate(camera->position.x, camera->position.y, camera->position.z);
        SetShaderValueMatrix(*shader, GetShaderLocation(*shader, "matModel"), modelMatrix);
        SetShaderValueMatrix(m->shadowShader, GetShaderLocation(m->shadowShader, "view"), GetCameraMatrix(*camera));
        // Assuming 'camera' is your scene's camera and its view matrix is updated elsewhere
    }
}


#endif