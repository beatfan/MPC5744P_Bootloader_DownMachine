/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

#include "project.h"
#include "flexcan_init.h"
#include "MPC574xP_SystemInit.h"
#include "Bootloader/Bootloader.h"
#include "Bootloader/UserAppFlash.h"
#include "System.h"

/* get the content(reset/start function address) of the user application project from its boot_reset_vector address*/
#define Bootloader_StartAddr 0x00F98004
#define APP_StartAddr 0x01000004

#define  _STA_NOT_LINKED       0x00
#define  _STA_LINKED           0x01
#define  _STA_PRGEND           0x02
#define  _STA_RESET           0x03

/*
 * multicore interrupt(IVOR and INTC mode as well as CPU global interrupt enable) setup
 */
extern void xcptn_xmpl(void);
void CAN_Tasks();
uint8_t Time_Tasks();
uint8_t Tick_Tasks();



void bootloader_entry()
{
	Prepare_Before_Jump_m();/*de-initialize the used peripherals and clean RAM*/
	(*(void (*)(void))(*(uint32_t*)Bootloader_StartAddr))();
	while(1);
}

void app_entry(void)
{
	Prepare_Before_Jump_m();/*de-initialize the used peripherals and clean RAM*/

	uint32_t result = *(uint32_t *)APP_StartAddr;
	if(result!=0xFFFFFFFF)
		(*(void (*)(void))(*(uint32_t*)APP_StartAddr))();/*run the function via function pointer convert with a certain address*/
	else
		(*(void (*)(void))(*(uint32_t*)Bootloader_StartAddr))();

	while(1);/*stop here for anyhow*/
}


int main(void)
{

	xcptn_xmpl ();             /* Configure and Enable Interrupts */
	Sys_Init();                /*system initialization*/
	GPIO_Aux_Function_Config();/*configure GPIO function for LEDs and FlexCAN*/
	flexcan_0_init_fnc();      /*initialize the FlexCAN0 with 500Kbps  for CAN message TX/RX */
	//Periphral_INTC_Init();     /* the peripheral interrupt INTC  priority initialization */

	//reponse I am in bootloader
	Load_ReponseToUpMachine(g_Bootloader_EntryUserAppReponse,8);

	while(1)
	{
		CAN_Tasks(); //can

		Tick_Tasks();
	}


	return 0;


}


uint32_t countTick = 0;
uint8_t Tick_Tasks()
{
    countTick++;
    //每隔100大约1ms
    if(countTick==50)
    {
        countTick=0;
        return Time_Tasks();
    }
    return 1;
}

uint32_t countTimer=0;
uint8_t Time_Tasks()
{
    countTimer++;
    switch(countTimer%500)
    {
    case 0:
    		SIUL2.GPDO[LED_R].B.PDO ^= 1;   /* toggle the red LED*/
    		break;
    case 170:
    	SIUL2.GPDO[LED_G].B.PDO ^= 1;   /* toggle the green LED*/
    	break;
    case 380:
    		SIUL2.GPDO[LED_B].B.PDO ^= 1;   /* toggle the blue LED*/
    		break;
    default:
    	break;
    }

    return 1;
}
uint32_t g_ReceiveCount=0;
CAN_Msg_Strcut CAN_RxMsg;  /* for CAN RX  */
//Receive Can Data
void CAN_Tasks()
{
	if(CAN_0.IFLAG1.B.BUF6I==1)       /*is new CAN data received*/
	{
		CAN_RxMsg = CAN0_ReceiveMsg(MB6);
		//CAN_0.IFLAG1.B.BUF6I=0;
		AfterBootloader_CmdType result = Bootloader_DataParse_g(CAN_RxMsg.Frame_Data,CAN_RxMsg.Frame_Length);

		switch (result)
		{
			case EntryBootloader:
				bootloader_entry();          /*jump to app and should not back */
				break;
			case Reset:
				app_entry();          /*jump to app and should not back */
				break;
			default:
				break;
		}/*end if switch*/



	}/*end of g_CAN0_BufferReceive_Flag==1 if*/
}
