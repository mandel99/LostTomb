#include "restore_types.h"
#define DAT_00705708 (*(int32_t *)0x00705708u)
#define DAT_00705714 (*(int32_t *)0x00705714u)
#define DAT_00705718 (*(int32_t *)0x00705718u)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
#define PTR_DAT_005c8168 (*(unsigned char * *)0x005c8168u)
extern void FUN_00425730();
extern void FUN_004ccb30();

void FUN_00425640(void)

{
  if (DAT_00705708 == 0) {
    FUN_004ccb30(0xd,1,DAT_00705718,DAT_00705714 + 0x80,0x1e0,DAT_00f56f8c,0);
    return;
  }
  FUN_00425730(DAT_00705718,DAT_00705714,&PTR_DAT_005c8168,DAT_00705708);
  return;
}
