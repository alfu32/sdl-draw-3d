#ifndef __VXDI_APP_LIGHT_H__
#define __VXDI_APP_LIGHT_H__

#include <raylib.h>

typedef struct vxdi_light_s {
    Vector3 direction;
    float shadow_strength;
    float light_strength;
} vxdi_light_t;


#endif