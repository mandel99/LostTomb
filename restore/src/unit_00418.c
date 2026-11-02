#include "restore_types.h"
#define DAT_00705704 (*(int32_t *)0x00705704u)
#define DAT_00705708 (*(int32_t *)0x00705708u)
#define DAT_00705710 (*(int32_t *)0x00705710u)
#define DAT_00705714 (*(int32_t *)0x00705714u)
#define DAT_00705718 (*(int32_t *)0x00705718u)
#define DAT_00d3b439 (*(undefined1 *)0x00d3b439u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(uint8_t *)0x00e38ddau)
#define _DAT_0070570c (*(undefined4 *)0x0070570cu)

void FUN_004264d0(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  _DAT_0070570c = (uint)DAT_00e38dda;
  DAT_00705718 = DAT_00e38da8 + 0x50;
  DAT_00705714 = DAT_00e38da4 + 0x50;
  DAT_00e38dda = 0x3f;
  DAT_00d3b439 = 0;
  DAT_00705704 = param_2;
  DAT_00705710 = param_1;
  DAT_00705708 = param_3;
  DAT_00e38dca = 1;
  return;
}
