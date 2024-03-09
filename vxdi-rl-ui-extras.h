#ifndef __VXDI_RL_UI_EXTRAS_H__
#define __VXDI_RL_UI_EXTRAS_H__

#include <raylib.h>
#include <raymath.h>

int ext_RoundButton(Vector2 pos,float radius,Color color){
    Vector2 mp = GetMousePosition();
    if(Vector2Distance(pos,mp)<=radius){
        DrawCircle(pos.x,pos.y,radius,ColorBrightness(color,0.55f));
        return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    } else {
        DrawCircle(pos.x,pos.y,radius,color);
        DrawCircleLines(pos.x,pos.y,radius,color);
        return 0;
    }
}
int ext_SquareButton(Vector2 pos,float radius,Color color){
    Vector2 mp = GetMousePosition();
    if(Vector2Distance(pos,mp)<=radius){
        DrawRectangle(pos.x-radius,pos.y-radius,radius*2,radius*2,ColorBrightness(color,0.55f));
        return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    } else {
        DrawRectangle(pos.x-radius,pos.y-radius,radius*2,radius*2,color);
        DrawRectangleLines(pos.x-radius,pos.y-radius,radius*2,radius*2,color);
        return 0;
    }
}

#endif