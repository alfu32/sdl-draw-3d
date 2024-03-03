#ifndef __RLMATH_EXTRAS__
#define __RLMATH_EXTRAS__
#import <raylib.h>

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

#endif