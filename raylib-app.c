#include "raylib.h"
#include "raymath.h"
#include "voxel.h"
#include "scene.h"

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

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

    scene_t myScene;
    scene__init(&myScene);

    // Example: Add a voxel to the scene
    scene__add_voxel(&myScene, 0.0f, 0.0f, 0.0f, 1);
    voxel_t cursor;

    // SetCameraMode(camera, CAMERA_FREE); // Let Raylib handle camera controls
    Vector3 pos_init;
    Vector2 mouse_init;
    Vector2 mouse_orbit;
    char start_orbit=0;
    double azimuth_0;
    double elevation_0;
    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE );// Update camera position/movement

        if (IsKeyPressed('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)){
            Ray ray = GetMouseRay(GetMousePosition(), camera);
            cursor.x=ray.direction.x*10;
            cursor.y=ray.direction.y*10;
            cursor.z=ray.direction.z*10;
            //GetRayCollisionMesh
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            pos_init=(Vector3){camera.position.x,camera.position.y,camera.position.z};
            mouse_init=GetMousePosition();
            start_orbit=1;
            azimuth_0 = ((mouse_orbit.x - mouse_init.x) / screenWidth) * 2 * PI;
            elevation_0 = ((mouse_orbit.y - mouse_init.y) / screenHeight) * PI / 2;
        }
        if (start_orbit && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
            float orbitRadius = Vector3Distance(camera.position, camera.target);
            Vector2 mouse_orbit = GetMousePosition();
            double azimuth = ((mouse_orbit.x - mouse_init.x) / screenWidth) * 2 * PI; // 2 * PI for full horizontal rotation
            double elevation = ((mouse_orbit.y - mouse_init.y) / screenHeight) * PI / 2; // PI / 2 for vertical rotation within reasonable limits

            // Horizontal orbit (azimuth)
            camera.position.x = camera.target.x + cos(azimuth-azimuth_0) * orbitRadius * cos(elevation-elevation_0);
            camera.position.z = camera.target.z + sin(azimuth-azimuth_0) * orbitRadius * cos(elevation-elevation_0);

            // Vertical orbit (elevation)
            camera.position.y = camera.target.y + sin(elevation-elevation_0) * orbitRadius;
        }
        if (start_orbit && IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)){
            start_orbit=0;
        }


        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);

        // Render the scene
        scene__render0(&myScene);
        voxel__render0(&cursor);
        DrawGrid(10, 1.0f);

        // Optionally, to cast a ray from the mouse:
        Ray ray = GetMouseRay(GetMousePosition(), camera);
        // You can then use Raylib functions to check for intersections, etc.

        EndMode3D();
        DrawFPS(10, 10);


        DrawRectangle( 10, 30, 320, 93, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines( 10, 30, 320, 93, BLUE);

        DrawText("Free camera default controls:", 20, 40, 10, BLACK);
        DrawText("- Mouse Wheel to Zoom in-out" , 40, 60, 10, DARKGRAY);
        DrawText("- Mouse Wheel Pressed to Pan" , 40, 80, 10, DARKGRAY);
        DrawText("- Z to zoom to (0, 0, 0)"     , 40, 100, 10, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}