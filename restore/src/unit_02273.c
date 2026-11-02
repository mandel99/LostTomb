#include "restore_types.h"
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5d74 (*(undefined4 *)0x00ea5d74u)
#define DAT_00ea5d78 (*(undefined4 *)0x00ea5d78u)
#define DAT_00ea5d7c (*(undefined4 *)0x00ea5d7cu)
#define DAT_00ea5d80 (*(undefined4 *)0x00ea5d80u)
#define DAT_00ea5d84 (*(undefined4 *)0x00ea5d84u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
extern void FUN_004ccc30();
extern void FUN_004cf090();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern int FUN_0053e590();

void FUN_00516210(void)

{
  int iVar1;
  
  FUN_004d63d0(1,0x84,0xf6,0x1ae,0x1a,DAT_007056c8 == 6,DAT_00eadd90,DAT_00eadd8c);
  DAT_00e20778 = 0;
  FUN_004d62f0(0x3a,0x1f,0x8c,0xfe,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  FUN_004ccc30(8,0,(&DAT_00ea5d74)[DAT_00eaac28 * 0x1276],DAT_00e20778 + 0x8c,0xfe,DAT_00f56f8c,0);
  FUN_004d63d0(1,0x84,0x114,0x1ae,0x1a,DAT_007056c8 == 7,DAT_00eadd90,DAT_00eadd8c);
  DAT_00e20778 = 0;
  FUN_004d62f0(0x3a,0x20,0x8c,0x11c,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  FUN_004ccc30(8,0,(&DAT_00ea5d78)[DAT_00eaac28 * 0x1276],DAT_00e20778 + 0x8c,0x11c,DAT_00f56f8c,0);
  FUN_004d63d0(1,0x84,0x132,0x1ae,0x1a,DAT_007056c8 == 8,DAT_00eadd90,DAT_00eadd8c);
  DAT_00e20778 = 0;
  FUN_004d62f0(0x3a,0x21,0x8c,0x13a,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  iVar1 = DAT_00e20778;
  FUN_004ccc30(8,0,(&DAT_00ea5d7c)[DAT_00eaac28 * 0x1276],DAT_00e20778 + 0x8c,0x13a,DAT_00f56f8c,0);
  iVar1 = DAT_00e20778 + iVar1;
  DAT_00e20778 = iVar1;
  FUN_004ccc30(8,10,(&DAT_00ea5d80)[DAT_00eaac28 * 0x1276],iVar1 + 0x96,0x13a,DAT_00f56f8c,0);
  DAT_00e20778 = DAT_00e20778 + iVar1;
  FUN_0053e590(DAT_00f571fc + 0xf,DAT_00e20778 + 0x96,0x135);
  if (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) < -4999) {
    FUN_004cf090(0x86,0xf8,0x1aa,0x16,0);
    FUN_004cf090(0x86,0x116,0x1aa,0x16,0);
    FUN_004cf090(0x86,0x134,0x1aa,0x16,0);
    return;
  }
  if (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) < (int)(&DAT_00ea5d74)[DAT_00eaac28 * 0x1276])
  {
    FUN_004cf090(0x86,0xf8,0x1aa,0x16,0);
  }
  if (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) < (int)(&DAT_00ea5d78)[DAT_00eaac28 * 0x1276])
  {
    FUN_004cf090(0x86,0x116,0x1aa,0x16,0);
  }
  if ((*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) < (int)(&DAT_00ea5d7c)[DAT_00eaac28 * 0x1276]
      ) || ((&DAT_00ea5d84)[DAT_00eaac28 * 0x1276] != 0)) {
    FUN_004cf090(0x86,0x134,0x1aa,0x16,0);
  }
  return;
}
