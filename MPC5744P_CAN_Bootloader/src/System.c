
#include "derivative.h"      /* derivative-specific definitions */

#include "System.h"
#include "compiler_api.h"

#include "flexcan_init.h"


#define __SRAM_BASE_ADDR		0x40000000
#define __SRAM_SIZE				0x00060000
#define __LOCAL_DMEM_BASE_ADDR  0x50800000
#define __LOCAL_DMEM_SIZE		0x00010000

#define RAM_RANGE_CHECK(addr)     ((addr>(__SRAM_BASE_ADDR-1))&&(addr<(__SRAM_BASE_ADDR + __SRAM_SIZE)) ? 1 :0)

//uint32_t returnCode;         /* Return code from each SSD function. */
//uint32_t  demoResult;
//
//uint32_t dest;               /* destination address */
//uint32_t size;               /* size applicable */
//uint32_t source;             /* source address for program and verify */
//
//uint32_t failedAddress;     /* save the failed address in flash */
//uint32_t failedData;        /* save the failed data in flash */
//uint32_t failedSource;      /* Save the failed source for verification */
//
//uint32_t sum;               /* check sum result */
//
//uint32_t temp;

void EER_exception_handler()
{

}

/************************************************************************
** CAN received data process, feed back busy and flag new data received
** input: none
** output: none
***********************************************************************/


/***************************************************************
** bootloader status acknowledge to PC terminal
** input: the status to feed back
** output: none
****************************************************************/




/*******************************************************************
** function: clean the bootloader used RAM  for NVM driver
** input: none
** output: none
********************************************************************/
void CleanRAM(void)
{
    uint32_t cnt;
    uint32_t *pRam;

    /*
     * clean system SRAM
     */
    cnt = __SRAM_SIZE/4;
    pRam = (uint32_t*)__SRAM_BASE_ADDR;

    while(cnt--)
        *pRam++ = 0;

    /*
	 * clean system Local DMEM
	 */
	cnt = __LOCAL_DMEM_SIZE/4;
	pRam = (uint32_t*)__LOCAL_DMEM_BASE_ADDR;

	while(cnt--)
		*pRam++ = 0;
}

/*******************************************************************
** function: do the preparation for bootloader jump to user app
** input: none
** output: none
********************************************************************/
void Prepare_Before_Jump_m(void)
{
	flexcan_0_deinit_fnc();   /*shutdown the MSCAN module*/
	PPCASM (" wrteei 0 "); 		  /*disable the CPU interrupt*/
    CleanRAM();       /*clean the bootloader used RAM  for NVM driver*/

}


/*************************************************************************************************
 * this function call the Flash driver to:
 *  1.initialize the Flash module
 *  2. lock the UTest Flash space and bootloader reside space
 *  2.unlock the user application code Flash space
 ************************************************************************************************/



/***************************************************************************************
 * delay some microsecond with software loop
 ***************************************************************************************/
void Cpu_DelayMS(uint32_t ms)
{
	uint32_t Cpu_Freq;
	while(ms--)
	{
		for(Cpu_Freq = 0x1000;Cpu_Freq>0;)
			Cpu_Freq--;
	}
}

/****************************** CODE FOR CAN COMMUNICATION *************************************/
/* Values in ASCII table should be avoided 0~9, a~f, A~F, s, S                                 */
/* 0~9: 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39                                      */
/* a~f: 0x61,0x62,0x63,0x64,0x65,0x66                                                          */
/* A~F: 0x41,0x42,0x43,0x44,0x45,0x46                                                          */
/* s:   0x73                                                                                   */
/* S:   0x53                                                                                   */
/***********************************************************************************************/
/*
#define  LOAD_LINK         0xFF  
#define  LOAD_DATA         0xFE
#define  LOAD_LINE_END     0xFD
#define  LOAD_FILE_END     0xFC

#define  UP_READY          0xC3
#define  UP_BUSY           0xC2
#define  UP_PRGEND         0xC1
#define  UP_ERR            0x00
*/



        
      
    
    
    
    
    
