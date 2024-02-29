#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "voxel.h"
#include "scene.h"


    int screenWidth = 800;
    int screenHeight = 450;
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------

    // Set window to be resizable
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "Raylib Voxel Scene");

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
    scene__add_voxel(&scene, (Vector3){0.0f, 0.0f, 0.0f}, 1);
    voxel_t cursor={20,30,20,3};

    // SetCameraMode(camera, CAMERA_FREE); // Let Raylib handle camera controls
    
    Vector2 lastMousePos = GetMousePosition();
    float azimuth = 0.709f; // Horizontal angle
    float elevation = 0.709f; // Vertical angle
    float radius = 17.3f; // Distance from the target
    bool isOrbiting = false;


    char status[100];
    Ray ray = { 0 };                    // Picking line ray
    RayCollision collision = { 0 };     // Ray collision hit info
    char *pointer_source="";
    int ctrl=0;
    int btn_left_active=0;
    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_PERSPECTIVE );// Update camera position/movement
                // Get current window dimensions
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        ray = GetMouseRay(GetMousePosition(), camera);
        Vector3 mouse_pointer_3d0;
        Vector3 mouse_pointer_3d;
        collision=scene__ray_intersect_point(&scene,&ray);

        if(collision.hit) {
            mouse_pointer_3d0=(Vector3){1*round(collision.point.x/1),1*round(collision.point.y/1),1*round(collision.point.z/1)};
            mouse_pointer_3d=(Vector3){collision.point.x,collision.point.y,collision.point.z};
            pointer_source="voxel";
            cursor.position.x=mouse_pointer_3d0.x;
            cursor.position.y=mouse_pointer_3d0.y;
            cursor.position.z=mouse_pointer_3d0.z;
        } else {
        // Check collision between ray and box
            collision = GetRayCollisionBox(ray,
                    (BoundingBox){(Vector3){ -100.0f, -0.1f, -100.0f},
                                    (Vector3){ 100.0f, 0.0f, 100.0f }});
            mouse_pointer_3d0=(Vector3){1*round(collision.point.x/1),1*round(collision.point.y/1),1*round(collision.point.z/1)};
            mouse_pointer_3d=(Vector3){collision.point.x,collision.point.y,collision.point.z};
            pointer_source="plane";
            cursor.position.x=mouse_pointer_3d0.x;
            cursor.position.y=mouse_pointer_3d0.y;
            cursor.position.z=mouse_pointer_3d0.z;
        }
        
    
            if(IsKeyPressed(KEY_LEFT_CONTROL)){
                ctrl=1;
            }
            if(IsKeyReleased(KEY_LEFT_CONTROL)){
                ctrl=0;
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                btn_left_active=1;
            }
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
                btn_left_active=0;
            }
        if (IsKeyPressed('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(ctrl){
                scene__remove_voxel(&scene,mouse_pointer_3d);
            }else{
                scene__add_voxel(&scene,cursor.position,4);
            }
            printf("left click\n");
        }
        Vector2 mouseDelta = Vector2Subtract(GetMousePosition(), lastMousePos);
        lastMousePos = GetMousePosition();
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            isOrbiting = true;
            // camera.target=mouse_pointer_3d;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            isOrbiting = false;
        }
        
        if (isOrbiting) {
            azimuth -= mouseDelta.x * 0.01f;
            elevation += mouseDelta.y * 0.01f;
        }

        // Zoom in or out
        double inc = GetMouseWheelMove() * 0.8f;
        if(inc){
            //camera.target=mouse_pointer_3d;
        }
        radius -= inc; // Adjust the factor (0.8f here) to control zoom sensitivity
        radius = Clamp(radius, 1.0f, 20000000.0f); // Clamp the radius to prevent it from going too low or too high
        
        
        // Calculate camera position
        camera.position.x = camera.target.x + radius * cosf(elevation) * sinf(azimuth);
        camera.position.y = camera.target.y + radius * sinf(elevation);
        camera.position.z = camera.target.z + radius * cosf(elevation) * cosf(azimuth);

        snprintf(status,100,"collision %s t(%3.2f %3.2f %3.2f) p(%3.2f %3.2f %3.2f)",pointer_source,collision.point.x,collision.point.y,collision.point.z,camera.position.x,camera.position.y,camera.position.z);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);

        // Render the scene
        scene__render(&scene);

        DrawCubeWires(cursor.position, 1.0f, 1.0f, 1.0f, ctrl?Fade(RED, 0.5f):Fade(GREEN, 0.5f));
        DrawGrid(10, 1.0f);

        // Optionally, to cast a ray from the mouse:
        Ray ray = GetMouseRay(GetMousePosition(), camera);
        // You can then use Raylib functions to check for intersections, etc.

        EndMode3D();
        DrawFPS(10, 10);


        //// DrawRectangle( 10, 30, 320, 93, Fade(SKYBLUE, 0.5f));
        //// DrawRectangleLines( 10, 30, 320, 93, BLUE);
        //// 
        //// DrawText("Free camera default controls:", 20, 40, 10, BLACK);
        //// DrawText("- Mouse Wheel to Zoom in-out" , 40, 60, 10, DARKGRAY);
        //// DrawText("- Mouse Wheel Pressed to Orbit" , 40, 80, 10, DARKGRAY);
        //// DrawText("- Z to zoom to (0, 0, 0)"     , 40, 100, 10, DARKGRAY);


        DrawRectangle( 0, screenHeight-20, screenWidth, 20, Fade(DARKGRAY, 0.5f));
        DrawText(status     , 5, screenHeight-15, 10, Fade(BLACK, 0.5f));

        EndDrawing();
    }

    CloseWindow();

    return 0;
}