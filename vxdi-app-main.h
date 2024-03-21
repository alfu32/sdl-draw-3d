
#include <stdio.h>
#include <raylib.h>

#include <raymath.h>
#include "vxdi-app-voxel.h"
#include "vxdi-app-scene.h"
#include "vxdi-lib-general.h"
#include "vxdi-app-camera-orbit-control.h"
#include "vxdi-rl-models-extras.h"
#include "vxdi-rl-math-extras.h"
#include "vxdi-rl-ui-extras.h"
#include "vxdi-app-editor.h"
#include "vxdi-app-voxel-rasterizers.h"
#include "vxdi-app-multistep-tool.h"
#include "vxdi-app-multistep-tool-map.h"
#include "vxdi-lib-shadowmapping.h"

#define SHADOWMAP_RESOLUTION 512

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            120
#endif



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


void generic_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    /// printf("aquire: generic tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
}
void generic_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    printf("finish generic tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
}

void help_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    /// printf("aquire: help tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
}
void help_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    printf("finish help tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
}

void select_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    /// printf("aquire: select tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
}
void select_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    printf("finish select tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
}

void voxel_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    /// printf("aquire: voxel tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    //scene__add_voxel(&(app->construction_hints),point,app->current_color,app->current_color_index);
}
void voxel_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    printf("finish voxel tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);

    if(IsKeyDown(KEY_LEFT_ALT)){
        printf("finish voxel tool : removing [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        scene__remove_voxel(&(app->scene),app->model_point_int,app->current_color,app->current_color_index);
    } else {
        printf("finish voxel tool : adding [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        scene__add_voxel(&(app->scene),app->model_point_next_int,app->current_color,app->current_color_index);
    }
}

void shell_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    /// printf("aquire: shell tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(tool->last_input_index==0){
        scene__add_voxel(&(app->construction_hints),point,app->current_color,app->current_color_index);
    } else if(tool->last_input_index==1){
        rasterizeHollowCube(tool->inputs[0],point,&(app->construction_hints),app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void shell_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    printf("finish shell tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    
    if(IsKeyDown(KEY_LEFT_ALT)){
        printf("shell tool : removing [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeHollowCube(tool->inputs[0],point,&(app->scene),app->current_color,app->current_color_index,scene__remove_voxel);
    } else {
        printf("shell tool : adding [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeHollowCube(tool->inputs[0],point,&(app->scene),app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void line_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    /// printf("aquire: line tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(tool->last_input_index==0){
        scene__add_voxel(&(app->construction_hints),point,app->current_color,app->current_color_index);
    } else if(tool->last_input_index==1){
        rasterizeLine(tool->inputs[0],point,&(app->construction_hints),app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void line_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    printf("finish line tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(IsKeyDown(KEY_LEFT_ALT)){
        printf("line tool : removing [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeLine(tool->inputs[0],point,&(app->scene),app->current_color,app->current_color_index,scene__remove_voxel);
    } else {
        printf("line tool : adding [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeLine(tool->inputs[0],point,&(app->scene),app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void structure_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    /// printf("aquire: structure tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(tool->last_input_index==0){
        scene__add_voxel(&(app->construction_hints),point,app->current_color,app->current_color_index);
    } else if(tool->last_input_index==1){
        rasterizeStructureCube(tool->inputs[0],point,&(app->construction_hints),app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void structure_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    printf("finish structure tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(IsKeyDown(KEY_LEFT_ALT)){
        printf("plate tool : removing [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeStructureCube(tool->inputs[0],tool->inputs[1],&(app->scene),app->current_color,app->current_color_index,scene__remove_voxel);
    } else {
        printf("plate tool : adding [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeStructureCube(tool->inputs[0],tool->inputs[1],&(app->scene),app->current_color,app->current_color_index,scene__add_voxel);
    }
    rasterizeStructureCube(tool->inputs[0],point,&(app->scene),app->current_color,app->current_color_index,IsKeyPressed(KEY_LEFT_ALT)?scene__remove_voxel:scene__add_voxel);
}
void plate_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    /// printf("aquire: plate tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(tool->last_input_index==0){
        rasterizePlane(point,point,point,&(app->construction_hints),app->current_color,app->current_color_index,scene__add_voxel);
    } else if(tool->last_input_index==1){
        rasterizePlane(tool->inputs[0],point,point,&(app->construction_hints),app->current_color,app->current_color_index,scene__add_voxel);
    } else if(tool->last_input_index==2){
        rasterizePlane(tool->inputs[0],tool->inputs[1],point,&(app->construction_hints),app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void plate_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    printf("finish plate tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(IsKeyDown(KEY_LEFT_ALT)){
        printf("plate tool : removing [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizePlane(tool->inputs[0],tool->inputs[1],tool->inputs[2],&(app->scene),app->current_color,app->current_color_index,scene__remove_voxel);
    } else {
        printf("plate tool : adding [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizePlane(tool->inputs[0],tool->inputs[1],tool->inputs[2],&(app->scene),app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void volume_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    /// printf("aquire: volume tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(tool->last_input_index==0){
        scene__add_voxel(&(app->construction_hints),point,app->current_color,app->current_color_index);
    } else if(tool->last_input_index==1){
        rasterizeSolidCube(tool->inputs[0],point,&(app->construction_hints),app->current_color,app->current_color_index,scene__add_voxel);
    }
}
void volume_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    printf("finish volume tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(IsKeyDown(KEY_LEFT_ALT)){
        printf("volume tool : removing [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeSolidCube(tool->inputs[0],tool->inputs[1],&(app->scene),app->current_color,app->current_color_index,scene__remove_voxel);
    } else {
        printf("volume tool : adding [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
        rasterizeSolidCube(tool->inputs[0],tool->inputs[1],&(app->scene),app->current_color,app->current_color_index,scene__add_voxel);
    }
    
}

RenderTexture2D LoadShadowmapRenderTexture(int width, int height);
void UnloadShadowmapRenderTexture(RenderTexture2D target);

int main(int argc, char *argv[]) {
    int color_btn_size=25;
    int color_btn_spacing=1;
    int left_menu_sz_width = 20 + color_btn_size*5;
    // Initialization
    //--------------------------------------------------------------------------------------

    vxdi_tools_map_t *tools=(vxdi_tools_map_t*)malloc(sizeof(vxdi_tools_map_t));
    vxdi_tools_map__init(tools);

    vxdi_multistep_tool_t select_objects_tool;
    multistep_tool__init(&select_objects_tool,1,select_tool_acquire,select_tool_finish);
    vxdi_tools_map__add(tools,"select_objects",&select_objects_tool);

    vxdi_multistep_tool_t voxel_tool;
    multistep_tool__init(&voxel_tool,1,voxel_tool_acquire,voxel_tool_finish);
    vxdi_tools_map__add(tools,"voxel",&voxel_tool);

    vxdi_multistep_tool_t shell_tool;
    multistep_tool__init(&shell_tool,2,shell_tool_acquire,shell_tool_finish);
    vxdi_tools_map__add(tools,"shell",&shell_tool);

    vxdi_multistep_tool_t structure_tool;
    multistep_tool__init(&structure_tool,2,structure_tool_acquire,structure_tool_finish);
    vxdi_tools_map__add(tools,"structure",&structure_tool);

    vxdi_multistep_tool_t line_tool;
    multistep_tool__init(&line_tool,2,line_tool_acquire,line_tool_finish);
    vxdi_tools_map__add(tools,"line",&line_tool);

    vxdi_multistep_tool_t volume_tool;
    multistep_tool__init(&volume_tool,2,volume_tool_acquire,volume_tool_finish);
    vxdi_tools_map__add(tools,"volume",&volume_tool);

    printf(" -- plate_tool\n");
    vxdi_multistep_tool_t plate_tool;
    multistep_tool__init(&plate_tool,3,plate_tool_acquire,plate_tool_finish);
    vxdi_tools_map__add(tools,"plane",&plate_tool);

    vxdi_multistep_tool_t help_tool;
    multistep_tool__init(&help_tool,1,help_tool_acquire,help_tool_finish);
    vxdi_tools_map__add(tools,"help",&help_tool);

    // Set window to be resizable


    // DisableCursor();                    // Limit cursor to relative movement inside the window

    printf(" -- SetExitKey\n");
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    SetExitKey(KEY_Q); // Use KEY_NULL or 0 to disable
    printf(" -- vxdi_app_editor__setup\n");
    vxdi_app_editor_t app;
    vxdi_app_editor__setup(&app,(Vector3){ 0.7f, -1.0f, 0.3f });
    if(argc >1) {
        app.scene.temp_filename = argv[1];
    }

    printf(" -- scene__load_model\n");
    scene__load_model(&app.scene,app.scene.temp_filename);

    // SetCameraMode(app.camera, CAMERA_FREE); // Let Raylib handle app.camera controls
    

    printf(" -- setup orbiter \n");
    orbit_t orbiter=orbit_init(&app.camera);


    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    printf(" -- Init Window \n");
    char app_title[255];
    snprintf(app_title,255,"V0XD31-%s",app.scene.temp_filename);

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    // Shadows are a HUGE topic, and this example shows an extremely simple implementation of the shadowmapping algorithm,
    // which is the industry standard for shadows. This algorithm can be extended in a ridiculous number of ways to improve
    // realism and also adapt it for different scenes. This is pretty much the simplest possible implementation.
    InitWindow(app.screenWidth, app.screenHeight, app_title);

    shadow_mapper_t shadow_mapper;

    InitShadowMapping(&shadow_mapper);
    
    SetLight(&shadow_mapper,app.light,app.scene.bounds);


    mut char status[1024];
    // mut int ctrl,left_ctrl;
    mut char show_help=0;
    char window_should_close=0;
    app.current_mouse_position=GetMousePosition();
    app.previous_mouse_position=app.current_mouse_position;
    app.is_mouse_position_changed = 1;
    unsigned char dbg_move_number=0;

    printf(" -- Starting Loop %d \n",window_should_close);
    for (;!(WindowShouldClose() || window_should_close);) {
        float dt = GetFrameTime();
        if(IsKeyPressed(KEY_ESCAPE)){
            // do nothing
        }

        
        app.current_mouse_position=GetMousePosition();
        if(Vector2Length(Vector2Subtract(app.current_mouse_position,app.previous_mouse_position)) > 5) {
            app.previous_mouse_position=app.current_mouse_position;
            app.is_mouse_position_changed=1;
            dbg_move_number=(dbg_move_number+app.is_mouse_position_changed)%256;
        } else {
            app.is_mouse_position_changed=0;
        }

        if(1 || app.is_mouse_position_changed){
            // UpdateCamera(&app.camera, app.current_camera_mode );// Update app.camera position/movement
            vxdi_app_editor__update(&app);


            app.mouse_model=scene__get_intersections(app.camera,&app.scene);
            if(app.mouse_model.collision_hit == COLLISION_HIT_PLANE || app.mouse_model.collision_hit == COLLISION_HIT_NONE) {
                app.mouse_model=scene__get_intersections(app.camera,&app.guides);

                if(app.mouse_model.collision_hit == COLLISION_HIT_VOXEL) {
                    app.mouse_model.collision_hit=COLLISION_HIT_GUIDE;
                }
            }

            app.model_point_int=Vector3Floor(app.mouse_model.point);
            app.model_point_next_int=Vector3Add(app.model_point_int,(Vector3){0,1,0});
            switch(app.mouse_model.collision_hit){
                case COLLISION_HIT_NONE:
                    app.model_point_next_int=Vector3Round(app.mouse_model.point);//Vector3Add(Vector3Round(app.mouse_model.point),(Vector3){1,0,1});
                    app.model_point_int=app.model_point_next_int;
                    break;
                case COLLISION_HIT_VOXEL:
                    app.model_point_int=app.mouse_model.voxel.position;
                    app.model_point_next_int=Vector3Add(app.mouse_model.voxel.position,app.mouse_model.normal);
                    break;
                case COLLISION_HIT_PLANE:
                    app.model_point_next_int=Vector3Round(app.mouse_model.point);//Vector3Add(Vector3Round(app.mouse_model.point),(Vector3){1,0,1});
                    app.model_point_int=app.model_point_next_int;
                    break;
                case COLLISION_HIT_GUIDE:
                    app.model_point_next_int=Vector3Round(app.mouse_model.point);//Vector3Add(Vector3Round(app.mouse_model.point),(Vector3){1,0,1});
                    app.model_point_int=app.model_point_next_int;
                    break;
            }
            int keyboard_wait_time;
            if( abs(keyboard_wait_time=vxdi_app_editor__InputMathExpr(&app))  ){
                WaitTime(((float)keyboard_wait_time)/1000.0f);
                continue;
            }


            if(IsKeyPressed(KEY_G)){
                scene__add_voxel(&app.guides,app.model_point_int,RED,0);
                for(int i=1;i<20;i++){
                    scene__add_voxel(&app.guides,(Vector3){app.model_point_int.x+(float)i,app.model_point_int.y,app.model_point_int.z},RED,0);
                    scene__add_voxel(&app.guides,(Vector3){app.model_point_int.x-(float)i,app.model_point_int.y,app.model_point_int.z},Fade(RED,0.5f),0);
                    scene__add_voxel(&app.guides,(Vector3){app.model_point_int.x,app.model_point_int.y+(float)i,app.model_point_int.z},GREEN,0);
                    scene__add_voxel(&app.guides,(Vector3){app.model_point_int.x,app.model_point_int.y-(float)i,app.model_point_int.z},Fade(GREEN,0.5f),0);
                    scene__add_voxel(&app.guides,(Vector3){app.model_point_int.x,app.model_point_int.y,app.model_point_int.z+(float)i},BLUE,0);
                    scene__add_voxel(&app.guides,(Vector3){app.model_point_int.x,app.model_point_int.y,app.model_point_int.z-(float)i},Fade(BLUE,0.5f),0);
                }
            }


            if (IsKeyPressed('Z')) orbiter=orbit_init(&app.camera);

            orbit__control_camera(&orbiter);

            snprintf(status,1024,"voxels: %d/10000 mode : %s[%d/%d] %lu, text %s file %s",
                app.scene.numVoxels,
                tools->tools_names[tools->current_tool_index],
                tools->current_tool_index,
                tools->last_tool_index,
                tools->tools[tools->current_tool_index]->last_input_index,
                app.text_buffer,
                app.scene.temp_filename
            );
            
            
            const float cameraSpeed = 0.025f;
            if (IsKeyDown(KEY_LEFT))
            {
                if (app.light.position.x < 0.6f)
                    app.light.position.x += cameraSpeed * 60.0f * dt;
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                if (app.light.position.x > -0.6f)
                    app.light.position.x -= cameraSpeed * 60.0f * dt;
            }
            if (IsKeyDown(KEY_UP))
            {
                if (app.light.position.z < 0.6f)
                    app.light.position.z += cameraSpeed * 60.0f * dt;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                if (app.light.position.z > -0.6f)
                    app.light.position.z -= cameraSpeed * 60.0f * dt;
            }
            SetLight(&shadow_mapper,app.light,app.scene.bounds);
            
            BeginDrawing();
                // Record the light matrices for future use!
                
                // 1. First pass, from light's perspective
                ClearBackground(BLACK);
                BeginShadowPass(&shadow_mapper);
                    RenderModelWithShadows(&shadow_mapper,&app.camera, &shadow_mapper.depthShader); // Use depth shader
                    scene__render(&app.scene,0);
                    scene__render(&app.guides,1);
                    scene__render(&app.construction_hints,2);
                EndShadowPass(&shadow_mapper);

                BeginMode3D(app.camera);
                
                ClearBackground(GRAY);

                BeginScenePass(&shadow_mapper);
                    RenderModelWithShadows(&shadow_mapper, &app.camera, &shadow_mapper.shadowShader); // Use shadow shader
                    scene__render(&app.scene,0);
                EndScenePass(&shadow_mapper);
                    scene__render(&app.scene,0);
                    scene__render(&app.guides,1);
                    scene__render(&app.construction_hints,2);
                    // DrawCube(app.light.position,2,2,2,YELLOW);
                    // DrawLine3D(app.light.position,app.light.target,GREEN);
                    DrawCube(Vector3Scale(Vector3Normalize(Vector3Subtract(app.light.position,app.light.target)),15.0f),0.5,0.5,0.5,YELLOW);

                    DrawCubeWires(app.model_point_int, 1.0f, 1.0f, 1.0f, Fade(RED, 0.5f));
                    DrawCubeWires(app.model_point_next_int, 1.0f, 1.0f, 1.0f, Fade(GREEN, 0.5f));
                    
                    DrawGridAt((Vector3){-0.5f,-0.5f,-0.5f},33, 1.0f); // Draw a grid
                    DrawBoundingBox(
                        app.scene.bounds,
                        RED
                    );

                    if(app.current_mouse_position.x>left_menu_sz_width && app.current_mouse_position.x<(app.screenWidth-70)){
                        DrawLine3D(
                            app.mouse_model.point,
                            Vector3Add(
                                app.mouse_model.point,
                                (Vector3){
                                    app.mouse_model.normal.x*1,
                                    app.mouse_model.normal.y*1,
                                    app.mouse_model.normal.z*1,
                                }
                            ),
                            (Color){
                                    app.mouse_model.normal.x*255,
                                    app.mouse_model.normal.y*255,
                                    app.mouse_model.normal.z*255,
                                    255,
                            });
                        DrawSphereEx(app.mouse_model.point,.1f,3,5,BLUE);
                    }
                EndMode3D();

                DrawTextureRec(shadow_mapper.shadowMap.depth, (Rectangle){ 0, 0, shadow_mapper.shadowMap.depth.width, -shadow_mapper.shadowMap.depth.height }, (Vector2){ 0, 0 }, WHITE);
                
                for(int i=0;i<=tools->last_tool_index;i++) {
                    char itext[20]; // Make sure the array is large enough to hold the converted string
                    sprintf(itext, "%d", i);
                    Vector2 a={app.screenWidth-70,32+64*i};
                    Rectangle r={a.x,a.y,62,62};
                    DrawRectangleRec(r,(Color){200,200,200,255});
                    DrawText(itext,r.x+50,r.y+16,14,BLACK);
                    DrawText(tools->tools_names[i],r.x+6,r.y+46,14,BLACK);
                    if(i==tools->current_tool_index){
                        DrawRectangle(a.x,a.y,6,62,(Color){96,160,160,255});
                    }
                    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && RectangleContains(r,app.current_mouse_position)) {
                        vxdi_tools_map__select(tools,i);
                    }
                }



                if(IsKeyPressed(KEY_SPACE)){
                    scene__clear(&app.guides);
                    scene__clear(&app.construction_hints);
                    tools->current_tool_index=0;
                    for(int i=0;i<=tools->last_tool_index;i++){
                        multistep_tool__reset(tools->tools[i]);
                    }
                }
                /// DrawFPS(10, 10);


                // draw and check tools buttons
                if(IsKeyReleased(KEY_T) ){
                    vxdi_tools_map__next( tools );
                    printf("\rsetting current tool : %d/%d     //////  ",tools->current_tool_index,tools->last_tool_index);
                }
                
                if (
                    app.current_mouse_position.x>left_menu_sz_width && 
                    app.current_mouse_position.x<(app.screenWidth-70)
                ){
                    if (
                        (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) &&
                        !IsKeyDown(KEY_LEFT_CONTROL) &&
                        !IsKeyDown(KEY_LEFT_SHIFT) 
                    ) {
                        multistep_tool__receive_point(vxdi_app_editor__get_current(tools),&app,&app.scene,app.model_point_next_int);
                    } else if(app.is_mouse_position_changed ){
                        multistep_tool__receive_moving_point(vxdi_app_editor__get_current(tools),&app,app.model_point_next_int);
                    }
                }
                // draw color pallete
                for(int ci=0;ci<24;ci+=1){
                    for(int lum=-1;lum<2;lum+=1){
                        Vector2 pos = {20+color_btn_size+color_btn_spacing+lum*(color_btn_size+color_btn_spacing), (color_btn_size+color_btn_spacing)+ci*color_btn_size};
                        Color cl=ColorBrightness(app.colors[ci*15],((float)lum*5.0f)/10.0f);
                        if (ext_SquareButton(pos,(color_btn_size-color_btn_spacing)/2,cl) && app.current_mouse_position.x<left_menu_sz_width) {
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
                    if (ext_SquareButton(pos,(color_btn_size-color_btn_spacing)/2,cl) && app.current_mouse_position.x<left_menu_sz_width) {
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
                        if (ext_SquareButton(pos,(color_btn_size-color_btn_spacing)/2,cl) && app.current_mouse_position.x<left_menu_sz_width) {
                            app.current_color = cl;
                            app.current_color_index = ci*0x100;
                            // goto start;
                        }
                        if(app.current_color_index == ci*0x100){
                            DrawRectangleLines(pos.x-color_btn_size/2,pos.y-color_btn_size/2,color_btn_size,color_btn_size,WHITE);
                        }
                }

                DrawRectangle( 0, 0, app.screenWidth, 20, Fade(DARKGRAY, 0.95f));
                DrawText(status     , 20, 3, 14, (Color){20,20,20,255});
                char coords[12];
                snprintf(coords,12,"%3d %3d %3d",(int)app.model_point_int.x,(int)app.model_point_int.y,(int)app.model_point_int.z);
                DrawText(coords, app.current_mouse_position.x+10, app.current_mouse_position.y-10, 20, BLACK);


                // Draw the shadow map texture
                /// DrawTextureRec(shadowMapTexture.depth, (Rectangle){ 0, 0, shadowMapTexture.depth.width, shadowMapTexture.depth.height }, (Vector2){ 100, 1000 }, WHITE);
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
            Vector2 m = app.current_mouse_position;
            Rectangle b = btn_rectangle;
            if (m.x>b.x && m.x<b.x+b.width && m.y>b.y && m.y<b.y+b.height) {
                show_help=0;
            }
            DrawText("Free app.camera default controls:", left, row+=10, 10, BLACK);
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
    vxdi_tools_map__deinit(tools);

    
    // UnloadShader(localShadowShader);
    // UnloadShadowmapRenderTexture(shadowMapTexture);
    CloseWindow();
    /// scene__save_model(&app.scene,"temp.vxde");

    return 0;
}



RenderTexture2D LoadShadowmapRenderTexture(int width, int height)
{
    RenderTexture2D target = { 0 };

    target.id = rlLoadFramebuffer(width,height); // Load an empty framebuffer
    target.texture.width = width;
    target.texture.height = height;

    if (target.id > 0)
    {
        rlEnableFramebuffer(target.id);

        // Create depth texture
        // We don't need a color texture for the shadowmap
        target.depth.id = rlLoadTextureDepth(width, height, false);
        target.depth.width = width;
        target.depth.height = height;
        target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
        target.depth.mipmaps = 1;

        // Attach depth texture to FBO
        rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

        // Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

        rlDisableFramebuffer();
    }
    else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

    return target;
}

// Unload shadowmap render texture from GPU memory (VRAM)
void UnloadShadowmapRenderTexture(RenderTexture2D target)
{
    if (target.id > 0)
    {
        // NOTE: Depth texture/renderbuffer is automatically
        // queried and deleted before deleting framebuffer
        rlUnloadFramebuffer(target.id);
    }
}