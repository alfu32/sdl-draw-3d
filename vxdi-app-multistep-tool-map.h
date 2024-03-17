#ifndef __VXDI_APP_MULTISTEP_TOOL_MAP_H__
#define __VXDI_APP_MULTISTEP_TOOL_MAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vxdi-app-multistep-tool.h"


#define INITIAL_CAPACITY 10
#define TOOL_MAP_MAX_CAPACITY 100

typedef struct vxdi_tools_map_s {
    vxdi_multistep_tool_t tools[TOOL_MAP_MAX_CAPACITY];
    char * tools_names[TOOL_MAP_MAX_CAPACITY];
    int current_tool_index;
    int last_tool_index;
} vxdi_tools_map_t;

int vxdi_tools_map__init(vxdi_tools_map_t* map) {
    if (map == NULL) {
        return -1; // Nothing to initialize
    }
    
    // Initialize members
    map->current_tool_index = -1;
    map->last_tool_index = -1;

    return 0;
}

int vxdi_tools_map__add(vxdi_tools_map_t* map, const char* tool_name, vxdi_multistep_tool_t *tool) {
        printf("// adding tool %s to the map\n",tool_name);
    if (map == NULL || tool_name == NULL || tool == NULL) {
        // Invalid arguments
        printf("// ERR-1  Invalid arguments %s\n",tool_name);
        return -1; 
    }

    printf("// Check if %s tool_name already exists\n",tool_name);
    for (int i = 0; i < map->last_tool_index; ++i) {
        if (strcmp(map->tools_names[i], tool_name) == 0) {
            // Tool with same name already exists
            printf("// ERR-2  Tool with same name already exists %s\n",tool_name);
            return -2;
        }
    }

    printf("// Resize arrays %s if necessary\n",tool_name);
    if ((map->last_tool_index + 1) >= TOOL_MAP_MAX_CAPACITY) {
            printf("// ERR-3  Memory allocation for tool failed %s\n",tool_name);
            return -3;
    }

    printf("// Add tool %s to arrays\n",tool_name);
    map->tools[map->last_tool_index+1] = *tool;
    map->tools_names[map->last_tool_index+1] = strdup(tool_name);
    if (map->tools_names[map->last_tool_index] == NULL) {
        // Memory allocation failed
        printf("// ERR-4  Memory allocation for names failed %s\n",tool_name);
        return -4;
    }
    map->last_tool_index++;

    printf("// Update last_tool_index %s\n",tool_name);
    printf("// Finished adding %s : capacity:%d,last_tool_index:%d,index:%d,last:%d\n",tool_name,TOOL_MAP_MAX_CAPACITY,map->last_tool_index,map->current_tool_index,map->last_tool_index);

    return 0; // Success
}

int vxdi_tools_map__remove(vxdi_tools_map_t* map, const char* tool_name) {
    // Update last_tool_index
    map->last_tool_index--;

    return 0; // Success
}

int vxdi_tools_map__select(vxdi_tools_map_t* map, int tool_index) {
    if (map == NULL || tool_index < 0 || tool_index >= map->last_tool_index) {
        return -1; // Invalid arguments
    }

    map->current_tool_index = tool_index;
    return 0; // Success
}

vxdi_multistep_tool_t * vxdi_app_editor__get_current(vxdi_tools_map_t* map) {
    if (map == NULL || map->last_tool_index == 0 || map->current_tool_index < 0 || map->current_tool_index >= map->last_tool_index) {
        return NULL; // Invalid arguments or no current tool selected
    }

    return &map->tools[map->current_tool_index];
}

int vxdi_tools_map_test() {
    // Example usage
    vxdi_tools_map_t map;
    map.last_tool_index = 0;
    map.current_tool_index = -1;
    map.last_tool_index = -1;

    // Add tools
    vxdi_multistep_tool_t tool1, tool2;
    // Initialize tool1 and tool2
    vxdi_tools_map__add(&map, "Tool1", &tool1);
    vxdi_tools_map__add(&map, "Tool2", &tool2);

    // Select tool
    vxdi_tools_map__select(&map, 0);

    // Get current tool
    vxdi_multistep_tool_t *current_tool = vxdi_app_editor__get_current(&map);
    if (current_tool != NULL) {
        // Process current tool
    }

    // Remove tool
    vxdi_tools_map__remove(&map, "Tool1");

    return 0;
}


#endif