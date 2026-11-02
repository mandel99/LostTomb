#include "restore_types.h"
#define DAT_005c7e98 (*(undefined4 *)0x005c7e98u)
#define DAT_005c80f0 (*(undefined4 *)0x005c80f0u)
#define DAT_005c8118 (*(undefined4 *)0x005c8118u)
#define DAT_005c8140 (*(undefined4 *)0x005c8140u)
#define DAT_007056c4 (*(undefined *)0x007056c4u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e209ba (*(undefined4 *)0x00e209bau)
#define DAT_00e209fc (*(undefined4 *)0x00e209fcu)
#define DAT_00e36c74 (*(uint32_t *)0x00e36c74u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00e950f4 (*(undefined *)0x00e950f4u)
#define DAT_00e950f8 (*(undefined *)0x00e950f8u)
#define DAT_00e950fc (*(undefined *)0x00e950fcu)
#define DAT_00e95100 (*(undefined *)0x00e95100u)
#define DAT_00e95104 (*(undefined *)0x00e95104u)
#define DAT_00e95108 (*(undefined *)0x00e95108u)
#define DAT_00e9510c (*(undefined *)0x00e9510cu)
#define DAT_00e95110 (*(undefined *)0x00e95110u)
#define DAT_00e95114 (*(undefined *)0x00e95114u)
#define DAT_00e95118 (*(undefined *)0x00e95118u)
#define DAT_00e9511c (*(undefined *)0x00e9511cu)
#define DAT_00e95120 (*(undefined *)0x00e95120u)
#define DAT_00e951dc (*(undefined *)0x00e951dcu)
#define DAT_00e951e4 (*(undefined *)0x00e951e4u)
#define DAT_00e951ec (*(undefined *)0x00e951ecu)
#define DAT_00e95236 (*(undefined4 *)0x00e95236u)
#define DAT_00f56494 (*(undefined *)0x00f56494u)
#define DAT_00f566ec (*(undefined *)0x00f566ecu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
extern void FUN_00425730();
extern void FUN_00448330();
extern void FUN_004cc770();
extern void FUN_004ccbb0();
extern void FUN_004cccb0();
extern int FUN_004cde40();
extern int FUN_004cde60();
extern void FUN_00536c30();
extern int FUN_0053e590();

void FUN_004e1a50(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined1 *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  char *pcVar8;
  undefined4 uVar9;
  int local_d4;
  int local_d0;
  undefined1 local_c8 [200];
  
  DAT_00e38dc9 = 1;
  FUN_00425730(DAT_00e95110 + 0x10,DAT_00e95104 * 0x10 + -0x2a + DAT_00e9510c,&DAT_005c7e98,1);
  FUN_00425730(DAT_00e95108 * 0x10 + -0x26 + DAT_00e95110,DAT_00e95104 * 0x10 + -0x24 + DAT_00e9510c
               ,&DAT_005c80f0,1);
  if (0 < DAT_00e951ec) {
    local_d4 = 10;
    if (DAT_00e951ec < 0xb) {
      local_d4 = DAT_00e951ec;
    }
    iVar6 = 0;
    local_d0 = DAT_00e95120;
    if (0 < local_d4) {
      iVar7 = 0;
      pcVar8 = &DAT_00e95236 + DAT_00e95120 * 0x30;
      do {
        DAT_00e951dc = (int)*(short *)(pcVar8 + -4);
        if (*(short *)(&DAT_00e209ba + DAT_00e951dc * 0x50) == 4) {
          iVar3 = 4;
        }
        else {
          iVar3 = (-(uint)(*(short *)(&DAT_00e209ba + DAT_00e951dc * 0x50) != 1) & 0xfffffffe) + 2;
        }
        if ((*pcVar8 == '\0') && (iVar3 != 4)) {
          iVar5 = DAT_00e95100 + 8;
          iVar1 = DAT_00f566ec + 0xe;
        }
        else {
          iVar5 = DAT_00e95100 + 0xc;
          iVar1 = DAT_00f566ec + 0xf;
        }
        FUN_0053e590(iVar1 + iVar3,iVar5,iVar7 + 6 + DAT_00e950fc);
        iVar6 = iVar6 + 1;
        uVar9 = DAT_00f56fa4;
        if (DAT_007056c8 != iVar6) {
          uVar9 = DAT_00f56f9c;
        }
        DAT_00e20778 = 0;
        FUN_004ccbb0(0x19,pcVar8[1],DAT_00e95100 + 0x2a,iVar7 + 8 + DAT_00e950fc,uVar9,0);
        FUN_004cccb0(0,(int)*(short *)(pcVar8 + -6),DAT_00e95100 + 0x5a,iVar7 + 8 + DAT_00e950fc,
                     uVar9,0);
        uVar2 = DAT_00e951e4;
        puVar4 = (undefined1 *)(*(int *)(&DAT_00e209fc + DAT_00e951dc * 0x50) + DAT_00e36c74);
        if (-1 < *(short *)(pcVar8 + 4)) {
          DAT_00e951e4 = local_d0;
          FUN_00448330((int)*(short *)(pcVar8 + 4),local_c8,200);
          puVar4 = local_c8;
        }
        DAT_00e951e4 = uVar2;
        FUN_004cc770(puVar4,DAT_00e95100 + 200,iVar7 + 8 + DAT_00e950fc,uVar9,0);
        iVar7 = iVar7 + 0x14;
        local_d0 = local_d0 + 1;
        pcVar8 = pcVar8 + 0x30;
      } while (iVar6 < local_d4);
    }
    if (DAT_007056c4 == 0) {
      uVar9 = 0;
    }
    else {
      uVar9 = 0xf800;
    }
    FUN_00536c30(0,(DAT_00e95108 * 0x10) / 2 + -0x8c + DAT_00e95110,
                 DAT_00e95104 * 0x10 + -0x28 + DAT_00e9510c,0x118,0x1c,uVar9);
    if (0 < DAT_00e9511c) {
      FUN_00425730(DAT_00e950f8 * 0x10 + DAT_00e95100,DAT_00e950fc,&DAT_005c8118,1);
      FUN_00425730(DAT_00e950f8 * 0x10 + DAT_00e95100,DAT_00e950f4 * 0x10 + -0x1a + DAT_00e950fc,
                   &DAT_005c8140,1);
      if (DAT_00e95120 < 1) {
        uVar9 = 0;
      }
      else if (DAT_00e95120 < DAT_00e9511c) {
        uVar9 = FUN_004cde60(DAT_00e95120,DAT_00e9511c);
      }
      else {
        uVar9 = 100;
      }
      iVar6 = FUN_004cde40(DAT_00e950f4 * 0x10 + -0x4d,uVar9);
      if (DAT_00e95118 != 0) {
        iVar6 = DAT_00e95114;
      }
      FUN_0053e590(DAT_00f56494 + 0x27,DAT_00e950f8 * 0x10 + 9 + DAT_00e95100,
                   DAT_00e950fc + 0x1a + iVar6);
    }
  }
  return;
}
