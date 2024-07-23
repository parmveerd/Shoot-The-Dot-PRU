#ifndef _DIGITS_CONTROL_H_
#define _DIGITS_CONTROL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdatomic.h>


void display_init(int*);
void display_join();
void display_cleanup();
void set_display(long);
void* display_loop(); 
void second_on();
void first_on();
void turn_off();

#endif