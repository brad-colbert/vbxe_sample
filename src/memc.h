#ifndef _MEMC_H_
#define _MEMC_H_

// Memory control registers

//-----------------------------------------
// MEMC registers
#define MEMC_WINDOW_SIZE_4KB  0x00 // 00000000 // bit 0,1=00(256 pixels)
#define MEMC_WINDOW_SIZE_8KB  0x01 // 00000001 // bit 0,1=01(320 pixels)
#define MEMC_WINDOW_SIZE_16KB 0x02 // 00000010 // bit 0,1=10(336 pixels)
#define MEMC_WINDOW_SIZE_32KB 0x03 // 00000011 // bit 0,1=11(336 pixels)

//-----------------------------------------
// MEMS registers
#define MEMS_MGE 0x80 // 10000000 // bit 7=1

#endif // _MEMC_H_