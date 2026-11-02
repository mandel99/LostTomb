#include "restore_types.h"
#define DAT_005c4ec8 (*(undefined4 *)0x005c4ec8u)
#define DAT_005c4ee4 (*(undefined4 *)0x005c4ee4u)
#define DAT_005f8518 (*(undefined1 *)0x005f8518u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00bcba2f (*(undefined4 *)0x00bcba2fu)
#define DAT_00bcba4a (*(undefined4 *)0x00bcba4au)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f53dd4 (*(undefined *)0x00f53dd4u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern undefined4 FUN_0040b5e0();
extern int FUN_004b78b0();
extern int FUN_004b78f0();
extern void FUN_004cd1a0();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_00521a60();

void FUN_00522a60(void)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  bool bVar7;
  char local_6;
  char local_5;
  int local_4;
  
  if (-DAT_00f53f48 == -5 || -DAT_00f53f48 + 5 < 0) {
    DAT_00f53f0c = DAT_00e38da8;
    DAT_00f53f08 = DAT_00e38da4;
    _DAT_00f53f04 = 0x28;
    DAT_00f53f00 = 0x1a;
    DAT_00f53dd4 = FUN_004b78f0();
    FUN_00521a60(&local_5,&local_6);
    bVar1 = false;
    if (0 < DAT_00f53dd4) {
      iVar6 = 0;
      iVar4 = 2;
      local_4 = 0;
      do {
        local_4 = local_4 + 1;
        iVar3 = FUN_004b78b0(local_4);
        iVar5 = iVar3 * 0x90;
        if (*(short *)(&DAT_00bcba4a + iVar5) < 0) {
          FUN_004d63d0(0,DAT_00f53f0c + 0x1b8,iVar6 + 0x53 + DAT_00f53f08,0x1e,0x1e,
                       DAT_007056c8 == iVar4 + -1,DAT_00e38da8,DAT_00e38da4);
          FUN_004d63d0(0,DAT_00f53f0c + 500,iVar6 + 0x53 + DAT_00f53f08,0x1e,0x1e,
                       DAT_007056c8 == iVar4,DAT_00e38da8,DAT_00e38da4);
          iVar3 = FUN_0040b5e0(iVar3);
          if (*(short *)(&DAT_00bcba4a + iVar5) < 0) {
            cVar2 = local_5;
            if (iVar3 != 0) {
              cVar2 = local_6;
            }
            if (cVar2 == '\0') goto LAB_00522bb3;
            bVar7 = DAT_007056c8 == iVar4 + 1;
          }
          else {
LAB_00522bb3: ;
            bVar7 = false;
          }
          FUN_004d63d0(0,DAT_00f53f0c + 0x230,iVar6 + 0x53 + DAT_00f53f08,0x1e,0x1e,bVar7,
                       DAT_00e38da8,DAT_00e38da4);
          if ((&DAT_00bcba2f)[iVar5] != '\0') {
            bVar1 = true;
          }
        }
        iVar4 = iVar4 + 3;
        iVar6 = iVar6 + 0x2c;
      } while (local_4 < DAT_00f53dd4);
    }
    FUN_004cd1a0(0,&DAT_005c4ec8,DAT_007056c8 == 0x13,0,0);
    if (bVar1) {
      FUN_004cd1a0(0,&DAT_005c4ee4,DAT_007056c8 == 0x14,0,0);
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
