#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005f89d0 (*(undefined4 *)0x005f89d0u)
#define DAT_005f89d4 (*(undefined4 *)0x005f89d4u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea4464 (*(undefined4 *)0x00ea4464u)
#define DAT_00ea4468 (*(undefined4 *)0x00ea4468u)
#define DAT_00ea446c (*(undefined4 *)0x00ea446cu)
#define DAT_00ea4478 (*(undefined4 *)0x00ea4478u)
#define DAT_00ea447c (*(undefined4 *)0x00ea447cu)
#define DAT_00ea5f10 (*(undefined4 *)0x00ea5f10u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define PTR_DAT_005e03ec (*(unsigned char * *)0x005e03ecu)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern undefined4 FUN_004c38f0();
extern void FUN_004cd680();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00521080(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_004c38f0();
  DAT_00f53f0c = DAT_00e38da8;
  iVar2 = 5 - DAT_00f53f48;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x1a;
  if (iVar2 < 5) {
    if (0 < iVar2) {
      FUN_004cd680(iVar2 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar2 * 8,(iVar2 * -2 + 0x28) * 0x10,
                   (iVar2 * -2 + 0x1a) * 0x10,0);
      return;
    }
    FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x1a);
    FUN_0053e590(DAT_00f5681c,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
    FUN_004d62f0(0x32,0,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x32,0x15,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x38,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x32,0x16,DAT_00f53f0c + 0xaa,DAT_00f53f08 + 0x38,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x32,0x17,DAT_00f53f0c + 400,DAT_00f53f08 + 0x38,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x32,0x18,DAT_00f53f0c + 500,DAT_00f53f08 + 0x38,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d6860((&DAT_00ea4464)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x20,
                 DAT_00f53f08 + 0x140,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x32,0xc,DAT_00e20778 + 0x20 + DAT_00f53f0c,DAT_00f53f08 + 0x140,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea4468)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,
                 DAT_00e20778 + 0x32 + DAT_00f53f0c,DAT_00f53f08 + 0x140,DAT_00f56f8c,0,DAT_00e38da8
                 ,DAT_00e38da4);
    FUN_004d62f0(0x32,0xd,DAT_00e20778 + 0x32 + DAT_00f53f0c,DAT_00f53f08 + 0x140,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea446c)[DAT_00eaac28 * 0x1276],0x40,&DAT_005f89d4,
                 DAT_00e20778 + 0x32 + DAT_00f53f0c,DAT_00f53f08 + 0x140,DAT_00f56f8c,0,DAT_00e38da8
                 ,DAT_00e38da4);
    FUN_004d65b0(DAT_00f53f0c + 0x40,DAT_00f53f08 + 0x15e,0x20,2,DAT_00e38da8,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d62f0(0x32,0xe,DAT_00f53f0c + 0x46,DAT_00f53f08 + 0x167,DAT_00f56f9c,0,DAT_00e38da8,
                 DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d6860((&DAT_00ea4478)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0xe6,
                 DAT_00f53f08 + 0x167,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x32,0xf,DAT_00e20778 + 0xe6 + DAT_00f53f0c,DAT_00f53f08 + 0x167,DAT_00f56f9c,0,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x32,0x12,DAT_00e20778 + 0xe6 + DAT_00f53f0c,DAT_00f53f08 + 0x167,DAT_00f56f9c,0,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea447c)[DAT_00eaac28 * 0x1276] + iVar1,0x40,&DAT_005f89d0,
                 DAT_00e20778 + 0xe6 + DAT_00f53f0c,DAT_00f53f08 + 0x167,DAT_00f56f9c,0,DAT_00e38da8
                 ,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d62f0(0x32,0x13,DAT_00f53f0c + 0x40,DAT_00f53f08 + 0x186,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea5f10)[DAT_00eaac28 * 0x1276],0x40,&PTR_DAT_005e03ec,
                 DAT_00e20778 + 0x40 + DAT_00f53f0c,DAT_00f53f08 + 0x186,DAT_00f56f8c,0,DAT_00e38da8
                 ,DAT_00e38da4);
  }
  return;
}
