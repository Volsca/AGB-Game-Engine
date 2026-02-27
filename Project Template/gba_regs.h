
// common gba registers
// mem size non specified (TODO), may depend on graphics mode

#ifndef GBA_REGS_H
#define GBA_REGS_H

#include "gba_types.h"

// OAM : Object Attribute Memory
#define OAM_Mem        ((u16*)0x7000000) // Sprites, coordinates, size. 128 sprites, 8 bytes per.
										 // (0x7000000 - 0x70003FF)
#define OBJ_PaletteMem ((u16*)0x5000200) // Sprite palette memory (512 bytes - 0x50003FF)
#define OAM_Data       ((u16*)0x6010000) //Sprite pixel data

#define FrontBuffer   ((u16*)0x6000000) // Front buffer | 0x6000000-0x6013FFF in mode 3 (75Kb/96Kb of VRAM)
										// 0x6000000-0x6009FFF in mode 4 (double buffering, 37,5Kb)
#define BackBuffer    ((u16*)0x600A000) // Back buffer used in double buffering modes (to 0x6013FFF)
#define BG_PaletteMem ((u16*)0x5000000) // Background palette memory

// Display control mode
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

#define REG_VCOUNT   *(volatile u16*)0x4000006 // Vsync control (GBA uses scan lines)
#define vsync() while(REG_VCOUNT != 160); // Simple macro to wait for screen writing to end

#endif
