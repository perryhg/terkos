//==========================================================================
//
//      ep93xx_misc.c
//
//      HAL misc board support code for ARM9/EP93XX
//
//==========================================================================
//####COPYRIGHTBEGIN####
//
// -------------------------------------------
// The contents of this file are subject to the Red Hat eCos Public License
// Version 1.1 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://www.redhat.com/
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See the
// License for the specific language governing rights and limitations under
// the License.
//
// The Original Code is eCos - Embedded Configurable Operating System,
// released September 30, 1998.
//
// The Initial Developer of the Original Code is Red Hat.
// Portions created by Red Hat are
// Copyright (C) 1998, 1999, 2000, 2001 Red Hat, Inc.
// All Rights Reserved.
// -------------------------------------------
//
//####COPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    gthomas
// Contributors: hmt, Travis C. Furrer <furrer@mit.edu>, jskov
// Date:         2001-04-23
// Purpose:      HAL board support
// Description:  Implementations of HAL board interfaces
//
//####DESCRIPTIONEND####
//
//========================================================================*/
#include <pkgconf/hal.h>
#include <pkgconf/system.h>
#include CYGBLD_HAL_PLATFORM_H

#include <cyg/infra/cyg_type.h>         // base types
#include <cyg/infra/cyg_trac.h>         // tracing macros
#include <cyg/infra/cyg_ass.h>          // assertion macros

#include <cyg/hal/hal_io.h>             // IO macros
#include <cyg/hal/hal_arch.h>           // Register state info
#include <cyg/hal/hal_diag.h>
#include <cyg/hal/hal_intr.h>           // Interrupt names
#include <cyg/hal/hal_cache.h>
#include <cyg/hal/ep93xx.h>             // Platform specifics
#include <cyg/hal/regs_VIC.h>

#include <cyg/infra/diag.h>             // diag_printf

unsigned long ConfigInfo[5];
unsigned long SDRAMAddressMap[8];
unsigned long SDRAMSize;

#define CONFIG_SDRAM_BANK_SIZE          0
#define CONFIG_SDRAM_ADDRESS_MASK       1
#define CONFIG_SDRAM_BANK_COUNT         2
#define CONFIG_SDRAM_BASE               3
#define CONFIG_FLASH_BASE               4

//=============================================================================
// hal_virt_to_phys_address
//=============================================================================
// Returns a physical address given a virtual address.
cyg_uint32 hal_virt_to_phys_address(cyg_uint32 vaddr)
{
    int i;

    /*
     * If the virtual address is in SDRAM, then find its physical address.
     */
    if(((vaddr >= 0x00000000) && (vaddr < 0x10000000)) ||
       ((vaddr >= 0xc0000000) && (vaddr < 0xd0000000)))
    {
        vaddr &= 0x0fffffff;

        for(i = 0; i < ConfigInfo[CONFIG_SDRAM_BANK_COUNT]; i++)
        {
            if(vaddr < ConfigInfo[CONFIG_SDRAM_BANK_SIZE])
            {
                break;
            }

            vaddr -= ConfigInfo[CONFIG_SDRAM_BANK_SIZE];
        }

        if(i == ConfigInfo[CONFIG_SDRAM_BANK_COUNT])
        {
            vaddr = 0xBAD0ADD8;
        }
        else
        {
            vaddr = ConfigInfo[CONFIG_SDRAM_BASE] + SDRAMAddressMap[i] + vaddr;
        }
    }

    /*
     * If the virtual is in FLASH, then find its physical address.
     */
    else if(((vaddr >= 0x60000000) && (vaddr < 0x70000000)) |
            ((vaddr >= 0xe0000000) && (vaddr < 0xf0000000)))
    {
        vaddr &= 0x0fffffff;

        vaddr |= ConfigInfo[CONFIG_FLASH_BASE];
    }

    return(vaddr);
}

cyg_uint32 hal_virt_to_uncached_address(cyg_uint32 vaddr)
{
    if(vaddr < 0x10000000)
    {
        vaddr += 0xc0000000;
    }
    else
    {
        vaddr = 0xBAD0ADD8u; /* nothing there; uncached unavailable */
    }

    return (vaddr);
}

cyg_uint32 hal_phys_to_virt_address(cyg_uint32 paddr)
{
    int i;

    if((paddr >= ConfigInfo[CONFIG_SDRAM_BASE]) &&
       (paddr <= (ConfigInfo[CONFIG_SDRAM_BASE] + 0x0fffffff)))
    {
        paddr &= 0x0fffffff;

        for(i = 0; i < ConfigInfo[CONFIG_SDRAM_BANK_COUNT]; i++)
        {
            if((paddr >= SDRAMAddressMap[i]) &&
               (paddr <
                (SDRAMAddressMap[i] + ConfigInfo[CONFIG_SDRAM_BANK_SIZE])))
            {
                break;
            }
        }

        if(i != ConfigInfo[CONFIG_SDRAM_BANK_COUNT])
        {
            paddr = ((ConfigInfo[CONFIG_SDRAM_BANK_SIZE] * i) +
                     (paddr & (ConfigInfo[CONFIG_SDRAM_BANK_SIZE] - 1)));
        }
        else
        {
            paddr = 0xBAD0ADD8;
        }
    }
    else if((paddr >= ConfigInfo[CONFIG_FLASH_BASE]) &&
            (paddr <= (ConfigInfo[CONFIG_FLASH_BASE] + 0x0fffffff)))
    {
        paddr &= 0x0fffffff;
        paddr |= 0x60000000;
    }

    return (paddr);
}

//=============================================================================
//=============================================================================
// Write a locked syscon Register.
static __inline__ void
sysconWriteReg(int offset, unsigned long val)
{
    HAL_WRITE_UINT32(EP9312_SYSCON+EP9312_SYSCON_LOCK, 0xAA);
    HAL_WRITE_UINT32(EP9312_SYSCON+offset, val);
}

//
// Platform specific initialization
//
void
plf_hardware_init(void)
{
    //
    // Get a copy of the memory configuration information from the startup
    // code.
    //
    ConfigInfo[CONFIG_SDRAM_BANK_SIZE] = *(unsigned long *)0x2000;
    ConfigInfo[CONFIG_SDRAM_ADDRESS_MASK] = *(unsigned long *)0x2004;
    ConfigInfo[CONFIG_SDRAM_BANK_COUNT] = *(unsigned long *)0x2008;
    ConfigInfo[CONFIG_SDRAM_BASE] = *(unsigned long *)0x200c;
    ConfigInfo[CONFIG_FLASH_BASE] = *(unsigned long *)0x2010;

    //
    // Determine the total SDRAM available.
    //
    SDRAMSize = (ConfigInfo[CONFIG_SDRAM_BANK_SIZE] *
                 ConfigInfo[CONFIG_SDRAM_BANK_COUNT]);

    //
    // Get a copy of the SDRAM address map from the startup code.
    //
    SDRAMAddressMap[0] = *(unsigned long *)0x2100;
    SDRAMAddressMap[1] = *(unsigned long *)0x2104;
    SDRAMAddressMap[2] = *(unsigned long *)0x2108;
    SDRAMAddressMap[3] = *(unsigned long *)0x210c;
    SDRAMAddressMap[4] = *(unsigned long *)0x2110;
    SDRAMAddressMap[5] = *(unsigned long *)0x2114;
    SDRAMAddressMap[6] = *(unsigned long *)0x2118;
    SDRAMAddressMap[7] = *(unsigned long *)0x211c;

    //
    // Set this bit like the Kernel expects it(runs serial off of the 14Mhz).
    //
    HAL_WRITE_UINT32(EP9312_PWRCNT, EP9312_PWRCNT_UARTBAUD);
}

//-----------------------------------------------------------------------------
// This will get defined to HAL_CLOCK_INITIALIZE in hal_intr.h. This routine
// should only be called by test code.
//-----------------------------------------------------------------------------
void hal_clock_initialize(cyg_uint32 period)
{
    while(1);
}

//-----------------------------------------------------------------------------
// This will get defined to HAL_CLOCK_RESET.  This routine should only be
// called by test code.
//-----------------------------------------------------------------------------
externC void hal_clock_reset(cyg_uint32 vector, cyg_uint32 period)
{
    while(1);
}

// Read the current value of the clock, returning the number of hardware
// "ticks" that have occurred (i.e. how far away the current value is from
// the start)

// Note: The "contract" for this function is that the value is the number
// of hardware clocks that have happened since the last interrupt (i.e.
// when it was reset).  This value is used to measure interrupt latencies.
// However, since the hardware counter does not automatically reset, as
// many do, the value may be larger than the system "tick" interval.
// If this turns out to be the case, the result will be adjusted to be
// within the interval [0..N-1], where N is the number of hardware clocks/tick.

void hal_clock_read(cyg_uint32 *pvalue)
{
    while(1);
}

//
// Delay for some number of micro-seconds
//
void hal_delay_us(cyg_int32 usecs)
{
    cyg_uint32 val, val2, hi, hi2;
    int i, safety;

    //
    // Reset debug timer
    //
    HAL_WRITE_UINT32(EP9312_TIMERS_DEBUG_HI, EP9312_TIMERS_DEBUG_HI_RESET);

    // TODO is this required?
    // Required for timer to properly reset??
    //
    for (i = 0;  i < 100;  i++) ;

    HAL_WRITE_UINT32(EP9312_TIMERS_DEBUG_HI, EP9312_TIMERS_DEBUG_HI_START);

    //
    // Wait for timer
    //
    do {
        HAL_READ_UINT32(EP9312_TIMERS_DEBUG_LO, val);
        HAL_READ_UINT32(EP9312_TIMERS_DEBUG_HI, hi);
        safety = 1000;  // Surely this is more than 1us
        while (safety-- > 0) {
            HAL_READ_UINT32(EP9312_TIMERS_DEBUG_LO, val2);
            HAL_READ_UINT32(EP9312_TIMERS_DEBUG_HI, hi2);
            if (val != val2) {
                // Timer is going, continue
                break;
            }
        }
        if (safety == 0) {
            // Timer must be stuck, give up
            return;
        }
    } while (val < usecs);
}

//-----------------------------------------------------------------------------
// int hal_IRQ_handler(void)
//-----------------------------------------------------------------------------
// This routine is called to respond to a hardware interrupt (IRQ).  It
// should interrogate the hardware and return the IRQ vector number.
int hal_IRQ_handler(void)
{
    return(0);
}

//-----------------------------------------------------------------------------
// void hal_interrupt_mask(int InterruptNumber)
//-----------------------------------------------------------------------------
// Disable the interrupt indicated by InterruptNumber
//
void hal_interrupt_mask(int InterruptNumber)
{
    if (InterruptNumber<32)
    {
        //
        // Mask the interrupt
        //
        IntControl[0]->VICIntEnClear.Value = (0x1 << InterruptNumber);
    }
    else
    {
        //
        // Mask the interrupt
        //
        IntControl[1]->VICIntEnClear.Value = (0x1 << (InterruptNumber - 32));
    }
}

//-----------------------------------------------------------------------------
// void hal_interrupt_unmask(int InterruptNumber)
//-----------------------------------------------------------------------------
// Enable the interrupt indicated by InterruptNumber
//
void hal_interrupt_unmask(int InterruptNumber)
{
    if (InterruptNumber<32)
    {
        //
        // Mask the interrupt
        //
        IntControl[0]->VICIntEnable.Value = (0x1 << InterruptNumber);
    }
    else
    {
        //
        // Mask the interrupt
        //
        IntControl[1]->VICIntEnable.Value = (0x1 << (InterruptNumber - 32));
    }
}

//-----------------------------------------------------------------------------
// void hal_interrupt_acknowledge(int vector)
//-----------------------------------------------------------------------------
// There is no acknowledge at the global level in the ARM VIC.
//
void hal_interrupt_acknowledge(int vector)
{
    return;
}

//-----------------------------------------------------------------------------
// void hal_interrupt_acknowledge(int vector)
//-----------------------------------------------------------------------------
// All interrupts are level triggered, so this function cannot be implemented.
//
void hal_interrupt_configure(int vector, int level, int up)
{
    return;
}

//-----------------------------------------------------------------------------
// void hal_interrupt_acknowledge(int vector)
//-----------------------------------------------------------------------------
// This function is unimplemented because unless vectored interrupts are used,
// priority is fixed.
//
void hal_interrupt_set_level(int vector, int level)
{
    return;
}

// ------------------------------------------------------------------------
// EOF ep93xx_misc.c
