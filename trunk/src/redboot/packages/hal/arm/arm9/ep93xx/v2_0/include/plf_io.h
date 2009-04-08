#ifndef CYGONCE_HAL_PLF_IO_H
#define CYGONCE_HAL_PLF_IO_H

/*=============================================================================
//
//      plf_io.h
//
//      Platform specific support (register layout, etc)
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2003 Gary Thomas <gary@mind.be>
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    jskov
// Contributors: jskov, gthomas
// Date:         2002-01-28
// Purpose:      Platform specific support routines
// Description:
// Usage:        #include <cyg/hal/hal_io.h>
//
//####DESCRIPTIONEND####
//
//===========================================================================*/
#include <pkgconf/hal.h>
#include <pkgconf/system.h>
#include CYGBLD_HAL_PLATFORM_H

#define CYGARC_PHYSICAL_ADDRESS(x) hal_virt_to_phys_address(x)

extern unsigned long ConfigInfo[];
extern unsigned long SDRAMAddressMap[];

#define CYGHWR_REDBOOT_LINUX_ATAG_MEM(_p_)                                    \
    CYG_MACRO_START                                                           \
        int i;                                                                \
                                                                              \
        for(i = 0; i < ConfigInfo[2]; i++)                                    \
        {                                                                     \
            _p_->hdr.size = (sizeof(struct tag_mem32) +                       \
                             sizeof(struct tag_header)) / sizeof(long);       \
            _p_->hdr.tag = ATAG_MEM;                                          \
                                                                              \
            _p_->u.mem.size = ConfigInfo[0];                                  \
            _p_->u.mem.start = ConfigInfo[3] + SDRAMAddressMap[i];            \
                                                                              \
            if(i != (ConfigInfo[2] - 1))                                      \
            {                                                                 \
                _p_ = (struct tag *)((long *)_p_ + _p_->hdr.size);            \
            }                                                                 \
        }                                                                     \
    CYG_MACRO_END

#endif // CYGONCE_HAL_PLF_IO_H
// EOF plf_io.h
