#include "Bootloader.h"
#include "Drivers/Flash/flash.h"


/***************************************
 * Flash Block Range

锛�
// LOW--not program, partition 0 and 1
0x00800000-0x00803FFF		--		16KB,EEPROM-low block0, RWW_P:0
0x00804000-0x00807FFF		--		16KB,EEPROM-low block1, RWW_P:0
0x00F98000-0x00F9BFFF		--		16KB,low flash memory block2, RWW_P:1  //bootloader
0x00F9C000-0x00F9FFFF		--		16KB,low flash memory block3, RWW_P:1  //bootloader

// MID , partition 2 and 3
0x00808000-0x0080FFFF		--		32KB,EEPROM-mid block0, RWW_P:2
0x00810000-0x00817FFF		--		32KB,EEPROM-mid block1, RWW_P:3

//HIGH, , partition 4 and 5
0x00FA0000-0x00FAFFFF		--		64KB,high flash memory block0, RWW_P:4  //User App
0x00FB0000-0x00FBFFFF		--		64KB,high flash memory block1, RWW_P:4
0x00FC0000-0x00FCFFFF		--		64KB,high flash memory block2, RWW_P:4
0x00FD0000-0x00FDFFFF		--		64KB,high flash memory block3, RWW_P:5
0x00FE0000-0x00FEFFFF		--		64KB,high flash memory block4, RWW_P:5
0x00FF0000-0x00FFFFFF		--		64KB,high flash memory block5, RWW_P:5

//256K--all used
0x01000000-0x0103FFFF		--		256KB,256k flash memory block0, RWW_P:6
0x01040000-0x0107FFFF		--		256KB,256k flash memory block1, RWW_P:6
0x01080000-0x010BFFFF		--		256KB,256k flash memory block2, RWW_P:6
0x010C0000-0x010FFFFF		--		256KB,256k flash memory block3, RWW_P:6
0x01100000-0x0113FFFF		--		256KB,256k flash memory block4, RWW_P:7
0x01140000-0x0117FFFF		--		256KB,256k flash memory block5, RWW_P:7
0x01180000-0x011BFFFF		--		256KB,256k flash memory block6, RWW_P:7
0x011C0000-0x011FFFFF		--		256KB,256k flash memory block7, RWW_P:7

 **************************************/

uint32_t USERAPPFLASH_STARTADDR=0x00FA0000;
uint32_t USERAPPFLASH_ENDADDR=0x011FFFFF;

//erase
//0xFD0000-0x010032E4, 0x05,0x7d,0x00,0x00,0x80,0x32,0xe4,0x00
void UserFlash_EraseIvtAndUserAppBlock_g(uint8_t *data, uint8_t length)
{
	uint32_t count=0;
	USERAPPFLASH_STARTADDR = data[1]*256*256 + data[2]*256 + data[3] + 0x800000;
	USERAPPFLASH_ENDADDR = data[4]*256*256 + data[5]*256 + data[6] + 0x800000;

	Flash_Unlock_g(USERAPPFLASH_STARTADDR,USERAPPFLASH_ENDADDR);

	Flash_Erase_g(USERAPPFLASH_STARTADDR,USERAPPFLASH_ENDADDR);

	//Flash_Check_g(USERAPPFLASH_STARTADDR,USERAPPFLASH_ENDADDR);
	while(count<100000)
		count++;

//	erase_flash_except_bootloader();
}

//length%2==0,length must be even number
void UserFlash_WriteData(uint32_t addr, uint32_t *data, uint32_t length)
{
	uint32_t startAddr;
	uint32_t endAddr;
	startAddr = addr;
	endAddr = startAddr+length-1;

//	Flash_Unlock_g(startAddr,endAddr);

//	ret = Flash_Erase_g(startAddr,endAddr);

	//ret = Flash_Check_g(startAddr,endAddr);

	Flash_Program_g(startAddr,endAddr, data,length);

//	Flash_Lock_g(startAddr,endAddr);
}

uint32_t m_LastAddr=0x000004+0x800000; //program every 8 address锛�64bits
uint32_t dataForWrite[2]; //鐎涙ê鍋嶇憰浣稿晸閸忋儳娈�2娑擃亝瀵氭禒銈呯摟閿涘本鐦℃稉顏呭瘹娴犮倕鐡�2娑擄拷16娴ｅ秶绮嶉幋锟�

//program addr must add 0x800000
void UserFlash_DataParseAddrData(uint8_t *data, uint8_t length)
{
    uint32_t addr;
    uint32_t dataTmp;
    
    addr = data[1]*256*256 + data[2]*256 + data[3] + 0x800000;
    
    //large-endian
    dataTmp = data[4]*256*256*256 +data[5]*256*256+ data[6]*256 + data[7];
   

    if(addr%8==0) //鏈鍦板潃鏄粍鍐呯涓�涓暟鎹�
        {
            if(m_LastAddr%8==0) //涓婁竴娆′篃鏄涓�涓紝鍒欏厛濉厖涓婁竴娆＄浜屼釜鎸囦护瀛楋紝鍐欏叆flash锛岀劧鍚庢湰娆￠噸鏂板紑濮�
            {
                dataForWrite[1] = 0xFFFFFFFF; //涓婁竴缁勭浜屼釜鎸囦护瀛楀～鍏�
                UserFlash_WriteData(m_LastAddr,dataForWrite,2); //涓婁竴娆℃暟鎹粍鍚堝悗锛屽啓鍏�2鎸囦护瀛�
            }
            //涓婁竴娆℃槸绗�1涓垨鑰呯2涓紝鏈閮借閲嶆柊寮�濮�
            dataForWrite[0] = dataTmp; //鏈涓虹涓�涓寚浠ゅ瓧

        }
        else //鏈鍦板潃鏄浜屼釜
        {
            if(m_LastAddr%8==0) //涓婁竴娆℃槸绗竴涓紝瑕佺户缁垽鏂槸鍚﹁繛缁�
            {
                if((addr-m_LastAddr)==4) //涓庝笂涓�娆¤繛缁紝濉厖鍒颁笂涓�娆＄殑鏁版嵁涓紝骞跺啓鍏�2涓寚浠ゅ瓧
                {
                    dataForWrite[1] = dataTmp; //鏈涓虹浜屼釜鎸囦护瀛�
                    UserFlash_WriteData(m_LastAddr,dataForWrite,2); //鏈娆℃暟鎹粍鍚堝悗锛屽啓鍏�2鎸囦护瀛�
                }
                else //涓庝笂涓�娆′笉杩炵画锛屽～鍏呬笂涓�娆＄殑绗簩涓寚浠ゅ瓧锛岀劧鍚庡～鍏呮湰娆＄殑绗竴涓寚浠ゅ瓧锛屽悓鏃朵袱娆″叡鍐欏叆4涓寚浠ゅ瓧
                {
                    dataForWrite[1] = 0xFFFFFFFF; //涓婄粍绗簩涓寚浠ゅ瓧濉厖
                    UserFlash_WriteData(m_LastAddr,dataForWrite,2); //涓婁竴娆℃暟鎹粍鍚堝悗锛屽啓鍏�2鎸囦护瀛�

                    dataForWrite[0] = 0xFFFFFFFF; //濉厖鏈粍绗竴涓寚浠ゅ瓧
                    dataForWrite[1] = dataTmp; //鏈粍绗簩涓寚浠ゅ瓧
                    UserFlash_WriteData(addr-4,dataForWrite,2); //鏈娆℃暟鎹粍鍚堝悗锛屽啓鍏�2鎸囦护瀛�
                }
            }
            else//涓婁竴娆′篃鏄2涓紝灏嗘湰娆＄涓�涓寚浠ゅ瓧涔熷～鍏咃紝鍐欏叆
            {
                    dataForWrite[0] = 0xFFFFFFFF; //濉厖鏈粍绗竴涓寚浠ゅ瓧
                    dataForWrite[1] = dataTmp; //鏈粍绗簩涓寚浠ゅ瓧
                    UserFlash_WriteData(addr-4,dataForWrite,2); //鏈娆℃暟鎹粍鍚堝悗锛屽啓鍏�2鎸囦护瀛楋紝娉ㄦ剰鍦板潃瑕佸噺2
            }
        }
    

    m_LastAddr = addr;
    

}


void UserFlash_LastIsFull64Bits_g()
{
	if(m_LastAddr%8==0) //涓婁竴娆℃槸绗竴涓紝瑕佺户缁垽鏂槸鍚﹁繛缁�
	{
		dataForWrite[1] = 0xFFFFFFFF; //涓婄粍绗簩涓寚浠ゅ瓧濉厖
		UserFlash_WriteData(m_LastAddr,dataForWrite,2); //涓婁竴娆℃暟鎹粍鍚堝悗锛屽啓鍏�2鎸囦护瀛�
	}

	Flash_Lock_g(USERAPPFLASH_STARTADDR,USERAPPFLASH_ENDADDR);
}


