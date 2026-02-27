
// Sprite macros

#ifndef GBA_SPRITES_H
#define GBA_SPRITES_H

#include "gba_types.h"

// Sprite structure definitions 
// 8 bytes per entry
typedef struct tagOAMEntry
{
	u16 attribute0;
	u16 attribute1;
	u16 attribute2;
	u16 attribute3;
}OAMEntry, *pOAMEntry;

// attribute0
/* Attribute 0 looks like this
// 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
//  shape  | C  | M  |  Alpha  | SD| R |        Y - coordinate
// 0-7   : y coordinate (0-255, limited to 0-159 screen size)
// 8     : Set sprite rotation
// 9     : Set double size. Is used when rotating sprites, 
//         preventing parts of the sprite from disappearing during rotation
// 10-11 : Set alpha blending
// 12    : Set mosaic flag
// 13    : Set colour palette mode. (1 = 256 colours, 0 = 16) Palette set in attribute 2
// 14-15 : Together with 14-15 in attr1, sets which shape the sprite will have */
#define ROTATION_FLAG    0x0100
#define SIZE_DOUBLE      0x0200
#define MODE_NORMAL      0x0000
#define MODE_TRANSPARENT 0x0400
#define MODE_WINDOWED    0x0800
#define MOSAIC           0x1000
#define COLOR_16         0x0000
#define COLOR_256        0x2000
#define SQUARE           0x0000
#define WIDE			 0x4000
#define TALL			 0x8000

// attribute1
/* Attribute 1 looks like this : 
// 15 | 14 | 13 | 12 |   11 | 10 | 9  | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
//  form   | Vf | HF | Rot data index |         X - coordinate
// 0-8   : x coordinate (0-512, limited to 0-239 screen size)
// 9-13  : if bit 8 attr0 is set, determines rot/scale parametres is being used
// 12    : Set horizontal flip (just flips it)
// 13    : //  vertical //
// 14-15 : Together with 14-15 in attr0, sets which shapt the sprite will have*/
#define ROTDATA(n)      (n<<9)
#define HORIZONTAL_FLIP 0x1000
#define VERTICAL_FLIP   0x2000
#define SIZE_8		    0x0000
#define SIZE_16	        0x4000
#define SIZE_32         0x8000
#define SIZE_64			0xC000

// attribute2
/* attribute 2 looks like this : 
// 15 | 14 | 13 | 12 | 11 | 10  | 9  | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
//  palette number   | priority |             sprite number            
// 0-9   : Decides where sprite data is stored (8x8 tiles, 32 bytes)  
//         in 16 colour mode  - index 0 is the first tile, 1 the next, etc. (00 01  10  11)
//         in 256 colour mode - index 0 is the first tile, 2 the next, etc. (00 10 100 110)        
// 10-11 : Set priority (0-3), which determines layering (0 first, 3 last)
//         Same priority sprites/background will always draw sprite on top
//         Two sprites with same priority will draw the one with the lower OAM index
// 12-15 : Determines palette used for sprite. Unused in 256 mode
// */
#define PRIORITY(n) ((n)<<10)
#define PALETTE(n)  ((n)<<12)

// no attribute3 ?

typedef struct tagRotData
{
	u16 filler1[3];
	u16 pa;
	u16 filler2[3];
	u16 pb;
	u16 filler3[3];
	u16 pc;
	u16 filler4[3];
	u16 pd;
}RotData, *pRotData;

#endif

// EXTRA DOCUMENTATION : 
/* This table shows the 122 possible shapes of sprites, and their mem usage : 
*        |      |      |  max    |  num  |  max    |  num 
*  Shape | Form | Size | sprites | bytes | sprites | bytes
*        |      |      |  256    | 256   |  16     | 16
* -------|------|------|---------|-------|---------|------
* Square | S_8  |8x8   |   512   |   64  |  1024   |   32
* Square | S_16 |16x16 |   128   |  256  |   256   |  128
* Square | S_32 |32x32 |    32   | 1024  |    64   |  512
* Square | S_64 |64x64 |     8   | 4096  |    16   | 2048
* -------|------|------|---------|-------|---------|------
* Tall   | S_8  |8x16  |   256   |  128  |   512   |   64
* Tall   | S_16 |8x32  |   128   |  256  |   256   |  128
* Tall   | S_32 |16x32 |    64   |  512  |   128   |  256
* Tall   | S_64 |32x64 |    16   | 2048  |    32   | 1024
* -------|------|------|---------|-------|---------|------
* Wide   | S_8  |16x8  |   256   |  128  |   512   |   64
* Wide   | S_16 |32x8  |   128   |  256  |   256   |  128
* Wide   | S_32 |32x16 |    64   |  512  |   128   |  256
* Wide   | S_64 |64x32 |    16   | 2048  |    32   | 1024
* 
* S_8 : SIZE_8 | S_16 : SIZE_16 | S_32 : SIZE_32 | S_64 | SIZE_64
*/
