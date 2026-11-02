#include "restore_types.h"
#define DAT_005e0720 (*(undefined4 *)0x005e0720u)
#define DAT_00e36c78 (*(undefined4 *)0x00e36c78u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00e92e18 (*(undefined *)0x00e92e18u)
extern void FUN_004cbea0();
extern void * FUN_00561b68();
extern void FUN_00561bdc();
extern undefined4 FUN_005647eb();
extern int FUN_005664d5();
extern uint FUN_00568eab();

undefined4 FUN_004ceed0(undefined4 param_1)

{
  undefined2 uVar1;
  char *pcVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined2 *puVar8;
  int *piVar9;
  int iVar10;
  undefined2 local_38;
  undefined4 uStack_36;
  undefined2 uStack_32;
  undefined2 local_30;
  undefined2 uStack_2e;
  undefined2 local_2c;
  int local_28 [3];
  undefined2 local_1c;
  undefined2 local_1a;
  undefined4 local_18;
  int local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  iVar6 = 0;
  DAT_00e36c78 = (void *)0x0;
  DAT_00e36c78 = FUN_00561b68(3100000);
  if (DAT_00e36c78 != (void *)0x0) {
    local_38 = 0;
    RESTORE_SLICE2(uStack_36,0) = 0;
    RESTORE_SLICE2(uStack_36,2) = 0;
    uStack_32 = 0;
    local_30 = 0;
    uStack_2e = 0;
    piVar9 = local_28;
    for (iVar3 = 10; iVar3 != 0; iVar3 = iVar3 + -1) {
      *piVar9 = 0;
      piVar9 = piVar9 + 1;
    }
    local_2c = 0;
    FUN_004cbea0(&DAT_005e0720,&local_38,2);
    local_28[1] = DAT_00e38db0;
    local_14 = DAT_00e38dac * DAT_00e38db0 * 3;
    uStack_2e = 0x36;
    local_2c = 0;
    uStack_36 = local_14 + 0x36;
    local_28[0] = 0x28;
    local_28[2] = DAT_00e38dac;
    local_1c = 1;
    local_1a = 0x18;
    local_18 = 0;
    local_10 = 0;
    local_c = 0;
    local_8 = 0;
    local_4 = 0;
    iVar3 = FUN_00568eab(param_1,0x8301,0x180);
    if (iVar3 != -1) {
      FUN_004cbea0(&local_38,DAT_00e36c78,0xe);
      FUN_004cbea0(local_28,(int)DAT_00e36c78 + 0xe,0x28);
      iVar7 = DAT_00e92e18 + DAT_00e38dac * DAT_00e38db0 * 2;
      pcVar2 = (char *)((int)DAT_00e36c78 + 0x36);
      iVar4 = DAT_00e38db0;
      iVar5 = DAT_00e38dac;
      if (0 < DAT_00e38dac) {
        do {
          iVar10 = 0;
          if (0 < iVar4) {
            puVar8 = (undefined2 *)(iVar7 + iVar4 * -2);
            iVar4 = iVar4 * 2;
            do {
              uVar1 = *puVar8;
              puVar8 = puVar8 + 1;
              *pcVar2 = (char)uVar1 << 3;
              pcVar2[1] = (byte)(CONCAT22((short)((uint)iVar4 >> 0x10),uVar1) >> 3) & 0xfc;
              pcVar2[2] = (byte)((ushort)uVar1 >> 8) & 0xf8;
              pcVar2 = pcVar2 + 3;
              iVar10 = iVar10 + 1;
              iVar4 = DAT_00e38db0;
              iVar5 = DAT_00e38dac;
            } while (iVar10 < DAT_00e38db0);
          }
          iVar6 = iVar6 + 1;
          iVar7 = iVar7 + iVar4 * -2;
        } while (iVar6 < iVar5);
      }
      FUN_005664d5(iVar3,DAT_00e36c78,(iVar5 * iVar4 + 0x12) * 3);
      FUN_005647eb(iVar3);
    }
    if (DAT_00e36c78 != (void *)0x0) {
      FUN_00561bdc(DAT_00e36c78);
    }
    return 1;
  }
  return 0;
}
