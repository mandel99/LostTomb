#include "restore_types.h"
#define DAT_0070e430 (*(uint32_t *)0x0070e430u)
#define DAT_0070e434 (*(uint32_t *)0x0070e434u)
#define DAT_0070e438 (*(uint32_t *)0x0070e438u)
#define DAT_0070e43c (*(uint32_t *)0x0070e43cu)
#define DAT_0070e440 (*(uint32_t *)0x0070e440u)
#define DAT_0070e444 (*(uint32_t *)0x0070e444u)
#define DAT_0070e448 (*(uint32_t *)0x0070e448u)
#define DAT_0070e44c (*(uint32_t *)0x0070e44cu)
#define DAT_0070e450 (*(uint32_t *)0x0070e450u)
#define DAT_0070e454 (*(uint32_t *)0x0070e454u)
#define DAT_0070e458 (*(uint32_t *)0x0070e458u)
#define DAT_0070e45c (*(uint32_t *)0x0070e45cu)
#define DAT_0070e460 (*(uint32_t *)0x0070e460u)
#define DAT_0070e464 (*(uint32_t *)0x0070e464u)
#define DAT_0070e474 (*(uint32_t *)0x0070e474u)
#define DAT_0070e478 (*(uint32_t *)0x0070e478u)
#define DAT_0070e48c (*(uint32_t *)0x0070e48cu)
#define DAT_0070e490 (*(uint32_t *)0x0070e490u)
#define _DAT_0070e468 (*(undefined4 *)0x0070e468u)
#define _DAT_0070e46c (*(undefined4 *)0x0070e46cu)
#define _DAT_0070e470 (*(undefined4 *)0x0070e470u)

void FUN_004303a0(int param_1,undefined4 param_2,int param_3,int param_4,int param_5,int param_6,
                 int param_7,int param_8)

{
  DAT_0070e460 = param_6;
  _DAT_0070e46c = param_2;
  DAT_0070e48c = param_4;
  DAT_0070e45c = param_7 / 2;
  DAT_0070e454 = param_7 / 2 + param_7;
  DAT_0070e450 = param_7 * 2;
  DAT_0070e458 = param_7;
  DAT_0070e44c = param_8 / 2;
  DAT_0070e444 = DAT_0070e44c + param_8;
  DAT_0070e440 = param_8 * 2;
  _DAT_0070e468 = param_1 - param_5;
  DAT_0070e438 = (param_7 + 2) * param_5 + -2 + param_3;
  DAT_0070e430 = (param_6 + -1) * DAT_0070e44c + param_4;
  _DAT_0070e470 = param_1;
  DAT_0070e448 = param_8;
  DAT_0070e478 = param_1 / 2;
  DAT_0070e490 = param_3;
  DAT_0070e43c = param_3;
  DAT_0070e464 = param_5;
  DAT_0070e434 = param_4;
  DAT_0070e474 = param_2;
  return;
}
