/*
    NeoPixel RGBW demo program for 8 LED unit such as:
        https://www.adafruit.com/product/2868
    NOTE: This is RGBW, not RGB!

    Install process
    - Put the NeoPixel into a breadboard
    - Connect the NeoPixel with 3 wires:
        - Connect NeoPixel "GND" and "DIN" (data in) to the 3-pin "LEDS" header on Zen
            Zen Cape's LEDS header:
                Pin 1: DIN (Data): left most pin; beside USB-micro connection, connects to P8.11
                Pin 2: GND (Ground): middle pin
                Pin 3: Unused (it's "5V external power", which is not powered normally on the BBG)
        - Connect NeoPixel "5VDC" to P9.7 or P9.8
            Suggest using the header-extender to make it easier to make a good connection.
        - OK to wire directly to BBG: no level shifter required.
    - Software Setup
        - On Host
            make      # on parent folder to copy to NFS
        - On Target: 
            config-pin P8.11 pruout
            make
            make install_PRU0
    - All lights should light up on the LED strip

    Based on code from the PRU Cookbook by Mark A. Yoder:
        https://beagleboard.org/static/prucookbook/#_setting_neopixels_to_different_colors
*/

#include <stdint.h>
#include <pru_cfg.h>
#include <stdbool.h>

#include "resource_table_empty.h"
#include "./sharedDataStruct.h"

#define JOYSTICK_RIGHT_MASK (1 << 15)
#define JOYSTICK_DOWN_MASK (1 << 14)

#define STR_LEN         8      
#define oneCyclesOn     700/5   // Stay on 700ns
#define oneCyclesOff    800/5
#define zeroCyclesOn    350/5
#define zeroCyclesOff   600/5
#define resetCycles     60000/5 // Must be at least 50u, use 60u

volatile register uint32_t __R30;
volatile register uint32_t __R31;

#define DATA_PIN 15          

#define THIS_PRU_DRAM  0x00000         
#define OFFSET 0x200           
#define THIS_PRU_DRAM_USABLE (THIS_PRU_DRAM + OFFSET)


volatile sharedMemStruct_t *pSharedMemStruct = (volatile void *)THIS_PRU_DRAM_USABLE;

int terminate = 0;

void reset_mem(volatile sharedMemStruct_t * pSharedMemory);
void reset_col(uint32_t *color);
void show_led(uint32_t * color);



void show_led(uint32_t * color)
{
    __delay_cycles(resetCycles);

    for(int j = 0; j < STR_LEN; j++) {
        for(int i = 31; i >= 0; i--) {
            if(color[j] & ((uint32_t)0x1 << i)) {
                __R30 |= 0x1<<DATA_PIN;      // Set the GPIO pin to 1
                __delay_cycles(oneCyclesOn-1);
                __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
                __delay_cycles(oneCyclesOff-2);
            } else {
                __R30 |= 0x1<<DATA_PIN;      // Set the GPIO pin to 1
                __delay_cycles(zeroCyclesOn-1);
                __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
                __delay_cycles(zeroCyclesOff-2);
            }
        }
    }

    __R30 &= ~(0x1<<DATA_PIN); 

}

void reset_mem(volatile sharedMemStruct_t *pSharedMemory) {
    pSharedMemory->down = false;
    pSharedMemory->right = false;
    pSharedMemory->pos1 = 0;
    pSharedMemory->pos2 = 0;
    pSharedMemory->pos3 = 0;
    pSharedMemory->pos4 = 0;
    pSharedMemory->pos5 = 0;
    pSharedMemory->pos6 = 0;
    pSharedMemory->pos7 = 0;
    pSharedMemory->pos8 = 0;
}

void reset_col(uint32_t *color) {
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    color[3] = 0;
    color[4] = 0;
    color[5] = 0;
    color[6] = 0;
    color[7] = 0;
}


void main() {
    uint32_t color[STR_LEN];

    reset_mem(pSharedMemStruct);

    pSharedMemStruct->stop = terminate;
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;


    while(!terminate) {
        terminate = pSharedMemStruct->stop;

        pSharedMemStruct->down = (__R31 & JOYSTICK_DOWN_MASK) == 0;
        pSharedMemStruct->right = (__R31 & JOYSTICK_RIGHT_MASK) == 0;

        color[0] = pSharedMemStruct->pos1;
        color[1] = pSharedMemStruct->pos2;
        color[2] = pSharedMemStruct->pos3;
        color[3] = pSharedMemStruct->pos4;
        color[4] = pSharedMemStruct->pos5;
        color[5] = pSharedMemStruct->pos6;
        color[6] = pSharedMemStruct->pos7;
        color[7] = pSharedMemStruct->pos8;

        if(pSharedMemStruct->stop) {
            reset_mem(pSharedMemStruct);
            reset_col(color);
        }

        show_led(color);      
    }
}