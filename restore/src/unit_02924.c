#include "crt_environment.h"

void restore_crt_initialize_timezone_once(void)
{
 if(RESTORE_CRT_TIMEZONE_INITIALIZED==0){
  FUN_00566bc3();
  /* Native INC reloads the callback's actual DWORD, including carry wrap. */
  RESTORE_CRT_TIMEZONE_INITIALIZED=RESTORE_CRT_TIMEZONE_INITIALIZED+1u;
 }
}
void FUN_00566bae(void){restore_crt_initialize_timezone_once();}
