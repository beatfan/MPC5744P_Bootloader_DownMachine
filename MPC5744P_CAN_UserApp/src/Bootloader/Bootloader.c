#include "Bootloader.h"



        
/// <summary>
/// The Command for entering bootloader
/// </summary>
uint8_t m_Bootloader_EntryBootloaderCmd[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/// <summary>
/// The Command for reseting mcu
/// </summary>
uint8_t m_Bootloader_ResetCmd_g[8] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/// <summary>
/// The Command for checking if it is in bootloader
/// </summary>
uint8_t m_Bootloader_CheckBootloaderCmd[8] = {  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


/// <summary>
/// response after entering bootloader after reset
/// </summary>
uint8_t g_Bootloader_EntryBootloaderReponse[8] = { 0x00, g_GOTOAPP_TIMEOUT, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/// <summary>
/// response after entering user after reset
/// </summary>
uint8_t g_Bootloader_EntryUserAppReponse[8] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


/// <summary>
/// response for check in bootloader command
/// </summary>
uint8_t g_Bootloader_CheckBootloaderReponse[8] = { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/// <summary>
/// after receiving erasing command and then response data
/// </summary>
uint8_t g_Bootloader_EraseFlashReponse[8] = {0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/// <summary>
/// after receiving data responsing
/// 02 00 00 00 00 00 00 00
/// </summary>
uint8_t g_Bootloader_DataReponse[8] ={ 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        
/// <summary>
/// after receiving program end command response
/// </summary>
uint8_t g_Bootloader_DataProgramEndReponse[8] = { 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
   

/* Check if the data1 is same to data2 , the length is the check depth, return 0 is same, return 1 is not same */
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

/* parse receive one data frame */
AfterBootloader_CmdType Bootloader_DataParse_g(uint8_t *data, uint8_t dataLength)
{
    uint8_t result=0;
    if(dataLength!=8)
        return 0xff;
    
    //check command type if is Data
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