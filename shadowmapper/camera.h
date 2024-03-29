

#ifndef __SHADOWMAPPER_CAMERA_H__
#define __SHADOWMAPPER_CAMERA_H__
#include <GL/gl.h>
#include <raylib.h>
#include <GLES2/gl2.h>
void cameraSetup(GLuint shaderProgram, Camera camera) {
    // Use shader program and set uniforms
    glUseProgram(shaderProgram);
    //// glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &camera.viewMatrix[0][0]);
    //// glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &camera.projectionMatrix[0][0]);
    
    // Assuming 'setupTextureUniforms()' sets up the shadow map texture uniform
    //// setupTextureUniforms(shaderProgram, depthMap);
}

void cameraBegin() {
    // Optional: Any setup needed before rendering from the camera's perspective
}

void cameraEnd() {
    // Optional: Cleanup or reset after rendering from the camera's perspective
}

#endif