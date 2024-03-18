#ifndef __VXDI_RL_MATH_EXTRAS_H__
#define __VXDI_RL_MATH_EXTRAS_H__
#include <raylib.h>

Vector3 Vector3Floor(Vector3 v){
    return (Vector3){
        floor(v.x),
        floor(v.y),
        floor(v.z),
    };
}
Vector3 Vector3Round(Vector3 v){
    return (Vector3){
        round(v.x),
        round(v.y),
        round(v.z),
    };
}

int min(int a,int b){
    return a<b?a:b;
}
int max(int a,int b){
    return a<b?b:a;
}
int abs(int a){
    if(a<0){
        return -a;
    }else{
        return a;
    }
}

void fillColorCircle(Color colors[360]) {
    for (int i = 0; i < 360; i++) {
        colors[i]=ColorFromHSV(i,1.0f,1.0f);
    }
}

int RectangleContains(Rectangle r,Vector2 v) {
    return r.x<=v.x && v.x <= (r.x + r.width)
        && r.y<=v.y && v.y <= (r.y + r.height);
}

#endif