#include "restore_types.h"
#define DAT_005c404a (*(undefined4 *)0x005c404au)
#define DAT_005c412a (*(undefined4 *)0x005c412au)
#define DAT_0077591c (*(undefined *)0x0077591cu)
#define DAT_0077f760 (*(undefined4 *)0x0077f760u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define PTR_DAT_005c3ec2 (*(unsigned char * *)0x005c3ec2u)
extern undefined4 FUN_00443000();
extern undefined4 FUN_00443040();
extern undefined4 FUN_004cf620();

int FUN_004c95d0(undefined1 *param_1)

{
  char cVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  short *psVar7;
  undefined **ppuVar8;
  int local_4;
  
  iVar3 = DAT_0077591c;
  cVar1 = (&DAT_0077f760)[DAT_0077591c * 0x62];
  if (((cVar1 != '\x01') && (cVar1 != '\x03')) && (cVar1 != '\x05')) {
    if (cVar1 == '\0') {
      iVar5 = 1;
      ppuVar8 = &PTR_DAT_005c3ec2;
      do {
        sVar2 = *(short *)((int)ppuVar8 + -2);
        iVar4 = *(short *)ppuVar8 + -0x72 + DAT_00e38dac;
        iVar6 = FUN_00443000(iVar3,iVar5);
        if (iVar6 != 0) {
          iVar4 = FUN_004cf620(sVar2 + 0xdf,iVar4,0x1a,0x1a);
          if (iVar4 != 0) {
            *param_1 = 1;
            return iVar5;
          }
          ppuVar8 = ppuVar8 + 7;
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 < 0x24);
    }
    return 0;
  }
  local_4 = 1;
  psVar7 = &DAT_005c404a;
  do {
    sVar2 = psVar7[-1];
    iVar5 = *psVar7 + -0x72 + DAT_00e38dac;
    iVar4 = FUN_00443000(iVar3,local_4);
    if (iVar4 != 0) {
      iVar5 = FUN_004cf620(sVar2 + 0xdf,iVar5,0x1a,0x1a);
      if (iVar5 != 0) {
        *param_1 = 0;
        return local_4;
      }
      psVar7 = psVar7 + 0xe;
    }
    local_4 = local_4 + 1;
  } while (local_4 < 0x24);
  iVar5 = 1;
  psVar7 = &DAT_005c412a;
  do {
    sVar2 = psVar7[-1];
    iVar4 = *psVar7 + -0x72 + DAT_00e38dac;
    iVar6 = FUN_00443040(iVar3,iVar5);
    if (iVar6 != 0) {
      iVar4 = FUN_004cf620(sVar2 + 0xdf,iVar4,0x1a,0x1a);
      if (iVar4 != 0) {
        *param_1 = 1;
        return iVar5;
      }
      psVar7 = psVar7 + 0xe;
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 0x24);
  return 0;
}
