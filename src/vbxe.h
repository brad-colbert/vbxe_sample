#ifndef VBXE_H
#define VBXE_H

/*
// VBXE main registers 
// some of the registeres can be r/w  
#define VBXE_VIDEO_CONTROL 0x40  // write
#define VBXE_CORE_VERSION  0x40  // read

#define VBXE_XDL_ADR0      0x41  // write
#define VBXE_MINOR_BERSION 0x41  // read

#define VBXE_XDL_ADR1 0x42 // write
#define VBXE_XDL_ADR2 0x43 // write 

// Pallette registers
#define VBXE_CSEL 0x44 // write 
#define VBXE_PSEL 0x45 // write 
#define VBXE_CR 0x46 // write
#define VBXE_CG 0x47 // write
#define VBXE_CB 0x48 // write 

#define VBXE_COLMASK 0x49 // write

#define VBXE_COLCLR    0x4A // write
#define VBXE_COLDETECT 0x4A // read

// 4B-4F skipped, no use

#define VBXE_BL_ADR0            0x50 // write 
#define VBXE_BLT_COLLISION_CODE 0x50 // read

#define VBXE_BL_ADR1            0x51 // write 
#define VBXE_BL_ADR2            0x52 // write 

#define VBXE_BLITTER_START 0x53 // write
#define VBXE_BLITTER_BUSY  0x53 // read 

#define VBXE_IRQ_CONTROL 0x54 // write 
#define VBXE_IRQ_STATUS  0x54 // read 

#define VBXE_P0 0x55 // write
#define VBXE_P1 0x56 // write
#define VBXE_P2 0x57 // write
#define VBXE_P3 0x58 // write

// 59-5C skipped, no use

#define VBXE_MEMAC_B_CONTROL 0x5D // write 
#define VBXE_MEMAC_CTRL      0x5E // write & read
#define VBXE_MEM_BANK_SEL    0x5F // write & read 
*/

#include <types.h>

struct __vbxe {
    union {
        uint8_t VIDEO_CONTROL;
        uint8_t CORE_VERSION;
    };
    union {
        uint8_t XDL_ADR0;
        uint8_t MINOR_BERSION;
    };
    uint8_t XDL_ADR1;
    uint8_t XDL_ADR2;
    uint8_t CSEL;
    uint8_t PSEL;
    uint8_t CR;
    uint8_t CG;
    uint8_t CB;
    uint8_t COLMASK;
    union {
        uint8_t COLCLR;
        uint8_t COLDETECT;
    };
    uint8_t reserved1[5]; // 4B-4F skipped
    union {
        uint8_t BL_ADR0;
        uint8_t BLT_COLLISION_CODE;
    };
    uint8_t BL_ADR1;
    uint8_t BL_ADR2;
    union {
        uint8_t BLITTER_START;
        uint8_t BLITTER_BUSY;
    };
    union {
        uint8_t IRQ_CONTROL;
        uint8_t IRQ_STATUS;
    };
    uint8_t P0;
    uint8_t P1;
    uint8_t P2;
    uint8_t P3;
    uint8_t reserved2[4]; // 59-5C skipped
    uint8_t MEMAC_B_CONTROL;
    uint8_t MEMAC_CTRL;
    uint8_t MEM_BANK_SEL;
};

#define VBXE_D640 (*(struct __vbxe*)0xD640)
#define VBXE_D740 (*(struct __vbxe*)0xD740)

#endif // VBXE_H