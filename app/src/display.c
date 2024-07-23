#include "../include/display.h"
#include "../../hal/include/config_display.h"

#define REG_DIRA 0x02
#define REG_DIRB 0x03
#define REG_OUTA 0x01
#define REG_OUTB 0x00

#define GPIO_61_ON "echo 1 > /sys/class/gpio/gpio61/value"
#define GPIO_61_OFF "echo 0 > /sys/class/gpio/gpio61/value"
#define GPIO_44_ON "echo 1 > /sys/class/gpio/gpio44/value"
#define GPIO_44_OFF "echo 0 > /sys/class/gpio/gpio44/value"

typedef struct {
    const int outA; // upper triangle
    const int outB; // lower triangle
} HexPairDigit;

// Index corresponds to actual digit 0 to 9 inclusive
const HexPairDigit digit_hex_lookup_table[10] = {
    {0xE1, 0xD0}, // 0
    {0x04, 0xC0}, // 1
    {0xC3, 0x98}, // 2
    {0x03, 0xD8}, // 3
    {0x22, 0xC8}, // 4
    {0x23, 0x58}, // 5
    {0xE3, 0x58}, // 6
    {0x05, 0x01}, // 7
    {0xE3, 0xD8}, // 8
    {0x23, 0xC8}  // 9
};

static int secondDigit;
static int firstDigit;

static int *continueFlag4;
static int score = 0;
static pthread_t displayThread;

void display_init(int* flag) {
    continueFlag4 = flag;
    pthread_create(&displayThread, NULL, display_loop, NULL);
}

void display_join() {
    pthread_join(displayThread, NULL);
}

void display_cleanup() {
    display_join();
    turn_off();
}


void set_display(long value) {
    score = value;

    if (value > 99) {
        score = 99;
    } 
        
    secondDigit = score % 10;
    score = score / 10;
    firstDigit = score % 10;   
}


void* display_loop() {
    int i2cFileDesc = get_i2c();
    
    while(*continueFlag4) {
        turn_off();

        writeI2cReg(i2cFileDesc, REG_OUTA, digit_hex_lookup_table[secondDigit].outA);
        writeI2cReg(i2cFileDesc, REG_OUTB, digit_hex_lookup_table[secondDigit].outB);
        second_on();
        sleepForMs(5);

        turn_off();

        writeI2cReg(i2cFileDesc, REG_OUTA, digit_hex_lookup_table[firstDigit].outA);
        writeI2cReg(i2cFileDesc, REG_OUTB, digit_hex_lookup_table[firstDigit].outB);
        first_on();
        sleepForMs(5);
    }

    close(i2cFileDesc);
    return NULL;
}


void first_on() {
    runCommand(GPIO_61_ON);
}

void second_on() {
    runCommand(GPIO_44_ON);
}

void turn_off() {
    runCommand(GPIO_61_OFF);
    runCommand(GPIO_44_OFF);
}
