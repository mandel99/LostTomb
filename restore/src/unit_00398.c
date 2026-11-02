#include "restore_types.h"
#define DAT_00705710 (*(int32_t *)0x00705710u)
#define DAT_00705714 (*(int32_t *)0x00705714u)
#define DAT_00705718 (*(int32_t *)0x00705718u)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
#define DAT_00f56fb4 (*(int32_t *)0x00f56fb4u)
extern int FUN_004cc6c0();
extern void FUN_004ccb30();
extern void FUN_004ccfd0();
extern void FUN_00536820();

void FUN_00425570(void)

{
  int iVar1;
  
  FUN_00536820(DAT_00705718,DAT_00705714,0x1e,10);
  FUN_004ccb30(5,DAT_00705710,DAT_00705718,DAT_00705714 + 0x14,0x1e0,DAT_00f56fb4,0);
  iVar1 = FUN_004cc6c0(5,DAT_00705710 + 1,DAT_00f56f8c);
  if (iVar1 < 0x1a4) {
    FUN_004ccb30(5,DAT_00705710 + 1,DAT_00705718,DAT_00705714 + 0x3c,0x1e0,DAT_00f56f8c,0);
    return;
  }
  FUN_004ccfd0(5,DAT_00705710 + 1,DAT_00705718 + 0x1e,DAT_00705714 + 0x3c,0x1a4,DAT_00f56f8c,0);
  return;
}
