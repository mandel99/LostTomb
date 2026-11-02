#include "restore_types.h"
#define DAT_006fdcbc (*(undefined1 *)0x006fdcbcu)
#define DAT_006fdcc4 (*(undefined1 *)0x006fdcc4u)
#define DAT_006fdcc8 (*(undefined1 *)0x006fdcc8u)
#define DAT_006fdcd0 (*(undefined1 *)0x006fdcd0u)
#define DAT_006fdcd4 (*(undefined1 *)0x006fdcd4u)
#define DAT_006fdcdc (*(undefined1 *)0x006fdcdcu)
#define DAT_006fdce0 (*(uint32_t *)0x006fdce0u)
#define DAT_006fdce4 (*(undefined *)0x006fdce4u)
#define DAT_006fdcec (*(char *)0x006fdcecu)
#define DAT_006fdcf0 (*(undefined *)0x006fdcf0u)
#define DAT_006fdcf8 (*(char *)0x006fdcf8u)
#define DAT_006fdcfc (*(undefined *)0x006fdcfcu)
#define DAT_006fdd04 (*(char *)0x006fdd04u)
#define _DAT_006fdcc0 (*(undefined4 *)0x006fdcc0u)
#define _DAT_006fdccc (*(undefined4 *)0x006fdcccu)
#define _DAT_006fdcd8 (*(undefined4 *)0x006fdcd8u)
#define _DAT_006fdce8 (*(undefined4 *)0x006fdce8u)
#define _DAT_006fdcf4 (*(undefined4 *)0x006fdcf4u)
#define _DAT_006fdd00 (*(undefined4 *)0x006fdd00u)

void FUN_004d3c40(void)

{
  DAT_006fdce0 = 0;
  _DAT_006fdce8 = 0xffffffff;
  RESTORE_SLICE1(DAT_006fdcec,0) = 0;
  RESTORE_SLICE1(DAT_006fdce4,0) = 0xff;
  _DAT_006fdcf4 = 0xffffffff;
  RESTORE_SLICE1(DAT_006fdcf8,0) = 0;
  RESTORE_SLICE1(DAT_006fdcf0,0) = 0xff;
  _DAT_006fdd00 = 0xffffffff;
  RESTORE_SLICE1(DAT_006fdd04,0) = 0;
  RESTORE_SLICE1(DAT_006fdcfc,0) = 0xff;
  _DAT_006fdcc0 = 0xffffffff;
  DAT_006fdcc4 = 0;
  RESTORE_SLICE1(DAT_006fdcbc,0) = 0xff;
  _DAT_006fdccc = 0xffffffff;
  DAT_006fdcd0 = 0;
  DAT_006fdcc8 = 0xff;
  _DAT_006fdcd8 = 0xffffffff;
  DAT_006fdcdc = 0;
  DAT_006fdcd4 = 0xff;
  return;
}
