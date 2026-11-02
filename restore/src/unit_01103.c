#include "restore_types.h"
#define DAT_005cb9d8 (*(undefined4 *)0x005cb9d8u)
#define DAT_005cbcd8 (*(undefined4 *)0x005cbcd8u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9740 (*(undefined *)0x00bc9740u)
#define DAT_00bc9748 (*(undefined *)0x00bc9748u)
#define DAT_00bc9760 (*(undefined *)0x00bc9760u)
#define DAT_00bc9764 (*(undefined *)0x00bc9764u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00f5686c (*(undefined *)0x00f5686cu)
extern void FUN_0047db00();
extern undefined4 FUN_0047e000();
extern void FUN_0047e750();
extern int FUN_00484be0();
extern void FUN_00484f00();
extern void FUN_0051f780();

void FUN_0047b1a0(void)

{
  byte *pbVar1;
  int iVar2;
  int iVar3;
  
  DAT_00bc9768 = DAT_00bca2d8;
  DAT_00bc9760 = 0;
  iVar3 = DAT_00bca2d0;
  if (0 < DAT_00bca2d4) {
    do {
      DAT_00bc9764 = 0;
      if (0 < iVar3) {
        do {
          if ((((&DAT_00af598c)[DAT_00bc9768] & 0x4000) != 0) &&
             (((&DAT_00af598c)[DAT_00bc9768] & 8) == 0)) {
            FUN_0047db00(0x4000);
            FUN_00484f00();
            FUN_00484be0(&DAT_005cb9d8,0x30,0xffffffff,0);
            iVar2 = DAT_00bc9768;
            pbVar1 = &DAT_00b67d1c + DAT_00bc9768;
            (&DAT_00b285cc)[DAT_00bc9768] = (&DAT_00b285cc)[DAT_00bc9768] & 0xf0;
            iVar3 = DAT_00bc9740;
            (&DAT_00b67d1c)[iVar2] = *pbVar1 | 0x40;
            (&DAT_00b7482c)[iVar2] = DAT_00f5686c + iVar3 + DAT_00bc9748;
            iVar3 = FUN_0047e000();
            if (iVar3 != 0) {
              FUN_0047e750();
              iVar3 = FUN_00484be0(&DAT_005cbcd8,0xf,0xffffffff,0);
              if (iVar3 == 0) {
                FUN_0051f780(1);
              }
              else {
                (&DAT_00b7482c)[DAT_00bc9768] = DAT_00f5686c + DAT_00bc9740 + DAT_00bc9748;
              }
            }
            FUN_0051f780(0);
            iVar3 = DAT_00bca2d0;
          }
          DAT_00bc9764 = DAT_00bc9764 + 1;
          DAT_00bc9768 = DAT_00bc9768 + 1;
        } while (DAT_00bc9764 < iVar3);
      }
      DAT_00bc9768 = DAT_00bc9768 + DAT_00bca2dc;
      DAT_00bc9760 = DAT_00bc9760 + 1;
    } while (DAT_00bc9760 < DAT_00bca2d4);
  }
  return;
}
