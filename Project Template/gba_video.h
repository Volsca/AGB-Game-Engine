
// Video memory setup using REG_DISPCNT in gba_regs.h

#ifndef GBA_VIDEO_H
#define GBA_VIDEO_H

#include "gba_types.h"
#include "gba_regs.h"

// Define screen width and height values (better than remembering them) (240*160, I remember!)
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

// ---------------- Video Registers 
// Sorted in order

// Display control register
/* On a bit level, REG_DISPCNT looks like :
// 15 | 14 | 13 | 12 | 11 | 10 | 9  |  8 |  7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
// XX | XX | XX | OBJ| BG3| BG2| BG1| BG0| FB | OM| HB| DB| XX|   MODE
//
// DB : Current active buffer while double buffering in mode 4 & 5
// HB : Allows for horizontal blank OAM updates
// OM : Set the mapping mode for sprites 1D = 0 | 2D = 1
// FB : forced blank, screen goes white
// BG0-3 : Sets active background layers
// OBJ : activates objects (sprites)
//
// Uses the SetMode() macro defined in GBA_VIDEO_H, along with macros for each
// parameters value
// eg : SetMode(MODE_3 | BG3_ENABLE); */
#define REG_DISPCNT  *(u16*)0x4000000 

#define REG_DISPSTAT *(u16*)0x4000004 

// Vsync control (GBA uses scan lines)
#define REG_VCOUNT   *(volatile u16*)0x4000006 
// Simple macro to wait for screen writing to end
#define vsync() while(REG_VCOUNT != 160);

// Front buffer | 0x6000000-0x6013FFF in mode 3 (75Kb/96Kb of VRAM)
// 0x6000000-0x6009FFF in mode 4 (double buffering, 37,5Kb)
#define FrontBuffer   ((u16*)0x6000000) 
// Back buffer used in double buffering modes (to 0x6013FFF)
#define BackBuffer    ((u16*)0x600A000) 

// Different mode masks for DISPCNT
#define MODE_0 0x0
#define MODE_1 0x1
#define MODE_2 0x2
#define MODE_3 0x3
#define MODE_4 0x4
#define MODE_5 0x5

// define the buffer used as active (replaces directly using BackBuffer in gba_regs.h?)
#define backbuffer  0x10 

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
