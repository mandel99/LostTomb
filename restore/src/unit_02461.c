#include "restore_types.h"
#define DAT_005760c0 (*(undefined4 *)0x005760c0u)
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005d2c48 (*(undefined4 *)0x005d2c48u)
#define DAT_005d2c4c (*(undefined4 *)0x005d2c4cu)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
#define PTR_DAT_005c3d80 (*(unsigned char * *)0x005c3d80u)
extern undefined4 FUN_00443a80();
extern undefined4 FUN_00443ad0();
extern void FUN_004cce90();
extern void FUN_004cd1a0();
extern void FUN_004d6200();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00537cf0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int local_10;
  int local_c;
  int *local_8;
  
  iVar2 = DAT_00e38da8;
  iVar3 = DAT_00e38da4;
  iVar1 = DAT_00e38da8 + -0x13;
  FUN_00536820(iVar1,DAT_00e38da4,0x2b,0x10);
  iVar2 = iVar2 + -9;
  FUN_004d62f0(0x2c,0xcc,iVar2,iVar3 + 9,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2c,0xcd,iVar2,iVar3 + 0x54,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2c,0xce,iVar2,iVar3 + 0x6d,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2c,0xcd,iVar2,iVar3 + 0x9f,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2c,0xce,iVar2,iVar3 + 0xb8,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  local_10 = 1;
  local_c = 2;
  local_8 = &DAT_005760c0;
  do {
    iVar2 = *local_8;
    if ((((iVar2 != 0) && (iVar2 != 0x1b)) && (-1 < iVar2)) && (iVar2 < 0x24)) {
      if (iVar2 == 0x12) {
        local_10 = 1;
        iVar5 = 3;
      }
      else if (iVar2 < 0x12) {
        iVar5 = 0;
      }
      else {
        iVar5 = 3;
      }
      iVar6 = local_10 * 0x23 + iVar1;
      FUN_0053e590(DAT_00f571fc + iVar2,iVar6 + 0x28,iVar3 + 0x32 + iVar5 * 0x1a);
      iVar5 = iVar3 + iVar5 * 0x19;
      iVar6 = iVar6 + 0x22;
      FUN_004d63d0(0,iVar6,iVar5 + 0x50,0x23,0x14,DAT_007056c8 == local_c + -1,DAT_00e38da8,
                   DAT_00e38da4);
      FUN_004d63d0(0,iVar6,iVar5 + 0x69,0x23,0x14,DAT_007056c8 == local_c,DAT_00e38da8,DAT_00e38da4)
      ;
      iVar4 = FUN_00443a80(iVar2);
      if (iVar4 == 0) {
        iVar4 = FUN_00443ad0(iVar2);
        if (iVar4 != 0) goto LAB_00537f50;
        FUN_004d62f0(0x12,6,iVar6,iVar5 + 0x55,DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4);
        FUN_004d62f0(0x12,6,iVar6,iVar5 + 0x6e,DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4);
      }
      else {
LAB_00537f50: ;
        FUN_004cce90(*(undefined4 *)(&DAT_005d2c48 + iVar2 * 8),0x40,&DAT_0057c048,iVar6,
                     iVar5 + 0x55,0x1e,DAT_00f56f84,0);
        FUN_004cce90(*(undefined4 *)(&DAT_005d2c4c + iVar2 * 8),0x40,&DAT_0057c048,iVar6,
                     iVar5 + 0x6e,0x1e,DAT_00f56f84,0);
      }
      local_10 = local_10 + 1;
    }
    local_8 = local_8 + 1;
    local_c = local_c + 2;
    if (0x576143 < (int)local_8) {
      FUN_004cd1a0(0,&PTR_DAT_005c3d80,DAT_007056c8 == 0x43,0,0);
      FUN_004d6200(0x2c,0xd3,&PTR_DAT_005c3d80,DAT_00f56f8c,0,5,8,DAT_00e38da8,DAT_00e38da4);
      FUN_004d6250(0xd,1,iVar1,iVar3 + 0xd1,0x260,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      return;
    }
  } while( true );
}
