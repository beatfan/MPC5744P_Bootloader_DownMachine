/*
 * MPC574xP_SystemInit.c
 *
 *  Created on: 2017Äê11ÔÂ3ÈÕ
 *      Author: pc
 */

#include "derivative.h" /* include peripheral declarations */

#include "MPC574xP_SystemInit.h"

/*******************************************************************************
Function Name : Sys_Init
Engineer      : b21190
Date          : Oct-22-2014
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Clock settings
Issues        : NONE
*******************************************************************************/
void Sys_Init(void)
{
    //Clk Out
    MC_CGM.AC6_SC.R = 0x02000000;	//Enable PLL0 - clkout0 //MC_CGM_AC6_SC
    MC_CGM.AC6_DC0.B.DE = 1;		//Enable divider for SYSCLK0
    MC_CGM.AC6_DC0.B.DIV = 3;		//Set divider for SYSCLK0 (div by 4)

    //Configure RunPeripheralConfiguration regist	ers in ME_RUN_PC0
    MC_ME.RUN_PC[0].B.DRUN = 1;		//Enable DRUN mode for all peripherals using ME_RUN_PC[0]

    MC_ME.DRUN_MC.B.XOSCON = 1;		//Enable external oscilator

    //AUX Clock Selector 3 setup
    MC_CGM.AC3_SC.B.SELCTL =0x01;	//connect (8..40MHz) XTALL to the PLL0 input
    MC_ME.DRUN_MC.B.PLL0ON = 1;		//Enable PLL0 for DRUN mode

    //AUX Clock Selector 0 setup
    MC_CGM.AC0_SC.B.SELCTL=0x2;		//source PLL0 PHI 200MHz
    MC_CGM.AC0_DC0.R = 0x80010000;	//MOTC_CLK enable with divider 1
    MC_CGM.AC0_DC2.R = 0x80010000;	//ADC_CLK enable with divider 1

    // Set PLL0 to 200MHz
    PLLDIG.PLL0CR.B.CLKCFG = 1;		//Bypass mode PLL0 on
    // RFDPHI1 = 10, RFDPHI = 2, PREDIV = 2, MFD = 14
    PLLDIG.PLL0DV.R = 0x50000000 |0x00020000 |0x00002000 |0x0014 ; //predefined PLL0 divider register

    //Mode transition to apply the PLL0 setup and set Normal mode with PLL running
    MC_ME.MCTL.R = 0x30005AF0;		//DRUN Mode & Key
    MC_ME.MCTL.R = 0x3000A50F;		//DRUN Mode & Key

    while(!MC_ME.GS.B.S_PLL0);          //ME_GS Wait for PLL stabilization.
    while(MC_ME.GS.B.S_MTRANS);         //Waiting for end of transaction
    while(MC_ME.GS.B.S_CURRENT_MODE != DRUN_MODE);  // ME_GS Check DRUN mode has successfully been entered
}//Sys_Init


/*
 * GPIO function configuration
 */

void GPIO_Aux_Function_Config(void)
{
	  /* Configure the CAN0_TX pin to transmit. */
	  SIUL2.MSCR[PB0].B.SSS = 1; //PTB0 is for CAN0_TX. Select signal source select to CAN0_TX
	  SIUL2.MSCR[PB0].B.OBE = 1; //Set pin to output. Enable output buffer
	  SIUL2.MSCR[PB0].B.SRC = 3; //Maximum slew rate

	  /* Configure the CAN0_RX pin. */

	  SIUL2.MSCR[PB1].B.IBE = 1; //PB1 is CAN0_RX pin. Enable input buffer
	  SIUL2.IMCR[32].B.SSS = 0b0010; //Set PB1 as CAN0_RX.

	  /*
	   * the DEVKIT-MPC5744P onboard CAN tranceiver--MC33901 is always enabled, so no need to configure the control pin
	   * for user's own board, please note to configure such control pins here, for example standby/sleep control pin for low-power mode
	   */

	  SIUL2.MSCR[LED_R].R = 0x02030000; /*output buffer enable with internal pull-up enable*/
	  SIUL2.MSCR[LED_G].R = 0x02030000; /*output buffer enable with internal pull-up enable*/
	  SIUL2.MSCR[LED_B].R = 0x02030000; /*output buffer enable with internal pull-up enable*/

	  SIUL2.GPDO[LED_R].B.PDO = 1;  /* output High to turn OFF the LED*/
	  SIUL2.GPDO[LED_G].B.PDO = 1;  /* output High to turn OFF the LED*/
	  SIUL2.GPDO[LED_B].B.PDO = 1;  /* output High to turn OFF the LED*/

}

