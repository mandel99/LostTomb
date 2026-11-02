#include "restore_types.h"
#define DAT_005763bc (*(undefined4 *)0x005763bcu)
#define DAT_00579280 (*(undefined4 *)0x00579280u)
#define DAT_0057928c (*(undefined4 *)0x0057928cu)
#define DAT_00e38ed8 (*(uint32_t *)0x00e38ed8u)
#define DAT_00e38edc (*(uint32_t *)0x00e38edcu)
#define DAT_00e39089 (*(uint8_t *)0x00e39089u)
#define s_CheckCD_00579284 (*(char (*)[8])0x00579284u)
#define s_Err_Could_not_read_sierra_ini_00579290 (*(char (*)[30])0x00579290u)
#define s_Pharaoh_ini_005764d8 (*(char (*)[12])0x005764d8u)
extern int FUN_004cbdd0();
extern int FUN_004cbe30();
extern void FUN_004ce6e0();
extern undefined4 FUN_00561994();
extern undefined4 * FUN_00561a0a();
extern void * FUN_00561a1d();
extern void FUN_00561a2b();
extern void FUN_00561bdc();
extern int FUN_00561c96();
extern uint FUN_00561ca1();

undefined4 FUN_00414a90(void)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  
  puVar1 = FUN_00561a1d(20000);
  if (puVar1 == (undefined4 *)0x0) {
    return 0;
  }
  puVar4 = puVar1;
  for (iVar3 = 5000; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar4 = 0;
    puVar4 = puVar4 + 1;
  }
  iVar3 = FUN_00561a0a(s_Pharaoh_ini_005764d8,&DAT_005763bc);
  if (iVar3 == 0) {
    FUN_004ce6e0(s_Err_Could_not_read_sierra_ini_00579290,0,0);
    FUN_00561bdc(puVar1);
    return 0;
  }
  FUN_00561ca1(puVar1,20000,1,iVar3);
  FUN_00561994(iVar3);
  iVar3 = FUN_004cbdd0(puVar1,&DAT_0057928c,20000);
  iVar2 = FUN_004cbe30(iVar3 + (int)puVar1,0x3d,20000);
  if (0 < iVar3 + iVar2) {
    DAT_00e38ed8 = FUN_00561c96(iVar3 + iVar2 + (int)puVar1);
  }
  iVar3 = FUN_004cbdd0(puVar1,s_CheckCD_00579284,20000);
  if (0 < iVar3) {
    iVar2 = FUN_004cbe30(iVar3 + (int)puVar1,0x3d,20000);
    if (0 < iVar3 + iVar2) {
      DAT_00e39089 = FUN_00561c96(iVar3 + iVar2 + (int)puVar1);
    }
  }
  iVar3 = FUN_004cbdd0(puVar1,&DAT_00579280,20000);
  if (0 < iVar3) {
    iVar2 = FUN_004cbe30(iVar3 + (int)puVar1,0x3d,20000);
    if (0 < iVar3 + iVar2) {
      DAT_00e38edc = FUN_00561c96(iVar3 + iVar2 + (int)puVar1);
    }
  }
  FUN_00561a2b(puVar1);
  return 1;
}
