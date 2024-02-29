#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "liblog.h"
#include "vector.h"
#include "ray.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846264338327950288
#endif

typedef struct {
    vector_t eye;       // Camera's position
    vector_t target;    // Where the camera is looking
    vector_t up;        // The up direction from the camera's perspective
    GLfloat fov;    // Field of view
    GLfloat aspect; // Aspect ratio
    GLfloat near;   // Near clipping plane
    GLfloat far;    // Far clipping plane
} camera_t;

void camera__init(camera_t *camera, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far) {
    camera->eye = (vector_t){0.0f, 0.0f, 5.0f};
    camera->target = (vector_t){0.0f, 0.0f, 0.0f};
    camera->up = (vector_t){0.0f, 1.0f, 0.0f};
    camera->fov = fov;
    camera->aspect = aspect;
    camera->near = near;
    camera->far = far;
}

void camera__setup_projection(camera_t *camera) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera->fov, camera->aspect, camera->near, camera->far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera->eye.x, camera->eye.y, camera->eye.z, 
              camera->target.x, camera->target.y, camera->target.z, 
              camera->up.x, camera->up.y, camera->up.z);
}
void camera__rotate_around_target(camera_t* self, float angle) {
    // Distance between the camera and the target
    float distance = sqrtf(powf(self->eye.x - self->target.x, 2) +
                           powf(self->eye.z - self->target.z, 2));
    
    // Current angle to target in radians
    float currentAngle = atan2f(self->eye.x - self->target.x, self->eye.z - self->target.z);
    
    // Calculate new angle
    float newAngle = currentAngle + angle;
    
    // Calculate new camera position
    self->eye.x = self->target.x + sinf(newAngle) * distance;
    self->eye.z = self->target.z + cosf(newAngle) * distance;
}

void camera__rotate_vertical_around_target(camera_t* camera, GLfloat angle) {
    // Vector from the eye to the target
    vector_t direction = {
        camera->eye.x - camera->target.x,
        camera->eye.y - camera->target.y,
        camera->eye.z - camera->target.z
    };

    // Calculate horizontal distance polar_rxz to maintain the distance polar_r during vertical rotation
    GLfloat polar_r = sqrtf(direction.x * direction.x + direction.z * direction.z + direction.y * direction.y);
    GLfloat polar_rxz = sqrtf(direction.x * direction.x + direction.z * direction.z);
    GLfloat polar_a = atan2f(direction.z,direction.x);
    GLfloat polar_f = atan2f(direction.y,polar_rxz);

    // New vertical angle after rotation
    GLfloat desired_polar_f = polar_f + angle;
    //// // Limit the vertical angle to prevent the camera from flipping over
    //// const GLfloat maxVerticalAngle = (M_PI-0.167f) / 2; // Just shy of vertical to avoid gimbal lock
    //// if(newVerticalAngle>maxVerticalAngle) newVerticalAngle = maxVerticalAngle;

    // Calculate new eye position
    GLfloat newY = sinf(desired_polar_f) * polar_r;
    GLfloat desired_polar_rxz = cosf(desired_polar_f) * polar_r;

    // Maintain the camera's horizontal orientation
    // GLfloat ratio = newHorizontalDistance / horizontalDistance;
    // Lfloat newX = camera->target.x - direction.x * ratio;
    // GLfloat newZ = camera->target.z - direction.z * ratio;

    GLfloat newX = cosf(polar_a) * desired_polar_rxz;
    GLfloat newZ = sinf(polar_a) * desired_polar_rxz;
    // Update camera's eye position
    camera->eye.x = camera->target.x + newX;
    camera->eye.y = camera->target.y + newY;
    camera->eye.z = camera->target.z + newZ;

    // Vector from the eye to the target
    vector_t new_d = {
        camera->eye.x - camera->target.x,
        camera->eye.y - camera->target.y,
        camera->eye.z - camera->target.z
    };
    GLfloat resulted_polar_r=sqrtf(new_d.x * new_d.x + new_d.z * new_d.z + new_d.y * new_d.y);
    GLfloat resulted_polar_rxz = sqrtf(new_d.x * new_d.x + new_d.z * new_d.z);
    GLfloat resulted_polar_a = atan2f(new_d.z,new_d.x);
    GLfloat resulted_polar_f = atan2f(new_d.y,resulted_polar_rxz);
    LOG_D("a: %3.4f",angle)
    LOG_D("incoming     r: %3.4f,  rxz: %3.4f,  a: %3.4f,  f: %3.4f",polar_r,polar_rxz,polar_a,polar_f)
    LOG_D("calculated   r: %3.4f,  rxz: %3.4f,  a: %3.4f,  f: %3.4f",polar_r,desired_polar_rxz,polar_a,desired_polar_f)
    LOG_D("result       r: %3.4f,  rxz: %3.4f,  a: %3.4f,  f: %3.4f",resulted_polar_r,resulted_polar_rxz,resulted_polar_a,resulted_polar_f)

}


void camera__move_by(camera_t *camera, GLfloat x, GLfloat y, GLfloat z) {
    // Move the eye position
    camera->eye.x += x;
    camera->eye.y += y;
    camera->eye.z += z;

    // Move the target position
    camera->target.x += x;
    camera->target.y += y;
    camera->target.z += z;
}

void camera__slide_along_direction(camera_t* camera, GLfloat distance) {
    // Calculate the direction vector from eye to target
    vector_t direction;
    direction.x = camera->target.x - camera->eye.x;
    direction.y = camera->target.y - camera->eye.y;
    direction.z = camera->target.z - camera->eye.z;

    // Normalize the direction vector
    GLfloat magnitude = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    direction.x /= magnitude;
    direction.y /= magnitude;
    direction.z /= magnitude;

    // Move the eye and target positions along the direction vector
    camera->eye.x += direction.x * distance;
    camera->eye.y += direction.y * distance;
    camera->eye.z += direction.z * distance;

    camera->target.x += direction.x * distance;
    camera->target.y += direction.y * distance;
    camera->target.z += direction.z * distance;
}

void camera__move_camera_along_direction(camera_t* camera, GLfloat distance) {
    // Calculate the direction vector from eye to target
    vector_t direction;
    direction.x = camera->target.x - camera->eye.x;
    direction.y = camera->target.y - camera->eye.y;
    direction.z = camera->target.z - camera->eye.z;

    // Normalize the direction vector
    GLfloat magnitude = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    direction.x /= magnitude;
    direction.y /= magnitude;
    direction.z /= magnitude;

    // Move the eye and target positions along the direction vector
    camera->eye.x += direction.x * distance;
    camera->eye.y += direction.y * distance;
    camera->eye.z += direction.z * distance;
}

void camera__strife(camera_t *self, GLfloat distance) {
    // Calculate direction vector from eye to target
    vector_t direction = {
        self->target.x - self->eye.x,
        0, // Ignore Y to project onto ground plane
        self->target.z - self->eye.z
    };

    // Calculate normal to direction on the ground plane (strife direction)
    vector_t normal = {-direction.z, 0, direction.x};

    // Normalize the normal vector
    GLfloat magnitude = sqrt(normal.x * normal.x + normal.z * normal.z);
    normal.x /= magnitude;
    normal.z /= magnitude;

    // Move the eye and target positions along the normal vector
    self->eye.x += normal.x * distance;
    self->eye.z += normal.z * distance;
    self->target.x += normal.x * distance;
    self->target.z += normal.z * distance;
}

void camera__advance(camera_t* self, GLfloat distance) {
    // Calculate ground plane projection of direction vector from eye to target
    vector_t direction = {
        self->target.x - self->eye.x,
        0, // Project onto the ground plane by setting Y component to 0
        self->target.z - self->eye.z
    };

    // Normalize the direction vector
    GLfloat magnitude = sqrt(direction.x * direction.x + direction.z * direction.z);
    direction.x /= magnitude;
    direction.z /= magnitude;

    // Move the eye and target positions along the direction vector
    self->eye.x += direction.x * distance;
    self->eye.z += direction.z * distance;
    self->target.x += direction.x * distance;
    self->target.z += direction.z * distance;
}
/// @brief The camera__get_ray function will calculate a ray from the camera position
///         through a point on the screen. The screen coordinates (screen_x, screen_y) 
///         should be normalized to the range [-1, 1], where (-1, -1) 
///         is the bottom-left corner of the screen, and (1, 1) is the top-right corner.
/// @param self camera_t*
/// @param screen_x float normalized cursor x position
/// @param screen_y float normalized cursor y position
/// @return 
ray_t camera__get_ray(camera_t* self, float screen_x, float screen_y) {
    // Calculate normalized direction vector from the camera to its target
    vector_t direction = {
        self->target.x - self->eye.x,
        self->target.y - self->eye.y,
        self->target.z - self->eye.z
    };
    normalize(&direction);

    // Calculate the right vector as cross product of direction and world up
    vector_t worldUp = {0, 1, 0}; // Use the Y-axis as world up
    vector_t right;
    cross_product(&direction, &worldUp, &right);
    normalize(&right);

    // Recalculate the true up vector using the right and direction vectors
    vector_t up;
    cross_product(&right, &direction, &up);

    // Account for FOV and aspect ratio to find the size of the plane at the target
    float height = tan(self->fov / 2.0f) * 2.0f;
    float width = self->aspect * height;

    // Calculate offsets based on screen coordinates
    vector_t rightOffset, upOffset;
    scale_vector(&right, screen_x * width / 2.0f, &rightOffset);
    scale_vector(&up, screen_y * height / 2.0f, &upOffset); // Negative to match screen coordinates

    // Calculate final through point by adding offsets to target
    vector_t through_point = {
        self->target.x + rightOffset.x + upOffset.x,
        self->target.y + rightOffset.y + upOffset.y,
        self->target.z + rightOffset.z + upOffset.z
    };

    ray_t ray = {self->eye, through_point};
    return ray;
}

#endif