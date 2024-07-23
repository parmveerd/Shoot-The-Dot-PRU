#include "../include/joystick.h"
#include "../include/acc.h"
#include "../include/shared_linux.h"
#include "../include/buzzer.h"
#include "../include/display.h"
#include "../../hal/include/config_display.h"


static int *continueFlag;
static long score = 0;
static pthread_t joyThread;

void joy_init(int* flag) {
    continueFlag = flag;
    pthread_create(&joyThread, NULL, joy_loop, NULL);
}

void* joy_loop() {
    int down_pressed = 0;
    int check = 0;

    while(*continueFlag) {
        if(joy_down()) {
            if(get_target()) {
                if(down_pressed == 0) {
                    dot_hit(1);
                    score++;
                    set_display(score);
                    new_dot();
                    down_pressed = 1;
                    check = 1;
                }
            } else {
                dot_hit(2);
            }
        } else if(joy_right()) {
            *continueFlag = 0;
            stop_flag();
        } else {
            down_pressed = 0;
        }

        if (check == 1) {
            check = 0;
            sleepForMs(200);
        } else {
            sleepForMs(20);
        }
    }

    return NULL;
}


void joy_join() {
    pthread_join(joyThread, NULL);
}

void joy_cleanup() {
    joy_join();
}