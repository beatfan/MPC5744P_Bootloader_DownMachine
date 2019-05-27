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
 * file                   : flexcan_init.c
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
 * Brief Description      : This file contains FlexCAN register configuration in freeze mode
 *
 ******************************************************************************** 
 *
 * Detail Description     : File contains functions where each FlexCAN configuration for
 *                          maximum number of MBs, CAN speed, different operating modes,
 *                          Receive Mask and Interrupt Mask can be done in freeze mode.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "flexcan_init.h"

#define G_ReceiveCANID  0x00c18064U
#define G_SendCANID    0x00a180c8U

/*    CAN_1_RximrArray Intialisation. 
     It is used to intialise CAN_1.RX Individual Mask0 to 
    CAN_1.RX Individual Mask63 register.                 */

const static uint32_t CAN_1_RximrArrayt[64] = {
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX00,RX01,RX02,RX03,*/
0xFFFFFFF0,0x00000000,0xFFFFFFF0,0x00000000,  /*RX04,RX05,RX06,RX07,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX08,RX09,RX10,RX11,*/
0x00000000,0x00000000,0xFFFFFFFC,0xFFFFFFFB,  /*RX12,RX13,RX14,RX15,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX16,RX17,RX18,RX19,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX20,RX21,RX22,RX23,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX24,RX25,RX26,RX27,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX28,RX29,RX30,RX31,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX32,RX33,RX34,RX35,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX36,RX37,RX38,RX39,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX40,RX41,RX42,RX43,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX44,RX45,RX46,RX47,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX48,RX49,RX50,RX51,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX52,RX53,RX54,RX55,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX56,RX57,RX58,RX59,*/
0x00000000,0x00000000,0x00000000,0x00000000,  /*RX60,RX61,RX62,RX63,*/
};

/*********************  Initialization Function(s) ************************/

void CAN0_ReceiveInit(CAN_MessageBuffer Buffer,CAN_FrameType FrameType, CAN_DataType DataType, uint32_t ID, uint32_t ID_Mask);
void CAN1_ReceiveInit(CAN_MessageBuffer Buffer,CAN_FrameType FrameType, CAN_DataType DataType, uint32_t ID, uint32_t ID_Mask);

/*
 * FlexCAN0 de-initialize function
 * reset its registers and shutdown the peripheral module
 */
uint8_t flexcan_0_deinit_fnc(void)
{
	/* Reset FlexCAN Module */
	CAN_0.MCR.B.SOFTRST = 1;
	while( CAN_0.MCR.B.SOFTRST ==1){}

	/* Set FRZ bit */
	CAN_0.MCR.B.FRZ       = 1;
	while( CAN_0.MCR.B.FRZACK == 0){}

	CAN_0.MCR.B.MDIS = 1;       /* Disable the module */

	return 0;

}

/*
 * FlexCAN1 de-initialize function
 * reset its registers and shutdown the peripheral module
 */
uint8_t flexcan_1_deinit_fnc(void)
{
	/* Reset FlexCAN Module */
	CAN_1.MCR.B.SOFTRST = 1;
	while( CAN_0.MCR.B.SOFTRST ==1){}

	/* Set FRZ bit */
	CAN_1.MCR.B.FRZ       = 1;
	while( CAN_1.MCR.B.FRZACK == 0){}

	CAN_1.MCR.B.MDIS = 1;       /* Disable the module */

	return 0;

}

/*********************  CAN_0 Initialization Function(s) ************************/

uint8_t flexcan_0_init_fnc()
{

    uint16_t i, x;
    uint8_t status;
    uint16_t j;

    CAN_0.MCR.B.MDIS = 1;       /* Disable module before selecting clock source*/
    CAN_0.CTRL1.B.CLKSRC=0;     /* Clock Source = oscillator clock (40 MHz) */

    /*enabling the CAN module */
    CAN_0.MCR.B.MDIS    = 0;

    /* Reset FlexCAN Module */
    CAN_0.MCR.B.SOFTRST = 1;
    while( CAN_0.MCR.B.SOFTRST ==1){}

    /* Set FRZ bit */
    CAN_0.MCR.B.FRZ       = 1;
    while( CAN_0.MCR.B.FRZACK == 0){}

    /* Set CAN clock source 1: bus , 0: oscillator */
    CAN_0.CTRL1.B.CLKSRC   = 0;


/* ----------------------------------------------------------- */
/*                   CAN_0.MCR Control Register                          */
/* ----------------------------------------------------------- */
    CAN_0.MCR.R = 0x5983000F;

    /*Enable the FlexCAN module: 0*/
    /* FRZ bit :1*/
    /* FIFO Enable :0*/
    /* Halt : 1*/
    /* FlexCAN not ready: 1*/
    /* Soft Reset : 0*/
    /* Freeze Mode Acknowledge : 1*/
    /* Supervisor Mode : 1*/
    /* Warning Interrupt Enable : 0*/
    /* Low Power Mode Acknowledge : 0*/
    /* Doze Mode Enable : 0*/
    /* Self Reception Disable : 1*/
    /* Backwards Compatibility Configuration : 0*/
    /* Local Priority Enable : 0*/
    /* Abort Enable : 0*/
    /* ID Acceptance Mode : 0*/
    /* Maximum Number of Message Buffers : 15*/

/* ----------------------------------------------------------- */
/*    CAN_0.CR Control Register                                */
/* ----------------------------------------------------------- */
    CAN_0.CTRL1.R  = 0x0414C036;
    /* Bus -Off Interrupt Mask: Enabled         */
    /* CAN bit Sampling Mode: 1 Sample           */
    /* CAN Error Interrupt Mask: Enabled        */
    /* CLOCK Source: Oscillator                   */
    /* Listen Only Mode: Disabled                */
    /* Loop Back Mode: Disabled                  */
    /* Bus off Recovery: Enabled                */
    /* Lowest Buffer Transfer First: Enabled    */
    /* Phase Segment1: 3                         */
    /* Phase Segment2: 5                         */
    /* Prescalar Division Factor: 5(40MHz/5==8MHz--> 8/16= 500kbps)  */
    /* Propagation Segment: 7                    */
    /* Resynchronization Jump Width: 1           */
    /* Timer Synch Mode: Enabled                */



    /* Set the FlexCAN Maximum Buffer Number */
    CAN_0.MCR.B.MAXMB  = 16;


    /* clear memory from message buffer 8 to 15 */
    for(x=8; x < 16; x++)
    {
        CAN_0.MB[x].CS.R = 0;
        CAN_0.MB[x].ID.R = 0;
        for(i=0; i < 2; i++) 
        {
            CAN_0.MB[x].DATA.W[i] = 0;
        }
    }

/* ----------------------------------------------------------- */
/*    CAN_0.IMASK2 Interrupt Masks High Register                 */
/* ----------------------------------------------------------- */
    CAN_0.IMASK2.R  = 0x00000000;

/* ----------------------------------------------------------- */
/*    CAN_0.IMASK1 Interrupt Masks Low Register                  */
/* ----------------------------------------------------------- */

    CAN_0.IFLAG1.R  = 0x000000F0;/*clean the flags first*/
    CAN_0.IMASK1.R  = 0x000000F0;
    
    //CAN0_ReceiveInit(MB4, StandardFrame, DataFrame, 0x123,0xFF0);
    CAN0_ReceiveInit(MB6, ExtendFrame, DataFrame, G_ReceiveCANID,0xFFFFFFFF);/*receive only standard ID 0x64 for bootloader download CAN frame ID*/

    /* Negate the HALT and the FREEZE bit to let the FlexCAN synchronize. */
    /* Exit and Disable freeze mode */
    CAN_0.MCR.B.HALT  =  0;
    CAN_0.MCR.B.FRZ   =  0;
    

    /* await synchronization (delay) */
    for(j=1; j < 255; j++){}

    if(CAN_0.MCR.B.NOTRDY == 1)
    {
       status = 1;
    }
    else
    {
       status = 0;
    }


    return status;

}
/*********************  CAN_1 Initialization Function(s) ************************/

uint8_t flexcan_1_init_fnc(void)
{

    uint16_t i, x;
    uint8_t status;
    uint16_t j;

    /*enabling the CAN module */
    CAN_1.MCR.B.MDIS    = 0;

    /* Reset FlexCAN Module */
    CAN_1.MCR.B.SOFTRST = 1;
    while( CAN_1.MCR.B.SOFTRST ==1){}

    /* Set FRZ bit */
    CAN_1.MCR.B.FRZ       = 1;
    while( CAN_1.MCR.B.FRZACK == 0){}

    /* Set CAN clock source 1: bus , 0: oscillator */
    CAN_1.CTRL1.B.CLKSRC   = 0;


    /* ----------------------------------------------------------- */
    /*                   CAN_1.MCR Control Register                          */
    /* ----------------------------------------------------------- */
        CAN_1.MCR.R = 0x5983000F;

        /*Enable the FlexCAN module: 0*/
        /* FRZ bit :1*/
        /* FIFO Enable :0*/
        /* Halt : 1*/
        /* FlexCAN not ready: 1*/
        /* Soft Reset : 0*/
        /* Freeze Mode Acknowledge : 1*/
        /* Supervisor Mode : 1*/
        /* Warning Interrupt Enable : 0*/
        /* Low Power Mode Acknowledge : 0*/
        /* Doze Mode Enable : 0*/
        /* Self Reception Disable : 1*/
        /* Backwards Compatibility Configuration : 0*/
        /* Local Priority Enable : 0*/
        /* Abort Enable : 0*/
        /* ID Acceptance Mode : 0*/
        /* Maximum Number of Message Buffers : 15*/

    /* ----------------------------------------------------------- */
    /*    CAN_1.CR Control Register                                */
    /* ----------------------------------------------------------- */
        CAN_1.CTRL1.R  = 0x0414C016;
        /* Bus -Off Interrupt Mask: Enabled         */
        /* CAN bit Sampling Mode: 1 Sample           */
        /* CAN Error Interrupt Mask: Enabled        */
        /* CLOCK Source: Oscillator                   */
        /* Listen Only Mode: Disabled                */
        /* Loop Back Mode: Disabled                  */
        /* Bus off Recovery: Enabled                */
        /* Lowest Buffer Transfer First: Enabled    */
        /* Phase Segment1: 3                         */
        /* Phase Segment2: 5                         */
        /* Prescalar Division Factor: 5(40MHz/5==8MHz-->500kbps)               */
        /* Propagation Segment: 7                    */
        /* Resynchronization Jump Width: 1           */
        /* Timer Synch Mode: Disabled                */



        /* Set the FlexCAN Maximum Buffer Number */
        CAN_1.MCR.B.MAXMB  = 16;


        /* clear memory from message buffer 0 to 15 */
        for(x=0; x < 16; x++)
        {
            CAN_1.MB[x].CS.R = 0;
            CAN_1.MB[x].ID.R = 0;
            for(i=0; i < 2; i++) 
            {
                CAN_1.MB[x].DATA.W[i] = 0;
            }
        }


    /* ----------------------------------------------------------- */
    /*    CAN_1.RX Mask Individual Mask                             */
    /* ----------------------------------------------------------- */
        for(x=0 ; x<64 ; x++)
        {
            CAN_1.RXIMR[x].R = CAN_1_RximrArrayt[x];
        }

    /* ----------------------------------------------------------- */
    /*    CAN_1.IMASK2 Interrupt Masks High Register                 */
    /* ----------------------------------------------------------- */
        CAN_1.IMASK2.R  = 0x00000000;

    /* ----------------------------------------------------------- */
    /*    CAN_1.IMASK1 Interrupt Masks Low Register                  */
    /* ----------------------------------------------------------- */

        CAN_1.IFLAG1.R = 0x0000FFFF;/*clean the flags first*/
        CAN_1.IMASK1.R = 0x0000FFFF;
    
    
    CAN1_ReceiveInit(MB4,StandardFrame, DataFrame, 0x333, 0xFF0);  /*receive 0x330~0x33F, total 16 ID CAN Frame */
    CAN1_ReceiveInit(MB6, ExtendFrame, RemoteFrame, 0x111,0xFFFFFFF0);  /*receive 0x110~0x11F, total 16 ID CAN Frame */
    

    CAN1_ReceiveInit(MB14, StandardFrame, DataFrame, 0x222,0xFFC); /*receive 0x220~0x223, total 4 ID CAN Frame */
    CAN1_ReceiveInit(MB15, ExtendFrame, RemoteFrame, 0x1111666,0xFFFFFFFB); /*receive 0x111662 and 0x111666, total 2 ID CAN Frame */
    

    /* Negate the HALT and the FREEZE bit to let the FlexCAN synchronize. */
    /* Exit and Disable freeze mode */
    CAN_1.MCR.B.HALT  =  0;
    CAN_1.MCR.B.FRZ   =  0;
    

    /* await synchronization (delay) */
    for(j=1; j < 255; j++){}

    if(CAN_1.MCR.B.NOTRDY == 1)
    {
       status = 1;
    }
    else
    {
       status = 0;
    }


    return status;

}

 
/***********************************************************************
 * call this function for CAN0 data transmit
 * Buffer: select the MB to transmit the CAN message(MB0~MB63)
 * CAN_Meg: CAN message to send
 ***********************************************************************/
void CAN0_TransmitMsg (CAN_MessageBuffer Buffer, CAN_Msg_Strcut CAN_Msg)
{
	  uint8_t	i;

	  /* Assumption:  Message buffer CODE is INACTIVE */
	  CAN_0.MB[Buffer].CS.B.IDE = CAN_Msg.Frame_Type;           /* Frame type setting */

	  if( CAN_Msg.Frame_Type == StandardFrame)
	  	  CAN_0.MB[Buffer].ID.B.ID_STD = CAN_Msg.Frame_ID;      /* Transmit ID       */
	  else
	  {
		  CAN_0.MB[Buffer].ID.R = CAN_Msg.Frame_ID;      		 /* Transmit ID       */
		  CAN_0.MB[Buffer].CS.B.SRR = 1;           /* Tx frame (not req'd for standard frame)*/
	  }

	  CAN_0.MB[Buffer].CS.B.RTR = CAN_Msg.Frame_DataType;       /* Data frame, not remote Tx request frame */
	  CAN_0.MB[Buffer].CS.B.DLC = sizeof(CAN_Msg.Frame_Data); /* # bytes to transmit w/o null */
	  for (i=0; i<sizeof(CAN_Msg.Frame_Data); i++) {
		CAN_0.MB[Buffer].DATA.B[i] = CAN_Msg.Frame_Data[i];      /* Data to be transmitted */
	  }


	  CAN_0.MB[Buffer].CS.B.CODE =0xC;         /* Activate msg. buf. to transmit data frame */
}

/***********************************************************************
 * call this function for CAN1 data transmit
 * Buffer: select the MB to transmit the CAN message(MB0~MB63)
 * CAN_Meg: CAN message to send
 ***********************************************************************/
void CAN1_TransmitMsg (CAN_MessageBuffer Buffer, CAN_Msg_Strcut CAN_Msg)
{
	uint8_t	i;

	/* Assumption:  Message buffer CODE is INACTIVE */

	  CAN_1.MB[Buffer].CS.B.IDE = CAN_Msg.Frame_Type;           /* Frame type setting */

	  if( CAN_Msg.Frame_Type == StandardFrame)
		  CAN_1.MB[Buffer].ID.B.ID_STD = CAN_Msg.Frame_ID;      /* Transmit ID       */
	  else
	  {
		  CAN_1.MB[Buffer].ID.R = CAN_Msg.Frame_ID;      		 /* Transmit ID       */
		  CAN_1.MB[Buffer].CS.B.SRR = 1;           /* Tx frame (not req'd for standard frame)*/
	  }


	  CAN_1.MB[Buffer].CS.B.RTR = CAN_Msg.Frame_DataType;       /* Data frame, not remote Tx request frame */
	  CAN_1.MB[Buffer].CS.B.DLC = sizeof(CAN_Msg.Frame_Data); /* # bytes to transmit w/o null */
	  for (i=0; i<sizeof(CAN_Msg.Frame_Data); i++) {
		CAN_1.MB[Buffer].DATA.B[i] = CAN_Msg.Frame_Data[i];      /* Data to be transmitted */
	  }

	  CAN_1.MB[Buffer].CS.B.CODE =0xC;         /* Activate msg. buf. to transmit data frame */
}


/********************************************************************************
 * call this function for CAN0 receive enable
 * Buffer: select the MB to transmit the CAN message (MB0~MB63)
 * FrameType: set the receive CAM message frame type(ExtendFrame or StandardFrame)
 * DataType: set the receive CAM message data type(RemoteFrame or DataFrame)
 * ID: the receive CAN message ID
 * ID_Mask: the receive CAN message ID mask
 ********************************************************************************/
void CAN0_ReceiveInit(CAN_MessageBuffer Buffer,CAN_FrameType FrameType, CAN_DataType DataType, uint32_t ID, uint32_t ID_Mask)
{
    /**********for receive **********/
    CAN_0.MB[Buffer].CS.B.IDE = FrameType;  /* MB buffer will look for a standard ID */
    CAN_0.MB[Buffer].CS.B.RTR = DataType;   /* receive remote frame or data frame*/

    if(FrameType == StandardFrame)
    {
       CAN_0.MB[Buffer].ID.B.ID_STD = ID; 	/* MB buffer will look for standard ID of ID */
       CAN_0.RXIMR[Buffer].R = ID_Mask<<18; /* set individual receive match ID mask */
    }
    else
    {
       CAN_0.MB[Buffer].ID.R = ID; 	    /* MB buffer will look for extend ID of ID */
       CAN_0.RXIMR[Buffer].R = ID_Mask;     /* set individual receive match ID mask */
    }

   CAN_0.MB[Buffer].CS.B.CODE = 4;     	/* MB buffer set to RX EMPTY */

}

/********************************************************************************
 * call this function for CAN1 receive enable
 * Buffer: select the MB to transmit the CAN message (MB0~MB63)
 * FrameType: set the receive CAM message frame type(ExtendFrame or StandardFrame)
 * DataType: set the receive CAM message data type(RemoteFrame or DataFrame)
 * ID: the receive CAN message ID
 * ID_Mask: the receive CAN message ID mask
 ********************************************************************************/
void CAN1_ReceiveInit(CAN_MessageBuffer Buffer, CAN_FrameType FrameType, CAN_DataType DataType, uint32_t ID, uint32_t ID_Mask)
{
    /**********for receive **********/
    CAN_1.MB[Buffer].CS.B.IDE = FrameType;     /* MB buffer will look for a FrameType(standard/extend) ID */
    CAN_1.MB[Buffer].CS.B.RTR = DataType;      /* receive remote frame or data frame*/

    if(FrameType == StandardFrame)
    {
        CAN_1.MB[Buffer].ID.B.ID_STD = ID; 	/* MB buffer will look for standard ID of ID */
        CAN_1.RXIMR[Buffer].R = ID_Mask<<18;    /* set individual receive match ID mask */
    }
    else
    {
        CAN_1.MB[Buffer].ID.R = ID; 	        /* MB buffer will look for extend ID of ID */
        CAN_1.RXIMR[Buffer].R = ID_Mask;        /* set individual receive match ID mask */
    }

        CAN_1.MB[Buffer].CS.B.CODE = 4;        /* MB buffer set to RX EMPTY */
}


CAN_Msg_Strcut CAN_TxMsg;
void Load_ReponseToUpMachine(uint8_t *data,uint8_t length)
{
	uint8_t i;
	CAN_TxMsg.Frame_ID = G_SendCANID;
	CAN_TxMsg.Frame_DataType = DataFrame;
	CAN_TxMsg.Frame_Type = ExtendFrame;
	CAN_TxMsg.Frame_Length = length;
	for(i=0;i<length;i++)
		CAN_TxMsg.Frame_Data[i] = data[i];

   /* feedback UP_BUSY immediately after a valid frame received  */
	CAN0_TransmitMsg(MB0,CAN_TxMsg);

}


/********************************************************************************
 * call this function read the received CAN message of CAN0
 * Buffer: select the MB to read the CAN message (MB0~MB63)
 * Return: the received CAN message in CAN message store struct
 * Note:
 *      this function should be called in CAN receive ISR to get the received CAN
 *      message and unlock the MB for next receive
 ********************************************************************************/
CAN_Msg_Strcut CAN0_ReceiveMsg( CAN_MessageBuffer Buffer)
{
	  uint8_t j;
	  uint32_t dummy;

	  CAN_Msg_Strcut CAN_Msg;

	  CAN_Msg.Frame_CODE   = CAN_0.MB[Buffer].CS.B.CODE;    /* Read CODE, ID, LENGTH, DATA, TIMESTAMP */
	  dummy  = CAN_0.MB[Buffer].CS.B.IDE;
	  if(dummy == ExtendFrame)
	  {
		  CAN_Msg.Frame_Type = ExtendFrame;
		  CAN_Msg.Frame_ID   = CAN_0.MB[Buffer].ID.R;
	  }
	  else
	  {
		  CAN_Msg.Frame_Type = StandardFrame;
		  CAN_Msg.Frame_ID   = CAN_0.MB[Buffer].ID.B.ID_STD;
	  }
	  CAN_Msg.Frame_DataType = CAN_0.MB[Buffer].CS.B.RTR;
	  CAN_Msg.Frame_Length = CAN_0.MB[Buffer].CS.B.DLC;
	  for (j=0; j<CAN_Msg.Frame_Length; j++)
	  {
	    CAN_Msg.Frame_Data[j] = CAN_0.MB[Buffer].DATA.B[j];
	  }
	  CAN_Msg.Frame_TIMESTAMP = CAN_0.MB[Buffer].CS.B.TIMESTAMP;
	  dummy = CAN_0.TIMER.R;                /* Read TIMER to unlock message buffers */
	  CAN_0.IFLAG1.R = (uint32_t)(1 <<Buffer);/* Clear CAN1 MB buffer flag */


	  return CAN_Msg;
}



/********************************************************************************
 * call this function read the received CAN message of CAN1
 * Buffer: select the MB to read the CAN message (MB0~MB63)
 * Return: the received CAN message in CAN message store struct
 * Note:
 *      this function should be called in CAN receive ISR to get the received CAN
 *      message and unlock the MB for next receive
 ********************************************************************************/
CAN_Msg_Strcut CAN1_ReceiveMsg( CAN_MessageBuffer Buffer)
{
	  uint8_t j;
	  uint32_t dummy;

	  CAN_Msg_Strcut CAN_Msg;

	  CAN_Msg.Frame_CODE   = CAN_1.MB[Buffer].CS.B.CODE;    /* Read CODE, ID, LENGTH, DATA, TIMESTAMP */
	  dummy  = CAN_1.MB[Buffer].CS.B.IDE;
	  if(dummy == ExtendFrame)
	  {
	  	CAN_Msg.Frame_Type = ExtendFrame;
	  	CAN_Msg.Frame_ID   = CAN_1.MB[Buffer].ID.R;
	  }
	  else
	  {
	  	CAN_Msg.Frame_Type = StandardFrame;
	  	CAN_Msg.Frame_ID   = CAN_1.MB[Buffer].ID.B.ID_STD;
	  }
	  CAN_Msg.Frame_DataType = CAN_1.MB[Buffer].CS.B.RTR;
	  CAN_Msg.Frame_Length = CAN_1.MB[Buffer].CS.B.DLC;
	  for (j=0; j<CAN_Msg.Frame_Length; j++)
	  {
	    CAN_Msg.Frame_Data[j] = CAN_1.MB[Buffer].DATA.B[j];
	  }
	  CAN_Msg.Frame_TIMESTAMP = CAN_1.MB[Buffer].CS.B.TIMESTAMP;
	  dummy = CAN_1.TIMER.R;                /* Read TIMER to unlock message buffers */
	  CAN_1.IFLAG1.R = (uint32_t)(1 <<Buffer);/* Clear CAN1 MB buffer flag */

	  return CAN_Msg;
}

