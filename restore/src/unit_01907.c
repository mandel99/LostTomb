#include "restore_types.h"
#define DAT_00799af0 (*(volatile short *)0x00799af0u)
extern uint FUN_004e05f0(int building,int actor,char mode);

int FUN_004e0c00(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  char cVar1;
  volatile short *psVar2;
  int iVar3;
  
  (void)param_1;
  (void)param_2;
  iVar3 = 1;
  psVar2 = &DAT_00799af0;
  do {
    if ((*(volatile unsigned char *)(psVar2 - 8) == '\x01') && (*psVar2 == 0xde)) {
      cVar1 = (char)FUN_004e05f0(iVar3,(int)param_3,1);
      if (cVar1 != '\0') {
        return iVar3;
      }
    }
    psVar2 = psVar2 + 0x84;
    iVar3 = iVar3 + 1;
  } while ((int)psVar2 < 0x89b6e8);
  return 0;
}
