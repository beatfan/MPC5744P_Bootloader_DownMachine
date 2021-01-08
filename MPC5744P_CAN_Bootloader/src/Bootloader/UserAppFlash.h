#ifndef _AFTER_PROTOCOL_USERAPPFLASH_H_
#define _AFTER_PROTOCOL_USERAPPFLASH_H_

#include <stdint.h>


//erase user app, the address must plus 0x8000000, due to upmachine has minus it
void UserFlash_EraseIvtAndUserAppBlock_g(uint8_t *data, uint8_t length);


void UserFlash_DataParseRowSplit(uint8_t *data, uint8_t length);

/* parse data, the flash segment is 64bits, but one frame is 32bits data */
void UserFlash_DataParseAddrData(uint8_t *data, uint8_t length);

void UserFlash_LastIsFull64Bits_g();

#endif
