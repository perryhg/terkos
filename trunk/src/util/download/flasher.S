//*****************************************************************************
//
// FLASHER.S - Code which is loaded into the Cirrus Logic ARM processor via the
//             boot ROM to program the FLASH.
//
// Copyright (c) 2001,2002,2003,2004 Cirrus Logic, Inc.
//
//*****************************************************************************

#include "asmdefs.h"

//*****************************************************************************
//
// The read-only code area.
//
//*****************************************************************************
    _TEXT_

    //
    // This is the entry point of the program.
    //
    _ENTRY_

    //
    // Set up the stack pointer.
    //
    bic     r0, pc, _CONST_ 0x000000ff
    orr     r13, r0, _CONST_ 0x00001000

    //
    // Read the ID code register from the MMU.
    //
    mrc     p15, 0, r1, c0, c0, 0

    //
    // The processor is in ARM mode when we are called, but we want to run in
    // Thumb mode.  So, switch to thumb mode.  Note that this code sequence
    // will not work if any code or data is inserted between the "orr" and the
    // "thumb_entry" label.
    //
    orr     r2, pc, _CONST_ 0x01
    bx      r2

    //
    // Switch the assembler into Thumb mode.
    //
    _CODE16_

    //
    // Now we're in thumb mode.
    //
_THUMB_LABEL_
thumb_entry _LABEL_

    //
    // Load a pointer to the structure containing various information used by
    // the FLASH programmer.
    //
    _ALIGN_ 2
    mov     r7, pc
    b       do_fixups

        //
        // The base address of the FLASH.
        //
#define FLASH_BASE_OFFSET                       0x00000000
        _WORD_  0x00000000

        //
        // The address of the data buffer.
        //
#define DATA_BUFFER_OFFSET                      0x00000004
        _WORD_  0x00000000

        //
        // The flags are zero if the FLASH is 32-bits wide and non-zero if it
        // 16-bits wide (8-bit wide configurations are not supported).
        //
#define FLAGS_OFFSET                            0x00000008
        _WORD_  0x00000000

        //
        // The error flag is non-zero if an error occurred during the FLASH
        // programming sequence.
        //
#define ERROR_OFFSET                            0x0000000c
        _WORD_  0x00000000

        //
        // Various words used to communicate with the FLASH.
        //
#define HEX_00200020_OFFSET                     0x00000010
        _WORD_  0x00200020
#define HEX_00600060_OFFSET                     0x00000014
        _WORD_  0x00600060
#define HEX_00D000D0_OFFSET                     0x00000018
        _WORD_  0x00d000d0
#define HEX_00FF00FF_OFFSET                     0x0000001c
        _WORD_  0x00ff00ff

        //
        // Bit masks used for determining valid block start addresses in the
        // FLASH.
        //
#define HEX_0003FFFF_OFFSET                     0x00000020
        _WORD_  0x0003ffff

        //
        // The offsets of various processor registers.
        //
#define HEX_80000140_OFFSET                     0x00000024
        _WORD_  0x80000140
#define HEX_80000480_OFFSET                     0x00000028
        _WORD_  0x80000480
#define HEX_800004C0_OFFSET                     0x0000002c
        _WORD_  0x800004c0
#define HEX_80840000_OFFSET                     0x00000030
        _WORD_  0x80840000
#define HEX_808A0000_OFFSET                     0x00000034
        _WORD_  0x808a0000
#define HEX_808C0000_OFFSET                     0x00000038
        _WORD_  0x808c0000
#define HEX_8093009C_OFFSET                     0x0000003c
        _WORD_  0x8093009c

        //
        // The table of known FLASH types.
        //
#define FLASH_TYPE_OFFSET                       0x00000040
            //
            // An Intel B3/C3 FLASH.
            //
            _WORD_  IntelB3C3Erase
            _WORD_  IntelB3C3Program
            _WORD_  IntelB3C3Check

            //
            // An Intel J3 FLASH.
            //
            _WORD_  IntelJ3Erase
            _WORD_  IntelJ3Program
            _WORD_  IntelJ3Check

        //
        // The routine to read from FLASH.
        //
#define FLASH_READ_OFFSET                       0x00000058
        _WORD_  FlashRead

        //
        // The routines to access the Atmel 25F1024 EEPROM.
        //
#define ATMEL_ERASE_OFFSET                      0x0000005c
        _WORD_  AtmelF1024Erase
#define ATMEL_PROGRAM_OFFSET                    0x00000060
        _WORD_  AtmelF1024Program
#define ATMEL_CHECK_OFFSET                      0x00000064
        _WORD_  AtmelF1024Check

        //
        // The routine to read from EEPROM.
        //
#define EEPROM_READ_OFFSET                      0x00000068
        _WORD_  EepromRead

        //
        // A table that maps FLASH IDs to sizes and types.
        //
#define INTEL_FLASH_SIZE_OFFSET                 0x0000006c
#define INTEL_FLASH_COUNT                       0x0000000b
            //
            // 4Mb Intel B3 FLASH.
            //
            _SHORT_ 0x8895
            _SHORT_ 0x0008
            _SHORT_ 0x0000

            //
            // 8Mb Intel B3 FLASH.
            //
            _SHORT_ 0x8893
            _SHORT_ 0x0010
            _SHORT_ 0x0000

            //
            // 16Mb Intel B3 FLASH.
            //
            _SHORT_ 0x8891
            _SHORT_ 0x0020
            _SHORT_ 0x0000

            //
            // 32Mb Intel B3 FLASH.
            //
            _SHORT_ 0x8897
            _SHORT_ 0x0040
            _SHORT_ 0x0000

            //
            // 8Mb Intel C3 FLASH.
            //
            _SHORT_ 0x88c1
            _SHORT_ 0x0010
            _SHORT_ 0x0000

            //
            // 16Mb Intel C3 FLASH.
            //
            _SHORT_ 0x88c3
            _SHORT_ 0x0020
            _SHORT_ 0x0000

            //
            // 32Mb Intel C3 FLASH.
            //
            _SHORT_ 0x88c5
            _SHORT_ 0x0040
            _SHORT_ 0x0000

            //
            // 64Mb Intel C3 FLASH.
            //
            _SHORT_ 0x88cd
            _SHORT_ 0x0080
            _SHORT_ 0x0000

#define MICRON_FLASH_SIZE_OFFSET                0x0000009c
#define MICRON_FLASH_COUNT                      0x00000003
            //
            // 32Mb Intel/Micron J3 FLASH.
            //
            _SHORT_ 0x0016
            _SHORT_ 0x0040
            _SHORT_ 0x0001

            //
            // 64Mb Intel/Micron J3 FLASH.
            //
            _SHORT_ 0x0017
            _SHORT_ 0x0080
            _SHORT_ 0x0001

            //
            // 128Mb Intel/Micron J3 FLASH.
            //
            _SHORT_ 0x0018
            _SHORT_ 0x0100
            _SHORT_ 0x0001

        //
        // A table of the baud rate divisors for the EP7xxx processors.
        //
#define EP7xxx_BAUD_OFFSET                      0x000000ae
        _BYTE_  0x17, 0x0b, 0x05, 0x03, 0x01, 0x00

        //
        // A table of the baud rate divisors for the EP93xx processors.
        //
#define EP93xx_BAUD_OFFSET                      0x000000b4
        _BYTE_  0x2f, 0x17, 0x0b, 0x07, 0x03, 0x00

    //
    // Set the address of the data buffer.
    //
_THUMB_LABEL_
do_fixups _LABEL_
    mov     r2, _CONST_ 0x08
    lsl     r2, r2, _CONST_ 8
    add     r2, r2, r0
    str     r2, [r7, _CONST_ DATA_BUFFER_OFFSET]

    //
    // Loop through the entries of the FLASH type table, and the two read
    // routines that follow the table.
    //
    mov     r2, _CONST_ FLASH_TYPE_OFFSET
    mov     r3, _CONST_ 0x0b
_THUMB_LABEL_
fixup_loop _LABEL_
        //
        // Relocate this entry.
        //
        ldr     r4, [r7, r2]
        add     r4, r4, r0
        str     r4, [r7, r2]

        //
        // Advance to the next entry.
        //
        add     r2, r2, _CONST_ 4
        sub     r3, r3, _CONST_ 1
        bne     fixup_loop

    //
    // Get the processor identifier.
    //
    lsr     r1, r1, _CONST_ 0x0c
    mov     r2, _CONST_ 0x0f
    and     r1, r1, r2

    //
    // See if this is an ARM9.
    //
    cmp     r1, _CONST_ 0x09
    beq     setup_arm9

    //
    // Setup for EP7xxx processors.  First, set the base address of the FLASH.
    //
_THUMB_LABEL_
setup_arm7 _LABEL_
    mov     r1, _CONST_ 0x70
    lsl     r1, r1, _CONST_ 24
    str     r1, [r7, _CONST_ FLASH_BASE_OFFSET]

    //
    // Load the value of the system status register.
    //
    ldr     r1, [r7, _CONST_ HEX_80000140_OFFSET]
    ldr     r1, [r1]

    //
    // Get the BOOTBIT field of the system status register.
    //
    mov     r2, _CONST_ 0x18
    lsr     r1, r1, _CONST_ 24
    and     r1, r1, r2

    //
    // Save the BOOTBIT field as the flags.
    //
    str     r1, [r7, _CONST_ FLAGS_OFFSET]

    //
    // Replace the first instruction of SendChar, ReceiveChar, SetBaud, and
    // CheckEeprom with a NOP so that the EP73xx version will be used instead
    // of the EP93xx version.
    //
    mov     r1, _CONST_ 0
    mov     r2, _CONST_ 1
    ldr     r3, =SendChar
    bic     r3, r3, r2
    add     r3, r3, r0
    strh    r1, [r3]
    ldr     r3, =ReceiveChar
    bic     r3, r3, r2
    add     r3, r3, r0
    strh    r1, [r3]
    ldr     r3, =SetBaud
    bic     r3, r3, r2
    add     r3, r3, r0
    strh    r1, [r3]
    ldr     r3, =CheckEeprom
    bic     r3, r3, r2
    add     r3, r3, r0
    strh    r1, [r3]

    //
    // The setup for the EP7xxx processors is done.
    //
    b       loop

    //
    // Setup for EP93xx processors.  First, set the base address of the FLASH.
    //
_THUMB_LABEL_
setup_arm9 _LABEL_
    mov     r1, _CONST_ 0x60
    lsl     r1, r1, _CONST_ 24
    str     r1, [r7, _CONST_ FLASH_BASE_OFFSET]

    //
    // Load the value of the system configuration register.
    //
    ldr     r1, [r7, _CONST_ HEX_8093009C_OFFSET]
    ldr     r1, [r1]

    //
    // Get the LCSn7 field of the system configuration register.
    //
    mov     r2, _CONST_ 0x80
    and     r1, r1, r2

    //
    // Invert the LCSn7 field and save it as the flags.
    //
    eor     r1, r1, r2
    str     r1, [r7, _CONST_ FLAGS_OFFSET]

    //
    // Loop forever reading commands from the host and performing them.
    //
_THUMB_LABEL_
loop _LABEL_
        //
        // Write the command prompt to the host.
        //
        mov     r0, _CONST_ 0x3f // '?'
        bl      SendChar

        //
        // Read a command from the host.
        //
        bl      ReceiveChar

        //
        // Determine how to handle this command.  Is this a 'B'?
        //
        cmp     r0, _CONST_ 0x42 // 'B'
        bne     loop1

            //
            // Change the baud rate of the serial port.
            //
            bl      SetBaud
            b       loop

        //
        // Is this a 'F'?
        //
_THUMB_LABEL_
loop1 _LABEL_
        cmp     r0, _CONST_ 0x46 // 'F'
        bne     loop4

            //
            // Check the FLASH to see if it is something that we recognize and
            // can program.
            //
            bl      CheckFlash

            //
            // See if we recognize the FLASH.
            //
            cmp     r0, _CONST_ 0x01
            beq     loop3

                //
                // Send a 'X' to the host to indicate that we will not be able
                // to program the FLASH.
                //
_THUMB_LABEL_
loop2 _LABEL_
                mov     r0, _CONST_ 0x58 // 'X'
                bl      SendChar
                b       loop

            //
            // Send a 'O' to the host to indicate that we are able to program
            // the FLASH.
            //
_THUMB_LABEL_
loop3 _LABEL_
            mov     r0, _CONST_ 0x4f // 'O'
            bl      SendChar

            //
            // Program data into the FLASH.
            //
            bl      ProgramFlash
            b       loop

        //
        // Is this a 'S'?
        //
_THUMB_LABEL_
loop4 _LABEL_
        cmp     r0, _CONST_ 0x53 // 'S'
        bne     loop

            //
            // Check the EEPROM to see if it is something that we recognize and
            // can program.
            //
            bl      CheckEeprom

            //
            // See if we recognize the EEPROM.
            //
            cmp     r0, _CONST_ 0x01
            bne     loop2

            //
            // Program the EEPROM.
            //
            b       loop3

    //
    // Place any assembler generated data here.
    //
    _LTORG_

//*****************************************************************************
//
// Checks the FLASH to determine if it is a device that we know how to program.
//
//*****************************************************************************
_THUMB_LABEL_
CheckFlash _LABEL_
    //
    // Load a pointer to the FLASH.
    //
    ldr     r0, [r7, _CONST_ FLASH_BASE_OFFSET]

    //
    // See if the FLASH is 16 or 32 bits wide.
    //
    ldr     r1, [r7, _CONST_ FLAGS_OFFSET]
    mov     r2, _CONST_ 0x00
    cmp     r1, r2
    bne     check_flash_16

    //
    // The FLASH is 32-bits wide.  Write the read identifier command to the
    // FLASH.
    //
_THUMB_LABEL_
check_flash_32 _LABEL_
    ldr     r1, =0x00900090
    str     r1, [r0]

    //
    // Load the device IDs from each 16-bit wide FLASH.
    //
    ldrh    r1, [r0, _CONST_ 4]
    ldrh    r2, [r0, _CONST_ 6]

    //
    // See if the IDs are identical.
    //
    cmp     r1, r2
    bne     unknown_device

    //
    // Read back the manufacturer ID from each 16-bit wide FLASH.
    //
    ldr     r2, [r0]

    //
    // See if the IDs are Intel.
    //
    ldr     r3, =0x00890089
    cmp     r2, r3
    beq     is_intel

    //
    // See if the IDs are Micron.
    //
    ldr     r3, =0x002c002c
    cmp     r2, r3
    beq     is_micron

    //
    // See if the IDs are Sharp.
    //
    ldr     r3, =0x00b000b0
    cmp     r2, r3
    beq     is_micron

    //
    // This is a FLASH configuration we can not handle, either an unknown
    // manufacturer, an unknown device ID, or the FLASH devices do not match.
    // Return an error.
    //
_THUMB_LABEL_
unknown_device _LABEL_
    mov     r0, _CONST_ 0x00
    mov     pc, lr

    //
    // The FLASH is 16-bits wide.  Write the read identifier command to the
    // FLASH.
    //
_THUMB_LABEL_
check_flash_16 _LABEL_
    mov     r1, _CONST_ 0x90
    strh    r1, [r0]

    //
    // Read the device ID.
    //
    ldrh    r1, [r0, _CONST_ 2]

    //
    // On the EP73XX evaluation board, the FLASH can be jumpered to be either
    // 32 or 16 bits wide.  If it is jumpered to be 16 bits wide, the LSB of
    // the FLASH (which must be high to read the device ID) is connected to A2
    // of the processor.  In this case, the read at offset 2 will actually read
    // the manufacturer ID.  So, read the manufacturer ID and see if it matches
    // the device ID.
    //
    ldrh    r2, [r0]
    cmp     r1, r2
    bne     id_is_ok

        //
        // The manufacturer ID matches the device ID, so read the device ID
        // from from offset 4 (which will assert the LSB of the FLASH, giving
        // the real device ID).
        //
        ldrh    r1, [r0, _CONST_ 4]

    //
    // Read back the manufacturer ID from the FLASH.
    //
_THUMB_LABEL_
id_is_ok _LABEL_
    ldrh    r2, [r0]

    //
    // See if the ID is Intel.
    //
    cmp     r2, _CONST_ 0x89
    beq     is_intel

    //
    // See if the ID is Sharp.
    //
    cmp     r2, _CONST_ 0xb0
    beq     is_micron

    //
    // See if the ID is Micron.
    //
    cmp     r2, _CONST_ 0x2c
    bne     unknown_device

    //
    // We have a Micron FLASH, so get the device ID table for Micron FLASH.
    //
_THUMB_LABEL_
is_micron _LABEL_
    mov     r3, _CONST_ MICRON_FLASH_COUNT
    mov     r4, _CONST_ MICRON_FLASH_SIZE_OFFSET
    b       find_device

    //
    // We have an Intel FLASH, so get the device ID table for Intel FLASH.
    //
_THUMB_LABEL_
is_intel _LABEL_
    mov     r3, _CONST_ INTEL_FLASH_COUNT
    mov     r4, _CONST_ INTEL_FLASH_SIZE_OFFSET

    //
    // We now have the device ID.  Load a pointer to the FLASH size table.
    //
_THUMB_LABEL_
find_device _LABEL_
    add     r4, r4, r7

        //
        // Get the ID of the current FLASH.
        //
_THUMB_LABEL_
id_check_loop _LABEL_
        ldrh    r2, [r4]

        //
        // See if it matches the ID of the actual FLASH.
        //
        cmp     r1, r2
        beq     found_id

        //
        // Advance to the next FLASH in the table.
        //
        add     r4, r4, _CONST_ 6
        sub     r3, r3, _CONST_ 1
        bne     id_check_loop

    //
    // We do not recognize the device ID, so this is an unknown device.
    //
    b       unknown_device

    //
    // The FLASH was found, so save the FLASH size.
    //
_THUMB_LABEL_
found_id _LABEL_
    ldrh    r3, [r4, _CONST_ 2]
    lsl     r3, r3, _CONST_ 16
    mov     r8, r3

    //
    // Get the FLASH type index and multiply it by 12.
    //
    ldrh    r3, [r4, _CONST_ 4]
    lsl     r4, r3, _CONST_ 1
    add     r3, r3, r4
    lsl     r3, r3, _CONST_ 2

    //
    // Load a pointer to the FLASH type table.
    //
    mov     r4, _CONST_ FLASH_TYPE_OFFSET
    add     r4, r4, r7

    //
    // Advance to the entry for the FLASH that was found.
    //
    add     r4, r4, r3

    //
    // Save the address of the erase routine.
    //
    ldr     r3, [r4]
    mov     r9, r3

    //
    // Save the address of the program routine.
    //
    ldr     r3, [r4, _CONST_ 4]
    mov     r10, r3

    //
    // Save the address of the block address check routine.
    //
    ldr     r3, [r4, _CONST_ 8]
    mov     r11, r3

    //
    // Save the address of the FLASH read routine.
    //
    ldr     r3, [r7, _CONST_ FLASH_READ_OFFSET]
    mov     r12, r3

    //
    // We now have the size of the FLASH and pointers to the routines that will
    // erase and program it.  See if we are in a 32-bit wide configuration, in
    // which case we must double the size of the FLASH (since there are two 16
    // bit wides devices creating the 32-bit wide "device").
    //
    ldr     r0, [r7, _CONST_ FLAGS_OFFSET]
    mov     r1, _CONST_ 0x00
    cmp     r0, r1
    bne     check_done

    //
    // The FLASH is 32-bits wide, so double the size.
    //
    mov     r0, r8
    lsl     r0, r0, _CONST_ 1
    mov     r8, r0

    //
    // Increment the address of the erase, program and check routines by 2, so
    // that we will use the 32-bit wide versions instead of the 16-bit wide
    // versions.
    //
    mov     r0, r9
    add     r0, r0, _CONST_ 2
    mov     r9, r0
    mov     r0, r10
    add     r0, r0, _CONST_ 2
    mov     r10, r0
    mov     r0, r11
    add     r0, r0, _CONST_ 2
    mov     r11, r0

    //
    // We've completed our checking successfully.  Return success.
    //
_THUMB_LABEL_
check_done _LABEL_
    mov     r0, _CONST_ 0x01
    mov     pc, lr

    //
    // Place any assembler generated data here.
    //
    _LTORG_

//*****************************************************************************
//
// Waits for an internal erase or program operation to complete on a 16-bit
// wide Intel FLASH (B3, C3, or J3).
//
//*****************************************************************************
_THUMB_LABEL_
IntelWait16 _LABEL_
    //
    // Load the bit mask for which we are waiting.
    //
    mov     r1, _CONST_ 0x80

    //
    // Read from the FLASH memory until bit 7 is one.
    //
_THUMB_LABEL_
intel_wait_16_loop _LABEL_
        ldrh    r2, [r0]
        and     r2, r2, r1
        cmp     r2, r1
        bne     intel_wait_16_loop

    //
    // Clear the status register in the FLASH.
    //
    mov     r1, _CONST_ 0x50
    strh    r1, [r0]

    //
    // Return to the caller.
    //
    mov     pc, lr

//*****************************************************************************
//
// Waits for an internal erase or program operation to complete on a 32-bit
// wide Intel FLASH (B3, C3, or J3).
//
//*****************************************************************************
_THUMB_LABEL_
IntelWait32 _LABEL_
    //
    // Load the bit mask for which we are waiting.
    //
    ldr     r1, [r7, _CONST_ HEX_00200020_OFFSET]
    lsl     r1, r1, _CONST_ 2

    //
    // Read from the FLASH memory until bit 7 is one.
    //
_THUMB_LABEL_
intel_wait_32_loop _LABEL_
        ldr     r2, [r0]
        and     r2, r2, r1
        cmp     r2, r1
        bne     intel_wait_32_loop

    //
    // Clear the status register in the FLASH.
    //
    ldr     r1, =0x00500050
    str     r1, [r0]

    //
    // Return to the caller.
    //
    mov     pc, lr

    //
    // Place any assembler generated data here.
    //
    _LTORG_

//*****************************************************************************
//
// The routine to check the starting address for an Intel B3/C3 FLASH.  This
// must be a branch to the routine for a single 16-bit wide FLASH, immediately
// followed by the routine for a 32-bit wide FLASH (i.e. two 16-bit wide
// FLASHes).
//
//*****************************************************************************
_THUMB_LABEL_
IntelB3C3Check _LABEL_
    b       intel_b3_c3_check_16

//*****************************************************************************
//
// The routine for checking the starting address for a pair of 16-bit wide
// Intel B3/C3 FLASHes in a 32-bit configuration.
//
//*****************************************************************************
_THUMB_LABEL_
intel_b3_c3_check_32 _LABEL_
    //
    // See if the lower 17 bits are all zero.  If so, the starting address is
    // on a 128K boundary, which is the start of a block.
    //
    ldr     r1, [r7, _CONST_ HEX_0003FFFF_OFFSET]
    lsr     r1, r1, _CONST_ 1
_THUMB_LABEL_
intel_b3_c3_check_common _LABEL_
    tst     r0, r1
    beq     check_ok

    //
    // See if the lower 13 bits are all zero.  If not, the starting address is
    // not on a block start.
    //
    lsr     r2, r1, _CONST_ 3
    tst     r0, r2
    bne     check_bad

    //
    // See if the upper 15 bits are all zero.  If so, the starting address is
    // on a 16K boundary within the first 128K, which is the start of a block.
    //
    mvn     r1, r1
    tst     r0, r1
    bne     check_bad

    //
    // The starting address is at the start of a block.
    //
_THUMB_LABEL_
check_ok _LABEL_
    mov     r0, _CONST_ 0x00
    mov     pc, lr

    //
    // The starting address is not at the start of a block.
    //
_THUMB_LABEL_
check_bad _LABEL_
    mov     r0, _CONST_ 0x01
    mov     pc, lr

//*****************************************************************************
//
// The routine for checking the starting address for a single 16-bit wide
// Intel B3/C3 FLASH.
//
//*****************************************************************************
_THUMB_LABEL_
intel_b3_c3_check_16 _LABEL_
    //
    // See if the lower 16 bits are all zero.  If so, the starting address is
    // on a 64K boundary, which is the start of a block.
    //
    ldr     r1, [r7, _CONST_ HEX_0003FFFF_OFFSET]
    lsr     r1, r1, _CONST_ 2
    b       intel_b3_c3_check_common

//*****************************************************************************
//
// The routine to check the starting address of an Intel J3 FLASH.  This must
// be a branch to the routine for a single 16-bit wide FLASH, immediately
// followed by the routine for a 32-bit wide FLASH (i.e. two 16-bit wide
// FLASHes).
//
//*****************************************************************************
_THUMB_LABEL_
IntelJ3Check _LABEL_
    b       intel_j3_check_16

//*****************************************************************************
//
// The routine for checking the starting address for a pair of 16-bit wide
// Intel J3 FLASHes in a 32-bit configuration.
//
//*****************************************************************************
_THUMB_LABEL_
intel_j3_check_32 _LABEL_
    //
    // See if the lower 18 bits are all zero.  If so, the starting address is
    // on a 256K boundary, which is the start of a block.
    //
    ldr     r1, [r7, _CONST_ HEX_0003FFFF_OFFSET]
_THUMB_LABEL_
intel_j3_check_common _LABEL_
    tst     r0, r1
    beq     check_ok
    b       check_bad

//*****************************************************************************
//
// The routine for checking the starting address for a single 16-bit wide
// Intel J3 FLASH.
//
//*****************************************************************************
_THUMB_LABEL_
intel_j3_check_16 _LABEL_
    //
    // See if the lower 17 bits are all zero.  If so, the starting address is
    // on a 128K boundary, which is the start of a block.
    //
    ldr     r1, [r7, _CONST_ HEX_0003FFFF_OFFSET]
    lsr     r1, r1, _CONST_ 1
    b       intel_j3_check_common

//*****************************************************************************
//
// The routine for checking the start address for an Atmel 25F1024 EEPROM.
//
//*****************************************************************************
_THUMB_LABEL_
AtmelF1024Check _LABEL_
    //
    // See if the lower 14 bits are all zero.  If so, the starting address is
    // on a 32K boundary, which is the start of a block.
    //
    ldr     r1, [r7, _CONST_ HEX_0003FFFF_OFFSET]
    lsr     r1, r1, _CONST_ 3
    b       intel_j3_check_common

//*****************************************************************************
//
// The routine to program a Intel B3/C3 FLASH.  This must be a branch to the
// routine for a single 16-bit wide FLASH, immediately followed by the routine
// for a 32-bit wide FLASH (i.e. two 16-bit wide FLASHes).
//
//*****************************************************************************
_THUMB_LABEL_
IntelB3C3Program _LABEL_
    b       intel_b3_c3_program_16

//*****************************************************************************
//
// The routine for programming a pair of 16-bit wide Intel B3/C3 FLASHes in a
// 32-bit configuration.
//
//*****************************************************************************
_THUMB_LABEL_
intel_b3_c3_program_32 _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {r4-r6, lr}

    //
    // Save the offset, length, and pointer to the data to be programmed into
    // the FLASH.
    //
    mov     r4, r0
    mov     r5, r1
    mov     r6, r2

    //
    // Load a pointer to the FLASH.
    //
    ldr     r0, [r7, _CONST_ FLASH_BASE_OFFSET]

    //
    // Program the FLASH a word at a time.
    //
_THUMB_LABEL_
intel_b3_c3_program_32_word _LABEL_
        //
        // See if we have programmed all the required words.
        //
        cmp     r6, _CONST_ 0
        beq     intel_b3_c3_program_32_done

        //
        // Load the next word to be programmed.
        //
        ldr     r1, [r4]
        add     r4, r4, _CONST_ 4

        //
        // Program the next word.
        //
        ldr     r2, [r7, _CONST_ HEX_00200020_OFFSET]
        str     r2, [r0, r5]
        str     r1, [r0, r5]

        //
        // Wait until the word has been programmed.
        //
        bl      IntelWait32

        //
        // Increment the address to be programmed.
        //
        add     r5, r5, _CONST_ 4

        //
        // Decrement the count of words to be programmed.
        //
        sub     r6, r6, _CONST_ 4

        //
        // Loop back.
        //
        b       intel_b3_c3_program_32_word

    //
    // We're done programming the FLASH, so put the FLASH into read array mode.
    //
_THUMB_LABEL_
intel_b3_c3_program_32_done _LABEL_
    ldr     r1, [r7, _CONST_ HEX_00FF00FF_OFFSET]
    str     r1, [r0]

    //
    // Return to the caller.
    //
    pop     {r4-r6, pc}

//*****************************************************************************
//
// The routine for programming a 16-bit wide Intel B3/C3 FLASH.
//
//*****************************************************************************
_THUMB_LABEL_
intel_b3_c3_program_16 _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {r4-r6, lr}

    //
    // Save the offset, length, and pointer to the data to be programmed into
    // the FLASH.
    //
    mov     r4, r0
    mov     r5, r1
    mov     r6, r2

    //
    // Load a pointer to the FLASH.
    //
    ldr     r0, [r7, _CONST_ FLASH_BASE_OFFSET]

    //
    // Program the FLASH a word at a time.
    //
_THUMB_LABEL_
intel_b3_c3_program_16_word _LABEL_
        //
        // See if we have programmed all the required words.
        //
        cmp     r6, _CONST_ 0
        beq     intel_b3_c3_program_16_done

        //
        // Load the next word to be programmed.
        //
        ldrh    r1, [r4]
        add     r4, r4, _CONST_ 2

        //
        // Program the next word.
        //
        mov     r2, _CONST_ 0x40
        strh    r2, [r0, r5]
        strh    r1, [r0, r5]

        //
        // Wait until the word has been programmed.
        //
        bl      IntelWait16

        //
        // Increment the address to be programmed.
        //
        add     r5, r5, _CONST_ 2

        //
        // Decrement the count of words to be programmed.
        //
        sub     r6, r6, _CONST_ 2

        //
        // Loop back.
        //
        b       intel_b3_c3_program_16_word

    //
    // We're done programming the FLASH, so put the FLASH into read array mode.
    //
_THUMB_LABEL_
intel_b3_c3_program_16_done _LABEL_
    mov     r1, _CONST_ 0xff
    str     r1, [r0]

    //
    // Return to the caller.
    //
    pop     {r4-r6, pc}

//*****************************************************************************
//
// The routine to erase a Intel B3/C3 FLASH.  This must be a branch to the
// routine for a single 16-bit wide FLASH, immediately followed by the routine
// for a 32-bit wide FLASH (i.e. two 16-bit wide FLASHes).
//
//*****************************************************************************
_THUMB_LABEL_
IntelB3C3Erase _LABEL_
    b       intel_b3_c3_erase_16

//*****************************************************************************
//
// The routine for erasing a pair of 16-bit wide Intel B3/C3 FLASHes in a
// 32-bit configuration.
//
//*****************************************************************************
_THUMB_LABEL_
intel_b3_c3_erase_32 _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {r4, lr}

    //
    // Save the length of the FLASH to be erased.
    //
    mov     r4, r0

    //
    // Save the offset of the FLASH to be erased.
    //
    mov     r3, r1

    //
    // Load a pointer to the FLASH.
    //
    ldr     r0, [r7, _CONST_ FLASH_BASE_OFFSET]

    //
    // Start erasing the FLASH.
    //
_THUMB_LABEL_
intel_b3_c3_erase_32_block _LABEL_
        //
        // See if we have erased all the required blocks.
        //
        cmp     r3, r4
        bge     intel_b3_c3_erase_32_done

        //
        // Unlock this block.
        //
        ldr     r1, [r7, _CONST_ HEX_00600060_OFFSET]
        str     r1, [r0, r3]
        ldr     r2, [r7, _CONST_ HEX_00D000D0_OFFSET]
        str     r2, [r0, r3]

        //
        // Erase this block.
        //
        ldr     r1, [r7, _CONST_ HEX_00200020_OFFSET]
        str     r1, [r0, r3]
        str     r2, [r0, r3]

        //
        // Wait until the erase has completed.
        //
        bl      IntelWait32

        //
        // Increment the block address.  The first 8 blocks of the FLASH have a
        // block size of 16K and the remaining blocks have a size of 128K.  So,
        // if the offset is less than 128K, increment it by 16K, otherwise
        // increment it by 128K.
        //
        mov     r2, _CONST_ 0x02
        lsl     r2, r2, _CONST_ 16
        cmp     r3, r2
        blt     intel_b3_c3_erase_32_small
        mov     r2, _CONST_ 0x20
        b       intel_b3_c3_erase_32_increment
_THUMB_LABEL_
intel_b3_c3_erase_32_small _LABEL_
        mov     r2, _CONST_ 0x04
_THUMB_LABEL_
intel_b3_c3_erase_32_increment _LABEL_
        lsl     r2, r2, _CONST_ 12
        add     r3, r3, r2

        //
        // Loop back.
        //
        b       intel_b3_c3_erase_32_block

    //
    // We're done erasing the FLASH, so return to the caller.
    //
_THUMB_LABEL_
intel_b3_c3_erase_32_done _LABEL_
    pop     {r4, pc}

//*****************************************************************************
//
// The routine for erasing a single 16-bit wide Intel B3/C3 FLASH.
//
//*****************************************************************************
_THUMB_LABEL_
intel_b3_c3_erase_16 _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {r4, lr}

    //
    // Save the length of the FLASH to be erased.
    //
    mov     r4, r0

    //
    // Save the offset of the FLASH to be erased.
    //
    mov     r3, r1

    //
    // Load a pointer to the FLASH.
    //
    ldr     r0, [r7, _CONST_ FLASH_BASE_OFFSET]

    //
    // Start erasing the FLASH.
    //
_THUMB_LABEL_
intel_b3_c3_erase_16_block _LABEL_
        //
        // See if we have erased all the required blocks.
        //
        cmp     r3, r4
        bge     intel_b3_c3_erase_16_done

        //
        // Unlock this block.
        //
        mov     r1, _CONST_ 0x60
        strh    r1, [r0, r3]
        mov     r2, _CONST_ 0xd0
        strh    r2, [r0, r3]

        //
        // Erase this block.
        //
        mov     r1, _CONST_ 0x20
        strh    r1, [r0, r3]
        strh    r2, [r0, r3]

        //
        // Wait until the erase has completed.
        //
        bl      IntelWait16

        //
        // Increment the block address.  The first 8 blocks of the FLASH have a
        // block size of 8K and the remaining blocks have a size of 64K.  So,
        // if the offset is less than 64K, increment it by 8K, otherwise
        // increment it by 64K.
        //
        mov     r2, _CONST_ 0x01
        lsl     r2, r2, _CONST_ 16
        cmp     r3, r2
        blt     intel_b3_c3_erase_16_small
        mov     r2, _CONST_ 0x10
        b       intel_b3_c3_erase_16_increment
_THUMB_LABEL_
intel_b3_c3_erase_16_small _LABEL_
        mov     r2, _CONST_ 0x02
_THUMB_LABEL_
intel_b3_c3_erase_16_increment _LABEL_
        lsl     r2, r2, _CONST_ 12
        add     r3, r3, r2

        //
        // Loop back.
        //
        b       intel_b3_c3_erase_16_block

    //
    // We're done erasing the FLASH, so return to the caller.
    //
_THUMB_LABEL_
intel_b3_c3_erase_16_done _LABEL_
    pop     {r4, pc}

//*****************************************************************************
//
// The routine to erase a Intel J3 FLASH.  This must be a branch to the routine
// for a single 16-bit wide FLASH, immediately followed by the routine for a
// 32-bit wide FLASH (i.e. two 16-bit wide FLASHes).
//
//*****************************************************************************
_THUMB_LABEL_
IntelJ3Erase _LABEL_
    b       intel_j3_erase_16

//*****************************************************************************
//
// The routine for erasing a pair of 16-bit wide Intel J3 FLASHes in a 32-bit
// configuration.
//
//*****************************************************************************
_THUMB_LABEL_
intel_j3_erase_32 _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {r4, lr}

    //
    // Save the length of the FLASH to be erased.
    //
    mov     r4, r0

    //
    // Save the offset of the FLASH to be erased.
    //
    mov     r3, r1

    //
    // Load a pointer to the FLASH.
    //
    ldr     r0, [r7, _CONST_ FLASH_BASE_OFFSET]

    //
    // Start erasing the FLASH.
    //
_THUMB_LABEL_
intel_j3_erase_32_block _LABEL_
        //
        // See if we have erased all the required blocks.
        //
        cmp     r3, r4
        bge     intel_j3_erase_32_done

        //
        // Unlock this block.
        //
        ldr     r1, [r7, _CONST_ HEX_00600060_OFFSET]
        str     r1, [r0, r3]
        ldr     r1, [r7, _CONST_ HEX_00D000D0_OFFSET]
        str     r1, [r0, r3]

        //
        // Wait until the unlock has completed.
        //
        bl      IntelWait32

        //
        // Erase this block.
        //
        ldr     r1, [r7, _CONST_ HEX_00200020_OFFSET]
        str     r1, [r0, r3]
        ldr     r1, [r7, _CONST_ HEX_00D000D0_OFFSET]
        str     r1, [r0, r3]

        //
        // Wait until the erase has completed.
        //
        bl      IntelWait32

        //
        // Increment the block address.  Each block is 256K.
        //
        mov     r2, _CONST_ 0x04
        lsl     r2, r2, _CONST_ 16
        add     r3, r3, r2

        //
        // Loop back.
        //
        b       intel_j3_erase_32_block

    //
    // We're done erasing the FLASH, so return to the caller.
    //
_THUMB_LABEL_
intel_j3_erase_32_done _LABEL_
    pop     {r4, pc}

//*****************************************************************************
//
// The routine for erasing a 16-bit wide Intel J3 FLASH.
//
//*****************************************************************************
_THUMB_LABEL_
intel_j3_erase_16 _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {r4, lr}

    //
    // Save the length of the FLASH to be erased.
    //
    mov     r4, r0

    //
    // Save the offset of the FLASH to be erased.
    //
    mov     r3, r1

    //
    // Load a pointer to the FLASH.
    //
    ldr     r0, [r7, _CONST_ FLASH_BASE_OFFSET]

    //
    // Start erasing the FLASH.
    //
_THUMB_LABEL_
intel_j3_erase_16_block _LABEL_
        //
        // See if we have erased all the required blocks.
        //
        cmp     r3, r4
        bge     intel_j3_erase_16_done

        //
        // Unlock this block.
        //
        mov     r1, _CONST_ 0x60
        strh    r1, [r0, r3]
        mov     r1, _CONST_ 0xd0
        strh    r1, [r0, r3]

        //
        // Wait until the unlock has completed.
        //
        bl      IntelWait16

        //
        // Erase this block.
        //
        mov     r1, _CONST_ 0x20
        strh    r1, [r0, r3]
        mov     r1, _CONST_ 0xd0
        strh    r1, [r0, r3]

        //
        // Wait until the erase has completed.
        //
        bl      IntelWait16

        //
        // Increment the block address.  Each block is 128K.
        //
        mov     r2, _CONST_ 0x02
        lsl     r2, r2, _CONST_ 16
        add     r3, r3, r2

        //
        // Loop back.
        //
        b       intel_j3_erase_16_block

    //
    // We're done erasing the FLASH, so return to the caller.
    //
_THUMB_LABEL_
intel_j3_erase_16_done _LABEL_
    pop     {r4, pc}

//*****************************************************************************
//
// The routine to program a Intel J3 FLASH.  This must be a branch to the
// routine for a single 16-bit wide FLASH, immediately followed by the routine
// for a 32-bit wide FLASH (i.e. two 16-bit wide FLASHes).
//
//*****************************************************************************
_THUMB_LABEL_
IntelJ3Program _LABEL_
    //b       intel_j3_program_16
    b       intel_b3_c3_program_16

//*****************************************************************************
//
// The routine for programming a pair of 16-bit wide Intel J3 FLASHes.
//
//*****************************************************************************
_THUMB_LABEL_
intel_j3_program_32 _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {r4-r6, lr}

    //
    // Save the offset, length, and pointer to the data to be programmed into
    // the FLASH.
    //
    mov     r4, r0
    mov     r5, r1
    mov     r6, r2

    //
    // Load a pointer to the FLASH.
    //
    ldr     r0, [r7, _CONST_ FLASH_BASE_OFFSET]

    //
    // Program the FLASH sixteen words at a time.
    //
_THUMB_LABEL_
intel_j3_program_32_16words _LABEL_
        //
        // See if we have programmed all the required words.
        //
        cmp     r6, _CONST_ 0
        beq     intel_j3_program_32_done

        //
        // Send the write to buffer command.
        //
        ldr     r1, =0x00e800e8
        str     r1, [r0, r5]
        ldr     r1, =0x000f000f
        str     r1, [r0, r5]

        //
        // Write the next 16 words to the FLASH.
        //
        mov     r2, _CONST_ 0x10
        mov     r3, r5
_THUMB_LABEL_
intel_j3_program_32_word _LABEL_
            //
            // Load the next word to be programmed.
            //
            ldr     r1, [r4]
            add     r4, r4, _CONST_ 4

            //
            // Write it to the FLASH.
            //
            str     r1, [r0, r3]
            add     r3, r3, _CONST_ 4

            //
            // Decrement the count of words to write.
            //
            sub     r2, r2, _CONST_ 1
            bne     intel_j3_program_32_word

        //
        // Write the program confirm command.
        //
        ldr     r1, [r7, _CONST_ HEX_00D000D0_OFFSET]
        str     r1, [r0, r5]

        //
        // Wait until the words have been programmed.
        //
        bl      IntelWait32

        //
        // Increment the address to be programmed.
        //
        add     r5, r5, _CONST_ 64

        //
        // Decrement the count of words to be programmed.
        //
        sub     r6, r6, _CONST_ 64

        //
        // Loop back.
        //
        b       intel_j3_program_32_16words

    //
    // We're done programming the FLASH, so put the FLASH into read array mode.
    //
_THUMB_LABEL_
intel_j3_program_32_done _LABEL_
    ldr     r1, [r7, _CONST_ HEX_00FF00FF_OFFSET]
    str     r1, [r0]

    //
    // Return to the caller.
    //
    pop     {r4-r6, pc}

    //
    // Place any assembler generated data here.
    //
    _LTORG_

//*****************************************************************************
//
// The routine for programming a 16-bit wide Intel J3 FLASH.
//
//*****************************************************************************
#if 0
_THUMB_LABEL_
intel_j3_program_16 _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {r4-r6, lr}

    //
    // Save the offset, length, and pointer to the data to be programmed into
    // the FLASH.
    //
    mov     r4, r0
    mov     r5, r1
    mov     r6, r2

    //
    // Load a pointer to the FLASH.
    //
    ldr     r0, [r7, _CONST_ FLASH_BASE_OFFSET]

    //
    // Program the FLASH a sixteen words at a time.
    //
_THUMB_LABEL_
intel_j3_program_16_16words _LABEL_
        //
        // See if we have programmed all the required words.
        //
        cmp     r6, _CONST_ 0
        beq     intel_j3_program_16_done

        //
        // Send the write to buffer command.
        //
        mov     r1, _CONST_ 0xe8
        strh    r1, [r0, r5]
        mov     r1, _CONST_ 0x0f
        strh    r1, [r0, r5]

        //
        // Write the next 16 words to the FLASH.
        //
        mov     r2, _CONST_ 0x10
        mov     r3, r5
_THUMB_LABEL_
intel_j3_program_16_word _LABEL_
            //
            // Load the next word to be programmed.
            //
            ldrh    r1, [r4]
            add     r4, r4, _CONST_ 2

            //
            // Write it to the FLASH.
            //
            strh    r1, [r0, r3]
            add     r3, r3, _CONST_ 2

            //
            // Decrement the count of words to write.
            //
            sub     r2, r2, _CONST_ 1
            bne     intel_j3_program_16_word

        //
        // Write the program confirm command.
        //
        mov     r1, _CONST_ 0xd0
        strh    r1, [r0, r5]

        //
        // Wait until the words have been programmed.
        //
        bl      IntelWait16

        //
        // Increment the address to be programmed.
        //
        add     r5, r5, _CONST_ 32

        //
        // Decrement the count of words to be programmed.
        //
        sub     r6, r6, _CONST_ 32

        //
        // Loop back.
        //
        b       intel_j3_program_16_16words

    //
    // We're done programming the FLASH, so put the FLASH into read array mode.
    //
_THUMB_LABEL_
intel_j3_program_16_done _LABEL_
    mov     r1, _CONST_ 0xff
    strh    r1, [r0]

    //
    // Return to the caller.
    //
    pop     {r4-r6, pc}
#endif

//*****************************************************************************
//
// The routine for reading a word of data from the FLASH.
//
//*****************************************************************************
_THUMB_LABEL_
FlashRead _LABEL_
    //
    // Load the base address of the FLASH.
    //
    ldr     r1, [r7, _CONST_ FLASH_BASE_OFFSET]

    //
    // Read the requested word.
    //
    ldr     r0, [r1, r0]

    //
    // Return to the caller.
    //
    mov     pc, lr

//*****************************************************************************
//
// Wait until the SPI has finished transmitting all data in its FIFO.
//
//*****************************************************************************
_THUMB_LABEL_
SPITransmitWait _LABEL_
    //
    // Get the transmit FIFO empty flag.
    //
    mov     r1, _CONST_ 0x01

        //
        // Load the SPI status register.
        //
_THUMB_LABEL_
spi_wait1 _LABEL_
        ldr     r2, [r0, _CONST_ 0x0c]

        //
        // See if the transmit FIFO empty bit is set and loop back if not.
        //
        tst     r2, r1
        beq     spi_wait1

    //
    // Get the busy flag.
    //
    mov     r1, _CONST_ 0x10

        //
        // Load the SPI status register.
        //
_THUMB_LABEL_
spi_wait2 _LABEL_
        ldr     r2, [r0, _CONST_ 0x0c]

        //
        // See if the busy bit is set and loop back if so.
        //
        tst     r2, r1
        bne     spi_wait2

    //
    // Delay for 200us for good measure.
    //
_THUMB_LABEL_
spi_delay _LABEL_
    mov     r1, _CONST_ 0x40

        //
        // Loop until the count is expired.
        //
_THUMB_LABEL_
spi_delay1 _LABEL_
        sub     r1, r1, _CONST_ 1
        bne     spi_delay1

    //
    // Return to the caller.
    //
    mov     pc, lr

//*****************************************************************************
//
// Clear any data from the receive FIFO.
//
//*****************************************************************************
_THUMB_LABEL_
SPIReceiveClear _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {lr}

    //
    // Wait until the transmit FIFO is emtpy.
    //
    bl      SPITransmitWait

    //
    // Get the SPI receive FIFO empty flag.
    //
    mov     r1, _CONST_ 0x04

        //
        // Read the SPI status register.
        //
_THUMB_LABEL_
receive_clear1 _LABEL_
        ldr     r2, [r0, _CONST_ 0x0c]

        //
        // Stop looping if there is no more data in the receive FIFO.
        //
        tst     r2, r1
        beq     receive_clear2

        //
        // Read a byte from the receive FIFO and loop back.
        //
        ldr     r2, [r0, _CONST_ 0x08]
        b       receive_clear1

    //
    // Return to the caller.
    //
_THUMB_LABEL_
receive_clear2 _LABEL_
    pop     {pc}

//*****************************************************************************
//
// Reads a byte of data from the SPI port.
//
//*****************************************************************************
_THUMB_LABEL_
SPIReadByte _LABEL_
    //
    // Get the flag to wait for (receive FIFO empty).
    //
    mov     r1, _CONST_ 0x04

        //
        // Load the SPI status register.
        //
_THUMB_LABEL_
spi_read1 _LABEL_
        ldr     r2, [r0, _CONST_ 0x0c]

        //
        // See if the bit is clear and loop back if so.
        //
        tst     r2, r1
        beq     spi_read1

    //
    // Read the byte from the SPI.
    //
    ldr     r1, [r0, _CONST_ 0x08]

    //
    // Return to the caller.
    //
    mov     pc, lr

//*****************************************************************************
//
// Check the EEPROM to determine if it is a device that we know how to program.
//
//*****************************************************************************
_THUMB_LABEL_
CheckEeprom _LABEL_
    //
    // Branch to the ARM9 version of CheckEeprom.  If running on an ARM7, this
    // instruction will be replaced with a NOP.
    //
    b       CheckEeprom9

    //
    // The EEPROM is not supported on the EP73xx, so return a failure.
    //
    b       eeprom_bad

    //
    // Save the link register to the stack.
    //
_THUMB_LABEL_
CheckEeprom9 _LABEL_
    push    {lr}

    //
    // Load a pointer to the GPIO registers.
    //
    ldr     r0, [r7, _CONST_ HEX_80840000_OFFSET]

    //
    // Set GPIO pins 12 and 14 as outputs.
    //
    mov     r1, _CONST_ 0x50
    str     r1, [r0, _CONST_ 0x14]

    //
    // Set GPIO pins 12 and 14 high to disable the keyboard.
    //
    str     r1, [r0, _CONST_ 0x04]

    //
    // Set GPIO pin 6 and 7 as an outputs.
    //
    mov     r1, _CONST_ 0xc0
    str     r1, [r0, _CONST_ 0x10]

    //
    // Clear GPIO pin 7 to enable the frame line.
    //
    mov     r1, _CONST_ 0x00
    str     r1, [r0, _CONST_ 0x00]
    
    //
    // Set GPIO pin 6 to disable the CS4271.
    //
    mov     r1, _CONST_ 0x40
    str     r1, [r0, _CONST_ 0x00]

    //
    // Set FGPIO pin 0 as an output.
    //
    mov     r1, _CONST_ 0x01
    str     r1, [r0, _CONST_ 0x34]

    //
    // Set FGPIO pin 0 to disable the TLV2542.
    //
    mov     r1, _CONST_ 0x01
    str     r1, [r0, _CONST_ 0x30]
    
    //
    // Load a pointer to the SPI registers.
    //
    ldr     r0, [r7, _CONST_ HEX_808A0000_OFFSET]

    //
    // Program the CR0 register.
    //
    ldr     r1, =0x000001c7
    str     r1, [r0, _CONST_ 0x00]

    //
    // Program the predivisor register.
    //
    mov     r1, _CONST_ 2
    str     r1, [r0, _CONST_ 0x10]

    //
    // Program the CR1 register.
    //
    mov     r1, _CONST_ 0
    str     r1, [r0, _CONST_ 0x04]

    //
    // Enable the SPI interface.
    //
    mov     r1, _CONST_ 0x10
    str     r1, [r0, _CONST_ 0x04]

    //
    // Issue the read ID command.
    //
    mov     r1, _CONST_ 0x15
    str     r1, [r0, _CONST_ 0x08]
    mov     r1, _CONST_ 0x00
    str     r1, [r0, _CONST_ 0x08]
    str     r1, [r0, _CONST_ 0x08]

    //
    // Wait until the transmit FIFO is empty.
    //
    bl      SPITransmitWait

    //
    // Read and throw away a byte.
    //
    bl      SPIReadByte

    //
    // Read the manufacturer code.
    //
    bl      SPIReadByte
    mov     r4, r1

    //
    // Read the device code.
    //
    bl      SPIReadByte

    //
    // See if the manufacturer code is Atmel.
    //
    cmp     r4, _CONST_ 0x1f
    bne     eeprom_bad

    //
    // See if the device code is the 28F1024.
    //
    cmp     r1, _CONST_ 0x60
    bne     eeprom_bad

    //
    // An Atmel 25F1024 was found, so load the pointers to the routines to
    // handle it.
    //
    mov     r0, _CONST_ 0x02
    lsl     r0, r0, _CONST_ 16
    mov     r8, r0
    ldr     r0, [r7, _CONST_ ATMEL_ERASE_OFFSET]
    mov     r9, r0
    ldr     r0, [r7, _CONST_ ATMEL_PROGRAM_OFFSET]
    mov     r10, r0
    ldr     r0, [r7, _CONST_ ATMEL_CHECK_OFFSET]
    mov     r11, r0
    ldr     r0, [r7, _CONST_ EEPROM_READ_OFFSET]
    mov     r12, r0

    //
    // An EEPROM was found that we know how to handle,
    //
    mov     r0, _CONST_ 0x01
    pop     {pc}

    //
    // An unrecognized (or no) EEPROM was found, so return a failure.
    //
_THUMB_LABEL_
eeprom_bad _LABEL_
    mov     r0, _CONST_ 0x00
    pop     {pc}

//*****************************************************************************
//
// Wait until the EEPROM is not busy.
//
//*****************************************************************************
_THUMB_LABEL_
AtmelF1024Wait _LABEL_
    //
    // Save the link register.
    //
    push    {lr}

        //
        // Send the read status register command to the EEPROM.
        //
_THUMB_LABEL_
atmel_wait1 _LABEL_
        mov     r1, _CONST_ 0x05
        str     r1, [r0, _CONST_ 0x08]
        mov     r1, _CONST_ 0x00
        str     r1, [r0, _CONST_ 0x08]

        //
        // Wait until the transmit FIFO is empty.
        //
        bl      SPITransmitWait

        //
        // Read the status register.
        //
        bl      SPIReadByte
        bl      SPIReadByte

        //
        // See if the not ready bit is set and loop back if so.
        //
        mov     r2, _CONST_ 0x01
        tst     r1, r2
        bne     atmel_wait1

    //
    // Return to the caller.
    //
    pop     {pc}

//*****************************************************************************
//
// The routine to erase an Atmel 25F1024 EEPROM.
//
//*****************************************************************************
_THUMB_LABEL_
AtmelF1024Erase _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {r4-r5, lr}

    //
    // Save the length of the EEPROM to be erased.
    //
    mov     r5, r0

    //
    // Save the offset of the EEPROM to be erased.
    //
    mov     r4, r1

    //
    // Load a pointer to the SPI registers.
    //
    ldr     r0, [r7, _CONST_ HEX_808A0000_OFFSET]

_THUMB_LABEL_
atmel_erase1 _LABEL_
        cmp     r4, r5
        bge     atmel_erase_done

        //
        // Send the write enable command to the EEPROM.
        //
        mov     r1, _CONST_ 0x06
        str     r1, [r0, _CONST_ 0x08]

        //
        // Clear out the receive FIFO.
        //
        bl      SPIReceiveClear

        //
        // Send the sector erase command to the EEPROM.
        //
        mov     r1, _CONST_ 0x52
        str     r1, [r0, _CONST_ 0x08]
        lsr     r1, r4, _CONST_ 16
        str     r1, [r0, _CONST_ 0x08]
        lsr     r1, r4, _CONST_ 8
        str     r1, [r0, _CONST_ 0x08]
        str     r4, [r0, _CONST_ 0x08]

        //
        // Clear out the receive FIFO.
        //
        bl      SPIReceiveClear

        //
        // Wait until the sector has been erased.
        //
        bl      AtmelF1024Wait

        //
        // Advance to the next sector.
        //
        mov     r1, _CONST_ 0x40
        lsl     r1, r1, _CONST_ 8
        add     r4, r4, r1

        //
        // Loop back.
        //
        b       atmel_erase1

    //
    // Return to the caller.
    //
_THUMB_LABEL_
atmel_erase_done _LABEL_
    pop     {r4-r5, pc}

//*****************************************************************************
//
// The routine to program an Atmel 25F1024 EEPROM.
//
//*****************************************************************************
_THUMB_LABEL_
AtmelF1024Program _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {r4-r6, lr}

    //
    // Save the offset, length and pointer to the data to be programmed into
    // the EEPROM.
    //
    mov     r4, r0
    mov     r5, r1
    mov     r6, r2

    //
    // Load a pointer to the SPI registers.
    //
    ldr     r0, [r7, _CONST_ HEX_808A0000_OFFSET]

        //
        // Send the write enable command to the EEPROM.
        //
_THUMB_LABEL_
atmel_program1 _LABEL_
        mov     r1, _CONST_ 0x06
        str     r1, [r0, _CONST_ 0x08]

        //
        // Clear out the receive FIFO.
        //
        bl      SPIReceiveClear

        //
        // Load the word to be programmed.
        //
        ldr     r3, [r4]
        add     r4, r4, _CONST_ 4

        //
        // Tell the EEPROM to program this word.
        //
        mov     r1, _CONST_ 0x02
        str     r1, [r0, _CONST_ 0x08]
        lsr     r1, r5, _CONST_ 16
        str     r1, [r0, _CONST_ 0x08]
        lsr     r1, r5, _CONST_ 8
        str     r1, [r0, _CONST_ 0x08]
        str     r5, [r0, _CONST_ 0x08]
        str     r3, [r0, _CONST_ 0x08]
        lsr     r3, r3, _CONST_ 8
        str     r3, [r0, _CONST_ 0x08]
        lsr     r3, r3, _CONST_ 8
        str     r3, [r0, _CONST_ 0x08]
        lsr     r3, r3, _CONST_ 8
        str     r3, [r0, _CONST_ 0x08]

        //
        // Clear out the receive FIFO.
        //
        bl      SPIReceiveClear

        //
        // Wait until the data has been programmed.
        //
        bl      AtmelF1024Wait

        //
        // Advance to the next word.
        //
        add     r5, r5, _CONST_ 4
        sub     r6, r6, _CONST_ 4
        bne     atmel_program1

    //
    // Return to the caller.
    //
    pop     {r4-r6, pc}

//*****************************************************************************
//
// The routine for reading a word of data from the EEPROM.
//
//*****************************************************************************
_THUMB_LABEL_
EepromRead _LABEL_
    //
    // Save the link register.
    //
    push    {r2-r4, lr}

    //
    // Save the offset to be read.
    //
    mov     r4, r0

    //
    // Load a pointer to the SPI registers.
    //
    ldr     r0, [r7, _CONST_ HEX_808A0000_OFFSET]

    //
    // Send the SPI read command.
    //
    mov     r1, _CONST_ 0x03
    str     r1, [r0, _CONST_ 0x08]
    lsr     r1, r4, _CONST_ 16
    str     r1, [r0, _CONST_ 0x08]
    lsr     r1, r4, _CONST_ 8
    str     r1, [r0, _CONST_ 0x08]
    str     r4, [r0, _CONST_ 0x08]

    //
    // Read a byte from the SPI to make sure that the FIFO isn't overrun.
    //
    bl      SPIReadByte

    //
    // The SPI can not read unless it is writing, so write out four zero bytes
    // so that four bytes can be read from the EEPROM.
    //
    mov     r1, _CONST_ 0x00
    str     r1, [r0, _CONST_ 0x08]
    str     r1, [r0, _CONST_ 0x08]
    str     r1, [r0, _CONST_ 0x08]
    str     r1, [r0, _CONST_ 0x08]

    //
    // Read and throw away the bytes corresponding to the sent address.
    //
    bl      SPIReadByte
    bl      SPIReadByte
    bl      SPIReadByte

    //
    // Read the data word from the EEPROM.
    //
    bl      SPIReadByte
    mov     r4, r1
    bl      SPIReadByte
    lsl     r1, r1, _CONST_ 8
    orr     r4, r4, r1
    bl      SPIReadByte
    lsl     r1, r1, _CONST_ 16
    orr     r4, r4, r1
    bl      SPIReadByte
    lsl     r1, r1, _CONST_ 24
    orr     r4, r4, r1

    //
    // Clear the receive FIFO.
    //
    bl      SPIReceiveClear

    //
    // Return to the caller.
    //
    mov     r0, r4
    pop     {r2-r4, pc}

//*****************************************************************************
//
// SendChar sends a character to UART1.
//
//*****************************************************************************
_THUMB_LABEL_
SendChar _LABEL_
    //
    // Branch to the ARM9 version of SendChar.  If running on an ARM7, this
    // instruction will be replaced with a NOP.
    //
    b       SendChar9

    //
    // Load a pointer to the system status register.
    //
    ldr     r1, [r7, _CONST_ HEX_80000140_OFFSET]

    //
    // Wait until there is space in the transmit FIFO for UART1.
    //
    mov     r3, _CONST_ 0x80
    lsl     r3, r3, _CONST_ 16
_THUMB_LABEL_
sendchar0 _LABEL_
        ldr     r2, [r1]
        tst     r2, r3
        bne     sendchar0

    //
    // Write the character to UART1.
    //
    ldr     r1, [r7, _CONST_ HEX_80000480_OFFSET]
    str     r0, [r1]

    //
    // Return to the caller.
    //
    mov     pc, lr

    //
    // SendChar for the EP93xx.  Load the base address of UART1.
    //
_THUMB_LABEL_
SendChar9 _LABEL_
    ldr     r1, [r7, _CONST_ HEX_808C0000_OFFSET]

    //
    // Wait until the FIFO is empty.
    //
    mov     r3, _CONST_ 0x80
_THUMB_LABEL_
sendchar1 _LABEL_
        ldr     r2, [r1, _CONST_ 0x18]
        tst     r2, r3
        beq     sendchar1

    //
    // Write the character to UART1.
    //
    str     r0, [r1]

    //
    // Return to the caller.
    //
    mov     pc, lr

//*****************************************************************************
//
// ReceiveChar receives a character from UART1.
//
//*****************************************************************************
_THUMB_LABEL_
ReceiveChar _LABEL_
    //
    // Branch to the ARM9 version of ReceiveChar.  If running on an ARM7, this
    // instruction will be replaced with a NOP.
    //
    b       ReceiveChar9

    //
    // Load a pointer to the system status register.
    //
    ldr     r1, [r7, _CONST_ HEX_80000140_OFFSET]

    //
    // Wait until there is a character in the receive FIFO for UART1.
    //
    mov     r2, _CONST_ 0x40
    lsl     r2, r2, _CONST_ 16
_THUMB_LABEL_
receivechar0 _LABEL_
        ldr     r0, [r1]
        tst     r0, r2
        bne     receivechar0

    //
    // Read the character from UART1.
    //
    ldr     r1, [r7, _CONST_ HEX_80000480_OFFSET]
    ldr     r0, [r1]
_THUMB_LABEL_
receivechar1 _LABEL_
    mov     r1, _CONST_ 0xff
    and     r0, r0, r1

    //
    // Return to the caller.
    //
    mov     pc, lr

    //
    // ReceiveChar for the EP93xx.  Load the base address of UART1.
    //
_THUMB_LABEL_
ReceiveChar9 _LABEL_
    ldr     r1, [r7, _CONST_ HEX_808C0000_OFFSET]

    //
    // Wait until there is a character in the receive FIFO for UART1.
    //
    mov     r2, _CONST_ 0x10
_THUMB_LABEL_
receivechar2 _LABEL_
        ldr     r0, [r1, _CONST_ 0x18]
        tst     r0, r2
        bne     receivechar2

    //
    // Read the character from UART1.
    //
    ldr     r0, [r1]
    b       receivechar1

//*****************************************************************************
//
// SetBaud sets the baud rate to the specified rate.
//
//*****************************************************************************
_THUMB_LABEL_
SetBaud _LABEL_
    //
    // Branch to the ARM9 version of SetBaud.  If running on an ARM7, this
    // instruction will be replaced with a NOP.
    //
    b       SetBaud9

    //
    // Save the link register to the stack.
    //
    push    {lr}

    //
    // Read the baud rate indicator from the host.
    //
    bl      ReceiveChar

    //
    // Get the baud rate divisor based on the requested baud rate.
    //
    add     r0, r0, _CONST_ (EP7xxx_BAUD_OFFSET - 0x30)
    ldrb    r1, [r7, r0]

    //
    // Set the data length to 8 bits per character and enable the FIFO.
    //
    mov     r0, _CONST_ 0x07
    lsl     r0, r0, _CONST_ 16
    orr     r1, r1, r0

    //
    // Change the configuration of UART1.
    //
    ldr     r0, [r7, _CONST_ HEX_800004C0_OFFSET]
    str     r1, [r0]

    //
    // Wait until we receive a '-' character from the host.
    //
_THUMB_LABEL_
setbaud1 _LABEL_
        bl      ReceiveChar
        cmp     r0, _CONST_ 0x2d
        bne     setbaud1

    //
    // Return to the caller.
    //
    pop     {pc}

    //
    // SetBaud for the EP93xx.  Save the link register to the stack.
    //
_THUMB_LABEL_
SetBaud9 _LABEL_
    push    {lr}

    //
    // Read the baud rate indicator from the host.
    //
    bl      ReceiveChar

    //
    // Get the baud rate divisor based on the requested baud rate.
    //
    add     r0, r0, _CONST_ (EP93xx_BAUD_OFFSET - 0x30)
    ldrb    r1, [r7, r0]

    //
    // Load the base address of UART1.
    //
    ldr     r0, [r7, _CONST_ HEX_808C0000_OFFSET]

    //
    // Set the baud rate divider.
    //
    str     r1, [r0, _CONST_ 0x10]

    //
    // Clear the upper baud rate divider register.
    //
    mov     r1, _CONST_ 0x00
    str     r1, [r0, _CONST_ 0x0c]

    //
    // Set the data format to 8-N-1, causing the new baud rate divider to take
    // effect.
    //
    mov     r1, _CONST_ 0x60
    str     r1, [r0, _CONST_ 0x08]

    //
    // Wait until we receive a '-' character from the host.
    //
    b       setbaud1

//*****************************************************************************
//
// ReadLong reads a 32-bit value from the serial port.
//
//*****************************************************************************
_THUMB_LABEL_
ReadLong _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {r4, lr}

    //
    // Read the first byte.
    //
    bl      ReceiveChar

    //
    // The first byte is the least significant 8 bits of the long value.
    //
    mov     r4, r0

    //
    // Read the second byte.
    //
    bl      ReceiveChar

    //
    // The second byte is the next 8 significant bits of the long value.
    //
    lsl     r0, r0, _CONST_ 8
    orr     r4, r4, r0

    //
    // Read the third byte.
    //
    bl      ReceiveChar

    //
    // The third byte is the next 8 significant bits of the long value.
    //
    lsl     r0, r0, _CONST_ 16
    orr     r4, r4, r0

    //
    // Read the fourth byte.
    //
    bl      ReceiveChar

    //
    // The fourth byte is the most significant 8 bits of the long value.
    //
    lsl     r0, r0, _CONST_ 24
    orr     r0, r0, r4

    //
    // Return to the caller.
    //
    pop     {r4, pc}

//*****************************************************************************
//
// ReadBlock reads a 1K block of data from the host.
//
//*****************************************************************************
_THUMB_LABEL_
ReadBlock _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {r4-r6, lr}

    //
    // Load a pointer to the data buffer.
    //
    ldr     r4, [r7, _CONST_ DATA_BUFFER_OFFSET]

    //
    // Read 1K bytes of data.
    //
    mov     r5, _CONST_ 0x04
    lsl     r5, r5, _CONST_ 8
_THUMB_LABEL_
read_byte _LABEL_
        //
        // Read the next byte of data.
        //
        bl      ReceiveChar

        //
        // Add this byte to the local word.
        //
        lsr     r6, r6, _CONST_ 8
        lsl     r0, r0, _CONST_ 24
        orr     r6, r6, r0

        //
        // See if there is a full word to be stored.
        //
        mov     r0, _CONST_ 3
        and     r0, r0, r5
        cmp     r0, _CONST_ 1
        bne     next_byte

            //
            // Store this word in the data buffer.
            //
            str     r6, [r4]
            add     r4, r4, _CONST_ 4

        //
        // Decrement the count of bytes.
        //
_THUMB_LABEL_
next_byte _LABEL_
        sub     r5, r5, _CONST_ 1
        bne     read_byte

    //
    // Return to the caller.
    //
    pop     {r4-r6, pc}

//*****************************************************************************
//
// ProgramFlash programs the data receive via the serial port into the on-board
// FLASH.
//
//*****************************************************************************
_THUMB_LABEL_
ProgramFlash _LABEL_
    //
    // Save the link register to the stack.
    //
    push    {lr}

    //
    // Read the starting offset of the data to be programmed.
    //
    bl      ReadLong
    mov     r5, r0

    //
    // Read the length of the data to be programmed.
    //
    bl      ReadLong
    mov     r4, r0

    //
    // See if the starting offset is on a block boundary.
    //
    mov     r0, r5
    mov     lr, pc
    mov     pc, r11
    cmp     r0, _CONST_ 0x00
    beq     offset_ok

        //
        // Tell the host that the offset is not valid.
        //
        mov     r0, _CONST_ 0x25 // '%'
        b       program_exit

    //
    // See if the data is small enough to fit into the FLASH.
    //
_THUMB_LABEL_
offset_ok _LABEL_
    mov     r0, r4
    add     r0, r0, r5
    mov     r1, r8
    cmp     r0, r1
    blt     size_ok

        //
        // Tell the host that the data is too large.
        //
        mov     r0, _CONST_ 0x26 // '&'
        b       program_exit

    //
    // Erase the FLASH.
    //
_THUMB_LABEL_
size_ok _LABEL_
    mov     r1, r5
    mov     r0, r4
    add     r0, r0, r1
    mov     lr, pc
    mov     pc, r9

    //
    // Tell the host that we're ready to start receiving file data.
    //
    mov     r0, _CONST_ 0x21 // '!'
    bl      SendChar

    //
    // Now, read data in 1K blocks until we've read all the file data.
    //
    mov     r0, _CONST_ 0x00
    str     r0, [r7, _CONST_ ERROR_OFFSET]
_THUMB_LABEL_
read_block _LABEL_
        //
        // Read 1K bytes from the serial port.
        //
        bl      ReadBlock

        //
        // Read the checksum from the host.
        //
        bl      ReadLong

        //
        // Checksum the data we read.
        //
        mov     r1, _CONST_ 0x04
        lsl     r1, r1, _CONST_ 8
        ldr     r2, [r7, _CONST_ DATA_BUFFER_OFFSET]
_THUMB_LABEL_
checksum _LABEL_
            //
            // Read this byte of the data buffer.
            //
            ldrb    r3, [r2]
            add     r2, r2, _CONST_ 1

            //
            // Subtract this byte from the checksum from the host.
            //
            sub     r0, r0, r3

            //
            // Decrement the count of bytes.
            //
            sub     r1, r1, _CONST_ 1
            bne     checksum

        //
        // See if the checksum matched (i.e. the counter is now zero).
        //
        cmp     r0, _CONST_ 0x00
        beq     checksum_ok

            //
            // The checksum was bad, so ask the host to send this block again.
            //
            mov     r0, _CONST_ 0x40 // '@'
            bl      SendChar
            b       read_block

        //
        // The checksum matched, so program this block of data into the FLASH.
        // First, get some values which we'll need.
        //
_THUMB_LABEL_
checksum_ok _LABEL_
        ldr     r0, [r7, _CONST_ DATA_BUFFER_OFFSET]
        mov     r1, r5
        mov     r2, _CONST_ 0x04
        lsl     r2, r2, _CONST_ 8
        mov     lr, pc
        mov     pc, r10

        //
        // Verify that the data was programmed correctly.
        //
        mov     r2, _CONST_ 0x00
        mov     r3, _CONST_ 0x04
        lsl     r3, r3, _CONST_ 8
        ldr     r6, [r7, _CONST_ DATA_BUFFER_OFFSET]
_THUMB_LABEL_
verify_word _LABEL_
            //
            // Load this word from FLASH.
            //
            add     r0, r2, r5
            mov     lr, pc
            mov     pc, r12

            //
            // Load this word from the internal buffer.
            //
            ldr     r1, [r6, r2]

            //
            // See if the two words are identical.
            //
            cmp     r0, r1
            beq     word_ok

                //
                // Indicate that there was an error.
                //
                str     r3, [r7, _CONST_ ERROR_OFFSET]

            //
            // Decrement the word count.
            //
_THUMB_LABEL_
word_ok _LABEL_
            add     r2, r2, _CONST_ 4
            sub     r3, r3, _CONST_ 4
            bne     verify_word

        //
        // Tell the host that we're done with this block and are ready for the
        // next one.
        //
        mov     r0, _CONST_ 0x23 // '#'
        bl      SendChar

        //
        // Increment the block address.
        //
        mov     r0, _CONST_ 0x04
        lsl     r0, r0, _CONST_ 8
        add     r5, r5, r0

        //
        // Decrement the count of bytes to be programmed.
        //
        sub     r4, r4, r0

        //
        // Loop back to the next block.
        //
        bne     read_block

    //
    // See if there was an error programming the FLASH.
    //
    ldr     r0, [r7, _CONST_ ERROR_OFFSET]
    cmp     r0, _CONST_ 0x00
    beq     program_done

        //
        // Tell the host that there was an error programming the FLASH.
        //
        mov     r0, _CONST_ 0x5e // '^'
        bl      SendChar

    //
    // Tell the host that we're done programming the FLASH.
    //
_THUMB_LABEL_
program_done _LABEL_
    mov     r0, _CONST_ 0x2a // '*'
_THUMB_LABEL_
program_exit _LABEL_
    bl      SendChar

    //
    // Return to the caller.
    //
    pop     {pc}

    _END_
