#ifndef _SHARED_LINUX_H_
#define _SHARED_LINUX_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdatomic.h>

void data_init();
void data_cleanup();
void stop_flag();
void set_colour(uint32_t);
void set_colour_miss(uint32_t, int);
bool joy_down();
bool joy_right();

#endif