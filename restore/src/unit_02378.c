#include "restore_types.h"
#define DAT_005c50f8 (*(undefined4 *)0x005c50f8u)
#define DAT_005c5114 (*(undefined4 *)0x005c5114u)
#define DAT_005f8518 (*(undefined1 *)0x005f8518u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00bcd5fa (*(undefined4 *)0x00bcd5fau)
#define DAT_00bcd72a (*(undefined4 *)0x00bcd72au)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea5ed8 (*(undefined4 *)0x00ea5ed8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53dd0 (*(undefined *)0x00f53dd0u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern int FUN_004bc620();
extern void FUN_004cd1a0();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_00521a60();

void FUN_00523bb0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uStack_4;
  
  if (-DAT_00f53f48 == -5 || -DAT_00f53f48 + 5 < 0) {
    DAT_00f53f0c = DAT_00e38da8;
    DAT_00f53f08 = DAT_00e38da4;
    DAT_00f53dd0 = (uint)(byte)(&DAT_00ea5ed8)[DAT_00eaac28 * 0x49d8];
    _DAT_00f53f04 = 0x28;
    DAT_00f53f00 = 0x1a;
    FUN_00521a60((int)&uStack_4 + 3,(int)&uStack_4 + 2);
    iVar1 = 0;
    RESTORE_SLICE2(uStack_4,0) = (ushort)(byte)uStack_4;
    if (0 < (int)DAT_00f53dd0) {
      iVar4 = 0;
      iVar3 = 2;
      do {
        iVar1 = iVar1 + 1;
        iVar2 = FUN_004bc620(iVar1);
        if (*(short *)(&DAT_00bcd72a + iVar2 * 0x184) < 0) {
          FUN_004d63d0(0,DAT_00f53f0c + 0x1b8,iVar4 + 0x53 + DAT_00f53f08,0x1e,0x1e,
                       DAT_007056c8 == iVar3 + -1,DAT_00e38da8,DAT_00e38da4);
          if ((&DAT_00bcd5fa)[iVar2 * 0x184] != '\0') {
            RESTORE_SLICE2(uStack_4,0) = CONCAT11(1,(byte)uStack_4);
          }
          FUN_004d63d0(0,DAT_00f53f0c + 500,iVar4 + 0x53 + DAT_00f53f08,0x1e,0x1e,
                       DAT_007056c8 == iVar3,DAT_00e38da8,DAT_00e38da4);
          FUN_004d63d0(0,DAT_00f53f0c + 0x230,iVar4 + 0x53 + DAT_00f53f08,0x1e,0x1e,
                       RESTORE_SLICE1(uStack_4,2) != '\0' && DAT_007056c8 == iVar3 + 1,DAT_00e38da8,DAT_00e38da4
                      );
        }
        iVar3 = iVar3 + 3;
        iVar4 = iVar4 + 0x2c;
      } while (iVar1 < (int)DAT_00f53dd0);
    }
    FUN_004cd1a0(0,&DAT_005c50f8,DAT_007056c8 == 0x13,0,0);
    if (RESTORE_SLICE1(uStack_4,1) != '\0') {
      FUN_004cd1a0(0,&DAT_005c5114,DAT_007056c8 == 0x14,0,0);
      FUN_004d62f0(0x33,0x1e,DAT_00f53f0c + 0x226,DAT_00f53f08 + 0x28,DAT_00f56f8c,0,DAT_00e38da8,
                   DAT_00e38da4);
      FUN_004d62f0(0x33,0x1f,DAT_00f53f0c + 0x226,DAT_00f53f08 + 0x36,DAT_00f56f8c,0,DAT_00e38da8,
                   DAT_00e38da4);
      DAT_005f8518 = 1;
      return;
    }
    FUN_004d62f0(0x33,5,DAT_00f53f0c + 0x226,DAT_00f53f08 + 0x2b,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x33,6,DAT_00f53f0c + 0x226,DAT_00f53f08 + 0x3a,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    DAT_005f8518 = 0;
  }
  return;
}
