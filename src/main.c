#include "vbxe.h"
#include "xdlc.h"

#include <stdbool.h>
#include <conio.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
// #include <string.h>
// #include <ctype.h> 

#define VBXE_D600_BASE 0xD600
#define VBXE_D700_BASE 0xD700

//unsigned short* VBXE_BASE_ADDR = 0x00FE;  // Put it in zero page
struct __vbxe* VBXE = 0xD640; // Store the pointer in the zero page

/*
void vbxe_version(void* VBXE_BASE, byte* vbxe_major_version, byte* vbxe_minor_version) {
    *vbxe_major_version = *((byte*)VBXE_BASE + VBXE_CORE_VERSION);
    *vbxe_minor_version = *((byte*)VBXE_BASE + VBXE_CORE_VERSION + 1);
}
*/

bool vbxe_detect() {
    /*
    byte vbxe_major_version, vbxe_minor_version;

    *VBXE_BASE_ADDR = VBXE_D600_BASE;
    vbxe_version((void*)*VBXE_BASE_ADDR, &vbxe_major_version, &vbxe_minor_version);

    if(vbxe_major_version != 0x10 && (vbxe_minor_version & 0x70) != 0x20) {
        *VBXE_BASE_ADDR = VBXE_D700_BASE;
        vbxe_version((void*)VBXE_D700_BASE, &vbxe_major_version, &vbxe_minor_version);
        if(vbxe_major_version != 0x10 && (vbxe_minor_version & 0x70) != 0x20) {
            *VBXE_BASE_ADDR = 0x0000;
            return false;
        }
    }
    */
    // Try the first address
    if(VBXE_D640.CORE_VERSION != 0x10 && (VBXE_D640.MINOR_BERSION & 0x70) != 0x20) {
        // Try the second address
        if(VBXE_D740.CORE_VERSION != 0x10 && (VBXE_D740.MINOR_BERSION & 0x70) != 0x20) {
            *VBXE = VBXE_D740;
            return false;
        }
    }

    return true;
}

/*
void vbxe_disable()
{
   //   *((byte*)(*VBXE_BASE_ADDR + VBXE_VIDEO_CONTROL)) = 0x00;
   VBXE->VIDEO_CONTROL = 0x00;
}

void vbxe_enable()
{
   //   *((byte*)(*VBXE_BASE_ADDR + VBXE_VIDEO_CONTROL)) = 0x01;
    VBXE->VIDEO_CONTROL = 0x01;
}

void vbxe_clear_screen_mem()
{

}
*/

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

    VBXE->VIDEO_CONTROL = 0x02;

    //vbxe_disable();

    // Set memory window location to 0x8000 and let only the CPU have access
    // *((byte*)(*VBXE_BASE_ADDR + VBXE_MEMAC_CTRL)) = 0x88;
    // *((byte*)(*VBXE_BASE_ADDR + VBXE_MEM_BANK_SEL)) = 0x80;
    VBXE->MEMAC_CTRL = 0x88;
    VBXE->MEM_BANK_SEL = 0x80;

    // Setup XDL
    XDL[0] = XDLC_GMON | XDLC_RPTL | XDLC_OVADR; // OVERLAY, REPEAT, SET OVADR
    XDL[1] = XDLC_ATT | XDLC_END; // ATTRIB, END XDL
    XDL[2] = 239; // Repeat 239 lines
    // Overlay location (0x2000)
    XDL[3] = 0;
    XDL[4] = 2;
    XDL[5] = 5;
    // Step
    XDL[6] = 0x40; // 320 bytes
    XDL[7] = 0x01;
    // Width, palette, priority
    XDL[8] = XDLC_ATT_OV_WIDTH_NORMAL | XDLC_ATT_OV_PALETTE_01; // NORMAL WIDTH, PALETTE 1 FOR OVERLAY
    XDL[9] = XDLC_ATT_MAIN_PRIORITY_OVERLAY_ALL ^ XDLC_ATT_MAIN_PRIORITY_OVERLAY_PF1; // OVERLAY HAS PRIORITY OVER GTIA EXCEPT PF1

    // XDL ADDRESS
    VBXE->XDL_ADR0 = 0x00;
    VBXE->XDL_ADR1 = 0x00;
    VBXE->XDL_ADR2 = 0x00;
    // XDL Enable
    VBXE->VIDEO_CONTROL = 0x03;

    // Load the palette
    fd = open("D2:EG1.PAL", O_RDONLY);
    if(fd < 0) {
        printf("Error opening palette file\n");
        return 1;
    }
    printf("Opened file %d\n", fd);
    VBXE->PSEL = 0x00;
    VBXE->CSEL = 0x00;
    for(i = 0; i < 256; ++i) {
        read(fd, rgb, 3);
        //printf("(%d %d %d) ", rgb[0], rgb[1], rgb[2]);
        VBXE->CR = rgb[0];
        VBXE->CG = rgb[1];
        VBXE->CB = rgb[2];
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
        VBXE->MEM_BANK_SEL = 128 + i;  // MOVE WINDOW STARTING $2000
        read(fd, XDL, 4096);
    }
    close(fd);

    cgetc();

    return 0;
}