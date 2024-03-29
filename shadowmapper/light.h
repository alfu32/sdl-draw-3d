
#ifndef __SHADOWMAPPER_LIGHT_H__
#define __SHADOWMAPPER_LIGHT_H__
#include <stdlib.h>
#include <GL/gl.h>
#include <GLES2/gl2.h>

#include <raylib.h>
typedef struct depth_map_s {

GLuint depthMapFBO;
GLuint depthMap;
} depth_map_t;
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

void setupShadowLight(depth_map_t* dm) {
    // Generate and bind the framebuffer for the depth map
    glGenFramebuffers(1, &dm->depthMapFBO);
    
    // Create the depth map texture
    glGenTextures(1, &dm->depthMap);
    glBindTexture(GL_TEXTURE_2D, dm->depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
    // Bind with framebuffer's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, dm->depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dm->depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void shadowLightBegin(depth_map_t* dm,GLuint shaderProgram, Vector3 lightPos, Vector3 center) {
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, dm->depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // Assuming 'calculateLightSpaceMatrix()' calculates the light's view and projection matrices
    ///// Mat4 lightSpaceMatrix = calculateLightSpaceMatrix(lightPos, center);
    ///// 
    ///// // Use shader program and set uniforms
    ///// glUseProgram(shaderProgram);
    ///// glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
    
    // Render the scene from the light's perspective
}

void shadowLightEnd( int screenWidth,  int screenHeight) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Reset viewport back to the original size
    glViewport(0, 0, screenWidth, screenHeight); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



#endif