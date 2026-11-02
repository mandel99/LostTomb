#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a02 (*(undefined4 *)0x00799a02u)
#define DAT_00799a0d (*(undefined4 *)0x00799a0du)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a9a (*(undefined4 *)0x00799a9au)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd65e (*(undefined4 *)0x00bcd65eu)
extern int FUN_00488030();

bool FUN_00434820(int param_1)

{
  short sVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = param_1 * 0x108;
  if (*(char *)((int)&DAT_007999dc + iVar4 + 1) != '\0') {
    return false;
  }
  if (((&DAT_007999e8)[param_1 * 0x84] == 0x5c) || ((&DAT_007999e8)[param_1 * 0x84] == 0xb4)) {
    if ((&DAT_00799a0d)[iVar4] != '\0') goto switchD_004348a0_caseD_21;
  }
  else {
    cVar2 = FUN_00488030(param_1);
    if ((cVar2 == '\0') &&
       ((sVar1 = (&DAT_007999e8)[param_1 * 0x84], sVar1 < 0x6a || (0x6d < sVar1)))) {
      if ((sVar1 < 0x6e) || (0x72 < sVar1)) {
        switch((int)(sVar1)) {
        case 0x20: ;
        case 0x2f: ;
        case 0x33: ;
        case 0x35: ;
        case 0x4a: ;
        case 0x71: ;
        case 0xb1: ;
        case 0xb2: ;
        case 0xbb: ;
        case 0xbc: ;
        case 0xbd: ;
        case 0xc2: ;
        case 0xcb: ;
        case 0xcc: ;
        case 0xcd: ;
        case 0xe2: ;
        case 0xe7: ;
        case 0xe8: ;
        case 0xe9: ;
          break;
default: ;
          goto switchD_004348a0_caseD_21;
        }
      }
      if (((((short)(&DAT_00799a02)[param_1 * 0x84] == 0) ||
           (iVar3 = (short)(&DAT_00799a02)[param_1 * 0x84] * 0x184, (&DAT_00bcd602)[iVar3] != '\x04'
           )) || (((&DAT_00bcd630)[iVar3] != '\b' || ((&DAT_00bcd65e)[iVar3] == '\0')))) &&
         (0 < (short)(&DAT_00799a14)[param_1 * 0x84])) goto switchD_004348a0_caseD_21;
    }
    else if (((short)(&DAT_00799a02)[param_1 * 0x84] == 0) ||
            (((iVar3 = (short)(&DAT_00799a02)[param_1 * 0x84] * 0x184,
              (&DAT_00bcd602)[iVar3] != '\x04' || ((&DAT_00bcd630)[iVar3] != '\b')) ||
             ((&DAT_00bcd65e)[iVar3] == '\0')))) goto switchD_004348a0_caseD_21;
  }
  *(undefined1 *)((int)&DAT_00799a9a + iVar4 + 1) = 1;
switchD_004348a0_caseD_21: ;
  return *(char *)((int)&DAT_00799a9a + iVar4 + 1) != '\0';
}
