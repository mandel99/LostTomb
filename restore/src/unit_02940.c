#include "crt_startup_arguments.h"
uint32_t FUN_005697ef(uint32_t character);

uint8_t *restore_crt_command_line_tail(void)
{
 uint8_t *scan;
 if(RESTORE_CRT_MB_INITIALIZED==0)FUN_00569bf5();
 scan=RESTORE_CRT_COMMAND_LINE;
 if(*scan=='"'){
  for(;;){
   uint8_t character=*++scan;
   if(character=='"' || character==0)break;
   if(FUN_005697ef(character)!=0)++scan;
  }
  if(*scan=='"')++scan;
 }else{
  while(*scan>0x20u)++scan;
 }
 while(*scan!=0 && *scan<=0x20u)++scan;
 return scan;
}
uint8_t *FUN_0056793c(void){return restore_crt_command_line_tail();}
