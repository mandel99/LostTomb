#include "restore_types.h"
#define DAT_00775780 (*(undefined4 *)0x00775780u)
#define DAT_00775784 (*(undefined4 *)0x00775784u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a1c (*(undefined4 *)0x00799a1cu)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00ea5d04 (*(undefined4 *)0x00ea5d04u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_004434e0();
extern int FUN_0045d510();
extern int FUN_0045dc50();
extern undefined4 FUN_00480180();

int FUN_00445ac0(undefined4 param_1,undefined4 param_2,int param_3,undefined4 param_4,uint param_5)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  uint *puVar4;
  uint *puVar5;
  int iVar6;
  short *psVar7;
  int local_8;
  int local_4;
  
  iVar6 = (&DAT_00ea5d04)[DAT_00eaac28 * 0x1276];
  iVar2 = DAT_00eaac28 * 0x1276;
  (&DAT_00ea5d04)[iVar2] = iVar6 + 1;
  if (0x23 < iVar6 + 1) {
    (&DAT_00ea5d04)[iVar2] = 1;
  }
  local_8 = 10000;
  local_4 = 0;
  iVar6 = 1;
  puVar4 = &DAT_00775784;
  do {
    iVar2 = FUN_004434e0(DAT_00eaac28,param_3,iVar6);
    puVar5 = puVar4 + 1;
    *puVar4 = (uint)(iVar2 != 0);
    iVar2 = DAT_00eaac28;
    iVar6 = iVar6 + 1;
    puVar4 = puVar5;
  } while ((int)puVar5 < 0x775810);
  iVar6 = 1;
  piVar1 = &DAT_00ea5d04 + DAT_00eaac28 * 0x1276;
  do {
    if (*(int *)(&DAT_00775780 + *piVar1 * 4) != 0) break;
    iVar3 = *piVar1 + 1;
    *piVar1 = iVar3;
    if (0x23 < iVar3) {
      *piVar1 = 1;
    }
    iVar6 = iVar6 + 1;
  } while (iVar6 < 0x24);
  if (*(int *)(&DAT_00775780 + *piVar1 * 4) == 0) {
    return 0;
  }
  param_3 = 1;
  psVar7 = &DAT_00799af0;
  do {
    if ((((char)psVar7[-8] == '\x01') &&
        ((((*psVar7 == 0x48 || (*psVar7 == 0xc1)) && ((char)psVar7[0x1a] != '\0')) &&
         ((0 < psVar7[8] && (*(byte *)(psVar7 + 2) == param_5)))))) &&
       (iVar6 = FUN_0045d510(param_3,(&DAT_00ea5d04)[iVar2 * 0x1276]), 99 < iVar6)) {
      iVar3 = 0x20;
      iVar2 = 8;
      iVar6 = param_3;
      do {
        iVar6 = (int)*(short *)(&DAT_00799a12 + iVar6 * 0x108);
        if (((iVar6 != 0) &&
            ((int)(short)(&DAT_007999ea)[iVar6 * 0x84] == (&DAT_00ea5d04)[DAT_00eaac28 * 0x1276]))
           && (0 < (short)(&DAT_00799a14)[iVar6 * 0x84])) {
          iVar3 = iVar3 + -1;
        }
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
      if (iVar3 < 0x20) {
        iVar6 = FUN_0045dc50((int)psVar7[-5],(int)psVar7[-4],param_1,param_2,param_4,(int)psVar7[8])
        ;
        if (iVar3 + iVar6 < local_8) {
          local_4 = param_3;
          local_8 = iVar3 + iVar6;
        }
      }
    }
    psVar7 = psVar7 + 0x84;
    param_3 = param_3 + 1;
    iVar2 = DAT_00eaac28;
  } while ((int)psVar7 < 0x89b6e8);
  iVar6 = local_4 * 0x108;
  if ((&DAT_00799a1c)[iVar6] == '\x01') {
    _DAT_00d3b4e4 = (int)(short)(&DAT_007999fe)[local_4 * 0x84];
    _DAT_00d3b4e0 = (int)(short)(&DAT_00799a00)[local_4 * 0x84];
  }
  else {
    iVar6 = FUN_00480180((int)*(short *)((int)&DAT_007999dc + iVar6 + 2),
                         (int)*(short *)(&DAT_007999e0 + iVar6),3);
    if (iVar6 == 0) {
      return 0;
    }
  }
  return local_4;
}
