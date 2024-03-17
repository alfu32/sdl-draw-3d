#ifndef __VXDI_APP_MULTISTEP_TOOL_H__
#define __VXDI_APP_MULTISTEP_TOOL_H__
#include <raylib.h>
#include "vxdi-app-scene.h"
#include "vxdi-app-editor.h"

struct vxdi_multistep_tool_s;

typedef void (*vxdi_multistep_tool__mutate_vxdi_app_editor_fn)(struct vxdi_multistep_tool_s* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point);
typedef void (*vxdi_multistep_tool__app_react_fn)(struct vxdi_multistep_tool_s* tool,vxdi_app_editor_t* app,Vector3 point);



typedef struct vxdi_multistep_tool_s {
    vxdi_multistep_tool__app_react_fn on_point_aquired;
    vxdi_multistep_tool__app_react_fn on_point_move;
    vxdi_multistep_tool__app_react_fn on_finish;
    unsigned long int last_input_index;
    unsigned long int num_inputs;
    Vector3* inputs;
} vxdi_multistep_tool_t;

int multistep_tool__init(vxdi_multistep_tool_t* tool,int num_inputs,vxdi_multistep_tool__app_react_fn on_point_aquired_fn,vxdi_multistep_tool__app_react_fn on_finish_fn){
    tool->num_inputs=num_inputs;
    tool->last_input_index=0;
    tool->inputs=(Vector3*)malloc(sizeof(Vector3)*num_inputs);
    if(tool->inputs == 0) {
        return -1;
    }
    tool->on_point_aquired=on_point_aquired_fn;
    tool->on_finish=on_finish_fn;
    return 0;
}

int multistep_tool__reset(vxdi_multistep_tool_t* tool){
    tool->last_input_index=0;
    return 0;
}

int multistep_tool__receive_point(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,scene_t* scene,Vector3 point){
    tool->inputs[tool->last_input_index++] = point;
    if((tool->last_input_index) == tool->num_inputs) {
        //finalize and reset
        tool->on_finish(tool,app,point);
        tool->last_input_index=0;
        scene__clear(&app->construction_hints);
    }
    return 0;
}
int multistep_tool__receive_moving_point(vxdi_multistep_tool_t* tool,vxdi_app_editor_t* app,Vector3 point){
    
    // tool->inputs[tool->last_input_index] = point;
    scene__clear(&(app->construction_hints));

    if((tool->last_input_index) == tool->num_inputs) {
        //finalize and reset
        tool->on_finish(tool,app,point);
    }else {
        tool->on_point_aquired(tool,app,point);
    }
    return 0;
}
int multistep_tool__deinit(vxdi_multistep_tool_t* tool){
    free(tool->inputs);
    return 0;
}

#endif