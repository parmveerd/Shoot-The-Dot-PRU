#include "../include/config_display.h"

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x20
#define I2C_DEVICE_ADDRESS_ACC 0x18

#define SET_0x02_0  "i2cset -y 1 0x20 0x02 0x00"
#define SET_0x03_0  "i2cset -y 1 0x20 0x03 0x00"
#define SET_0x08_0  "i2cset -y 1 0x20 0x08 0x00"
#define SET_0x09_0  "i2cset -y 1 0x20 0x09 0x00"

#define DIRECTION_61_OUT "echo out > /sys/class/gpio/gpio61/direction"

#define DIRECTION_44_OUT "echo out > /sys/class/gpio/gpio44/direction"

#define CONFIG_P17 "config-pin p9_17 i2c > /dev/null"
#define CONFIG_P18 "config-pin p9_18 i2c > /dev/null"

#define CONFIG_P11 "config-pin P8_11 pruout > /dev/null"
#define CONFIG_P15 "config-pin p8_15 pruin > /dev/null"

static int desc;
static int desc_acc;


void config_pins() {
    desc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    desc_acc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS_ACC);

    runCommand(CONFIG_P17);
    runCommand(CONFIG_P18);
    runCommand(CONFIG_P11);
    runCommand(CONFIG_P15);
    config_nums();
    runCommand(DIRECTION_44_OUT);
    runCommand(DIRECTION_61_OUT); 
}

int get_i2c() {
    return desc;
}

int get_i2c_acc() {
    return desc_acc;
}


void config_nums() {
    runCommand(SET_0x02_0);
    runCommand(SET_0x03_0);
    runCommand(SET_0x08_0);
    runCommand(SET_0x09_0);
}

int initI2cBus(char* bus, int address)
{
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);

    if (result < 0) {
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }
    return i2cFileDesc;
}

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2) {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
    int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
    if (res != sizeof(regAddr)) {
        perror("I2C: Unable to write to i2c register.");
        exit(1);
    }
    char value = 0;
    res = read(i2cFileDesc, &value, sizeof(value));
    if (res != sizeof(value)) {
        perror("I2C: Unable to read from i2c register");
        exit(1);
    }
    return value;
}

void runCommand(char* command)
{
    FILE *pipe = popen(command, "r");

    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL) {
            break;
        }
    }

    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}
