#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5d74 (*(undefined4 *)0x00ea5d74u)
#define DAT_00ea5d78 (*(undefined4 *)0x00ea5d78u)
#define DAT_00ea5d7c (*(undefined4 *)0x00ea5d7cu)
#define DAT_00ea5d80 (*(undefined4 *)0x00ea5d80u)
#define DAT_00ea5d84 (*(undefined4 *)0x00ea5d84u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
extern void FUN_004ccc30();
extern void FUN_004cf090();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d6860();

void FUN_00529530(void)

{
  int iVar1;
  
  FUN_004d63d0(1,DAT_00f53f0c + 0x66,DAT_00f53f08 + 0xd8,0x1ae,0x1a,DAT_007056c8 == 6,DAT_00e38da8,
               DAT_00e38da4);
  DAT_00e20778 = 0;
  FUN_004d62f0(0x3a,0x1f,DAT_00f53f0c + 0x6e,DAT_00f53f08 + 0xe0,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004ccc30(8,0,(&DAT_00ea5d74)[DAT_00eaac28 * 0x1276],DAT_00e20778 + 0x6e + DAT_00f53f0c,
               DAT_00f53f08 + 0xe0,DAT_00f56f8c,0);
  FUN_004d63d0(1,DAT_00f53f0c + 0x66,DAT_00f53f08 + 0xf6,0x1ae,0x1a,DAT_007056c8 == 7,DAT_00e38da8,
               DAT_00e38da4);
  DAT_00e20778 = 0;
  FUN_004d62f0(0x3a,0x20,DAT_00f53f0c + 0x6e,DAT_00f53f08 + 0xfe,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004ccc30(8,0,(&DAT_00ea5d78)[DAT_00eaac28 * 0x1276],DAT_00e20778 + 0x6e + DAT_00f53f0c,
               DAT_00f53f08 + 0xfe,DAT_00f56f8c,0);
  FUN_004d63d0(1,DAT_00f53f0c + 0x66,DAT_00f53f08 + 0x114,0x1ae,0x1a,DAT_007056c8 == 8,DAT_00e38da8,
               DAT_00e38da4);
  DAT_00e20778 = 0;
  FUN_004d62f0(0x3a,0x21,DAT_00f53f0c + 0x6e,DAT_00f53f08 + 0x11c,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  iVar1 = DAT_00e20778;
  FUN_004ccc30(8,0,(&DAT_00ea5d7c)[DAT_00eaac28 * 0x1276],DAT_00e20778 + 0x6e + DAT_00f53f0c,
               DAT_00f53f08 + 0x11c,DAT_00f56f8c,0);
  DAT_00e20778 = DAT_00e20778 + iVar1;
  FUN_004d6860((&DAT_00ea5d80)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,
               DAT_00e20778 + 0x78 + DAT_00f53f0c,DAT_00f53f08 + 0x11c,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x17,0xf,DAT_00e20778 + 0x78 + DAT_00f53f0c,DAT_00f53f08 + 0x11c,DAT_00f56f8c,0,
               DAT_00e38da8,DAT_00e38da4);
  if (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) < -4999) {
    FUN_004cf090(DAT_00f53f0c + 0x68,DAT_00f53f08 + 0xda,0x1aa,0x16,0);
    FUN_004cf090(DAT_00f53f0c + 0x68,DAT_00f53f08 + 0xf8,0x1aa,0x16,0);
    FUN_004cf090(DAT_00f53f0c + 0x68,DAT_00f53f08 + 0x116,0x1aa,0x16,0);
    return;
  }
  if ((&DAT_00ea5d84)[DAT_00eaac28 * 0x1276] != 0) {
    FUN_004cf090(DAT_00f53f0c + 0x68,DAT_00f53f08 + 0x116,0x1aa,0x16,0);
  }
  return;
}
