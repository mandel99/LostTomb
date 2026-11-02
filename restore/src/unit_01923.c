#include "restore_types.h"
#define DAT_005c4810 (*(undefined4 *)0x005c4810u)
#define DAT_005c4830 (*(undefined4 *)0x005c4830u)
#define DAT_005c7e98 (*(undefined4 *)0x005c7e98u)
#define DAT_005c80f0 (*(undefined4 *)0x005c80f0u)
#define DAT_005c8118 (*(undefined4 *)0x005c8118u)
#define DAT_005c8140 (*(undefined4 *)0x005c8140u)
#define DAT_007056c4 (*(undefined *)0x007056c4u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38df8 (*(undefined1 *)0x00e38df8u)
#define DAT_00e950f4 (*(undefined *)0x00e950f4u)
#define DAT_00e950f8 (*(undefined *)0x00e950f8u)
#define DAT_00e950fc (*(undefined *)0x00e950fcu)
#define DAT_00e95100 (*(undefined *)0x00e95100u)
#define DAT_00e95104 (*(undefined *)0x00e95104u)
#define DAT_00e95108 (*(undefined *)0x00e95108u)
#define DAT_00e9510c (*(undefined *)0x00e9510cu)
#define DAT_00e95110 (*(undefined *)0x00e95110u)
extern int FUN_00425990();
extern void FUN_00425bc0();
extern int FUN_004261d0();
extern int FUN_00426450();
extern undefined4 FUN_004e2090();

void FUN_004e1e10(void)

{
  int iVar1;
  
  FUN_00425bc0(DAT_00e95110 + 0x10,DAT_00e95104 * 0x10 + -0x2a + DAT_00e9510c,&DAT_005c7e98,1,10);
  iVar1 = FUN_00425990(DAT_00e95110 + 0x10,DAT_00e95104 * 0x10 + -0x2a + DAT_00e9510c,&DAT_005c7e98,
                       1);
  if (iVar1 == 0) {
    FUN_00425bc0(DAT_00e95108 * 0x10 + -0x26 + DAT_00e95110,
                 DAT_00e95104 * 0x10 + -0x24 + DAT_00e9510c,&DAT_005c80f0,1,0xb);
    iVar1 = FUN_00425990(DAT_00e95108 * 0x10 + -0x26 + DAT_00e95110,
                         DAT_00e95104 * 0x10 + -0x24 + DAT_00e9510c,&DAT_005c80f0,1);
    if (iVar1 == 0) {
      FUN_00425bc0(DAT_00e950f8 * 0x10 + DAT_00e95100,DAT_00e950fc,&DAT_005c8118,1,0xc);
      iVar1 = FUN_00425990(DAT_00e950f8 * 0x10 + DAT_00e95100,DAT_00e950fc,&DAT_005c8118,1);
      if (iVar1 == 0) {
        FUN_00425bc0(DAT_00e950f8 * 0x10 + DAT_00e95100,DAT_00e950f4 * 0x10 + -0x1a + DAT_00e950fc,
                     &DAT_005c8140,1,0xc);
        iVar1 = FUN_00425990(DAT_00e950f8 * 0x10 + DAT_00e95100,
                             DAT_00e950f4 * 0x10 + -0x1a + DAT_00e950fc,&DAT_005c8140,1);
        if (iVar1 == 0) {
          DAT_007056c8 = FUN_00426450(DAT_00e95100,DAT_00e950fc + 4,&DAT_005c4830,10);
          iVar1 = FUN_004261d0(DAT_00e95100,DAT_00e950fc + 4,&DAT_005c4830,10);
          if (iVar1 == 0) {
            DAT_007056c4 = FUN_00426450((DAT_00e95108 * 0x10) / 2 + -0x8c + DAT_00e95110,
                                        DAT_00e95104 * 0x10 + -0x28 + DAT_00e9510c,&DAT_005c4810,1);
            iVar1 = FUN_004261d0((DAT_00e95108 * 0x10) / 2 + -0x8c + DAT_00e95110,
                                 DAT_00e95104 * 0x10 + -0x28 + DAT_00e9510c,&DAT_005c4810,1);
            if (iVar1 == 0) {
              iVar1 = FUN_004e2090();
              if ((iVar1 == 0) && (DAT_00e38df8 != '\0')) {
                DAT_00e38dda = 1;
                DAT_00e38dca = 1;
              }
            }
          }
        }
      }
    }
  }
  return;
}
