#ifndef __VXDI_LIB_SHAODWMAPPING_H__
#define __VXDI_LIB_SHAODWMAPPING_H__

#include <raylib.h>
#include <raymath.h>
#include "vxdi-app-scene.h"

// Global variables
Camera3D shadowCamera; // Camera from the light's perspective
RenderTexture2D shadowMap;
Shader depthShader, shadowShader;
Matrix lightProjection, lightView;
const int shadowMapWidth = 2048;
const int shadowMapHeight = 2048;
Vector3 lightDirection = { -1.0f, -1.0f, -1.0f }; // Example light direction

// Function declarations
void InitShadowMapping();
void BeginShadowPass();
void EndShadowPass();
void BeginScenePass();
void EndScenePass();
void RenderModelWithShadows(scene_t* model, Camera3D position, Shader shader);

// Main application entry point
int vsxdi_shadowmap_test_main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raylib Shadow Mapping Example");

    // Initialize camera
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_FREE | CAMERA_PERSPECTIVE;                   // Camera mode type

    // Initialize shadow mapping
    InitShadowMapping();

    /// SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

    scene_t scene; // Load your model
    Vector3 modelPosition = { 0.0f, 0.0f, 0.0f };   // Set model position

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera,CAMERA_FREE); // Update camera
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // 1. First pass, from light's perspective
            BeginShadowPass();
                RenderModelWithShadows(&scene, camera, depthShader); // Use depth shader
            EndShadowPass();

            // 2. Then render scene as normal with shadows
            BeginMode3D(camera);
                BeginScenePass();
                    RenderModelWithShadows(&scene, camera, shadowShader); // Use shadow shader
                EndScenePass();
            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(depthShader); // Unload shader
    UnloadShader(shadowShader); // Unload shader
    UnloadRenderTexture(shadowMap); // Unload render texture
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void InitShadowMapping() {
    shadowMap = LoadRenderTexture(shadowMapWidth, shadowMapHeight);
    depthShader = LoadShader("depth.vs", "depth.fs");
    shadowShader = LoadShader("shadow.vs", "shadow.fs");
    shadowCamera=(Camera){};
    shadowCamera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    shadowCamera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    shadowCamera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    shadowCamera.fovy = 45.0f;                                // Camera field-of-view Y
    shadowCamera.projection = CAMERA_FREE | CAMERA_PERSPECTIVE;                   // Camera mode type
    // Setup orthogonal projection matrix for the shadow map (light's perspective)
    float left = -10.0f, right = 10.0f, bottom = -10.0f, top = 10.0f;
    float nearVal = 1.0f, farVal = 25.0f;
    lightProjection = MatrixOrtho(left, right, bottom, top, nearVal, farVal);

    // Setup the view matrix from the light's perspective
    Vector3 lightTarget = Vector3Add((Vector3){0, 0, 0}, lightDirection);
    lightView = MatrixLookAt(lightDirection, lightTarget, (Vector3){ 0.0f, 1.0f, 0.0f });

    // Set shader uniforms that won't change
    SetShaderValueMatrix(depthShader, GetShaderLocation(depthShader, "lightSpaceMatrix"), MatrixMultiply(lightView, lightProjection));
    SetShaderValueTexture(shadowShader, GetShaderLocation(shadowShader, "shadowMap"), shadowMap.texture);
}

void BeginShadowPass() {
    BeginTextureMode(shadowMap);
    ClearBackground(BLANK); // Important to clear to BLACK (0, 0, 0, 255)
    BeginShaderMode(depthShader);
}

void EndShadowPass() {
    EndShaderMode();
    EndTextureMode();
}

void BeginScenePass() {
    BeginShaderMode(shadowShader);
    // Additional scene setup can go here
}

void EndScenePass() {
    EndShaderMode();
}

void RenderModelWithShadows(scene_t *scene, Camera3D camera, Shader shader) {
    // Setup model matrix
    Matrix modelMatrix = MatrixTranslate(camera.position.x, camera.position.y, camera.position.z);
    SetShaderValueMatrix(shader, GetShaderLocation(shader, "model"), modelMatrix);

    if (shader.id == shadowShader.id) {
        // Pass additional uniforms if needed (for shadowShader)
        // e.g., SetShaderValueMatrix(shadowShader, GetShaderLocation(shadowShader, "view"), MatrixToFloat(camera.view));
        // Assuming 'camera' is your scene's camera and its view matrix is updated elsewhere
    }

    // Draw the model
    scene__render(scene, 0);
}


#endif