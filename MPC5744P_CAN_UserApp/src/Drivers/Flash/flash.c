#include "Drivers/Flash/flash.h"

/***************************************
 * Flash Block Range

£º
// LOW--not program, partition 0 and 1
0x00800000-0x00803FFF		--		16KB,EEPROM-low block0, RWW_P:0
0x00804000-0x00807FFF		--		16KB,EEPROM-low block1, RWW_P:0
0x00F98000-0x00F9BFFF		--		16KB,low flash memory block2, RWW_P:1
0x00F9C000-0x00F9FFFF		--		16KB,low flash memory block3, RWW_P:1

// MID , partition 2 and 3
0x00808000-0x0080FFFF		--		32KB,EEPROM-mid block0, RWW_P:2
0x00810000-0x00817FFF		--		32KB,EEPROM-mid block1, RWW_P:3

//HIGH, , partition 4 and 5
0x00FA0000-0x00FAFFFF		--		64KB,high flash memory block0, RWW_P:4
0x00FB0000-0x00FBFFFF		--		64KB,high flash memory block1, RWW_P:4
0x00FC0000-0x00FCFFFF		--		64KB,high flash memory block2, RWW_P:4
0x00FD0000-0x00FDFFFF		--		64KB,high flash memory block3, RWW_P:5
0x00FE0000-0x00FEFFFF		--		64KB,high flash memory block4, RWW_P:5
0x00FF0000-0x00FFFFFF		--		64KB,high flash memory block5, RWW_P:5

//256K--all used
0x01000000-0x0103FFFF		--		256KB,256k flash memory block0, RWW_P:6
0x01040000-0x0107FFFF		--		256KB,256k flash memory block1, RWW_P:6
0x01080000-0x010BFFFF		--		256KB,256k flash memory block2, RWW_P:6
0x010C0000-0x010FFFFF		--		256KB,256k flash memory block3, RWW_P:6
0x01100000-0x0113FFFF		--		256KB,256k flash memory block4, RWW_P:7
0x01140000-0x0117FFFF		--		256KB,256k flash memory block5, RWW_P:7
0x01180000-0x011BFFFF		--		256KB,256k flash memory block6, RWW_P:7
0x011C0000-0x011FFFFF		--		256KB,256k flash memory block7, RWW_P:7

 **************************************/


#define FLASH_TOTAL_BLOCKCOUNT 20
#define FLASH_LOWPART_BLOCKCOUNT 4
#define FLASH_MIDPART_BLOCKCOUNT 2
#define FLASH_HIGHPART_BLOCKCOUNT 6
#define FLASH_256KPART_BLOCKCOUNT 8


//low
#define FLASH_BLOCKPART_LOW_BLOCKNUM_0_START 0x00800000
#define FLASH_BLOCKPART_LOW_BLOCKNUM_0_END	0x00803FFF
#define FLASH_BLOCKPART_LOW_BLOCKNUM_1_START 0x00804000
#define FLASH_BLOCKPART_LOW_BLOCKNUM_1_END 0x00807FFF
#define FLASH_BLOCKPART_LOW_BLOCKNUM_2_START 0x00F98000
#define FLASH_BLOCKPART_LOW_BLOCKNUM_2_END 0x00F9BFFF
#define FLASH_BLOCKPART_LOW_BLOCKNUM_3_START 0x00F9C000
#define FLASH_BLOCKPART_LOW_BLOCKNUM_3_END 0x00F9FFFF

//mid
#define FLASH_BLOCKPART_MID_BLOCKNUM_0_START 0x00808000
#define FLASH_BLOCKPART_MID_BLOCKNUM_0_END 0x0080FFFF
#define FLASH_BLOCKPART_MID_BLOCKNUM_1_START 0x00810000
#define FLASH_BLOCKPART_MID_BLOCKNUM_1_END 0x00817FFF

//high
#define FLASH_BLOCKPART_HIGH_BLOCKNUM_0_START 0x00FA0000
#define FLASH_BLOCKPART_HIGH_BLOCKNUM_0_END 0x00FAFFFF
#define FLASH_BLOCKPART_HIGH_BLOCKNUM_1_START 0x00FB0000
#define FLASH_BLOCKPART_HIGH_BLOCKNUM_1_END 0x00FBFFFF
#define FLASH_BLOCKPART_HIGH_BLOCKNUM_2_START 0x00FC0000
#define FLASH_BLOCKPART_HIGH_BLOCKNUM_2_END 0x00FCFFFF
#define FLASH_BLOCKPART_HIGH_BLOCKNUM_3_START 0x00FD0000
#define FLASH_BLOCKPART_HIGH_BLOCKNUM_3_END 0x00FDFFFF
#define FLASH_BLOCKPART_HIGH_BLOCKNUM_4_START 0x00FE0000
#define FLASH_BLOCKPART_HIGH_BLOCKNUM_4_END 0x00FEFFFF
#define FLASH_BLOCKPART_HIGH_BLOCKNUM_5_START 0x00FF0000
#define FLASH_BLOCKPART_HIGH_BLOCKNUM_5_END 0x00FFFFFF

//256k
#define FLASH_BLOCKPART_256K_BLOCKNUM_0_START 0x01000000
#define FLASH_BLOCKPART_256K_BLOCKNUM_0_END 0x0103FFFF
#define FLASH_BLOCKPART_256K_BLOCKNUM_1_START 0x01040000
#define FLASH_BLOCKPART_256K_BLOCKNUM_1_END 0x0107FFFF
#define FLASH_BLOCKPART_256K_BLOCKNUM_2_START 0x01080000
#define FLASH_BLOCKPART_256K_BLOCKNUM_2_END 0x010BFFFF
#define FLASH_BLOCKPART_256K_BLOCKNUM_3_START 0x010C0000
#define FLASH_BLOCKPART_256K_BLOCKNUM_3_END 0x010FFFFF
#define FLASH_BLOCKPART_256K_BLOCKNUM_4_START 0x01100000
#define FLASH_BLOCKPART_256K_BLOCKNUM_4_END 0x0113FFFF
#define FLASH_BLOCKPART_256K_BLOCKNUM_5_START 0x01140000
#define FLASH_BLOCKPART_256K_BLOCKNUM_5_END 0x0117FFFF
#define FLASH_BLOCKPART_256K_BLOCKNUM_6_START 0x01180000
#define FLASH_BLOCKPART_256K_BLOCKNUM_6_END 0x011BFFFF
#define FLASH_BLOCKPART_256K_BLOCKNUM_7_START 0x011C0000
#define FLASH_BLOCKPART_256K_BLOCKNUM_7_END 0x011FFFFF


/* Lock State , shift is in the setlock function*/
#define LOCK_LOW_BLOCKS               0x00003FFFU  //lock0
#define LOCK_MID_BLOCKS               0x0000FFFFU  //lock0
#define LOCK_HIGH_BLOCKS              0x0000FFFFU  //lock1
#define LOCK_FIRST256_BLOCKS          0xFFFFFFFFU  //lock2
//#define UNLOCK_SECOND256_BLOCKS         0x0000FFFFU  //lock3

#define UNLOCK_LOW_BLOCKS               0x00000000U
#define UNLOCK_MID_BLOCKS               0x00000000U
#define UNLOCK_HIGH_BLOCKS              0x00000000U
#define UNLOCK_FIRST256_BLOCKS          0x00000000U
//#define UNLOCK_SECOND256_BLOCKS         0x00000000U


#define NUMBER_OF_WORD_BLANK_CHECK      0x90
#define NUMBER_OF_WORD_PGM_VERIFY       0x80
#define NUMBER_OF_WORD_CHECK_SUM        0x120

/* Platform Flash */
#define FLASH_FMC                       PFLASH_BASE
#define FLASH_PFCR1                     0x000000000U
#define FLASH_PFCR2                     0x000000004U
#define FLASH_FMC_BFEN_MASK             0x000000001U



//Block part and number structure
typedef struct
{
	uint32_t BlockPart; //Low,Mid,High or 256K
	uint32_t BlockNum; //Block 0...n
}Flash_BlockPartAndNum;



uint32_t Flash_Block_Lock[4]={
	LOCK_LOW_BLOCKS
	,LOCK_MID_BLOCKS
	,LOCK_HIGH_BLOCKS
	,LOCK_FIRST256_BLOCKS
};

uint32_t Flash_Block_UnLock[4]={
	UNLOCK_LOW_BLOCKS
	,UNLOCK_MID_BLOCKS
	,UNLOCK_HIGH_BLOCKS
	,UNLOCK_FIRST256_BLOCKS
};

//every part block count
uint32_t Flash_Block_Count[4]={
	FLASH_LOWPART_BLOCKCOUNT
	,FLASH_MIDPART_BLOCKCOUNT
	,FLASH_HIGHPART_BLOCKCOUNT
	,FLASH_256KPART_BLOCKCOUNT
};



//low space block start array
uint32_t Flash_Block_Low_Start_Array[FLASH_LOWPART_BLOCKCOUNT]={
	FLASH_BLOCKPART_LOW_BLOCKNUM_0_START
	,FLASH_BLOCKPART_LOW_BLOCKNUM_1_START
    ,FLASH_BLOCKPART_LOW_BLOCKNUM_2_START
    ,FLASH_BLOCKPART_LOW_BLOCKNUM_3_START
};

//mid space block start array
uint32_t Flash_Block_Mid_Start_Array[FLASH_MIDPART_BLOCKCOUNT]={

//mid
    FLASH_BLOCKPART_MID_BLOCKNUM_0_START
    ,FLASH_BLOCKPART_MID_BLOCKNUM_1_START
};

//high space block start array
uint32_t Flash_Block_High_Start_Array[FLASH_HIGHPART_BLOCKCOUNT]={
//high
    FLASH_BLOCKPART_HIGH_BLOCKNUM_0_START
    ,FLASH_BLOCKPART_HIGH_BLOCKNUM_1_START
    ,FLASH_BLOCKPART_HIGH_BLOCKNUM_2_START
    ,FLASH_BLOCKPART_HIGH_BLOCKNUM_3_START
    ,FLASH_BLOCKPART_HIGH_BLOCKNUM_4_START
    ,FLASH_BLOCKPART_HIGH_BLOCKNUM_5_START

};

//256k space block start array
uint32_t Flash_Block_256K_Start_Array[FLASH_256KPART_BLOCKCOUNT]={
//256k
    FLASH_BLOCKPART_256K_BLOCKNUM_0_START
    ,FLASH_BLOCKPART_256K_BLOCKNUM_1_START
    ,FLASH_BLOCKPART_256K_BLOCKNUM_2_START
    ,FLASH_BLOCKPART_256K_BLOCKNUM_3_START
    ,FLASH_BLOCKPART_256K_BLOCKNUM_4_START
    ,FLASH_BLOCKPART_256K_BLOCKNUM_5_START
    ,FLASH_BLOCKPART_256K_BLOCKNUM_6_START
    ,FLASH_BLOCKPART_256K_BLOCKNUM_7_START

};


//low space block end array
uint32_t Flash_Block_Low_End_Array[FLASH_LOWPART_BLOCKCOUNT]={
	FLASH_BLOCKPART_LOW_BLOCKNUM_0_END
	,FLASH_BLOCKPART_LOW_BLOCKNUM_1_END
    ,FLASH_BLOCKPART_LOW_BLOCKNUM_2_END
    ,FLASH_BLOCKPART_LOW_BLOCKNUM_3_END
};

//mid space block end array
uint32_t Flash_Block_Mid_End_Array[FLASH_MIDPART_BLOCKCOUNT]={

//mid
    FLASH_BLOCKPART_MID_BLOCKNUM_0_END
    ,FLASH_BLOCKPART_MID_BLOCKNUM_1_END
};

//high space block end array
uint32_t Flash_Block_High_End_Array[FLASH_HIGHPART_BLOCKCOUNT]={
//high
    FLASH_BLOCKPART_HIGH_BLOCKNUM_0_END
    ,FLASH_BLOCKPART_HIGH_BLOCKNUM_1_END
    ,FLASH_BLOCKPART_HIGH_BLOCKNUM_2_END
    ,FLASH_BLOCKPART_HIGH_BLOCKNUM_3_END
    ,FLASH_BLOCKPART_HIGH_BLOCKNUM_4_END
    ,FLASH_BLOCKPART_HIGH_BLOCKNUM_5_END

};

//256k space block end array
uint32_t Flash_Block_256K_End_Array[FLASH_256KPART_BLOCKCOUNT]={
//256k
    FLASH_BLOCKPART_256K_BLOCKNUM_0_END
    ,FLASH_BLOCKPART_256K_BLOCKNUM_1_END
    ,FLASH_BLOCKPART_256K_BLOCKNUM_2_END
    ,FLASH_BLOCKPART_256K_BLOCKNUM_3_END
    ,FLASH_BLOCKPART_256K_BLOCKNUM_4_END
    ,FLASH_BLOCKPART_256K_BLOCKNUM_5_END
    ,FLASH_BLOCKPART_256K_BLOCKNUM_6_END
    ,FLASH_BLOCKPART_256K_BLOCKNUM_7_END

};



uint32_t pflash_pfcr1, pflash_pfcr2;
uint32_t g_usrCnt = 0U;


/**************************************************************
*                          Disable Flash Cache                *
***************************************************************/
void DisableFlashControllerCache(uint32_t flashConfigReg,
                                 uint32_t disableVal,
                                 uint32_t *origin_pflash_pfcr)
{
    /* Read the values of the register of flash configuration */
    *origin_pflash_pfcr = REG_READ32(FLASH_FMC + flashConfigReg);

    /* Disable Caches */
    REG_BIT_CLEAR32(FLASH_FMC + flashConfigReg, disableVal);
}

/*****************************************************************
*               Restore configuration register of FCM            *
******************************************************************/
void RestoreFlashControllerCache(uint32_t flashConfigReg,
                                 uint32_t pflash_pfcr)
{
    REG_WRITE32(FLASH_FMC + flashConfigReg, pflash_pfcr);
}

void UserCallBack(void)
{
    /* increase g_usrCnt */
    g_usrCnt++;
}



//get addressrage select block
flash_block_select_t Flash_GetSelectBlock(uint32_t startAddr, uint32_t endAddr)
{
	flash_block_select_t blockSelect={.lowBlockSelect=0,.midBlockSelect=0,.highBlockSelect=0,.first256KBlockSelect = 0,.second256KBlockSelect=0};
	uint8_t i;

	for(i=0;i<FLASH_LOWPART_BLOCKCOUNT;i++)
	{

		if( !(startAddr>Flash_Block_Low_End_Array[i] || endAddr<Flash_Block_Low_Start_Array[i] ) )
		{
			blockSelect.lowBlockSelect |= 1<<i;
		}

	}

	for(i=0;i<FLASH_MIDPART_BLOCKCOUNT;i++)
		{

			if( !(startAddr>Flash_Block_Mid_End_Array[i] || endAddr<Flash_Block_Mid_Start_Array[i] ) )
			{
				blockSelect.midBlockSelect |= 1<<i;
			}

		}

	for(i=0;i<FLASH_HIGHPART_BLOCKCOUNT;i++)
			{

				if( !(startAddr>Flash_Block_High_End_Array[i] || endAddr<Flash_Block_High_Start_Array[i] ) )
				{
					blockSelect.highBlockSelect |= 1<<i;
				}

			}

	for(i=0;i<FLASH_256KPART_BLOCKCOUNT;i++)
			{

				if( !(startAddr>Flash_Block_256K_End_Array[i] || endAddr<Flash_Block_256K_Start_Array[i] ) )
				{
					blockSelect.first256KBlockSelect |= 1<<i;
				}

			}

	//no more 256K block for second256KBlockSelect;



	return blockSelect;
}

/*****************************************************************
 *
 *
 *  Functions Can Be Called Below
 *
 *
 * **************************************************************/


status_t Flash_Unlock_g(uint32_t startAddr, uint32_t endAddr)
{
	status_t ret = STATUS_SUCCESS;
	uint32_t blkLockState;         /* block lock status to be retrieved */
	flash_block_select_t blockSelect={.lowBlockSelect=0,.midBlockSelect=0,.highBlockSelect=0,.first256KBlockSelect = 0,.second256KBlockSelect=0};


	/* Invalidate flash controller cache */
	DisableFlashControllerCache(FLASH_PFCR1, FLASH_FMC_BFEN_MASK, &pflash_pfcr1);
	DisableFlashControllerCache(FLASH_PFCR2, FLASH_FMC_BFEN_MASK, &pflash_pfcr2);

	/* Flash Initialization */
	ret = FLASH_DRV_Init();
	DEV_ASSERT(ret == STATUS_SUCCESS);

	/**************************************************************************/
	/* Lock to protect UTest address space                                    */
	/**************************************************************************/
	ret = FLASH_DRV_GetLock(C55_BLOCK_UTEST, &blkLockState);

	if (!(blkLockState & 0x00000001U))
	{
		ret = FLASH_DRV_SetLock(C55_BLOCK_UTEST, 0x1U);
		if (STATUS_SUCCESS != ret)
		{
			return ret;
		}
	}

	blockSelect = Flash_GetSelectBlock(startAddr, endAddr);

	//UnLock all block in range
	if(blockSelect.lowBlockSelect!=0)
	{
		ret = FLASH_DRV_SetLock(0, Flash_Block_UnLock[0]);
		if (STATUS_SUCCESS != ret)
			return ret;

	}

	if(blockSelect.midBlockSelect!=0)
		{
			ret = FLASH_DRV_SetLock(1, Flash_Block_UnLock[1]);
			if (STATUS_SUCCESS != ret)
				return ret;

		}

	if(blockSelect.highBlockSelect!=0)
			{
				ret = FLASH_DRV_SetLock(2, Flash_Block_UnLock[2]);
				if (STATUS_SUCCESS != ret)
					return ret;

			}

	if(blockSelect.first256KBlockSelect!=0)
			{
				ret = FLASH_DRV_SetLock(3, Flash_Block_UnLock[3]);
				if (STATUS_SUCCESS != ret)
					return ret;

			}


	return ret;
}


status_t Flash_Erase_g(uint32_t startAddr, uint32_t endAddr)
{
	status_t ret = STATUS_SUCCESS;
	flash_state_t opResult;
	flash_block_select_t blockSelect;

	/**************************************************************************/
	/* Call FLASH_DRV_Erase, FLASH_DRV_BlankCheck, FLASH_DRV_Program,         */
	/* FLASH_DRV_ProgramVerify, FLASH_DRV_CheckSum functions                  */
	/**************************************************************************/
//	blockSelect.lowBlockSelect = 0x0U;
//	blockSelect.midBlockSelect = 0x0U;
//	blockSelect.highBlockSelect = 0x4U;
//	blockSelect.first256KBlockSelect = 0x0U;
//	blockSelect.second256KBlockSelect = 0x0U;
	blockSelect = Flash_GetSelectBlock(startAddr,endAddr);
	g_usrCnt = 0U;

	/* Erase block */
	ret = FLASH_DRV_Erase(ERS_OPT_MAIN_SPACE, &blockSelect);

	if (STATUS_SUCCESS == ret)
	{
		do
		{
			/* The user can do any tasks while check status function is still in progress */
			UserCallBack();
			ret = FLASH_DRV_CheckEraseStatus(&opResult);
		}while(ret == STATUS_FLASH_INPROGRESS);
	}

	if (STATUS_SUCCESS != ret)
	{
		return ret;
	}

	return ret;
}

status_t Flash_Check_g(uint32_t startAddr, uint32_t endAddr)
{
	status_t ret = STATUS_SUCCESS;
	uint32_t dest;                 /* destination address */
	uint32_t size;                 /* size applicable */
	uint32_t numOfWordCycle;
	uint32_t failedAddress;        /* save the failed address in flash */

	dest = startAddr;
	size = endAddr + 1 - startAddr;
	numOfWordCycle = NUMBER_OF_WORD_BLANK_CHECK;

	/* Blank check */
	ret = FLASH_DRV_BlankCheck(dest,
							   size,
							   numOfWordCycle,
							   &failedAddress,
							   NULL_CALLBACK);

	if (STATUS_SUCCESS != ret)
	{
		return ret;
	}
	return ret;
}

//every 4 addresses is the same, such 0xFC0000=0xFC0001=0xFC0002=0xFC0003
status_t Flash_Program_g(uint32_t startAddr, uint32_t endAddr, uint32_t *buffer, uint32_t len)
{
	status_t ret = STATUS_SUCCESS;
	uint32_t dest;                 /* destination address */
	uint32_t size;                 /* size applicable */
	uint32_t source;               /* source address for program and verify */
	uint32_t numOfWordCycle;
	flash_state_t opResult;
	uint32_t failedAddress;        /* save the failed address in flash */
	uint32_t sum;                  /* check sum result */
	flash_context_data_t pCtxData;

	/* Program to beginning of block */
	dest = startAddr;
	size = len*C55_WORD_SIZE;
	source = (uint32_t)buffer;
	g_usrCnt = 0U;
	ret = FLASH_DRV_Program(&pCtxData,
							dest,
							size,
							source);

	if (STATUS_SUCCESS == ret)
	{
		do
		{
			/* The user can do any tasks while check status function is still in progress */
			UserCallBack();
			ret = FLASH_DRV_CheckProgramStatus(&pCtxData, &opResult);
		}while(ret == STATUS_FLASH_INPROGRESS);
	}

	if (STATUS_SUCCESS != ret)
	{
		return ret;
	}

	numOfWordCycle = NUMBER_OF_WORD_PGM_VERIFY;
	/* Program verify */
	ret = FLASH_DRV_ProgramVerify(dest,
								  size,
								  source,
								  numOfWordCycle,
								  &failedAddress,
								  NULL_CALLBACK);

	if (STATUS_SUCCESS != ret)
	{
		return ret;
	}

	numOfWordCycle = NUMBER_OF_WORD_CHECK_SUM;
	/* Check sum */
	ret = FLASH_DRV_CheckSum(dest,
							 size,
							 numOfWordCycle,
							 &sum,
							 NULL_CALLBACK);

	if ((STATUS_SUCCESS != ret) && (sum != 0U))
	{
		return ret;
	}

	return ret;
}


status_t Flash_Lock_g(uint32_t startAddr, uint32_t endAddr)
{
	status_t ret = STATUS_SUCCESS;

	flash_block_select_t blockSelect={.lowBlockSelect=0,.midBlockSelect=0,.highBlockSelect=0,.first256KBlockSelect = 0,.second256KBlockSelect=0};

	//UnLock block
	blockSelect = Flash_GetSelectBlock(startAddr, endAddr);

		//UnLock all block in range
		if(blockSelect.lowBlockSelect!=0)
		{
			ret = FLASH_DRV_SetLock(0, Flash_Block_Lock[0]);
			if (STATUS_SUCCESS != ret)
				return ret;

		}

		if(blockSelect.midBlockSelect!=0)
			{
				ret = FLASH_DRV_SetLock(1, Flash_Block_Lock[1]);
				if (STATUS_SUCCESS != ret)
					return ret;

			}

		if(blockSelect.highBlockSelect!=0)
				{
					ret = FLASH_DRV_SetLock(2, Flash_Block_Lock[2]);
					if (STATUS_SUCCESS != ret)
						return ret;

				}

		if(blockSelect.first256KBlockSelect!=0)
				{
					ret = FLASH_DRV_SetLock(3, Flash_Block_Lock[3]);
					if (STATUS_SUCCESS != ret)
						return ret;

				}


	/* Restore flash controller cache */
	RestoreFlashControllerCache(FLASH_PFCR1, pflash_pfcr1);
	RestoreFlashControllerCache(FLASH_PFCR2, pflash_pfcr2);


	return ret;

}

//every 4 addresses is the same, such 0xFC0000=0xFC0001=0xFC0002=0xFC0003
uint32_t Flash_Read_g(uint32_t addr)
{
	uint32_t result;
	result = *(volatile uint32_t *)addr;
	return result;
}
