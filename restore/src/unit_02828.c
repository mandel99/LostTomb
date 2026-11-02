#include "restore_types.h"
#define DAT_01266434 (*(undefined *)0x01266434u)
#define _DAT_01266430 (*(undefined4 *)0x01266430u)
extern void * FUN_00561b68();
extern undefined4 FUN_005624e0();
extern uint * FUN_00566750();

undefined4 FUN_0056240f(uint param_1,void *param_2,size_t param_3)

{
  int iVar1;
  DWORD DVar2;
  uint uVar3;
  void *pvVar4;
  undefined4 uVar5;
  CHAR local_10c [260];
  LPSTR local_8;
  
  uVar3 = param_1;
  if (param_1 == 0) {
    DVar2 = GetCurrentDirectoryA(0x104,local_10c);
  }
  else {
    iVar1 = FUN_005624e0(param_1);
    if (iVar1 == 0) {
      _DAT_01266430 = 0xd;
      DAT_01266434 = 0xf;
      return 0;
    }
    param_1 = (uint)CONCAT12(0x2e,CONCAT11(0x3a,(char)uVar3 + '@'));
    DVar2 = GetFullPathNameA((LPCSTR)&param_1,0x104,local_10c,&local_8);
  }
  if ((DVar2 != 0) && (uVar3 = DVar2 + 1, uVar3 < 0x105)) {
    if (param_2 == (void *)0x0) {
      if ((int)uVar3 <= (int)param_3) {
        uVar3 = param_3;
      }
      pvVar4 = FUN_00561b68(uVar3);
      if (pvVar4 != (void *)0x0) {
LAB_005624cc: ;
        uVar5 = FUN_00566750(pvVar4,local_10c);
        return uVar5;
      }
      _DAT_01266430 = 0xc;
    }
    else {
      pvVar4 = param_2;
      if ((int)uVar3 <= (int)param_3) goto LAB_005624cc;
      _DAT_01266430 = 0x22;
    }
  }
  return 0;
}
