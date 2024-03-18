#ifndef __VXDI_LIB_SHAODWMAPPING_H__
#define __VXDI_LIB_SHAODWMAPPING_H__

#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>

// Global variables
Camera3D shadowCamera; // Camera from the light's perspective
RenderTexture2D shadowMap;
Shader depthShader, shadowShader;
Matrix lightProjection, lightView;
const int shadowMapWidth = 1024;
const int shadowMapHeight = 1024;

// Function declarations
int shadowmapping_test_main();
void InitShadowMapping();
void BeginShadowPass();
void EndShadowPass();
void BeginScenePass();
void EndScenePass();
void RenderModelWithShadows(Camera3D position, Shader shader);

void InitShadowMapping(Vector3 light_direction,Vector3 light_target) {
    shadowMap = LoadRenderTexture(shadowMapWidth, shadowMapHeight);
    depthShader = LoadShader("assets/shaders/depth.vs", "assets/shaders/depth.fs");
    shadowShader = LoadShader("assets/shaders/shadow.vs", "assets/shaders/shadow.fs");
    shadowCamera=(Camera3D){};
    shadowCamera.position = Vector3Scale(light_direction, -30); // Camera position
    shadowCamera.target = light_target;      // Camera looking at point
    shadowCamera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    shadowCamera.fovy = 45.0f;                                // Camera field-of-view Y
    shadowCamera.projection = CAMERA_FREE | CAMERA_ORTHOGRAPHIC;                   // Camera mode type
    // Setup orthogonal projection matrix for the shadow map (light's perspective)
    float left = -50.0f, right = 50.0f, bottom = -50.0f, top = 50.0f;
    float nearVal = 1.0f, farVal = 100.0f;
    lightProjection = MatrixOrtho(left, right, bottom, top, nearVal, farVal);

    lightView = MatrixLookAt(shadowCamera.position, shadowCamera.target, (Vector3){ 0.0f, 1.0f, 0.0f });

    // Set shader uniforms that won't change
    SetShaderValueMatrix(depthShader, GetShaderLocation(depthShader, "lightSpaceMatrix"), MatrixMultiply(lightView, lightProjection));
    SetShaderValueTexture(shadowShader, GetShaderLocation(shadowShader, "shadowMap"), shadowMap.texture);
}

void BeginShadowPass() {
    BeginTextureMode(shadowMap);
    ClearBackground(DARKGRAY); // Important to clear to BLACK (0, 0, 0, 255)
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
    DrawCube(shadowCamera.position,2,2,2,YELLOW);
    DrawLine3D(shadowCamera.position,shadowCamera.target,GREEN);
}

void RenderModelWithShadows(Camera3D camera, Shader shader) {
    // Setup model matrix
    Matrix modelMatrix = MatrixTranslate(camera.position.x, camera.position.y, camera.position.z);
    SetShaderValueMatrix(shader, GetShaderLocation(shader, "model"), modelMatrix);

    if (shader.id == shadowShader.id) {
        // Pass additional uniforms if needed (for shadowShader)
        // e.g., SetShaderValueMatrix(shadowShader, GetShaderLocation(shadowShader, "view"), MatrixToFloat(camera.view));
        // Assuming 'camera' is your scene's camera and its view matrix is updated elsewhere
    }
}


#endif