#ifndef _LIBSDL_H_
#define _LIBSDL_H_
#include <GL/gl.h>
#include <GL/glu.h>

#define GL_DRAW_TEXT(N,X,Y,FONT,COLOR,TEXT, ...) \
/* Render text*/\
printf("Render text\n");\
char buffer_##N[1000];\
snprintf(buffer_##N,1000,TEXT,__VA_ARGS__);\
SDL_Surface *text_surface_##N = TTF_RenderText_Blended(FONT, buffer_##N, COLOR);\
printf("1\n");\
GLuint textureID_##N;\
glGenTextures(1, &textureID_##N);\
glBindTexture(GL_TEXTURE_2D, textureID_##N);\
printf("2\n");\
glTexImage2D(\
    GL_TEXTURE_2D, 0, GL_RGBA,\
    text_surface_##N->w, text_surface_##N->h,\
    0, GL_RGBA, GL_UNSIGNED_BYTE,\
    text_surface_##N->pixels\
);\
printf("Set texture parameters\n");\
/* Set texture parameters*/\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);\
\
printf("Remember to free the original surface\n");\
/* Remember to free the original surface*/\
SDL_FreeSurface(text_surface_##N);\
\
glEnable(GL_BLEND);\
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);\
printf("Bind the text texture\n");\
/* Bind the text texture*/\
glBindTexture(GL_TEXTURE_2D, textureID_##N);\
/* Render the texture here; you'll need to set up vertex data for a quad and use a shader that can render the texture.*/\
\
printf("Render the texture here\n");\
glDisable(GL_BLEND);\
glDeleteTextures(1, &textureID_##N);\

#endif