#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005c59e8 (*(undefined4 *)0x005c59e8u)
#define DAT_00705af0 (*(undefined4 *)0x00705af0u)
#define DAT_007848e2 (*(undefined *)0x007848e2u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea5bcc (*(undefined4 *)0x00ea5bccu)
#define DAT_00ea5c00 (*(undefined4 *)0x00ea5c00u)
#define DAT_00ea5c0a (*(undefined4 *)0x00ea5c0au)
#define DAT_00ea5c14 (*(undefined4 *)0x00ea5c14u)
#define DAT_00ea5c50 (*(undefined4 *)0x00ea5c50u)
#define DAT_00ea5c78 (*(undefined4 *)0x00ea5c78u)
#define DAT_00ea5c90 (*(undefined4 *)0x00ea5c90u)
#define DAT_00ea6086 (*(undefined4 *)0x00ea6086u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eadd44 (*(undefined *)0x00eadd44u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f56464 (*(undefined4 *)0x00f56464u)
#define DAT_00f56794 (*(undefined4 *)0x00f56794u)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern undefined4 FUN_004c06e0();
extern void FUN_004cce90();
extern void FUN_004cd680();
extern void FUN_004d6160();
extern void FUN_004d6200();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern bool FUN_004e9f50();
extern void FUN_00528f10();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_005298e0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  short *local_8;
  undefined *local_4;
  
  DAT_00f53f0c = DAT_00e38da8;
  iVar1 = 5 - DAT_00f53f48;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x1b;
  if (4 < iVar1) {
    DAT_00f53f00 = 0x1b;
    _DAT_00f53f04 = 0x28;
    DAT_00f53f08 = DAT_00e38da4;
    DAT_00f53f0c = DAT_00e38da8;
    return;
  }
  if (0 < iVar1) {
    FUN_004cd680(iVar1 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar1 * 8,(iVar1 * -2 + 0x28) * 0x10,
                 (iVar1 * -2 + 0x1b) * 0x10,0);
    return;
  }
  FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x1b);
  FUN_0053e590(DAT_00f5681c + 9,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
  FUN_004d62f0(0x3b,0,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x3b,5,DAT_00f53f0c + 0xb4,DAT_00f53f08 + 0x20,0x41,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x1c,0x96,DAT_00f53f0c + 0x145,DAT_00f53f08 + 0x2e,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x3b,1,DAT_00f53f0c + 0xff,DAT_00f53f08 + 0x2e,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x3b,2,DAT_00f53f0c + 0xb4,DAT_00f53f08 + 0x2e,0x41,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x3b,3,DAT_00f53f0c + 0x1ea,DAT_00f53f08 + 0x2e,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  DAT_00e20778 = 0;
  FUN_004d6250(0x3b,6,DAT_00f53f0c + 0x186,DAT_00f53f08 + 0x12,0x41,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x3b,8,DAT_00f53f0c + 0x186,DAT_00f53f08 + 0x20,0x41,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x3b,7,DAT_00f53f0c + 0x186,DAT_00f53f08 + 0x2e,0x41,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d65b0(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x3c,0x24,0xd,DAT_00e38da8,DAT_00e38da4);
  iVar4 = 0;
  DAT_00eadd44 = 0;
  iVar1 = 0;
  local_8 = &DAT_007848e2;
  local_4 = &DAT_005c59e8;
  do {
    FUN_004d6200(0x9d,iVar4,local_4,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
    if (DAT_00eadd44 == 0) {
      FUN_004d62f0(0xbb,(int)*local_8,DAT_00f53f0c + 100,iVar1 + 0x43 + DAT_00f53f08,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      FUN_004d62f0(0x9e,iVar4,DAT_00f53f0c + 0x28,iVar1 + 0x57 + DAT_00f53f08,DAT_00f56f94,0,
                   DAT_00e38da8,DAT_00e38da4);
      if (*local_8 == 0) {
        FUN_004d6250(0x3b,0x25,DAT_00f53f0c + 0x104,iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,0,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004d6250(0x3b,0x25,DAT_00f53f0c + 0x14a,iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,0,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004d6250(0x3b,0x25,DAT_00f53f0c + 0x186,iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,0,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004d6250(0x3b,0x25,DAT_00f53f0c + 0x1cc,iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,0,
                     DAT_00e38da8,DAT_00e38da4);
      }
      else {
        iVar2 = FUN_004c06e0(iVar4,0,0x40,&DAT_0057c048,DAT_00f53f0c + 0x104,
                             iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,0);
        FUN_004cce90((&DAT_00705af0)[iVar2]);
        iVar2 = FUN_004c06e0(iVar4,2,0x40,&DAT_0057c048,DAT_00f53f0c + 0x14a,
                             iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,0);
        FUN_004cce90((&DAT_00705af0)[iVar2]);
        FUN_004cce90((&DAT_00ea5c50)[DAT_00eaac28 * 0x1276 + iVar4],0x40,&DAT_0057c048,
                     DAT_00f53f0c + 0x186,iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,0);
        DAT_00e20778 = 0;
        FUN_004d62f0(0x3b,(int)*(char *)((int)&DAT_00ea5c0a + iVar4 + DAT_00eaac28 * 0x49d8) / 10 +
                          0x14,DAT_00f53f0c + 0x1c2,iVar1 + 0x42 + DAT_00f53f08,DAT_00f56f9c,0,
                     DAT_00e38da8,DAT_00e38da4);
        iVar2 = 0;
        if (0 < (int)*(char *)((int)&DAT_00ea5c14 + iVar4 + DAT_00eaac28 * 0x49d8) / 10) {
          iVar3 = 0x1c2;
          do {
            FUN_0053e590(DAT_00f56794,DAT_00e20778 + DAT_00f53f0c + iVar3,
                         iVar1 + 0x3e + DAT_00f53f08);
            iVar2 = iVar2 + 1;
            iVar3 = iVar3 + 10;
          } while (iVar2 < (int)*(char *)((int)&DAT_00ea5c14 + iVar4 + DAT_00eaac28 * 0x49d8) / 10);
        }
        iVar2 = 0;
        if (0 < (int)*(char *)((int)&DAT_00ea6086 + iVar4 + DAT_00eaac28 * 0x49d8) / 10) {
          iVar3 = 0x1c2;
          do {
            FUN_0053e590(DAT_00f56464,DAT_00e20778 + iVar3 + DAT_00f53f0c,
                         iVar1 + 0x3e + DAT_00f53f08);
            iVar2 = iVar2 + 1;
            iVar3 = iVar3 + 10;
          } while (iVar2 < (int)*(char *)((int)&DAT_00ea6086 + iVar4 + DAT_00eaac28 * 0x49d8) / 10);
        }
      }
      if (local_8[0x267] == 1) {
        iVar2 = FUN_004c06e0(iVar4,1,0x40,&DAT_0057c048,DAT_00f53f0c + 200,
                             iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,0);
        FUN_004cce90((&DAT_00705af0)[iVar2]);
      }
      else {
        FUN_004d6250(0x3b,0x25,DAT_00f53f0c + 200,iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,0,
                     DAT_00e38da8,DAT_00e38da4);
      }
    }
    else {
      FUN_004cce90((int)*(char *)((int)&DAT_00ea5c14 + iVar4 + DAT_00eaac28 * 0x49d8),0x40,
                   &DAT_0057c048,DAT_00f53f0c + 0xd2,iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,0
                  );
      FUN_004cce90((int)*(char *)((int)&DAT_00ea6086 + iVar4 + DAT_00eaac28 * 0x49d8),0x40,
                   &DAT_0057c048,DAT_00f53f0c + 0xf5,iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,0
                  );
      FUN_004cce90((int)*(char *)((int)&DAT_00ea5c00 + iVar4 + DAT_00eaac28 * 0x49d8),0x40,
                   &DAT_0057c048,DAT_00f53f0c + 0x136,iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,
                   0);
      FUN_004cce90((int)*(char *)((int)&DAT_00ea5c0a + iVar4 + DAT_00eaac28 * 0x49d8),0x40,
                   &DAT_0057c048,DAT_00f53f0c + 0x168,iVar1 + 0x42 + DAT_00f53f08,0x32,DAT_00f56f9c,
                   0);
    }
    iVar4 = iVar4 + 1;
    local_4 = local_4 + 0x1c;
    local_8 = local_8 + 1;
    iVar1 = iVar1 + 0x28;
  } while ((int)local_4 < 0x5c5a74);
  FUN_004e9f50();
  iVar1 = (&DAT_00ea5c78)[DAT_00eaac28 * 0x1276];
  if ((iVar1 < 1) || (*(char *)(iVar1 + 0xea5c13 + DAT_00eaac28 * 0x49d8) < '\x05')) {
    iVar4 = (&DAT_00ea5c90)[DAT_00eaac28 * 0x1276];
    if (iVar4 == 1) {
      iVar4 = ((&DAT_00ea5bcc)[DAT_00eaac28 * 0x1276] != 0) + 9;
      goto LAB_0052a1f8;
    }
    if (iVar4 == 2) {
      iVar4 = 0xb;
      goto LAB_0052a1f8;
    }
    if (iVar4 == 3) {
      iVar4 = 0xc;
      goto LAB_0052a1f8;
    }
    iVar4 = 0xd;
    if ((&DAT_00ea5bcc)[DAT_00eaac28 * 0x1276] == 0) goto LAB_0052a1f8;
  }
  iVar4 = iVar1 + 0xe;
LAB_0052a1f8: ;
  FUN_004d6160(0x3b,iVar4,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x110,0x200,DAT_00f56f8c,0,DAT_00e38da8
               ,DAT_00e38da4);
  FUN_00528f10();
  return;
}
