/*
 *######################################################################
 *                                RAppIDJDP
 *           Rapid Application Initialization and Documentation Tool
 *                         Freescale Semiconductor Inc.
 *
 *######################################################################
 *
 * Project Name           : baseconfigMPC5606S_for_CQCYM
 *
 * Project File           : baseconfigMPC5606S_for_CQCYM.rsp
 *
 * Revision Number        : 8.5
 *
 * Tool Version           : 1.0.0.3
 *
 * file                   : flexcan_init.h
 *
 * Target Compiler        : Codewarrior
 *
 * Target Part            : MPC5606S
 *
 * Part Errata Fixes      : none
 *
 * Project Last Save Date : 01-May-2014 11:42:45
 *
 * Created on Date        : 01-May-2014 11:45:39
 *
 * Brief Description      : This file contains functions declaration of FlexCAN code file
 *
 *
 *######################################################################
*/

#ifndef  _FLEXCAN_INIT_H
#define  _FLEXCAN_INIT_H
/********************  Dependent Include files here **********************/

#include "derivative.h" /* include peripheral declarations */


/*********************  Receive FIFO Macro here ****************************/
#define FLEXCAN_0_RXFIFO     (*(volatile struct FLEXCAN_RXFIFO_t *)   &CAN_0.BUF[0])

typedef enum {
ExtendFrame = 1,
StandardFrame = 0
}CAN_FrameType;

typedef enum {
RemoteFrame = 1,
DataFrame = 0
}CAN_DataType;

/** struct used for CAN message store **/
typedef struct {
	CAN_FrameType Frame_Type;          /* CAN frame type */
	uint32_t Frame_CODE;               /* CAN frame message buffer code */
	uint32_t Frame_ID;                 /* CAN frame message ID */
	CAN_DataType Frame_DataType;       /* is it a remote frame data */
	uint32_t Frame_Length;             /* CAN frame message number of data bytes */
	uint8_t  Frame_Data[8];            /* CAN frame message data string*/
	uint32_t Frame_TIMESTAMP;          /* CAN frame message time */
}CAN_Msg_Strcut;

/** for FlexCAN Message Buffer select**/
typedef enum{
	MB0 = 0,
	MB1 = 1,
	MB2 = 2,
	MB3 = 3,
	MB4 = 4,
	MB5 = 5,
	MB6 = 6,
	MB7 = 7,
	MB8 = 8,
	MB9 = 9,
	MB10 = 10,
	MB11 = 11,
	MB12 = 12,
	MB13 = 13,
	MB14 = 14,
	MB15 = 15,
	MB16 = 16,
	MB17 = 17,
	MB18 = 18,
	MB19 = 19,
	MB20 = 20,
	MB21 = 21,
	MB22 = 22,
	MB23 = 23,
	MB24 = 24,
	MB25 = 25,
	MB26 = 26,
	MB27 = 27,
	MB28 = 28,
	MB29 = 29,
	MB30 = 30,
	MB31 = 31,
	MB32 = 32,
	MB33 = 33,
	MB34 = 34,
	MB35 = 35,
	MB36 = 36,
	MB37 = 37,
	MB38 = 38,
	MB39 = 39,
	MB40 = 40,
	MB41 = 41,
	MB42 = 42,
	MB43 = 43,
	MB44 = 44,
	MB45 = 45,
	MB46 = 46,
	MB47 = 47,
	MB48 = 48,
	MB49 = 49,
	MB50 = 50,
	MB51 = 51,
	MB52 = 52,
	MB53 = 53,
	MB54 = 54,
	MB55 = 55,
	MB56 = 56,
	MB57 = 57,
	MB58 = 58,
	MB59 = 59,
	MB60 = 60,
	MB61 = 61,
	MB62 = 62,
	MB63 = 63
}CAN_MessageBuffer;

/**********************  Function Prototype here *************************/


uint8_t flexcan_0_deinit_fnc(void);
uint8_t flexcan_1_deinit_fnc(void);

uint8_t flexcan_0_init_fnc();
uint8_t flexcan_1_init_fnc(void);

CAN_Msg_Strcut CAN0_ReceiveMsg( CAN_MessageBuffer Buffer);

void Load_ReponseToUpMachine(uint8_t *data,uint8_t length);


#endif  /*_FLEXCAN_INIT_H*/

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

