#ifndef _BUZZER_H_
#define _BUZZER_H_

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdatomic.h>

void buzzer_init(int*);
void buzzer_join();
void buzzer_cleanup();
void* buzzer_loop();
void dot_hit(int);
void play_buzzer(int);

#endif