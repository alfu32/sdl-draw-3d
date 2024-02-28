
#ifndef LIB_H
#define LIB_H

    typedef char BOOL;
    #define true 1
    #define false 0


    typedef int error_id;


    // Utility function to swap two floats
    void swap(float* a, float* b) {
        float temp = *a;
        *a = *b;
        *b = temp;
    }

#endif
