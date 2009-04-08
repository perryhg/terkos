#define FLASH_P2V( _a_ ) ((volatile flash_t *)((unsigned int)(_a_)))
#define FLASHWORD( k ) (k)
typedef unsigned short flash_t;
#define FLASH_Read_ID      		FLASHWORD( 0x90 )
#define FLASH_Read_Status  		FLASHWORD( 0x70 )
#define FLASH_Clear_Status 		FLASHWORD( 0x50 )
#define FLASH_Status_Ready 		FLASHWORD( 0x80 )

#define FLASH_ProtectProgram	FLASHWORD( 0xC0 )
#define FLASH_Reset        		FLASHWORD( 0xFF )
                                                     
// Status that we read back:                         
#define FLASH_ErrorMask			FLASHWORD( 0x7E )
#define FLASH_ErrorProgram		FLASHWORD( 0x10 )
#define FLASH_ErrorErase		FLASHWORD( 0x20 )

flash_t flash_protect_read(int index);
int flash_protect_write(int index, flash_t data);
void terk_init(void);


