#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_0070573c (*(undefined4 *)0x0070573cu)
#define DAT_007057f4 (*(undefined *)0x007057f4u)
#define DAT_0078d5f0 (*(undefined *)0x0078d5f0u)
#define DAT_00e20774 (*(undefined *)0x00e20774u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea1888 (*(undefined4 *)0x00ea1888u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea41e0 (*(undefined4 *)0x00ea41e0u)
#define DAT_00ea5bb4 (*(undefined4 *)0x00ea5bb4u)
#define DAT_00ea5bb8 (*(undefined4 *)0x00ea5bb8u)
#define DAT_00ea5bbc (*(undefined4 *)0x00ea5bbcu)
#define DAT_00ea5c8c (*(undefined4 *)0x00ea5c8cu)
#define DAT_00ea5fb8 (*(undefined4 *)0x00ea5fb8u)
#define DAT_00ea5fc0 (*(undefined4 *)0x00ea5fc0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern void FUN_004cce90();
extern void FUN_004cd680();
extern int FUN_004cde40();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00527bf0(void)

{
  int iVar1;
  undefined4 uVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int local_4;
  
  DAT_00f53f0c = DAT_00e38da8;
  iVar1 = 5 - DAT_00f53f48;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x16;
  if (iVar1 < 5) {
    if (0 < iVar1) {
      FUN_004cd680(iVar1 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar1 * 8,(iVar1 * -2 + 0x28) * 0x10,
                   (iVar1 * -2 + 0x16) * 0x10,0);
      return;
    }
    FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x16);
    FUN_0053e590(DAT_00f5681c + 6,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
    FUN_004d62f0(0x38,0,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
                 DAT_00e38da4);
    if (*(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8) < 200) {
      iVar1 = 0x10;
    }
    else {
      iVar1 = *(int *)(&DAT_00ea1888 + DAT_00eaac28 * 0x49d8) / 10 + 0x11;
    }
    FUN_004d6160(0x38,iVar1,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x2e,0x200,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x38,3,DAT_00f53f0c + 0xb4,DAT_00f53f08 + 0x5e,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x38,4,DAT_00f53f0c + 0x122,DAT_00f53f08 + 0x5e,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x38,5,DAT_00f53f0c + 0x1de,DAT_00f53f08 + 0x5e,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d65b0(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x6c,0x24,4,DAT_00e38da8,DAT_00e38da4);
    local_4 = 0;
    do {
      if (local_4 == 0) {
        iVar5 = 0xce;
        uVar2 = 0x27;
        iVar1 = 10;
      }
      else if (local_4 == 1) {
        iVar5 = 0x31;
        uVar2 = 0x28;
        iVar1 = 8;
      }
      else {
        iVar5 = 0x2f;
        uVar2 = 0x2a;
        iVar1 = 0xb;
      }
      iVar4 = local_4 * 0x14;
      FUN_004d62f0(0x38,uVar2,DAT_00f53f0c + 0x28,iVar4 + 0x70 + DAT_00f53f08,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      FUN_004cce90((&DAT_0070573c)[iVar5],0x40,&DAT_0057c048,DAT_00f53f0c + 0x96,
                   iVar4 + 0x70 + DAT_00f53f08,100,DAT_00f56f9c,0);
      DAT_00e20778 = 0;
      if (((&DAT_0078d5f0)[iVar1] == 0) || (*(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8) == 0)) {
        FUN_004d6860(0,0x40,&DAT_0057c048,DAT_00f53f0c + 0x118,iVar4 + 0x70 + DAT_00f53f08,
                     DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
        iVar5 = iVar4 + 0x70 + DAT_00f53f08;
        iVar6 = DAT_00e20778 + 0x118 + DAT_00f53f0c;
      }
      else {
        uVar2 = FUN_004cde40(*(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8),(&DAT_0078d5f0)[iVar1],
                             0x40,&DAT_0057c048,DAT_00f53f0c + 0x118,iVar4 + 0x70 + DAT_00f53f08,
                             DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
        FUN_004d6860(uVar2);
        iVar5 = iVar4 + 0x70 + DAT_00f53f08;
        iVar6 = DAT_00e20778 + 0x118 + DAT_00f53f0c;
      }
      FUN_004d62f0(0x38,6,iVar6,iVar5,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
      iVar1 = (&DAT_0078d5f0)[iVar1];
      if (iVar1 == 0) {
        iVar5 = iVar4 + 0x70 + DAT_00f53f08;
        iVar1 = 2;
      }
      else {
        iVar5 = iVar4 + 0x70 + DAT_00f53f08;
        if (iVar1 < 100) {
          iVar1 = iVar1 / 10 + 0x2b;
        }
        else {
          iVar1 = 0x35;
        }
      }
      FUN_004d6250(0x38,iVar1,DAT_00f53f0c + 0x1a4,iVar5,200,DAT_00f56f9c,0,DAT_00e38da8,
                   DAT_00e38da4);
      local_4 = local_4 + 1;
    } while (local_4 < 3);
    DAT_00e20778 = 0;
    DAT_00e20774 = 0;
    FUN_004cce90(DAT_007057f4,0x40,&DAT_0057c048,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc2,5,
                 DAT_00f56f8c,0);
    FUN_004d6160(0x38,0x3b,DAT_00e20778 + 0x39 + DAT_00f53f0c,DAT_00f53f08 + 0xc2,0x200,DAT_00f56f8c
                 ,0,DAT_00e38da8,DAT_00e38da4);
    iVar5 = DAT_00e20774;
    DAT_00e20778 = 0;
    iVar1 = *(int *)(&DAT_00ea5fc0 + DAT_00eaac28 * 0x49d8);
    if (iVar1 == 0) {
      cVar3 = '6';
    }
    else if (iVar1 < 3) {
      cVar3 = '7';
    }
    else if (iVar1 < 6) {
      cVar3 = '8';
    }
    else {
      cVar3 = (8 < iVar1) + '9';
    }
    FUN_004d6160(0x38,cVar3,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc2 + DAT_00e20774,0x200,
                 DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    DAT_00e20774 = iVar5;
    FUN_004d6160(0x38,0x3c,DAT_00e20778 + 0x39 + DAT_00f53f0c,DAT_00f53f08 + 0xc2 + iVar5,0x200,
                 DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004cce90(*(undefined4 *)(&DAT_00ea41e0 + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048,
                 DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xfa,5,DAT_00f56f8c,0);
    FUN_004d6160(0x38,0x3d,DAT_00e20778 + 0x39 + DAT_00f53f0c,DAT_00f53f08 + 0xfa,0x200,DAT_00f56f8c
                 ,0,DAT_00e38da8,DAT_00e38da4);
    switch((&DAT_00ea5c8c)[DAT_00eaac28 * 0x1276]) {
    case 1: ;
      cVar3 = ((&DAT_00ea5bb8)[DAT_00eaac28 * 0x1276] != 0) + '\a';
      break;
    case 2: ;
      cVar3 = ((&DAT_00ea5bb4)[DAT_00eaac28 * 0x1276] != 0) + '\t';
      break;
    case 3: ;
      cVar3 = ((&DAT_00ea5bbc)[DAT_00eaac28 * 0x1276] != 0) + '\v';
      break;
    case 4: ;
      cVar3 = (*(int *)(&DAT_00ea5fb8 + DAT_00eaac28 * 0x49d8) != 0) + '\r';
      break;
default: ;
      cVar3 = '\x0f';
    }
    FUN_004d6160(0x38,cVar3,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x113,0x200,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
  }
  return;
}
