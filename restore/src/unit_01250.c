#include "restore_types.h"
#define DAT_00a43eac (*(undefined4 *)0x00a43eacu)
#define DAT_00a509bc (*(undefined4 *)0x00a509bcu)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f56b5c (*(undefined *)0x00f56b5cu)
#define DAT_00f56b94 (*(undefined *)0x00f56b94u)
#define DAT_00f56b9c (*(undefined *)0x00f56b9cu)
#define DAT_00f56c3c (*(undefined *)0x00f56c3cu)
#define DAT_00f5723c (*(undefined *)0x00f5723cu)
#define DAT_00f57244 (*(undefined *)0x00f57244u)
extern char FUN_00477690();
extern uint32_t FUN_0047a1c0(int32_t);
extern undefined4 FUN_00481490();

void FUN_00486600(int param_1)

{
  uint uVar1;
  byte bVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  bool bVar8;
  
  uVar1 = (&DAT_00af598c)[param_1];
  if (((uVar1 & 1) == 0) || ((uVar1 & 0x80000) != 0)) {
    if (((uVar1 & 0x40000) != 0) &&
       (((uVar1 & 0x80000) == 0 && (iVar5 = FUN_00477690(0x40000,param_1), iVar5 == 8)))) {
      cVar3 = FUN_0047a1c0((param_1 - DAT_00bca2d8) % 0xe4);
      if (*(char *)((int)&DAT_00a43eac + param_1) == -1) {
        if (cVar3 == '\0') {
          (&DAT_00b7482c)[param_1] = ((byte)(&DAT_00acf85c)[param_1] & 7) + DAT_00f56c3c;
          return;
        }
        (&DAT_00b7482c)[param_1] = ((byte)(&DAT_00acf85c)[param_1] & 7) + DAT_00f57244;
        return;
      }
      if (cVar3 != '\0') {
        (&DAT_00b7482c)[param_1] = ((byte)(&DAT_00acf85c)[param_1] & 7) + DAT_00f5723c;
        return;
      }
      (&DAT_00b7482c)[param_1] = ((byte)(&DAT_00acf85c)[param_1] & 7) + DAT_00f56b94;
    }
    return;
  }
  bVar2 = (&DAT_00a509bc)[param_1];
  if (100 < bVar2) {
    bVar2 = 100;
  }
  if (*(char *)((int)&DAT_00a43eac + param_1) != -1) {
    (&DAT_00b7482c)[param_1] =
         DAT_00f56b9c + ((byte)(&DAT_00acf85c)[param_1] & 7) + (bVar2 / 0x22) * 8;
    return;
  }
  iVar6 = param_1 - DAT_00bca2d8;
  iVar5 = 3;
  do {
    iVar4 = FUN_00481490(iVar6 % 0xe4,iVar6 / 0xe4,iVar5);
    iVar7 = iVar5;
    if (iVar4 != 0) break;
    iVar7 = iVar5 + -1;
    bVar8 = iVar5 != 0;
    iVar5 = iVar7;
  } while (bVar8);
  if (iVar7 < 2) {
    iVar5 = bVar2 / 0x22 << 4;
  }
  else {
    iVar5 = 0x30;
    iVar7 = iVar7 + -2;
  }
  (&DAT_00b7482c)[param_1] = DAT_00f56b5c + ((byte)(&DAT_00acf85c)[param_1] & 7) + iVar7 * 8 + iVar5
  ;
  return;
}
