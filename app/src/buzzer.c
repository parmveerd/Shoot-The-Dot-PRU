#include "../include/buzzer.h"
#include "../include/joystick.h"
#include "../../hal/include/config_display.h"

#define CONFIG_P22 "sudo config-pin p9_22 pwm"
#define DUTYCYCLE "echo 0 > /dev/bone/pwm/0/a/duty_cycle"

static int *continueFlag2;
static int hit = 0;

static pthread_t threadBuzzer;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void buzzer_init (int* flag) {
    continueFlag2 = flag;
    runCommand(CONFIG_P22);

    pthread_create(&threadBuzzer, NULL, buzzer_loop, NULL);
}

void buzzer_join () {
    pthread_join(threadBuzzer, NULL);
}

void buzzer_cleanup () {
    buzzer_join();
}

void dot_hit(int a) {
    pthread_mutex_lock(&mutex);
    hit = a;
    pthread_mutex_unlock(&mutex);
}


void* buzzer_loop() {
    while(*continueFlag2) {
        pthread_mutex_lock(&mutex);
        // Hit the dot
        if(hit == 1) {
            runCommand(DUTYCYCLE);
            runCommand("echo 300984 > /dev/bone/pwm/0/a/period");
            runCommand("echo 250492 > /dev/bone/pwm/0/a/duty_cycle");
            play_buzzer(300);

            runCommand(DUTYCYCLE);
            runCommand("echo 253100 > /dev/bone/pwm/0/a/period");
            runCommand("echo 226550 > /dev/bone/pwm/0/a/duty_cycle");
            play_buzzer(100);

            runCommand(DUTYCYCLE);
            runCommand("echo 284091 > /dev/bone/pwm/0/a/period");
            runCommand("echo 142046 > /dev/bone/pwm/0/a/duty_cycle");
            play_buzzer(100);

            runCommand(DUTYCYCLE);
            runCommand("echo 379216 > /dev/bone/pwm/0/a/period");
            runCommand("echo 189607 > /dev/bone/pwm/0/a/duty_cycle"); 
            play_buzzer(100);

            hit = 0;
        } else if(hit == 2) { // Missed the dot
            runCommand(DUTYCYCLE);
            runCommand("echo 3822256 > /dev/bone/pwm/0/a/period");
            runCommand("echo 1911128 > /dev/bone/pwm/0/a/duty_cycle");
            play_buzzer(500);

            hit = 0;
        }
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}


void play_buzzer(int time) {
    runCommand("echo 0 > /dev/bone/pwm/0/a/enable");
    runCommand("echo 1 > /dev/bone/pwm/0/a/enable");
    sleepForMs(time);
    runCommand("echo 0 > /dev/bone/pwm/0/a/enable");
}
