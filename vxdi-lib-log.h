#ifndef __VXDI_LIB_LOG_H__  // Include guard to prevent double inclusion
#define __VXDI_LIB_LOG_H__

// Include standard headers or other necessary headers here
#include <stdio.h>
#include <time.h>

// Declare any constants, data types, and function prototypes here

// Example function declaration
    #ifndef LOG_ALL
        #ifndef LOG_DEBUG
            #define LOG_DEBUG 0
        #else
            #define LOG_DEBUG 1
        #endif
        #ifndef LOG_TRACE
            #define LOG_TRACE 0
        #else
            #define LOG_TRACE 1
        #endif
        #ifndef LOG_WARNING
            #define LOG_WARNING 0
        #else
            #define LOG_WARNING 1
        #endif
        #ifndef LOG_ERROR
            #define LOG_ERROR 0
        #else
            #define LOG_ERROR 1
        #endif
    
    #else
        #define LOG_DEBUG 1
        #define LOG_TRACE 1
        #define LOG_WARNING 1
        #define LOG_ERROR 1
    #endif


    #ifdef _WIN64
        // printf("Running on 64-bit Windows\n");
        #define VXDI_LOG_FORMAT_0 "%-*s [%lld]%s(%s):%d :"
        #define VXDI_LOG_FORMAT_1 "%-*s [%lld]%s(%s):%d : %s\n"
    #elif _WIN32
        // printf("Running on 32-bit Windows\n");
        #define VXDI_LOG_FORMAT_0 "%-*s [%ld]%s(%s):%d :"
        #define VXDI_LOG_FORMAT_1 "%-*s [%ld]%s(%s):%d : %s\n"
    #elif __linux__
        // printf("Running on Linux\n");
        #define VXDI_LOG_FORMAT_0 "%-*s [%ld]%s(%s):%d :"
        #define VXDI_LOG_FORMAT_1 "%-*s [%ld]%s(%s):%d : %s\n"
    #else
        // printf("Unknown platform\n");
        #define VXDI_LOG_FORMAT_0 "%-*s [%ld]%s(%s):%d :"
        #define VXDI_LOG_FORMAT_1 "%-*s [%ld]%s(%s):%d : %s\n"
    #endif



    #define LOG_LEVEL_ALL(LEVEL,FMT,...) printf(VXDI_LOG_FORMAT_0 FMT "\n",10,LEVEL,time(NULL),__func__,__FILE__,__LINE__,  __VA_ARGS__);
    #define LOG_LEVEL(LEVEL,message) printf(VXDI_LOG_FORMAT_1,10,LEVEL,time(NULL),__func__,__FILE__,__LINE__,message);

    #define LOG_D(fmt,...) if(LOG_DEBUG)LOG_LEVEL_ALL("\033[36mDEBUG\033[0m",fmt,  __VA_ARGS__);
    #define LOG_D0(message) if(LOG_DEBUG)LOG_LEVEL("\033[36mDEBUG\033[0m",message);

    #define LOG_T(fmt,...) if(LOG_TRACE)LOG_LEVEL_ALL("\033[34mTRACE\033[0m",fmt,  __VA_ARGS__);
    #define LOG_T0(message) if(LOG_TRACE)LOG_LEVEL("\033[34mTRACE\033[0m",message);

    #define LOG_W(fmt,...) if(LOG_WARNING)LOG_LEVEL_ALL("\033[33mWARNING\033[0m",fmt,  __VA_ARGS__);
    #define LOG_W0(message) if(LOG_WARNING)LOG_LEVEL("\033[33mWARNING\033[0m",message);

    #define LOG_E(fmt,...) if(LOG_ERROR)LOG_LEVEL_ALL("\033[31mERROR\033[0m",fmt,  __VA_ARGS__);
    #define LOG_E0(message) if(LOG_ERROR)LOG_LEVEL("\033[31mERROR\033[0m",message);

#endif // LIBLOG_H