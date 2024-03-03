#include <stdio.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "raylib-nuklear.h"

#include <raymath.h>
#include "voxel.h"
#include "scene.h"
#include "lib.h"
#include "orbit-control.h"
#include "rmodels-extras.h"
#include "rmath-extras.h"


typedef struct control_keys_s { char ctrl,alt,shift,left_alt,right_alt,left_shift,right_shift,left_ctrl,right_ctrl; } control_keys_t;
control_keys_t get_control_keys(){
    control_keys_t r;
    if(IsKeyPressed(KEY_LEFT_CONTROL)){r.ctrl=1;r.left_ctrl=1;}
    if(IsKeyReleased(KEY_LEFT_CONTROL)){r.ctrl=0;r.left_ctrl=0;}
    if(IsKeyPressed(KEY_RIGHT_CONTROL)){r.ctrl=1;r.right_ctrl=1;}
    if(IsKeyReleased(KEY_RIGHT_CONTROL)){r.ctrl=0;r.right_ctrl=0;}
    if(IsKeyPressed(KEY_LEFT_SHIFT)){r.shift=1;r.left_shift=1;}
    if(IsKeyReleased(KEY_LEFT_SHIFT)){r.shift=0;r.left_shift=0;}
    if(IsKeyPressed(KEY_RIGHT_SHIFT)){r.shift=1;r.right_shift=1;}
    if(IsKeyReleased(KEY_RIGHT_SHIFT)){r.shift=0;r.right_shift=0;}
    if(IsKeyPressed(KEY_LEFT_ALT)){r.alt=1;r.left_alt=1;}
    if(IsKeyReleased(KEY_LEFT_ALT)){r.alt=0;r.left_alt=0;}
    if(IsKeyPressed(KEY_RIGHT_ALT)){r.alt=1;r.right_alt=1;}
    if(IsKeyReleased(KEY_RIGHT_ALT)){r.alt=0;r.right_alt=0;}
    return r;
}

typedef enum app_construction_mode_e{
    APP_CONSTRUCTION_MODE_VOLUME=0x100,
    APP_CONSTRUCTION_MODE_SHELL=0x101,
    APP_CONSTRUCTION_MODE_PLATE=0x102,
    APP_CONSTRUCTION_MODE_VOXEL=0x103,
} app_construction_mode_e;

typedef struct voxd3i_edit_app_s {
    app_construction_mode_e construction_mode;//=APP_CONSTRUCTION_MODE_VOLUME;

    Color current_color;//=GREEN;
    unsigned int current_color_index;//=GREEN;
    Color colors[9];//={WHITE,RED,ORANGE,YELLOW,GREEN,BLUE,MAGENTA,PINK,BLACK}
    const char* color_names[9];//={WHITE,RED,ORANGE,YELLOW,GREEN,BLUE,MAGENTA,PINK,BLACK}

    int screenWidth;
    int screenHeight;

} voxd3i_edit_app_t;


voxd3i_edit_app_t voxd3i_edit_app__setup(Camera3D* camera){
    // Define the camera to look into our 3d world
    camera->position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera->up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera->fovy = 45.0f;                                // Camera field-of-view Y
    camera->projection = CAMERA_PERSPECTIVE;             // Camera projection type
    return (voxd3i_edit_app_t){
        .construction_mode=APP_CONSTRUCTION_MODE_VOXEL,

        .current_color=GREEN,
        .current_color_index=4,
        .colors={WHITE,RED,ORANGE,YELLOW,GREEN,BLUE,MAGENTA,PINK,BLACK},
        .color_names={"WHITE","RED","ORANGE","YELLOW","GREEN","BLUE","MAGENTA","PINK","BLACK"},

        .screenWidth = 800,
        .screenHeight = 450,
    };
}

int voxd3i_edit_app__update(voxd3i_edit_app_t* app){
    // Get current window dimensions
    app->screenWidth = GetScreenWidth();
    app->screenHeight = GetScreenHeight();
    return 0;
}


int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    // Set window to be resizable


    // DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

    Camera3D camera;
    scene_t scene;
    voxd3i_edit_app_t app=voxd3i_edit_app__setup(&camera);
    scene__init(&scene);

    scene__load_model(&scene,"temp.vxde");

    // Example: Add a voxel to the scene
    scene__add_voxel(&scene, (Vector3){0.0f, 0.0f, 0.0f}, RED,1);
    voxel_t cursor={20,30,20,3};
    voxel_t cursor2={20,30,20,2};

    // SetCameraMode(camera, CAMERA_FREE); // Let Raylib handle camera controls
    
    orbit_t orbiter=orbit_init(&camera);


    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(app.screenWidth, app.screenHeight, "Raylib Voxel Scene");

    mut char status[100];
    mut int ctrl,left_ctrl;
    mut char show_help=0;
    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_PERSPECTIVE );// Update camera position/movement
        voxd3i_edit_app__update(&app);

        collision_t mouse_model=scene__get_intersections(camera,&scene);

        Vector3 model_point_int=Vector3Floor(mouse_model.point);
        Vector3 model_point_next_int=Vector3Add(model_point_int,(Vector3){0,1,0});
        if(mouse_model.voxel_index>=0){
            /// printf("\r on voxel %d                             ",mouse_model.voxel_index);
            model_point_int=mouse_model.voxel.position;
            model_point_next_int=Vector3Add(mouse_model.voxel.position,mouse_model.normal);
        }else {
            model_point_next_int=Vector3Round(mouse_model.point);//Vector3Add(Vector3Round(mouse_model.point),(Vector3){1,0,1});
            model_point_int=model_point_next_int;

        }
        cursor.position=model_point_int;
        cursor2.position=model_point_next_int;
        
    
        if(IsKeyPressed(KEY_LEFT_CONTROL)){ctrl=1;}
        if(IsKeyReleased(KEY_LEFT_CONTROL)){ctrl=0;}
        if(IsKeyPressed(KEY_RIGHT_CONTROL)){ctrl=1;}
        if(IsKeyReleased(KEY_RIGHT_CONTROL)){ctrl=0;}


        if (IsKeyPressed('Z')) orbiter=orbit_init(&camera);
        
        
        orbit__control_camera(&orbiter);

        snprintf(status,100,"voxels %d/%d temp file %s",scene.numVoxels,MAX_VOXELS,scene.temp_filename);

        BeginDrawing();
        // ClearBackground(RAYWHITE);
        // DrawCircle(app.screenWidth/2,app.screenHeight/2,app.screenHeight/2.0f-10.0f,RED);
        ClearBackground(GRAY);

        BeginMode3D(camera);

            // Render the scene
            scene__render(&scene);

            DrawCubeWires(model_point_int, 1.0f, 1.0f, 1.0f, Fade(RED, 0.5f));
            DrawCubeWires(model_point_next_int, 1.0f, 1.0f, 1.0f, Fade(GREEN, 0.5f));
            
            // rlPushMatrix(); // Push the current matrix to the stack
            // rlTranslatef(0.5f, 0.5f, 0.5f); // Translate the grid
            DrawGridAt((Vector3){0.0f,-0.5f,0.0f},33, 1.0f); // Draw a grid
            // rlPopMatrix(); // Pop the matrix from the stack to revert the translation
        

            // Optionally, to cast a ray from the mouse:
            Ray ray = GetMouseRay(GetMousePosition(), camera);
            // You can then use Raylib functions to check for intersections, etc.
            if(GetMousePosition().x>50 && GetMousePosition().x<(app.screenWidth-60)){
                DrawLine3D(
                    mouse_model.point,
                    Vector3Add(
                        mouse_model.point,
                        (Vector3){
                            mouse_model.normal.x*1,
                            mouse_model.normal.y*1,
                            mouse_model.normal.z*1,
                        }
                    ),
                    (Color){
                            mouse_model.normal.x*255,
                            mouse_model.normal.y*255,
                            mouse_model.normal.z*255,
                            255,
                    });
                DrawSphereEx(mouse_model.point,.1f,3,5,BLUE);
            }
        EndMode3D();

        /// DrawFPS(10, 10);
        switch(app.construction_mode){
            case APP_CONSTRUCTION_MODE_PLATE:
            break;
            case APP_CONSTRUCTION_MODE_SHELL:
            break;
            case APP_CONSTRUCTION_MODE_VOLUME:
            break;
            case APP_CONSTRUCTION_MODE_VOXEL:
                if (
                    GetMousePosition().x>50 && 
                    GetMousePosition().x<(app.screenWidth-60) && 
                    IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
                    !IsKeyDown(KEY_LEFT_CONTROL) &&
                    !IsKeyDown(KEY_LEFT_SHIFT)
                ){
                    scene__remove_voxel(&scene,model_point_int);
                }
                if (
                    GetMousePosition().x>50 && 
                    GetMousePosition().x<(app.screenWidth-60) && 
                    IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                    !IsKeyDown(KEY_LEFT_CONTROL) &&
                    !IsKeyDown(KEY_LEFT_SHIFT)
                ){
                    scene__add_voxel(&scene,model_point_next_int,app.current_color,app.current_color_index);
                }
            break;
        }
        


        for(int ci=0;ci<9;ci++){
            Rectangle button_rect=(Rectangle){ 10, 25+ci*45, 40, 40 };
            
            if(app.current_color_index == ci){
                DrawRectangleRoundedLines(button_rect,.05,4,2,app.colors[ci]);
            } else {
                DrawRectangleRoundedLines(button_rect,.05,4,2,Fade(app.colors[ci],0.25f));
            }
            if (GuiButton(button_rect, app.color_names[ci]) && GetMousePosition().x<60) {
                app.current_color = app.colors[ci];
                app.current_color_index = ci;
            }
        }

        int crt=25;
        if (GuiButton((Rectangle){ app.screenWidth-60, crt, 50, 30 }, "Close")) {
            
        }
        if (GuiButton((Rectangle){ app.screenWidth-60, crt+=50, 50, 30 }, "Help")) {
            show_help=!show_help;
        }
        if (GuiButton((Rectangle){ app.screenWidth-60, crt=crt+90, 50, 30 }, "Volume")) {
            app.construction_mode = APP_CONSTRUCTION_MODE_VOLUME;
        }
        if (GuiButton((Rectangle){ app.screenWidth-60,crt=crt+50, 50, 30 }, "SHELL")) {
            app.construction_mode = APP_CONSTRUCTION_MODE_SHELL;
        }
        if (GuiButton((Rectangle){ app.screenWidth-60,crt=crt+50, 50, 30 }, "Plate")) {
            app.construction_mode = APP_CONSTRUCTION_MODE_PLATE;
        }
        if (GuiButton((Rectangle){ app.screenWidth-60,crt=crt+50, 50, 30 }, "Voxel")) {
            app.construction_mode = APP_CONSTRUCTION_MODE_VOXEL;
        }

        if(show_help){
                int row=60;
                int left=app.screenWidth-380;
                DrawRectangle( left-10, row, 320, 160, Fade(SKYBLUE, 0.5f));
                DrawRectangleLines( left-10, row, 320, 160, BLUE);
                if (GuiButton((Rectangle){ left+290,row, 20, 20 }, "X")) {
                    show_help=0;
                }
                DrawText("Free camera default controls:", left, row+=10, 10, BLACK);
                DrawText("- Mouse Wheel to Zoom in-out" , left, row+=20, 10, DARKGRAY);
                DrawText("- Ctrl-Left Click Pressed to Orbit" , left, row+=20, 10, DARKGRAY);
                DrawText("- Shift-Left Click Pressed to Pan" , left, row+=20, 10, DARKGRAY);
                DrawText("- Left Click to Add Voxels" , left, row+=20, 10, DARKGRAY);
                DrawText("- Right Click to Remove Voxels" , left, row+=20, 10, DARKGRAY);
                DrawText("- Z to zoom to (0, 0, 0)"     , left, row+=20, 10, DARKGRAY);
        }
        DrawRectangle( 0, 0, app.screenWidth, 20, Fade(DARKGRAY, 0.95f));
        DrawText(status     , app.screenWidth-300, 5, 10, (Color){200,200,200,255});

        EndDrawing();
    }

    CloseWindow();
    /// scene__save_model(&scene,"temp.vxde");

    return 0;
}