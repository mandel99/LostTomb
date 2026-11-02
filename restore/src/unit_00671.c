#include "restore_types.h"
#define DAT_00784f0c (*(int32_t *)0x00784f0cu)
#define DAT_00785424 (*(undefined4 *)0x00785424u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define s__s__s_005d9a10 (*(char (*)[6])0x005d9a10u)
extern void FUN_004cc110();
extern undefined4 FUN_00561a36();

undefined * FUN_00447840(void)

{
  undefined4 uVar1;
  
  uVar1 = DAT_00e20768;
  if (DAT_00784f0c != 0) {
    FUN_004cc110(0x98,DAT_00784f0c);
    FUN_00561a36(&DAT_00785424,s__s__s_005d9a10,&DAT_00e38e74,DAT_00e20768);
    DAT_00e20768 = uVar1;
    return &DAT_00785424;
  }
  return &DAT_00e38e74;
}
