/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!
 * @file flash_c55_driver.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.1, Conversion between a pointer
 * to function and another type.
 * The cast is required to define a callback function.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, cast from pointer to unsigned long
 * The cast is needed to get the addresses of register on hardware modules.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block scope
 * The variables FLASH_BLOCK_ADDR,FLASH_BLOCK_MASK identifier only appears in a
 * single function and they must be visible.
 */

#include "flash_c55_driver.h"
#include "flash_c55_hw_access.h"

/* Block base address */
static const uint32_t FLASH_BLOCK_ADDR[NUM_BLOCK] = FLASH_BLOCK_ADDR_DEFINE;

/* Block mask */
static const uint32_t FLASH_BLOCK_MASK[NUM_BLOCK] = FLASH_BLOCK_MASK_DEFINE;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_CheckProgramStatus
 * Description   : This function will check the status of on-going high voltage
 * in program operation.
 *
 * Implements    : FLASH_DRV_CheckProgramStatus_Activity
 *END**************************************************************************/
status_t FLASH_DRV_CheckProgramStatus(flash_context_data_t * pCtxData,
                                      flash_state_t * opResult)
{
    DEV_ASSERT(pCtxData != NULL);
    DEV_ASSERT(opResult != NULL);
    status_t returnCode = STATUS_SUCCESS;        /* return code */
    uint32_t RegAddress;                         /* address of MCR or UT0 register */
    uint32_t RegValue;                           /* content of MCR or UT0 register */

    *opResult = C55_OK;
    RegAddress = FLASH_C55_BASE + C55_MCR;
    RegValue = FLASH_C55->MCR;

    /* There must be a program operation */
    if ((RegValue & C55_MCR_PGM) != 0x0U)
    {
        /* If DONE bit goes high */
        if ((RegValue & C55_MCR_DONE) != 0x0U)
        {
            /* If the operation is suspended */
            if ((RegValue & C55_MCR_PSUS) != 0x0U)
            {
                /* Check if the suspended program is PGM_SUS or ERS_SUS_PGM_SUS */
                if ((RegValue & C55_MCR_ERS) != 0x0U)
                {
                    *opResult = C55_ERS_SUS_PGM_SUS;
                }
                else
                {
                    *opResult = C55_PGM_SUS;
                }
            }
            else
            {
                /* Check PGOOD (PEG) */
                if ((RegValue & C55_MCR_PEG) != 0x0U)
                {
                    /* Clear EHV if it is high */
                    if ((RegValue & C55_MCR_EHV) != 0x0U)
                    {
                        REG_BIT_CLEAR32(RegAddress, C55_MCR_EHV);
                        /* If there is remaining data needs to be programmed */
                        if ((pCtxData->size) != 0x0U)
                        {
                            /* Call FLASH_DRV_Program to continue the operation */
                            returnCode = FLASH_DRV_Program(pCtxData,
                                                           pCtxData->dest,
                                                           pCtxData->size,
                                                           pCtxData->source);

                            if (returnCode == STATUS_SUCCESS)
                            {
                                returnCode = STATUS_FLASH_INPROGRESS;
                            }
                        }
                        else /* pCtxData->size == 0 */
                        {
                            /* Program finished successfully, clear PGM */
                            REG_BIT_CLEAR32(RegAddress, C55_MCR_PGM);
                        }
                    }
                    else /* EHV == 0 */
                    {
                        if ((RegValue & C55_MCR_ERS) != 0x0U)
                        {
                            *opResult = C55_ERS_SUS_PGM_WRITE;
                        }
                        else
                        {
                            *opResult = C55_PGM_WRITE;
                        }
                    }
                }
                else /* PEG == 0 */
                {
                    if ((RegValue & C55_MCR_EHV) != 0x0U)
                    {
                        /* PGOOD (PEG) is low */
                        returnCode = STATUS_ERROR;
                        /* Clear EHV */
                        REG_BIT_CLEAR32(RegAddress, C55_MCR_EHV);
                        /* Program finished unsuccessfully, clear PGM */
                        REG_BIT_CLEAR32(RegAddress, C55_MCR_PGM);
                    }
                    else /* EHV == 0 */
                    {
                        /* PEG is invalid at this stage */
                        if ((RegValue & C55_MCR_ERS) != 0x0U)
                        {
                            *opResult = C55_ERS_SUS_PGM_WRITE;
                        }
                        else
                        {
                            *opResult = C55_PGM_WRITE;
                        }
                    }
                }
            }
        }
        else /* DONE == 0 */
        {
            /* DONE bit is still low means the operation is in-progress */
            returnCode = STATUS_FLASH_INPROGRESS;
        }
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_CheckEraseStatus
 * Description   : This function will check the status of on-going high voltage
 * in erase operation.
 *
 * Implements    : FLASH_DRV_CheckEraseStatus_Activity
 *END**************************************************************************/
status_t FLASH_DRV_CheckEraseStatus(flash_state_t * opResult)
{
    DEV_ASSERT(opResult != NULL);
    status_t returnCode = STATUS_SUCCESS;        /* return code */
    uint32_t RegAddress;                         /* address of MCR or UT0 register */
    uint32_t RegValue;                           /* content of MCR or UT0 register */

    *opResult = C55_OK;
    RegAddress = FLASH_C55_BASE + C55_MCR;
    RegValue = FLASH_C55->MCR;

    /* There must be an erase operation */
    if ((RegValue & C55_MCR_ERS) != 0x0U)
    {
        /* If DONE bit goes high */
        if ((RegValue & C55_MCR_DONE) != 0x0U)
        {
            /* If the operation is suspended */
            if ((RegValue & C55_MCR_ESUS) != 0x0U)
            {
                *opResult = C55_ERS_SUS;
            }
            else
            {
                /* Check EGOOD (PEG) */
                if ((RegValue & C55_MCR_PEG) != 0x0U)
                {
                    /* Clear EHV if it is high */
                    if ((RegValue & C55_MCR_EHV) != 0x0U)
                    {
                        REG_BIT_CLEAR32(RegAddress, C55_MCR_EHV);
                    }
                    else
                    {
                        *opResult = C55_ERS_WRITE;
                    }
                }
                else
                {
                    /* EGOOD (PEG) is low */
                    returnCode = STATUS_ERROR;

                    /* Clear EHV if it is high */
                    if ((RegValue & C55_MCR_EHV) != 0x0U)
                    {
                        REG_BIT_CLEAR32(RegAddress, C55_MCR_EHV);
                    }
                }

                if (*opResult != C55_ERS_WRITE)
                {
                    /* Clear ERS */
                    REG_BIT_CLEAR32(RegAddress, C55_MCR_ERS);
                }
            }
        }
        else
        {
            /* DONE bit is still low means the operation is in-progress */
            returnCode = STATUS_FLASH_INPROGRESS;
        }
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_CheckUserTestStatus
 * Description   : This function will check the status array integrity check
 * in user test mode.
 *
 * Implements    : FLASH_DRV_CheckUserTestStatus_Activity
 *END**************************************************************************/
status_t FLASH_DRV_CheckUserTestStatus(const flash_context_data_t * pCtxData,
                                       flash_state_t * opResult)
{
    DEV_ASSERT(pCtxData != NULL);
    DEV_ASSERT(opResult != NULL);
    status_t returnCode = STATUS_SUCCESS;        /* return code */
    uint32_t RegAddress;                         /* address of MCR or UT0 register */
    uint32_t RegValue;                           /* content of MCR or UT0 register */
    uint8_t index;
    uint32_t temp;

    *opResult = C55_OK;
    RegAddress = FLASH_C55_BASE + C55_UT0;
    RegValue = FLASH_C55->UT0;

    /* There must be an UTest operation */
    if ((RegValue & C55_UT0_AIE) != 0x0U)
    {
        /* If DONE bit goes high */
        if ((RegValue & C55_UT0_AID) != 0x0U)
        {
            /* If the operation is suspended */
            if ((RegValue & C55_UT0_AISUS) != 0x0U)
            {
                *opResult = C55_USER_TEST_SUS;
            }
            else
            {
                /* Compare the hardware calculated MISR to the user input */
                for (index = 0x0U; index < C55FMC_UM_COUNT; index++)
                {
                    temp = pCtxData->pMisr->arrMISRValue[index];
                    if (FLASH_C55->UM[index] != temp)
                    {
                        returnCode = STATUS_ERROR;
                        break;
                    }
                }

                if (FLASH_C55->UM9 != pCtxData->pMisr->arrMISRValue[9])
                {
                    returnCode = STATUS_ERROR;
                }

                /* If the operation is broken */
                if ((RegValue & C55_UT0_NAIBP) != 0x0U)
                {
                    if ((FLASH_C55->MCR & C55_MCR_SBC) != 0x0U)
                    {
                        *opResult = C55_USER_TEST_BREAK_SBC;
                    }
                    else
                    {
                        *opResult = C55_USER_TEST_BREAK_DBD;
                    }
                }
                else
                {
                    /* Clear UT0_MRE, UT0_AIS, UT0_AIE, then UT0_UTE */
                    REG_BIT_CLEAR32(RegAddress, C55_UT0_AIS | C55_UT0_MRE);
                    REG_BIT_CLEAR32(RegAddress, C55_UT0_AIE);
                    REG_BIT_CLEAR32(RegAddress, C55_UT0_UTE);
                }
            }
        }
        else
        {
            /* DONE bit is still low means the operation is in-progress */
            returnCode = STATUS_FLASH_INPROGRESS;
        }
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_Init
 * Description   : This function will initialize flash module and clear all error flags.
 *
 * Implements    : FLASH_DRV_Init_Activity
 *END**************************************************************************/
status_t FLASH_DRV_Init(void)
{
    /* Clear error bits MCR_EER, MCR_RWE, MCR_SBC, MCR_RVE, MCR_RRE, MCR_AEE, MCR_EEE */
    FLASH_C55->MCR = (C55_MCR_RVE | C55_MCR_RRE |
                      C55_MCR_AEE | C55_MCR_EEE |
                      C55_MCR_EER | C55_MCR_RWE | C55_MCR_SBC);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_SetLock
 * Description   : This function will set the block lock state for Low/Middle/High/256K
 * address space on the C55 module to protect them from program or erase.
 *
 * Implements    : FLASH_DRV_SetLock_Activity
 *END**************************************************************************/
status_t FLASH_DRV_SetLock(flash_address_space_t lockedBlockSelection,
                           uint32_t lockedBlockState)
{
    DEV_ASSERT(lockedBlockSelection <= C55_BLOCK_UTEST);
    status_t returnCode = STATUS_SUCCESS;    /* return code */
    uint32_t address = 0x0U;                 /* address of the block locking register */
    uint32_t mask = 0x0U;                    /* block locking field mask of an address space */
    uint32_t shift = 0x0U;                   /* bit shift value for the block locking field of an address space */
    uint32_t lockValue0;                     /* first value of the lock register */
    uint32_t lockValue1;                     /* second value of the lock register  */

    /* Determine which address space is selected */
    switch (lockedBlockSelection)
    {
        case C55_BLOCK_LOW:
            address = C55_LOCK0;
            shift = C55_LOCK0_LOWLOCK_SHIFT;
            mask = (0xFFFFFFFFU >> (32U - FLASH_GetNumOfLowBlock())) << 16U;
            break;
        case C55_BLOCK_MID:
            address = C55_LOCK0;
            shift = C55_LOCK0_MIDLOCK_SHIFT;
            mask = 0xFFFFFFFFU >> (32U - FLASH_GetNumOfMidBlock());
            break;
        case C55_BLOCK_HIGH:
            address = C55_LOCK1;
            shift = C55_LOCK1_HIGHLOCK_SHIFT;
            mask = 0xFFFFFFFFU >> (32U - FLASH_GetNumOfHighBlock());
            break;
        case C55_BLOCK_256K_FIRST:
            address = C55_LOCK2;
            shift = C55_LOCK2_A256KLOCK_SHIFT;
            if ( FLASH_GetNumOf256KBlock() >= 32U )
            {
                mask = 0xFFFFFFFFU;
            }
            else
            {
                mask = 0xFFFFFFFFU >> (32U - FLASH_GetNumOf256KBlock());
            }
            break;
        case C55_BLOCK_256K_SECOND:
            address = C55_LOCK3;
            shift = C55_LOCK3_A256KLOCK_SHIFT;
            /* Re-use lockValue0 to get the number of 256K blocks */
            lockValue0 = FLASH_GetNumOf256KBlock();
            if ( lockValue0 > 32U )
            {
                mask = 0xFFFFFFFFU >> (64U - lockValue0);
            }
            else
            {
                mask = 0x0U;
            }
            break;
        case C55_BLOCK_UTEST:
            address = C55_LOCK0;
            shift = C55_LOCK0_TSLOCK_SHIFT;
            mask = C55_LOCK0_TSLOCK_MASK;
            break;
        default: /* Nothing to do */
            break;
    }

    address += FLASH_C55_BASE;
    /* Clear the block lock field */
    REG_BIT_CLEAR32(address, mask);
    /* Set the new value to the block lock field */
    lockValue0 = (lockedBlockState << shift) & mask;
    REG_BIT_SET32(address, lockValue0);
    /* Read out the locking status */
    lockValue1 = REG_READ32(address) & lockValue0;
    /* Check the setting lock value with block lock field */
    if (lockValue0 != lockValue1)
    {
        returnCode = STATUS_ERROR;
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_GetLock
 * Description   : This function will check the block locking status of
 * Low/Middle/High/256K address spaces in the C55 module.
 *
 * Implements    : FLASH_DRV_GetLock_Activity
 *END**************************************************************************/
status_t FLASH_DRV_GetLock(flash_address_space_t lockedBlockSelection,
                           uint32_t * lockedBlockState)
{
    DEV_ASSERT(lockedBlockSelection <= C55_BLOCK_UTEST);
    uint32_t value;                              /* value of block locking register */
    uint32_t mask;                               /* block locking bit mask for the address space */
    uint32_t temp;

    /* Determine which address space is selected */
    switch (lockedBlockSelection)
    {
        case C55_BLOCK_LOW:
            value = FLASH_C55->LOCK0;
            mask = (0xFFFFFFFFU >> (32U - FLASH_GetNumOfLowBlock())) << C55_LOCK0_LOWLOCK_SHIFT;
            /* Mask off the other bits and make proper shift */
            *lockedBlockState = (value & mask) >> C55_LOCK0_LOWLOCK_SHIFT;
            break;
        case C55_BLOCK_MID:
            value = FLASH_C55->LOCK0;
            mask = 0xFFFFFFFFU >> (32U - FLASH_GetNumOfMidBlock());
            *lockedBlockState = (value & mask) >> C55_LOCK0_MIDLOCK_SHIFT;
            break;
        case C55_BLOCK_HIGH:
            value = FLASH_C55->LOCK1;
            mask = 0xFFFFFFFFU >> (32U - FLASH_GetNumOfHighBlock());
            *lockedBlockState = (value & mask) >> C55_LOCK1_HIGHLOCK_SHIFT;
            break;
        case C55_BLOCK_256K_FIRST:
            value = FLASH_C55->LOCK2;
            temp = FLASH_GetNumOf256KBlock();
            if ( temp >= 32U )
            {
                mask = 0xFFFFFFFFU;
            }
            else
            {
                mask = 0xFFFFFFFFU >> (32U - temp);
            }
            *lockedBlockState = (value & mask) >> C55_LOCK2_A256KLOCK_SHIFT;
            break;
        case C55_BLOCK_256K_SECOND:
            value = FLASH_C55->LOCK3;
            temp = FLASH_GetNumOf256KBlock();
            if ( temp > 32U )
            {
                mask = 0xFFFFFFFFU >> (64U - temp);
            }
            else
            {
                mask = 0x0U;
            }
            *lockedBlockState = (value & mask) >> C55_LOCK3_A256KLOCK_SHIFT;
            break;
        case C55_BLOCK_UTEST:
            value = FLASH_C55->LOCK0;
            mask = C55_LOCK0_TSLOCK_MASK;
            *lockedBlockState = (value & mask) >> C55_LOCK0_TSLOCK_SHIFT;
            break;
        default: /* Nothing to do */
            break;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_FindInterLockWriteAdd
 * Description   : This function finds the selected block address. It's used in
 * FLASH_DRV_Erase function to set value of interlock write the memory array.
 *
 *END**************************************************************************/
static uint32_t FLASH_DRV_FindInterLockWriteAdd(const flash_block_select_t * block)
{
    uint32_t retAdd = 0U;
    uint8_t i = 0U;

    if (block->lowBlockSelect > 0U)
    {
        for(i = 0U; i < NUM_LOW_BLOCK; i++)
        {
            if (((block->lowBlockSelect & FLASH_BLOCK_MASK[i]) >> i) == 1U)
            {
                retAdd = FLASH_BLOCK_ADDR[i];
                break;
            }
        }
    }
    #if (NUM_MID_BLOCK != 0U)
    else if (block->midBlockSelect > 0U)
    {
        for(i = 0U; i < NUM_MID_BLOCK; i++)
        {
            if (((block->midBlockSelect & FLASH_BLOCK_MASK[i + NUM_LOW_BLOCK]) >> i) == 1U)
            {
                retAdd = FLASH_BLOCK_ADDR[i + NUM_LOW_BLOCK];
                break;
            }
        }
    }
    #endif /* #if (NUM_MID_BLOCK != 0U) */
    #if (NUM_HIGH_BLOCK != 0U)
    else if (block->highBlockSelect > 0U)
    {
        for(i = 0U; i < NUM_HIGH_BLOCK; i++)
        {
            if (((block->highBlockSelect & FLASH_BLOCK_MASK[i + NUM_LOW_BLOCK + NUM_MID_BLOCK]) >> i) == 1U)
            {
                retAdd = FLASH_BLOCK_ADDR[i + NUM_LOW_BLOCK + NUM_MID_BLOCK];
                break;
            }
        }
    }
    #endif /* #if (NUM_HIGH_BLOCK != 0U) */
    #if (NUM_256K_BLOCK_FIRST != 0U)
    else if (block->first256KBlockSelect > 0U)
    {
        for(i = 0U; i < NUM_256K_BLOCK_FIRST; i++)
        {
            if (((block->first256KBlockSelect & FLASH_BLOCK_MASK[i + NUM_LOW_BLOCK + NUM_MID_BLOCK + NUM_HIGH_BLOCK]) >> i) == 1U)
            {
                retAdd = FLASH_BLOCK_ADDR[i + NUM_LOW_BLOCK + NUM_MID_BLOCK + NUM_HIGH_BLOCK];
                break;
            }
        }
    }
    #endif /* #if (NUM_256K_BLOCK_FIRST != 0U) */
    #if (NUM_256K_BLOCK_SECOND != 0U)
    else if (block->second256KBlockSelect > 0U)
    {
        for(i = 0U; i < NUM_256K_BLOCK_SECOND; i++)
        {
            if (((block->second256KBlockSelect & FLASH_BLOCK_MASK[i + NUM_LOW_BLOCK + NUM_MID_BLOCK + NUM_HIGH_BLOCK + NUM_256K_BLOCK_FIRST]) >> i) == 1U)
            {
                retAdd = FLASH_BLOCK_ADDR[i + NUM_LOW_BLOCK + NUM_MID_BLOCK + NUM_HIGH_BLOCK + NUM_256K_BLOCK_FIRST];
                break;
            }
        }
    }
    #endif /* #if (NUM_256K_BLOCK_SECOND != 0U) */
    else
    {
        /* Undefined value */
    }

    return retAdd;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_Erase
 * Description   : This function is to do erase operation for multi-blocks on single
 * flash module according to user's input arguments via main interface. The target
 * flash module status will be checked in advance to return relevant error code if any.
 * This function only sets the high voltage without waiting for the operation to be finished.
 * Instead, user must call FLASH_DRV_CheckEraseStatus function to confirm the successful
 * completion of this operation.
 *
 * Implements    : FLASH_DRV_Erase_Activity
 *END**************************************************************************/
status_t FLASH_DRV_Erase(flash_erase_option_t eraseOption,
                         flash_block_select_t * blockSelect)
{
    DEV_ASSERT(blockSelect != NULL);
    status_t returnCode = STATUS_SUCCESS;        /* return code */
    uint32_t interlockWriteAddress = 0U;         /* interlock write address */
    uint32_t MCRAddress;                         /* address of C55_MCR register */
    uint32_t MCRValue;                           /* content of C55_MCR register */
    uint32_t temp;

    /* Check for validity of erase option */
    if (eraseOption != ERS_OPT_UTEST_SPACE)
    {
        interlockWriteAddress = FLASH_DRV_FindInterLockWriteAdd(blockSelect);
    }
    else
    {
        interlockWriteAddress = FLASH_C55_INTERLOCK_WRITE_UTEST_ADDRESS;
    }

    MCRAddress = FLASH_C55_BASE + C55_MCR;
    MCRValue = FLASH_C55->MCR;
    if (((MCRValue & C55_MCR_PGM) != 0x0U) || (((MCRValue & C55_MCR_ERS) != 0x0U) && ((MCRValue & (C55_MCR_EHV | C55_MCR_ESUS)) != 0x0U)))
    {
        returnCode = STATUS_BUSY;
    }
    else
    {
        /* Set MCR_ERS to start erase operation */
        REG_BIT_SET32(MCRAddress, C55_MCR_ERS);
        MCRValue = FLASH_C55->MCR;
        if ((MCRValue & C55_MCR_ERS) == 0x0U)
        {
            returnCode = STATUS_FLASH_ERROR_ENABLE;
        }
        else
        {
            /* Correct the block selection for the case we erase Main array */
            if (eraseOption != ERS_OPT_UTEST_SPACE)
            {
                /* Mask off reserved bits for low address space */
                blockSelect->lowBlockSelect &= 0xFFFFFFFFU >> (32U - FLASH_GetNumOfLowBlock());
                /* Mask off reserved bits for mid address space */
                blockSelect->midBlockSelect &= 0xFFFFFFFFU >> (32U - FLASH_GetNumOfMidBlock());
                /* Mask off reserved bits for high address space */
                blockSelect->highBlockSelect &= 0xFFFFFFFFU >> (32U - FLASH_GetNumOfHighBlock());
                /* get the number of 256K blocks */
                temp = FLASH_GetNumOf256KBlock();

                /* Mask off reserved bits for 256K address space */
                if (temp <= 32U)
                {
                    blockSelect->first256KBlockSelect &= 0xFFFFFFFFU >> (32U - temp);
                    blockSelect->second256KBlockSelect &= 0U;
                }
                else
                {
                    blockSelect->first256KBlockSelect &= 0xFFFFFFFFU;
                    blockSelect->second256KBlockSelect &= 0xFFFFFFFFU >> (64U - temp);
                }

                /* No blocks to be selected for erase, just return */
                if ((blockSelect->lowBlockSelect | blockSelect->midBlockSelect | blockSelect->highBlockSelect |
                      blockSelect->first256KBlockSelect | blockSelect->second256KBlockSelect) == 0x0U)
                {
                    /* Return with STATUS_SUCCESS */
                    REG_BIT_CLEAR32(MCRAddress, C55_MCR_ERS);
                    MCRValue = FLASH_C55->MCR;
                }
                else
                {
                    /* Set the block selection registers */
                    FLASH_C55->SEL0 = ((blockSelect->lowBlockSelect << C55_SEL0_LOWSEL_SHIFT) | blockSelect->midBlockSelect);
                    FLASH_C55->SEL1 = blockSelect->highBlockSelect;
                    FLASH_C55->SEL2 = blockSelect->first256KBlockSelect;
                    FLASH_C55->SEL3 = blockSelect->second256KBlockSelect;
                }
            }

            /* Check if ERS bit is set */
            if ((MCRValue & C55_MCR_ERS) != 0x0U)
            {
                /* Interlock write */
                *((volatile uint32_t*)interlockWriteAddress) = 0xFFFFFFFFU;
                /* Write a 1 to MCR_EHV */
                REG_BIT_SET32(MCRAddress, C55_MCR_EHV);
                /* Return here, not wait until MCR_DONE is set */
            }
        }
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_BlankCheck
 * Description   : This function is to do blank check for the previous erase operation.
 * It will verify whether the expected flash range is blank or not. In case of mismatch,
 * the failed address and failed destination will be saved and relevant error code will
 * be returned.
 *
 * Implements    : FLASH_DRV_BlankCheck_Activity
 *END**************************************************************************/
status_t FLASH_DRV_BlankCheck(uint32_t dest,
                              uint32_t size,
                              uint32_t numOfWordCycle,
                              uint32_t * pFailedAddress,
                              void (*CallBack)(void))
{
    DEV_ASSERT(((dest | size) % C55_WORD_SIZE) == 0U);
    status_t returnCode = STATUS_SUCCESS;    /* return code */
    uint32_t destIndex;                      /* destination address index */
    uint32_t temp;

    while ((size > 0U) && (STATUS_SUCCESS == returnCode))
    {
        /* Callback service */
        if (NULL_CALLBACK != CallBack)
        {
             CallBack();
        }

        /* Number of words needs to be checked for blank */
        temp = size / C55_WORD_SIZE;

        /* In each blank check cycle, we take up to a certain words */
        if (temp >= numOfWordCycle)
        {
            temp = numOfWordCycle;
        }

        /* Word by word blank check */
        for (destIndex = 0U; destIndex < temp; destIndex++)
        {
            if ((*(volatile uint32_t *)dest) != 0xFFFFFFFFU)
            {
                returnCode = STATUS_ERROR;
                *pFailedAddress = dest;
                break;
            }
            dest += C55_WORD_SIZE;
            size -= C55_WORD_SIZE;
        }
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_Program
 * Description   : This function is to do program operation for single or
 * multi-programmable size via different interface on targeted flash module according
 * to user's input arguments. The targeted flash module status will be checked
 * in advance to return relevant error code if any.
 * This function only set the high voltage without waiting for the operation
 * to be finished. Instead, user must call FLASH_DRV_CheckProgramStatus function to confirm
 * the successful completion of this operation.
 *
 * Implements    : FLASH_DRV_Program_Activity
 *END**************************************************************************/
status_t FLASH_DRV_Program(flash_context_data_t * pCtxData,
                           uint32_t dest,
                           uint32_t size,
                           uint32_t source)
{
    DEV_ASSERT(pCtxData != NULL);
    DEV_ASSERT((dest % C55_DWORD_SIZE) == 0U);
    DEV_ASSERT((size % C55_WORD_SIZE) == 0U);
    status_t returnCode = STATUS_SUCCESS;    /* return code */
    uint32_t sourceIndex;                    /* source address index */
    uint32_t MCRAddress;                     /* address of C55_MCR register */
    uint32_t MCRValue;                       /* content of C55_MCR register */
    uint32_t temp;

    /* Address the MCR register to use */
    MCRAddress = FLASH_C55_BASE + C55_MCR;
    MCRValue = FLASH_C55->MCR;
    /* Cases that program operation can start:
     * 1. no program and erase sequence:(PGM low and ERS low)
     * 2. erase suspend with EHV low: (PGM low, ERS high, ESUS high, EHV low)
     *
     * Cases that program operation cannot start:
     * 1. program in progress (PGM high & EHV high)
     * 2. program in suspended state (PGM high & PSUS high)
     * 3. program not in progress (PGM low), but erase in progress but not in suspend state
     */
    if ((((MCRValue & C55_MCR_PGM) != 0x0U) && ((MCRValue & (C55_MCR_EHV | C55_MCR_PSUS)) != 0x0U)) ||
         (((MCRValue & C55_MCR_ERS) != 0x0U) && ((MCRValue & C55_MCR_ESUS) == 0x0U)))
    {
        returnCode = STATUS_BUSY;
    }
    else
    {
        /* Anything to program? */
        if (0U == size)
        {
            pCtxData->size = 0U;
        }
        else
        {
            /* Set MCR_PGM to start program operation */
            REG_BIT_SET32(MCRAddress, C55_MCR_PGM);
            MCRValue = FLASH_C55->MCR;
            if ((MCRValue & C55_MCR_PGM) == 0x0U)
            {
                returnCode = STATUS_FLASH_ERROR_ENABLE;
            }
            else
            {
                /* Compute the buffer size used in the program cycle and calculate number of word for interlock write */
                if (size > FLASH_C55_PROGRAMABLE_SIZE)
                {
                    temp = FLASH_C55_PROGRAMABLE_SIZE / C55_WORD_SIZE;
                }
                else
                {
                    temp = size / C55_WORD_SIZE;
                }

                /* Program data */
                for (sourceIndex = 0U; sourceIndex < temp; sourceIndex++)
                {
                    /* Programming write */
                    *(volatile uint32_t *)dest = *(volatile uint32_t *)source;
                    /* Update dest, size, source index */
                    dest += C55_WORD_SIZE;
                    source += C55_WORD_SIZE;
                    size -= C55_WORD_SIZE;

                    /* Is it time to do page programming?  */
                    if(0U == (dest % FLASH_C55_PROGRAMABLE_SIZE))
                    {
                        break;
                    }
                }

                /* Set MCR_EHV bit, then return immediately */
                REG_BIT_SET32(MCRAddress, C55_MCR_EHV);
                /* Update the context data */
                pCtxData->dest = dest;
                pCtxData->size = size;
                pCtxData->source = source;
            }
        }
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_ProgramVerify
 * Description   : This function is to verify the previous program operation.
 * It will verify if the programmed flash range matches the corresponding source
 * data buffer. In case of mismatch, the failed address, failed destination and
 * failed source will be saved and relevant error code will be returned.
 *
 * Implements    : FLASH_DRV_ProgramVerify_Activity
 *END**************************************************************************/
status_t FLASH_DRV_ProgramVerify(uint32_t dest,
                                 uint32_t size,
                                 uint32_t source,
                                 uint32_t numOfWordCycle,
                                 uint32_t * pFailedAddress,
                                 void (*CallBack)(void))
{
    DEV_ASSERT(((dest | size | source) % C55_WORD_SIZE) == 0U);
    status_t returnCode = STATUS_SUCCESS;    /* return code */
    uint32_t destIndex;                      /* destination address index */
    uint32_t temp;
    uint32_t valDest;

    while ((size > 0U) && (STATUS_SUCCESS == returnCode))
    {
        /* Callback service */
        if (NULL_CALLBACK != CallBack)
        {
             CallBack();
        }

        /* Number of words needs to be verified */
        temp = size / C55_WORD_SIZE;

        /* In each verify cycle, we take up to a certain words */
        if (temp >= numOfWordCycle)
        {
            temp = numOfWordCycle;
        }

        /* Word by word verify */
        for (destIndex = 0U; destIndex < temp; destIndex++)
        {
            valDest = *(volatile uint32_t *)dest;
            if (valDest != *(volatile uint32_t *)source)
            {
                returnCode = STATUS_ERROR;
                *pFailedAddress = dest;
                break;
            }

            dest += C55_WORD_SIZE;
            source += C55_WORD_SIZE;
            size -= C55_WORD_SIZE;
        }
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_CheckSum
 * Description   : This function will perform a 32-bit sum over the specified
 * flash memory range without carry, which provides a rapid method for data
 * integrity checking.
 *
 * Implements    : FLASH_DRV_CheckSum_Activity
 *END**************************************************************************/
status_t FLASH_DRV_CheckSum(uint32_t dest,
                            uint32_t size,
                            uint32_t numOfWordCycle,
                            uint32_t *pSum,
                            void (*CallBack)(void))
{
    DEV_ASSERT(((dest | size) % C55_WORD_SIZE) == 0U);
    uint32_t destIndex;                          /* destination address index */
    uint32_t temp;

    /* Initialize the sum variable */
    *pSum = 0U;

    while (size != 0U)
    {
        /* Callback service */
        if (NULL_CALLBACK != CallBack)
        {
             CallBack();
        }

        /* Number of words needs to be checked sum */
        temp = size / C55_WORD_SIZE;

        /* In each check sum cycle, we take up to a certain words */
        if (temp >= numOfWordCycle)
        {
            temp = numOfWordCycle;
        }

        /* Word by word checksum */
        for (destIndex = 0U; destIndex < temp; destIndex++)
        {
            *pSum += *(volatile uint32_t *)dest;
            dest += C55_WORD_SIZE;
            size -= C55_WORD_SIZE;
        }
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_SuspendExecution
 * Description   : This function will suspend the program, erase activities.
 * This one is called by the FLASH_DRV_Suspend function.
 *
 *END**************************************************************************/
static void FLASH_DRV_SuspendExecution(uint32_t MCRAdd,flash_state_t * suspendSta)
{
    uint16_t delay = 0U;                       /* variable added for delay*/
    uint32_t i = FLASH_SUSPEND_WAIT;           /* the loop count for the wait loop */

    if ((FLASH_C55->MCR & C55_MCR_EHV) == 0x0U)
    {
        /* a. interlock write */
        /* g. abort period */
        if ((FLASH_C55->MCR & C55_MCR_PGM) != 0x0U)
        {
            *suspendSta = C55_PGM_WRITE;

            if ((FLASH_C55->MCR & C55_MCR_ERS) != 0x0U)
            {
                *suspendSta = C55_ERS_SUS_PGM_WRITE;
            }
        }
        else
        {
            *suspendSta = C55_ERS_WRITE;
        }
        /* For interlock write stage, MCR_DONE bit already high */
        /* For abort transition stage, wait MCR_DONE bit set */
        /* Wait until MCR_DONE = 1 */
        while (((REG_READ32(MCRAdd) & C55_MCR_DONE) == 0x0U) && (i > 0U))
        {
            i--;
        }
        /* Set MCR_EHV to 0 */
        REG_BIT_CLEAR32(MCRAdd, C55_MCR_EHV);
    }
    else
    {
        /* b. high voltage active */
        /* e. resuming from suspend state */
        /* f. high voltage stopped */
        if ((FLASH_C55->MCR & C55_MCR_PGM) != 0x0U)
        {
            /* Set MCR_PSUS bit */
            REG_BIT_SET32(MCRAdd, C55_MCR_PSUS);
            /* Add delay to counter hardware bug */
            /* A delay of approximately 20us for the below clock settings */
            /* SYSTEM CLOCK FREQUENCY = 128MHZ */
            /* A delay of about 20us */
            for (delay = 0U; delay < FLASH_C55_PROGRAM_SUSPEND_DELAY; delay++)
            {}
        }
        else
        {
            /* Set MCR-ESUS bit */
            REG_BIT_SET32(MCRAdd, C55_MCR_ESUS);
            /* Add delay to counter hardware bug */
            /* A delay of approximately 40us for the below clock settings */
            /* SYSTEM CLOCK FREQUENCY = 128MHZ */
            /* A delay of about 40us */
            for (delay = 0U; delay < FLASH_C55_ERASE_SUSPEND_DELAY; delay++)
            {}
        }
    }
}
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_Suspend
 * Description   : This function will check if there is any high voltage operation
 * being in progress on the C55 module and if this operation can be suspended.
 * This function will suspend the ongoing operation if it can be suspended.
 *
 * Implements    : FLASH_DRV_Suspend_Activity
 *END**************************************************************************/
status_t FLASH_DRV_Suspend(flash_state_t * suspendState)
{
    DEV_ASSERT(suspendState != NULL);
    status_t returnCode = STATUS_SUCCESS;        /* return code */
    uint32_t MCRAddress;                         /* address of C55_MCR register */
    uint32_t MCRValue;                           /* content of C55_MCR register */
    uint32_t count;                              /* the loop count for the wait loop */
    *suspendState = C55_SUS_NOTHING;

    MCRAddress = FLASH_C55_BASE + C55_MCR;
    MCRValue = FLASH_C55->MCR;
    count = FLASH_SUSPEND_WAIT;
    /* no P/E sequence: PGM=0 and ERS=0
     * possible stages for program sequence:
     *    a. interlock write;               (PGM=1;EHV=0;  PSUS=0;DONE=1; ignore PEG)  OR (ERS=1;ESUS=1)
     *    b. high voltage active;           (PGM=1;EHV=1;  PSUS=0;DONE=0; ignore PEG)  OR (ERS=1;ESUS=1)
     *    c. entering suspend state;        (PGM=1;EHV=1;  PSUS=1;DONE=0; ignore PEG)  OR (ERS=1;ESUS=1)
     *    d. in suspend state;              (PGM=1;EHV=1/0;PSUS=1;DONE=1; ignore PEG)  OR (ERS=1;ESUS=1)
     *    e. resuming from suspend state;   (PGM=1;EHV=1;  PSUS=0;DONE=1; ignore PEG)  OR (ERS=1;ESUS=1)
     *    f. high voltage stopped;          (PGM=1;EHV=1;  PSUS=0;DONE=1; valid  PEG)  OR (ERS=1;ESUS=1)
     *    g. abort period.                  (PGM=1;EHV=0;  PSUS=0;DONE=0; ignore PEG)  OR (ERS=1;ESUS=1)
     * Note: 1. Only stage e and f has the same condition;
     *       2. PGM  set: STOP=0 and ERS=0 or ERS=1 and ESUS=1 and EHV=0
     *       3. PGM  clear: EHV=0 and PSUS=0 and DONE=1
     *       4. PSUS set: PGM=1 and EHV=1;
     *       5. PSUS clear: DONE=1 and EHV=1
     *
     * possible stages for erase sequence:
     *    a. interlock write;               (ERS=1;EHV=0;  ESUS=0;DONE=1; ignore PEG)  AND (PGM=0;PSUS=0)
     *    b. high voltage active;           (ERS=1;EHV=1;  ESUS=0;DONE=0; ignore PEG)  AND (PGM=0;PSUS=0)
     *    c. entering suspend state;        (ERS=1;EHV=1;  ESUS=1;DONE=0; ignore PEG)  AND (PGM=0;PSUS=0)
     *    d. in suspend state;              (ERS=1;EHV=1/0;ESUS=1;DONE=1; ignore PEG)  AND (PGM=0;PSUS=0)
     *    e. resuming from suspend state;   (ERS=1;EHV=1;  ESUS=0;DONE=1; ignore PEG)  AND (PGM=0;PSUS=0)
     *    f. high voltage stopped;          (ERS=1;EHV=1;  ESUS=0;DONE=1; valid  PEG)  AND (PGM=0;PSUS=0)
     *    g. abort period.                  (ERS=1;EHV=0;  ESUS=0;DONE=0; ignore PEG)  AND (PGM=0;PSUS=0)
     * Note: 1. Only stage e and f has the same condition
     *       2. ERS  set: STOP=0 and ERS=0 and PGM=0
     *       3. ERS  clear: EHV=0 and ESUS=0 and DONE=1
     *       4. ESUS set: ERS=1 and EHV=1 and PGM=0
     *       5. ESUS clear: DONE=1 and EHV=1 and PGM=0
     *
     * FlashSuspend procedures:
     * no sequence: NO_OPERATION
     * stage     a: PGM_WRITE, ERS_WRITE, ERS_SUS_PGM_WRITE
     * stage b,e,f: suspend and return the current state
     * stage   c,d: return the current state
     * stage     g: switch to stage "a" and return the current state
     */

    /* No program/erase sequence */
    if ((MCRValue & (C55_MCR_PGM | C55_MCR_ERS)) == 0x0U)
    {
        /* Nothing to do */
    }
    else
    {
        if ((((MCRValue & C55_MCR_PGM) != 0x0U) && ((MCRValue & C55_MCR_PSUS) == 0x0U)) ||
             (((MCRValue & C55_MCR_ERS) != 0x0U) && ((MCRValue & C55_MCR_ESUS) == 0x0U)))
        {
            FLASH_DRV_SuspendExecution(MCRAddress,suspendState);
        }

        /* Stages b, e, f, c and d will become a suspend state */
        /* Read MCR again */
        MCRValue = FLASH_C55->MCR;

        /* Check the high voltage is cleared or not */
        if ((MCRValue & C55_MCR_EHV) != 0x0U)
        {
            if ((MCRValue & C55_MCR_PSUS) != 0x0U)
            {
                *suspendState = C55_PGM_SUS;

                if ((MCRValue & C55_MCR_ESUS) != 0x0U)
                {
                    *suspendState = C55_ERS_SUS_PGM_SUS;
                }
            }
            else if ((MCRValue & C55_MCR_ESUS) != 0x0U)
            {
                *suspendState = C55_ERS_SUS;
            }
            else
            {
                returnCode = STATUS_ERROR;
            }

            /* Wait until MCR_DONE = 1 */
            while (((REG_READ32(MCRAddress) & C55_MCR_DONE) == 0x0U) && (count > 0U))
            {
                count--;
            }

            /* Set MCR_EHV to 0 */
            REG_BIT_CLEAR32(MCRAddress, C55_MCR_EHV);
        }
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_Resume
 * Description   : This function will check if there is any suspended erase
 * or program operation on the C55 module, and will resume the suspended operation
 * if there is any.
 *
 * Implements    : FLASH_DRV_Resume_Activity
 *END**************************************************************************/
status_t FLASH_DRV_Resume(flash_state_t * resumeState)
{
    DEV_ASSERT(resumeState != NULL);
    status_t returnCode = STATUS_SUCCESS;    /* return code */
    uint32_t MCRAddress;                     /* address of C55_MCR register */
    uint32_t MCRValue;                       /* content of C55_MCR register */
    uint32_t bitMask;                        /* PSUS or ESUS bit to be cleared */
    uint32_t count;                          /* the loop count for the wait loop */
    *resumeState = C55_RES_NOTHING;

    /* Resume the operation according to the interface selection */
    MCRAddress = FLASH_C55_BASE + C55_MCR;
    MCRValue = FLASH_C55->MCR;
    count = FLASH_RESUME_WAIT;
    bitMask = 0x0U;

    /* Program in suspend state or entering suspend state */
    if ((MCRValue & C55_MCR_PSUS) != 0x0U)
    {
        *resumeState = C55_RES_PGM;
        bitMask = C55_MCR_PSUS;

        if ((MCRValue & C55_MCR_ESUS) != 0x0U)
        {
            /* Erase-suspended program is suspended */
            *resumeState = C55_RES_ERS_PGM;
        }
    }
    else
    {
        /* Erase in suspend state or entering suspend state */
        if (((MCRValue & C55_MCR_ESUS) != 0x0U) && ((MCRValue & C55_MCR_PGM) == 0x0U))
        {
            *resumeState = C55_RES_ERS;
            bitMask = C55_MCR_ESUS;
        }
        else
        {
            /* No suspended operation */
        }
    }

    if (*resumeState != C55_RES_NOTHING)
    {
        /* Set MCR_EHV bit */
        REG_BIT_SET32(MCRAddress, C55_MCR_EHV);
        /* Clear the MCR_ESUS bit or MCR_PSUS bit */
        REG_BIT_CLEAR32(MCRAddress, bitMask);
        MCRValue = FLASH_C55->MCR;
        if ((MCRValue & bitMask) != 0x0U)
        {
            returnCode = STATUS_ERROR;
        }

        /* Wait the MCR_DONE bit goes low */
        while (((REG_READ32(MCRAddress) & C55_MCR_DONE) != 0x0U) && (count > 0U))
        {
            count--;
        }
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_Abort
 * Description   : This function will abort a program or erase operation in user
 * mode and clear all PGM, PSUS, ERS, ESUS, EHV bits in MCR via main interface.
 *
 * Implements    : FLASH_DRV_Abort_Activity
 *END**************************************************************************/
status_t FLASH_DRV_Abort(void)
{
    volatile uint32_t MCRAddress;
    uint32_t MCRValue;
    volatile uint32_t clearFlag = 0x0U;
    uint32_t count = FLASH_ABORT_WAIT;    /* the loop count for the wait loop */

    MCRAddress = FLASH_C55_BASE + C55_MCR;
    MCRValue = FLASH_C55->MCR;

    if ((MCRValue & C55_MCR_ESUS) != 0x0U)
    {
        if ((MCRValue & C55_MCR_PSUS) != 0x0U)
        {
            clearFlag = 0x3U;
        }
        else
        {
            clearFlag = 0x2U;
        }
    }
    else
    {
        if ((MCRValue & C55_MCR_PSUS) != 0x0U)
        {
            clearFlag = 0x1U;
        }
    }

    if ((clearFlag & 0x1U) != 0x0U)  /* Program-suspended Erase-suspended or Program-suspended only */
    {
        /* Wait until MCR_DONE = 1 to program-suspended operation accomplish */
        while (((REG_READ32(MCRAddress) & C55_MCR_DONE) == 0x0U) && (count > 0U))
        {
            count--;
        }
        /* Set MCR_EHV bit then clear MCR_PSUS to resume program operation */
        REG_BIT_SET32(MCRAddress, C55_MCR_EHV);
        REG_BIT_CLEAR32(MCRAddress, C55_MCR_PSUS);
        /* workaround to avoid optimization on Marterhorn2 - C-array.
        If don't have redundant code to clear PSUS again, it will fail on MH2*/
        REG_BIT_CLEAR32(MCRAddress, C55_MCR_PSUS);
    }

    if ((clearFlag & 0x2U) != 0x0U)
    {
        /* Clear EHV to abort program operation and then clear MCR_PGM */
        REG_BIT_CLEAR32(MCRAddress, C55_MCR_EHV);
        /* Wait until MCR_DONE = 1 */
        while (((REG_READ32(MCRAddress) & C55_MCR_DONE) == 0x0U) && (count > 0U))
        {
            count--;
        }
        REG_BIT_CLEAR32(MCRAddress, C55_MCR_PGM);
        /* Set MCR_EHV bit then clear MCR_PSUS to resume erase operation */
        REG_BIT_SET32(MCRAddress, C55_MCR_EHV);
        REG_BIT_CLEAR32(MCRAddress, C55_MCR_ESUS);
        /* workaround to avoid optimization on Marterhorn2 - C-array.
        If don't have redundant code to clear PSUS again, it will fail on MH2*/
        REG_BIT_CLEAR32(MCRAddress, C55_MCR_ESUS);

    }

    /* Clear EHV bit to abort flash operation */
    REG_BIT_CLEAR32(MCRAddress, C55_MCR_EHV);
    /* Wait until MCR_DONE = 1 */
    while (((REG_READ32(MCRAddress) & C55_MCR_DONE) == 0x0U) && (count > 0U))
    {
        count--;
    }

    /* Clear PGM, ERS bit */
    REG_BIT_CLEAR32(MCRAddress, C55_MCR_PGM);
    REG_BIT_CLEAR32(MCRAddress, C55_MCR_ERS);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_GetOverPgmProtStatus
 * Description   : This function will return the over-program protection status
 * via main interface. This value shows blocks that are protected from being
 * over programmed.
 *
 * Implements    : FLASH_DRV_GetOverPgmProtStatus_Activity
 *END**************************************************************************/
status_t FLASH_DRV_GetOverPgmProtStatus(flash_address_space_t blkProtIndicator,
                                        uint32_t * blkProtState)
{
    DEV_ASSERT(blkProtIndicator <= C55_BLOCK_256K_SECOND);
    uint32_t value;                              /* value of block over program protection register */
    uint32_t mask;                               /* block locking bit mask for the address space */
    uint32_t temp;

    /* Determine which address space is selected */
    switch (blkProtIndicator)
    {
        case C55_BLOCK_LOW:
            value = FLASH_C55->OPP0;
            mask = (0xFFFFFFFFU >> (32U - FLASH_GetNumOfLowBlock())) << 16U;
            /* Mask off the other bits and make proper shift */
            *blkProtState = (value & mask) >> C55_OPP0_LOWOPP_SHIFT;
            break;
        case C55_BLOCK_MID:
            value = FLASH_C55->OPP0;
            mask = 0xFFFFFFFFU >> (32U - FLASH_GetNumOfMidBlock());
            *blkProtState = (value & mask) >> C55_OPP0_MIDOPP_SHIFT;
            break;
        case C55_BLOCK_HIGH:
            value = FLASH_C55->OPP1;
            mask = 0xFFFFFFFFU >> (32U - FLASH_GetNumOfHighBlock());
            *blkProtState = (value & mask) >> C55_OPP1_HIGHOPP_SHIFT;
            break;
        case C55_BLOCK_256K_FIRST:
            value = FLASH_C55->OPP2;
            temp = FLASH_GetNumOf256KBlock();
            if (temp >= 32U)
            {
                mask = 0xFFFFFFFFU;
            }
            else
            {
                mask = 0xFFFFFFFFU >> (32U - temp);
            }
            *blkProtState = (value & mask) >> C55_OPP2_A256KOPP_SHIFT;
            break;
        case C55_BLOCK_256K_SECOND:
            value = FLASH_C55->OPP3;
            temp = FLASH_GetNumOf256KBlock();
            if (temp > 32U)
            {
                mask = 0xFFFFFFFFU >> (64U - temp);
            }
            else
            {
                mask = 0x0U;
            }
            *blkProtState = (value & mask) >> C55_OPP3_A256KOPP_SHIFT;
            break;
        default: /* Nothing to do */
            break;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_GetFailedAddress
 * Description   : This function will get the failing address in the event of ECC
 * event error, Single Bit Correction, as well as providing the address of a
 * failure that may have occurred in a program/erase operation.
 * The address captured is from main array space or UTest array space.
 *
 * Implements    : FLASH_DRV_GetFailedAddress_Activity
 *END**************************************************************************/
uint32_t FLASH_DRV_GetFailedAddress(void)
{
    uint32_t addr = 0xFFFFFFFFU;
    uint32_t addrValue;
    uint32_t addrOffset;

    addrValue = FLASH_C55->ADR;
    addrOffset = (addrValue & C55_ADR_ADDR_MASK);

    if ((addrValue & C55_ADR_aH_MASK) != 0U)
    {
        if ((addrValue & C55_ADR_a16k_MASK) != 0U)
        {
            addr = addrOffset + FLASH_C55_16KB_HIGH_BASE_ADDRESS;
        }
        else if ((addrValue & C55_ADR_a32k_MASK) != 0U)
        {
            addr = addrOffset + FLASH_C55_32KB_HIGH_BASE_ADDRESS;
        }
        else if ((addrValue & C55_ADR_a64k_MASK) != 0U)
        {
            addr = addrOffset + FLASH_C55_64KB_HIGH_BASE_ADDRESS;
        }
        else
        {
            /* Do nothing */
        }
    }
    else if ((addrValue & C55_ADR_aM_MASK) != 0U)
    {
        if ((addrValue & C55_ADR_a16k_MASK) != 0U)
        {
            addr = addrOffset + FLASH_C55_16KB_MID_BASE_ADDRESS;
        }
        else if ((addrValue & C55_ADR_a32k_MASK) != 0U)
        {
            addr = addrOffset + FLASH_C55_32KB_MID_BASE_ADDRESS;
        }
        else if ((addrValue & C55_ADR_a64k_MASK) != 0U)
        {
            addr = addrOffset + FLASH_C55_64KB_MID_BASE_ADDRESS;
        }
        else
        {
            /* Do nothing */
        }
    }
    else if ((addrValue & C55_ADR_aL_MASK) != 0U)
    {
        if ((addrValue & C55_ADR_a16k_MASK) != 0U)
        {
            addr = addrOffset + FLASH_C55_16KB_LOW_BASE_ADDRESS;
        #if defined(FLASH_C55_16KB_SECOND_LOW_BASE_ADDRESS)
            if (addrOffset >= 0x8000U)
            {
                addr = addrOffset + FLASH_C55_16KB_SECOND_LOW_BASE_ADDRESS;
            }
        #endif
        }
        else if ((addrValue & C55_ADR_a32k_MASK) != 0U)
        {
            addr = addrOffset + FLASH_C55_32KB_LOW_BASE_ADDRESS;
        }
        else if ((addrValue & C55_ADR_a64k_MASK) != 0U)
        {
            addr = addrOffset + FLASH_C55_64KB_LOW_BASE_ADDRESS;
        }
        else
        {
            /* Do nothing */
        }
    }
    else if ((addrValue & C55_ADR_a256k_MASK) != 0U)
    {
        addr = addrOffset + FLASH_C55_256KB_BASE_ADDRESS;
    }
    else
    {
        /* Do nothing */
    }

    return addr;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_ArrayIntegrityCheck
 * Description   : This function will check the array integrity of the flash via
 * main interface. The user specified address sequence is used for array integrity
 * reads and the operation is done on the specified blocks. The MISR values calculated
 * by the hardware is compared to the values passed by the user, if they are not
 * the same, then an error code is returned.
 * In order to support asynchronous design, this function will store the necessary
 * information to 'pCtxData' (ex: user provided MISR value) and be terminated without
 * waiting for completion of this operation. User should call FLASH_DRV_CheckUserTestStatus
 * to check the on-going status of this function. And once finish, it will do comparison
 * between MISR values provided by user which is currently stored in 'pCtxData' and
 * MISR values generated by hardware and return an appropriate code according to
 * this compared result.
 *
 * Implements    : FLASH_DRV_ArrayIntegrityCheck_Activity
 *END**************************************************************************/
status_t FLASH_DRV_ArrayIntegrityCheck(flash_context_data_t * pCtxData,
                                       flash_block_select_t * blockSelect,
                                       flash_ai_break_option_t breakOption,
                                       flash_ai_sequence_option_t addrSeq,
                                       flash_misr_t * pMisrValue)
{
    DEV_ASSERT(pCtxData != NULL);
    DEV_ASSERT(pMisrValue != NULL);
    DEV_ASSERT(blockSelect != NULL);
    DEV_ASSERT((addrSeq == C55_AI_ADDR_SEQ_LINEAR) || (addrSeq == C55_AI_ADDR_SEQ_PROPRIETARY));
    status_t returnCode = STATUS_SUCCESS;        /* return code */
    uint32_t UT0Address;                         /* address of the C55_UT0 register */
    uint32_t UT0Value;                           /* content of the C55_UT0 register */
    uint32_t count;                              /* the loop count for the wait loop */
    uint32_t temp;

    UT0Address = FLASH_C55_BASE + C55_UT0;
    UT0Value = FLASH_C55->UT0;
    count = FLASH_USER_TEST_WAIT;

    /* There is an in-progress UTest operation */
    if ((UT0Value & C55_UT0_AIE) != 0x0U)
    {
        /* In case the previous check is broken, we allow to start a new check */
        if ((UT0Value & C55_UT0_NAIBP) != 0x0U)
        {
            /* Clear error bits MCR_EER, MCR_RWE, MCR_SBC */
            FLASH_C55->MCR = (FLASH_C55->MCR | C55_MCR_EER | C55_MCR_RWE | C55_MCR_SBC | \
                           C55_MCR_RVE | C55_MCR_RRE | C55_MCR_AEE | C55_MCR_EEE);
            /* Clear bits Array Integrity Break Point */
            REG_BIT_CLEAR32(UT0Address, C55_UT0_NAIBP);
            /* abort to start next operation */
            REG_BIT_CLEAR32(UT0Address, C55_UT0_AIE);
            while (((REG_READ32(UT0Address) & C55_UT0_AID) == 0x0U)&& (count > 0U))
            {
                count--;
            }

            /* Clear UT0_AIS, UT0_UTE */
            REG_BIT_CLEAR32(UT0Address, C55_UT0_AIS);
            REG_BIT_CLEAR32(UT0Address, C55_UT0_UTE);
        }
        else
        {
            returnCode = STATUS_BUSY;
        }
    }

    if (returnCode == STATUS_SUCCESS)
    {
        pCtxData->pMisr = pMisrValue;
        /* Mask off reserved bits for low address space */
        blockSelect->lowBlockSelect &= 0xFFFFFFFFU >> (32U - FLASH_GetNumOfLowBlock());
        /* Mask off reserved bits for mid address space */
        blockSelect->midBlockSelect &= 0xFFFFFFFFU >> (32U - FLASH_GetNumOfMidBlock());
        /* Mask off reserved bits for high address space */
        blockSelect->highBlockSelect &= 0xFFFFFFFFU >> (32U - FLASH_GetNumOfHighBlock());
        /* get the number of 256K blocks */
        temp = FLASH_GetNumOf256KBlock();

        /* Mask off reserved bits for 256K address space */
        if (temp <= 32U)
        {
            blockSelect->first256KBlockSelect &= 0xFFFFFFFFU >> (32U - temp);
            blockSelect->second256KBlockSelect &= 0U;
        }
        else
        {
            blockSelect->first256KBlockSelect &= 0xFFFFFFFFU;
            blockSelect->second256KBlockSelect &= 0xFFFFFFFFU >> (64U - temp);
        }

        if ((blockSelect->lowBlockSelect | blockSelect->midBlockSelect | blockSelect->highBlockSelect |
                blockSelect->first256KBlockSelect | blockSelect->second256KBlockSelect) == 0x0U)
        {
            /* No blocks to be selected for evaluation */
            returnCode = STATUS_FLASH_ERROR_NO_BLOCK;
        }
        else
        {
            /* Write password to UT0 register to enable test mode */
            FLASH_C55->UT0 = C55_USER_TEST_ENABLE_PASSWORD;
            UT0Value = FLASH_C55->UT0;
            if ((UT0Value & C55_UT0_UTE) == 0x0U)
            {
                returnCode = STATUS_FLASH_ERROR_ENABLE;
            }
            else
            {
                /* Set the block selection registers */
                FLASH_C55->SEL0 = ((blockSelect->lowBlockSelect << C55_SEL0_LOWSEL_SHIFT) | blockSelect->midBlockSelect);
                FLASH_C55->SEL1 = blockSelect->highBlockSelect;
                FLASH_C55->SEL2 = blockSelect->first256KBlockSelect;
                FLASH_C55->SEL3 = blockSelect->second256KBlockSelect;

                /* Set the Address sequence to be used */
                if (C55_AI_ADDR_SEQ_LINEAR == addrSeq)
                {
                    /* The logical sequence is used */
                    REG_BIT_SET32(UT0Address, C55_UT0_AIS);
                }
                else
                {
                    /* The proprietary sequence is used */
                    REG_BIT_CLEAR32(UT0Address, C55_UT0_AIS);
                }

                /* Set the breakpoint if need to */
                if ((C55_AI_BREAK_ON_DBD == breakOption) || (C55_AI_BREAK_ON_DBD_SBC == breakOption))
                {
                    /* Clear bits MCR_EER, MCR_SBC (notes: write 1 to clear) */
                    FLASH_C55->MCR = (FLASH_C55->MCR | C55_MCR_EER);
                    FLASH_C55->MCR = (FLASH_C55->MCR | C55_MCR_SBC);
                    /* Set bit UT0_AIBPE */
                    REG_BIT_SET32(UT0Address, C55_UT0_AIBPE);
                    /* If desires to break on SBC, set bit UT0_SBCE */
                    if (C55_AI_BREAK_ON_DBD_SBC == breakOption)
                    {
                        REG_BIT_SET32(UT0Address, C55_UT0_SBCE);
                    }
                    else
                    {
                        /* Clear SBCE bit if only double bit detection is selected */
                        REG_BIT_CLEAR32(UT0Address, C55_UT0_SBCE);
                    }
                }
                else
                {
                    /* Clear AIBPE bit if no break */
                    REG_BIT_CLEAR32(UT0Address, C55_UT0_AIBPE);
                    /* Clear SBCE bit if only double bit detection is selected */
                    REG_BIT_CLEAR32(UT0Address, C55_UT0_SBCE);

                }

                /* Set the seed values of the MISR in UMx registers */
                FLASH_C55->UM[0] = 0U;
                FLASH_C55->UM[1] = 0U;
                FLASH_C55->UM[2] = 0U;
                FLASH_C55->UM[3] = 0U;
                FLASH_C55->UM[4] = 0U;
                FLASH_C55->UM[5] = 0U;
                FLASH_C55->UM[6] = 0U;
                FLASH_C55->UM[7] = 0U;
                FLASH_C55->UM[8] = 0U;
                FLASH_C55->UM9 = 0U;
                /* Initiate the Array Integrity Check by writing 1 to UT0_AIE bit*/
                REG_BIT_SET32(UT0Address, C55_UT0_AIE);
                /* Return here, not wait until UT0_AID is set */
            }
        }
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_ArrayIntegritySuspend
 * Description   : This function will check if there is an on-going array integrity
 * check of the flash and suspend it via main interface.
 *
 * Implements    : FLASH_DRV_ArrayIntegritySuspend_Activity
 *END**************************************************************************/
status_t FLASH_DRV_ArrayIntegritySuspend(flash_state_t * suspendState)
{
    uint32_t UT0Address;                         /* address of C55_UT0 register */
    uint32_t UT0Value;                           /* content of C55_UT0 register */
    uint32_t count;                              /* the loop count for the wait loop */

    *suspendState = C55_SUS_NOTHING;
    count = FLASH_USER_TEST_WAIT;
    UT0Address = FLASH_C55_BASE + C55_UT0;
    UT0Value = FLASH_C55->UT0;

    /* There must be an UTest operation is in-progress */
    if (((UT0Value & C55_UT0_AIE) != 0x0U) && ((UT0Value & (C55_UT0_AISUS | C55_UT0_AID)) == 0x0U))
    {
        *suspendState = C55_USER_TEST_SUS;
        /* Set bit UT0_AISUS */
        REG_BIT_SET32(UT0Address, C55_UT0_AISUS);
        /* Wait the UT0_AID bit goes low */
        while (((REG_READ32(UT0Address) & C55_UT0_AID) == 0x0U) && (count > 0U))
        {
            count--;
        }
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_ArrayIntegrityResume
 * Description   : This function will check if there is an on-going array integrity
 * check of the flash being suspended and resume it via main interface.
 *
 * Implements    : FLASH_DRV_ArrayIntegrityResume_Activity
 *END**************************************************************************/
status_t FLASH_DRV_ArrayIntegrityResume(flash_state_t * resumeState)
{
    uint32_t UT0Address;                     /* address of C55_UT0 register */
    uint32_t UT0Value;                       /* content of C55_UT0 register */
    uint32_t count;                          /* the loop count for the wait loop */

    *resumeState = C55_RES_NOTHING;
    count = FLASH_USER_TEST_WAIT;
    UT0Address = FLASH_C55_BASE + C55_UT0;
    UT0Value = FLASH_C55->UT0;

    /* There must be an UTest operation */
    if ((UT0Value & C55_UT0_AIE) != 0x0U)
    {
        /* The operation is in suspended state */
        if ((UT0Value & (C55_UT0_AISUS | C55_UT0_AID)) == (C55_UT0_AISUS | C55_UT0_AID))
        {
            *resumeState = C55_RES_USER_TEST;
            /* Clear bit UT0_AISUS */
            REG_BIT_CLEAR32(UT0Address, C55_UT0_AISUS);
            /* Wait the UT0_AID bit goes low */
            while (((REG_READ32(UT0Address) & C55_UT0_AID) != 0x0U) && (count> 0U))
            {
                count--;
            }
        }
        else if ((UT0Value & C55_UT0_NAIBP) != 0x0U)
        {
            /* The operation is broken due to an error */
            *resumeState = C55_RES_USER_TEST;
            /* Clear bits MCR_EER, MCR_SBC (notes: write 1 to clear) */
            FLASH_C55->MCR = (FLASH_C55->MCR | C55_MCR_EER);
            FLASH_C55->MCR = (FLASH_C55->MCR | C55_MCR_SBC);

            /* Clear UT0_NAIBP to resume the operation */
            REG_BIT_CLEAR32(UT0Address, C55_UT0_NAIBP);
            /* Wait the UT0_AID bit goes low */
            while (((REG_READ32(UT0Address) & C55_UT0_AID) != 0x0U) && (count> 0U))
            {
                count--;
            }
        }
        else
        {
            /* Do nothing */
        }
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_UserMarginReadCheck
 * Description   : This function will check the user margin reads of the flash
 * via main interface. The user specified margin level is used for reads and the
 * operation is done on the specified blocks. The MISR values calculated by
 * the hardware are compared to the values passed by the user, if they are not
 * the same, then an error code is returned.
 * In order to support asynchronous design, this function will store the necessary
 * information to 'pCtxData' (example: user provided MISR value) and be terminated without
 * waiting for completion of this operation. User should call FLASH_DRV_CheckUserTestStatus
 * to check the on-going status of this function. And once finish, it will do comparison
 * between MISR values provided by user which are currently stored in 'pCtxData'
 * and MISR values generated by hardware and return an appropriate code according to
 * this compared result.
 *
 * Implements    : FLASH_DRV_UserMarginReadCheck_Activity
 *END**************************************************************************/
status_t FLASH_DRV_UserMarginReadCheck(flash_context_data_t * pCtxData,
                                       flash_block_select_t * blockSelect,
                                       flash_ai_break_option_t breakOption,
                                       flash_margin_option_t marginLevel,
                                       flash_misr_t * pMisrValue)
{
    DEV_ASSERT(pCtxData != NULL);
    DEV_ASSERT(pMisrValue != NULL);
    DEV_ASSERT(blockSelect != NULL);
    DEV_ASSERT( (marginLevel == C55_MARGIN_LEVEL_ERASE) || (marginLevel == C55_MARGIN_LEVEL_PROGRAM) );
    status_t returnCode = STATUS_SUCCESS;          /* return code */
    uint32_t UT0Address;                           /* address of the C55_UT0 register */
    uint32_t UT0Value;                             /* content of the C55_UT0 register */
    uint32_t count;                                /* the loop count for the wait loop */
    uint32_t temp;

    UT0Address = FLASH_C55_BASE + C55_UT0;
    UT0Value = FLASH_C55->UT0;
    count = FLASH_USER_TEST_WAIT;

    /* There is an in-progress UTest operation */
    if (( UT0Value & C55_UT0_AIE ) != 0x0U)
    {
        /* In case the previous check is broken, we allow to start a new check */
        if ((UT0Value & C55_UT0_NAIBP ) != 0x0U)
        {
            /* Clear error bits MCR_EER, MCR_RWE, MCR_SBC */
            FLASH_C55->MCR = (FLASH_C55->MCR | C55_MCR_EER | C55_MCR_RWE | C55_MCR_SBC | \
                           C55_MCR_RVE | C55_MCR_RRE | C55_MCR_AEE | C55_MCR_EEE);
            /* Clear bits Array Integrity Break Point */
            REG_BIT_CLEAR32(UT0Address,C55_UT0_NAIBP);
            /* abort to start next operation */
            REG_BIT_CLEAR32(UT0Address, C55_UT0_AIE);
            while (((REG_READ32(UT0Address) & C55_UT0_AID) == 0x0U) && (count > 0U))
            {
                count--;
            }
            /* Clear UT0_AIS, UT0_UTE, UT0_MRE */
            REG_BIT_CLEAR32(UT0Address, C55_UT0_AIS | C55_UT0_MRE);
            REG_BIT_CLEAR32(UT0Address, C55_UT0_UTE);
        }
        else
        {
            returnCode = STATUS_BUSY;
        }
    }

    if (returnCode == STATUS_SUCCESS)
    {
        pCtxData->pMisr = pMisrValue;
        /* Mask off reserved bits for low address space */
        blockSelect->lowBlockSelect &= 0xFFFFFFFFU >> (32U - FLASH_GetNumOfLowBlock());
        /* Mask off reserved bits for mid address space */
        blockSelect->midBlockSelect &= 0xFFFFFFFFU >> (32U - FLASH_GetNumOfMidBlock());
        /* Mask off reserved bits for high address space */
        blockSelect->highBlockSelect &= 0xFFFFFFFFU >> (32U - FLASH_GetNumOfHighBlock());
        /* get the number of 256K blocks */
        temp = FLASH_GetNumOf256KBlock();

        /* Mask off reserved bits for 256K address space */
        if (temp <= 32U)
        {
            blockSelect->first256KBlockSelect &= 0xFFFFFFFFU >> (32U - temp);
            blockSelect->second256KBlockSelect &= 0U;
        }
        else
        {
            blockSelect->first256KBlockSelect &= 0xFFFFFFFFU;
            blockSelect->second256KBlockSelect &= 0xFFFFFFFFU >> (64U - temp);
        }

        if ((blockSelect->lowBlockSelect | blockSelect->midBlockSelect | blockSelect->highBlockSelect |
                blockSelect->first256KBlockSelect | blockSelect->second256KBlockSelect) == 0x0U)
        {
            /* No blocks to be selected for evaluation */
            returnCode = STATUS_FLASH_ERROR_NO_BLOCK;
        }
        else
        {
            /* Write password to UT0 register to enable test mode */
            FLASH_C55->UT0 = C55_USER_TEST_ENABLE_PASSWORD;
            UT0Value = FLASH_C55->UT0;
            if ((UT0Value & C55_UT0_UTE) == 0x0U)
            {
                returnCode = STATUS_FLASH_ERROR_ENABLE;
            }
            else
            {
                /* Set the block selection registers */
                FLASH_C55->SEL0 = ((blockSelect->lowBlockSelect << C55_SEL0_LOWSEL_SHIFT) | blockSelect->midBlockSelect);
                FLASH_C55->SEL1 = blockSelect->highBlockSelect;
                FLASH_C55->SEL2 = blockSelect->first256KBlockSelect;
                FLASH_C55->SEL3 = blockSelect->second256KBlockSelect;
                /* Set the Address sequence to be used: linear sequence */
                REG_BIT_SET32(UT0Address, C55_UT0_AIS);

                /* Set the breakpoint if need to */
                if ((C55_AI_BREAK_ON_DBD == breakOption) || (C55_AI_BREAK_ON_DBD_SBC == breakOption))
                {
                    /* Clear bits MCR_EER, MCR_SBC (notes: write 1 to clear) */
                    FLASH_C55->MCR = (FLASH_C55->MCR | C55_MCR_EER);
                    FLASH_C55->MCR = (FLASH_C55->MCR | C55_MCR_SBC);
                    /* Set bit UT0_AIBPE */
                    REG_BIT_SET32(UT0Address, C55_UT0_AIBPE);
                    /* If desires to break on SBC, set bit UT0_SBCE */
                    if (C55_AI_BREAK_ON_DBD_SBC == breakOption)
                    {
                        REG_BIT_SET32(UT0Address, C55_UT0_SBCE);
                    }
                    else
                    {
                        /* Clear SBCE bit if only double bit detection is selected */
                        REG_BIT_CLEAR32(UT0Address, C55_UT0_SBCE);
                    }
                }
                else
                {
                    /* Clear AIBPE bit if no break */
                    REG_BIT_CLEAR32(UT0Address, C55_UT0_AIBPE);
                    /* Clear SBCE bit if only double bit detection is selected */
                    REG_BIT_CLEAR32(UT0Address, C55_UT0_SBCE);
                }

                /* Set UT0_MRE to enable User Margin Read */
                REG_BIT_SET32(UT0Address , C55_UT0_MRE);

                /* Set the Margin level to be used */
                if (C55_MARGIN_LEVEL_ERASE == marginLevel)
                {
                    /* The erased level is used */
                    REG_BIT_SET32(UT0Address, C55_UT0_MRV);
                }
                else
                {
                    /* The programmed level is used */
                    REG_BIT_CLEAR32(UT0Address, C55_UT0_MRV);
                }

                /* Set the seed values of the MISR in UMx registers */
                FLASH_C55->UM[0] = 0U;
                FLASH_C55->UM[1] = 0U;
                FLASH_C55->UM[2] = 0U;
                FLASH_C55->UM[3] = 0U;
                FLASH_C55->UM[4] = 0U;
                FLASH_C55->UM[5] = 0U;
                FLASH_C55->UM[6] = 0U;
                FLASH_C55->UM[7] = 0U;
                FLASH_C55->UM[8] = 0U;
                FLASH_C55->UM9 = 0U;
                /* Initiate the Array Integrity Check by writing 1 to UT0_AIE bit*/
                REG_BIT_SET32(UT0Address, C55_UT0_AIE);
                /* Return here, not wait until UT0_AID is set */
            }
        }
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EnableCmdCompleteInterupt
 * Description   : Enable the command complete interrupt is generated when
 * an program erase complete or suspend program, erase. User must enable the flash
 * interrupt in interrupt controller when uses this function.
 *
 * Implements    : FLASH_DRV_EnableCmdCompleteInterupt_Activity
 *END**************************************************************************/
status_t FLASH_DRV_EnableCmdCompleteInterupt(void)
{
    /* Enable the command complete interrupt */
    FLASH_C55->MCR |= C55_MCR_PECIE;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_DisableCmdCompleteInterupt
 * Description   : Disable the command program, erase or suspend complete interrupt.
 *
 * Implements    : FLASH_DRV_DisableCmdCompleteInterupt_Activity
 *END**************************************************************************/
void FLASH_DRV_DisableCmdCompleteInterupt(void)
{
    /* Disable the command complete interrupt */
    FLASH_C55->MCR &= (uint32_t)(~C55_MCR_PECIE);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
