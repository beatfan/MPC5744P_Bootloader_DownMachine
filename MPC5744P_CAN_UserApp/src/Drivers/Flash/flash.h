#ifndef _DRIVER_FLASH_FLASH_H_
#define _DRIVER_FLASH_FLASH_H_


#include "flash_c55_driver.h"

//get addressrage select block
flash_block_select_t Flash_GetSelectBlock(uint32_t startAddr, uint32_t endAddr);

status_t Flash_Unlock_g(uint32_t startAddr, uint32_t endAddr);

status_t Flash_Erase_g(uint32_t startAddr, uint32_t endAddr);

status_t Flash_Check_g(uint32_t startAddr, uint32_t endAddr);

status_t Flash_Program_g(uint32_t startAddr, uint32_t endAddr, uint32_t *buffer, uint32_t len);

status_t Flash_Lock_g(uint32_t startAddr, uint32_t endAddr);

uint32_t Flash_Read_g(uint32_t addr);

#endif /* End of (__BLOCK_MPC57XX__) */
