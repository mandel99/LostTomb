#include "restore_types.h"
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
#define DAT_007056c4 (*(undefined *)0x007056c4u)
#define DAT_0077592c (*(undefined4 *)0x0077592cu)
#define DAT_0077646c (*(undefined4 *)0x0077646cu)
#define DAT_007770f0 (*(undefined4 *)0x007770f0u)
#define DAT_0077de02 (*(undefined4 *)0x0077de02u)
#define DAT_0077de04 (*(undefined4 *)0x0077de04u)
#define DAT_0077de05 (*(undefined4 *)0x0077de05u)
#define DAT_0077de4e (*(undefined4 *)0x0077de4eu)
#define DAT_00784f14 (*(undefined *)0x00784f14u)
#define DAT_00784f18 (*(undefined *)0x00784f18u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00f56974 (*(undefined *)0x00f56974u)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
extern undefined4 FUN_00443000();
extern undefined4 FUN_00443040();
extern bool FUN_00447790();
extern int FUN_004c5240();
extern void FUN_004ccc30();
extern void FUN_004cd510();
extern void FUN_004cd890();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d6860();
extern undefined4 FUN_0052dac0();
extern int FUN_0053e590();

void FUN_0052db10(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  bool bVar11;
  int local_20;
  int local_1c;
  int local_18;
  uint local_10;
  int local_c;
  int local_8;
  
  iVar9 = DAT_00e38dac;
  iVar2 = DAT_00784f14 + -1;
  cVar1 = (&DAT_0077de02)[DAT_00784f18 * 0x6a];
  if (cVar1 == '\0') {
    FUN_004d6250(0x2f,1,(DAT_00e38db0 + -0xf0) / 2,DAT_00e38dac + -0x2e,0xf0,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
    return;
  }
  if (cVar1 == '\x02') {
    FUN_004d6250(0x2f,0x13,(DAT_00e38db0 + -0xf0) / 2,DAT_00e38dac + -0x2e,0xf0,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
    return;
  }
  if (cVar1 == '\x04') {
    FUN_004d6250(0x2f,0xd,(DAT_00e38db0 + -0xf0) / 2,DAT_00e38dac + -0x2e,0xf0,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
    return;
  }
  if (((cVar1 == '\x01') || (cVar1 == '\x03')) || (cVar1 == '\x05')) {
    iVar10 = (DAT_00e38db0 + -500) / 2;
    iVar3 = DAT_00e38dac + -0x6c;
    if ((&DAT_0077de05)[DAT_00784f18 * 0x6a] != '\0') {
      local_1c = 0;
      local_10 = 0;
      FUN_004d62f0(0x2f,0xb,iVar10 + 0x1e,iVar3,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      local_18 = iVar9 + -0x5d;
      local_20 = 1;
      local_c = 0xf;
      do {
        iVar4 = FUN_00443000(iVar2,local_20);
        if ((iVar4 != 0) && (cVar1 = FUN_00447790(DAT_00784f18,local_20), cVar1 != '\0')) {
          iVar4 = local_1c * 0x87;
          iVar5 = FUN_004c5240(*(undefined4 *)
                                (&DAT_0077646c +
                                (local_20 + (uint)(byte)(&DAT_0077de04)[DAT_00784f18 * 0x6a] * 0x24)
                                * 4));
          FUN_004cd890(iVar4 + iVar10,local_c + 1 + iVar3,0x13,0x11);
          FUN_0053e590(DAT_00f571fc + local_20,iVar4 + 1 + iVar10,local_c + 2 + iVar3);
          if (iVar5 < 0x1a) {
            if (0xf < iVar5) {
              iVar7 = iVar4 + 0xd + iVar10;
              iVar8 = DAT_00f56974 + 1;
              goto LAB_0052e13b;
            }
            if (0 < iVar5) {
              iVar7 = iVar4 + 0xd + iVar10;
              iVar8 = DAT_00f56974;
              goto LAB_0052e13b;
            }
          }
          else {
            iVar7 = iVar4 + 0xd + iVar10;
            iVar8 = DAT_00f56974 + 2;
LAB_0052e13b: ;
            FUN_0053e590(iVar8,iVar7,local_18);
          }
          local_8 = *(int *)(&DAT_0077592c +
                            (local_20 + (uint)(byte)(&DAT_0077de04)[DAT_00784f18 * 0x6a] * 0x24) * 4
                            );
          if (iVar5 < local_8) {
            iVar5 = local_8;
          }
          switch((int)(local_20)) {
          case 0x18: ;
          case 0x19: ;
          case 0x1a: ;
          case 0x1e: ;
          case 0x23: ;
            break;
          case 0x1b: ;
          case 0x1c: ;
          case 0x1d: ;
          case 0x1f: ;
          case 0x20: ;
          case 0x21: ;
          case 0x22: ;
default: ;
            if (((local_20 != 0x1c) && (local_20 != 10)) && (local_20 != 0x24)) {
              local_8 = local_8 * 100;
              iVar5 = iVar5 * 100;
            }
          }
          cVar1 = FUN_0052dac0(iVar2,local_20);
          if (cVar1 == '\0') {
            DAT_00e20778 = 0;
            FUN_004d6860(local_8,0x40,&DAT_007027d4,iVar4 + 0x12 + iVar10,local_18 + 8,DAT_00f56f84,
                         0,DAT_00e38da8,DAT_00e38da4);
            FUN_004d62f0(0x2f,0xc,DAT_00e20778 + iVar4 + 0x1e + -0xe + iVar10,local_18 + 8,
                         DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4);
            FUN_004d6860(iVar5,0x40,&DAT_007027d4,DAT_00e20778 + iVar4 + 0x1e + -0x14 + iVar10,
                         local_18 + 8,DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4);
          }
          local_1c = local_1c + 1;
          local_10 = local_10 + 1;
          uVar6 = local_10 & 0x80000001;
          bVar11 = uVar6 == 0;
          if ((int)uVar6 < 0) {
            bVar11 = (uVar6 - 1 | 0xfffffffe) == 0xffffffff;
          }
          if (bVar11) {
            local_1c = 0;
            local_c = local_c + 0x14;
            local_18 = local_18 + 0x14;
          }
        }
        local_20 = local_20 + 1;
      } while (local_20 < 0x24);
      local_18 = iVar9 + -0x5d;
      FUN_004cd510(iVar10 + 0xf0,local_18,iVar10 + 0xf0,iVar9 + -0x12,0);
      local_1c = 0;
      local_10 = 0;
      FUN_004d62f0(0x2f,10,iVar10 + 0x17c,iVar3,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      local_20 = 1;
      local_c = 0xf;
      do {
        iVar9 = FUN_00443040(iVar2,local_20);
        if ((iVar9 != 0) && (cVar1 = FUN_00447790(DAT_00784f18,local_20), cVar1 != '\0')) {
          iVar9 = local_1c * 0x87;
          iVar4 = FUN_004c5240(*(undefined4 *)
                                (&DAT_0077646c +
                                (local_20 + (uint)(byte)(&DAT_0077de04)[DAT_00784f18 * 0x6a] * 0x24)
                                * 4));
          FUN_004cd890(iVar9 + 0x118 + iVar10,local_18,0x13,0x11);
          FUN_0053e590(DAT_00f571fc + local_20,iVar9 + 0x119 + iVar10,local_c + 1 + iVar3);
          if (iVar4 < 0x1a) {
            if (0xf < iVar4) {
              iVar5 = iVar9 + 0x125 + iVar10;
              iVar7 = DAT_00f56974 + 1;
              goto LAB_0052e42a;
            }
            if (0 < iVar4) {
              iVar5 = iVar9 + 0x125 + iVar10;
              iVar7 = DAT_00f56974;
              goto LAB_0052e42a;
            }
          }
          else {
            iVar5 = iVar9 + 0x125 + iVar10;
            iVar7 = DAT_00f56974 + 2;
LAB_0052e42a: ;
            FUN_0053e590(iVar7,iVar5,local_18);
          }
          iVar5 = *(int *)(&DAT_0077592c +
                          (local_20 + (uint)(byte)(&DAT_0077de04)[DAT_00784f18 * 0x6a] * 0x24) * 4);
          if (iVar4 < iVar5) {
            iVar4 = iVar5;
          }
          switch((int)(local_20)) {
          case 0x18: ;
          case 0x19: ;
          case 0x1a: ;
          case 0x1e: ;
          case 0x23: ;
            break;
default: ;
            if (((local_20 != 0x1c) && (local_20 != 10)) && (local_20 != 0x24)) {
              iVar5 = iVar5 * 100;
              iVar4 = iVar4 * 100;
            }
          }
          iVar7 = local_18 + 8;
          DAT_00e20778 = 0;
          FUN_004d6860(iVar5,0x40,&DAT_007027d4,iVar9 + 0x12a + iVar10,iVar7,DAT_00f56f84,0,
                       DAT_00e38da8,DAT_00e38da4);
          FUN_004d62f0(0x2f,0xc,DAT_00e20778 + iVar9 + 0x136 + -0xe + iVar10,iVar7,DAT_00f56f84,0,
                       DAT_00e38da8,DAT_00e38da4);
          FUN_004d6860(iVar4,0x40,&DAT_007027d4,DAT_00e20778 + iVar9 + 0x136 + -0x14 + iVar10,iVar7,
                       DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4);
          local_1c = local_1c + 1;
          local_10 = local_10 + 1;
          uVar6 = local_10 & 0x80000001;
          bVar11 = uVar6 == 0;
          if ((int)uVar6 < 0) {
            bVar11 = (uVar6 - 1 | 0xfffffffe) == 0xffffffff;
          }
          if (bVar11) {
            local_1c = 0;
            local_c = local_c + 0x14;
            local_18 = local_18 + 0x14;
          }
        }
        local_20 = local_20 + 1;
        if (0x23 < local_20) {
          return;
        }
      } while( true );
    }
    DAT_00e20778 = 0;
    FUN_004d62f0(0x2f,5,iVar10 + 0x32,DAT_00e38dac + -0x4b,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4)
    ;
    DAT_00e20778 = 0;
    local_20 = 1;
    do {
      iVar3 = FUN_00443000(iVar2,local_20);
      if ((iVar3 != 0) && (cVar1 = FUN_00447790(DAT_00784f18,local_20), cVar1 != '\0')) {
        iVar3 = FUN_004c5240(*(undefined4 *)
                              (&DAT_0077646c +
                              (local_20 + (uint)(byte)(&DAT_0077de04)[DAT_00784f18 * 0x6a] * 0x24) *
                              4));
        FUN_004cd890(DAT_00e20778 + 100 + iVar10,iVar9 + -0x50,0x13,0x11);
        FUN_0053e590(DAT_00f571fc + local_20,DAT_00e20778 + 0x65 + iVar10,iVar9 + -0x4f);
        if (iVar3 < 0x1a) {
          if (0xf < iVar3) {
            iVar3 = DAT_00e20778 + 0x71 + iVar10;
            iVar4 = DAT_00f56974 + 1;
            goto LAB_0052dd8a;
          }
          if (0 < iVar3) {
            iVar3 = DAT_00e20778 + 0x71 + iVar10;
            iVar4 = DAT_00f56974;
            goto LAB_0052dd8a;
          }
        }
        else {
          iVar3 = DAT_00e20778 + 0x71 + iVar10;
          iVar4 = DAT_00f56974 + 2;
LAB_0052dd8a: ;
          FUN_0053e590(iVar4,iVar3,iVar9 + -0x4f);
        }
        DAT_00e20778 = DAT_00e20778 + 0x20;
      }
      local_20 = local_20 + 1;
    } while (local_20 < 0x24);
    FUN_004d62f0(0x2f,4,iVar10 + 0x32,iVar9 + -0x37,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    DAT_00e20778 = 0;
    local_20 = 1;
    do {
      iVar3 = FUN_00443040(iVar2,local_20);
      if ((iVar3 != 0) && (cVar1 = FUN_00447790(DAT_00784f18,local_20), cVar1 != '\0')) {
        iVar4 = FUN_004c5240(*(undefined4 *)
                              (&DAT_0077646c +
                              (local_20 + (uint)(byte)(&DAT_0077de04)[DAT_00784f18 * 0x6a] * 0x24) *
                              4));
        iVar3 = iVar9 + -0x3a;
        FUN_004cd890(DAT_00e20778 + 100 + iVar10,iVar3,0x13,0x11);
        FUN_0053e590(DAT_00f571fc + local_20,DAT_00e20778 + 0x65 + iVar10,iVar3);
        if (iVar4 < 0x1a) {
          if (0xf < iVar4) {
            iVar4 = DAT_00e20778 + 0x71 + iVar10;
            iVar5 = DAT_00f56974 + 1;
            goto LAB_0052ded2;
          }
          if (0 < iVar4) {
            iVar4 = DAT_00e20778 + 0x71 + iVar10;
            iVar5 = DAT_00f56974;
            goto LAB_0052ded2;
          }
        }
        else {
          iVar4 = DAT_00e20778 + 0x71 + iVar10;
          iVar5 = DAT_00f56974 + 2;
LAB_0052ded2: ;
          FUN_0053e590(iVar5,iVar4,iVar3);
        }
        DAT_00e20778 = DAT_00e20778 + 0x20;
      }
      local_20 = local_20 + 1;
    } while (local_20 < 0x24);
    FUN_004d63d0(0,iVar10 + 0x32,iVar9 + -0x28,400,0x14,DAT_007056c4 != 0,DAT_00e38da8,DAT_00e38da4)
    ;
    iVar9 = iVar9 + -0x23;
    FUN_004ccc30(8,0,(int)*(short *)((int)&DAT_0077de4e + DAT_00784f18 * 0x6a),iVar10 + 0x3c,iVar9,
                 DAT_00f56f8c,0);
    if ((byte)(&DAT_0077de04)[DAT_00784f18 * 0x6a] < 0x32) {
      if ((&DAT_007770f0)[(uint)(byte)(&DAT_0077de04)[DAT_00784f18 * 0x6a] * 0x144] == '\x01') {
        FUN_004d62f0(0x2f,6,DAT_00e20778 + 0x3c + iVar10,iVar9,DAT_00f56f8c,0,DAT_00e38da8,
                     DAT_00e38da4);
        return;
      }
      FUN_004d62f0(0x2f,7,DAT_00e20778 + 0x3c + iVar10,iVar9,DAT_00f56f8c,0,DAT_00e38da8,
                   DAT_00e38da4);
      return;
    }
  }
  else {
    FUN_004d6250(0x2f,0,(DAT_00e38db0 + -0xf0) / 2,DAT_00e38dac + -0x2e,0xf0,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
  }
  return;
}
