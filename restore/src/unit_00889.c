#include "restore_types.h"
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_00790134 (*(undefined4 *)0x00790134u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00ea42f8 (*(undefined4 *)0x00ea42f8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53dd8 (*(undefined *)0x00f53dd8u)
#define DAT_00f53ddc (*(undefined4 *)0x00f53ddcu)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
#define DAT_00f53e70 (*(undefined4 *)0x00f53e70u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern int FUN_0045bb50();
extern int FUN_0045d510();

void FUN_0045b880(int param_1,int *param_2,int *param_3,int *param_4,int *param_5,int *param_6,
                 int *param_7)

{
  int iVar1;
  short sVar2;
  bool bVar3;
  bool bVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int local_b0;
  int local_ac;
  int aiStack_90 [36];
  
  uVar6 = _DAT_00d3b4e4;
  uVar5 = _DAT_00d3b4e0;
  local_ac = 100;
  uVar9 = (uint)*(byte *)((int)&DAT_00799a98 + param_1 * 0x108 + 1);
  bVar3 = false;
  if (((&DAT_007999e8)[param_1 * 0x84] == 0x47) ||
     (iVar10 = DAT_00f53e6c, (&DAT_007999e8)[param_1 * 0x84] == 0xbf)) {
    bVar3 = true;
    iVar10 = DAT_00f53dd8;
  }
  bVar4 = false;
  local_b0 = 0;
  uVar7 = uVar5;
  uVar8 = uVar6;
  if (0 < iVar10) {
    do {
      if (bVar3) {
        iVar12 = (&DAT_00f53ddc)[local_b0];
      }
      else {
        iVar12 = (&DAT_00f53e70)[local_b0];
      }
      aiStack_90[iVar12] = 100;
      if (*(char *)((int)&DAT_007900c2 + iVar12 + uVar9 * 0xc4) == '\x02') {
        bVar4 = true;
        if ((&DAT_007999e8)[param_1 * 0x84] == 0x48) {
          iVar1 = iVar12 + uVar9 * 0x62;
          iVar11 = FUN_0045d510(param_1,iVar12);
          if (iVar11 < *(short *)(&DAT_00790134 + iVar1 * 2)) {
            if (0 < iVar12) {
              iVar11 = FUN_0045d510(param_1,iVar12);
              sVar2 = *(short *)(&DAT_00790134 + iVar1 * 2);
LAB_0045b9f4: ;
              local_ac = (iVar11 * 100) / (int)sVar2;
            }
          }
          else {
            local_ac = 100;
          }
        }
        else {
          sVar2 = *(short *)(&DAT_00790134 + (iVar12 + uVar9 * 0x62) * 2);
          if ((short)(&DAT_00799a2e)[iVar12 + param_1 * 0x84] < sVar2) {
            if (0 < iVar12) {
              iVar11 = (int)(short)(&DAT_00799a2e)[iVar12 + param_1 * 0x84];
              goto LAB_0045b9f4;
            }
          }
          else {
            local_ac = 100;
          }
        }
        aiStack_90[iVar12] = local_ac;
      }
      local_b0 = local_b0 + 1;
    } while (local_b0 < iVar10);
    uVar7 = _DAT_00d3b4e0;
    uVar8 = _DAT_00d3b4e4;
    if (bVar4) {
      local_b0 = 0;
      do {
        if (bVar3) {
          iVar12 = (&DAT_00f53ddc)[local_b0];
        }
        else {
          iVar12 = (&DAT_00f53e70)[local_b0];
        }
        iVar11 = FUN_0045bb50(param_1,iVar12,0);
        if ((*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + iVar12) * 4) == 0) && (iVar11 != 0))
        {
          iVar1 = aiStack_90[iVar12];
          if (iVar1 < *param_4) {
            *param_5 = *param_4;
            *param_3 = *param_2;
            *param_7 = *param_6;
            *param_4 = iVar1;
            *param_2 = iVar12;
            *param_6 = iVar11;
          }
          else if (iVar1 < *param_5) {
            *param_5 = iVar1;
            *param_3 = iVar12;
            *param_7 = iVar11;
          }
        }
        local_b0 = local_b0 + 1;
        uVar7 = uVar5;
        uVar8 = uVar6;
      } while (local_b0 < iVar10);
    }
  }
  _DAT_00d3b4e4 = uVar8;
  _DAT_00d3b4e0 = uVar7;
  return;
}
