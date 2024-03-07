#ifndef __VXDI_LIB_SHAODWMAPPING_H__
#define __VXDI_LIB_SHAODWMAPPING_H__

#include "raylib.h"
#include "raymath.h"
#include "vxdi-app-scene.h"

// Global variables for shadow mapping
RenderTexture2D shadowMap;
Shader depthShader, shadowShader;
Matrix lightProjection, lightView;
const int shadowMapWidth = 2048;
const int shadowMapHeight = 2048;

// Initialization Phase
void InitShadowMapping() {
    // Initialize framebuffers, shaders, etc.
    shadowMap = LoadRenderTexture(shadowMapWidth, shadowMapHeight);
    depthShader = LoadShader("shaders/depth.vs", "shaders/depth.fs");
    shadowShader = LoadShader("shaders/shadow.vs", "shaders/shadow.fs");
    // Set up light projection & view matrices, etc.
}

// Shadow Capture Phase
void BeginShadowPass(Vector3 lightPosition) {
    BeginTextureMode(shadowMap); // Using Raylib to render to texture
    ClearBackground(BLANK); // Important to clear depth information
    BeginShaderMode(depthShader);
    // Set up light's view and projection matrices here
    // Note: Actual implementation will depend on light's properties
}

void EndShadowPass() {
    EndShaderMode();
    EndTextureMode();
}

// Shadow Application Phase
void BeginScenePass() {
    BeginShaderMode(shadowShader);
    // Bind shadow map and set shader uniforms here
    // Note: Actual implementation will depend on shader specifics
}

void EndScenePass() {
    EndShaderMode();
}

// Model Render (Within the Render Loop)
void RenderModelWithShadows(scene_t *scene,Camera3D camera, Vector3 position, Shader shader) {
    // Note: Model rendering specifics will depend on your scene setup
    BeginMode3D(camera); // Assuming 'camera' is defined elsewhere
    scene__render(scene, 0);
    EndMode3D();
}

// Main function and render loop (simplified)
int vxdi_shadow__example_main(Vector3 lightPosition) {
    InitWindow(800, 600, "Shadow Mapping Example");
    Camera3D camera = {/* Camera setup */};
    InitShadowMapping();

    while (!WindowShouldClose()) {
        // Begin Shadow Pass
        BeginShadowPass(lightPosition);
        // RenderScene(); // Your scene rendering function for shadow capture
        EndShadowPass();

        // Begin Scene Pass
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginScenePass();
        // RenderScene(); // Your scene rendering function for final render
        EndScenePass();
        EndDrawing();
    }

    // Cleanup
    UnloadShader(depthShader);
    UnloadShader(shadowShader);
    UnloadRenderTexture(shadowMap);
    CloseWindow();

    return 0;
}

#endif