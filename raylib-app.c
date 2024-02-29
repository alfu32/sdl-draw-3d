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

    scene_t scene;
    scene__init(&scene);

    // Example: Add a voxel to the scene
    scene__add_voxel(&scene, 0.0f, 0.0f, 0.0f, 1);
    voxel_t cursor={20,30,20,3};

    // SetCameraMode(camera, CAMERA_FREE); // Let Raylib handle camera controls
    Vector3 pos_init;
    Vector2 mouse_init;
    Vector2 mouse_orbit;
    char start_orbit=0;
    double azimuth0;
    double elevation0;
    char status[100];
    Ray ray = { 0 };                    // Picking line ray
    RayCollision collision = { 0 };     // Ray collision hit info
    Vector2 lastMousePos = GetMousePosition();
    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE );// Update camera position/movement
        ray = GetMouseRay(GetMousePosition(), camera);
        // Check collision between ray and box
        collision = GetRayCollisionBox(ray,
                    (BoundingBox){(Vector3){ -100.0f, -0.1f, -100.0f},
                                    (Vector3){ 100.0f, 0.0f, 100.0f }});
        Vector3 mouse_pointer_3d0={1*round(collision.point.x/1),1*round(collision.point.y/1),1*round(collision.point.z/1)};
        Vector3 mouse_pointer_3d={collision.point.x,collision.point.y,collision.point.z};
        cursor.x=mouse_pointer_3d0.x;
        cursor.y=mouse_pointer_3d0.y;
        cursor.z=mouse_pointer_3d0.z;
    

        if (IsKeyPressed('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
            scene__add_voxel(&scene,cursor.x,cursor.y,cursor.z,4);
            printf("left click\n");
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            printf("right click\n");
            pos_init=(Vector3){camera.position.x,camera.position.y,camera.position.z};
            lastMousePos = GetMousePosition();
            mouse_init=GetMousePosition();
            start_orbit=1;
            double azimuth0 = atan2(camera.target.z-camera.position.z,camera.target.x-camera.position.x); // 2 * PI for full horizontal rotation
            double ground_dist= sqrt(pow(camera.target.z-camera.position.z,2) + pow(camera.target.x-camera.position.x,2));
            double elevation0 = atan2(camera.target.y-camera.position.y,ground_dist); // PI / 2 for vertical rotation within reasonable limits
        }
        if (start_orbit && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
            Vector2 mouseDelta = Vector2Subtract(GetMousePosition(), lastMousePos);
            float orbitRadius = Vector3Distance(camera.position, camera.target);
            Vector2 mouse_orbit = GetMousePosition();
            double azimuth = (mouseDelta.x / screenWidth) * PI; // 2 * PI for full horizontal rotation
            double elevation = (mouseDelta.y / screenHeight) * PI / 2; // PI / 2 for vertical rotation within reasonable limits

            // Horizontal orbit (azimuth)
            camera.position.x = camera.target.x + cos(azimuth + 0) * orbitRadius * cos(elevation+0);
            camera.position.z = camera.target.z + sin(azimuth + 0) * orbitRadius * cos(elevation+0);

            // Vertical orbit (elevation)
            camera.position.y = camera.target.y + sin(elevation+0) * orbitRadius;
        }
        if (start_orbit && IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)){
            start_orbit=0;
            lastMousePos = GetMousePosition();
        }
        snprintf(status,100,"collision t(%3.2f %3.2f %3.2f) p(%3.2f %3.2f %3.2f)",collision.point.x,collision.point.y,collision.point.z,camera.position.x,camera.position.y,camera.position.z);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);

        // Render the scene
        scene__render0(&scene);
        scene__render_voxel(&scene,&cursor);
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


        DrawRectangle( 0, screenHeight-20, screenWidth, 20, Fade(DARKGRAY, 0.5f));
        DrawText(status     , 5, screenHeight-15, 10, Fade(BLACK, 0.5f));

        EndDrawing();
    }

    CloseWindow();

    return 0;
}