#ifndef _AFTER_PROTOCOL_USERAPPFLASH_H_
#define _AFTER_PROTOCOL_USERAPPFLASH_H_

#include <stdint.h>


//闁瑰尅绠撳▍搴㈢▔椤撶喐鐒介柛姘灴閸ｈ櫣鎮伴妸銈勭鞍闁告瑥锕﹂弫銈夊箣閻ゎ垵鎷ù鐘烘硾濞硷拷
//Bootloader闁圭鎷烽柛锔哄妼濠�鎾锤閿熻姤绋夐悮瀵哥獥 ORIGIN = 0x800, LENGTH = 0x6400
//  reset : ORIGIN = 0x0, LENGTH = 0x4
//  ivt   : ORIGIN = 0x4, LENGTH = 0x1FC
//  User  闁挎稒鐡孯IGIN = 0x8000, LENGTH = 0x22FEC
// 婵絽绻戦濂稿箼閿曞倹鐝�0x400濞戞搩浜濈�垫碍绂掗妶鍛憻闁挎稑鑻悿鍕⒔閸涱厼绐楅柣鈧妼濠�鎾锤閿熻姤绋夐敓锟�0x800闁挎稑濂旂欢銉︿繆閸屾稒鎲归梻鍕舵嫹0x0,閻庡湱鍋ゅ顖涘濮橆厽鎲归梻鍕╁�曢崺锟�0x7ff
void UserFlash_EraseIvtAndUserAppBlock_g(uint8_t *data, uint8_t length);

//閻熸瑱绲鹃悗绱縠x闁哄倸娲ｅ▎锟�
//缂佹鎷�4闁靛棴鎷�5閻炴稏鍔庨妵姘辨偘鐏炶棄濞囬柨娑樼焸閻濐喚锟芥稒顨夋俊顓㈠捶閵娿儱顤�
//缂佹鎷�6閻炴稏鍔庨妵姘跺极閻楀牆绁﹂梻锟介崹顔碱唺
void UserFlash_DataParseRowSplit(uint8_t *data, uint8_t length);

//閻熸瑱绲鹃悗绱縠x闁哄倸娲ｅ▎锟�
//0闁靛棴鎷�1閻炴稏鍔庨妵姘嚕閿熻姤寰勯敓锟�
//2閻炴稏鍔庨妵姘辩尵鐠囪尙锟斤拷
//3闁靛棴鎷�4闁靛棴鎷�5閻炴稏鍔庨妵姘跺捶閺夋寧绲婚柨娑樼焸閻濐喚锟芥稒顨夋俊顓㈠捶閵娿儱顤呴柨娑樻湰濞撹埖顨囧Ο铏规憻闁煎搫鍊瑰Λ锟�
//6闁靛棴鎷�7闁靛棴鎷�8闁靛棴鎷�9閻炴稏鍔庨妵姘跺极閻楀牆绁﹂柨娑樺缂嶅棛锟芥稒顨夋俊顓㈠捶閵娿儱顤�
void UserFlash_DataParseAddrData(uint8_t *data, uint8_t length);

void UserFlash_LastIsFull64Bits_g();

#endif
