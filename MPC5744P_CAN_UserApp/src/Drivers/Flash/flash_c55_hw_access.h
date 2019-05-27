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
 * @file flash_c55_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * This is required to enable the use of a macro needed by
 * the user code (even if the macro is not used inside the flash driver code)
 * or driver code in the future.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro defined.
 * This macro is needed in creating a common name for any IP.
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
 */

#ifndef FLASH_C55_HW_ACCESS_H
#define FLASH_C55_HW_ACCESS_H

#include <stdbool.h>
#include <stddef.h>

//#include "common/s32_core_e200.h"
//#include "devassert.h"
/* Register definitions */
#include "Drivers/System/MPC5744P.h"
/* CPU specific feature definitions */
#include "Drivers/System/MPC5744P_features.h"

//#define MPC5744P_SERIES

/*!
 * flash_c55_hw_access Special hardware of C55 module
 * @details This section describes some access into register of hardware.
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*************************************************************************/
/*                  Offsets of C55 Control Registers                     */
/*************************************************************************/
#define C55_MCR             0x0000U       /*!< Module Configuration Register */
#define C55_MCRE            0x0008U       /*!< Extended Module Configuration Register */
#define C55_LOCK0           0x0010U       /*!< Lock 0 Register */
#define C55_LOCK1           0x0014U       /*!< Lock 1 Register */
#define C55_LOCK2           0x0018U       /*!< Lock 2 Register */
#define C55_LOCK3           0x001CU       /*!< Lock 3 Register */
#define C55_SEL0            0x0038U       /*!< Select 0 Register */
#define C55_SEL1            0x003CU       /*!< Select 1 Register */
#define C55_SEL2            0x0040U       /*!< Select 2 Register */
#define C55_SEL3            0x0044U       /*!< Select 3 Register */
#define C55_ADR             0x0050U       /*!< Address Register */
#define C55_UT0             0x0054U       /*!< User Test 0 register */
#define C55_UM0             0x0058U       /*!< User MISR 0 Register */
#define C55_UM1             0x005CU       /*!< User MISR 1 Register */
#define C55_UM2             0x0060U       /*!< User MISR 2 Register */
#define C55_UM3             0x0064U       /*!< User MISR 3 Register */
#define C55_UM4             0x0068U       /*!< User MISR 4 Register */
#define C55_UM5             0x006CU       /*!< User MISR 5 Register */
#define C55_UM6             0x0070U       /*!< User MISR 6 Register */
#define C55_UM7             0x0074U       /*!< User MISR 7 Register */
#define C55_UM8             0x0078U       /*!< User MISR 8 Register */
#define C55_UM9             0x007CU       /*!< User MISR 9 Register */
#define C55_OPP0            0x0080U       /*!< Over-Program Protection 0 Register */
#define C55_OPP1            0x0084U       /*!< Over-Program Protection 1 Register */
#define C55_OPP2            0x0088U       /*!< Over-Program Protection 2 Register */
#define C55_OPP3            0x008CU       /*!< Over-Program Protection 3 Register */

#ifdef C55FMC
/*!
 * @name Bit map for Module Configuration Register
 * @{
 */
#define FLASH_C55_BASE                    C55FMC_BASE                /*!< Base of register in flash c55 module */
#define FLASH_C55                         C55FMC                     /*!< Base pointer of flash peripheral */
#define C55_MCR_RVE                       C55FMC_MCR_RVE_MASK        /*!< Read Voltage Error */
#define C55_MCR_RRE                       C55FMC_MCR_RRE_MASK        /*!< Read Reference Error */
#define C55_MCR_AEE                       C55FMC_MCR_AEE_MASK        /*!< Address Encode Error */
#define C55_MCR_EEE                       C55FMC_MCR_EEE_MASK        /*!< ECC after ECC Error */
#define C55_MCR_EER                       C55FMC_MCR_EER_MASK        /*!< ECC Event Error */
#define C55_MCR_RWE                       C55FMC_MCR_RWE_MASK        /*!< Read While Write Event Error */
#define C55_MCR_SBC                       C55FMC_MCR_SBC_MASK        /*!< Single Bit Correction Error */
#define C55_MCR_PEAS                      C55FMC_MCR_PEAS_MASK       /*!< Program/Erase Access Space */
#define C55_MCR_DONE                      C55FMC_MCR_DONE_MASK       /*!< State Machine Status */
#define C55_MCR_PEG                       C55FMC_MCR_PEG_MASK        /*!< Program/Erase Good */
#define C55_MCR_PECIE                     C55FMC_MCR_PECIE_MASK      /*!< Program/Erase Complete Interrupt Enable */
#define C55_MCR_PGM                       C55FMC_MCR_PGM_MASK        /*!< Program */
#define C55_MCR_PSUS                      C55FMC_MCR_PSUS_MASK       /*!< Program Suspend */
#define C55_MCR_ERS                       C55FMC_MCR_ERS_MASK        /*!< Erase */
#define C55_MCR_ESUS                      C55FMC_MCR_ESUS_MASK       /*!< Erase Suspend */
#define C55_MCR_EHV                       C55FMC_MCR_EHV_MASK        /*!< Enable High Voltage */
/*@}*/

/* LOCK0 Bit Fields */
#define C55_LOCK0_MIDLOCK_MASK            C55FMC_LOCK0_MIDLOCK_MASK
#define C55_LOCK0_MIDLOCK_SHIFT           C55FMC_LOCK0_MIDLOCK_SHIFT
#define C55_LOCK0_LOWLOCK_MASK            C55FMC_LOCK0_LOWLOCK_MASK
#define C55_LOCK0_LOWLOCK_SHIFT           C55FMC_LOCK0_LOWLOCK_SHIFT
#define C55_LOCK0_TSLOCK_MASK             C55FMC_LOCK0_TSLOCK_MASK
#define C55_LOCK0_TSLOCK_SHIFT            C55FMC_LOCK0_TSLOCK_SHIFT
/* LOCK1 Bit Fields */
#define C55_LOCK1_HIGHLOCK_MASK           C55FMC_LOCK1_HIGHLOCK_MASK
#define C55_LOCK1_HIGHLOCK_SHIFT          C55FMC_LOCK1_HIGHLOCK_SHIFT
/* LOCK2 Bit Fields */
#define C55_LOCK2_A256KLOCK_MASK          C55FMC_LOCK2_A256KLOCK_MASK
#define C55_LOCK2_A256KLOCK_SHIFT         C55FMC_LOCK2_A256KLOCK_SHIFT
/* LOCK3 Bit Fields */
#define C55_LOCK3_A256KLOCK_MASK          C55FMC_LOCK3_A256KLOCK_MASK
#define C55_LOCK3_A256KLOCK_SHIFT         C55FMC_LOCK3_A256KLOCK_SHIFT
/* SEL0 Bit Fields */
#define C55_SEL0_MIDSEL_MASK              C55FMC_SEL0_MIDSEL_MASK
#define C55_SEL0_MIDSEL_SHIFT             C55FMC_SEL0_MIDSEL_SHIFT
#define C55_SEL0_LOWSEL_MASK              C55FMC_SEL0_LOWSEL_MASK
#define C55_SEL0_LOWSEL_SHIFT             C55FMC_SEL0_LOWSEL_SHIFT
/* SEL1 Bit Fields */
#define C55_SEL1_HIGHSEL_MASK             C55FMC_SEL1_HIGHSEL_MASK
#define C55_SEL1_HIGHSEL_SHIFT            C55FMC_SEL1_HIGHSEL_SHIFT
/* SEL2 Bit Fields */
#define C55_SEL2_A256KSEL_MASK            C55FMC_SEL2_A256KSEL_MASK
#define C55_SEL2_A256KSEL_SHIFT           C55FMC_SEL2_A256KSEL_SHIFT
/* SEL3 Bit Fields */
#define C55_SEL3_A256KSEL_MASK            C55FMC_SEL3_A256KSEL_MASK
#define C55_SEL3_A256KSEL_SHIFT           C55FMC_SEL3_A256KSEL_SHIFT
/* OPP0 Bit Fields */
#define C55_OPP0_MIDOPP_MASK              C55FMC_OPP0_MIDOPP_MASK
#define C55_OPP0_MIDOPP_SHIFT             C55FMC_OPP0_MIDOPP_SHIFT
#define C55_OPP0_LOWOPP_MASK              C55FMC_OPP0_LOWOPP_MASK
#define C55_OPP0_LOWOPP_SHIFT             C55FMC_OPP0_LOWOPP_SHIFT
/* OPP1 Bit Fields */
#define C55_OPP1_HIGHOPP_MASK             C55FMC_OPP1_HIGHOPP_MASK
#define C55_OPP1_HIGHOPP_SHIFT            C55FMC_OPP1_HIGHOPP_SHIFT
/* OPP2 Bit Fields */
#define C55_OPP2_A256KOPP_MASK            C55FMC_OPP2_A256KOPP_MASK
#define C55_OPP2_A256KOPP_SHIFT           C55FMC_OPP2_A256KOPP_SHIFT
/* OPP3 Bit Fields */
#define C55_OPP3_A256KOPP_MASK            C55FMC_OPP3_A256KOPP_MASK
#define C55_OPP3_A256KOPP_SHIFT           C55FMC_OPP3_A256KOPP_SHIFT

/* ADR Bit Fields */
#define C55_ADR_ADDR_MASK                 C55FMC_ADR_ADDR_MASK
#define C55_ADR_a16k_MASK                 C55FMC_ADR_a16k_MASK
#define C55_ADR_a32k_MASK                 C55FMC_ADR_a32k_MASK
#define C55_ADR_a64k_MASK                 C55FMC_ADR_a64k_MASK
#define C55_ADR_a256k_MASK                C55FMC_ADR_a256k_MASK
#define C55_ADR_aL_MASK                   C55FMC_ADR_aL_MASK
#define C55_ADR_aM_MASK                   C55FMC_ADR_aM_MASK
#define C55_ADR_aH_MASK                   C55FMC_ADR_aH_MASK
#define C55_ADR_SAD_MASK                  C55FMC_ADR_SAD_MASK


/*!
 * @name Bit map for User Test 0 Register
 * @{
 */
#define C55_UT0_UTE                       C55FMC_UT0_UTE_MASK        /*!< U-Test Enable */
#define C55_UT0_SBCE                      C55FMC_UT0_SBCE_MASK       /*!< Single Bit Correction Enable */
#define C55_UT0_NAIBP                     C55FMC_UT0_NAIBP_MASK      /*!< Next Array Integrity Break Point */
#define C55_UT0_AIBPE                     C55FMC_UT0_AIBPE_MASK      /*!< Array Integrity Break Point Enable */
#define C55_UT0_AISUS                     C55FMC_UT0_AISUS_MASK      /*!< Array Integrity Suspend */
#define C55_UT0_MRE                       C55FMC_UT0_MRE_MASK        /*!< Margin Read Enable */
#define C55_UT0_MRV                       C55FMC_UT0_MRV_MASK        /*!< Margin Read Value */
#define C55_UT0_AIS                       C55FMC_UT0_AIS_MASK        /*!< Array Integrity Sequence */
#define C55_UT0_AIE                       C55FMC_UT0_AIE_MASK        /*!< Array Integrity Enable */
#define C55_UT0_AID                       C55FMC_UT0_AID_MASK        /*!< Array Integrity Done */
/*@}*/
#elif defined(C55MP)

#else

#endif

/*! @brief get the value of bit mask in register */
#define FLASH_C55_GET_REG(reg, mask, shift) ((reg & mask) >> shift)

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name FLASH HARDWARE ACCESS API
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif


/*!
 * @brief Get the number of low blocks in flash memory.
 *
 */
static inline uint32_t FLASH_GetNumOfLowBlock(void)
{
    uint32_t retValue;

    retValue = FLASH_C55_GET_REG(FLASH_C55->MCRE, C55FMC_MCRE_n16Kl_MASK, C55FMC_MCRE_n16Kl_SHIFT);
    retValue += FLASH_C55_GET_REG(FLASH_C55->MCRE, C55FMC_MCRE_n32Kl_MASK, C55FMC_MCRE_n32Kl_SHIFT);
    retValue += FLASH_C55_GET_REG(FLASH_C55->MCRE, C55FMC_MCRE_n64Kl_MASK, C55FMC_MCRE_n64Kl_SHIFT);
    retValue = retValue << 1U;

    return retValue;
}

/*!
 * @brief Get the number of mid blocks in flash memory.
 *
 */
static inline uint32_t FLASH_GetNumOfMidBlock(void)
{
    uint32_t retValue;

    retValue = FLASH_C55_GET_REG(FLASH_C55->MCRE, C55FMC_MCRE_n16Km_MASK, C55FMC_MCRE_n16Km_SHIFT);
    retValue += FLASH_C55_GET_REG(FLASH_C55->MCRE, C55FMC_MCRE_n32Km_MASK, C55FMC_MCRE_n32Km_SHIFT);
    retValue += FLASH_C55_GET_REG(FLASH_C55->MCRE, C55FMC_MCRE_n64Km_MASK, C55FMC_MCRE_n64Km_SHIFT);
    retValue = retValue << 1U;

    return retValue;
}

/*!
 * @brief Get the number of high blocks in flash memory.
 *
 */
static inline uint32_t FLASH_GetNumOfHighBlock(void)
{
    uint32_t retValue;

    retValue = FLASH_C55_GET_REG(FLASH_C55->MCRE, C55FMC_MCRE_n16Kh_MASK, C55FMC_MCRE_n16Kh_SHIFT);
    retValue += FLASH_C55_GET_REG(FLASH_C55->MCRE, C55FMC_MCRE_n32Kh_MASK, C55FMC_MCRE_n32Kh_SHIFT);
    retValue += FLASH_C55_GET_REG(FLASH_C55->MCRE, C55FMC_MCRE_n64Kh_MASK, C55FMC_MCRE_n64Kh_SHIFT);
    retValue = retValue << 1U;

    return retValue;
}

/*!
 * @brief Get the number of 256K blocks in flash memory.
 *
 */
static inline uint32_t FLASH_GetNumOf256KBlock(void)
{
    return (FLASH_C55_GET_REG(FLASH_C55->MCRE, C55FMC_MCRE_n256K_MASK, C55FMC_MCRE_n256K_SHIFT) << 1U);
}

/*! @} */
#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* FLASH_C55_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
