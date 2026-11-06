#include "restore_types.h"
#define DAT_00705738 (*(undefined *)0x00705738u)
#define DAT_00ea5ca4 (*(undefined4 *)0x00ea5ca4u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)

void FUN_00426b00(void)

{
  (&DAT_00ea5ca4)[DAT_00eaac28 * 0x1276] = DAT_00705738;
  return;
}
