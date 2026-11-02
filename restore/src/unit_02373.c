#include "restore_types.h"
#include "town_trade.h"
#define DAT_007770f0 (*(undefined4 *)0x007770f0u)
extern int * FUN_0044baf0();


void FUN_00521aa0(int *param_1,int *param_2,int *param_3,int *param_4)

{
  char cVar1;
  short *psVar2;
  int iVar3;
  
  *param_1 = -1;
  *param_2 = -1;
  *param_3 = 0;
  *param_4 = 0;
  for (psVar2 = (short *)FUN_0044baf0(0); psVar2 != (short *)0x0;
      psVar2 = (short *)FUN_0044baf0(psVar2)) {
    cVar1 = FUN_0044d4c0((const struct RestoreTownRecord *)(uintptr_t)psVar2);
    if ((((cVar1 != '\0') && (psVar2[2] == 0x25)) && (iVar3 = (int)psVar2[0xe], -1 < iVar3)) &&
       (iVar3 < 0x32)) {
      if ((&DAT_007770f0)[iVar3 * 0x144] == '\x02') {
        if (*param_2 < 0) {
          *param_2 = (int)*psVar2;
        }
        *param_4 = *param_4 + 1;
      }
      else {
        if (*param_1 < 0) {
          *param_1 = (int)*psVar2;
        }
        *param_3 = *param_3 + 1;
      }
    }
  }
  return;
}
