#include "restore_types.h"
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd636 (*(undefined4 *)0x00bcd636u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd7b4 (*(undefined4 *)0x00bcd7b4u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
extern void FUN_004b3250();
extern undefined4 FUN_004ea9a0();
extern int FUN_005190a0();

int FUN_004a2320(undefined4 param_1,undefined4 param_2,int param_3)

{
  short sVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  int iVar6;
  int local_f0 [30];
  int aiStack_78 [30];
  
  iVar4 = 1;
  pcVar5 = &DAT_00bcd7b4;
  iVar3 = 0;
  do {
    iVar6 = iVar3;
    if (pcVar5[-0x2a] == '\x01') {
      cVar2 = FUN_004ea9a0(pcVar5[-0x2e]);
      if (cVar2 != '\0') {
        if (((*pcVar5 != '\x04') && (*pcVar5 != '\f')) &&
           ((*(int *)(pcVar5 + 0x100) == 0 ||
            (*(int *)(pcVar5 + 0x100) == (int)(short)(&DAT_00bcd652)[DAT_00d056f8 * 0xc2])))) {
          sVar1 = *(short *)(pcVar5 + -0x22);
          local_f0[iVar3 + 0x1e] = iVar4;
          iVar6 = iVar3 + 1;
          local_f0[iVar3] = DAT_00bca2d8 + *(short *)(pcVar5 + -0x24) + sVar1 * 0xe4;
          if (iVar6 == 0x1e) break;
        }
      }
    }
    pcVar5 = pcVar5 + 0x184;
    iVar4 = iVar4 + 1;
    iVar3 = iVar6;
  } while ((int)pcVar5 < 0xc8ad70);
  if (iVar6 != 0) {
    iVar3 = FUN_005190a0(param_1,param_2,local_f0,iVar6);
    if ((iVar3 != 0) && (*(short *)(&DAT_00bcd636 + DAT_00d056f8 * 0x184) <= param_3)) {
      FUN_004b3250(DAT_00d056f8);
      return local_f0[iVar3 + 0x1d];
    }
  }
  return 0;
}
