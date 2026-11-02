#include "restore_types.h"
#define DAT_0056f840 (*(undefined4 *)0x0056f840u)
#define DAT_0056f880 (*(undefined4 *)0x0056f880u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00f57364 (*(undefined *)0x00f57364u)
#define DAT_00f5736c (*(undefined *)0x00f5736cu)
#define PTR_DAT_005f6cd0 (*(unsigned char * *)0x005f6cd0u)
#define PTR_DAT_005f6d58 (*(unsigned char * *)0x005f6d58u)
extern void FUN_0047bf50();
extern char FUN_004f3f70();

void FUN_004f4160(int param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  byte bVar2;
  uint uVar3;
  byte bVar4;
  int iVar5;
  undefined **ppuVar6;
  int *piVar7;
  
  ppuVar6 = &PTR_DAT_005f6d58;
  if (DAT_005e4ef4 != 5) {
    ppuVar6 = &PTR_DAT_005f6cd0;
  }
  iVar5 = param_1 * 0x108;
  switch(*(undefined1 *)((int)&DAT_00799a2c + iVar5 + 1)) {
  case 3: ;
    iVar1 = 0;
    if (0 < (int)ppuVar6[param_3 * 2 + 1]) {
      piVar7 = (int *)ppuVar6[param_3 * 2];
      while ((int)(short)(&DAT_007999ea)[param_1 * 0x84] != *piVar7) {
        iVar1 = iVar1 + 1;
        piVar7 = piVar7 + 2;
        if ((int)ppuVar6[param_3 * 2 + 1] <= iVar1) {
          return;
        }
      }
      iVar5 = *(int *)((int)ppuVar6[param_3 * 2] + (iVar1 * 2 + 1) * 4) +
              *(int *)(&DAT_007999e4 + iVar5);
      (&DAT_00b7482c)[iVar5] = DAT_00f5736c;
      if (((&DAT_00acf85c)[iVar5] & 1) != 0) {
        (&DAT_00b285cc)[iVar5] = (&DAT_00b285cc)[iVar5] | 0x20;
        return;
      }
    }
    break;
  case 4: ;
    iVar1 = *(int *)(&DAT_007999e4 + iVar5);
    bVar2 = 0;
    piVar7 = &DAT_0056f880;
    bVar4 = *(byte *)((int)&DAT_00799a30 + iVar5 + 3);
    do {
      if (((uint)bVar4 & 1 << (bVar2 & 0x1f)) != 0) {
        (&DAT_00af598c)[iVar1 + *piVar7] = (&DAT_00af598c)[iVar1 + *piVar7] & 0xffffff6e;
      }
      piVar7 = piVar7 + 1;
      bVar2 = bVar2 + 1;
    } while ((int)piVar7 < 0x56f890);
    return;
  case 5: ;
  case 6: ;
  case 7: ;
  case 8: ;
  case 9: ;
  case 10: ;
    bVar4 = 0;
    piVar7 = &DAT_0056f880;
    do {
      if ((*(byte *)((int)&DAT_00799a30 + iVar5 + 3) & (byte)(1 << (bVar4 & 0x1f))) == 0) {
        uVar3 = *(byte *)((int)&DAT_00799a2c + iVar5 + 1) - 1;
      }
      else {
        uVar3 = (uint)*(byte *)((int)&DAT_00799a2c + iVar5 + 1);
      }
      FUN_004f3f70(param_1,*piVar7 + *(int *)(&DAT_007999e4 + iVar5),uVar3,1);
      piVar7 = piVar7 + 1;
      bVar4 = bVar4 + 1;
    } while ((int)piVar7 < 0x56f890);
    return;
  case 0xb: ;
    FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar5 + 2),
                 (int)*(short *)(&DAT_007999e0 + iVar5),(&DAT_007999db)[iVar5],
                 (&DAT_007999db)[iVar5],
                 *(int *)(&DAT_0056f840 +
                         ((int)(short)(&DAT_007999ea)[param_1 * 0x84] + (DAT_00bca2e0 / 2) * 4) * 4)
                 + DAT_00f57364,8);
  }
  return;
}
