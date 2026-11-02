#include "restore_types.h"
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00e94e04 (*(undefined *)0x00e94e04u)
#define DAT_00ebab20 (*(undefined4 *)0x00ebab20u)
extern void FUN_0040f8f0();

void FUN_005178e0(void)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint *puVar4;
  int iVar5;
  int iVar6;
  char cVar7;
  int local_4;
  
  iVar6 = DAT_00bca2d8;
  FUN_0040f8f0(&DAT_00ebab20,0xcb10,0xffffffff);
  iVar3 = DAT_00e94e04;
  iVar2 = DAT_00bca2d0;
  if (0 < DAT_00bca2d4) {
    local_4 = DAT_00bca2d4;
    do {
      if (0 < iVar2) {
        puVar4 = (uint *)(&DAT_00af5988 + iVar6 * 4);
        iVar5 = iVar2;
        do {
          if ((puVar4[1] & 0x4000) == 0) {
            if ((puVar4[1] & 0x8000) == 0) goto LAB_005179e5;
            (&DAT_00ebab20)[iVar6] = 0;
          }
          else {
            if (iVar3 == 0) {
              cVar7 = (puVar4[0xe5] & 0xc000) != 0;
              if ((puVar4[0xe6] & 0xc000) != 0) {
                cVar7 = cVar7 + '\x01';
              }
              uVar1 = puVar4[2];
            }
            else if (iVar3 == 2) {
              cVar7 = (puVar4[0xe5] & 0xc000) != 0;
              if ((puVar4[0xe4] & 0xc000) != 0) {
                cVar7 = cVar7 + '\x01';
              }
              uVar1 = *puVar4;
            }
            else if (iVar3 == 4) {
              cVar7 = (puVar4[-0xe3] & 0xc000) != 0;
              if ((puVar4[-0xe4] & 0xc000) != 0) {
                cVar7 = cVar7 + '\x01';
              }
              uVar1 = *puVar4;
            }
            else {
              if (iVar3 != 6) goto LAB_005179e5;
              cVar7 = (puVar4[-0xe3] & 0xc000) != 0;
              if ((puVar4[-0xe2] & 0xc000) != 0) {
                cVar7 = cVar7 + '\x01';
              }
              uVar1 = puVar4[2];
            }
            if ((uVar1 & 0xc000) != 0) {
              cVar7 = cVar7 + '\x01';
            }
            if (cVar7 == '\x03') {
              (&DAT_00ebab20)[iVar6] = 0;
            }
            else {
LAB_005179e5: ;
              (&DAT_00ebab20)[iVar6] = 0xff;
            }
          }
          iVar6 = iVar6 + 1;
          puVar4 = puVar4 + 1;
          iVar5 = iVar5 + -1;
        } while (iVar5 != 0);
      }
      iVar6 = iVar6 + DAT_00bca2dc;
      local_4 = local_4 + -1;
    } while (local_4 != 0);
  }
  return;
}
