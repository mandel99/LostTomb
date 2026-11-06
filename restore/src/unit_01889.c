#include "restore_types.h"
#define DAT_005e2950 (*(undefined4 *)0x005e2950u)
#define DAT_005e2954 (*(undefined4 *)0x005e2954u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00bca2d0 (*(int32_t *)0x00bca2d0u)
#define DAT_00bca2d4 (*(int32_t *)0x00bca2d4u)
#define DAT_00bca2d8 (*(uint32_t *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00e94dd4 (*(undefined4 *)0x00e94dd4u)
#define DAT_00e94dd8 (*(undefined4 *)0x00e94dd8u)

void FUN_004de200(int param_1)

{
  char cVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  iVar2 = DAT_00bca2d0;
  if (0 < DAT_00bca2d4) {
    iVar6 = DAT_00e94dd4 + *(int *)(&DAT_005e2954 + param_1 * 8) * 1000 + DAT_00e94dd8 +
            *(int *)(&DAT_005e2950 + param_1 * 8);
    iVar5 = DAT_00bca2d4;
    iVar7 = DAT_00bca2d8;
    do {
      iVar4 = 0;
      if (0 < iVar2) {
        puVar3 = &DAT_00af598c + iVar7;
        do {
          cVar1 = *(char *)(iVar6 + iVar4);
          if (cVar1 == '\x10') {
            *puVar3 = 2;
          }
          else if (cVar1 == '\v') {
            *puVar3 = 1;
          }
          else if (cVar1 == '\x04') {
            *puVar3 = 4;
          }
          else if (cVar1 == '\b') {
            *puVar3 = 0x40;
          }
          else if (cVar1 == '\x03') {
            (&DAT_00b285cc)[iVar7] = (&DAT_00b285cc)[iVar7] | 0x20;
          }
          iVar4 = iVar4 + 1;
          iVar7 = iVar7 + 1;
          puVar3 = puVar3 + 1;
        } while (iVar4 < iVar2);
      }
      iVar6 = iVar6 + 1000;
      iVar7 = iVar7 + DAT_00bca2dc;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
  }
  return;
}
