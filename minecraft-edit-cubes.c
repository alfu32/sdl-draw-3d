#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "lib.h"
#include "liblog.h"
#include "camera.h"
#include "voxel.h"
#include "scene.h"



typedef struct horizontal_plane_grid_s{
    float x, y, z;          // Center position of the grid
    float line_spacing;     // Space between lines
    unsigned int number_of_lines;  // How many lines in each direction (total lines = number_of_lines^2)
} horizontal_plane_grid_t;

intersection_result_t horizontal_plane_grid__intersects(horizontal_plane_grid_t* grid,ray_t* ray) {
    intersection_result_t result;
    result.intersects = false;
    
    // The normal of a horizontal plane
    vector_t normal = {0.0f, 1.0f, 0.0f};
    
    // Calculate the dot product of the ray direction and the normal
    vector_t ray_direction = {
        ray->through_point.x - ray->origin.x,
        ray->through_point.y - ray->origin.y,
        ray->through_point.z - ray->origin.z
    };
    float denominator = normal.x * ray_direction.x + normal.y * ray_direction.y + normal.z * ray_direction.z;
    
    // If the denominator is close to 0, the ray is parallel to the plane
    if (fabs(denominator) > 1e-6) {
        vector_t plane_point = {grid->x, grid->y, grid->z}; // A point on the plane
        vector_t origin_to_plane = {
            plane_point.x - ray->origin.x,
            plane_point.y - ray->origin.y,
            plane_point.z - ray->origin.z
        };
        
        float t = (normal.x * origin_to_plane.x + normal.y * origin_to_plane.y + normal.z * origin_to_plane.z) / denominator;
        
        // If t is negative, the intersection is behind the ray's origin
        if (t >= 0) {
            result.intersects = true;
            result.point.x = ray->origin.x + t * ray_direction.x;
            result.point.y = ray->origin.y + t * ray_direction.y;
            result.point.z = ray->origin.z + t * ray_direction.z;
        }
    }
    
    return result;
}

void horizontal_plane_grid__render(horizontal_plane_grid_t* self, camera_t* camera) {
    // Setup camera view
    camera__setup_projection(camera);

    // Calculate half the total grid size to start drawing from one end
    float half_grid_extent = self->line_spacing * (self->number_of_lines - 1) / 2;

    // Set grid color
    glColor3f(0.5f, 0.5f, 0.5f); // Gray color

    // Draw grid lines parallel to the X-axis
    glBegin(GL_LINES);
    for (int i = 0; i < self->number_of_lines; ++i) {
        float z = self->z - half_grid_extent + i * self->line_spacing;
        glVertex3f(self->x - half_grid_extent, self->y, z);
        glVertex3f(self->x + half_grid_extent, self->y, z);
    }
    // Draw grid lines parallel to the Z-axis
    for (int i = 0; i < self->number_of_lines; ++i) {
        float x = self->x - half_grid_extent + i * self->line_spacing;
        glVertex3f(x, self->y, self->z - half_grid_extent);
        glVertex3f(x, self->y, self->z + half_grid_extent);
    }
    glEnd();
}

    scene_t scene;
    BOOL running = true;
    camera_t camera;
    horizontal_plane_grid_t grid;
// Function that handles events
void handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                camera.aspect = (GLfloat)event.window.data1 / (GLfloat)event.window.data2;
            } else if (event.type == SDL_KEYDOWN){
                switch(event.key.keysym.scancode){
                    case SDL_SCANCODE_ESCAPE:running = false;break;
                    case SDL_SCANCODE_Z:camera__rotate_around_target(&camera, 0.042f);break;
                    case SDL_SCANCODE_C:camera__rotate_around_target(&camera, -0.042f);break;
                    case SDL_SCANCODE_W:camera__advance(&camera,-0.5f);break;
                    case SDL_SCANCODE_S:camera__advance(&camera,0.5f);break;
                    case SDL_SCANCODE_A:camera__strife(&camera,0.5f);break;
                    case SDL_SCANCODE_D:camera__strife(&camera,-0.5f);break;
                    case SDL_SCANCODE_R:camera__rotate_vertical_around_target(&camera, 0.01f);break;
                    case SDL_SCANCODE_F:camera__rotate_vertical_around_target(&camera, -0.01f);break;
                }
            } else if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.direction == SDL_MOUSEWHEEL_NORMAL){
                    LOG_W("SDL_MOUSEWHEEL_NORMAL %3.4f %3.4f %d",event.wheel.preciseX,event.wheel.preciseY,event.wheel.direction)
                    camera__move_camera_along_direction(&camera,event.wheel.preciseY*0.5f);
                } else if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED){
                    LOG_W("SDL_MOUSEWHEEL_FLIPPED %3.4f %3.4f %d",event.wheel.preciseX,event.wheel.preciseY,event.wheel.direction)
                    camera__move_camera_along_direction(&camera,event.wheel.preciseY*0.5f);
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouse_x,mouse_y;
                int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
                if(buttons){
                    ray_t ray=camera__get_ray(&camera,(float)mouse_x,(float)mouse_y);
                    intersection_result_t intersection = scene__intersects_ray(&scene,&ray);
                    if(!intersection.intersects) {
                        intersection = horizontal_plane_grid__intersects(&grid,&ray);
                    }

                    LOG_E("MOUSE_CLICKED %4d %4d -> %3.2f %3.2f %3.2f",mouse_x,mouse_y,intersection.point.x,intersection.point.y,intersection.point.z)
                    if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                        LOG_W("ADD_VOXEL %4d %4d -> %3.2f %3.2f %3.2f",mouse_x,mouse_y,intersection.point.x,intersection.point.y,intersection.point.z)
                    } else if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
                        LOG_W("REMOVE_VOXEL %4d %4d -> %3.2f %3.2f %3.2f",mouse_x,mouse_y,intersection.point.x,intersection.point.y,intersection.point.z)
                    } 
                }
            }
    }
}


int main(int argc, char *argv[]) {
    scene__init(&scene);
    scene__add_voxel(&scene,0,0,0,1);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDL2 OpenGL Voxel with Camera Movement", 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    camera__init(&camera, 45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    camera__rotate_vertical_around_target(&camera, 0.7f);
    camera__move_by(&camera,0.5f,0.0f,-0.5f);

    voxel_t voxel = {0.0f, 0.0f, 0.0f, 1}; // Example voxel
    grid = (horizontal_plane_grid_t){0.0f, 0.0f, 0.0f, 1.0f,10}; // Example grid
    
    SDL_Event event;
    while (running) {
        handleEvents();
        // camera__rotate_around_target(&camera, 0.042f);

        scene__render(&scene, &camera);
        horizontal_plane_grid__render(&grid, &camera);
        SDL_GL_SwapWindow(window);
    }
    
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}