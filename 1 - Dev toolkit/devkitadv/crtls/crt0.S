/********************************************************************
 *   crt0.S for DevKit Advance R5 by Jason Wilkins <fenix@io.com>   *
 ********************************************************************/

@@@@@@@@@@@@@@@@
@ Adminitrivia @
@@@@@@@@@@@@@@@@

@ This file is released into the public domain for commercial
@ or non-commercial usage with no restrictions placed upon it.

@ THERE IS NO WARRANTY EXPRESSED OR IMPLIED FOR THIS SOFTWARE
@ INCLUDING, BUT NOT LIMITED TO, FUNCTIONALITY OR FITNESS FOR
@ A PARTICULAR USE.  THE RISK OF USING THE SOFTWARE IS YOURS.
@ THOSE WHO HAVE PROVIDED THIS SOFTWARE TO YOU ACCEPT NO
@ LIABILITY.

@ Based on crt0.S v1.28 by Jeff Frohwein
@ The change log associated with that version has been removed
@ because it was not relavent anymore.  There are significant
@ modifications made for DevKit Advance R5

@ The following people have contributed:
@	DarkFader
@	Jeff Frohwien
@	Mike Heckenbach
@	Mark Price
@	Jason Wilkins <fenix@io.com>
@	and others

@ ChangeLog
@   R5 - using the C preprocessor for some macros
@      - removed interrupt processing code to a seperate library file
@      - uses _init/_fini instead of __main for C++ ctor/dtor support
@      - no longer clears ext/int ram to zero, only the bss
@      - removed __FarProcedure/__FarFunction because they are unnecessary
@      - no longer saving the user a few miliseconds by allowing DMA Copy/Clear
@      - hid many labels from public view by prefixing them with .L*



@@@@@@@@@@
@ Macros @
@@@@@@@@@@

#define CALL_COPY_MEMORY(dst_start, src_start, src_end) \
	ldr r0, =dst_start ; ldr r1, =src_start ; ldr r2, =src_end ; bl __crt0_copy_memory

#define CALL_ZERO_MEMORY(start, end) \
	ldr r0, =start ; ldr r1, =end ; bl __crt0_zero_memory



@@@@@@@@@@
@ Header @
@@@@@@@@@@

	.text

	.global	_start
        .arm				@ code starts in ARM mode
_start:
	@ Start Vector
	b	.Lrom_header_end	@ skip the first part of the header

	@ Nintendo Logo Character Data (8000004h)
	.fill   156,1,0

	@ Game Title (80000A0h)
	.byte   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	.byte   0x00,0x00,0x00,0x00

	@ Game Code (80000ACh)
	.byte   0x00,0x00,0x00,0x00

	@ Maker Code (80000B0h)
	.byte   0x30,0x31

	@ Fixed Value (80000B2h)
	.byte   0x96

	@ Main Unit Code (80000B3h)
	.byte   0x00

	@ Device Type (80000B4h)
	.byte   0x00

	@ Unused Data (7Byte) (80000B5h)
	.byte   0x00,0x00,0x00,0x00,0x00,0x00,0x00

	@ Software Version No (80000BCh)
 	.byte   0x00

	@ Complement Check (80000BDh)
 	.byte   0xf0

	@ Checksum (80000BEh)
	.byte   0x00,0x00

	.arm
.Lrom_header_end:
	b	__crt0_init		@ skip the second part of the header

@ The following reserved bytes are used if the code is compiled for
@ multiboot mode. It does not hurt anything to leave this header in
@ even if the code is not compiled for multiboot. The GBA BIOS will
@ auto-patch the first two bytes with 0x03 and 0x01, respectively,
@ before running any code if it is executed as multiboot.

@ The following two bytes are included even for non-multiboot supporting
@ builds to guarantee that any generic library code that depends on them
@ will still be functional.

	.global	__boot_method
__boot_method:
        .byte   0       @ boot method (0=ROM boot, 3=Multiplay boot)

	.global __slave_number
__slave_number:
        .byte   0       @ slave # (1=slave#1, 2=slave#2, 3=slave#3)

.Lmb_reserved:
        .byte   0       @ reserved
        .byte   0       @ reserved
        .word   0       @ reserved
        .word   0       @ reserved
        .word   0       @ reserved
        .word   0       @ reserved
        .word   0       @ reserved
        .word   0       @ reserved



@@@@@@@@@@@@@@@@@@@@@@
@        Reset       @
@@@@@@@@@@@@@@@@@@@@@@

	.align
	.arm				@ IRQ setup has to be done using ARM mode
__crt0_init:
	@ set irq stack pointer
	mov	r0, #0x12		@ switch to IRQ mode
	msr	cpsr, r0
	ldr	ip, =__sp_irq
	ldr	sp, [ip]

	@ set user stack pointer
	mov	r0, #0x1f		@ switch to system mode
	msr	cpsr, r0
	ldr	ip, =__sp_usr
	ldr	sp, [ip]

	@ switch to thumb mode
	adr	r0, .Lthumb + 1
	bx	r0

	.align 2
        .thumb
.Lthumb:



@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Multiboot Copy Routine @
@@@@@@@@@@@@@@@@@@@@@@@@@@

@ Check to see if the image has been linked for ewram or for
@ ROM.  If it has been linked for ewram then check if the
@ program is currently running from ewram or from ROM.  If
@ it is running from ROM, then it needs to be copied to 
@ ewram and execution restarted.
@
@ The reason for all this is to allow a program to be used
@ "as is" with an flash-cart, emulator, or MBV2-style
@ multiboot cable.

	@ if _start symbol has not been placed in ROM then ...
	ldr	r0, =_start
	lsl	r0, #5
	bcs	.Linit_data

	@ if program is currently running out of ROM then ...
	mov	r0, pc
	lsl	r0, #5
	bcc	.Linit_data

	@ copy ROM image ROM to were is says it supposed to be
	CALL_COPY_MEMORY(_start, __memmap_rom_start, __rom_break)

	@ and restart execution (in ARM mode).
	ldr	r0, =_start
	bx	r0

	@ else, continue on, because the program is linked
	@ and running from where it is supposed to be.



@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ User Variable Initialization @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.Linit_data:

	@ Clear uninitialized data (.bss section) to zero
	CALL_ZERO_MEMORY(__bss_start, __bss_end)

	@ Copy initialized data (.data section) from LMA to VMA (ROM to RAM)
	CALL_COPY_MEMORY(__data_start, __load_start_data, __load_stop_data)

	@ Copy internal work ram (.iwram section) from LMA to VMA (ROM to RAM)
	CALL_COPY_MEMORY(__iwram_start, __load_start_iwram, __load_stop_iwram)

	@ Copy internal work ram overlay 0 (.iwram0 section) from LMA to VMA (ROM to RAM)
	CALL_COPY_MEMORY(__iwram_overlay_start, __load_start_iwram0, __load_stop_iwram0)

	@ Copy external work ram (.ewram section) from LMA to VMA (ROM to RAM)
	CALL_COPY_MEMORY(__ewram_start, __load_start_ewram, __load_stop_ewram)

	@ Copy external work ram overlay 0 (.ewram0 section) from LMA to VMA (ROM to RAM)
	CALL_COPY_MEMORY(__ewram_overlay_start, __load_start_ewram0, __load_stop_ewram0)



@@@@@@@@@@@@@@@@@@@@@
@ Execute User Code @
@@@@@@@@@@@@@@@@@@@@@

#ifdef __arm__

	# switch back to arm
	adr	r0, .Larm
	bx	r0

	.align
	.arm
.Larm:

#endif

	@ the following code can be assembled as either arm or thumb

	@ execute constructors
	bl	_init

	@ execute main program

	mov	r0, #0		@ r0 = int argc = 0;
	mov	r1, #0		@ r1 = char *argv[] = NULL;

	bl	main

	@ execute destructors
	bl	_fini

	@ start over if user code returns
	ldr	r0, =_start
	bx	r0



@@@@@@@@@@@@@@@@
@ Library Code @
@@@@@@@@@@@@@@@@

@ __crt0_zero_memory: clear memory to zero
@ r0 = start address
@ r1 = end address

	.align 2
	.global __crt0_zero_memory
	.thumb_func
	.type	__crt0_zero_memory, function
__crt0_zero_memory:

	cmp	r0, r1
	bcs	.Lclear_return	@ Length is zero so exit

	mov	r2, #0

.Lclear_loop:
	stmia	r0!, {r2}
	cmp	r0, r1
	bcc	.Lclear_loop

.Lclear_return:
	bx	lr



@ __crt0_copy_memory: copy source to destination
@ r0 = destination address
@ r1 = source address start
@ r2 = source address end

	.align 2
	.global	__crt0_copy_memory
	.thumb_func
	.type	__crt0_copy_memory, function
__crt0_copy_memory:

	cmp	r1, r2
	bcs	.Lcopy_return

.Lcopy_loop:
	ldmia	r1!, {r3}
	stmia	r0!, {r3}
	cmp	r1, r2
	bcc	.Lcopy_loop

.Lcopy_return:
	bx	lr



@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Legacy Long Call - DEPRECATED! @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .global __FarFunction
    .thumb_func
__FarFunction:

    @ fall through

    .global __FarProcedure
    .thumb_func
__FarProcedure:
    bx  r0
    nop
    nop



@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Initial Stack Pointers @
@@@@@@@@@@@@@@@@@@@@@@@@@@

        .weak   __sp_usr
	.section .rodata
        .align  2
        .type   __sp_usr, object
        .size   __sp_usr, 4
__sp_usr:
        .word   0x03008000 - 0x100

        .weak	__sp_irq
	.section .rodata
        .align  2
        .type   __sp_irq, object
        .size   __sp_irq, 4
__sp_irq:
        .word   0x03008000 - 0x60



@@@@@@@@@@@@@
@ Pool Data @
@@@@@@@@@@@@@

__crt0_literals:
	.pool


        
