#include "crt_multibyte.h"

void restore_crt_reset_multibyte(void)
{
 uint32_t i;
 /* Native REP STOSD(64), STOSB clears exactly257 flag bytes with DF clear. */
 for(i=0;i<64;++i)((volatile uint32_t *)RESTORE_CRT_MB_FLAGS)[i]=0;
 RESTORE_CRT_MB_FLAGS[256]=0;
 RESTORE_CRT_CODEPAGE=0;
 RESTORE_CRT_MB_ENABLED=0;
 RESTORE_CRT_MB_LOCALE=0;
 for(i=0;i<3;++i)RESTORE_CRT_MB_SYMBOLS[i]=0;
}
void FUN_00569a47(void){restore_crt_reset_multibyte();}
