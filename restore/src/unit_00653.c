#include "restore_types.h"
#define DAT_007756f0 (*(undefined4 *)0x007756f0u)
#define DAT_007756f4 (*(undefined4 *)0x007756f4u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007900ea (*(undefined4 *)0x007900eau)
#define DAT_00790134 (*(undefined4 *)0x00790134u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999ec (*(undefined4 *)0x007999ecu)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a1c (*(undefined4 *)0x00799a1cu)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00ea5d00 (*(undefined4 *)0x00ea5d00u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern bool FUN_004435f0();
extern int FUN_004454d0();
extern int FUN_0045d510();
extern int FUN_0045dc50();
extern undefined4 FUN_00480180();

int FUN_00445670(undefined4 param_1,undefined4 param_2,int param_3,undefined4 param_4,uint param_5)

{
  int *piVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint *puVar9;
  uint *puVar10;
  int iVar11;
  int local_8;
  int local_4;
  
  iVar11 = (&DAT_00ea5d00)[DAT_00eaac28 * 0x1276];
  iVar3 = DAT_00eaac28 * 0x1276;
  local_8 = 10000;
  local_4 = 0;
  (&DAT_00ea5d00)[iVar3] = iVar11 + 1;
  if (0x23 < iVar11 + 1) {
    (&DAT_00ea5d00)[iVar3] = 1;
  }
  iVar11 = 1;
  puVar9 = &DAT_007756f4;
  do {
    iVar3 = FUN_004435f0(DAT_00eaac28,param_3,iVar11);
    puVar10 = puVar9 + 1;
    *puVar9 = (uint)(iVar3 != 0);
    iVar3 = DAT_00eaac28;
    iVar11 = iVar11 + 1;
    puVar9 = puVar10;
  } while ((int)puVar10 < 0x775780);
  iVar11 = 1;
  piVar1 = &DAT_00ea5d00 + DAT_00eaac28 * 0x1276;
  do {
    if (*(int *)(&DAT_007756f0 + *piVar1 * 4) != 0) break;
    iVar4 = *piVar1 + 1;
    *piVar1 = iVar4;
    if (0x23 < iVar4) {
      *piVar1 = 1;
    }
    iVar11 = iVar11 + 1;
  } while (iVar11 < 0x24);
  if (*(int *)(&DAT_007756f0 + *piVar1 * 4) == 0) {
    return 0;
  }
  param_3 = 1;
  iVar11 = 0x108;
  do {
    if ((((&DAT_007999d8)[iVar11] == '\x01') &&
        ((((*(short *)((int)&DAT_007999e8 + iVar11) == 0x48 ||
           (*(short *)((int)&DAT_007999e8 + iVar11) == 0xc1)) && ((&DAT_00799a1c)[iVar11] != '\0'))
         && ((0 < *(short *)((int)&DAT_007999f8 + iVar11) &&
             ((byte)(&DAT_007999ec)[iVar11] == param_5)))))) &&
       (0 < *(short *)((int)&DAT_00799a18 + iVar11))) {
      uVar5 = (uint)*(byte *)((int)&DAT_00799a98 + iVar11 + 1);
      cVar2 = *(char *)((int)&DAT_007900c2 + (&DAT_00ea5d00)[iVar3 * 0x1276] + uVar5 * 0xc4);
      if (cVar2 == '\0') {
        iVar4 = (&DAT_00ea5d00)[iVar3 * 0x1276];
        iVar6 = FUN_0045d510(param_3,iVar4);
        iVar3 = DAT_00eaac28;
        if (iVar6 < *(short *)((int)&DAT_007900ea + (uVar5 * 0x62 + iVar4) * 2 + 2)) {
LAB_00445833: ;
          iVar6 = 0x20;
          iVar4 = FUN_004454d0(param_3,iVar4,400);
          iVar3 = DAT_00eaac28;
          if (iVar4 != 0) {
            iVar8 = 8;
            iVar4 = iVar11;
            do {
              iVar7 = (int)*(short *)(&DAT_00799a12 + iVar4);
              iVar4 = iVar7 * 0x108;
              if ((&DAT_007999ea)[iVar7 * 0x84] == 0) {
                iVar6 = iVar6 + -8;
              }
              if (((iVar7 != 0) &&
                  ((int)(short)(&DAT_007999ea)[iVar7 * 0x84] ==
                   (&DAT_00ea5d00)[DAT_00eaac28 * 0x1276])) &&
                 ((short)(&DAT_00799a14)[iVar7 * 0x84] < 400)) {
                iVar6 = iVar6 + -4;
              }
              iVar8 = iVar8 + -1;
            } while (iVar8 != 0);
            if (iVar6 < 0x20) {
              iVar4 = FUN_0045dc50((int)*(short *)((int)&DAT_007999dc + iVar11 + 2),
                                   (int)*(short *)(&DAT_007999e0 + iVar11),param_1,param_2,param_4,
                                   (int)*(short *)((int)&DAT_007999f8 + iVar11));
              iVar3 = DAT_00eaac28;
              if (iVar6 + iVar4 < local_8) {
                local_4 = param_3;
                local_8 = iVar6 + iVar4;
              }
            }
          }
        }
      }
      else if (cVar2 == '\x02') {
        iVar4 = (&DAT_00ea5d00)[iVar3 * 0x1276];
        iVar6 = FUN_0045d510(param_3,iVar4);
        iVar3 = DAT_00eaac28;
        if (iVar6 < *(short *)(&DAT_00790134 + (uVar5 * 0x62 + iVar4) * 2)) goto LAB_00445833;
      }
    }
    iVar11 = iVar11 + 0x108;
    param_3 = param_3 + 1;
    if (0x101cff < iVar11) {
      iVar11 = local_4 * 0x108;
      if ((&DAT_00799a1c)[iVar11] == '\x01') {
        _DAT_00d3b4e4 = (int)(short)(&DAT_007999fe)[local_4 * 0x84];
        _DAT_00d3b4e0 = (int)(short)(&DAT_00799a00)[local_4 * 0x84];
      }
      else {
        iVar11 = FUN_00480180((int)*(short *)((int)&DAT_007999dc + iVar11 + 2),
                              (int)*(short *)(&DAT_007999e0 + iVar11),3);
        if (iVar11 == 0) {
          return 0;
        }
      }
      return local_4;
    }
  } while( true );
}
