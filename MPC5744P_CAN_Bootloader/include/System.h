
#ifndef SYSTEM_H_
#define SYSTEM_H_




//#define  SPASS       0
//#define  SERR        1
//#define  SFileStart  2
//#define  SFileEnd    3
//
//
//
//
//#define NO_ERR 0x00
//#define ERR_BUFFER_FULL 0x80
//
//
//typedef struct {
//	  uint32_t  ID;
//	  uint8_t FrameType;
//	  uint8_t FrameFormat;
//	  uint8_t  DtLen;
//	  uint8_t  data[8];
//} CanMsg_t;


extern void Prepare_Before_Jump_m(void);
extern void app_entry(void);

extern void Cpu_DelayMS(uint32_t ms);


/*
 * all this symbols difined in project linker command file
 */
//extern uint32_t __SRAM_BASE_ADDR,__SRAM_SIZE,__LOCAL_DMEM_BASE_ADDR,__LOCAL_DMEM_SIZE;


#endif /*end of SYSTEM_H_*/
