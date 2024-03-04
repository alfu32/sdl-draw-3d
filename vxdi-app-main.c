#include <stdio.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"

#include "external/raylib-nuklear.h"

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
}
void shell_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("shell tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
}
void line_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("line tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(tool->num_inputs==1){
        scene__add_voxel(&(app->construction_hints),tool->inputs[0],app->current_color,app->current_color_index);
    } else if(tool->num_inputs==2){
        rasterizeLine(tool->inputs[0],point,scene,app->current_color,app->current_color_index);
    }
}
void line_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("line tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    rasterizeLine(tool->inputs[0],point,scene,app->current_color,app->current_color_index);
}
void plate_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("plate tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
}
void plate_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("plate tool : got last point [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
}
void volume_tool_acquire(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    printf("volume tool : got point number [%lu/%lu]\n",tool->last_input_index,tool->num_inputs);
    if(tool->num_inputs==1){
        scene__add_voxel(&(app->construction_hints),tool->inputs[0],app->current_color,app->current_color_index);
    } else if(tool->num_inputs==2){
    }
}
void volume_tool_finish(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){

    float x0=min(tool->inputs[0].x,tool->inputs[1].x);
    float y0=min(tool->inputs[0].y,tool->inputs[1].y);
    float z0=min(tool->inputs[0].z,tool->inputs[1].z);
    float x1=max(tool->inputs[0].x,tool->inputs[1].x);
    float y1=max(tool->inputs[0].y,tool->inputs[1].y);
    float z1=max(tool->inputs[0].z,tool->inputs[1].z);
    printf("volume tool : got last point [%lu/%lu] %2.2f,%2.2f,%2.2f -> %2.2f,%2.2f,%2.2f = \n",tool->last_input_index,tool->num_inputs, x0,y0,z0, x1,y1,z1);
        for(int x=x0;x<=x1;x+=1){
            for(int y=y0;y<=y1;y+=1){
                for(int z=z0;z<=z1;z+=1){
                    printf("scene_add_voxel %d,%d,%d\n",x,y,z);
                    scene__add_voxel(scene,(Vector3){x,y,z},(Color){255,255,255,255},2);
                }
            }
        }
}

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    vxdi_multistep_tool_t voxel_tool;
    multistep_tool__init(&voxel_tool,1,voxel_tool_acquire,voxel_tool_finish);
    vxdi_multistep_tool_t shell_tool;
    multistep_tool__init(&shell_tool,3,shell_tool_acquire,shell_tool_finish);
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
    vxdi_app_editor_t app=vxdi_app_editor__setup(&camera);
    scene__init(&scene,1);

    scene__load_model(&scene,"temp.vxde");

    // Example: Add a voxel to the scene
    scene__add_voxel(&scene, (Vector3){0.0f, 0.0f, 0.0f}, RED,1);
    voxel_t cursor={20,30,20,3};
    voxel_t cursor2={20,30,20,2};

    // SetCameraMode(camera, CAMERA_FREE); // Let Raylib handle camera controls
    
    orbit_t orbiter=orbit_init(&camera);


    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(app.screenWidth, app.screenHeight, "Raylib Voxel Scene");

    mut char status[100];
    mut int ctrl,left_ctrl;
    mut char show_help=0;
    char window_should_close=0;
    Vector2 current_mouse_position=GetMousePosition();
    Vector2 previous_mouse_position=current_mouse_position;
    char is_mouse_position_changed = 1;
    unsigned char dbg_move_number=0;
    for (;!(WindowShouldClose() || window_should_close);) {
        start:
        
        current_mouse_position=GetMousePosition();
        if(Vector2Length(Vector2Subtract(current_mouse_position,previous_mouse_position)) > 5) {
            previous_mouse_position=current_mouse_position;
            is_mouse_position_changed=1;
            dbg_move_number=(dbg_move_number+is_mouse_position_changed)%256;
        } else {
            is_mouse_position_changed=0;
        }

        if(1 || is_mouse_position_changed){
            UpdateCamera(&camera, app.current_camera_projection );// Update camera position/movement
            vxdi_app_editor__update(&app);

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

            snprintf(status,100,"num voxels: %d construction mode : %d, text_buffer %40s  voxels %d/%d temp file %s",scene.numVoxels,app.construction_mode,app.text_buffer,scene.numVoxels,MAX_VOXELS,scene.temp_filename);

            BeginDrawing();
            // ClearBackground(RAYWHITE);
            // DrawCircle(app.screenWidth/2,app.screenHeight/2,app.screenHeight/2.0f-10.0f,RED);
            ClearBackground(GRAY);

            BeginMode3D(camera);

                // Render the scene
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
                if(current_mouse_position.x>50 && current_mouse_position.x<(app.screenWidth-60)){
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
                multistep_tool__reset(&plate_tool);
                multistep_tool__reset(&shell_tool);
                multistep_tool__reset(&volume_tool);
                multistep_tool__reset(&voxel_tool);
            }
            /// DrawFPS(10, 10);
            switch(app.construction_mode){
                case APP_CONSTRUCTION_MODE_LINE:
                    if(IsKeyDown(KEY_SPACE)){}
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        printf("LINE tool click %d\n",APP_CONSTRUCTION_MODE_LINE);
                        multistep_tool__receive_point(&line_tool,&app,&scene,model_point_int);
                    } else if(is_mouse_position_changed){
                        printf("LINE tool move[%d] %d\n",dbg_move_number,APP_CONSTRUCTION_MODE_LINE);
                        multistep_tool__receive_moving_point(&line_tool,&app,model_point_int);
                    }
                break;
                case APP_CONSTRUCTION_MODE_PLATE:
                    if(IsKeyDown(KEY_SPACE)){}
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        printf("PLATE tool click %d\n",APP_CONSTRUCTION_MODE_PLATE);
                        multistep_tool__receive_point(&plate_tool,&app,&scene,model_point_int);
                    } else if(is_mouse_position_changed){
                        printf("PLATE tool move[%d] %d\n",dbg_move_number,APP_CONSTRUCTION_MODE_PLATE);
                        multistep_tool__receive_moving_point(&plate_tool,&app,model_point_int);
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
                case APP_CONSTRUCTION_MODE_VOXEL:
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        printf("VOLUME tool click %d\n",APP_CONSTRUCTION_MODE_VOLUME);
                        multistep_tool__receive_point(&voxel_tool,&app,&scene,model_point_int);
                    } else if(is_mouse_position_changed){
                        printf("VOLUME tool move[%d] %d\n",dbg_move_number,APP_CONSTRUCTION_MODE_VOLUME);
                        multistep_tool__receive_moving_point(&voxel_tool,&app,model_point_int);
                    }
                    if (
                        current_mouse_position.x>130 && 
                        current_mouse_position.x<(app.screenWidth-100) && 
                        IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
                        !IsKeyDown(KEY_LEFT_CONTROL) &&
                        !IsKeyDown(KEY_LEFT_SHIFT)
                    ){
                        scene__remove_voxel(&scene,model_point_int);
                    }
                    if (
                        current_mouse_position.x>130 && 
                        current_mouse_position.x<(app.screenWidth-100) && 
                        IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                        !IsKeyDown(KEY_LEFT_CONTROL) &&
                        !IsKeyDown(KEY_LEFT_SHIFT)
                    ){
                        scene__add_voxel(&scene,model_point_next_int,app.current_color,app.current_color_index);
                    }
                break;
            }
            


            for(int ci=0;ci<24;ci+=1){
                for(int lum=-1;lum<2;lum+=1){
                    Vector2 pos = {60+lum*40, 40+ci*35};
                    Color cl=ColorBrightness(app.colors[ci*15],((float)lum*5.0f)/10.0f);
                    if(app.current_color_index == ci*10 + lum+1){
                        DrawCircle(pos.x,pos.y,20,WHITE);
                    }
                    if (ext_RoundButton(pos,15,cl) && current_mouse_position.x<130) {
                        app.current_color = cl;
                        app.current_color_index = ci*10 + lum+1;
                        // goto start;
                    }
                }
            }

            int crt=25;
            if (GuiButton((Rectangle){ app.screenWidth-60, crt, 50, 30 }, "Close")) {
                window_should_close=1;
            }
            if (GuiButton((Rectangle){ app.screenWidth-60, crt+=50, 50, 30 }, "Help")) {
                show_help=!show_help;
            }
            if(app.construction_mode==APP_CONSTRUCTION_MODE_VOLUME) {
                DrawRectangleLinesEx((Rectangle){app.screenWidth-60-2, crt+90-2, 50+4, 30+4},2,PINK);
            }
            if (GuiButton((Rectangle){ app.screenWidth-60, crt=crt+90, 50, 30 }, "Volume")) {
                app.construction_mode = APP_CONSTRUCTION_MODE_VOLUME;
            }

            if(app.construction_mode==APP_CONSTRUCTION_MODE_SHELL) {
                DrawRectangleLinesEx((Rectangle){app.screenWidth-60-2, crt+50-2, 50+4, 30+4},2,PINK);
            }
            if (GuiButton((Rectangle){ app.screenWidth-60,crt=crt+50, 50, 30 }, "SHELL")) {
                app.construction_mode = APP_CONSTRUCTION_MODE_SHELL;
            }

            if(app.construction_mode==APP_CONSTRUCTION_MODE_PLATE) {
                DrawRectangleLinesEx((Rectangle){app.screenWidth-60-2, crt+50-2, 50+4, 30+4},2,PINK);
            }
            if (GuiButton((Rectangle){ app.screenWidth-60,crt=crt+50, 50, 30 }, "Plate")) {
                app.construction_mode = APP_CONSTRUCTION_MODE_PLATE;
            }

            if(app.construction_mode==APP_CONSTRUCTION_MODE_LINE) {
                DrawRectangleLinesEx((Rectangle){app.screenWidth-60-2, crt+50-2, 50+4, 30+4},2,PINK);
            }
            if (GuiButton((Rectangle){ app.screenWidth-60,crt=crt+50, 50, 30 }, "LINE")) {
                app.construction_mode = APP_CONSTRUCTION_MODE_LINE;
            }

            if(app.construction_mode==APP_CONSTRUCTION_MODE_VOXEL) {
                DrawRectangleLinesEx((Rectangle){app.screenWidth-60-2, crt+50-2, 50+4, 30+4},2,PINK);
            }
            if (GuiButton((Rectangle){ app.screenWidth-60,crt=crt+50, 50, 30 }, "Voxel")) {
                app.construction_mode = APP_CONSTRUCTION_MODE_VOXEL;
            }

            if(show_help){
                    int row=60;
                    int left=app.screenWidth-380;
                    DrawRectangle( left-10, row, 320, 160, Fade(SKYBLUE, 0.5f));
                    DrawRectangleLines( left-10, row, 320, 160, BLUE);
                    if (GuiButton((Rectangle){ left+290,row, 20, 20 }, "X")) {
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
            DrawRectangle( 0, 0, app.screenWidth, 20, Fade(DARKGRAY, 0.95f));
            DrawText(status     , app.screenWidth-300, 5, 10, (Color){200,200,200,255});

            EndDrawing();

        }/** endif mouse cursor changed */
    }
    multistep_tool__deinit(&voxel_tool);
    CloseWindow();
    /// scene__save_model(&scene,"temp.vxde");

    return 0;
}