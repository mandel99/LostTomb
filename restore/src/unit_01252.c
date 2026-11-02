#include "restore_types.h"
#define DAT_0096e6b0 (*(undefined4 *)0x0096e6b0u)
#define DAT_0096e6b4 (*(undefined4 *)0x0096e6b4u)
#define DAT_00978d98 (*(char *)0x00978d98u)
#define DAT_00978d9a (*(char *)0x00978d9au)
#define DAT_00a43eac (*(undefined4 *)0x00a43eacu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern char FUN_00477690();
extern int FUN_005190a0();

undefined4
FUN_00486810(undefined4 param_1,undefined4 param_2,undefined4 param_3,uint param_4,int param_5,
            int param_6,int param_7,int param_8)

{
  char cVar1;
  int iVar2;
  short *psVar3;
  int iVar4;
  int iVar5;
  uint *puVar6;
  int iVar7;
  int iVar8;
  int local_10;
  
  if (param_4 == 0x40000) {
    if (DAT_00978d9a == '\0') {
      return 0;
    }
    cVar1 = 'Z';
  }
  else {
    if (DAT_00978d98 == '\0') {
      return 0;
    }
    cVar1 = 'K';
  }
  local_10 = 0;
  if (param_6 < param_8) {
    iVar5 = param_6 * 0xe4;
    iVar4 = param_6;
    do {
      if (param_5 < (int)(&DAT_00bc9f24)[iVar4]) {
        iVar7 = (&DAT_00bc9804)[iVar4];
        iVar8 = (&DAT_00bc9f24)[iVar4];
      }
      else {
        iVar7 = DAT_00bca2d8 + iVar5 + param_5;
        iVar8 = param_5;
      }
      param_6 = (&DAT_00bc9b94)[iVar4];
      if (param_7 <= param_6) {
        param_6 = param_7;
      }
      if (iVar8 < param_6) {
        psVar3 = (short *)(&DAT_00b350dc + iVar7 * 2);
        puVar6 = &DAT_00af598c + iVar7;
        do {
          if ((*puVar6 & 0x80000) == 0) {
            if ((param_4 & *puVar6) != 0) {
              if (((param_4 == 0x40000) && (iVar2 = FUN_00477690(0x40000,iVar7), iVar2 != 8)) ||
                 ((&DAT_00bcd602)[*psVar3 * 0x184] == cVar1)) goto LAB_0048695e;
              if (*(char *)((int)&DAT_00a43eac + iVar7) == -1) {
                (&DAT_0096e6b4)[local_10] = iVar7;
                local_10 = local_10 + 1;
              }
            }
            if (local_10 == 3999) {
              iVar2 = FUN_005190a0(param_1,param_2,&DAT_0096e6b4,3999);
              if (iVar2 != 0) goto LAB_004869bd;
              local_10 = 0;
            }
          }
LAB_0048695e: ;
          iVar8 = iVar8 + 1;
          iVar7 = iVar7 + 1;
          puVar6 = puVar6 + 1;
          psVar3 = psVar3 + 1;
        } while (iVar8 < param_6);
      }
      iVar4 = iVar4 + 1;
      iVar5 = iVar5 + 0xe4;
    } while (iVar4 < param_8);
    if ((local_10 != 0) &&
       (iVar2 = FUN_005190a0(param_1,param_2,&DAT_0096e6b4,local_10), iVar2 != 0)) {
LAB_004869bd: ;
      _DAT_00d3b4e4 = ((&DAT_0096e6b0)[iVar2] - DAT_00bca2d8) % 0xe4;
      _DAT_00d3b4e0 = ((&DAT_0096e6b0)[iVar2] - DAT_00bca2d8) / 0xe4;
      return 1;
    }
  }
  if (param_4 != 0x40000) {
    DAT_00978d98 = 0;
    return 0;
  }
  DAT_00978d9a = 0;
  return 0;
}
