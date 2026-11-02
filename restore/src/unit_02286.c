#include "restore_types.h"
#define DAT_00d3b198 (*(int32_t *)0x00d3b198u)
#define DAT_00d3b1b3 (*(char *)0x00d3b1b3u)
#define DAT_00e20998 (*(int32_t *)0x00e20998u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define DAT_00eadfb0 (*(int32_t *)0x00eadfb0u)
#define DAT_00eadfc8 (*(int32_t *)0x00eadfc8u)
#define DAT_00eadfcc (*(int32_t *)0x00eadfccu)
#define _DAT_00e94dc0 (*(undefined4 *)0x00e94dc0u)
#define s_Save___dat_005f80f0 (*(char (*)[11])0x005f80f0u)
extern void FUN_004cc3c0();
extern void FUN_004cc3e0();
extern void FUN_004cc500();
extern void FUN_00516ab0();
extern void FUN_00516b50();

void FUN_00516bb0(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = (undefined4 *)&DAT_00e38e74;
  for (iVar1 = 8; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  FUN_00516b50(s_Save___dat_005f80f0);
  DAT_00e38dda = 0x2f;
  DAT_00d3b1b3 = 0;
  DAT_00eadfc8 = 0;
  DAT_00d3b198 = DAT_00eadfcc;
  FUN_00516ab0();
  DAT_00e38dca = 1;
  if (DAT_00eadfb0 == 0) {
    puVar2 = (undefined4 *)&DAT_00e38e74;
    for (iVar1 = 8; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar2 = 0;
      puVar2 = puVar2 + 1;
    }
    DAT_00e38dda = 0x36;
    DAT_00e20998 = 0;
    FUN_004cc500(1);
    FUN_004cc3c0();
    FUN_004cc3e0();
    _DAT_00e94dc0 = 0x2f;
    DAT_00e38dca = 1;
    return;
  }
  return;
}
