#ifndef __VXDI_APP_EDITOR_H__
#define __VXDI_APP_EDITOR_H__

#include <string.h>
#include <stdio.h>
#include <raylib.h>
#include "vxdi-app-voxel.h"
#include "vxdi-app-scene.h"


typedef struct vxdi_app_editor_s {

    Color current_color;//=GREEN;
    unsigned int current_color_index;//=GREEN;
    Color colors[360];//={WHITE,RED,ORANGE,YELLOW,GREEN,BLUE,MAGENTA,PINK,BLACK}
    //const char* color_names[9];//={WHITE,RED,ORANGE,YELLOW,GREEN,BLUE,MAGENTA,PINK,BLACK}

    CameraProjection current_camera_projection;
    unsigned int current_camera_projection_index;
    CameraProjection camera_projections[2];
    const char* camera_projection_names[2];

    CameraMode current_camera_mode;
    unsigned int current_camera_mode_index;
    CameraMode camera_modes[5];
    const char* camera_mode_names[5];

    int screenWidth;
    int screenHeight;

    Vector3 light_direction;
    Camera3D camera;
    scene_t scene;

    scene_t guides;
    scene_t construction_hints;

    char text_buffer[100];

    Vector2 current_mouse_position;
    Vector2 previous_mouse_position;
    char is_mouse_position_changed;

    collision_t mouse_model;
    Vector3 model_point_int;
    Vector3 model_point_next_int;

} vxdi_app_editor_t;

// #define PI 3.14159265358979323846


vxdi_app_editor_t vxdi_app_editor__setup(Vector3 light_direction){
    
    scene_t guides;
    scene__init(&guides,0,light_direction);           // Camera projection type
    guides.temp_filename="guides.vxde";
    scene_t construction_hints;
    scene__init(&construction_hints,0,light_direction);
    guides.temp_filename="construction_hints.vxde";

    scene_t layer0;
    scene__init(&layer0,1,light_direction);
    layer0.temp_filename="temp.vxdi";

    vxdi_app_editor_t app = {

        //.colors={WHITE,RED,ORANGE,YELLOW,GREEN,BLUE,MAGENTA,PINK,BLACK},
        //.color_names={"WHITE","RED","ORANGE","YELLOW","GREEN","BLUE","MAGENTA","PINK","BLACK"},


        .current_camera_projection=CAMERA_PERSPECTIVE,
        .current_camera_projection_index=0,
        .camera_projections={CAMERA_PERSPECTIVE,CAMERA_ORTHOGRAPHIC},
        .camera_projection_names={"PERSPECTIVE","ORTHOGRAPHIC"},

        .current_camera_mode=CAMERA_FREE,
        .current_camera_mode_index=0,
        .camera_modes={CAMERA_FREE,CAMERA_ORBITAL,CAMERA_FIRST_PERSON,CAMERA_THIRD_PERSON},
        .camera_mode_names={"FREE","ORBITAL","FIRST_PERSON","ORBITAL","THIRD_PERSON"},

        .screenWidth = 800,
        .screenHeight = 450,
        .light_direction = light_direction,

        .scene=layer0,
        .guides=guides,
        .construction_hints=construction_hints,
        .current_mouse_position=GetMousePosition(),
        .previous_mouse_position=GetMousePosition(),
        .is_mouse_position_changed=1,
    };
    // Define the camera to look into our 3d world
    app.camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    app.camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    app.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    app.camera.fovy = 45.0f;                                // Camera field-of-view Y
    app.camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
    fillColorCircle(app.colors);

    app.current_color=app.colors[4];
    app.current_color_index=4;
    return app;
}

int vxdi_app_editor__update(vxdi_app_editor_t* app){
    // Get current window dimensions
    app->screenWidth = GetScreenWidth();
    app->screenHeight = GetScreenHeight();
    return 0;
}
unsigned int vxdi_app_editor__InputMathExpr(vxdi_app_editor_t* app) {
    int keypressed;
    char b0[50]="";
    int l;
    switch(keypressed=GetKeyPressed()){
        case KEY_ZERO:
        case KEY_ONE:
        case KEY_TWO:
        case KEY_THREE:
        case KEY_FOUR:
        case KEY_FIVE:
        case KEY_SIX:
        case KEY_SEVEN:
        case KEY_EIGHT:
        case KEY_NINE:
            strcpy(b0,app->text_buffer);
            snprintf(app->text_buffer,100,"%s%d",b0,(keypressed-48));
            b0[0]='\0';
            return 25;
        break;
        case KEY_KP_0:
        case KEY_KP_1:
        case KEY_KP_2:
        case KEY_KP_3:
        case KEY_KP_4:
        case KEY_KP_5:
        case KEY_KP_6:
        case KEY_KP_7:
        case KEY_KP_8:
        case KEY_KP_9:
            strcpy(b0,app->text_buffer);
            snprintf(app->text_buffer,100,"%s%d",b0,(keypressed-320));
            return 25;
        case KEY_COMMA:
        case KEY_SEMICOLON:
        case KEY_X:
        case KEY_SLASH:
        case KEY_EQUAL:
        case KEY_MINUS:
        case KEY_KP_ADD:
            strcpy(b0,app->text_buffer);
            if(keypressed == KEY_EQUAL || keypressed == KEY_KP_ADD){
                snprintf(app->text_buffer,100,"%s%c",b0,keypressed);
            }else{
                snprintf(app->text_buffer,100,"%s%c",b0,keypressed);
            }
            return 25;
        case KEY_BACKSPACE:
            l=strlen(app->text_buffer);
            if(l>0){
                app->text_buffer[l-1] = '\0';
                return 25;
            }
        break;
        case KEY_ENTER:
            app->text_buffer[0] = '\0';
            return 25;
        default:
            // printf("pressed : [%d,%c]\n",keypressed,keypressed);
            // WaitTime(1.125);
            return 0;
    }
    return 0;
}
#endif