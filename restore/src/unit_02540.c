#include "restore_types.h"
#define DAT_007027ec (*(undefined4 *)0x007027ecu)
#define DAT_00784914 (*(undefined4 *)0x00784914u)
#define DAT_00784954 (*(undefined4 *)0x00784954u)
#define DAT_0078d7d4 (*(undefined *)0x0078d7d4u)
#define DAT_0078d7d8 (*(undefined *)0x0078d7d8u)
#define DAT_0078d7dc (*(undefined *)0x0078d7dcu)
#define DAT_00bca2bc (*(undefined *)0x00bca2bcu)
#define DAT_00bca2c0 (*(undefined *)0x00bca2c0u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00e20990 (*(int32_t *)0x00e20990u)
#define DAT_00e37078 (*(undefined4 *)0x00e37078u)
#define DAT_00e37100 (*(undefined4 *)0x00e37100u)
#define DAT_00e38dd9 (*(undefined1 *)0x00e38dd9u)
#define DAT_00e38e54 (*(undefined *)0x00e38e54u)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define DAT_00e94e04 (*(undefined *)0x00e94e04u)
#define DAT_00f53f78 (*(undefined *)0x00f53f78u)
#define DAT_00f56f84 (*(int32_t *)0x00f56f84u)
#define DAT_00f56f9c (*(int32_t *)0x00f56f9cu)
#define _DAT_00e38e50 (*(undefined4 *)0x00e38e50u)
extern void FUN_00430530();
extern void FUN_00481f20();
extern void FUN_004cc560();
extern void FUN_004cdd60();
extern void FUN_004ddec0();
extern void FUN_004ddf20();
extern void FUN_004ddf90();
extern void FUN_004de080();
extern void FUN_004e2ab0();

void FUN_00540b50(void)

{
  DAT_00e38dd9 = 0;
  _DAT_00e38e50 = 0;
  DAT_00e38e54 = 0;
  DAT_0078d7dc = 0;
  DAT_0078d7d8 = 0;
  DAT_0078d7d4 = 0;
  FUN_00481f20();
  DAT_00e94e04 = 0;
  FUN_00430530(0,1);
  if (DAT_00f53f78 == 1) {
    FUN_004ddec0();
  }
  else {
    FUN_004ddf20();
  }
  DAT_00bca2bc = 0x6d;
  DAT_00bca2c0 = 0xda;
  DAT_00bca2e0 = 0;
  FUN_004ddf90();
  FUN_004de080();
  FUN_004cdd60();
  DAT_00e20990 = 0;
  FUN_004cc560(1,&DAT_00e38e74,0x1f,0x68,1,DAT_00f56f84);
  FUN_004cc560(2,&DAT_00e37100,0x20,0x118,1,DAT_00f56f9c);
  FUN_004cc560(4,&DAT_00e37078,0x20,0x118,1,DAT_00f56f9c);
  FUN_004cc560(5,&DAT_00784914,0x18,0x118,0,DAT_00f56f84);
  FUN_004cc560(6,&DAT_00784954,500,400,0,DAT_00f56f84);
  FUN_004cc560(7,&DAT_007027ec,0x100,0x118,0,DAT_00f56f84);
  FUN_004e2ab0();
  return;
}
