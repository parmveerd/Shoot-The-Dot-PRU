#ifndef _CONFIG_DISPLAY_H_
#define _CONFIG_DISPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


void config_pins();
int get_i2c();
int get_i2c_acc();
void config_nums();
int initI2cBus(char*, int);
void writeI2cReg(int, unsigned char, unsigned char);
unsigned char readI2cReg(int, unsigned char);
void runCommand(char*);
void sleepForMs(long long);

#endif