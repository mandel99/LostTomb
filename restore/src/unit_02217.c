#include "restore_types.h"
#define DAT_007056c4 (*(undefined *)0x007056c4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00eadcc0 (*(undefined *)0x00eadcc0u)
#define DAT_00eadccc (*(undefined *)0x00eadcccu)
#define DAT_00eadcdc (*(undefined *)0x00eadcdcu)
#define DAT_00eadce4 (*(undefined *)0x00eadce4u)
#define DAT_00eadd24 (*(undefined4 *)0x00eadd24u)
#define DAT_00eadd48 (*(undefined *)0x00eadd48u)
#define DAT_00eadd50 (*(undefined *)0x00eadd50u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f55ba4 (*(undefined4 *)0x00f55ba4u)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
extern void FUN_004d6250();
extern void FUN_004d63d0();
extern void FUN_00501140();
extern void FUN_005011d0();
extern void FUN_00501a40();
extern void FUN_00501b60();
extern void FUN_005026f0();
extern void FUN_00502990();
extern void FUN_00503e10();
extern void FUN_00504630();
extern void FUN_00510b00();

void FUN_004f94a0(void)

{
  short sVar1;
  int iVar2;
  
  if (DAT_00eadd50 != 0) {
    if (DAT_00eadd50 == 1) {
      if (DAT_00eadd48 != 0) {
        switch((&DAT_00bcd602)[(&DAT_00eadd24)[DAT_00eadce4] * 0x184]) {
        case 5: ;
        case 7: ;
        case 10: ;
        case 0xf: ;
        case 0x10: ;
        case 0x11: ;
        case 0x12: ;
        case 0x1b: ;
        case 0x1d: ;
        case 0x1e: ;
        case 0x1f: ;
        case 0x20: ;
        case 0x22: ;
        case 0x23: ;
        case 0x27: ;
        case 0x28: ;
        case 0x57: ;
        case 0x58: ;
        case 0x59: ;
        case 0x69: ;
          iVar2 = 0;
          while ((iVar2 == DAT_00eadcdc || ((byte)(&DAT_00f55ba4)[iVar2] < 2))) {
            iVar2 = iVar2 + 1;
            if (7 < iVar2) {
              return;
            }
          }
          FUN_004d63d0(0,DAT_00eadd90 + 0x11d,DAT_00eadd8c + 0xc3,0x96,0x19,DAT_007056c4 == 1,
                       DAT_00eadd90,DAT_00eadd8c);
          FUN_004d6250(0x12,7,DAT_00eadd90 + 0x11d,DAT_00eadd8c + 200,0x96,DAT_00f56f94,0,
                       DAT_00eadd90,DAT_00eadd8c);
          return;
        }
      }
    }
    else {
      if (DAT_00eadd50 == 4) {
        FUN_00510b00(DAT_00eadccc);
        return;
      }
      if ((DAT_00eadd50 == 2) &&
         ((sVar1 = (&DAT_007999e8)[DAT_00eadd6c * 0x84], sVar1 < 10 || (0x1d < sVar1)))) {
        if ((sVar1 == 0x47) || (sVar1 == 0xbf)) {
          if (DAT_00eadcc0 != 0) {
            FUN_00501b60();
            return;
          }
          FUN_00501a40();
        }
        else {
          if ((sVar1 == 0x48) || (sVar1 == 0xc1)) {
            if (DAT_00eadcc0 != 0) {
              FUN_00502990();
              return;
            }
            FUN_005026f0();
            return;
          }
          if ((sVar1 == 0x46) || (sVar1 == 0xbe)) {
            if (DAT_00eadcc0 != 0) {
              FUN_005011d0();
              return;
            }
            FUN_00501140();
            return;
          }
          if (((sVar1 == 0xbb) || (sVar1 == 0xbc)) || (sVar1 == 0xbd)) {
            FUN_00504630();
            return;
          }
          if ((sVar1 == 0x56) || (sVar1 == 0x57)) {
            FUN_00504630();
            return;
          }
          if ((sVar1 == 0x4d) || (sVar1 == 0x4e)) {
            FUN_00503e10();
            return;
          }
        }
      }
    }
  }
  return;
}
