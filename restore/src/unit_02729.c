#include "restore_types.h"
#define DAT_012663b8 (*(undefined4 *)0x012663b8u)
#define PTR_FUN_00604804 (*(unsigned char * *)0x00604804u)
#define PTR_LAB_00604808 (*(unsigned char * *)0x00604808u)
#define PTR_LAB_0060480c (*(unsigned char * *)0x0060480cu)
#define PTR_LAB_00604810 (*(unsigned char * *)0x00604810u)
#define s__CallComponentFunctionWithStorag_0060483c (*(char (*)[34])0x0060483cu)
#define s__CallComponent_0060482c (*(char (*)[15])0x0060482cu)
#define s_theQuickTimeDispatcher_00604814 (*(char (*)[23])0x00604814u)
extern HMODULE FUN_005603f0();

void FUN_00560360(void)

{
  bool bVar1;
  
  bVar1 = false;
  if (DAT_012663b8 == (HMODULE)0x0) {
    DAT_012663b8 = (HMODULE)FUN_005603f0();
    if (DAT_012663b8 == (HMODULE)0x0) {
      return;
    }
    bVar1 = true;
  }
  PTR_LAB_00604810 = GetProcAddress(DAT_012663b8,s__CallComponentFunctionWithStorag_0060483c);
  if ((((FARPROC)PTR_LAB_00604810 != (FARPROC)0x0) &&
      (PTR_LAB_0060480c = GetProcAddress(DAT_012663b8,s__CallComponent_0060482c),
      (FARPROC)PTR_LAB_0060480c != (FARPROC)0x0)) &&
     (PTR_LAB_00604808 = GetProcAddress(DAT_012663b8,s_theQuickTimeDispatcher_00604814),
     (FARPROC)PTR_LAB_00604808 != (FARPROC)0x0)) {
    PTR_FUN_00604804 = (undefined *)0x0;
  }
  if (bVar1) {
    FreeLibrary(DAT_012663b8);
    DAT_012663b8 = (HMODULE)0x0;
  }
  return;
}
