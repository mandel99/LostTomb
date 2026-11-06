#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00ea5ccc (*(undefined4 *)0x00ea5cccu)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
#define DAT_00f53f08 (*(int32_t *)0x00f53f08u)
#define DAT_00f53f0c (*(int32_t *)0x00f53f0cu)
#define DAT_00f56f8c (*(uint32_t *)0x00f56f8cu)
#define DAT_00f56f9c (*(uint32_t *)0x00f56f9cu)
#define DAT_00f56fb4 (*(uint32_t *)0x00f56fb4u)
#define DAT_00f571fc (*(uint32_t *)0x00f571fcu)
extern void FUN_004cce90();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_005250f0(void)

{
  FUN_00536820(DAT_00f53f0c + 0x40,DAT_00f53f08 + 0xa0,0x20,10);
  FUN_0053e590(DAT_00f571fc + 0x15,DAT_00f53f0c + 0x60,DAT_00f53f08 + 0xb0);
  FUN_004d6250(0x34,0x10,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0xb0,0x1f0,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d65b0(DAT_00f53f0c + 0x70,DAT_00f53f08 + 0xd0,0x1a,4,DAT_00e38da8,DAT_00e38da4);
  FUN_004cce90(0,0x40,&DAT_0057c048,DAT_00f53f0c + 0x7c,DAT_00f53f08 + 0xdd,0x40,DAT_00f56f9c,0);
  FUN_004cce90(500,0x40,&DAT_0057c048,DAT_00f53f0c + 0xcc,DAT_00f53f08 + 0xdd,0x40,DAT_00f56f9c,0);
  FUN_004cce90(2000,0x40,&DAT_0057c048,DAT_00f53f0c + 0x11c,DAT_00f53f08 + 0xdd,0x40,DAT_00f56f9c,0)
  ;
  FUN_004cce90(5000,0x40,&DAT_0057c048,DAT_00f53f0c + 0x16c,DAT_00f53f08 + 0xdd,0x40,DAT_00f56f9c,0)
  ;
  FUN_004d6250(0x34,0x13,DAT_00f53f0c + 0x1bc,DAT_00f53f08 + 0xdd,0x40,DAT_00f56f9c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x34,0x11,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0xf8,DAT_00f56f9c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6860((&DAT_00ea5ccc)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x13c,
               DAT_00f53f08 + 0xf8,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0xd,4,DAT_00f53f0c + 0x150,DAT_00f53f08 + 0x120,0xa0,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x34,0x12,DAT_00f53f0c + 0x90,DAT_00f53f08 + 0x120,0xa0,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  return;
}
