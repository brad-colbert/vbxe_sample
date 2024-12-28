#include "vbxe.h"
#include "xdlc.h"

#include <stdbool.h>
#include <conio.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define VBXE_D600_BASE 0xD600
#define VBXE_D700_BASE 0xD700

//struct __vbxe* VBXE = 0xD640; // Store the pointer in the zero page
uint8_t* VBXE = 0xD600; // Store the pointer in the zero page

bool vbxe_detect() {
    // Try the first address
    if(VBXE_D640.CORE_VERSION != 0x10 && (VBXE_D640.MINOR_BERSION & 0x70) != 0x20) {
        // Try the second address
        if(VBXE_D740.CORE_VERSION != 0x10 && (VBXE_D740.MINOR_BERSION & 0x70) != 0x20) {
            //*VBXE = VBXE_D740;
            return false;
        }
    }

    return true;
}

int main(char argc, char* argv[]) {
    // The memory window to the VBXE
    uint8_t* XDL = 0x8000;
    int fd = -1;
    int i;
    uint8_t rgb[3];

    // ...
    if(!vbxe_detect()) {
        printf("VBXE not detected\n");
        return 1;
    }

    printf("VBXE detected, %04X\n", VBXE);

    //VBXE->VIDEO_CONTROL = 0x02;
    //*((uint16_t*)((uint16_t)VBXE + (uint16_t)64)) = 2;
    VBXE[64] = 2;

    // Set memory window location to 0x8000 and let only the CPU have access
    //VBXE->MEMAC_CTRL = 0x88;
    //VBXE->MEM_BANK_SEL = 0x80;
    //*((uint16_t*)((uint16_t)VBXE + (uint16_t)94)) = 128+8;
    //*((uint16_t*)((uint16_t)VBXE + (uint16_t)95)) = 128;
    VBXE[94] = 128+8;
    VBXE[95] = 128;

    // Setup XDL
    XDL[0] = 2+32+64; //XDLC_GMON | XDLC_RPTL | XDLC_OVADR; // OVERLAY, REPEAT, SET OVADR
    XDL[1] = 8+128; //XDLC_ATT | XDLC_END; // ATTRIB, END XDL
    XDL[2] = 239; // Repeat 239 lines
    // Overlay location (0x2000)
    XDL[3] = 0;
    XDL[4] = 2;
    XDL[5] = 0;
    // Step
    XDL[6] = 64; //0x40; // 320 bytes
    XDL[7] = 1; //0x01;
    // Width, palette, priority
    XDL[8] = 1+16; //XDLC_ATT_OV_WIDTH_NORMAL | XDLC_ATT_OV_PALETTE_01; // NORMAL WIDTH, PALETTE 1 FOR OVERLAY
    XDL[9] = 255-32; //XDLC_ATT_MAIN_PRIORITY_OVERLAY_ALL ^ XDLC_ATT_MAIN_PRIORITY_OVERLAY_PF1; // OVERLAY HAS PRIORITY OVER GTIA EXCEPT PF1

    // XDL ADDRESS
    VBXE[65] = 0; //->XDL_ADR0 = 0x00;
    VBXE[66] = 0; //->XDL_ADR1 = 0x00;
    VBXE[67] = 0; //->XDL_ADR2 = 0x00;
    // XDL Enable
    //VBXE->VIDEO_CONTROL = 0x03;
    //*((uint16_t*)((uint16_t)VBXE + (uint16_t)64)) = 3;
    VBXE[64] = (uint8_t)3;

    // Load the palette
    fd = open("D2:EG1.PAL", O_RDONLY);
    if(fd < 0) {
        printf("Error opening palette file\n");
        return 1;
    }
    printf("Opened file %d\n", fd);
    VBXE[68] = 0; //->CSEL = 0x00;
    VBXE[69] = 1; //->PSEL = 0x01;
    for(i = 0; i < 256; ++i) {
        read(fd, rgb, 3);
        VBXE[70] = rgb[0]; //->CR = rgb[0];
        VBXE[71] = rgb[1]; //->CG = rgb[1];
        VBXE[72] = rgb[2]; //>CB = rgb[2];
        //printf("%d %d %d\n", rgb[0], rgb[1], rgb[2]);
    }
    close(fd);

    // Load image
    fd = open("D2:EG1.PIC", O_RDONLY);
    if(fd < 0) {
        printf("Error opening image file\n");
        return 1;
    }
    printf("Opened file %d\n", fd);
    for(i = 2; i < 19; ++i) {
        VBXE[95] = 128 + i; //->MEM_BANK_SEL = 128 + i;  // MOVE WINDOW STARTING $2000
        read(fd, XDL, 4096);
        //printf("hit key to move to next %d\n", 128+i);
        //cgetc();
    }
    close(fd);

    cgetc();

    return 0;
}