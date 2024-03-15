
#include <stdio.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
/////////// #include "external/raygui.h"

/////////// #include "external/raylib-nuklear.h"

#include <raymath.h>
#include "vxdi-app-voxel.h"
#include "vxdi-app-scene.h"
#include "vxdi-lib-general.h"
#include "vxdi-app-camera-orbit-control.h"
#include "vxdi-rl-models-extras.h"
#include "vxdi-rl-math-extras.h"
#include "vxdi-rl-ui-extras.h"
#include "vxdi-app-multistep-tool.h"
#include "vxdi-app-editor.h"
#include "vxdi-app-voxel-rasterizers.h"
#include "vxdi-lib-shadowmapping.h"


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


void voxel_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("voxel tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
}
void voxel_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("voxel tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
}

void shell_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("shell tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(tool->num_inputs==1){
        scene__add_voxel(&(app->construction_hints),tool->inputs[0],app->current_color,app->current_color_index);
    } else if(tool->num_inputs==2){
        rasterizeHollowCube(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void shell_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("shell tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    
    if(IsKeyDown(KEY_LEFT_ALT)){
        printf("shell tool : removing [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeHollowCube(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__remove_voxel);
    } else {
        printf("shell tool : adding [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeHollowCube(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void line_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("line tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(tool->num_inputs==1){
        scene__add_voxel(&(app->construction_hints),tool->inputs[0],app->current_color,app->current_color_index);
    } else if(tool->num_inputs==2){
        rasterizeLine(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void line_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("line tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(IsKeyDown(KEY_LEFT_ALT)){
        printf("line tool : removing [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeLine(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__remove_voxel);
    } else {
        printf("line tool : adding [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeLine(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void structure_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("structure tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(tool->num_inputs==1){
        scene__add_voxel(&(app->construction_hints),tool->inputs[0],app->current_color,app->current_color_index);
    } else if(tool->num_inputs==2){
        rasterizeStructureCube(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void structure_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("structure tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(IsKeyDown(KEY_LEFT_ALT)){
        printf("plate tool : removing [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeStructureCube(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__remove_voxel);
    } else {
        printf("plate tool : adding [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeStructureCube(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__add_voxel);
    }
    rasterizeStructureCube(tool->inputs[0],point,scene,app->current_color,app->current_color_index,IsKeyPressed(KEY_LEFT_ALT)?scene__remove_voxel:scene__add_voxel);
}
void plate_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("plate tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
}
void plate_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("plate tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(IsKeyDown(KEY_LEFT_ALT)){
        printf("plate tool : removing [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        //rasterizeSolidCube(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__remove_voxel);
    } else {
        printf("plate tool : adding [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        //rasterizeSolidCube(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void volume_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("volume tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(tool->num_inputs==1){
        scene__add_voxel(&(app->construction_hints),tool->inputs[0],app->current_color,app->current_color_index);
    } else if(tool->num_inputs==2){
        rasterizeSolidCube(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void volume_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("volume tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(IsKeyDown(KEY_LEFT_ALT)){
        printf("volume tool : removing [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeSolidCube(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__remove_voxel);
    } else {
        printf("volume tool : adding [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeSolidCube(tool->inputs[0],point,scene,app->current_color,app->current_color_index,scene__add_voxel);
    }
    
}

/// Image SDL_SurfaceToRaylibImage(SDL_Surface* surface) {
///     Image image = {0};
/// 
///     if (surface == NULL) {
///         return image; // Return an empty image structure if the input surface is NULL
///     }
/// 
///     // Ensure the surface is locked before accessing the pixels
///     if (!SDL_MUSTLOCK(surface) || SDL_LockSurface(surface) == 0) {
///         // Initialize a Raylib Image with the surface's dimensions and pixel data
///         // Note: We assume the pixel format is SDL_PIXELFORMAT_RGBA32 (Raylib's default)
///         image.data = malloc(surface->w * surface->h * 4); // Allocate memory for pixels (4 bytes per pixel for RGBA)
///         image.width = surface->w;
///         image.height = surface->h;
///         image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8; // This matches SDL_PIXELFORMAT_RGBA32
///         image.mipmaps = 1; // Default to 1 mip level
/// 
///         // Copy pixel data from the SDL_Surface to the Raylib Image
///         memcpy(image.data, surface->pixels, surface->w * surface->h * 4);
/// 
///         // Unlock the surface if it was locked
///         if (SDL_MUSTLOCK(surface)) {
///             SDL_UnlockSurface(surface);
///         }
///     }
/// 
///     return image;
/// }

////// Texture2D load_texture(const char* filename){
////// 
////// 
//////         SDL_Surface* surface = IMG_Load(filename);
//////         printf(" image loaded \n");
//////         Texture2D texture ; // Convert Image to Texture2D
////// 
//////     if (surface == NULL) {
//////         printf(" surface was null \n");
//////         return texture; // Return an empty image structure if the input surface is NULL
//////     }
//////         printf(" surface was not null \n");
////// 
//////     Image image = {0};
//////     // Ensure the surface is locked before accessing the pixels
//////     // Initialize a Raylib Image with the surface's dimensions and pixel data
//////     // Note: We assume the pixel format is SDL_PIXELFORMAT_RGBA32 (Raylib's default)
//////     image.data = malloc(surface->w * surface->h * 4); // Allocate memory for pixels (4 bytes per pixel for RGBA)
//////     image.width = surface->w;
//////     image.height = surface->h;
//////     image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8; // This matches SDL_PIXELFORMAT_RGBA32
//////     image.mipmaps = 1; // Default to 1 mip level
//////     printf(" image initialized \n");
////// 
//////     // Copy pixel data from the SDL_Surface to the Raylib Image
//////     memcpy(image.data, surface->pixels, surface->w * surface->h * 4);
//////     printf(" memory copied \n");
//////     texture = LoadTextureFromImage(image); // Convert Image to Texture2D
//////     printf(" texture image loaded \n");
//////     SDL_FreeSurface(surface);
//////     UnloadImage(image);
//////     return texture;
////// }

int main(int argc, char *argv[]) {
    int color_btn_size=25;
    int color_btn_spacing=1;
    int left_menu_sz_width = 20 + color_btn_size*5;
    // Initialization
    //--------------------------------------------------------------------------------------
    vxdi_multistep_tool_t voxel_tool;
    multistep_tool__init(&voxel_tool,1,voxel_tool_acquire,voxel_tool_finish);
    vxdi_multistep_tool_t shell_tool;
    multistep_tool__init(&shell_tool,2,shell_tool_acquire,shell_tool_finish);
    vxdi_multistep_tool_t structure_tool;
    multistep_tool__init(&structure_tool,2,structure_tool_acquire,structure_tool_finish);
    vxdi_multistep_tool_t plate_tool;
    multistep_tool__init(&plate_tool,3,plate_tool_acquire,plate_tool_finish);
    vxdi_multistep_tool_t line_tool;
    multistep_tool__init(&line_tool,2,line_tool_acquire,line_tool_finish);
    vxdi_multistep_tool_t volume_tool;
    multistep_tool__init(&volume_tool,2,volume_tool_acquire,volume_tool_finish);
    // Set window to be resizable


    // DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    SetExitKey(KEY_Q); // Use KEY_NULL or 0 to disable

    Camera3D camera;
    scene_t scene;
    vxdi_app_editor_t app=vxdi_app_editor__setup(&camera,(Vector3){ 0.7f, -1.0f, 0.3f });
    scene__init(&scene,1,app.light_direction);
    scene.temp_filename="temp.vxdi";
    if(argc >1) {
        scene.temp_filename = argv[1];
    }

    scene__load_model(&scene,scene.temp_filename);

    // Example: Add a voxel to the scene
    // scene__add_voxel(&scene, (Vector3){0.0f, 0.0f, 0.0f}, RED,1);
    voxel_t cursor={20,30,20,3};
    voxel_t cursor2={20,30,20,2};

    // SetCameraMode(camera, CAMERA_FREE); // Let Raylib handle camera controls
    
    orbit_t orbiter=orbit_init(&camera);


    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(app.screenWidth, app.screenHeight, "Raylib Voxel Scene");
    /// InitShadowMapping();

    mut char status[500];
    mut int ctrl,left_ctrl;
    mut char show_help=0;
    char window_should_close=0;
    Vector2 current_mouse_position=GetMousePosition();
    Vector2 previous_mouse_position=current_mouse_position;
    char is_mouse_position_changed = 1;
    unsigned char dbg_move_number=0;



    printf(" loading buttons texture \n");
    /// Texture2D buttons_tex=load_texture("buttons.svg");
    printf(" got buttons texture \n");

    for (;!(WindowShouldClose() || window_should_close);) {
        if(IsKeyPressed(KEY_ESCAPE)){
            // do nothing
        }

        
        current_mouse_position=GetMousePosition();
        if(Vector2Length(Vector2Subtract(current_mouse_position,previous_mouse_position)) > 5) {
            previous_mouse_position=current_mouse_position;
            is_mouse_position_changed=1;
            dbg_move_number=(dbg_move_number+is_mouse_position_changed)%256;
        } else {
            is_mouse_position_changed=0;
        }

        if(1 || is_mouse_position_changed){
            // UpdateCamera(&camera, app.current_camera_mode );// Update camera position/movement
            vxdi_app_editor__update(&app);


            // 1. First pass, from light's perspective
            // BeginShadowPass();
            //     RenderModelWithShadows(&scene, camera, depthShader); // Use depth shader
            // EndShadowPass();

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
            if( abs(keyboard_wait_time=vxdi_app_editor__InputMathExpr(&app))  ){
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


            if (IsKeyPressed('Z')) orbiter=orbit_init(&camera);
            
            
            orbit__control_camera(&orbiter);

            snprintf(status,500,"num voxels: %d construction mode : %d, text_buffer %40s  voxels %d/%d temp file %s",scene.numVoxels,app.construction_mode,app.text_buffer,scene.numVoxels,MAX_VOXELS,scene.temp_filename);

            BeginDrawing();
            // ClearBackground(RAYWHITE);
            // DrawCircle(app.screenWidth/2,app.screenHeight/2,app.screenHeight/2.0f-10.0f,RED);
            ClearBackground(GRAY);

            BeginMode3D(camera);

                /// BeginScenePass();
                ///    RenderModelWithShadows(&scene, camera, shadowShader); // Use shadow shader
                ///    // Render the scene
                ///    // 
                /// EndScenePass();
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
                Ray ray = GetMouseRay(current_mouse_position, camera);
                // You can then use Raylib functions to check for intersections, etc.
                if(current_mouse_position.x>left_menu_sz_width && current_mouse_position.x<(app.screenWidth-70)){
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




            if(IsKeyPressed(KEY_SPACE)){
                scene__clear(&app.guides);
                scene__clear(&app.construction_hints);
                multistep_tool__reset(&structure_tool);
                multistep_tool__reset(&plate_tool);
                multistep_tool__reset(&shell_tool);
                multistep_tool__reset(&volume_tool);
                multistep_tool__reset(&voxel_tool);
            }
            /// DrawFPS(10, 10);


            // draw and check buttons on the right side

            // DrawTextureRec(buttons_tex, (Rectangle){0,0,64,448}, (Vector2){app.screenWidth-70,32}, WHITE);

            int Y=(int)((current_mouse_position.y-32)/64);
            // DrawRectangle(app.screenWidth-70,Y*64+32,64,64,(Color){128,255,255,128});
            if(
                current_mouse_position.x>(app.screenWidth-70)
            ){
                int Y=(int)((current_mouse_position.y-32)/64);
                DrawRectangle(app.screenWidth-70,Y*64+32,64,64,(Color){128,255,255,128});
                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                    if(Y==0){
                        show_help=!show_help;
                    } else {
                        app.construction_mode=(app_construction_mode_e)(Y+0x100);
                    }
                }
            }
            DrawRectangle(app.screenWidth-70,(((int)(app.construction_mode))&0xFF)*64+32,64,64,(Color){128,255,255,128});

            //feed mouse events in function of the selected tool
            if (
                current_mouse_position.x>left_menu_sz_width && 
                current_mouse_position.x<(app.screenWidth-70) && 
                (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || is_mouse_position_changed) &&
                !IsKeyDown(KEY_LEFT_CONTROL) &&
                !IsKeyDown(KEY_LEFT_SHIFT)
            ){
                switch(app.construction_mode){
                    case APP_CONSTRUCTION_MODE_HELP:
                    break;
                    case APP_CONSTRUCTION_MODE_SELECT:break;
                    case APP_CONSTRUCTION_MODE_VOXEL:
                        if (
                            current_mouse_position.x>left_menu_sz_width && 
                            current_mouse_position.x<(app.screenWidth-100) && 
                            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                            !IsKeyDown(KEY_LEFT_CONTROL) &&
                            !IsKeyDown(KEY_LEFT_SHIFT)
                        ){
                            if(IsKeyDown(KEY_LEFT_ALT)){
                                scene__remove_voxel(&scene,model_point_int,app.current_color,app.current_color_index);
                            } else {
                                scene__add_voxel(&scene,model_point_next_int,app.current_color,app.current_color_index);
                            }
                        }
                    break;
                    case APP_CONSTRUCTION_MODE_LINE:
                        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            printf("LINE tool click %d\n",APP_CONSTRUCTION_MODE_LINE);
                            multistep_tool__receive_point(&line_tool,&app,&scene,model_point_int);
                        } else if(is_mouse_position_changed){
                            printf("LINE tool move[%d] %d\n",dbg_move_number,APP_CONSTRUCTION_MODE_LINE);
                            multistep_tool__receive_moving_point(&line_tool,&app,model_point_int);
                        }
                    break;
                    case APP_CONSTRUCTION_MODE_STRUCTURE:
                        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            printf("SHELL tool click %d\n",APP_CONSTRUCTION_MODE_STRUCTURE);
                            multistep_tool__receive_point(&structure_tool,&app,&scene,model_point_int);
                        } else if(is_mouse_position_changed){
                            printf("SHELL tool move[%d] %d\n",dbg_move_number,APP_CONSTRUCTION_MODE_STRUCTURE);
                            multistep_tool__receive_moving_point(&structure_tool,&app,model_point_int);
                        }
                    break;
                    case APP_CONSTRUCTION_MODE_SHELL:
                        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            printf("SHELL tool click %d\n",APP_CONSTRUCTION_MODE_SHELL);
                            multistep_tool__receive_point(&shell_tool,&app,&scene,model_point_int);
                        } else if(is_mouse_position_changed){
                            printf("SHELL tool move[%d] %d\n",dbg_move_number,APP_CONSTRUCTION_MODE_SHELL);
                            multistep_tool__receive_moving_point(&shell_tool,&app,model_point_int);
                        }
                    break;
                    case APP_CONSTRUCTION_MODE_VOLUME:
                        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            printf("VOLUME tool click %d\n",APP_CONSTRUCTION_MODE_VOLUME);
                            multistep_tool__receive_point(&volume_tool,&app,&scene,model_point_int);
                        } else if(is_mouse_position_changed){
                            printf("VOLUME tool move[%d] %d\n",dbg_move_number,APP_CONSTRUCTION_MODE_VOLUME);
                            multistep_tool__receive_moving_point(&volume_tool,&app,model_point_int);
                        }
                    break;
                    case APP_CONSTRUCTION_MODE_PLATE:
                        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            printf("PLATE tool click %d\n",APP_CONSTRUCTION_MODE_PLATE);
                            multistep_tool__receive_point(&plate_tool,&app,&scene,model_point_int);
                        } else if(is_mouse_position_changed){
                            printf("PLATE tool move[%d] %d\n",dbg_move_number,APP_CONSTRUCTION_MODE_PLATE);
                            multistep_tool__receive_moving_point(&plate_tool,&app,model_point_int);
                        }
                    break;
                    default:
                    window_should_close=1;
                    break;
                }
            }
            // draw color pallete
            for(int ci=0;ci<24;ci+=1){
                for(int lum=-1;lum<2;lum+=1){
                    Vector2 pos = {20+color_btn_size+color_btn_spacing+lum*(color_btn_size+color_btn_spacing), (color_btn_size+color_btn_spacing)+ci*color_btn_size};
                    Color cl=ColorBrightness(app.colors[ci*15],((float)lum*5.0f)/10.0f);
                    if (ext_SquareButton(pos,(color_btn_size-color_btn_spacing)/2,cl) && current_mouse_position.x<left_menu_sz_width) {
                        app.current_color = cl;
                        app.current_color_index = ci*10 + lum+1;
                        // goto start;
                    }
                    if(app.current_color_index == ci*10 + lum+1){
                        DrawRectangleLines(pos.x-color_btn_size/2,pos.y-color_btn_size/2,color_btn_size,color_btn_size,WHITE);
                    }
                }
            }
            for(int lum=0;lum<24;lum+=1){
                Vector2 pos = {20+3*(color_btn_size+color_btn_spacing), (color_btn_size+color_btn_spacing)+lum*color_btn_size};
                Color cl=ColorFromHSV(0,0,(lum+1)/24.0f);
                if (ext_SquareButton(pos,(color_btn_size-color_btn_spacing)/2,cl) && current_mouse_position.x<left_menu_sz_width) {
                    app.current_color = cl;
                    app.current_color_index = 0xFFFF+lum;
                    // goto start;
                }
                if(app.current_color_index == 0xFFFF+lum){
                    DrawRectangleLines(pos.x-color_btn_size/2,pos.y-color_btn_size/2,color_btn_size,color_btn_size,WHITE);
                }
            }
            // draw color pallete
            for(int ci=0;ci<24;ci+=1){
                    Vector2 pos = {20+4*(color_btn_size+color_btn_spacing), (color_btn_size+color_btn_spacing)+ci*color_btn_size};
                    Color cl=Fade(app.colors[ci*15],0.2f);
                    if (ext_SquareButton(pos,(color_btn_size-color_btn_spacing)/2,cl) && current_mouse_position.x<left_menu_sz_width) {
                        app.current_color = cl;
                        app.current_color_index = ci*0x100;
                        // goto start;
                    }
                    if(app.current_color_index == ci*0x100){
                        DrawRectangleLines(pos.x-color_btn_size/2,pos.y-color_btn_size/2,color_btn_size,color_btn_size,WHITE);
                    }
            }

            DrawRectangle( 0, 0, app.screenWidth, 20, Fade(DARKGRAY, 0.95f));
            DrawText(status     , app.screenWidth-300, 5, 10, (Color){200,200,200,255});
            char coords[12];
            snprintf(coords,12,"%3d %3d %3d",(int)model_point_int.x,(int)model_point_int.y,(int)model_point_int.z);
            DrawText(coords, current_mouse_position.x+10, current_mouse_position.y-10, 20, BLACK);

            // Draw the shadow map texture
            DrawTextureRec(shadowMap.texture, (Rectangle){ 0, 0, shadowMap.texture.width, shadowMap.texture.height }, (Vector2){ 100, 1000 }, WHITE);
            /// DrawFPS(10, 10);

            EndDrawing();

        }/** endif mouse cursor changed */



        if(show_help){
            int row=60;
            int left=app.screenWidth-380;
            DrawRectangle( left-10, row, 320, 160, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines( left-10, row, 320, 160, BLUE);
            Rectangle btn_rectangle = (Rectangle){ left+290,row, 20, 20 };
            DrawRectangle(btn_rectangle.x,btn_rectangle.y,btn_rectangle.width,btn_rectangle.height,LIGHTGRAY);
            Vector2 m = current_mouse_position;
            Rectangle b = btn_rectangle;
            if (m.x>b.x && m.x<b.x+b.width && m.y>b.y && m.y<b.y+b.height) {
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
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    multistep_tool__deinit(&voxel_tool);

    UnloadShader(depthShader); // Unload shader
    UnloadShader(shadowShader); // Unload shader
    UnloadRenderTexture(shadowMap); // Unload render texture
    CloseWindow();
    /// scene__save_model(&scene,"temp.vxde");

    return 0;
}
