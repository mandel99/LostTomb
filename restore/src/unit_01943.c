#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00784bbc (*(undefined *)0x00784bbcu)
#define DAT_00e94e20 (*(undefined *)0x00e94e20u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea5e4c (*(undefined4 *)0x00ea5e4cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_00447530();
extern int FUN_004cde60();
extern void FUN_004e2970();

void FUN_004e3330(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  if ((((DAT_00605a3c != 1) && (DAT_00e94e20 == 0)) && (param_1 == 10)) &&
     ((299 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8) && (DAT_00784bbc == 0)))) {
    uVar1 = FUN_00447530((&DAT_00ea5e4c)[DAT_00eaac28 * 0x1276]);
    iVar2 = FUN_004cde60(uVar1);
    if (iVar2 < 0x60) {
      FUN_004e2970(0x5e,0,0);
    }
  }
  return;
}
