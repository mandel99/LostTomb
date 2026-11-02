#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00a5d4cc (*(undefined4 *)0x00a5d4ccu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern uint FUN_004894a0();

void FUN_00489180(int *param_1,int param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  
  cVar1 = FUN_004894a0(param_1);
  if (cVar1 != '\0') {
    iVar2 = (int)param_1 * 0x108;
    param_1 = &DAT_005cc8c0;
    iVar2 = DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar2 + 2) +
            *(short *)(&DAT_007999e0 + iVar2) * 0xe4;
    DAT_00bc9768 = iVar2;
    do {
      iVar5 = 3;
      piVar6 = param_1;
      do {
        iVar3 = *piVar6 + iVar2;
        iVar4 = (int)((uint)(byte)(&DAT_00a5d4cc)[iVar3] * (100 - param_2)) / 100;
        if (iVar4 < 1) {
          iVar4 = 1;
        }
        piVar6 = piVar6 + 2;
        iVar5 = iVar5 + -1;
        (&DAT_00a5d4cc)[iVar3] = (char)iVar4;
      } while (iVar5 != 0);
      param_1 = param_1 + 0xc;
    } while ((int)param_1 < 0x5cc950);
  }
  return;
}
