#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "lib.h"
#include "liblog.h"
#include "libsdl.h"
#include "camera.h"
#include "voxel.h"
#include "scene.h"



typedef struct horizontal_plane_grid_s{
    float x, y, z;          // Center position of the grid
    float line_spacing;     // Space between lines
    unsigned int number_of_lines;  // How many lines in each direction (total lines = number_of_lines^2)
} horizontal_plane_grid_t;

intersection_result_t horizontal_plane_grid__intersects(const horizontal_plane_grid_t* grid,const ray_t* ray) {
    intersection_result_t result = {false, {0, 0, 0}};

    // The normal vector of the horizontal plane (pointing upwards)
    vector_t normal = {0.0f, 1.0f, 0.0f};

    // Direction vector of the ray
    vector_t direction = {
        ray->through_point.x - ray->origin.x,
        ray->through_point.y - ray->origin.y,
        ray->through_point.z - ray->origin.z
    };

    // Normalize the direction vector
    float dir_magnitude = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    direction.x /= dir_magnitude;
    direction.y /= dir_magnitude;
    direction.z /= dir_magnitude;

    // Check if the ray is parallel to the plane
    float dot = normal.x * direction.x + normal.y * direction.y + normal.z * direction.z;
    if (fabs(dot) < 1e-6) {
        // Ray is parallel to the plane, no intersection
        return result;
    }

    // Calculate the distance from the ray origin to the intersection point on the plane
    vector_t plane_point = {grid->x, grid->y, grid->z}; // A point on the plane
    vector_t origin_to_plane = {
        plane_point.x - ray->origin.x,
        plane_point.y - ray->origin.y,
        plane_point.z - ray->origin.z
    };
    float t = (normal.x * origin_to_plane.x + normal.y * origin_to_plane.y + normal.z * origin_to_plane.z) / dot;

    // If t is negative, the intersection is behind the ray's origin, so no intersection
    if (t < 0) {
        LOG_W0("plane point outside")
        return result;
    }

    // Calculate the intersection point
    LOG_W0("plane point on plane")
    result.intersects = true;
    result.point.x = ray->origin.x + t * direction.x;
    result.point.y = ray->origin.y + t * direction.y;
    result.point.z = ray->origin.z + t * direction.z;

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
    #define screen_width 800
    #define screen_height 600
    scene_t scene;
    BOOL running = true;
    camera_t camera;
    horizontal_plane_grid_t grid;
    voxel_t cursor;
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
            int mx,my;
            int buttons = SDL_GetMouseState(&mx, &my);
            float mouse_x=2.0f * (mx-screen_width/2.0f)/screen_width;
            float mouse_y=2.0f * (screen_height/2.0f - my)/screen_height;
            if(buttons){
                ray_t ray=camera__get_ray(&camera,mouse_x,mouse_y);
                LOG_E("camera o(%3.2f %3.2f %3.2f) t(%3.2f %3.2f %3.2f)",camera.eye.x,camera.eye.y,camera.eye.z,camera.target.x,camera.target.y,camera.target.z)
                LOG_D("ray o(%3.2f %3.2f %3.2f) t(%3.2f %3.2f %3.2f)",ray.origin.x,ray.origin.y,ray.origin.z,ray.through_point.x,ray.through_point.y,ray.through_point.z)
                intersection_result_t intersection = scene__intersects_ray(&scene,&ray);
                if(!intersection.intersects) {
                    LOG_D0("getting intersection with ground plane")
                    intersection = horizontal_plane_grid__intersects(&grid,&ray);
                }

                LOG_E("MOUSE_CLICKED %0.4f %0.4f -> %3.2f %3.2f %3.2f",mouse_x,mouse_y,intersection.point.x,intersection.point.y,intersection.point.z)
                if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                    LOG_W("ADD_VOXEL %0.4f %0.4f -> %3.2f %3.2f %3.2f",mouse_x,mouse_y,intersection.point.x,intersection.point.y,intersection.point.z)
                } else if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
                    LOG_W("REMOVE_VOXEL %0.4f %0.4f -> %3.2f %3.2f %3.2f",mouse_x,mouse_y,intersection.point.x,intersection.point.y,intersection.point.z)
                } 
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            //float mouse_x=2.0f * (event.motion.x-screen_width/2.0f)/screen_width;
            //float mouse_y=2.0f * (screen_height/2.0f - event.motion.y)/screen_height;
            float mouse_x=(event.motion.x-((float)screen_width)/2.0f)/((float)screen_width/2.0f);
            float mouse_y=(((float)screen_height)/2.0f- event.motion.y)/((float)screen_height/2.0f);
            ray_t ray=camera__get_ray(&camera,mouse_x,mouse_y);
            intersection_result_t intersection = horizontal_plane_grid__intersects(&grid,&ray);
            LOG_D("mouse : px(%4d %4d) %%(%3.2f %3.2f) model(%3.2f %3.2f %3.2f)",event.motion.x,event.motion.y,mouse_x,mouse_y,intersection.point.x,intersection.point.y,intersection.point.z);
            LOG_D("ray o(%3.2f %3.2f %3.2f) t(%3.2f %3.2f %3.2f)",ray.origin.x,ray.origin.y,ray.origin.z,ray.through_point.x,ray.through_point.y,ray.through_point.z)
            cursor.x=ray.through_point.x;
            cursor.y=ray.through_point.y;
            cursor.z=ray.through_point.z;
        }
    }
}


int main(int argc, char *argv[]) {

    const char* font_name="fonts/Arcade.ttf";
    int pt_size=24;
    if(argc>=2){
        font_name=(argv[1]);
    }
    if(argc>=3){
        pt_size=atoi(argv[2]);
    }
    TTF_Font *font = TTF_OpenFont(font_name, pt_size);


    scene__init(&scene);
    /// scene__add_voxel(&scene,0,1,0,1);
    /// scene__add_voxel(&scene,0,2,0,1);
    /// scene__add_voxel(&scene,0,3,0,1);
    /// scene__add_voxel(&scene,0,4,0,1);
    /// scene__add_voxel(&scene,0,5,0,1);
    /// scene__add_voxel(&scene,0,0,0,1);
    /// scene__add_voxel(&scene,1,0,0,1);
    /// scene__add_voxel(&scene,2,0,0,1);
    /// scene__add_voxel(&scene,3,0,0,1);
    /// scene__add_voxel(&scene,4,0,0,1);
    /// scene__add_voxel(&scene,5,0,0,1);
    /// scene__add_voxel(&scene,6,0,0,1);
    /// scene__add_voxel(&scene,6,0,1,1);
    /// scene__add_voxel(&scene,6,0,2,1);
    /// scene__add_voxel(&scene,6,0,3,1);
    /// scene__add_voxel(&scene,6,0,4,1);

    voxel__init(&cursor,0,0,0,2);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDL2 OpenGL Voxel with Camera Movement", 100, 100, screen_width, screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    camera__init(&camera, 45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    camera__rotate_vertical_around_target(&camera, 0.7f);
    camera__move_by(&camera,0.5f,0.0f,-0.5f);

    voxel_t voxel = {0.0f, 0.0f, 0.0f, 1}; // Example voxel
    grid = (horizontal_plane_grid_t){0.0f, 0.0f, 0.0f, 1.0f,40}; // Example grid
    
    SDL_Event event;
    while (running) {
        handleEvents();
        // camera__rotate_around_target(&camera, 0.042f);

        scene__render(&scene, &camera);
        horizontal_plane_grid__render(&grid, &camera);
        voxel__render(&cursor,&camera);
        SDL_Color color=(SDL_Color){200,100,0,100};
        // GL_DRAW_TEXT(1,10,10,font,color,"hello %s","world");
        SDL_GL_SwapWindow(window);
    }
    
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}