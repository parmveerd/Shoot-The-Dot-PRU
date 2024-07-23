#ifndef _ACC_H_
#define _ACC_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdatomic.h>

void acc_init(int*);
void acc_join();
void acc_cleanup();
int get_target();
void new_dot();
void* acc_loop();


#endif