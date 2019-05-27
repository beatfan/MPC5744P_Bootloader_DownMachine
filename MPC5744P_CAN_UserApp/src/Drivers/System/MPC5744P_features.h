/*
** ###################################################################
**
**     Abstract:
**         Chip specific module features.
**
**     Copyright (c) 2015 Freescale Semiconductor, Inc.
**     Copyright 2016-2018 NXP
**     All rights reserved.
**
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
**
**
** ###################################################################
*/

/*!
 * @file MPC5744P_features.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * Type used only in some modules of the SDK.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macros defined are used to define features for each driver, so this might be reported
 * when the analysis is made only on one driver.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro
 * These are very simple macros used for abstracting hw implementation.
 * They help make the code easy to understand.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.1, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.2, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.4, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.5, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 */

#if !defined(MPC5744P_FEATURES_H)
#define MPC5744P_FEATURES_H

/* include device_registers.h for SIUL2 module configuration structures */
//#include "device_registers.h"

/* ERRATA sections*/

/* @brief e6358: ENET: Write to Transmit Descriptor Active Register (ENET_TDAR) is ignored */
#define ERRATA_E6358

/* @brief E7204: SENT: Number of Expected Edges Error status flag spuriously set when
 * operating with Option 1 of the Successive Calibration Check method */
#define ERRATA_E7204

/* @brief E7425: SENT: Unexpected NUM_EDGES_ERR error in certain conditions when
 * message has a pause pulse */
#define ERRATA_E7425

/* @brief E9595 FlexCAN: Corrupted frame possible if Freeze Mode or Low Power Mode are
 * entered during a Bus-Off state. if the Freeze Enable bit (FRZ) of the Module Configuration
 * Register (MCR) is asserted and the Freeze Mode is requested by asserting the Halt bit (HALT) of
 * the MCR register during the Bus Off state, the transmission after exiting the Bus-Off condition
 * will be corrupted. The issue occurs only if a transmission is pending before the freeze mode request.
 * This errata applies to mask 1N15P for MPC5744P CPU.
 */
#define ERRATA_E9595

/* @brief E9527 FlexCAN: The transmission abort mechanism may not work properly
 * The Flexible Controller Area NEtwork (FlexCAN) is not able to abort a transmission frame and
 * the abort process may remain pending in the following cases:
 * a) If a pending abort request occurs while the FlexCAN is receiving a remote frame.
 * b) When a frame is aborted during an overload frame after a frame reception.
 * c) When an abort is requested while the FlexCAN has just started a transmission.
 * d) When Freeze Mode request occurs and the FlexCAN has just started a transmission.
 * This errata applies to mask 1N15P for MPC574xP CPU.
 */
#define ERRATA_E9527

/* Mode Entry Module features */

/* @brief Key and inverted key values so write access to MCTL register is permitted */
#define FEATURE_MC_ME_KEY           (0x5AF0U)
#define FEATURE_MC_ME_KEY_INV       (0xA50FU)
/* @brief Mode entry has TEST mode */
#define FEATURE_MC_ME_HAS_TEST_MODE (1U)
/* @brief Mode entry has HALT mode */
#define FEATURE_MC_ME_HAS_HALT_MODE (1U)
/* @brief Mode entry has STOP0 mode */
#define FEATURE_MC_ME_HAS_STOP0_MODE (1U)
/* @brief Mode entry has STANDBY mode */
#define FEATURE_MC_ME_HAS_STANDBY_MODE (0U)
/* @brief Mode entry allow to control flash bit field*/
#define FEATURE_MC_ME_HAS_FLAON_CONFIG (1U)
/* @brief Mode entry has FLASH in low power mode */
#define FEATURE_MC_ME_HAS_FLAON_LP_MODE (1U)
/* @brief Enable or disable SDPLL clock */
#define FEATURE_HAS_SDPLL_CLK_CONFIG (0U)
/* @brief Number of cores. */
#define NUMBER_OF_CORES (1u)

/* WDOG module features */

/* @brief The key values to clear Soft Lock bit */
#define FEATURE_WDOG_UNLOCK_VALUE1  (0xC520U)
#define FEATURE_WDOG_UNLOCK_VALUE2  (0xD928U)

/* PIT module features */

/*! @brief PIT module has RTI channel */
#define FEATURE_PIT_HAS_RTI_CHANNEL (0U)
/*! @brief Number of interrupt vector for channels of the PIT module */
#define FEATURE_PIT_HAS_NUM_IRQS_CHANS (PIT_IRQS_CH_COUNT)
/*! @brief PIT module has a peculiar instance */
#define FEATURE_PIT_HAS_PECULIAR_INSTANCE   (0U)
/*! @brief The default value of MDIS bit */
#define PIT_MCR_MDIS_DEFAULT                (0U)

#if FEATURE_PIT_HAS_PECULIAR_INSTANCE
/*! @brief The peculiar instance */
#define PIT_PECULIAR_INSTANCE    (1U)
/*! @brief The number channel of the peculiar instance */
#define PIT_CHAN_NUM_OF_PECULIAR_INSTANCE   (2U)
#endif

#if FEATURE_PIT_HAS_RTI_CHANNEL
/*! @brief The default value of MDIS-RTI bit */
#define PIT_MCR_MDIS_RTI_DEFAULT            (0U)
/*! @brief Clock names for RTI. */
#define RTI_CLOCK_NAMES    {FXOSC_CLK}
#endif
/*! @brief Clock names for PIT. */
#define PIT_CLOCK_NAMES    {PITRTI0_CLK}

/* STM module features */

/*! @brief STM module CR register has CSL bit-field */
#define FEATURE_STM_HAS_CLOCK_SELECTION (0U)

/* SWI2C features */
#define SWI2C_INSTANCE_COUNT                   0xFFU

/* SWT module features */

/* @brief Support service mode watchpoint input */
#define FEATURE_SWT_SUPPORT_WATCHPOINT      (0U)
/* @brief Support for running in stop mode */
#define FEATURE_SWT_HAS_STOP_MODE           (1U)
/* @brief The key values to clear Soft Lock bit */
#define FEATURE_SWT_UNLOCK_VALUE1           (0xC520U)
#define FEATURE_SWT_UNLOCK_VALUE2           (0xD928U)
/* @brief The key values used for resetting the SWT counter in Fixed Service Sequence Mode */
#define FEATURE_SWT_FIXED_SERVICE_VALUE1    (0xA602U)
#define FEATURE_SWT_FIXED_SERVICE_VALUE2    (0xB480U)
/* @brief The reset value of the control register */
#define FEATURE_SWT_CR_RESET_VALUE          (0xFF00011AU)
/* @brief The reset value of the timeout register */
#define FEATURE_SWT_TO_RESET_VALUE          (0x3FDE0U)
/* @brief The reset value of the window register */
#define FEATURE_SWT_WN_RESET_VALUE          (0x0U)
/* @brief The reset value of the service key register */
#define FEATURE_SWT_SK_RESET_VALUE          (0x0U)
/* @brief The minimum timeout value */
#define FEATURE_SWT_TO_MINIMUM_VALUE        (0x100U)
/* @brief Sets the master access protection field */
#define FEATURE_SWT_MAP_MASK                (0xFF000000U)
#define FEATURE_SWT_MAP_SHIFT               (24U)
#define FEATURE_SWT_MAP(x)                  (((uint32_t)(((uint32_t)(x)) << FEATURE_SWT_MAP_SHIFT)) & FEATURE_SWT_MAP_MASK)

/* WKPU module features */

/* @brief WKPU core source. */
typedef enum
{
    WKPU_CORE0    = 0U    /*!< Core 0 */
} wkpu_core_t;

/* @brief The WKPU core array */
#define FEATURE_WKPU_CORE_ARRAY     \
{                                   \
    WKPU_CORE0    /*!< Core 0 */    \
}

/*! @brief The number core support for WKPU module */
#define FEATURE_WKPU_NMI_NUM_CORES                      (1U)
/*! @brief The distance between cores */
#define FEATURE_WKPU_CORE_OFFSET_SIZE                   (8U)
/*! @brief WKPU support non-maskable interrupt */
#define FEATURE_WKPU_SUPPORT_NON_MASK_INT               (1U)
/*! @brief WKPU support critical interrupt */
#define FEATURE_WKPU_SUPPORT_CRITICAL_INT               (2U)
/*! @brief WKPU support machine check request interrupt */
#define FEATURE_WKPU_SUPPORT_MACHINE_CHK_REQ            (3U)

/* CRC module features */

/* @brief CRC module use for C55. */
#define FEATURE_CRC_DRIVER_HARD_POLYNOMIAL
/* @brief Default CRC read transpose */
#define FEATURE_CRC_DEFAULT_READ_TRANSPOSE      false
/* @brief Default CRC bit mode polynomial */
#define FEATURE_CRC_DEFAULT_POLYNOMIAL          CRC_BITS_16_CCITT
/* @brief Default CRC write transpose */
#define FEATURE_CRC_DEFAULT_WRITE_TRANSPOSE     CRC_TRANSPOSE_BITS
/* @brief Default seed value is 0xFFFFU */
#define FEATURE_CRC_DEFAULT_SEED                (0xFFFFU)

/* SMPU module features */

/* @brief Specifies the SMPU hardware and definition revision level */
#define FEATURE_SMPU_HARDWARE_REVISION_LEVEL (1U)
/* @brief Specifies the SMPU has process identifier feature */
#define FEATURE_SMPU_HAS_PROCESS_IDENTIFIER  (0U)
/* @brief Specifies the SMPU has owner lock feature */
#define FEATURE_SMPU_HAS_OWNER_LOCK          (0U)
/* @brief Specifies the SMPU has specific access feature */
#define FEATURE_SMPU_HAS_SPECIFIC_ACCESS_RIGHT_COUNT (0U)
/* @brief Specifies the end address reset value */
#define FEATURE_SMPU_END_ADDRESS_RESET_VALUE (0U)
/* @brief Specifies the highest bus master */
#define FEATURE_SMPU_MAX_MASTER_NUMBER       (15U)
/* @brief Specifies total number of bus masters */
#define FEATURE_SMPU_MASTER_COUNT            (6U)
/* @brief The SMPU Logical Bus Master Number for Core Z4d_0 master */
#define FEATURE_SMPU_MASTER_CORE_Z4D_0       (0U)
/* @brief The SMPU Logical Bus Master Number for Direct Memory Access master */
#define FEATURE_SMPU_MASTER_DMA              (2U)
/* @brief The SMPU Logical Bus Master Number for FlexRay master */
#define FEATURE_SMPU_MASTER_FLEXRAY          (3U)
/* @brief The SMPU Logical Bus Master Number for Serial Interprocessor Interface master */
#define FEATURE_SMPU_MASTER_SIPI             (4U)
/* @brief The SMPU Logical Bus Master Number for Ethernet 0 master */
#define FEATURE_SMPU_MASTER_ENET             (5U)
/* @brief The SMPU Logical Bus Master Number for z4d_0 Core Nexus master */
#define FEATURE_SMPU_MASTER_NEXUS_Z4D_0      (8U)
/* @brief The SMPU Logical Bus Masters */
#define FEATURE_SMPU_MASTER                        \
{                                                  \
FEATURE_SMPU_MASTER_CORE_Z4D_0,  /* CORE Z4D_0 */  \
FEATURE_SMPU_MASTER_DMA,         /* DMA */         \
FEATURE_SMPU_MASTER_FLEXRAY,     /* FLEXRAY */     \
FEATURE_SMPU_MASTER_SIPI,        /* SIPI */        \
FEATURE_SMPU_MASTER_ENET,        /* ENET */        \
FEATURE_SMPU_MASTER_NEXUS_Z4D_0, /* NEXUS Z4D_0 */ \
}


/* @brief Fast IRC trimmed clock frequency(16MHz). */
#define FEATURE_IRCOSC0_FREQ (16000000U)

/* @brief Fast XOSC clock frequency(40MHz). */
#define FEATURE_XOSC0_FREQ  (40000000U)

/* @brief Define identifiers of auxiliary clock selectors */
#define NO_AC     0U
#define AC0__SC   1U
#define AC1__SC   2U
#define AC2__SC   3U
#define AC3__SC   4U
#define AC4__SC   5U
#define AC5__SC   6U
#define AC6__SC   7U
#define AC7__SC   8U
#define AC8__SC   9U
#define AC9__SC   10U
#define AC10__SC  11U
#define AC11__SC  12U
#define AC12__SC  13U
#define AC13__SC  14U
#define AC14__SC  15U
#define AC15__SC  16U
#define AC0__DC0  17U
#define AC0__DC1  18U
#define AC0__DC2  19U
#define AC0__DC3  20U
#define AC1__DC0  21U
#define AC1__DC1  22U
#define AC1__DC2  23U
#define AC1__DC3  24U
#define AC2__DC0  25U
#define AC2__DC1  26U
#define AC2__DC2  27U
#define AC2__DC3  28U
#define AC3__DC0  29U
#define AC3__DC1  40U
#define AC3__DC2  41U
#define AC3__DC3  42U
#define AC4__DC0  43U
#define AC4__DC1  44U
#define AC4__DC2  45U
#define AC4__DC3  46U
#define AC5__DC0  47U
#define AC5__DC1  48U
#define AC5__DC2  49U
#define AC5__DC3  50U
#define AC6__DC0  51U
#define AC6__DC1  52U
#define AC6__DC2  53U
#define AC6__DC3  54U
#define AC7__DC0  55U
#define AC7__DC1  56U
#define AC7__DC2  57U
#define AC7__DC3  58U
#define AC8__DC0  59U
#define AC8__DC1  60U
#define AC8__DC2  61U
#define AC8__DC3  62U
#define AC9__DC0  63U
#define AC9__DC1  64U
#define AC9__DC2  65U
#define AC9__DC3  66U
#define AC10__DC0 67U
#define AC10__DC1 68U
#define AC10__DC2 69U
#define AC10__DC3 70U
#define AC11__DC0 71U
#define AC11__DC1 72U
#define AC11__DC2 73U
#define AC11__DC3 74U
#define AC12__DC0 75U
#define AC12__DC1 76U
#define AC12__DC2 77U
#define AC12__DC3 78U
#define AC13__DC0 79U
#define AC13__DC1 80U
#define AC13__DC2 81U
#define AC13__DC3 82U
#define AC14__DC0 83U
#define AC14__DC1 84U
#define AC14__DC2 85U
#define AC14__DC3 86U
#define AC15__DC0 87U
#define AC15__DC1 88U
#define AC15__DC2 89U
#define AC15__DC3 90U

#define SYSTEM_CLOCK_DIVIDER0_MASK (1U<<0U)
#define SYSTEM_CLOCK_DIVIDER1_MASK (1U<<1U)
#define SYSTEM_CLOCK_DIVIDER2_MASK (1U<<2U)
#define SYSTEM_CLOCK_DIVIDER3_MASK (1U<<3U)
#define SYSTEM_CLOCK_DIVIDER4_MASK (1U<<4U)
#define SYSTEM_CLOCK_DIVIDER5_MASK (1U<<5U)
#define SYSTEM_CLOCK_DIVIDER6_MASK (1U<<6U)

/* @brief List of the configurable system clock dividers. */
#define SYSTEM_CLOCK_DIVIDERS  (SYSTEM_CLOCK_DIVIDER0_MASK)

/* @brief PLL input reference. */
#define FEATURE_PLL0_INPUT_REFERENCE  AC3__SC
#define FEATURE_PLL1_INPUT_REFERENCE  AC4__SC

/* @brief PLL reduced frequency divider version. */
#define FEATURE_PLL_REDUCED_FREQ_DIV_VERSION (0U)

/* @brief FLEXCAN0 configurable interface clock. */
#define FEATURE_FLEXCAN0_CLK_CONFIGURABLE_INTERFACE_CLOCK (0U)

/* @brief PBRIDGEx system clock divider index. */
#define FEATURE_PBRIDGEx_CLK_SYSTEM_CLOCK_DIVIDER_INDEX (0U)

/* @brief PLL0_PHI1 reference entry value. */
#define FEATURE_PLL0_PHI1_REFERENCE_ENTRY_VALUE (3U)


#define FEATURE_HAS_SIRC_CLK             (0U)
#define FEATURE_HAS_FIRC_CLK             (0U)
#define FEATURE_HAS_IRCOSC_CLK           (1U)
#define FEATURE_HAS_SXOSC_CLK            (0U)
#define FEATURE_HAS_FXOSC_CLK            (0U)
#define FEATURE_HAS_XOSC_CLK             (1U)
#define FEATURE_HAS_PLL_PHI0_CLK         (0U)
#define FEATURE_HAS_PLL_PHI1_CLK         (0U)
#define FEATURE_HAS_PLL0_PHI0_CLK        (1U)
#define FEATURE_HAS_PLL0_PHI1_CLK        (1U)
#define FEATURE_HAS_PLL1_PHI0_CLK        (1U)
#define FEATURE_HAS_PLL1_PHI1_CLK        (0U)
#define FEATURE_HAS_SDPLL_CLK            (0U)
#define FEATURE_HAS_SCS_CLK              (1U)
#define FEATURE_HAS_SCS_CLKS             (0U)
#define FEATURE_HAS_S160_CLK             (0U)
#define FEATURE_HAS_S160_CLKS            (0U)
#define FEATURE_HAS_S80_CLK              (0U)
#define FEATURE_HAS_S80_CLKS             (0U)
#define FEATURE_HAS_S40_CLK              (0U)
#define FEATURE_HAS_S40_CLKS             (0U)
#define FEATURE_HAS_F40_CLK              (0U)
#define FEATURE_HAS_F40_CLKS             (0U)
#define FEATURE_HAS_F80_CLK              (0U)
#define FEATURE_HAS_F80_CLKS             (0U)
#define FEATURE_HAS_FS80_CLK             (0U)
#define FEATURE_HAS_FS80_CLKS            (0U)
#define FEATURE_HAS_F20_CLK              (0U)
#define FEATURE_HAS_F20_CLKS             (0U)
#define FEATURE_HAS_DMA_CLK              (0U)
#define FEATURE_HAS_DMA_CLKS             (0U)
#define FEATURE_HAS_PBRIDGEx_CLK         (1U)
#define FEATURE_HAS_PBRIDGEx_CLKS        (1U)
#define FEATURE_HAS_HALFSYS_CLK          (1U)
#define FEATURE_HAS_HALFSYS_CLKS         (1U)
#define FEATURE_HAS_SYS_CLK              (0U)
#define FEATURE_HAS_SYS_CLKS             (0U)
#define FEATURE_HAS_MOTC_CLK             (1U)
#define FEATURE_HAS_MOTC_CLKS            (1U)
#define FEATURE_HAS_PER_CLK              (0U)
#define FEATURE_HAS_PER_CLKS             (0U)
#define FEATURE_HAS_SXBAR_CLK            (0U)
#define FEATURE_HAS_SXBAR_CLKS           (0U)
#define FEATURE_HAS_ADC_CLKS             (1U)
#define FEATURE_HAS_ADCSD_CLKS           (0U)
#define FEATURE_HAS_DSPI_CLKS            (0U)
#define FEATURE_HAS_DSPIM_CLKS           (0U)
#define FEATURE_HAS_ENET_CLKS            (1U)
#define FEATURE_HAS_ENET_TIME_CLKS       (1U)
#define FEATURE_HAS_EMIOS_CLKS           (0U)
#define FEATURE_HAS_ETPU_CLKS            (0U)
#define FEATURE_HAS_FLEXCAN_CLKS         (1U)
#define FEATURE_HAS_FLEXRAY_CLKS         (1U)
#define FEATURE_HAS_LFAST_CLKS           (1U)
#define FEATURE_HAS_LIN_CLKS             (0U)
#define FEATURE_HAS_RTI_CLKS             (0U)
#define FEATURE_HAS_SDHC_CLKS            (0U)
#define FEATURE_HAS_SENT_CLKS            (1U)
#define FEATURE_HAS_SGEN_CLKS            (1U)
#define FEATURE_HAS_SPI_CLKS             (0U)
#define FEATURE_HAS_SPT_CLKS             (0U)
#define FEATURE_HAS_CORE0_CLK            (0U)
#define FEATURE_HAS_CORE1_CLK            (0U)
#define FEATURE_HAS_CORE2_CLK            (0U)



#define FEATURE_PROTOCOL_CLOCK_FOR_ADC                 AC0__SC
#define FEATURE_PROTOCOL_CLOCK_FOR_ADCSD               NO_AC
#define FEATURE_PROTOCOL_CLOCK_FOR_DSPI                NO_AC
#define FEATURE_PROTOCOL_CLOCK_FOR_DSPIM               NO_AC
#define FEATURE_PROTOCOL_CLOCK_FOR_ENET                AC10__SC
#define FEATURE_PROTOCOL_CLOCK_FOR_ENET_TIME           AC11__SC
#define FEATURE_PROTOCOL_CLOCK_FOR_EMIOS               NO_AC
#define FEATURE_PROTOCOL_CLOCK_FOR_ETPU                NO_AC
#define FEATURE_PROTOCOL_CLOCK_FOR_FLEXCAN             NO_AC
#define FEATURE_PROTOCOL_CLOCK_FOR_FLEXRAY             NO_AC
#define FEATURE_PROTOCOL_CLOCK_FOR_LFAST               AC5__SC
#define FEATURE_PROTOCOL_CLOCK_FOR_LIN                 NO_AC
#define FEATURE_PROTOCOL_CLOCK_FOR_RTI                 NO_AC
#define FEATURE_PROTOCOL_CLOCK_FOR_SDHC                NO_AC
#define FEATURE_PROTOCOL_CLOCK_FOR_SENT                AC1__SC
#define FEATURE_PROTOCOL_CLOCK_FOR_SGEN                AC0__SC
#define FEATURE_PROTOCOL_CLOCK_FOR_SPI                 NO_AC
#define FEATURE_PROTOCOL_CLOCK_FOR_SPT                 NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_ADC                NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_ADCSD              NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_DSPI               NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_DSPIM              NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_ENET               NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_ENET_TIME          NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_EMIOS              NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_ETPU               NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_FLEXCAN            NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_FLEXRAY            NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_LFAST              NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_LIN                NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_RTI                NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_SDHC               NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_SENT               NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_SGEN               NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_SPI                NO_AC
#define FEATURE_INTERFACE_CLOCK_FOR_SPT                NO_AC
#define FEATURE_FRACTIONAL_DIVIDER_FOR_ADC             AC0__DC2
#define FEATURE_FRACTIONAL_DIVIDER_FOR_ADCSD           NO_AC
#define FEATURE_FRACTIONAL_DIVIDER_FOR_DSPI            NO_AC
#define FEATURE_FRACTIONAL_DIVIDER_FOR_DSPIM           NO_AC
#define FEATURE_FRACTIONAL_DIVIDER_FOR_ENET            AC10__DC0
#define FEATURE_FRACTIONAL_DIVIDER_FOR_ENET_TIME       AC11__DC0
#define FEATURE_FRACTIONAL_DIVIDER_FOR_EMIOS           NO_AC
#define FEATURE_FRACTIONAL_DIVIDER_FOR_ETPU            NO_AC
#define FEATURE_FRACTIONAL_DIVIDER_FOR_FLEXCAN         AC2__DC0
#define FEATURE_FRACTIONAL_DIVIDER_FOR_FLEXRAY         AC1__DC0
#define FEATURE_FRACTIONAL_DIVIDER_FOR_LFAST           AC5__DC0
#define FEATURE_FRACTIONAL_DIVIDER_FOR_LIN             NO_AC
#define FEATURE_FRACTIONAL_DIVIDER_FOR_RTI             NO_AC
#define FEATURE_FRACTIONAL_DIVIDER_FOR_SDHC            NO_AC
#define FEATURE_FRACTIONAL_DIVIDER_FOR_SENT            AC1__DC1
#define FEATURE_FRACTIONAL_DIVIDER_FOR_SGEN            AC0__DC1
#define FEATURE_FRACTIONAL_DIVIDER_FOR_SPI             NO_AC
#define FEATURE_FRACTIONAL_DIVIDER_FOR_SPT             NO_AC


/*! @brief Clock names. */
typedef enum {
    /* Clock sources */
    IRCOSC_CLK                     = 0u,      /*!< IRCOSC_CLK clock source                */
    XOSC_CLK                       = 1u,      /*!< XOSC_CLK clock source                  */
    PLL0_PHI0_CLK                  = 2u,      /*!< PLL0_PHI0_CLK clock source             */
    PLL0_PHI1_CLK                  = 3u,      /*!< PLL0_PHI1_CLK clock source             */
    PLL1_PHI0_CLK                  = 4u,      /*!< PLL1_PHI0_CLK clock source             */
    END_OF_CLK_SOURCES             = 20u,      /*!< End of clock sources                  */
    /* System and common clocks */
    SCS_CLK                        = 21u,      /*!< SCS_CLK common clock                  */
    PBRIDGEx_CLK                   = 22u,      /*!< PBRIDGEx_CLK common clock             */
    HALFSYS_CLK                    = 23u,      /*!< HALFSYS_CLK common clock              */
    MOTC_CLK                       = 24u,      /*!< MOTC_CLK common clock                 */
    CORE_CLK                       = 25u,      /*!< CORE_CLK                              */
    END_OF_SYSTEM_CLKS             = 40u,      /*!< End of common and system clocks       */
    CRC0_CLK                       = 41u,      /*!< CRC0_CLK clock source                 */
    DMAMUX0_CLK                    = 42u,      /*!< DMAMUX0_CLK clock source              */
    DMAMUX1_CLK                    = 43u,      /*!< DMAMUX1_CLK clock source              */
    MEMU0_CLK                      = 44u,      /*!< MEMU0_CLK clock source                */
    PITRTI0_CLK                    = 45u,      /*!< PITRTI0_CLK clock source              */
    SPI0_CLK                       = 46u,      /*!< SPI0_CLK clock source                 */
    SPI1_CLK                       = 47u,      /*!< SPI1_CLK clock source                 */
    SPI2_CLK                       = 48u,      /*!< SPI2_CLK clock source                 */
    SPI3_CLK                       = 49u,      /*!< SPI3_CLK clock source                 */
    WKPU0_CLK                      = 50u,      /*!< WKPU0_CLK clock source                */
    END_OF_PBRIDGEx_CLKS           = 51u,      /*!< End of PBRIDGEx_CLK clocks            */
    DMA0_CLK                       = 52u,      /*!< DMA0_CLK clock source                 */
    LIN0_CLK                       = 53u,      /*!< LIN0_CLK clock source                 */
    LIN1_CLK                       = 54u,      /*!< LIN1_CLK clock source                 */
    SIPI0_CLK                      = 55u,      /*!< SIPI0_CLK clock source                */
    END_OF_HALFSYS_CLKS            = 56u,      /*!< End of HALFSYS_CLK clocks             */
    CTU0_CLK                       = 57u,      /*!< CTU0_CLK clock source                 */
    CTU1_CLK                       = 58u,      /*!< CTU1_CLK clock source                 */
    ETIMER0_CLK                    = 59u,      /*!< ETIMER0_CLK clock source              */
    ETIMER1_CLK                    = 60u,      /*!< ETIMER1_CLK clock source              */
    ETIMER2_CLK                    = 61u,      /*!< ETIMER2_CLK clock source              */
    FLEXPWM0_CLK                   = 62u,      /*!< FLEXPWM0_CLK clock source             */
    FLEXPWM1_CLK                   = 63u,      /*!< FLEXPWM1_CLK clock source             */
    END_OF_MOTC_CLKS               = 64u,      /*!< End of MOTC_CLK clocks                */
    ADC0_CLK                       = 65u,      /*!< ADC0_CLK clock source                 */
    ADC1_CLK                       = 66u,      /*!< ADC1_CLK clock source                 */
    ADC2_CLK                       = 67u,      /*!< ADC2_CLK clock source                 */
    ADC3_CLK                       = 68u,      /*!< ADC3_CLK clock source                 */
    END_OF_ADC_CLKS                = 69u,      /*!< End of ADC_CLK clocks                 */
    ENET0_CLK                      = 70u,      /*!< ENET0_CLK clock source                */
    END_OF_ENET_CLKS               = 71u,      /*!< End of ENET_CLK clocks                */
    ENET0_TIME_CLK                 = 72u,      /*!< ENET0_TIME_CLK clock source           */
    END_OF_ENET_TIME_CLKS          = 73u,      /*!< End of ENET_TIME_CLK clocks           */
    FLEXCAN0_CLK                   = 74u,      /*!< FLEXCAN0_CLK clock source             */
    FLEXCAN1_CLK                   = 75u,      /*!< FLEXCAN1_CLK clock source             */
    FLEXCAN2_CLK                   = 76u,      /*!< FLEXCAN2_CLK clock source             */
    END_OF_FLEXCAN_CLKS            = 77u,      /*!< End of FLEXCAN_CLK clocks             */
    FLEXRAY0_CLK                   = 78u,      /*!< FLEXRAY0_CLK clock source             */
    END_OF_FLEXRAY_CLKS            = 79u,      /*!< End of FLEXRAY_CLK clocks             */
    LFAST0_CLK                     = 80u,      /*!< LFAST0_CLK clock source               */
    END_OF_LFAST_CLKS              = 81u,      /*!< End of LFAST_CLK clocks               */
    SENT0_CLK                      = 82u,      /*!< SENT0_CLK clock source                */
    SENT1_CLK                      = 83u,      /*!< SENT1_CLK clock source                */
    END_OF_SENT_CLKS               = 84u,      /*!< End of SENT_CLK clocks                */
    SGEN0_CLK                      = 85u,      /*!< SGEN0_CLK clock source                */
    END_OF_SGEN_CLKS               = 86u,      /*!< End of SGEN_CLK clocks                */
    SIUL0_CLK                      = 87u,      /*!< SIUL0_CLK clock source                */
    END_OF_PERIPHERAL_CLKS         = 88u,      /*!< End of peripheral clocks              */
    CLOCK_NAME_COUNT,
} clock_names_t;

#define MC_ME_INVALID_INDEX  MC_ME_PCTLn_COUNT

  /*! @brief MC_ME clock name mappings
   *  Mappings between clock names and peripheral clock control indexes.
   *  If there is no peripheral clock control index for a clock name,
   *  then the corresponding value is MC_ME_INVALID_INDEX.
   */
#define MC_ME_CLOCK_NAME_MAPPINGS \
{                                                                              \
MC_ME_INVALID_INDEX,                /*!< IRCOSC clock                                       0  */ \
MC_ME_INVALID_INDEX,                /*!< XOSC clock                                         1  */ \
MC_ME_INVALID_INDEX,                /*!< PLL0_PHI0 clock                                    2  */ \
MC_ME_INVALID_INDEX,                /*!< PLL0_PHI1 clock                                    3  */ \
MC_ME_INVALID_INDEX,                /*!< PLL1_PHI0 clock                                    4  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    5  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    6  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    7  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    8  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    9  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    10  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    11  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    12  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    13  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    14  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    15  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    16  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    17  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    18  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    19  */ \
MC_ME_INVALID_INDEX,                /*!< END_OF_CLK_SOURCES                                 20  */  \
MC_ME_INVALID_INDEX,                /*!< SCS_CLK clock                                      21  */ \
MC_ME_INVALID_INDEX,                /*!< PBRIDGEx_CLK clock                                 22  */ \
MC_ME_INVALID_INDEX,                /*!< HALFSYS_CLK clock                                  23  */ \
MC_ME_INVALID_INDEX,                /*!< MOTC_CLK clock                                     24  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    25  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    26  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    27  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    28  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    29  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    30  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    31  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    32  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    33  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    34  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    35  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    36  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    37  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    38  */ \
MC_ME_INVALID_INDEX,                /*!< No clock entry in clock_names_t                    39  */ \
MC_ME_INVALID_INDEX,                /*!< END_OF_SYSTEM_CLOCKS                               40  */  \
MC_ME_PCTL_CRC_0_INDEX,             /*!< CRC0 clock source                                  41 */ \
MC_ME_PCTL_DMAMUX_0_INDEX,          /*!< DMAMUX0 clock source                               42 */ \
MC_ME_PCTL_DMAMUX_1_INDEX,          /*!< DMAMUX1 clock source                               43 */ \
MC_ME_INVALID_INDEX,                /*!< MEMU0 clock source                                 44 */ \
MC_ME_PCTL_PIT_0_INDEX,             /*!< PITRTI0 clock source                               45 */ \
MC_ME_PCTL_DSPI_0_INDEX,            /*!< SPI0 clock source                                  46 */ \
MC_ME_PCTL_DSPI_1_INDEX,            /*!< SPI1 clock source                                  47 */ \
MC_ME_PCTL_DSPI_2_INDEX,            /*!< SPI2 clock source                                  48 */ \
MC_ME_PCTL_DSPI_3_INDEX,            /*!< SPI3 clock source                                  49 */ \
MC_ME_INVALID_INDEX,                /*!< WKPU0 clock source                                 50 */ \
MC_ME_INVALID_INDEX,                /*!< End of PBRIDGEx clocks                             51 */ \
MC_ME_INVALID_INDEX,                /*!< DMA0 clock source                                  52 */ \
MC_ME_PCTL_LINFlex_0_INDEX,         /*!< LIN0 clock source                                  53 */ \
MC_ME_PCTL_LINFlex_1_INDEX,         /*!< LIN1 clock source                                  54 */ \
MC_ME_PCTL_SIPI_0_INDEX,            /*!< SIPI0 clock source                                 55 */ \
MC_ME_INVALID_INDEX,                /*!< End of HALFSYS clocks                              56 */ \
MC_ME_PCTL_CTU_0_INDEX,             /*!< CTU0 clock source                                  57 */ \
MC_ME_PCTL_CTU_1_INDEX,             /*!< CTU1 clock source                                  58 */ \
MC_ME_PCTL_ETIMER_0_INDEX,          /*!< ETIMER0 clock source                               59 */ \
MC_ME_PCTL_ETIMER_1_INDEX,          /*!< ETIMER1 clock source                               60 */ \
MC_ME_PCTL_ETIMER_2_INDEX,          /*!< ETIMER2 clock source                               61 */ \
MC_ME_PCTL_PWM_0_INDEX,             /*!< FLEXPWM0 clock source                              62 */ \
MC_ME_PCTL_PWM_1_INDEX,             /*!< FLEXPWM1 clock source                              63 */ \
MC_ME_INVALID_INDEX,                /*!< End of MOTC clocks                                 64 */ \
MC_ME_PCTL_ADC_0_INDEX,             /*!< ADC0 clock source                                  65 */ \
MC_ME_PCTL_ADC_1_INDEX,             /*!< ADC1 clock source                                  66 */ \
MC_ME_PCTL_ADC_2_INDEX,             /*!< ADC2 clock source                                  67 */ \
MC_ME_PCTL_ADC_3_INDEX,             /*!< ADC3 clock source                                  68 */ \
MC_ME_INVALID_INDEX,                /*!< End of ADC clocks                                  69 */ \
MC_ME_PCTL_ENET_0_INDEX,            /*!< ENET0 clock source                                 70 */ \
MC_ME_INVALID_INDEX,                /*!< End of ENET clocks                                 71 */ \
MC_ME_INVALID_INDEX,                /*!< ENET0_TIME clock source                            72 */ \
MC_ME_INVALID_INDEX,                /*!< End of ENET_TIME clocks                            73 */ \
MC_ME_PCTL_CAN_0_INDEX,             /*!< FLEXCAN0 clock source                              74 */ \
MC_ME_PCTL_CAN_1_INDEX,             /*!< FLEXCAN1 clock source                              75 */ \
MC_ME_PCTL_CAN_2_INDEX,             /*!< FLEXCAN2 clock source                              76 */ \
MC_ME_INVALID_INDEX,                /*!< End of FLEXCAN clocks                              77 */ \
MC_ME_PCTL_FLEXRAY_INDEX,           /*!< FLEXRAY0 clock source                              78 */ \
MC_ME_INVALID_INDEX,                /*!< End of FLEXRAY clocks                              79 */ \
MC_ME_PCTL_LFAST_0_INDEX,           /*!< LFAST0 clock source                                80 */ \
MC_ME_INVALID_INDEX,                /*!< End of LFAST clocks                                81 */ \
MC_ME_PCTL_SENT_0_INDEX,            /*!< SENT0 clock source                                 82 */ \
MC_ME_PCTL_SENT_1_INDEX,            /*!< SENT1 clock source                                 83 */ \
MC_ME_INVALID_INDEX,                /*!< End of SENT clocks                                 84 */ \
MC_ME_PCTL_SGEN_0_INDEX,            /*!< SGEN0 clock source                                 85 */ \
MC_ME_INVALID_INDEX,                /*!< End of SGEN clocks                                 86 */ \
MC_ME_INVALID_INDEX,                /*!< SIUL0 clock source                                 87 */ \
MC_ME_INVALID_INDEX                /*!< END_OF_PERIPHERAL_CLK_SOURCES                       88 */ \
}


  /*! @brief interface clocks
   *  Mappings between clock names and interface clocks.
   *  If no interface clock exists for a given clock name,
   *  then the corresponding value is CLOCK_NAME_COUNT.
   */
#define INTERFACE_CLOCKS \
{                                                                           \
CLOCK_NAME_COUNT,                /*!< IRCOSC clock                    0  */ \
CLOCK_NAME_COUNT,                /*!< XOSC clock                      1  */ \
CLOCK_NAME_COUNT,                /*!< PLL0_PHI0 clock                 2  */ \
CLOCK_NAME_COUNT,                /*!< PLL0_PHI1 clock                 3  */ \
CLOCK_NAME_COUNT,                /*!< PLL1_PHI0 clock                 4  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 5  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 6  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 7  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 8  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 9  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 10  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 11  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 12  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 13  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 14  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 15  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 16  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 17  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 18  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 19  */ \
CLOCK_NAME_COUNT,                /*!< END_OF_CLK_SOURCES              20  */ \
CLOCK_NAME_COUNT,                /*!< SCS_CLK clock                   21  */ \
CLOCK_NAME_COUNT,                /*!< PBRIDGEx_CLK clock              22  */ \
CLOCK_NAME_COUNT,                /*!< HALFSYS_CLK clock               23  */ \
CLOCK_NAME_COUNT,                /*!< MOTC_CLK clock                  24  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 25  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 26  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 27  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 28  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 29  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 30  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 31  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 32  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 33  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 34  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 35  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 36  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 37  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 38  */ \
CLOCK_NAME_COUNT,                /*!< No clock entry in clock_names_t 39  */ \
CLOCK_NAME_COUNT,                /*!< END_OF_SYSTEM_CLOCKS            40  */ \
PBRIDGEx_CLK,                    /*!< CRC0 clock                      41 */ \
PBRIDGEx_CLK,                    /*!< DMAMUX0 clock                   42 */ \
PBRIDGEx_CLK,                    /*!< DMAMUX1 clock                   43 */ \
PBRIDGEx_CLK,                    /*!< MEMU0 clock                     44 */ \
PBRIDGEx_CLK,                    /*!< PITRTI0 clock                   45 */ \
PBRIDGEx_CLK,                    /*!< SPI0 clock                      46 */ \
PBRIDGEx_CLK,                    /*!< SPI1 clock                      47 */ \
PBRIDGEx_CLK,                    /*!< SPI2 clock                      48 */ \
PBRIDGEx_CLK,                    /*!< SPI3 clock                      49 */ \
PBRIDGEx_CLK,                    /*!< WKPU0 clock                     50 */ \
CLOCK_NAME_COUNT,                /*!< End of PBRIDGEx clocks          51 */ \
HALFSYS_CLK,                     /*!< DMA0 clock                      52 */ \
PBRIDGEx_CLK,                    /*!< LIN0 clock                      53 */ \
PBRIDGEx_CLK,                    /*!< LIN1 clock                      54 */ \
PBRIDGEx_CLK,                    /*!< SIPI0 clock                     55 */ \
CLOCK_NAME_COUNT,                /*!< End of HALFSYS clocks           56 */ \
PBRIDGEx_CLK,                    /*!< CTU0 clock                      57 */ \
PBRIDGEx_CLK,                    /*!< CTU1 clock                      58 */ \
MOTC_CLK,                        /*!< ETIMER0 clock                   59 */ \
MOTC_CLK,                        /*!< ETIMER1 clock                   60 */ \
MOTC_CLK,                        /*!< ETIMER2 clock                   61 */ \
PBRIDGEx_CLK,                    /*!< FLEXPWM0 clock                  62 */ \
PBRIDGEx_CLK,                    /*!< FLEXPWM1 clock                  63 */ \
CLOCK_NAME_COUNT,                /*!< End of MOTC clocks              64 */ \
PBRIDGEx_CLK,                    /*!< ADC0 clock                      65 */ \
PBRIDGEx_CLK,                    /*!< ADC1 clock                      66 */ \
PBRIDGEx_CLK,                    /*!< ADC2 clock                      67 */ \
PBRIDGEx_CLK,                    /*!< ADC3 clock                      68 */ \
CLOCK_NAME_COUNT,                /*!< End of ADC clocks               69 */ \
PBRIDGEx_CLK,                    /*!< ENET0 clock                     70 */ \
CLOCK_NAME_COUNT,                /*!< End of ENET clocks              71 */ \
PBRIDGEx_CLK,                    /*!< ENET0_TIME clock                72 */ \
CLOCK_NAME_COUNT,                /*!< End of ENET_TIME clocks         73 */ \
PBRIDGEx_CLK,                    /*!< FLEXCAN0 clock                  74 */ \
PBRIDGEx_CLK,                    /*!< FLEXCAN1 clock                  75 */ \
PBRIDGEx_CLK,                    /*!< FLEXCAN2 clock                  76 */ \
CLOCK_NAME_COUNT,                /*!< End of FLEXCAN clocks           77 */ \
PBRIDGEx_CLK,                    /*!< FLEXRAY0 clock                  78 */ \
CLOCK_NAME_COUNT,                /*!< End of FLEXRAY clocks           79 */ \
PBRIDGEx_CLK,                    /*!< LFAST0 clock                    80 */ \
CLOCK_NAME_COUNT,                /*!< End of LFAST clocks             81 */ \
PBRIDGEx_CLK,                    /*!< SENT0 clock                     82 */ \
PBRIDGEx_CLK,                    /*!< SENT1 clock                     83 */ \
CLOCK_NAME_COUNT,                /*!< End of SENT clocks              84 */ \
PBRIDGEx_CLK,                    /*!< SGEN0 clock                     85 */ \
CLOCK_NAME_COUNT,                /*!< End of SGEN clocks              86 */ \
PBRIDGEx_CLK,                    /*!< SIUL0 clock                     87 */ \
CLOCK_NAME_COUNT                /*!< END_OF_PERIPHERAL_CLK_SOURCES    88 */ \
}


/* Interrupt module features */

/* @brief Lowest interrupt request number. */
#define FEATURE_INTERRUPT_IRQ_MIN         (SS0_IRQn)
/* @brief Highest interrupt request number. */
#define FEATURE_INTERRUPT_IRQ_MAX         (SGEN_Err_IRQn)
/* @brief Highest interrupt priority number. */
#define FEATURE_INTERRUPT_PRIO_MAX  (31u)
/* @brief Highest software interrupt request number. */
#define FEATURE_INTERRUPT_SOFTWARE_IRQ_MAX  (SS15_IRQn)
/* @brief Has software interrupt. */
#define FEATURE_INTERRUPT_HAS_SOFTWARE_IRQ  (1u)
/* @brief Has pending interrupt state. */
#define FEATURE_INTERRUPT_HAS_PENDING_STATE (0u)
/* @brief Has active interrupt state. */
#define FEATURE_INTERRUPT_HAS_ACTIVE_STATE  (0u)
/* @brief Default interrupt priority for enable interrupts. */
#define FEATURE_INTERRUPT_DEFAULT_PRIO  (15u)
/* @brief Multicore support for interrupts */
#define FEATURE_INTERRUPT_MULTICORE_SUPPORT  (1u)
/* @brief Mask to enable interrupts for all cores */
#define FEATURE_INTERRUPT_ENABLE_ON_ALL_CORES_MASK  (0x8000u)
/* @brief Available cores for this device */
#define FEATURE_INTERRUPT_CORE_0_ENABLED (1u)
#define FEATURE_INTERRUPT_CORE_1_ENABLED (0u)
#define FEATURE_INTERRUPT_CORE_2_ENABLED (0u)
/* @brief Registers in which the start of interrupt vector table needs to be configured */
#define FEATURE_INTERRUPT_INT_VECTORS {&INTC->IACKR0}

/* FLASH C55 module features */

/* @brief Type of flash module is C55FMC. */
#define FEATURE_FLS_C55_C55FMC (1U)
/* @brief Type of flash module is C55MP. */
#define FEATURE_FLS_C55_C55MP (0U)
/* @brief Type of flash module is C55FP. */
#define FEATURE_FLS_C55_C55FP (0U)
/* @brief Over-program protection enabled block. */
#define FEATURE_FLS_C55_HAS_OTP
/* @brief Has alternate interface. */
#define FEATURE_FLS_C55_HAS_ALTERNATE (0U)
/* @brief Has erase operation in the alternate interface. */
#define FEATURE_FLS_C55_HAS_ERASE_ALTERNATE (0U)
/* @brief The size of writes that are allowed. */
#define FLASH_C55_PROGRAMABLE_SIZE (0x80U)
/* @brief The address in the uTest space. */
#define FLASH_C55_INTERLOCK_WRITE_UTEST_ADDRESS (0x00400000U)
/* @brief The base address of the 16KB high block. */
#define FLASH_C55_16KB_HIGH_BASE_ADDRESS (0xFFFFFFFFU)
/* @brief The base address of the 32KB high block. */
#define FLASH_C55_32KB_HIGH_BASE_ADDRESS (0xFFFFFFFFU)
/* @brief The base address of the 64KB high block. */
#define FLASH_C55_64KB_HIGH_BASE_ADDRESS (0x00FA0000U)
/* @brief The base address of the 16KB mid block. */
#define FLASH_C55_16KB_MID_BASE_ADDRESS (0xFFFFFFFFU)
/* @brief The base address of the 32KB mid block. */
#define FLASH_C55_32KB_MID_BASE_ADDRESS (0x00808000U)
/* @brief The base address of the 64KB mid block. */
#define FLASH_C55_64KB_MID_BASE_ADDRESS (0xFFFFFFFFU)
/* @brief The base address of the 16KB low block. */
#define FLASH_C55_16KB_LOW_BASE_ADDRESS (0x00800000U)
/* @brief The second base address of the 16KB low block. */
#define FLASH_C55_16KB_SECOND_LOW_BASE_ADDRESS (0x00F90000U)
/* @brief The base address of the 32KB low block. */
#define FLASH_C55_32KB_LOW_BASE_ADDRESS (0xFFFFFFFFU)
/* @brief The base address of the 64KB low block. */
#define FLASH_C55_64KB_LOW_BASE_ADDRESS (0xFFFFFFFFU)
/* @brief The base address of the 256KB block. */
#define FLASH_C55_256KB_BASE_ADDRESS (0x01000000U)
/* @brief The number of low block */
#define NUM_LOW_BLOCK                          4U
/* @brief The number of mid block */
#define NUM_MID_BLOCK                          2U
/* @brief The number of high block */
#define NUM_HIGH_BLOCK                         6U
/* @brief The number of 256k block */
#define NUM_256K_BLOCK_FIRST                   8U
#define NUM_256K_BLOCK_SECOND                  0U
#define NUM_256K_BLOCK                         (NUM_256K_BLOCK_FIRST + NUM_256K_BLOCK_SECOND)
/* @brief The number of all block */
#define NUM_BLOCK                              (NUM_LOW_BLOCK + NUM_MID_BLOCK + NUM_HIGH_BLOCK + NUM_256K_BLOCK)
/* Define Macros for low blocks */
#define LOW_BLOCK0_ADDR                        0x00800000U
#define LOW_BLOCK0_MASK                        (1UL << 0x0U)

#define LOW_BLOCK1_ADDR                        0x00804000U
#define LOW_BLOCK1_MASK                        (1UL << 0x1U)

#define LOW_BLOCK2_ADDR                        0x00F98000U
#define LOW_BLOCK2_MASK                        (1UL << 0x2U)

#define LOW_BLOCK3_ADDR                        0x00F9C000U
#define LOW_BLOCK3_MASK                        (1UL << 0x3U)

/* Define Macros for mid blocks */
#define MID_BLOCK0_ADDR                        0x00808000U
#define MID_BLOCK0_MASK                        (1UL << 0x0U)

#define MID_BLOCK1_ADDR                        0x00810000U
#define MID_BLOCK1_MASK                        (1UL << 0x1U)


/* Define Macros for high blocks */
#define HIGH_BLOCK0_ADDR                       0x00FA0000U
#define HIGH_BLOCK0_MASK                       (1UL << 0x0U)

#define HIGH_BLOCK1_ADDR                       0x00FB0000U
#define HIGH_BLOCK1_MASK                       (1UL << 0x1U)

#define HIGH_BLOCK2_ADDR                       0x00FC0000U
#define HIGH_BLOCK2_MASK                       (1UL << 0x2U)

#define HIGH_BLOCK3_ADDR                       0x00FD0000U
#define HIGH_BLOCK3_MASK                       (1UL << 0x3U)

#define HIGH_BLOCK4_ADDR                       0x00FE0000U
#define HIGH_BLOCK4_MASK                       (1UL << 0x4U)

#define HIGH_BLOCK5_ADDR                       0x00FF0000U
#define HIGH_BLOCK5_MASK                       (1UL << 0x5U)

/* Define Macros for 256k blocks */
#define N256K_BLOCK0_ADDR                       0x01000000U
#define N256K_BLOCK0_MASK                       (1UL << 0x0U)

#define N256K_BLOCK1_ADDR                       0x01040000U
#define N256K_BLOCK1_MASK                       (1UL << 0x1U)

#define N256K_BLOCK2_ADDR                       0x01080000U
#define N256K_BLOCK2_MASK                       (1UL << 0x2U)

#define N256K_BLOCK3_ADDR                       0x010C0000U
#define N256K_BLOCK3_MASK                       (1UL << 0x3U)

#define N256K_BLOCK4_ADDR                       0x01100000U
#define N256K_BLOCK4_MASK                       (1UL << 0x4U)

#define N256K_BLOCK5_ADDR                       0x01140000U
#define N256K_BLOCK5_MASK                       (1UL << 0x5U)

#define N256K_BLOCK6_ADDR                       0x01180000U
#define N256K_BLOCK6_MASK                       (1UL << 0x6U)

#define N256K_BLOCK7_ADDR                       0x011C0000U
#define N256K_BLOCK7_MASK                       (1UL << 0x7U)
#define FLASH_BLOCK_ADDR_DEFINE \
{                               \
LOW_BLOCK0_ADDR, /* Low block 0 address */ \
LOW_BLOCK1_ADDR, /* Low block 1 address */ \
LOW_BLOCK2_ADDR, /* Low block 2 address */ \
LOW_BLOCK3_ADDR, /* Low block 3 address */ \
MID_BLOCK0_ADDR, /* Mid block 0 address */ \
MID_BLOCK1_ADDR, /* Mid block 1 address */ \
HIGH_BLOCK0_ADDR, /* High block 0 address */  \
HIGH_BLOCK1_ADDR, /* High block 1 address */  \
HIGH_BLOCK2_ADDR, /* High block 2 address */ \
HIGH_BLOCK3_ADDR, /* High block 3 address */ \
HIGH_BLOCK4_ADDR, /* High block 4 address */  \
HIGH_BLOCK5_ADDR, /* High block 5 address */  \
N256K_BLOCK0_ADDR, /* 256K block 0 address */  \
N256K_BLOCK1_ADDR, /* 256K block 1 address */  \
N256K_BLOCK2_ADDR, /* 256K block 2 address */  \
N256K_BLOCK3_ADDR, /* 256K block 3 address */  \
N256K_BLOCK4_ADDR, /* 256K block 4 address */  \
N256K_BLOCK5_ADDR, /* 256K block 5 address */  \
N256K_BLOCK6_ADDR, /* 256K block 6 address */  \
N256K_BLOCK7_ADDR  /* 256K block 7 address */  \
}
#define FLASH_BLOCK_MASK_DEFINE \
{                              \
LOW_BLOCK0_MASK, /* Low block 0 mask */ \
LOW_BLOCK1_MASK, /* Low block 1 mask */ \
LOW_BLOCK2_MASK, /* Low block 2 mask */ \
LOW_BLOCK3_MASK, /* Low block 3 mask */ \
MID_BLOCK0_MASK, /* Mid block 0 mask */ \
MID_BLOCK1_MASK, /* Mid block 1 mask */ \
HIGH_BLOCK0_MASK, /* High block 0 mask */  \
HIGH_BLOCK1_MASK, /* High block 1 mask */  \
HIGH_BLOCK2_MASK, /* High block 2 mask */ \
HIGH_BLOCK3_MASK, /* High block 3 mask */ \
HIGH_BLOCK4_MASK, /* High block 4 mask */  \
HIGH_BLOCK5_MASK, /* High block 5 mask */  \
N256K_BLOCK0_MASK, /* 256K block 0 mask */  \
N256K_BLOCK1_MASK, /* 256K block 1 mask */  \
N256K_BLOCK2_MASK, /* 256K block 2 mask */  \
N256K_BLOCK3_MASK, /* 256K block 3 mask */  \
N256K_BLOCK4_MASK, /* 256K block 4 mask */  \
N256K_BLOCK5_MASK, /* 256K block 5 mask */  \
N256K_BLOCK6_MASK, /* 256K block 6 mask */  \
N256K_BLOCK7_MASK  /* 256K block 7 mask */  \
}
/* EEE module features */

/*  @brief The block endurance for erasing successful */
#define BLOCK_MAX_ENDURANCE       (250000U)
/*  @brief The EEC error will occur in IVOR exception handler */
#define EEE_ERR_IVOR_EXCEPTION    (0)
/*  @brief The EEC error will set an ECC error in MCR register */
#define EEE_ERR_C55_MCR           (1)
/*  @brief Has 4 bytes ECC */
#define EEE_ECC4                  (0)
/*  @brief Has 8 bytes ECC */
#define EEE_ECC8                  (1)
/*  @brief Has 16 bytes ECC */
#define EEE_ECC16                 (0)
/*  @brief Has 32 bytes ECC */
#define EEE_ECC32                 (0)
/* OSIF module features */

#define FEATURE_OSIF_USE_SYSTICK                         (0)
#define FEATURE_OSIF_USE_PIT                             (1)
#define FEATURE_OSIF_FREERTOS_ISR_CONTEXT_METHOD         (2) /* PowerPC FreeRTOS v9.0.0 */
#define OSIF_PIT (PIT_0)
#define OSIF_PIT_CHAN_ID (3u)

/* FCCU module features */

#define FEATURE_FCCU_UNLOCK_OP1                (0x913756AFU)
#define FEATURE_FCCU_UNLOCK_OP2                (0x825A132BU)
#define FEATURE_FCCU_UNLOCK_OP31               (0x29AF8752U)

#define FEATURE_FCCU_NCF_KEY                   (0xAB3498FEU)

#define FEATURE_FCCU_IRQ_EN_MASK               (0x1U)

#define FEATURE_FCCU_TRANS_UNLOCK              (0xBCU)
#define FEATURE_FCCU_PERMNT_LOCK               (0xFFU)
#define FEATURE_FCCU_EINOUT_EOUTX_MASK         (FCCU_EINOUT_EOUT0_MASK|FCCU_EINOUT_EOUT1_MASK)

#define FEATURE_FCCU_MAX_FAULTS_NO             (uint8_t)(74U)
/* FCUU filter feature */
#define FEATURE_FCCU_FILTER_EN                 (1U)
/* FCCU open drain for the error indicating pin(s) */
#define FEATURE_FCCU_OPEN_DRAIN_EN             (0U)

/* MSCM module features */

/* @brief Has interrupt router control registers (IRSPRCn). */
#define FEATURE_MSCM_HAS_INTERRUPT_ROUTER                (0)
/* @brief Has directed CPU interrupt routerregisters (IRCPxxx). */
#define FEATURE_MSCM_HAS_CPU_INTERRUPT_ROUTER            (0)


/* LINFlexD module features */

/* @brief LINFlexD separate interrupt lines for rx/tx/error. */
#define FEATURE_LINFLEXD_RX_TX_ERR_INT_LINES
/* @brief Clock names for LINFlexD */
#define LINFLEXD_CLOCK_NAMES {LIN0_CLK, LIN1_CLK}
/* @brief Address of the least significant byte in a 32-bit register (depends on endianess) */
#define FEATURE_LINFLEXD_LSB_ADDR(reg)  ((uint32_t)(&(reg)) + 3U)
/* @brief Address of the least significant word in a 32-bit register (depends on endianess) */
#define FEATURE_LINFLEXD_LSW_ADDR(reg)  ((uint32_t)(&(reg)) + 2U)
/* @brief LINFlexD "instance-implemented filters" mapping */
#define FEATURE_LINFLEXD_INST_HAS_IFCR {true, true}
/* @brief LINFlexD DMA support */
#define FEATURE_LINFLEXD_HAS_DMA_ENABLED
/* @brief LINFlexD DMA enabled instances */
#define FEATURE_LINFLEXD_INST_HAS_DMA {true, true}
/* @brief LINFlexD timeout interrupt enable bit mask */
#define LINFlexD_LINIER_TOIE_MASK LINFlexD_LINIER_DBEIETOIE_MASK

/* EDMA module features */

/* @brief Number of DMA channels. */
#define FEATURE_DMA_CHANNELS (32U)
/* @brief Number of DMA virtual channels. */
#define FEATURE_DMA_VIRTUAL_CHANNELS (FEATURE_DMA_CHANNELS * DMA_INSTANCE_COUNT)
/* @brief Number of DMA interrupt lines. */
#define FEATURE_DMA_CHANNELS_INTERRUPT_LINES (32U)
/* @brief Number of DMA virtual interrupt lines. */
#define FEATURE_DMA_VIRTUAL_CHANNELS_INTERRUPT_LINES ((uint32_t)FEATURE_DMA_CHANNELS_INTERRUPT_LINES * (uint32_t)DMA_INSTANCE_COUNT)
/* @brief Number of DMA error interrupt lines. */
#define FEATURE_DMA_ERROR_INTERRUPT_LINES (1U)
/* @brief Number of DMA virtual error interrupt lines. */
#define FEATURE_DMA_VIRTUAL_ERROR_INTERRUPT_LINES ((uint32_t)FEATURE_DMA_ERROR_INTERRUPT_LINES * (uint32_t)DMA_INSTANCE_COUNT)
/* @brief DMA module has error interrupt. */
#define FEATURE_DMA_HAS_ERROR_IRQ
/* @brief DMA module separate interrupt lines for each channel */
#define FEATURE_DMA_SEPARATE_IRQ_LINES_PER_CHN
/* @brief Conversion from channel index to DCHPRI index. */
#define FEATURE_DMA_CHN_TO_DCHPRI_INDEX(x) (x)
/* @brief DMA channel groups count. */
#define FEATURE_DMA_CHANNEL_GROUP_COUNT (2U)
/* @brief Clock name for DMA */
#define FEATURE_DMA_CLOCK_NAMES {DMA0_CLK}
/* @brief Macros defined for compatibility with ARM platforms */
#define DMA_TCD_CITER_ELINKYES_CITER_LE_MASK   DMA_TCD_CITER_ELINKYES_CITER_MASK
#define DMA_TCD_CITER_ELINKYES_CITER_LE_SHIFT  DMA_TCD_CITER_ELINKYES_CITER_SHIFT
#define DMA_TCD_CITER_ELINKYES_CITER_LE_WIDTH  DMA_TCD_CITER_ELINKYES_CITER_WIDTH
#define DMA_TCD_CITER_ELINKYES_CITER_LE(x)     DMA_TCD_CITER_ELINKYES_CITER(x)
/* @brief DMA channel width based on number of TCDs: 2^N, N=4,5,... */
#define FEATURE_DMA_CH_WIDTH (5U)
/* @brief DMA channel to instance */
#define FEATURE_DMA_VCH_TO_INSTANCE(x) 	((x) >> (uint32_t)FEATURE_DMA_CH_WIDTH)
/* @brief DMA virtual channel to channel */
#define FEATURE_DMA_VCH_TO_CH(x)        ((x) & ((uint32_t)FEATURE_DMA_CHANNELS - 1U))
/* @brief DMA supports the following particular transfer size: */
#define FEATURE_DMA_TRANSFER_SIZE_8B
#define FEATURE_DMA_TRANSFER_SIZE_32B


/* DMAMUX module features */

/* @brief DMAMUX peripheral is available in silicon. */
#define FEATURE_DMAMUX_AVAILABLE
/* @brief Number of DMA channels. */
#define FEATURE_DMAMUX_CHANNELS (16U)
/* @brief Has the periodic trigger capability */
#define FEATURE_DMAMUX_HAS_TRIG (1)
/* @brief Conversion from request source to the actual DMAMUX channel */
#define FEATURE_DMAMUX_REQ_SRC_TO_CH(x) ((x) & DMAMUX_CHCFG_SOURCE_MASK)
/* @brief Mapping between request source and DMAMUX instance */
#define FEATURE_DMAMUX_REQ_SRC_TO_INSTANCE(x) (((uint8_t)x) >> (uint8_t)DMAMUX_CHCFG_SOURCE_WIDTH)
/* @brief Conversion from eDMA channel index to DMAMUX channel. */
#define FEATURE_DMAMUX_DMA_CH_TO_CH(x) ((x) & (FEATURE_DMAMUX_CHANNELS - 1U))
/* @brief Conversion from DMAMUX channel DMAMUX register index. */
#define FEATURE_DMAMUX_CHN_REG_INDEX(x) (x)
/* @brief Clock names for DMAMUX */
#define FEATURE_DMAMUX_CLOCK_NAMES {DMAMUX0_CLK, DMAMUX1_CLK}
/*!
 * @brief Structure for the DMA hardware request
 *
 * Defines the structure for the DMA hardware request collections. The user can configure the
 * hardware request into DMAMUX to trigger the DMA transfer accordingly. The index
 * of the hardware request varies according  to the to SoC.
 */
typedef enum {
    EDMA_REQ_MUX_0_DISABLED = 0U,
    EDMA_REQ_DSPI_2_TFFF = 1U,
    EDMA_REQ_DSPI_2_RFDF = 2U,
    EDMA_REQ_DSPI_3_TFFF = 3U,
    EDMA_REQ_DSPI_3_RFDF = 4U,
    EDMA_REQ_CTU_0_CTU = 5U,
    EDMA_REQ_CTU_0_FIFO1 = 6U,
    EDMA_REQ_CTU_0_FIFO2 = 7U,
    EDMA_REQ_CTU_0_FIFO3 = 8U,
    EDMA_REQ_CTU_0_FIFO4 = 9U,
    EDMA_REQ_FLEXPWM_0_COMP_VAL = 10U,
    EDMA_REQ_FlexPWM_0_CAPT = 11U,
    EDMA_REQ_ETIMER_0_DREQ_0 = 12U,
    EDMA_REQ_ETIMER_0_DREQ_1 = 13U,
    EDMA_REQ_ETIMER_0_DREQ_2 = 14U,
    EDMA_REQ_ETIMER_0_DREQ_3 = 15U,
    EDMA_REQ_ETIMER_2_DREQ_0 = 16U,
    EDMA_REQ_ETIMER_2_DREQ_1 = 17U,
    EDMA_REQ_ADC_0 = 18U,
    EDMA_REQ_ADC_2 = 19U,
    EDMA_REQ_LIN_0_TX = 20U,
    EDMA_REQ_LIN_0_RX = 21U,
    EDMA_REQ_SENT_1_FAST_MSG = 22U,
    EDMA_REQ_SENT_1_SLOW_MSG = 23U,
    EDMA_REQ_MUX_0_ALWAYS_ENABLED_0 = 24U,
    EDMA_REQ_MUX_0_ALWAYS_ENABLED_1 = 25U,
    EDMA_REQ_MUX_0_ALWAYS_ENABLED_2 = 26U,
    EDMA_REQ_MUX_0_ALWAYS_ENABLED_3 = 27U,
    EDMA_REQ_MUX_0_ALWAYS_ENABLED_4 = 28U,
    EDMA_REQ_MUX_0_ALWAYS_ENABLED_5 = 29U,
    EDMA_REQ_MUX_1_DISABLED = 64U,
    EDMA_REQ_DSPI_0_TFFF = 65U,
    EDMA_REQ_DSPI_0_RFDF = 66U,
    EDMA_REQ_DSPI_1_TFFF = 67U,
    EDMA_REQ_DSPI_1_RFDF = 68U,
    EDMA_REQ_CTU_1_CTU = 69U,
    EDMA_REQ_CTU_1_FIFO1 = 70U,
    EDMA_REQ_CTU_1_FIFO2 = 71U,
    EDMA_REQ_CTU_1_FIFO3 = 72U,
    EDMA_REQ_CTU_1_FIFO4 = 73U,
    EDMA_REQ_ETIMER_1_DREQ_0 = 74U,
    EDMA_REQ_ETIMER_1_DREQ_1 = 75U,
    EDMA_REQ_ADC_1 = 76U,
    EDMA_REQ_ADC_3 = 77U,
    EDMA_REQ_LIN_1_TX = 78U,
    EDMA_REQ_LIN_1_RX = 79U,
    EDMA_REQ_FLEXPWM_1_COMP_VAL = 80U,
    EDMA_REQ_FLEXPWM_1_CAPT = 81U,
    EDMA_REQ_SIPI_CH_0 = 82U,
    EDMA_REQ_SIPI_CH_1 = 83U,
    EDMA_REQ_SIPI_CH_2 = 84U,
    EDMA_REQ_SIPI_CH_3 = 85U,
    EDMA_REQ_SENT_0_FAST_MSG = 86U,
    EDMA_REQ_SENT_0_SLOW_MSG = 87U,
    EDMA_REQ_SIUL2_REQ_0 = 88U,
    EDMA_REQ_SIUL2_REQ_1 = 89U,
    EDMA_REQ_SIUL2_REQ_2 = 90U,
    EDMA_REQ_SIUL2_REQ_3 = 91U,
    EDMA_REQ_MUX_1_ALWAYS_ENABLED_0 = 92U,
    EDMA_REQ_MUX_1_ALWAYS_ENABLED_1 = 93U,
    EDMA_REQ_MUX_1_ALWAYS_ENABLED_2 = 94U,
    EDMA_REQ_MUX_1_ALWAYS_ENABLED_3 = 95U,
    EDMA_REQ_MUX_1_ALWAYS_ENABLED_4 = 96U,
    EDMA_REQ_MUX_1_ALWAYS_ENABLED_5 = 97U
} dma_request_source_t;

/* ETIMER (Enhanced Motor Control Timer) module features */

/*!
 * @brief This etimer module has watchdog ability
 */
#define FEATURE_ETIMER_HAS_WATCHDOG			(0x1U)
/*! @brief Clock names for ETIMER. */
#define ETIMER_CLOCK_NAMES    {ETIMER0_CLK,ETIMER1_CLK,ETIMER2_CLK}

/* CTU (Cross Triggering Unit) module features */

/*!
 * @brief Macro defining number of ETIMER instances connected to the CTU
 */
#define CTU_ETIMER_COUNT   			 (ETIMER_INSTANCE_COUNT)

/*! @brief Number of ADC instances which can be triggered by CTU */
#define FEATURE_CTU_NUM_ADC          (2u)

/* ADC module features */

#define FEATURE_ADC_HAS_CTU_TRIGGER_MODE (0)
#define FEATURE_ADC_HAS_EXT_TRIGGER      (1)

#define ADC_PRESAMPLE_VSS_HV            (ADC_PRESAMPLE_SRC0)    /* Presampling from High voltage ground for ADC*/
#define ADC_PRESAMPLE_VDD_HV_DIV_8      (ADC_PRESAMPLE_SRC1)    /* Presampling from ower High voltage Supply for ADC, SGEN divided by 8 */
#define ADC_PRESAMPLE_VSS_HV_ADRE1      (ADC_PRESAMPLE_SRC2)    /* Presampling from High voltage ground for ADC/TSENS */
#define ADC_PRESAMPLE_VDD_HV_ADRE1      (ADC_PRESAMPLE_SRC3)    /* Presampling from High voltage reference for ADC/TSENS */

#define ADC_CLOCKS                      {ADC0_CLK, ADC1_CLK, ADC2_CLK, ADC3_CLK}

/* @brief Number of THRHLR registers. */
#define ADC_THRHLR_COUNT                (16u)
/* @brief Default value for sample time. */
#define ADC_DEF_SAMPLE_TIME_0           (0x14U)
#define ADC_DEF_SAMPLE_TIME_1           (0x14U)
#define ADC_DEF_SAMPLE_TIME_2           (0x0U)

/* SIUL2 module feature */
/*! @brief SIUL2 module external interrupt support DMA */
#define FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA          (1)
/*! @brief SIUL2 module MSCR register has INV bit */
#define FEATURE_SIUL2_HAS_INVERT_DATA_OUTPUT            (1)
/*! @brief SIUL2 module IMCR register has INV bit */
#define FEATURE_SIUL2_HAS_INVERT_DATA_INPUT             (1)
/*! @brief SIUL2 module support Analog Pad */
#define FEATURE_SIUL2_HAS_ANALOG_PAD                    (1)
/*! @brief SIUL2 module Slew rate combine with Drive strength */
#define FEATURE_SIUL2_HAS_SLEW_RATE_WITH_DRIVE_STRENGTH (1)
/*! @brief SIUL2 module input source select bit width */
#define FEATURE_SIUL2_INPUT_SOURCE_SELECT_WIDTH         SIUL2_IMCR_SSS_WIDTH
/*! @brief SIUL2 module input mux numbers */
#define FEATURE_SIUL2_INPUT_MUX_WIDTH                   (8U)

/** GPIO - Register Layout Typedef */
typedef struct {
  __IO uint16_t PGPDO;          /**< SIUL2 Parallel GPIO Pad Data Out Register, array offset: 0x1700, array step: 0x2 */
       uint16_t RESERVED_PGPDO[SIUL2_PGPDO_COUNT - 1U];
       uint8_t RESERVED_13[44];
  __I  uint16_t PGPDI;          /**< SIUL2 Parallel GPIO Pad Data In Register, array offset: 0x1740, array step: 0x2 */
       uint16_t RESERVED_PGPDI[SIUL2_PGPDI_COUNT - 1U];
       uint8_t RESERVED_14[44];
  __O  uint32_t MPGPDO;        /**< SIUL2 Masked Parallel GPIO Pad Data Out Register, array offset: 0x1780, array step: 0x4 */
} GPIO_Type;

 /** Number of instances of the SIUL2 module. */
#define GPIO_INSTANCE_COUNT (10u)

/* SIUL2 - Peripheral instance base addresses */
/** Peripheral SIUL2 base address */
#define GPIO_BASE                                (0xFFFC1700u)
/** Peripheral SIUL2 base pointer */
#define GPIO                                     ((GPIO_Type *)GPIO_BASE)

 /* GPIO - Peripheral instance base addresses */
/** Peripheral PTA base address */
#define PTA_BASE                                 (0xFFFC1700u)
/** Peripheral PTA base pointer */
#define PTA                                      ((GPIO_Type *)PTA_BASE)
/** Peripheral PTB base address */
#define PTB_BASE                                 (0xFFFC1702u)
/** Peripheral PTB base pointer */
#define PTB                                      ((GPIO_Type *)PTB_BASE)
/** Peripheral PTC base address */
#define PTC_BASE                                 (0xFFFC1704u)
/** Peripheral PTC base pointer */
#define PTC                                      ((GPIO_Type *)PTC_BASE)
/** Peripheral PTD base address */
#define PTD_BASE                                 (0xFFFC1706u)
/** Peripheral PTD base pointer */
#define PTD                                      ((GPIO_Type *)PTD_BASE)
/** Peripheral PTE base address */
#define PTE_BASE                                 (0xFFFC1708u)
/** Peripheral PTE base pointer */
#define PTE                                      ((GPIO_Type *)PTE_BASE)
/** Peripheral PTF base address */
#define PTF_BASE                                 (0xFFFC170Au)
/** Peripheral PTF base pointer */
#define PTF                                      ((GPIO_Type *)PTF_BASE)
/** Peripheral PTG base address */
#define PTG_BASE                                 (0xFFFC170Cu)
/** Peripheral PTG base pointer */
#define PTG                                      ((GPIO_Type *)PTG_BASE)
/** Peripheral PTH base address */
#define PTH_BASE                                 (0xFFFC170Eu)
/** Peripheral PTH base pointer */
#define PTH                                      ((GPIO_Type *)PTH_BASE)
/** Peripheral PTI base address */
#define PTI_BASE                                 (0xFFFC1710u)
/** Peripheral PTI base pointer */
#define PTI                                      ((GPIO_Type *)PTI_BASE)
/** Peripheral PTJ base address */
#define PTJ_BASE                                 (0xFFFC1712u)
/** Peripheral PTJ base pointer */
#define PTJ                                      ((GPIO_Type *)PTJ_BASE)

/** Array initializer of GPIO peripheral base addresses */
#define GPIO_BASE_ADDRS                          { PTA_BASE, PTB_BASE, PTC_BASE, PTD_BASE, \
                                                   PTE_BASE, PTF_BASE, PTG_BASE, PTH_BASE, \
                                                   PTI_BASE, PTJ_BASE }
/** Array initializer of GPIO peripheral base pointers */
#define GPIO_BASE_PTRS                           { PTA, PTB, PTC, PTD, PTE, PTF, PTG, PTH, \
                                                   PTI, PTJ }

/** PORT - Register Layout Typedef */
typedef struct {
  __IO  uint32_t MSCR[16];
        uint32_t RESERVED_MSCR[SIUL2_MSCR_COUNT - 16U];
        uint8_t RESERVED_9[1428];
  __IO  uint32_t IMCR[16];
} PORT_Type;

#define SIUL2_MSCR_BASE                          (SIUL2->MSCR)
#define SIUL2_IMCR_BASE                          (SIUL2->IMCR)
#define PORTA                                    ((PORT_Type *)(SIUL2_MSCR_BASE+0x00))
#define PORTB                                    ((PORT_Type *)(SIUL2_MSCR_BASE+0x10))
#define PORTC                                    ((PORT_Type *)(SIUL2_MSCR_BASE+0x20))
#define PORTD                                    ((PORT_Type *)(SIUL2_MSCR_BASE+0x30))
#define PORTE                                    ((PORT_Type *)(SIUL2_MSCR_BASE+0x40))
#define PORTF                                    ((PORT_Type *)(SIUL2_MSCR_BASE+0x50))
#define PORTG                                    ((PORT_Type *)(SIUL2_MSCR_BASE+0x60))
#define PORTH                                    ((PORT_Type *)(SIUL2_MSCR_BASE+0x70))
#define PORTI                                    ((PORT_Type *)(SIUL2_MSCR_BASE+0x80))
#define PORTJ                                    ((PORT_Type *)(SIUL2_MSCR_BASE+0x90))
#define PORT_BASE_PTRS                           { PORTA, PORTB, PORTC, PORTD, PORTE, PORTF, \
                                                   PORTG, PORTH, PORTI, PORTJ }

/*                 PA0,  PA1,  PA2,  PA3,  PA4,  PA5,  PA6,  PA7,  PA8, PA10, PA11, PA12, PA13, PA14, PA15,  PB0,*/
#define PORT_IRQS  { 0,    1,    2,    3,    4,    5,    6,    7,    8,   10,   11,   12,   13,   14,   15,   16, \
                    17,   18,   22,   30,   31,  104,   36,   37,   38,   77,   78,   79,   80,  105,   92,   93 }
/*                 PB1,  PB2,  PB6, PB14, PB15,  PG8,  PC4,  PC5,  PC6, PE13, PE14, PE15,  PF0,  PG9, PF12, PF13 */


/* ENET module features */

/*! @brief ENET peripheral clock names */
#define FEATURE_ENET_CLOCK_NAMES { ENET0_CLK }

/*! @brief The transmission interrupts */
#define FEATURE_ENET_TX_IRQS        ENET_TX_IRQS
/*! @brief The reception interrupts */
#define FEATURE_ENET_RX_IRQS        ENET_RX_IRQS
/*! @brief The error interrupts */
#define FEATURE_ENET_ERR_IRQS       ENET_ERR_IRQS
/*! @brief The timer interrupts */
#define FEATURE_ENET_TIMER_IRQS     ENET_TIMER_IRQS

/*! @brief The maximum supported frequency for MDC, in Hz. */
#define FEATURE_ENET_MDC_MAX_FREQUENCY 2500000U

/*! @brief Minimum hold time on the MDIO output, in nanoseconds. */
#define FEATURE_ENET_MDIO_MIN_HOLD_TIME_NS 10U

/*! @brief Definitions used for aligning the data buffers */
#define FEATURE_ENET_BUFF_ALIGNMENT      (16UL)
/*! @brief Definitions used for aligning the buffer descriptors */
#define FEATURE_ENET_BUFFDESCR_ALIGNMENT (64UL)

/*! @brief Has receive frame parser feature. */
#define FEATURE_ENET_HAS_RECEIVE_PARSER (0)

/*! @brief Has enhanced buffer descriptors programming model. */
#define FEATURE_ENET_HAS_ENHANCED_BD    (1)

/*! @brief Default configuration for the PHY interface */
#define FEATURE_ENET_DEFAULT_PHY_IF     ENET_MII_MODE

/*! @brief Has adjustable timer enabling IEEE 1588 support */
#define FEATURE_ENET_HAS_ADJUSTABLE_TIMER (1)

/*! @brief Has support for configuring the width of the output compare pulse */
#define FEATURE_ENET_HAS_TIMER_PULSE_WIDTH_CONTROL (0)

/*! @brief The number of available receive and transmit buffer descriptor rings */
#define FEATURE_ENET_RING_COUNT             (1U)

/*! @brief The number of available VLAN priority compare values per class */
#define FEATURE_ENET_CLASS_MATCH_CMP_COUNT  (0U)

/*! @brief Has credit-based shaper */
#define FEATURE_ENET_HAS_CBS                (0)

/*! @brief Has time-based shaper */
#define FEATURE_ENET_HAS_TBS                (0)

/*! @brief Has interrupt coalescing */
#define FEATURE_ENET_HAS_INTCOAL            (0)

/*! @brief Has RGMII mode for data interface */
#define FEATURE_ENET_HAS_RGMII              (0)

/*! @brief Has 1000-Mbit/s speed mode */
#define FEATURE_ENET_HAS_SPEED_1000M        (0)

/* CAN module features */

/* @brief Frames available in Rx FIFO flag shift */
#define FEATURE_CAN_RXFIFO_FRAME_AVAILABLE  (5U)
/* @brief Rx FIFO warning flag shift */
#define FEATURE_CAN_RXFIFO_WARNING          (6U)
/* @brief Rx FIFO overflow flag shift */
#define FEATURE_CAN_RXFIFO_OVERFLOW         (7U)
/* @brief Maximum number of Message Buffers supported for payload size 8 for CAN0 */
#define FEATURE_CAN0_MAX_MB_NUM             (64U)
/* @brief Maximum number of Message Buffers supported for payload size 8 for CAN1 */
#define FEATURE_CAN1_MAX_MB_NUM             (64U)
/* @brief Maximum number of Message Buffers supported for payload size 8 for CAN2 */
#define FEATURE_CAN2_MAX_MB_NUM             (64U)
/* @brief Maximum number of Message Buffers supported for payload size 8 for any of the CAN instances */
#define FEATURE_CAN_MAX_MB_NUM              (64U)
/* @brief Array of maximum number of Message Buffers supported for payload size 8 for all the CAN instances */
#define FEATURE_CAN_MAX_MB_NUM_ARRAY        { FEATURE_CAN0_MAX_MB_NUM, \
                                              FEATURE_CAN1_MAX_MB_NUM, \
                                              FEATURE_CAN2_MAX_MB_NUM }
/* @brief Has PE clock source select (bit field CAN_CTRL1[CLKSRC]). */
#define FEATURE_CAN_HAS_PE_CLKSRC_SELECT            (1)
/* @brief Has Pretending Networking mode */
#define FEATURE_CAN_HAS_PRETENDED_NETWORKING        (0)
/* @brief Has Stuff Bit Count Enable Bit */
#define FEATURE_CAN_HAS_STFCNTEN_ENABLE             (0)
/* @brief Has ISO CAN FD Enable Bit */
#define FEATURE_CAN_HAS_ISOCANFDEN_ENABLE           (1)
/* @brief Has Message Buffer Data Size Region 1 */
#define FEATURE_CAN_HAS_MBDSR1                      (1)
/* @brief Has Message Buffer Data Size Region 2 */
#define FEATURE_CAN_HAS_MBDSR2                      (0)
/* @brief Has Supervisor Mode MCR[SUPV]*/
#define FEATURE_CAN_HAS_SUPV             	        (1)
/* @brief Has DMA enable (bit field MCR[DMA]). */
#define FEATURE_CAN_HAS_DMA_ENABLE                  (0)
/* @brief Maximum number of Message Buffers IRQs */
#define FEATURE_CAN_MB_IRQS_MAX_COUNT       (9U)
/* @brief Has Wake Up Irq channels (CAN_Wake_Up_IRQS_CH_COUNT > 0u) */
#define FEATURE_CAN_HAS_WAKE_UP_IRQ         (0U)
/* @brief Message Buffers IRQs */
#define FEATURE_CAN_MB_IRQS                 { CAN_ORed_00_03_MB_IRQS, \
                                              CAN_ORed_04_07_MB_IRQS, \
                                              CAN_ORed_08_11_MB_IRQS, \
                                              CAN_ORed_12_15_MB_IRQS, \
                                              CAN_ORed_16_31_MB_IRQS, \
                                              CAN_ORed_32_39_MB_IRQS, \
                                              CAN_ORed_40_47_MB_IRQS, \
                                              CAN_ORed_48_55_MB_IRQS, \
                                              CAN_ORed_56_63_MB_IRQS }
/*! @brief FlexCAN Embedded RAM address offset */
#define FEATURE_CAN_RAM_OFFSET              (0x00000080u)
/* @brief Has Self Wake Up mode */
#define FEATURE_CAN_HAS_SELF_WAKE_UP        (0)
/* @brief Has Flexible Data Rate */
#define FEATURE_CAN_HAS_FD                  (0)
#define CAN_IMASK1_BUF31TO0M_MASK           CAN_IMASK1_BUFLM_MASK
#define CAN_IMASK2_BUF63TO32M_MASK          CAN_IMASK2_BUFHM_MASK
/* @brief Clock name for the PE oscillator clock source */
#define FEATURE_CAN_PE_OSC_CLK_NAME         XOSC_CLK
/* @bried FlexCAN has Detection And Correction of Memory Errors */
#define FEATURE_CAN_HAS_MEM_ERR_DET			(0)

/* DSPI module feature */

/* Define the PCS allocation for each DSPI/SPI module */
#define FEATURE_SPI_0_PCS_NUMBER 8
#define FEATURE_SPI_1_PCS_NUMBER 8
#define FEATURE_SPI_2_PCS_NUMBER 4
#define FEATURE_SPI_3_PCS_NUMBER 4
#define FEATURE_DSPI_PCS_MAPPING {FEATURE_SPI_0_PCS_NUMBER, FEATURE_SPI_1_PCS_NUMBER, \
                                 FEATURE_SPI_2_PCS_NUMBER, FEATURE_SPI_3_PCS_NUMBER}

/* Define the instance realocation */
#define FEATURE_DSPI_INSTANCES  {SPI_0, SPI_1, SPI_2, SPI_3}

/* Define the initial values for state structures */
#define FEATURE_DSPI_INITIAL_STATE {NULL, NULL, NULL, NULL}

/* Define clock sources for SPI/DSPI instances */
#define FEATURE_DSPI_CLOCK_MAPPING {SPI0_CLK, SPI1_CLK, SPI2_CLK, SPI3_CLK}

/* Define margins of instances interval */
#define SPI_LOWEST_INSTANCE 0U
#define SPI_HIGHEST_INSTANCE 3U

/* Define FIFO size */
#define DSPI_FIFO_SIZE 5U

/* Define number of DSPI instances */
#define DSPI_INSTANCE_COUNT         0U

/* Define bases for DSPI interrupts */
#define FEATURES_DSPI_SEND_INTERUPT_BASE 261U
#define FEATURES_DSPI_RECEIVE_INTERUPT_BASE 263U
#define FEATURES_DSPI_FAULT_INTERUPT_BASE 259U

/* SRX module features */

/* Maximum Fast messages FIFO depth */
#define FEATURE_SRX_MAX_FIFO_SIZE (4U)

/* FlexPWN module features */

/* Define clock sources for the current platform */
#define FLEXPWM_CLOCK_NAMES    {FLEXPWM0_CLK, FLEXPWM1_CLK}

#endif /* MPC5744P_FEATURES_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
