#include "../include/shared_linux.h"
#include "../../hal/include/config_display.h"
#include "../../pru-as4/sharedDataStruct.h"

#define CONFIGURE_PIN_815 "config-pin p8_15 pruin"
#define CONFIGURE_PIN_816 "config-pin p8_16 pruin"

// General PRU Memomry Sharing Routine
// ----------------------------------------------------------------
#define PRU_ADDR            0x4A300000   // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN             0x80000      // Length of PRU memory
#define PRU0_DRAM           0x00000      // Offset to DRAM
#define PRU1_DRAM           0x02000
#define PRU_SHAREDMEM       0x10000      // Offset to shared memory
#define PRU_MEM_RESERVED    0x200        // Amount used by stack and heap

// Convert base address to each memory section
#define PRU0_MEM_FROM_BASE(base) ((volatile sharedMemStruct_t *)((uintptr_t)(base) + PRU0_DRAM + PRU_MEM_RESERVED))
#define PRU1_MEM_FROM_BASE(base) ((volatile sharedMemStruct_t *)((uintptr_t)(base) + PRU1_DRAM + PRU_MEM_RESERVED))
#define PRUSHARED_MEM_FROM_BASE(base) ( (base) + PRU_SHAREDMEM)

volatile void *pPruBase0;
volatile sharedMemStruct_t *pSharedPru0;

volatile void* getPruMmapAddr(void);
void freePruMmapAddr(volatile void* pPruBase);


void data_init() {
    runCommand(CONFIGURE_PIN_815);
    runCommand(CONFIGURE_PIN_816);

    pPruBase0 = getPruMmapAddr();
    pSharedPru0 = PRU0_MEM_FROM_BASE(pPruBase0);
}

volatile void* getPruMmapAddr(void)
{
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("ERROR: could not open /dev/mem");
        exit(EXIT_FAILURE);
    }

    volatile void* pPruBase = mmap(0, PRU_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR);
    if (pPruBase == MAP_FAILED) {
        perror("ERROR: could not map memory");
        exit(EXIT_FAILURE);
    }
    close(fd);

    return pPruBase;
}

void freePruMmapAddr(volatile void* pPruBase)
{
    if (munmap((void*) pPruBase, PRU_LEN)) {
        perror("PRU munmap failed");
        exit(EXIT_FAILURE);
    }
}

void data_cleanup() {
    freePruMmapAddr(pPruBase0);
}

bool joy_down() {
    return pSharedPru0->down;
}

bool joy_right() {
    return pSharedPru0->right;
}

void stop_flag() {
    pSharedPru0->stop = 1;
}

// Got this function and how to set it up in shared data pru from ChatGPT
void set_colour(uint32_t temp) {
    pSharedPru0->pos1 = temp;
    pSharedPru0->pos2 = temp;
    pSharedPru0->pos3 = temp;
    pSharedPru0->pos4 = temp;
    pSharedPru0->pos5 = temp;
    pSharedPru0->pos6 = temp;
    pSharedPru0->pos7 = temp;
    pSharedPru0->pos8 = temp;
}

// Chat GPT helped with this function as well
void set_colour_miss(uint32_t temp, int p) {
    if (p == 1) {
        pSharedPru0->pos1 = temp;
    } else if (p == 2) {
        pSharedPru0->pos2 = temp;
    } else if (p == 3) {
        pSharedPru0->pos3 = temp;
    } else if (p == 4) {
        pSharedPru0->pos4 = temp;
    } else if (p == 5) {
        pSharedPru0->pos5 = temp;
    } else if (p == 6) {
        pSharedPru0->pos6 = temp;
    } else if (p == 7) {
        pSharedPru0->pos7 = temp;
    } else if (p == 8) {
        pSharedPru0->pos8 = temp;
    }
}


