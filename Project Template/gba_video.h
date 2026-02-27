
// Video memory setup using REG_DISPCNT in gba_regs.h

#ifndef GBA_VIDEO_H
#define GBA_VIDEO_H

#include "gba_types.h"
#include "gba_regs.h"

// Define screen width and height values (better than remembering them) (240*160, I remember!)
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

// Different mode masks for DISPCNT
#define MODE_0 0x0
#define MODE_1 0x1
#define MODE_2 0x2
#define MODE_3 0x3
#define MODE_4 0x4
#define MODE_5 0x5

#define backbuffer  0x10 // define the buffer used as active (replaces directly using BackBuffer in gba_regs.h?)

// When set, this bit allows OAM to be updated during a horizontal blank | ?
#define H_BLANK_OAM 0x20

#define OBJ_MAP_2D 0x00
#define OBJ_MAP_1D 0x40

#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200
#define BG2_ENABLE 0x400
#define BG3_ENABLE 0x800
#define OBJ_ENABLE 0x1000

//Set the mode that you want to use, logical OR them together as below:
//e.g. SetMode(MODE_2 | OBJ_ENABLE | OBJ_MAP_1D);
#define SetMode(mode) REG_DISPCNT = (mode)

#endif
