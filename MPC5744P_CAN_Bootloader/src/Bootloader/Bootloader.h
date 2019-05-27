#ifndef _AFTER_PROTOCOL_BOOTLOADER_H_
#define _AFTER_PROTOCOL_BOOTLOADER_H_

#include <stdint.h>

//bootloader wait time (seconds)
#define g_GOTOAPP_TIMEOUT 4

/// <summary>
/// 閸涙垝鎶ょ猾璇茬��
/// </summary>
typedef enum 
{
    EntryBootloader=0,Reset=1,Data=2,DataEnd=3,CheckBootloader=4,Erase=5,ERR=0xff
}AfterBootloader_CmdType;


/// <summary>
/// 閹恒儲鏁归崚鎷岀箻閸忣檲ootloader鐢嶇礉閸欏秹顩�
/// </summary>
extern uint8_t g_Bootloader_EntryBootloaderReponse[8];

/// <summary>
/// 濡拷閺岊檲ootloader鐢嶇礉閸欏秹顩�
/// </summary>
extern uint8_t g_Bootloader_CheckBootloaderReponse[8];

/// <summary>
/// 閹匡箓娅嶧lash鐢嶇礉閸欏秹顩�
/// </summary>
extern uint8_t g_Bootloader_EraseFlashReponse[8];

/// <summary>
/// 閺佺増宓佺敮褎鈷婇悶鍡楃暚濮ｆ洖娲栨＃锟�
/// FA 55 02 00 00 00 00 00 00 00 56 FB
/// </summary>
extern uint8_t g_Bootloader_DataReponse[8];

/// <summary>
/// 缂傛牜鈻肩紒鎾存将鐢嶇礉閸欏秹顩�
/// </summary>
extern uint8_t g_Bootloader_DataProgramEndReponse[8];


//閺佺増宓佺憴锝嗙�介敍宀冪箲閸ワ拷0xff閺堫亜绱撶敮锟�
AfterBootloader_CmdType Bootloader_DataParse_g(uint8_t *data, uint8_t dataLength);


#endif
