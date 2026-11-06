#include "crt_startup_arguments.h"
uint32_t FUN_00569831(int32_t codepage);

void restore_crt_initialize_multibyte(void)
{
 if(RESTORE_CRT_MB_INITIALIZED==0){
  (void)FUN_00569831(-3);
  RESTORE_CRT_MB_INITIALIZED=1;
 }
}
void FUN_00569bf5(void){restore_crt_initialize_multibyte();}
