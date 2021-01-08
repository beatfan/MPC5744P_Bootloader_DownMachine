#include "Bootloader.h"
#include "Drivers/Flash/flash.h"


/***************************************
 * Flash Block Range

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

//erase user app, the address must plus 0x8000000, due to upmachine has minus it
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

uint32_t m_LastAddr=0x000004+0x800000; //store last program address, every 32bits take 4 address
uint32_t dataForWrite[2]; //minimum flash write segment is 64bits, equal 2*32bits

//program addr must add 0x800000, due to upmachine has minus 0x800000 to reduce 1 byte occupy
void UserFlash_DataParseAddrData(uint8_t *data, uint8_t length)
{
    uint32_t addr;
    uint32_t dataTmp;
    
    addr = data[1]*256*256 + data[2]*256 + data[3] + 0x800000;
    
    //large-endian
    dataTmp = data[4]*256*256*256 +data[5]*256*256+ data[6]*256 + data[7];
   

    if(addr%8==0) //Check if address is align 64bits
        {
            if(m_LastAddr%8==0) //last addr also align 64bits, but one frame only 32bits, so we need to insert 32bits before this
            {
                dataForWrite[1] = 0xFFFFFFFF; //same to erased value
                UserFlash_WriteData(m_LastAddr,dataForWrite,2); //write last one segment to flash
            }
            //copy current data to array 0, new segment
            dataForWrite[0] = dataTmp;

        }
        else //address is not align 64bits
        {
            if(m_LastAddr%8==0) //last addr align 64bits
            {
                if((addr-m_LastAddr)==4) //check current data if is near last one, if yes fill the second 32bits
                {
                    dataForWrite[1] = dataTmp; //fill second 32bits
                    UserFlash_WriteData(m_LastAddr,dataForWrite,2); //write one segment to flash
                }
                else // current data is not near last one, we will fill two segments
                {
					//last addr is align 64bits, means the second 32 bits missed, we fill it
                    dataForWrite[1] = 0xFFFFFFFF; //fill with erased data
                    UserFlash_WriteData(m_LastAddr,dataForWrite,2); //write last segment to flash

                    //current addr is not align 64bits, we must insert 32bits 
                    dataForWrite[0] = 0xFFFFFFFF; //fill with erased data
                    dataForWrite[1] = dataTmp; //current data
                    UserFlash_WriteData(addr-4,dataForWrite,2); //write current segment to flash, addr must minus 4 to align 64bits
                }
            }
            else//last addr is not align 64bits, means already writton
            {
                    dataForWrite[0] = 0xFFFFFFFF; //fill with erased data
                    dataForWrite[1] = dataTmp; //current data
                    UserFlash_WriteData(addr-4,dataForWrite,2); //write current segment to flash, addr must minus 4 to align 64bits
            }
        }
    

    m_LastAddr = addr;
    

}


/* check if the lattest data is written */
void UserFlash_LastIsFull64Bits_g()
{
	if(m_LastAddr%8==0) //last addr align 64bits, means only 32bits, we fill the second 32bits
	{
		dataForWrite[1] = 0xFFFFFFFF; //fill with erased data
		UserFlash_WriteData(m_LastAddr,dataForWrite,2); //write current segment to flash
	}

	Flash_Lock_g(USERAPPFLASH_STARTADDR,USERAPPFLASH_ENDADDR);
}


