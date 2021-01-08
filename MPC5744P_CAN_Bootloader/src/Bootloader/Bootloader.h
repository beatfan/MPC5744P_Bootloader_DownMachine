#ifndef _AFTER_PROTOCOL_BOOTLOADER_H_
#define _AFTER_PROTOCOL_BOOTLOADER_H_

#include <stdint.h>

//bootloader wait time (seconds)
#define g_GOTOAPP_TIMEOUT 4

/// <summary>
/// All command type
/// in CAN frame first byte
/// </summary>
typedef enum 
{
    EntryBootloader=0,Reset=1,Data=2,DataEnd=3,CheckBootloader=4,Erase=5,ERR=0xff
}AfterBootloader_CmdType;


/// <summary>
/// The Command for entering bootloader
/// </summary>
extern uint8_t g_Bootloader_EntryBootloaderReponse[8];

/// <summary>
/// response for check in bootloader command
/// </summary>
extern uint8_t g_Bootloader_CheckBootloaderReponse[8];

/// <summary>
/// after receiving erasing command and then response data
/// </summary>
extern uint8_t g_Bootloader_EraseFlashReponse[8];

/// <summary>
/// after receiving data responsing
/// 02 00 00 00 00 00 00 00
/// </summary>
extern uint8_t g_Bootloader_DataReponse[8];

/// <summary>
/// after receiving program end command response
/// </summary>
extern uint8_t g_Bootloader_DataProgramEndReponse[8];


/* parse receive one data frame */
AfterBootloader_CmdType Bootloader_DataParse_g(uint8_t *data, uint8_t dataLength);


#endif
