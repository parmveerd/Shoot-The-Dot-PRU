
#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdatomic.h>
#include <time.h>

void joy_init(int*);
void* joy_loop();
void joy_join();
void joy_cleanup();

#endif