#include <stdio.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <raymath.h>
#include "voxel.h"
#include "scene.h"
#include "lib.h"
#include "rmodels-extras.h"


typedef stack struct orbit_s{
    owned mut Vector2 lastMousePos;
    owned mut float azimuth; // Horizontal angle
    owned mut float elevation; // Vertical angle
    owned mut float radius; // Distance from the target
    owned mut bool isOrbiting;
    owned mut bool isPanning;
    borrowed Camera *camera;
} orbit_t;

static constructor(orbit)(Camera *camera){
    return (orbit_t){
        .lastMousePos = GetMousePosition(),
        .azimuth = 0.709f,
        .elevation = 0.709f,
        .radius = 17.3f,
        .isOrbiting = false,
        .isPanning = false,
        .camera = camera,
    };
}

instance method(orbit_t) int orbit__control_camera(orbit_t* orbiter){
    Vector2 mouseDelta = Vector2Subtract(GetMousePosition(), orbiter->lastMousePos);
    orbiter->lastMousePos = GetMousePosition();

    // Check for right mouse button pressed for orbiting
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && !IsKeyDown(KEY_LEFT_SHIFT)) {
        orbiter->isOrbiting = true;
        orbiter->isPanning = false;
    } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && IsKeyDown(KEY_LEFT_SHIFT)) {
        // Check for right mouse button pressed with Shift for panning
        orbiter->isPanning = true;
        orbiter->isOrbiting = false;
    } else {
        orbiter->isOrbiting = orbiter->isPanning = false;
    }

    if (orbiter->isOrbiting) {
        orbiter->azimuth -= mouseDelta.x * 0.01f;
        orbiter->elevation -= mouseDelta.y * 0.01f;
    } else if (orbiter->isPanning) {
        Vector3 right = Vector3Normalize(Vector3CrossProduct(Vector3Subtract(orbiter->camera->position, orbiter->camera->target), orbiter->camera->up));
        Vector3 up = Vector3Normalize(Vector3CrossProduct(right, Vector3Subtract(orbiter->camera->position, orbiter->camera->target)));
        float panSpeed = 0.01f;
        orbiter->camera->target = Vector3Add(orbiter->camera->target, Vector3Scale(right, mouseDelta.x * panSpeed));
        orbiter->camera->target = Vector3Add(orbiter->camera->target, Vector3Scale(up, mouseDelta.y * panSpeed));
        orbiter->camera->position = Vector3Add(orbiter->camera->position, Vector3Scale(right, mouseDelta.x * panSpeed));
        orbiter->camera->position = Vector3Add(orbiter->camera->position, Vector3Scale(up, mouseDelta.y * panSpeed));
    }

    orbiter->radius -= GetMouseWheelMove() * 0.8f;
    orbiter->radius = Clamp(orbiter->radius, 1.0f, 200000000.0f);

    if (orbiter->isOrbiting || !orbiter->isPanning) { // Update position only if orbiting or not panning
        orbiter->camera->position.x = orbiter->camera->target.x + orbiter->radius * cosf(orbiter->elevation) * sinf(orbiter->azimuth);
        orbiter->camera->position.y = orbiter->camera->target.y + orbiter->radius * sinf(orbiter->elevation);
        orbiter->camera->position.z = orbiter->camera->target.z + orbiter->radius * cosf(orbiter->elevation) * cosf(orbiter->azimuth);
    }
}

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


collision_t scene__get_intersections(Camera camera,scene_t* scene){
    Ray ray = { 0 };                    // Picking line ray
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    ray = GetMouseRay(GetMousePosition(), camera);
    collision_t result=scene__ray_intersect_point(scene,&ray);

    if(!result.hit) {
        // Check collision between ray and box
        RayCollision collision = GetRayCollisionBox(ray,
                (BoundingBox){(Vector3){ -100.0f, -0.1f, -100.0f},
                                (Vector3){ 100.0f, 0.0f, 100.0f }});
        result=(collision_t){collision.hit,collision.distance,collision.point,collision.normal,kVOXEL_NONE,-1,"plane"};
    }
    return result;
}
typedef enum app_construction_mode_e{
    APP_CONSTRUCTION_MODE_VOLUME=0x100,
    APP_CONSTRUCTION_MODE_SOLID=0x101,
} app_construction_mode_e;

typedef struct mcedit_app_s {
    app_construction_mode_e construction_mode;//=APP_CONSTRUCTION_MODE_VOLUME;

    Color current_color;//=GREEN;
    unsigned int current_color_index;//=GREEN;
    Color colors[9];//={WHITE,RED,ORANGE,YELLOW,GREEN,BLUE,MAGENTA,PINK,BLACK}
    const char* color_names[9];//={WHITE,RED,ORANGE,YELLOW,GREEN,BLUE,MAGENTA,PINK,BLACK}

    int screenWidth;
    int screenHeight;
} mcedit_app_t;


static constructor(mcedit_app)(){
    return (mcedit_app_t){
        .construction_mode=APP_CONSTRUCTION_MODE_VOLUME,

        .current_color=GREEN,
        .current_color_index=4,
        .colors={WHITE,RED,ORANGE,YELLOW,GREEN,BLUE,MAGENTA,PINK,BLACK},
        .color_names={"WHITE","RED","ORANGE","YELLOW","GREEN","BLUE","MAGENTA","PINK","BLACK"},

        .screenWidth = 800,
        .screenHeight = 450,
    };
}
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    mcedit_app_t app=mcedit_app_init();
    // Set window to be resizable
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(app.screenWidth, app.screenHeight, "Raylib Voxel Scene");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    // DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

    scene_t scene;
    scene__init(&scene);

    // Example: Add a voxel to the scene
    scene__add_voxel(&scene, (Vector3){0.0f, 0.0f, 0.0f}, RED,1);
    voxel_t cursor={20,30,20,3};

    // SetCameraMode(camera, CAMERA_FREE); // Let Raylib handle camera controls
    
    orbit_t orbiter=orbit_init(&camera);


    mut char status[100];
    mut int ctrl,left_ctrl;
    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_PERSPECTIVE );// Update camera position/movement
                // Get current window dimensions
        app.screenWidth = GetScreenWidth();
        app.screenHeight = GetScreenHeight();

        collision_t q_model_collision=scene__get_intersections(camera,&scene);

        Vector3 model_point_int=(Vector3){1*round(q_model_collision.point.x/1),1*round(q_model_collision.point.y/1),1*round(q_model_collision.point.z/1)};

        cursor.position.x=model_point_int.x;
        cursor.position.y=model_point_int.y;
        cursor.position.z=model_point_int.z;
        
    
        if(IsKeyPressed(KEY_LEFT_CONTROL)){ctrl=1;}
        if(IsKeyReleased(KEY_LEFT_CONTROL)){ctrl=0;}
        if(IsKeyPressed(KEY_RIGHT_CONTROL)){ctrl=1;}
        if(IsKeyReleased(KEY_RIGHT_CONTROL)){ctrl=0;}


        if (IsKeyPressed('Z')) orbiter=orbit_init(&camera);
        
        
        orbit__control_camera(&orbiter);

        snprintf(status,100,"collision %s p(%3.2f %3.2f %3.2f)",q_model_collision.qualifier,camera.position.x,camera.position.y,camera.position.z);

        BeginDrawing();
        // ClearBackground(RAYWHITE);
        ClearBackground(GRAY);

        for(int ci=0;ci<9;ci++){
            Rectangle button_rect=(Rectangle){ 10, 10+ci*50, 50, 30 };
            if (GuiButton(button_rect, app.color_names[ci]) && GetMousePosition().x<60) {
                app.current_color = app.colors[ci];
                app.current_color_index = ci;
            }
            if(app.current_color_index == ci){
                DrawRectangleRoundedLines(button_rect,.05,4,8,(Color){0,255,255,255});
            }
        }
        
        if (GetMousePosition().x>50 && GetMousePosition().x<(app.screenWidth-60) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(ctrl){
                scene__remove_voxel(&scene,model_point_int);
            }else{
                scene__add_voxel(&scene,model_point_int,app.current_color,app.current_color_index);
            }
            printf("left click\n");
        }
        
        int crt=10;
        if (GuiButton((Rectangle){ app.screenWidth-60, crt, 50, 30 }, "Volume")) {
            app.construction_mode = APP_CONSTRUCTION_MODE_VOLUME;
        }
        if (GuiButton((Rectangle){ app.screenWidth-60,crt=crt+60, 50, 30 }, "Slab")) {
            app.construction_mode = APP_CONSTRUCTION_MODE_SOLID;
        }

        BeginMode3D(camera);

            // Render the scene
            scene__render(&scene);

            DrawCubeWires(cursor.position, 1.0f, 1.0f, 1.0f, ctrl?Fade(RED, 0.5f):Fade(GREEN, 0.5f));
            
            // rlPushMatrix(); // Push the current matrix to the stack
            // rlTranslatef(0.5f, 0.5f, 0.5f); // Translate the grid
            DrawGridAt((Vector3){0.0f,-0.5f,0.0f},33, 1.0f); // Draw a grid
            // rlPopMatrix(); // Pop the matrix from the stack to revert the translation
        

            // Optionally, to cast a ray from the mouse:
            Ray ray = GetMouseRay(GetMousePosition(), camera);
            // You can then use Raylib functions to check for intersections, etc.
            if(GetMousePosition().x>50 && GetMousePosition().x<(app.screenWidth-60)){
                DrawLine3D(
                    q_model_collision.point,
                    Vector3Add(
                        q_model_collision.point,
                        (Vector3){
                            q_model_collision.normal.x*1,
                            q_model_collision.normal.y*1,
                            q_model_collision.normal.z*1,
                        }
                    ),
                    (Color){
                            q_model_collision.normal.x*255,
                            q_model_collision.normal.y*255,
                            q_model_collision.normal.z*255,
                            255,
                    });
                DrawSphereEx(q_model_collision.point,.1f,3,5,BLUE);
            }
        EndMode3D();

        DrawFPS(10, 10);


        //// DrawRectangle( 10, 30, 320, 93, Fade(SKYBLUE, 0.5f));
        //// DrawRectangleLines( 10, 30, 320, 93, BLUE);
        //// 
        //// DrawText("Free camera default controls:", 20, 40, 10, BLACK);
        //// DrawText("- Mouse Wheel to Zoom in-out" , 40, 60, 10, DARKGRAY);
        //// DrawText("- Mouse Wheel Pressed to Orbit" , 40, 80, 10, DARKGRAY);
        //// DrawText("- Z to zoom to (0, 0, 0)"     , 40, 100, 10, DARKGRAY);


        DrawRectangle( 0, app.screenWidth-20, app.screenWidth, 20, Fade(DARKGRAY, 0.5f));
        DrawText(status     , 5, app.screenHeight-15, 10, Fade(BLACK, 0.5f));

        EndDrawing();
    }

    CloseWindow();

    return 0;
}