#include "restore_types.h"
#define DAT_00ea5b6c (*(undefined4 *)0x00ea5b6cu)
#define DAT_00ea5b70 (*(undefined4 *)0x00ea5b70u)
#define DAT_00ea5db8 (*(undefined4 *)0x00ea5db8u)
#define DAT_00ea5dbc (*(undefined4 *)0x00ea5dbcu)
#define DAT_00ea5dc0 (*(undefined4 *)0x00ea5dc0u)
#define DAT_00ea5dc4 (*(undefined4 *)0x00ea5dc4u)
#define DAT_00ea5ec8 (*(undefined4 *)0x00ea5ec8u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)

void FUN_0046e930(void)

{
  int iVar1;
  
  iVar1 = DAT_00eaac28;
  (&DAT_00ea5b6c)[DAT_00eaac28 * 0x1276] = 0;
  (&DAT_00ea5b70)[iVar1 * 0x1276] = 0;
  (&DAT_00ea5ec8)[iVar1 * 0x1276] = 0;
  (&DAT_00ea5db8)[iVar1 * 0x1276] = 0;
  (&DAT_00ea5dbc)[iVar1 * 0x1276] = 0;
  (&DAT_00ea5dc0)[iVar1 * 0x1276] = 0;
  (&DAT_00ea5dc4)[iVar1 * 0x1276] = 0;
  return;
}
