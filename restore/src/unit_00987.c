#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00784ca4 (*(undefined *)0x00784ca4u)
#define DAT_00784ca8 (*(undefined *)0x00784ca8u)
#define DAT_00784cac (*(undefined *)0x00784cacu)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d7f0 (*(undefined *)0x0078d7f0u)
#define DAT_0078d7f4 (*(undefined *)0x0078d7f4u)
#define DAT_0078d9a8 (*(undefined4 *)0x0078d9a8u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a0a (*(undefined4 *)0x00799a0au)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a9a (*(undefined4 *)0x00799a9au)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define DAT_00ea5e10 (*(undefined4 *)0x00ea5e10u)
#define DAT_00ea5e14 (*(undefined4 *)0x00ea5e14u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
#define DAT_00f565dc (*(undefined *)0x00f565dcu)
#define DAT_00f569d4 (*(undefined *)0x00f569d4u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern int FUN_0046a400();
extern void FUN_0047c480();
extern void FUN_00482a90();
extern void FUN_00482b50();

void FUN_0046d480(void)

{
  int *piVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  bool bVar9;
  int local_14;
  int local_10;
  int local_c;
  short *local_8;
  int local_4;
  
  local_14 = 0;
  local_10 = DAT_00bca2d8;
  if (0 < DAT_00bca2d4) {
    do {
      local_c = 0;
      if (0 < DAT_00bca2d0) {
        psVar5 = (short *)(&DAT_00b4e6fc + local_10 * 2);
        do {
          if (((*(byte *)(&DAT_00af598c + local_10) & 8) != 0) && (*psVar5 == 0)) {
            uVar6 = (byte)(&DAT_00acf85c)[local_10] & 1;
            if (DAT_00605a3c == 1) {
              uVar6 = 0;
            }
            piVar1 = &DAT_00b7482c + local_10;
            iVar8 = (&DAT_00b7482c)[local_10];
            if (iVar8 == DAT_00784ca4) {
              iVar8 = 0x58;
              *piVar1 = DAT_00f569d4;
            }
            else {
              if (iVar8 == DAT_00784ca4 + 1) {
                iVar8 = 0x58;
                iVar4 = DAT_00f569d4 + 1;
              }
              else {
                if (iVar8 == DAT_00784ca8) {
                  iVar8 = 0x59;
                  *piVar1 = DAT_00f569d4 + 2;
                  goto LAB_0046d55d;
                }
                if (iVar8 != DAT_00784cac) {
                  FUN_0047c480(0,local_c,local_14);
                  goto LAB_0046d696;
                }
                iVar8 = 0x5d;
                iVar4 = DAT_00f565dc + uVar6;
              }
              *piVar1 = iVar4;
            }
LAB_0046d55d: ;
            iVar3 = FUN_0046a400(1,iVar8,local_c,local_14,0,0);
            sVar2 = (short)iVar3;
            *psVar5 = sVar2;
            iVar7 = iVar3 * 0x108;
            (&DAT_007999d8)[iVar7] = 1;
            iVar4 = DAT_00605a3c;
            if (iVar8 == 0x5d) {
              (&DAT_00799a2c)[iVar3 * 0x84] = (short)uVar6;
            }
            else if (iVar8 == 0x59) {
              psVar5[1] = sVar2;
              psVar5[0xe4] = sVar2;
              psVar5[0xe5] = sVar2;
              bVar9 = DAT_00605a3c == 0;
              *(undefined1 *)(&DAT_00799a9a + iVar3 * 0x84) = 100;
              if (bVar9) {
                FUN_00482a90((int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                             (int)*(short *)(&DAT_007999e0 + iVar7),2,6);
              }
              else {
                FUN_00482b50((int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                             (int)*(short *)(&DAT_007999e0 + iVar7),2,8);
              }
              iVar8 = DAT_00eaac28;
              if ((&DAT_00ea5e10)[DAT_00eaac28 * 0x1276] == 0) {
                sVar2 = *(short *)(&DAT_007999e0 + iVar7);
                (&DAT_00ea5e10)[DAT_00eaac28 * 0x1276] =
                     (int)*(short *)((int)&DAT_007999dc + iVar7 + 2);
                (&DAT_00ea5e14)[iVar8 * 0x1276] = (int)sVar2;
              }
            }
            else if (iVar8 == 0x58) {
              *(undefined1 *)(&DAT_00799a9a + iVar3 * 0x84) = 100;
              (&DAT_00799a0a)[iVar7] = (char)uVar6;
              if (iVar4 == 0) {
                FUN_00482a90((int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                             (int)*(short *)(&DAT_007999e0 + iVar7),1,3);
              }
              else {
                FUN_00482b50((int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                             (int)*(short *)(&DAT_007999e0 + iVar7),1,5);
              }
            }
          }
LAB_0046d696: ;
          local_c = local_c + 1;
          local_10 = local_10 + 1;
          psVar5 = psVar5 + 1;
        } while (local_c < DAT_00bca2d0);
      }
      local_14 = local_14 + 1;
      local_10 = local_10 + DAT_00bca2dc;
    } while (local_14 < DAT_00bca2d4);
  }
  local_4 = DAT_0078d790;
  iVar4 = 1;
  iVar8 = 0;
  DAT_0078d7f0 = 0;
  if (0 < DAT_0078d790) {
    psVar5 = &DAT_00799af0;
    do {
      if (((char)psVar5[-8] == '\x01') && (*psVar5 == 0x59)) {
        (&DAT_0078d9a8)[iVar8] = (short)iVar4;
        iVar8 = iVar8 + 1;
        DAT_0078d7f0 = iVar8;
        if (499 < iVar8) {
          DAT_0078d7f0 = 499;
          iVar8 = 499;
        }
      }
      iVar4 = iVar4 + 1;
      psVar5 = psVar5 + 0x84;
    } while (iVar4 <= local_4);
    if (0 < iVar8) {
      local_8 = &DAT_00799af0;
      do {
        iVar4 = iVar8;
        if (((char)local_8[-8] == '\x01') && (*local_8 == 0x58)) {
          local_c = 1000;
          local_14 = 0;
          RESTORE_SLICE2(local_14,0) = 0;
          DAT_0078d7f4 = 0;
          if (0 < iVar8) {
            iVar3 = (int)local_8[-4];
            iVar7 = (int)local_8[-5];
            psVar5 = &DAT_0078d9a8;
            local_10 = iVar8;
            do {
              sVar2 = *psVar5;
              psVar5 = psVar5 + 1;
              iVar4 = sVar2 * 0x108;
              _DAT_00d3b4f8 = (int)*(short *)(&DAT_007999e0 + iVar4);
              iVar4 = (int)*(short *)((int)&DAT_007999dc + iVar4 + 2);
              if (iVar4 < iVar7) {
                DAT_00d3b4fc = iVar7 - iVar4;
              }
              else if (iVar7 < iVar4) {
                DAT_00d3b4fc = iVar4 - iVar7;
              }
              else {
                DAT_00d3b4fc = 0;
              }
              if (_DAT_00d3b4f8 < iVar3) {
                _DAT_00d3b4f8 = iVar3 - _DAT_00d3b4f8;
              }
              else if (iVar3 < _DAT_00d3b4f8) {
                _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar3;
              }
              else {
                _DAT_00d3b4f8 = 0;
              }
              iVar4 = _DAT_00d3b4f8;
              if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
                iVar4 = DAT_00d3b4fc;
              }
              if (iVar4 < local_c) {
                local_14 = (int)sVar2;
                local_c = iVar4;
              }
              local_10 = local_10 + -1;
              iVar4 = DAT_0078d7f0;
              DAT_0078d7f4 = iVar8;
            } while (local_10 != 0);
          }
          local_8[1] = (short)local_14;
        }
        local_8 = local_8 + 0x84;
        local_4 = local_4 + -1;
        iVar8 = iVar4;
      } while (local_4 != 0);
    }
  }
  return;
}
