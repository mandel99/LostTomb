#include "restore_types.h"
#define DAT_005e2a64 (*(undefined4 *)0x005e2a64u)
#define DAT_005e2a68 (*(undefined4 *)0x005e2a68u)
#define DAT_005e2a6c (*(undefined4 *)0x005e2a6cu)
#define DAT_005e2a70 (*(undefined4 *)0x005e2a70u)
#define DAT_00784904 (*(undefined *)0x00784904u)
#define DAT_00784908 (*(undefined *)0x00784908u)
#define DAT_0078490c (*(undefined *)0x0078490cu)
#define DAT_00784910 (*(undefined *)0x00784910u)
#define DAT_00784be0 (*(undefined4 *)0x00784be0u)
#define DAT_00784cc0 (*(undefined *)0x00784cc0u)
#define DAT_00784cc4 (*(undefined *)0x00784cc4u)
#define DAT_00784cd0 (*(undefined *)0x00784cd0u)
#define DAT_00784cd4 (*(undefined *)0x00784cd4u)
#define DAT_00784dc4 (*(undefined *)0x00784dc4u)
#define DAT_00784dc8 (*(undefined *)0x00784dc8u)
#define DAT_00784dd0 (*(undefined *)0x00784dd0u)
#define DAT_00784dd4 (*(undefined *)0x00784dd4u)
#define DAT_00bca2bc (*(undefined *)0x00bca2bcu)
#define DAT_00bca2c0 (*(undefined *)0x00bca2c0u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00e94e04 (*(undefined *)0x00e94e04u)
#define _DAT_00784b64 (*(undefined4 *)0x00784b64u)
#define _DAT_00784b68 (*(undefined4 *)0x00784b68u)
#define _DAT_00784b6c (*(undefined4 *)0x00784b6cu)
#define _DAT_00784b70 (*(undefined4 *)0x00784b70u)
#define _DAT_00784b74 (*(undefined4 *)0x00784b74u)
#define _DAT_00784b78 (*(undefined4 *)0x00784b78u)
#define _DAT_00784b7c (*(undefined4 *)0x00784b7cu)
#define _DAT_00784b80 (*(undefined4 *)0x00784b80u)
#define _DAT_00784b84 (*(undefined4 *)0x00784b84u)
#define _DAT_00784b88 (*(undefined4 *)0x00784b88u)
#define _DAT_00784b8c (*(undefined4 *)0x00784b8cu)
#define _DAT_00784b90 (*(undefined4 *)0x00784b90u)
#define _DAT_00784c60 (*(undefined4 *)0x00784c60u)
#define _DAT_00784c62 (*(undefined4 *)0x00784c62u)
#define _DAT_00784c64 (*(undefined4 *)0x00784c64u)
#define _DAT_00784c66 (*(undefined4 *)0x00784c66u)
#define _DAT_00784c68 (*(undefined4 *)0x00784c68u)
#define _DAT_00784c6a (*(undefined4 *)0x00784c6au)
#define _DAT_00784c8c (*(undefined4 *)0x00784c8cu)
#define _DAT_00784c8e (*(undefined4 *)0x00784c8eu)
#define _DAT_00784c90 (*(undefined4 *)0x00784c90u)
#define _DAT_00784c92 (*(undefined4 *)0x00784c92u)
#define _DAT_00784cc8 (*(undefined4 *)0x00784cc8u)
#define _DAT_00784ccc (*(undefined4 *)0x00784cccu)
#define _DAT_00784cd8 (*(undefined4 *)0x00784cd8u)
#define _DAT_00784cdc (*(undefined4 *)0x00784cdcu)
#define _DAT_00784dcc (*(undefined4 *)0x00784dccu)
#define _DAT_00784dd8 (*(undefined4 *)0x00784dd8u)
extern void FUN_00430530();
extern void FUN_0046f7b0();
extern void FUN_0046fa40();
extern void FUN_0046fda0();
extern void FUN_004767d0();
extern void FUN_004ddf90();
extern void FUN_004de080();
extern void FUN_00517370();
extern void FUN_00517660();
extern void FUN_005178e0();
extern void FUN_00517a20();

void FUN_004de430(int param_1)

{
  undefined2 *puVar1;
  
  param_1 = param_1 * 0x10;
  DAT_00784904 = *(int *)(&DAT_005e2a64 + param_1);
  DAT_00784908 = *(int *)(&DAT_005e2a68 + param_1);
  DAT_0078490c = *(undefined4 *)(&DAT_005e2a6c + param_1);
  DAT_00784910 = *(undefined4 *)(&DAT_005e2a70 + param_1);
  _DAT_00784c60 = 0xffff;
  _DAT_00784c62 = 0xffff;
  _DAT_00784c64 = 0xffff;
  _DAT_00784c66 = 0xffff;
  _DAT_00784c68 = 0xffff;
  _DAT_00784c6a = 0xffff;
  _DAT_00784c8c = 0xffff;
  _DAT_00784c8e = 0xffff;
  _DAT_00784c90 = 0xffff;
  _DAT_00784c92 = 0xffff;
  puVar1 = &DAT_00784be0;
  do {
    puVar1[-0x10] = 0xffff;
    *puVar1 = 0xffff;
    puVar1 = puVar1 + 1;
  } while ((int)puVar1 < 0x784c00);
  _DAT_00784b74 = 0xffffffff;
  _DAT_00784b84 = 0xffffffff;
  _DAT_00784b78 = 0xffffffff;
  _DAT_00784b88 = 0xffffffff;
  _DAT_00784b7c = 0xffffffff;
  _DAT_00784b8c = 0xffffffff;
  _DAT_00784b80 = 0xffffffff;
  DAT_00784cc0 = 0xffffffff;
  _DAT_00784b90 = 0xffffffff;
  DAT_00784cc4 = 0xffffffff;
  DAT_00784cd0 = 0xffffffff;
  _DAT_00784cc8 = 0xffffffff;
  DAT_00784cd4 = 0xffffffff;
  _DAT_00784ccc = 0xffffffff;
  _DAT_00784cd8 = 0xffffffff;
  DAT_00784dc4 = 0xffffffff;
  _DAT_00784cdc = 0xffffffff;
  DAT_00784dc8 = 0xffffffff;
  _DAT_00784b6c = 0xffffffff;
  DAT_00784dd0 = 0xffffffff;
  _DAT_00784dcc = 0xffffffff;
  _DAT_00784b70 = 0xffffffff;
  DAT_00784dd4 = 0xffffffff;
  _DAT_00784b64 = 0xffffffff;
  _DAT_00784dd8 = 0xffffffff;
  _DAT_00784b68 = 0xffffffff;
  DAT_00bca2d0 = DAT_00784904;
  DAT_00bca2d4 = DAT_00784908;
  DAT_00bca2d8 = DAT_0078490c;
  DAT_00bca2dc = DAT_00784910;
  FUN_0046f7b0();
  FUN_0046fa40();
  FUN_0046fda0();
  FUN_00430530(DAT_00e94e04,1);
  FUN_004767d0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,0,0);
  FUN_004ddf90();
  FUN_004de080();
  FUN_00517a20();
  FUN_00517370();
  FUN_00517660();
  FUN_005178e0();
  DAT_00bca2bc = 0x6d;
  DAT_00bca2c0 = 0xda;
  DAT_00bca2e0 = 0;
  return;
}
