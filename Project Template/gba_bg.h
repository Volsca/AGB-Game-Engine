
// bg registers and macros

#ifndef GBA_BG_H
#define GBA_BG_H

// BG control registers
/*
* Backgrounds can be used in two ways, text GBs or Rotation/Scaling (R/S),
* their info is then stored as Char Base Blocks (CBB) and Screen Base Blocks (SBB).
* Further documentation on these methods in GBA_BG_H.
*
* Text bacground control register (BG0-1-2-3)
* 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
*   SIZE  | XX |   Screen Base Block  | C | M | X | X |  CBB  |   P
* 0-1   : Priority of the background (equal its value, lower priority wins)
* 2-3   : Char Base Block (0-3), chooses where in memory we are going to put char data tiles (16Kb)
* 4-5   : X
* 6     : Set mosaic
* 7     : Choose between 16/256 colour palette (0 : 16x16 | 1 : 256x1)
* 8-12  : Screen Base Block (0-31), chooses where in memory we are supposed to put the map data for tiles
* 13    : X
* 14-15 : Choose the background size
*
* Text & R/S control register (BG2-3 only)
* 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
*   Size  |  W |   Screen Base Block  | C | M | X | X |  CBB  |   P
* 0-1   : Prority of the background (equal its value, lower priority wins)
* 2-3   : Char Base Block (0-3), chooses where in memory we are going to put char data tiles (16Kb per)
* 4-5   : X
* 6     : Set mosaic
* 7     : Choose between 16/256 colour palette (0 : 16x16 | 1 : 256x1)
* 8-12  : Screen Base Block (0-31), chooses where in memory we are supposed to put the map data for tiles
* 13    : Set wraparound
* 14-15 : Choose the background size
*
*/
#define REG_BG0CNT   *(u16*)0x4000008
#define REG_BG1CNT   *(u16*)0x400000A
#define REG_BG2CNT   *(u16*)0x400000C
#define REG_BG3CNT   *(u16*)0x400000E

#define BG_PaletteMem ((u16*)0x5000000) // Background palette memory

// macros for BGCNT
#define BG_MOSAIC_ENABLE 0x40
#define BG_COLOR_256     0x80
#define BG_COLOR_16      0x0

/*
* Using Tiled backgrounds, VRAM is available from 0x6000000 to 0x600F800 (64KB)
* for Char Base Blocks(0 - 3) and Screen Base Blocks(0 - 31).
* CBBs are the actual tile pixel data, partitioned in 16Kb chunks.
* SBBs are tile maps, partitioned in 2Kb chunks.
*
*Here is the map of the CBB and SBB indexes and their starting memory addresses : (2Kb chunks)
* CBB |  Address  | SBB
* 3   | 0x600F800 | 31
*     | 0x600F000 | 30
*     | 0x600E800 | 29
*     | 0x600E000 | 28
*     | 0x600D800 | 27
*     | 0x600D000 | 26
*     | 0x600C800 | 25
*     | 0x600C000 | 24
*-----|-----------|-----
* 2   | 0x600B800 | 23
*     | 0x600B000 | 22
*     | 0x600A800 | 21
*     | 0x600A000 | 20
*     | 0x6009800 | 19
*     | 0x6009000 | 18
*     | 0x6008800 | 17
*     | 0x6008000 | 16
*-----|-----------|-----
* 1   | 0x6007800 | 15
*     | 0x6007000 | 14
*     | 0x6006800 | 13
*     | 0x6006000 | 12
*     | 0x6005800 | 11
*     | 0x6005000 | 10
*     | 0x6004800 |  9
*     | 0x6004000 |  8
*-----|-----------|-----
* 0   | 0x6003800 |  7
*     | 0x6003000 |  6
*     | 0x6002800 |  5
*     | 0x6002000 |  4
*     | 0x6001800 |  3
*     | 0x6001000 |  2
*     | 0x6000800 |  1
*     | 0x6000000 |  0
*/
#define CharBaseBlock(n)   (((n)*0x4000)+0x6000000)
#define ScreenBaseBlock(n) (((n)*0x800)+0x6000000)

#define CHAR_SHIFT          2
#define SCREEN_SHIFT        8
#define TEXTBG_SIZE_256x256 0x0
#define TEXTBG_SIZE_256x512 0x8000
#define TEXTBG_SIZE_512x256 0x4000
#define TEXTBG_SIZE_512x512 0xC000

#define ROTBG_SIZE_128x128   0x0
#define ROTBG_SIZE_256x256   0x4000
#define ROTBG_SIZE_512x512   0x8000
#define ROTBG_SIZE_1024x1024 0xC000

#define WRAPAROUND 0x1

#endif

// EXTRA DOCUMENTATION : 
/*
* 
* ----- Background size table : (Actual & SBB size)
* Size Bits | Text Background | Rotat°/scaling BG
*   14-15   | N° pixls | size | N° pixls | size
*     00    |  256x256 |  2Kb |  128x128 | 256b
*     01    |  512x256 |  4Kb |  256x256 |  1Kb
*     10    |  256x512 |  4Kb |  512x512 |  4Kb
*     11    |  512x512 |  4Kb |1024x1024 | 16Kb
*
* ---- Mode support table :
* Mode | BG0 | BG1 | BG2 | BG3
*   0  | text| text| text| text
*   1  | text| text| R/S |  X
*   2  |  X  |  X  | R/S | R/S
* 
* ----- SSB formats : 
* If we use a text bg, the info every tile takes takes 16 bits
* but using rot/scale bg, the info each tile takes is 8 bits
* 
* Text Background :
* 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
*       Palette     | VF | HF |       Tile numbber (0-1023)
* 0-9   : Tile number for the tile (max 1024 tiles), using 256 colour tiles 
*		  means 512 max, stored every 2 bits (n°1 at 0, n°2 at 2 etc.)
* 10    :
* 11    :
* 12-15 :
* 
* Rotation/Scaling Background : 
* 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
*      Tile number (0-255)
*/
