#include "restore_types.h"
#define DAT_0060595c (*(undefined4 *)0x0060595cu)
#define DAT_00605960 (*(undefined4 *)0x00605960u)
#define DAT_00605964 (*(undefined4 *)0x00605964u)
#define DAT_00605968 (*(undefined4 *)0x00605968u)
#define DAT_0060596c (*(undefined4 *)0x0060596cu)
#define DAT_00605970 (*(undefined *)0x00605970u)
#define DAT_00605974 (*(undefined *)0x00605974u)
#define DAT_00605978 (*(undefined *)0x00605978u)
#define DAT_0060597c (*(undefined *)0x0060597cu)
#define DAT_00605a4c (*(undefined4 *)0x00605a4cu)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define _DAT_00605980 (*(undefined4 *)0x00605980u)

void FUN_00403790(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  DAT_00605974 = param_1;
  DAT_00605970 = param_2;
  DAT_00605968 = param_7;
  DAT_00605964 = param_4;
  DAT_0060596c = param_3;
  DAT_0060595c = param_6;
  DAT_00605960 = param_5;
  DAT_00605a4c = 0;
  _DAT_00605980 = (uint)DAT_00e38dda;
  DAT_0060597c = 0;
  DAT_00605978 = 0;
  DAT_00e38dda = 0xe;
  return;
}
