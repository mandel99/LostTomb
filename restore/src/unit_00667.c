#include "restore_types.h"
#define DAT_0078d668 (*(undefined *)0x0078d668u)
#define DAT_0078d66c (*(undefined *)0x0078d66cu)
#define DAT_00ea5fdc (*(undefined4 *)0x00ea5fdcu)
#define DAT_00ea5fe0 (*(undefined4 *)0x00ea5fe0u)
#define DAT_00ea5fe4 (*(undefined4 *)0x00ea5fe4u)
#define DAT_00ea5fe8 (*(undefined4 *)0x00ea5fe8u)
#define DAT_00ea5fec (*(undefined4 *)0x00ea5fecu)
#define DAT_00ea5ff0 (*(undefined4 *)0x00ea5ff0u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)

void FUN_004474e0(void)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = DAT_00eaac28 * 0x49d8;
  *(undefined4 *)(&DAT_00ea5fdc + iVar2) = DAT_0078d66c;
  *(undefined4 *)(&DAT_00ea5fe0 + iVar2) = *(undefined4 *)(&DAT_00ea5fe4 + iVar2);
  uVar1 = DAT_0078d668;
  *(undefined4 *)(&DAT_00ea5fe4 + iVar2) = 0;
  *(undefined4 *)(&DAT_00ea5fe8 + iVar2) = uVar1;
  *(undefined4 *)(&DAT_00ea5fec + iVar2) = *(undefined4 *)(&DAT_00ea5ff0 + iVar2);
  *(undefined4 *)(&DAT_00ea5ff0 + iVar2) = 0;
  return;
}
