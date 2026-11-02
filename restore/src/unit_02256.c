#include "restore_types.h"
#define DAT_005d2a3c (*(undefined4 *)0x005d2a3cu)
#define DAT_0077de6c (*(undefined4 *)0x0077de6cu)
#define DAT_0077de6d (*(undefined4 *)0x0077de6du)
#define DAT_0078d728 (*(undefined *)0x0078d728u)
#define DAT_00ea40ba (*(undefined4 *)0x00ea40bau)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443d90();
extern bool FUN_00447790();

int FUN_0050e3c0(int *param_1,char param_2)

{
  short sVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  byte *pbVar6;
  
  iVar4 = 0;
  if (param_2 == '\0') {
    iVar3 = FUN_00443d90(0x12);
    if (iVar3 != 0) {
      *param_1 = 1;
    }
    iVar3 = 1;
    pcVar5 = &DAT_0077de6c;
    do {
      if (pcVar5[-2] != '\0') {
        cVar2 = *pcVar5;
        if ((((cVar2 == '\x01') || (cVar2 == '\x03')) || (cVar2 == '\x05')) &&
           (pcVar5[0x3b] != '\0')) {
          cVar2 = FUN_00447790(iVar3,0x13);
          if (cVar2 != '\0') {
            param_1[*(int *)(&DAT_005d2a3c + (uint)(byte)pcVar5[1] * 8) + -0x26] =
                 param_1[*(int *)(&DAT_005d2a3c + (uint)(byte)pcVar5[1] * 8) + -0x26] + 1;
          }
        }
      }
      pcVar5 = pcVar5 + 0x6a;
      iVar3 = iVar3 + 1;
    } while ((int)pcVar5 < 0x77f744);
    iVar3 = 0x10;
    do {
      if (*param_1 != 0) {
        iVar4 = iVar4 + 1;
      }
      param_1 = param_1 + 1;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    return iVar4;
  }
  if (DAT_0078d728 < 1) {
    *param_1 = 1;
  }
  iVar3 = 1;
  pcVar5 = &DAT_0077de6c;
  do {
    if (pcVar5[-2] != '\0') {
      cVar2 = *pcVar5;
      if ((((cVar2 == '\x01') || (cVar2 == '\x03')) || (cVar2 == '\x05')) && (pcVar5[0x3b] != '\0'))
      {
        cVar2 = FUN_00447790(iVar3,0x13);
        if (cVar2 != '\0') {
          param_1[*(int *)(&DAT_005d2a3c + (uint)(byte)pcVar5[1] * 8) + -0x26] =
               param_1[*(int *)(&DAT_005d2a3c + (uint)(byte)pcVar5[1] * 8) + -0x26] + 1;
        }
      }
    }
    pcVar5 = pcVar5 + 0x6a;
    iVar3 = iVar3 + 1;
  } while ((int)pcVar5 < 0x77f744);
  iVar3 = 1;
  pbVar6 = &DAT_0077de6d;
  do {
    if (((pbVar6[-3] != 0) &&
        ((sVar1 = *(short *)((int)&DAT_00ea40ba + DAT_00eaac28 * 0x49d8 + 2), sVar1 == 1 ||
         (sVar1 == 3)))) && (pbVar6[0x3a] != 0)) {
      cVar2 = FUN_00447790(iVar3,0x13);
      if ((cVar2 != '\0') && (param_1[*(int *)(&DAT_005d2a3c + (uint)*pbVar6 * 8) + -0x26] != 0)) {
        param_1[*(int *)(&DAT_005d2a3c + (uint)*pbVar6 * 8) + -0x26] = 0;
      }
    }
    pbVar6 = pbVar6 + 0x6a;
    iVar3 = iVar3 + 1;
  } while ((int)pbVar6 < 0x77f745);
  iVar3 = 0x10;
  do {
    if (*param_1 != 0) {
      iVar4 = iVar4 + 1;
    }
    param_1 = param_1 + 1;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  return iVar4;
}
