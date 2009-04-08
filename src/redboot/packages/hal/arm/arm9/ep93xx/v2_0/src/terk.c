#include <cyg/hal/terk.h>

#include <pkgconf/system.h>
#include <pkgconf/io_flash.h>

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/hal_cache.h>
#include <cyg/io/edb9301_strataflash.inl>

flash_t flash_protect_read(int index)
{
    volatile flash_t *ROM;
    int d_cache, i_cache;
    flash_t result;
    volatile flash_t *addr = (flash_t *)(CYGNUM_FLASH_BASE + 0x100) + index;
	
    HAL_FLASH_CACHES_OFF(d_cache, i_cache);

    // Get base address and map addresses to virtual addresses
    ROM = (flash_t *)CYGNUM_FLASH_BASE;

    // BootBlock flash does not support full Read_Query - we have do a
    // table oriented thing above, after getting just two bytes of results:
    FLASH_P2V(ROM)[0] = FLASH_Read_ID;
    result = *addr;

    FLASH_P2V(ROM)[0] = FLASH_Reset;

    HAL_FLASH_CACHES_ON(d_cache, i_cache);
    return result;
}

int flash_protect_write(int index, flash_t data)
{
    volatile flash_t *ROM;
    volatile flash_t *BA;
    flash_t stat = 0;
    int timeout = 50000;
    volatile flash_t *addr = (flash_t *)(CYGNUM_FLASH_BASE + 0x100) + index;
    int d_cache, i_cache;
    HAL_FLASH_CACHES_OFF(d_cache, i_cache);

    // Get base address and map addresses to virtual addresses
    ROM = (flash_t *)(CYGNUM_FLASH_BASE_MASK & (unsigned int)addr);
    BA = (flash_t *)addr;

    // Clear any error conditions
    FLASH_P2V(ROM)[0] = FLASH_Clear_Status;


    *FLASH_P2V(BA) = FLASH_ProtectProgram;
    timeout = 5000000;
    *addr = data;

 
    FLASH_P2V(ROM)[0] = FLASH_Read_Status;
    timeout = 5000000;
    while(((stat = FLASH_P2V(ROM)[0]) & FLASH_Status_Ready) !=
        FLASH_Status_Ready) {
        if (--timeout == 0) {
            goto bad;
        }
    }
        // Jump out if there was an error
    if (stat & FLASH_ErrorMask) {
        goto bad;
    }

 bad:
    FLASH_P2V(ROM)[0] = FLASH_Reset;            

    HAL_FLASH_CACHES_ON(d_cache, i_cache);
	
    return stat;
}
// 0 write protect
// 1-4 factory 
// 5-8 user

void terk_init(void)
{
    if (flash_protect_read(8)!=0xc1ab)
	{
		diag_printf("Programming protect bits...\n");
	    flash_protect_write(7, 0x0025); // 32 Mb ram, 8 mb flash, 100K fpga
	    flash_protect_write(8, 0xc1ab);
		// protect
	    flash_protect_write(0, 0xfffd);
	}
}
		

