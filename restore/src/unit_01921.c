#include "restore_types.h"
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e950f4 (*(undefined *)0x00e950f4u)
#define DAT_00e950f8 (*(undefined *)0x00e950f8u)
#define DAT_00e950fc (*(undefined *)0x00e950fcu)
#define DAT_00e95100 (*(undefined *)0x00e95100u)
#define DAT_00e95104 (*(undefined *)0x00e95104u)
#define DAT_00e95108 (*(undefined *)0x00e95108u)
#define DAT_00e9510c (*(undefined *)0x00e9510cu)
#define DAT_00e95110 (*(undefined *)0x00e95110u)
#define DAT_00e951ec (*(undefined *)0x00e951ecu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004ccb30();
extern void FUN_004ccbb0();
extern void FUN_004ccfd0();
extern void FUN_004dddc0();
extern void FUN_00536820();
extern void FUN_00536a30();

void FUN_004e1870(void)

{
  DAT_00e9510c = DAT_00e38da4 + 0x20;
  DAT_00e95110 = DAT_00e38da8;
  DAT_00e95100 = DAT_00e38da8 + 0x10;
  DAT_00e950fc = DAT_00e38da4 + 0x66;
  DAT_00e95108 = 0x1e;
  DAT_00e95104 = 0x16;
  DAT_00e950f8 = 0x1a;
  DAT_00e950f4 = 0xd;
  FUN_004dddc0();
  FUN_00536820(DAT_00e95110,DAT_00e9510c,DAT_00e95108,DAT_00e95104);
  FUN_004ccb30(0x3f,0,DAT_00e95110,DAT_00e9510c + 0x10,DAT_00e95108 << 4,DAT_00f56fb4,0xffff);
  FUN_00536a30(DAT_00e95100,DAT_00e950fc,DAT_00e950f8,DAT_00e950f4);
  if (DAT_00e951ec < 1) {
    FUN_004ccfd0(0x3f,1,DAT_00e95100 + 0x10,DAT_00e950fc + 0x50,(DAT_00e950f8 + -3) * 0x10,
                 DAT_00f56f94,0xffff);
    return;
  }
  FUN_004ccbb0(0x3f,2,DAT_00e95100 + 0x2a,DAT_00e950fc + -0x12,DAT_00f56f8c,0);
  FUN_004ccbb0(0x3f,3,DAT_00e95100 + 200,DAT_00e950fc + -0x12,DAT_00f56f8c,0);
  FUN_004ccfd0(0x3f,4,DAT_00e95100 + 0x28,DAT_00e950f4 * 0x10 + 3 + DAT_00e950fc,
               DAT_00e950f8 * 0x10 + -0x32,DAT_00f56f8c,0);
  FUN_004ccb30(0x3f,6,(DAT_00e95108 * 0x10) / 2 + -0x8c + DAT_00e95110,
               DAT_00e95104 * 0x10 + -0x21 + DAT_00e9510c,0x118,DAT_00f56f8c,0);
  return;
}
