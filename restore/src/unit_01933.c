#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00bcb9b4 (*(undefined *)0x00bcb9b4u)
#define DAT_00ea5ed8 (*(undefined4 *)0x00ea5ed8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int * FUN_0044bb90();
extern uint FUN_0044d700();
extern void FUN_004e2970();

void FUN_004e2e20(int param_1)

{
  char cVar1;
  int iVar2;
  
  if (DAT_00605a3c != 1) {
    if ((((param_1 < 0x41) || (0x45 < param_1)) && ((param_1 < 0x3c || (0x40 < param_1)))) &&
       ((param_1 < 0x8c || (0x95 < param_1)))) {
      switch((int)(param_1)) {
      case 0x1e: ;
      case 0x1f: ;
      case 0x20: ;
      case 0x21: ;
      case 0x22: ;
      case 0x23: ;
      case 0x24: ;
      case 0x37: ;
      case 0x4d: ;
      case 0x4e: ;
      case 0x4f: ;
      case 0x51: ;
      case 0x56: ;
      case 0x8a: ;
      case 0xa7: ;
      case 0xb8: ;
      case 0xbb: ;
      case 0xbc: ;
      case 0xbd: ;
      case 0xe2: ;
        break;
default: ;
        goto switchD_004e2e67_caseD_25;
      }
    }
    iVar2 = FUN_0044bb90(0,0);
    if (iVar2 != 0) {
      while (cVar1 = FUN_0044d700(), cVar1 == '\0') {
        iVar2 = FUN_0044bb90(iVar2,0);
        if (iVar2 == 0) {
          return;
        }
      }
      if ((DAT_00bcb9b4 == 0) && ((&DAT_00ea5ed8)[DAT_00eaac28 * 0x49d8] == '\0')) {
        FUN_004e2970(0x68,0,0);
      }
    }
  }
switchD_004e2e67_caseD_25: ;
  return;
}
