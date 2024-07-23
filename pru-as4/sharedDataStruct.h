#ifndef _SHAREDDATASTRUCT_H_
#define _SHAREDDATASTRUCT_H_

#include <stdint.h>
#include <stdbool.h>



typedef struct {
    bool down;
    bool right;
    uint8_t stop;

    _Alignas(4) uint32_t pos1;
    _Alignas(4) uint32_t pos2;
    _Alignas(4) uint32_t pos3;
    _Alignas(4) uint32_t pos4;
    _Alignas(4) uint32_t pos5;
    _Alignas(4) uint32_t pos6;
    _Alignas(4) uint32_t pos7;
    _Alignas(4) uint32_t pos8;
    
    
} sharedMemStruct_t;

#endif
