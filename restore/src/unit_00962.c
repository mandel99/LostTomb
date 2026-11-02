#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799bab (*(undefined4 *)0x00799babu)
#define DAT_009783a4 (*(undefined4 *)0x009783a4u)
#define DAT_009783bc (*(undefined4 *)0x009783bcu)
#define DAT_009783d8 (*(undefined4 *)0x009783d8u)
extern void FUN_0046a280();

void FUN_0046a320(void)

{
  byte bVar1;
  undefined4 *puVar2;
  byte *pbVar3;
  int iVar4;
  
  puVar2 = &DAT_009783d8;
  do {
    *puVar2 = 0;
    puVar2 = puVar2 + 7;
  } while ((int)puVar2 < 0x9784f0);
  if (0 < DAT_0078d790) {
    pbVar3 = &DAT_00799bab;
    iVar4 = DAT_0078d790;
    do {
      if (((pbVar3[-0xcb] != 0) && (*(short *)(pbVar3 + -0xbb) == 0x4a)) && (*pbVar3 != 0)) {
        (&DAT_009783a4)[(uint)*pbVar3 * 0x1c] = 1;
      }
      pbVar3 = pbVar3 + 0x108;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
  }
  iVar4 = 1;
  if (0 < DAT_0078d790) {
    pbVar3 = &DAT_00799bab;
    do {
      if ((pbVar3[-0xcb] != 0) && (*(short *)(pbVar3 + -0xbb) == 0x4a)) {
        bVar1 = *pbVar3;
        if (bVar1 == 0) {
          FUN_0046a280(iVar4);
        }
        else if (*(int *)(&DAT_009783bc + (uint)bVar1 * 0x1c) == 0) {
          *(int *)(&DAT_009783bc + (uint)bVar1 * 0x1c) = iVar4;
        }
        else {
          FUN_0046a280(iVar4);
        }
      }
      iVar4 = iVar4 + 1;
      pbVar3 = pbVar3 + 0x108;
    } while (iVar4 <= DAT_0078d790);
  }
  return;
}
