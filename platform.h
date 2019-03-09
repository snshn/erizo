#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "platform_unix.h"

const char* platform_mmap(const char* filename);
void platform_get_time(int64_t* sec, int32_t* usec);

/*  Based on 8-color ANSI terminals */
typedef enum {
    TERM_COLOR_BLACK,
    TERM_COLOR_RED,
    TERM_COLOR_GREEN,
    TERM_COLOR_YELLOW,
    TERM_COLOR_BLUE,
    TERM_COLOR_MAGENTA,
    TERM_COLOR_CYAN,
    TERM_COLOR_WHITE,
} platform_terminal_color_t;
void platform_set_terminal_color(platform_terminal_color_t c);
void platform_clear_terminal_color();

/*  Threading API is a thin wrapper around pthreads */
int platform_mutex_init(platform_mutex_t* mutex);
int platform_mutex_lock(platform_mutex_t* mutex);
int platform_mutex_unlock(platform_mutex_t* mutex);

int platform_cond_init(platform_cond_t* cond);
int platform_cond_wait(platform_cond_t* cond,
                       platform_mutex_t* mutex);
int platform_cond_broadcast(platform_cond_t* cond);

int platform_thread_create(platform_thread_t* thread,
                           void *(*run)(void *), void* data);
int platform_thread_join(platform_thread_t* thread);

#endif
