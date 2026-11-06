#include "restore_types.h"
#define DAT_00785d70 (*(int32_t *)0x00785d70u)
#define DAT_00785d74 (*(uint16_t *)0x00785d74u)
#define DAT_00ea61fc (*(uint8_t *)0x00ea61fcu)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
extern int FUN_004b8360();
extern void FUN_004b8890();
extern int FUN_004cde60();

undefined4 FUN_00446260(int param_1)

{
  int iVar1;
  int iVar2;
  undefined2 *puVar3;
  
  if ((param_1 < 0) || (DAT_00785d70 <= param_1)) {
    puVar3 = (undefined2 *)0x0;
  }
  else {
    puVar3 = &DAT_00785d74 + (uint32_t)param_1 * 0x3eu;
  }
  iVar1 = FUN_004b8360(param_1,0,0);
  iVar2 = 1;
  if (puVar3 != (undefined2 *)0x0) {
    iVar2 = (int)(short)puVar3[6];
  }
  if ((iVar1 < iVar2) && (*(int *)(&DAT_00ea61fc + DAT_00eaac28 * 0x49d8) == 0)) {
    FUN_004b8890(100,param_1);
    return 0;
  }
  if (*(int *)(&DAT_00ea61fc + DAT_00eaac28 * 0x49d8) != 0) {
    *(undefined4 *)(&DAT_00ea61fc + DAT_00eaac28 * 0x49d8) = 0;
    FUN_004b8890(0,param_1);
    return 1;
  }
  iVar1 = FUN_004cde60(iVar1 - iVar2,iVar1);
  if (iVar1 < 10) {
    FUN_004b8890(0x46,param_1);
    return 1;
  }
  if (iVar1 < 0x19) {
    FUN_004b8890(0x32,param_1);
    return 1;
  }
  if (iVar1 < 0x32) {
    FUN_004b8890(0x19,param_1);
    return 1;
  }
  if (iVar1 < 0x4b) {
    FUN_004b8890(0xf,param_1);
    return 1;
  }
  if (iVar1 < 100) {
    FUN_004b8890(10,param_1);
    return 1;
  }
  FUN_004b8890((0x95 < iVar1) - 1U & 5,param_1);
  return 1;
}
