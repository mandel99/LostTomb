#include "restore_types.h"
#define DAT_00bcba9a (*(undefined4 *)0x00bcba9au)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd720 (*(undefined4 *)0x00bcd720u)
#define DAT_00bcd72a (*(undefined4 *)0x00bcd72au)
#define DAT_00bcd8a4 (*(undefined4 *)0x00bcd8a4u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_0040b5e0();
extern int FUN_004b8360();
extern int FUN_004b8460();
extern int FUN_004cde40();

void FUN_004b8890(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  short *psVar3;
  int iVar4;
  char *pcVar5;
  uint uVar6;
  int local_c;
  int local_8;
  undefined1 local_4 [4];
  
  FUN_004b8360(param_2,&local_8,local_4);
  local_c = 1;
  pcVar5 = &DAT_00bcba9a;
  do {
    if (((pcVar5[-0x42] == '\x01') && (*pcVar5 != '\0')) && (*(short *)(pcVar5 + 0x40) == param_2))
    {
      iVar4 = 0;
      uVar6 = (uint)(byte)pcVar5[-0x1a];
      if (uVar6 != 0) {
        psVar3 = (short *)(pcVar5 + -0x3a);
        do {
          if (((0 < *psVar3) && (iVar1 = *psVar3 * 0x184, (&DAT_00bcd606)[iVar1] == '\x01')) &&
             ((&DAT_00bcd630)[iVar1] != '\x04')) {
            iVar4 = iVar4 + 1;
          }
          psVar3 = psVar3 + 1;
          uVar6 = uVar6 - 1;
        } while (uVar6 != 0);
      }
      iVar1 = FUN_004cde40(iVar4,param_1);
      if (iVar4 <= iVar1) {
        iVar4 = FUN_0040b5e0(local_c);
        if ((iVar4 != 0) && (DAT_00d056f8 = FUN_004b8460(local_c), DAT_00d056f8 != 0)) {
          iVar4 = DAT_00d056f8 * 0x184;
          *(undefined4 *)(&DAT_00bcd720 + iVar4) = 0;
          (&DAT_00bcd606)[iVar4] = 4;
          *(undefined2 *)(&DAT_00bcd72a + iVar4) = 0xffff;
        }
        pcVar5[-0x3f] = '\x01';
        pcVar5[0x40] = -1;
        pcVar5[0x41] = -1;
      }
      uVar6 = (uint)(byte)pcVar5[-0x1a];
      if (uVar6 != 0) {
        psVar3 = (short *)(pcVar5 + -0x3a);
        do {
          if ((((0 < *psVar3) && (iVar4 = *psVar3 * 0x184, (&DAT_00bcd606)[iVar4] == '\x01')) &&
              ((&DAT_00bcd630)[iVar4] != '\x04')) && (iVar1 != 0)) {
            iVar1 = iVar1 + -1;
            (&DAT_00bcd606)[iVar4] = 2;
          }
          psVar3 = psVar3 + 1;
          uVar6 = uVar6 - 1;
        } while (uVar6 != 0);
      }
    }
    pcVar5 = pcVar5 + 0x90;
    local_c = local_c + 1;
  } while ((int)pcVar5 < 0xbcd62a);
  iVar4 = FUN_004cde40(local_8 * 10,param_1);
  uVar6 = DAT_00eaac28;
  DAT_00d056f8 = 1;
  piVar2 = &DAT_00bcd8a4;
  do {
    if (((*(char *)((int)piVar2 + -0x11a) != '\0') && (*(char *)((int)piVar2 + -0x11e) == 'N')) &&
       ((*(byte *)((int)piVar2 + -0x119) == uVar6 && (*(short *)((int)piVar2 + 10) == param_2)))) {
      iVar1 = *piVar2;
      if ((iVar4 <= iVar1) && (param_1 != 100)) {
        *(int *)(&DAT_00bcd720 + DAT_00d056f8 * 0x184) =
             *(int *)(&DAT_00bcd720 + DAT_00d056f8 * 0x184) - iVar4;
        return;
      }
      *piVar2 = 0;
      *(undefined1 *)(piVar2 + -0x3c) = 4;
      iVar4 = iVar4 - iVar1;
      *(undefined2 *)((int)piVar2 + 10) = 0xffff;
    }
    piVar2 = piVar2 + 0x61;
    DAT_00d056f8 = DAT_00d056f8 + 1;
    if (0xc8ae5f < (int)piVar2) {
      return;
    }
  } while( true );
}
