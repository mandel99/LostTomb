#include "restore_types.h"
#define DAT_00604e64 (*(undefined *)0x00604e64u)
#define DAT_00604e68 (*(undefined4 *)0x00604e68u)
#define PTR_DAT_00604c58 (*(unsigned char * *)0x00604c58u)
extern uint FUN_00565f66();
extern uint FUN_00566682();

void FUN_00564165(char *param_1)

{
  char cVar1;
  char cVar2;
  int iVar3;
  uint uVar4;
  
  iVar3 = FUN_00566682((int)*param_1);
  if (iVar3 != 0x65) {
    do {
      param_1 = param_1 + 1;
      if (DAT_00604e64 < 2) {
        uVar4 = (byte)((undefined4 *)&PTR_DAT_00604c58)[*param_1 * 2] & 4;
      }
      else {
        uVar4 = FUN_00565f66((int)*param_1,4);
      }
    } while (uVar4 != 0);
  }
  cVar2 = *param_1;
  *param_1 = DAT_00604e68;
  do {
    param_1 = param_1 + 1;
    cVar1 = *param_1;
    *param_1 = cVar2;
    cVar2 = cVar1;
  } while (*param_1 != '\0');
  return;
}
