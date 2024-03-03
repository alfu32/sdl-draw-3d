#ifndef __UI_MULTISTEP_TOOL_
#define __UI_MULTISTEP_TOOL_
#include <raylib.h>
#include "scene.h"




typedef struct multistep_tool_s {
    mutate_scene_fn on_point_aquired;
    mutate_scene_fn on_finish;
    size_t last_input_index;
    size_t num_inputs;
    Vector3* inputs;
} multistep_tool_t;

typedef struct three_step_tool_s {
    mutate_scene_fn on_point_aquired;
    mutate_scene_fn on_finish;
    Vector3 inputs[3];
} three_step_tool_t;

typedef struct two_step_tool_s {
    mutate_scene_fn on_point_aquired;
    mutate_scene_fn on_finish;
    Vector3 inputs[2];
} two_step_tool_t;

int multistep_tool__init(multistep_tool_t* tool,int num_inputs,mutate_scene_fn on_point_aquired_fn,mutate_scene_fn on_finish_fn){
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

int multistep_tool__receive_point(multistep_tool_t* tool,Vector3 point,scene_t* scene){
    tool->inputs[tool->last_input_index++] = point;
    if((tool->last_input_index+1) == tool->num_inputs) {
        //finalize and reset
        tool->on_finish(scene,tool->inputs,tool->last_input_index,tool->num_inputs);
        tool->last_input_index=0;
    }else {
        tool->on_point_aquired(scene,tool->inputs,tool->last_input_index,tool->num_inputs);
    }
    return 0;

}
int multistep_tool__deinit(multistep_tool_t* tool){
    free(tool->inputs);
    return 0;
}

#endif