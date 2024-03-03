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
#include "multistep-tool.h"


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

    scene_t guides;
    scene_t construction_hints;

    char buffer[1000];

} voxd3i_edit_app_t;

// #define PI 3.14159265358979323846

void fillColorCircle(Color colors[360]) {
    for (int i = 0; i < 360; i++) {
        colors[i]=ColorFromHSV(i,1.0f,1.0f);
    }
}

voxd3i_edit_app_t voxd3i_edit_app__setup(Camera3D* camera){
    // Define the camera to look into our 3d world
    camera->position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera->up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera->fovy = 45.0f;                                // Camera field-of-view Y
    camera->projection = CAMERA_PERSPECTIVE;             // Camera projection type
    scene_t guides;
    scene__init(&guides,0);           // Camera projection type
    scene_t construction_hints;
    scene__init(&construction_hints,0);

    voxd3i_edit_app_t app = {
        .construction_mode=APP_CONSTRUCTION_MODE_VOXEL,

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

        .guides=guides,
        .construction_hints=construction_hints,
    };
    fillColorCircle(app.colors);

    app.current_color=app.colors[4];
    app.current_color_index=4;
    return app;
}

int voxd3i_edit_app__update(voxd3i_edit_app_t* app){
    // Get current window dimensions
    app->screenWidth = GetScreenWidth();
    app->screenHeight = GetScreenHeight();
    return 0;
}
unsigned int voxd3i_edit_app__InputMathExpr(voxd3i_edit_app_t* app) {
    int keypressed;
    char b0[999]="";
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
            strcpy(b0,app->buffer);
            snprintf(app->buffer,1000,"%s%d",b0,(keypressed-48));
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
            strcpy(b0,app->buffer);
            snprintf(app->buffer,1000,"%s%d",b0,(keypressed-320));
            return 25;
        case KEY_COMMA:
        case KEY_SEMICOLON:
        case KEY_X:
        case KEY_SLASH:
        case KEY_EQUAL:
        case KEY_MINUS:
        case KEY_KP_ADD:
            strcpy(b0,app->buffer);
            if(keypressed == KEY_EQUAL || keypressed == KEY_KP_ADD){
                snprintf(app->buffer,1000,"%s%c",b0,keypressed);
            }else{
                snprintf(app->buffer,1000,"%s%c",b0,keypressed);
            }
            return 25;
        case KEY_BACKSPACE:
            l=strlen(app->buffer);
            if(l>0){
                app->buffer[l-1] = '\0';
                return 25;
            }
        break;
        case KEY_ENTER:
            app->buffer[0] = '\0';
            return 25;
        default:
            // printf("pressed : [%d,%c]\n",keypressed,keypressed);
            // WaitTime(1.125);
            return 0;
    }
    return 0;
}
int ext_RoundButton(Vector2 pos,float radius,Color color){
    Vector2 mp = GetMousePosition();
    if(Vector2Distance(pos,mp)<=radius){
        DrawCircle(pos.x,pos.y,radius,ColorBrightness(color,0.55f));
        return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    } else {
        DrawCircle(pos.x,pos.y,radius,color);
        DrawCircleLines(pos.x,pos.y,radius,color);
        return 0;
    }
}

void voxel_tool_acquire(scene_t* scene,Vector3* inputs,size_t num_inputs){}
void voxel_tool_finish(scene_t* scene,Vector3* inputs,size_t num_inputs){}

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    multistep_tool_t voxel_tool;
    multistep_tool__init(&voxel_tool,1,voxel_tool_acquire,voxel_tool_finish);
    // Set window to be resizable


    // DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    SetExitKey(KEY_Q); // Use KEY_NULL or 0 to disable

    Camera3D camera;
    scene_t scene;
    voxd3i_edit_app_t app=voxd3i_edit_app__setup(&camera);
    scene__init(&scene,1);

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
    char window_should_close=0;
    for (;!(WindowShouldClose() || window_should_close);) {
        start:

        UpdateCamera(&camera, app.current_camera_projection );// Update camera position/movement
        voxd3i_edit_app__update(&app);

        collision_t mouse_model=scene__get_intersections(camera,&scene);
        if(mouse_model.collision_hit == COLLISION_HIT_PLANE || mouse_model.collision_hit == COLLISION_HIT_NONE) {
            mouse_model=scene__get_intersections(camera,&app.guides);

            if(mouse_model.collision_hit == COLLISION_HIT_VOXEL) {
                mouse_model.collision_hit=COLLISION_HIT_GUIDE;
            }
        }

        Vector3 model_point_int=Vector3Floor(mouse_model.point);
        Vector3 model_point_next_int=Vector3Add(model_point_int,(Vector3){0,1,0});
        switch(mouse_model.collision_hit){
            case COLLISION_HIT_NONE:
                model_point_next_int=Vector3Round(mouse_model.point);//Vector3Add(Vector3Round(mouse_model.point),(Vector3){1,0,1});
                model_point_int=model_point_next_int;
                break;
            case COLLISION_HIT_VOXEL:
                model_point_int=mouse_model.voxel.position;
                model_point_next_int=Vector3Add(mouse_model.voxel.position,mouse_model.normal);
                break;
            case COLLISION_HIT_PLANE:
                model_point_next_int=Vector3Round(mouse_model.point);//Vector3Add(Vector3Round(mouse_model.point),(Vector3){1,0,1});
                model_point_int=model_point_next_int;
                break;
            case COLLISION_HIT_GUIDE:
                model_point_next_int=Vector3Round(mouse_model.point);//Vector3Add(Vector3Round(mouse_model.point),(Vector3){1,0,1});
                model_point_int=model_point_next_int;
                break;
        }
        cursor.position=model_point_int;
        cursor2.position=model_point_next_int;
        int keyboard_wait_time;
        if( abs(keyboard_wait_time=voxd3i_edit_app__InputMathExpr(&app))  ){
            WaitTime(((float)keyboard_wait_time)/1000.0f);
            continue;
        }
    
        if(IsKeyPressed(KEY_LEFT_CONTROL)){ctrl=1;}
        if(IsKeyReleased(KEY_LEFT_CONTROL)){ctrl=0;}
        if(IsKeyPressed(KEY_RIGHT_CONTROL)){ctrl=1;}
        if(IsKeyReleased(KEY_RIGHT_CONTROL)){ctrl=0;}


        if(IsKeyPressed(KEY_G)){
            scene__add_voxel(&app.guides,model_point_int,RED,0);
            for(int i=1;i<10;i++){
                scene__add_voxel(&app.guides,(Vector3){model_point_int.x+(float)i,model_point_int.y,model_point_int.z},RED,0);
                scene__add_voxel(&app.guides,(Vector3){model_point_int.x-(float)i,model_point_int.y,model_point_int.z},Fade(RED,0.5f),0);
                scene__add_voxel(&app.guides,(Vector3){model_point_int.x,model_point_int.y+(float)i,model_point_int.z},GREEN,0);
                scene__add_voxel(&app.guides,(Vector3){model_point_int.x,model_point_int.y-(float)i,model_point_int.z},Fade(GREEN,0.5f),0);
                scene__add_voxel(&app.guides,(Vector3){model_point_int.x,model_point_int.y,model_point_int.z+(float)i},BLUE,0);
                scene__add_voxel(&app.guides,(Vector3){model_point_int.x,model_point_int.y,model_point_int.z-(float)i},Fade(BLUE,0.5f),0);
            }
        }
        if(IsKeyPressed(KEY_SPACE)){
            scene__clear(&app.guides);
        }


        if (IsKeyPressed('Z')) orbiter=orbit_init(&camera);
        
        
        orbit__control_camera(&orbiter);

        snprintf(status,100,"buffer %40s  voxels %d/%d temp file %s",app.buffer,scene.numVoxels,MAX_VOXELS,scene.temp_filename);

        BeginDrawing();
        // ClearBackground(RAYWHITE);
        // DrawCircle(app.screenWidth/2,app.screenHeight/2,app.screenHeight/2.0f-10.0f,RED);
        ClearBackground(GRAY);

        BeginMode3D(camera);

            // Render the scene
            scene__render(&scene,0);
            scene__render(&app.guides,1);
            scene__render(&app.construction_hints,2);

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
                    GetMousePosition().x>130 && 
                    GetMousePosition().x<(app.screenWidth-100) && 
                    IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
                    !IsKeyDown(KEY_LEFT_CONTROL) &&
                    !IsKeyDown(KEY_LEFT_SHIFT)
                ){
                    scene__remove_voxel(&scene,model_point_int);
                }
                if (
                    GetMousePosition().x>130 && 
                    GetMousePosition().x<(app.screenWidth-100) && 
                    IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                    !IsKeyDown(KEY_LEFT_CONTROL) &&
                    !IsKeyDown(KEY_LEFT_SHIFT)
                ){
                    scene__add_voxel(&scene,model_point_next_int,app.current_color,app.current_color_index);
                }
            break;
        }
        


        for(int ci=0;ci<24;ci+=1){
            for(int lum=-1;lum<2;lum+=1){
                Vector2 pos = {60+lum*40, 40+ci*35};
                Color cl=ColorBrightness(app.colors[ci*15],((float)lum*5.0f)/10.0f);
                if(app.current_color_index == ci*10 + lum+1){
                    DrawCircle(pos.x,pos.y,20,WHITE);
                }
                if (ext_RoundButton(pos,15,cl) && GetMousePosition().x<130) {
                    app.current_color = cl;
                    app.current_color_index = ci*10 + lum+1;
                    // goto start;
                }
            }
        }

        int crt=25;
        if (GuiButton((Rectangle){ app.screenWidth-60, crt, 50, 30 }, "Close")) {
            window_should_close=1;
        }
        if (GuiButton((Rectangle){ app.screenWidth-60, crt+=50, 50, 30 }, "Help")) {
            show_help=!show_help;
        }
        if(app.construction_mode==APP_CONSTRUCTION_MODE_VOLUME) {
            DrawRectangleLinesEx((Rectangle){app.screenWidth-60-2, crt+90-2, 50+4, 30+4},2,PINK);
        }
        if (GuiButton((Rectangle){ app.screenWidth-60, crt=crt+90, 50, 30 }, "Volume")) {
            app.construction_mode = APP_CONSTRUCTION_MODE_VOLUME;
        }

        if(app.construction_mode==APP_CONSTRUCTION_MODE_SHELL) {
            DrawRectangleLinesEx((Rectangle){app.screenWidth-60-2, crt+50-2, 50+4, 30+4},2,PINK);
        }
        if (GuiButton((Rectangle){ app.screenWidth-60,crt=crt+50, 50, 30 }, "SHELL")) {
            app.construction_mode = APP_CONSTRUCTION_MODE_SHELL;
        }

        if(app.construction_mode==APP_CONSTRUCTION_MODE_PLATE) {
            DrawRectangleLinesEx((Rectangle){app.screenWidth-60-2, crt+50-2, 50+4, 30+4},2,PINK);
        }
        if (GuiButton((Rectangle){ app.screenWidth-60,crt=crt+50, 50, 30 }, "Plate")) {
            app.construction_mode = APP_CONSTRUCTION_MODE_PLATE;
        }

        if(app.construction_mode==APP_CONSTRUCTION_MODE_VOXEL) {
            DrawRectangleLinesEx((Rectangle){app.screenWidth-60-2, crt+50-2, 50+4, 30+4},2,PINK);
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