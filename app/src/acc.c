#include "../include/acc.h"
#include "../include/shared_linux.h"
#include "../../hal/include/config_display.h"

#define RED_LIGHT 0x000f0000
#define GREEN_LIGHT 0x0f000000
#define BLUE_LIGHT 0x00000f00
#define RED_BRIGHT 0x00ff0000 
#define GREEN_BRIGHT 0xff000000
#define BLUE_BRIGHT 0x0000ff00 

#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B

static float xdot;
static float ydot;

static int *continueFlag3;

static int loc_up;
static int loc_middle;
static int loc_down;
static uint32_t colour;
static uint32_t up_colour;
static uint32_t m_colour;
static uint32_t down_colour;

static unsigned char xl;
static unsigned char xh;
static unsigned char yl;
static unsigned char yh;

static int val;
static float x_value;
static float y_value;
static int x_calculation;
static int y_calculation;
static int target = 0;

static pthread_t accThread;


void acc_init(int* flag) {
    continueFlag3 = flag;
    
    // config_pins();
    writeI2cReg(get_i2c_acc(), 0x20, 0x27);
    new_dot();

    pthread_create(&accThread, NULL, acc_loop, NULL);
}


void acc_join() {
    pthread_join(accThread, NULL);
}


void acc_cleanup() {
    acc_join();
}

int get_target() {
    return target;
}

void new_dot() {
    srand(time(NULL));
    xdot = -0.5 + ((float)rand() / (float)RAND_MAX) * (0.5 - -0.5);
    ydot = -0.5 + ((float)rand() / (float)RAND_MAX) * (0.5 - -0.5);
}


void* acc_loop() {
    int16_t x;
    int16_t y;
    val = get_i2c_acc();  

    while(*continueFlag3) {
        xl = readI2cReg(val, OUT_X_L);
        xh = readI2cReg(val, OUT_X_H);
        x = (xh << 8) | (xl);
        x_value = (float)x / 16000;

        yl = readI2cReg(val, OUT_Y_L);
        yh = readI2cReg(val, OUT_Y_H);
        y = (yh << 8) | (yl);
        y_value = (float)y / 16000;

        if(x_value > (xdot + 0.075)){
            colour = RED_LIGHT;
            up_colour = RED_LIGHT;
            m_colour = RED_BRIGHT;
            down_colour = RED_LIGHT;
            x_calculation = 1;
        } else if(x_value < (xdot - 0.075)) {
            colour = GREEN_LIGHT;
            up_colour = GREEN_LIGHT;
            m_colour = GREEN_BRIGHT;
            down_colour = GREEN_LIGHT;
            x_calculation = 1;
        } else {
            colour = BLUE_LIGHT;
            up_colour = BLUE_LIGHT;
            m_colour = BLUE_BRIGHT;
            down_colour = BLUE_LIGHT;
            x_calculation = 0;
        }


        if(y_value > (ydot + 0.4)) {
            loc_down = 10;
            loc_middle = 9;
            loc_up = 8;
        } else if(y_value > (ydot + 0.3)) {
            loc_down = 9;
            loc_middle = 8;
            loc_up = 7;
        } else if(y_value > (ydot + 0.2)) {
            loc_down = 8;
            loc_middle = 7;
            loc_up = 6;
        } else if(y_value > (ydot + 0.1)) {
            loc_down = 7;
            loc_middle = 6;
            loc_up = 5;
        } else if(y_value > (ydot + 0.05)) {
            loc_down = 6;
            loc_middle = 5;
            loc_up = 4;
        } else if(y_value <= (ydot + 0.05) && y_value >= (ydot - 0.05)) {
            loc_down = 0;
            loc_middle = 0;
            loc_up = 0;
        } else if(y_value > (ydot - 0.1)) {
            loc_down = 3;
            loc_middle = 4;
            loc_up = 5;
        } else if(y_value > (ydot - 0.2)) {
            loc_down = 2;
            loc_middle = 3;
            loc_up = 4;
        } else if(y_value > (ydot - 0.3)) {
            loc_down = 1;
            loc_middle = 2;
            loc_up = 3;
        } else if(y_value > (ydot - 0.4)) {
            loc_down = 0;
            loc_middle = 1;
            loc_up = 2;
        } else if(y_value <= (ydot - 0.4)) {
            loc_down = -1;
            loc_middle = 0;
            loc_up = 1;
        }

        if(loc_up == 0 && loc_down == 0 && loc_middle == 0) {
            set_colour(colour);
            y_calculation = 0;
        } else {
            set_colour(0);
            set_colour_miss(up_colour, loc_up);
            set_colour_miss(m_colour, loc_middle);
            set_colour_miss(down_colour, loc_down);
            y_calculation = 1;
        }  

        if (x_calculation == 0 && y_calculation == 0) {
            target = 1;
        } else {
            target = 0;
        }

        sleepForMs(200);
    }

    close(val);
    return NULL;
}
