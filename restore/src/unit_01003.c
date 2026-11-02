#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799baa (*(undefined4 *)0x00799baau)
#define DAT_00eaaf68 (*(undefined4 *)0x00eaaf68u)
#define DAT_00eab044 (*(undefined4 *)0x00eab044u)
#define DAT_00eab124 (*(undefined4 *)0x00eab124u)
extern int FUN_0046efd0();

void FUN_0046f030(void)

{
  byte bVar1;
  undefined4 *puVar2;
  byte *pbVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  puVar2 = &DAT_00eab124;
  do {
    *puVar2 = 0;
    iVar4 = DAT_0078d790;
    puVar2 = puVar2 + 0x38;
  } while ((int)puVar2 < 0xeadc04);
  iVar6 = 1;
  if (0 < DAT_0078d790) {
    pbVar3 = &DAT_00799baa;
    iVar5 = DAT_0078d790;
    do {
      if (((pbVar3[-0xca] != 0) && (*(short *)(pbVar3 + -0xba) == 0x88)) && (*pbVar3 != 0)) {
        (&DAT_00eaaf68)[(uint)*pbVar3 * 0x38] = 1;
      }
      pbVar3 = pbVar3 + 0x108;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    if (0 < iVar4) {
      pbVar3 = &DAT_00799baa;
      do {
        if ((pbVar3[-0xca] != 0) && (*(short *)(pbVar3 + -0xba) == 0x88)) {
          if (*pbVar3 == 0) {
            bVar1 = FUN_0046efd0(iVar6);
            *pbVar3 = bVar1;
          }
          else {
            iVar4 = (uint)*pbVar3 * 0xe0;
            if (*(int *)(&DAT_00eab044 + iVar4) == 0) {
              *(int *)(&DAT_00eab044 + iVar4) = iVar6;
            }
            else {
              bVar1 = FUN_0046efd0(iVar6);
              *pbVar3 = bVar1;
            }
          }
        }
        iVar6 = iVar6 + 1;
        pbVar3 = pbVar3 + 0x108;
      } while (iVar6 <= DAT_0078d790);
    }
  }
  return;
}
