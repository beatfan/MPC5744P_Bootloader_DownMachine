#include "Bootloader.h"



        
/// <summary>
/// Bootloader鐢拷
/// </summary>
uint8_t m_Bootloader_EntryBootloaderCmd[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/// <summary>
/// 婢跺秳缍呯敮锟�
/// </summary>
uint8_t m_Bootloader_ResetCmd_g[8] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/// <summary>
/// 濡拷閺岊檲ootloader鐢拷
/// </summary>
uint8_t m_Bootloader_CheckBootloaderCmd[8] = {  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


/// <summary>
/// 閹恒儲鏁归崚鎷岀箻閸忣檲ootloader鐢嶇礉閸欏秹顩�
/// </summary>
uint8_t g_Bootloader_EntryBootloaderReponse[8] = { 0x00, g_GOTOAPP_TIMEOUT, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/// <summary>
/// 閹恒儲鏁归崚鎷岀箻閸忣檲ootloader鐢嶇礉閸欏秹顩�
/// </summary>
uint8_t g_Bootloader_EntryUserAppReponse[8] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/// <summary>
/// 濡拷閺岊檲ootloader鐢嶇礉閸欏秹顩�
/// </summary>
uint8_t g_Bootloader_CheckBootloaderReponse[8] = { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/// <summary>
/// 閹匡箓娅嶧lash鐢嶇礉閸欏秹顩�
/// </summary>
uint8_t g_Bootloader_EraseFlashReponse[8] = {0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/// <summary>
/// 閺佺増宓佺敮褎鈷婇悶鍡楃暚濮ｆ洖娲栨＃锟�
/// 02 00 00 00 00 00 00 00
/// </summary>
uint8_t g_Bootloader_DataReponse[8] ={ 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        
/// <summary>
/// 缂傛牜鈻肩紒鎾存将鐢嶇礉閸欏秹顩�
/// </summary>
uint8_t g_Bootloader_DataProgramEndReponse[8] = { 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

//濮ｆ柨顕弫鎵矋閿涘矁瀚㈢�瑰苯鍙忔稉锟介懛杈剧礉鏉╂柨娲�0閿涘苯鎯侀崚娆掔箲閸ワ拷1
uint8_t ArrayCheck(uint8_t *data1,uint8_t *data2,uint8_t length)
{
    uint8_t i,result=0;
    for(i=0;i<length;i++)
    {
        if(data1[i]!=data2[i])
        {
            result=1;
            return result;
        }
    }
    return result;
}

//閺佺増宓佺憴锝嗙�介敍宀冪箲閸ワ拷0xff閺堫亜绱撶敮锟�
AfterBootloader_CmdType Bootloader_DataParse_g(uint8_t *data, uint8_t dataLength)
{
    uint8_t result=0;
    if(dataLength!=8)
        return 0xff;
    
    //閸涙垝鎶ょ猾璇茬��
    switch(data[0])
    {
        case EntryBootloader:
            result = EntryBootloader;
            break;
        case Reset:
            result = Reset;
            break;
        case Data:
            result = Data;
            break;
        case DataEnd:
            result = DataEnd;
            break;
        case CheckBootloader:
            result = CheckBootloader;
            break;
        case Erase:
            result = Erase;
            break;
        default:return 0xff;
    }
    
    return result;
}

