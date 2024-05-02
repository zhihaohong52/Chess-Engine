/**
 * @file misc.c
 * @author zhihaohong52
 * @brief This file contains miscellaneous functions
 * @version 0.1
 * @date 2024-05-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "stdio.h"
#include "defs.h"

#ifdef WIN32
#include "windows.h"
#else
#include "sys/time.h"
#endif

int GetTimeMs() {
#ifdef WIN32
    return GetTickCount();
#else
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec*1000 + t.tv_usec/1000;
#endif
}