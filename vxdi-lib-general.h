
#ifndef __VXDI_LIB_GENERAL_H__
#define __VXDI_LIB_GENERAL_H__

    typedef char BOOL;
    #define true 1
    #define false 0

#define mut
#define borrowed
#define owned
#define stack
#define heap
#define static
#define constructor(target) target## _t target##_init
#define method(target)
#define interface
#define instance

#define QUALIFIED(type,qlen) struct q_##type##_s {\
    type value;\
    char qualifier[qlen];\
}\



#define TUPLE(id,type,...) struct tpl_##id {\
    type \
    __VA_ARGS__\
}

    typedef int error_id;


    // Utility function to swap two floats
    void swap(float* a, float* b) {
        float temp = *a;
        *a = *b;
        *b = temp;
    }

#endif