#ifndef __VXDI_APP_CAMERA_ORBIT_CONTROL_H__
#define __VXDI_APP_CAMERA_ORBIT_CONTROL_H__

#include <raylib.h>
#include <raymath.h>
#include "vxdi-lib-general.h"


typedef stack struct orbit_s{
    owned mut Vector2 lastMousePos;
    owned mut float azimuth; // Horizontal angle
    owned mut float elevation; // Vertical angle
    owned mut float radius; // Distance from the target
    owned mut bool isOrbiting;
    owned mut bool isPanning;
    borrowed Camera *camera;
} orbit_t;

orbit_t orbit_init(Camera *camera){
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

int orbit__control_camera(orbit_t* orbiter){
    Vector2 mouseDelta = Vector2Subtract(GetMousePosition(), orbiter->lastMousePos);
    orbiter->lastMousePos = GetMousePosition();
    float current_camera_dist = Vector3Length(Vector3Subtract(orbiter->camera->position,orbiter->camera->target));

    // Check for right mouse button pressed for orbiting
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)) {
        orbiter->isOrbiting = true;
        orbiter->isPanning = false;
    } else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)) {
        // Check for right mouse button pressed with Shift for panning
        orbiter->isPanning = true;
        orbiter->isOrbiting = false;
    } else {
        orbiter->isOrbiting = orbiter->isPanning = false;
    }

    if (orbiter->isOrbiting) {
        orbiter->azimuth -= mouseDelta.x * 0.003f;
        if(fabsf(orbiter->elevation + mouseDelta.y * 0.003f) < 1.57){
            orbiter->elevation += mouseDelta.y * 0.003f;
        }
    } else if (orbiter->isPanning) {
        Vector3 right = Vector3Normalize(Vector3CrossProduct(Vector3Subtract(orbiter->camera->position, orbiter->camera->target), orbiter->camera->up));
        Vector3 up = Vector3Normalize(Vector3CrossProduct(right, Vector3Subtract(orbiter->camera->position, orbiter->camera->target)));
        float panSpeed = current_camera_dist*0.00075f;
        orbiter->camera->target = Vector3Add(orbiter->camera->target, Vector3Scale(right, mouseDelta.x * panSpeed));
        orbiter->camera->target = Vector3Add(orbiter->camera->target, Vector3Scale(up, mouseDelta.y * panSpeed));
        orbiter->camera->position = Vector3Add(orbiter->camera->position, Vector3Scale(right, mouseDelta.x * panSpeed));
        orbiter->camera->position = Vector3Add(orbiter->camera->position, Vector3Scale(up, mouseDelta.y * panSpeed));
    }

    orbiter->radius -= GetMouseWheelMove() * current_camera_dist *0.08f;
    orbiter->radius = Clamp(orbiter->radius, 1.0f, 200000000.0f);

    if (orbiter->isOrbiting || !orbiter->isPanning) { // Update position only if orbiting or not panning
        orbiter->camera->position.x = orbiter->camera->target.x + orbiter->radius * cosf(orbiter->elevation) * sinf(orbiter->azimuth);
        orbiter->camera->position.y = orbiter->camera->target.y + orbiter->radius * sinf(orbiter->elevation);
        orbiter->camera->position.z = orbiter->camera->target.z + orbiter->radius * cosf(orbiter->elevation) * cosf(orbiter->azimuth);
    }
    return 0;
}

#endif