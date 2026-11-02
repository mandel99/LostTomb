#include "restore_types.h"
#define DAT_00e945d0 (*(int32_t *)0x00e945d0u)
#define DAT_00e945d4 (*(int32_t *)0x00e945d4u)
#define DAT_00e945d8 (*(int32_t *)0x00e945d8u)
#define DAT_00e945dc (*(int32_t *)0x00e945dcu)
#define DAT_00e945e0 (*(int32_t *)0x00e945e0u)
#define DAT_00e945e4 (*(int32_t *)0x00e945e4u)
extern int FUN_004d5f80();

void FUN_004d5dd0(void)

{
  int iVar1;
  
  iVar1 = FUN_004d5f80(1);
  DAT_00e945d0 = ((int)(iVar1 + (iVar1 >> 0x1f & 0xfU)) >> 4) + 2;
  iVar1 = FUN_004d5f80(2);
  DAT_00e945d4 = ((int)(iVar1 + (iVar1 >> 0x1f & 0xfU)) >> 4) + 2;
  iVar1 = FUN_004d5f80(3);
  DAT_00e945d8 = ((int)(iVar1 + (iVar1 >> 0x1f & 0xfU)) >> 4) + 2;
  iVar1 = FUN_004d5f80(4);
  DAT_00e945dc = ((int)(iVar1 + (iVar1 >> 0x1f & 0xfU)) >> 4) + 2;
  iVar1 = FUN_004d5f80(7);
  DAT_00e945e0 = ((int)(iVar1 + (iVar1 >> 0x1f & 0xfU)) >> 4) + 2;
  iVar1 = FUN_004d5f80(10);
  DAT_00e945e4 = ((int)(iVar1 + (iVar1 >> 0x1f & 0xfU)) >> 4) + 2;
  return;
}
