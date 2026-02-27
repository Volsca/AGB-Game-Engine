
// gba macros for ease of use

#ifndef GBA_TYPES_H
#define GBA_TYPES_H

// Common unsigned types
typedef unsigned char  u8;
typedef unsigned short u16; 
typedef unsigned long  u32;

// Common signed types (no floats idioto)
typedef signed char  s8;
typedef signed short s16;
typedef signed long  s32;

// RGB values (15bit colour) 5 bits each for r, g and b
#define RGB(r,g,b) ((u16)(r | (g<<5) | (b<<10))) 

#endif
