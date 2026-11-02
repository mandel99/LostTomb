#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_00705808 (*(undefined *)0x00705808u)
#define DAT_00705810 (*(undefined *)0x00705810u)
#define DAT_0078d604 (*(undefined *)0x0078d604u)
#define DAT_0078d608 (*(undefined *)0x0078d608u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea5b88 (*(undefined4 *)0x00ea5b88u)
#define DAT_00ea5b8c (*(undefined4 *)0x00ea5b8cu)
#define DAT_00ea5c94 (*(undefined4 *)0x00ea5c94u)
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
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00528360(void)

{
  byte bVar1;
  int iVar2;
  char cVar3;
  
  iVar2 = 5 - DAT_00f53f48;
  DAT_00f53f0c = DAT_00e38da8;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x10;
  if (4 < iVar2) {
    DAT_00f53f00 = 0x10;
    _DAT_00f53f04 = 0x28;
    DAT_00f53f08 = DAT_00e38da4;
    DAT_00f53f0c = DAT_00e38da8;
    return;
  }
  if (0 < iVar2) {
    FUN_004cd680(iVar2 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar2 * 8,(iVar2 * -2 + 0x28) * 0x10,
                 (iVar2 * -2 + 0x10) * 0x10,0);
    return;
  }
  FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x10);
  FUN_0053e590(DAT_00f5681c + 7,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
  FUN_004d62f0(0x39,0,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  DAT_00e20778 = 0;
  FUN_004d62f0(0x39,1,DAT_00f53f0c + 0xb4,DAT_00f53f08 + 0x56,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x39,2,DAT_00f53f0c + 0x122,DAT_00f53f08 + 0x56,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x39,3,DAT_00f53f0c + 0x1de,DAT_00f53f08 + 0x56,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d65b0(DAT_00f53f0c + 0x20,DAT_00f53f08 + 100,0x24,3,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x39,0x1c,DAT_00f53f0c + 0x28,DAT_00f53f08 + 0x69,DAT_00f56f9c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004cce90(DAT_00705808,0x40,&DAT_0057c048,DAT_00f53f0c + 0x96,DAT_00f53f08 + 0x69,100,
               DAT_00f56f9c,0);
  DAT_00e20778 = 0;
  FUN_004d6860(DAT_00705808 * 300,0x40,&DAT_0057c048,DAT_00f53f0c + 0x118,DAT_00f53f08 + 0x69,
               DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x39,4,DAT_00e20778 + 0x118 + DAT_00f53f0c,DAT_00f53f08 + 0x69,DAT_00f56f9c,0,
               DAT_00e38da8,DAT_00e38da4);
  if (DAT_0078d604 == 0) {
    iVar2 = 7;
  }
  else if (DAT_0078d604 < 100) {
    iVar2 = DAT_0078d604 / 10 + 8;
  }
  else {
    iVar2 = 0x12;
  }
  FUN_004d6250(0x39,iVar2,DAT_00f53f0c + 0x1a4,DAT_00f53f08 + 0x69,200,DAT_00f56f9c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x39,0x1d,DAT_00f53f0c + 0x28,DAT_00f53f08 + 0x7d,DAT_00f56f9c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004cce90(DAT_00705810,0x40,&DAT_0057c048,DAT_00f53f0c + 0x96,DAT_00f53f08 + 0x7d,100,
               DAT_00f56f9c,0);
  DAT_00e20778 = 0;
  FUN_004d6860(DAT_00705810 * 800,0x40,&DAT_0057c048,DAT_00f53f0c + 0x118,DAT_00f53f08 + 0x7d,
               DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x39,6,DAT_00e20778 + 0x118 + DAT_00f53f0c,DAT_00f53f08 + 0x7d,DAT_00f56f9c,0,
               DAT_00e38da8,DAT_00e38da4);
  if (DAT_0078d608 == 0) {
    iVar2 = 7;
  }
  else if (DAT_0078d608 < 100) {
    iVar2 = DAT_0078d608 / 10 + 8;
  }
  else {
    iVar2 = 0x12;
  }
  FUN_004d6250(0x39,iVar2,DAT_00f53f0c + 0x1a4,DAT_00f53f08 + 0x7d,200,DAT_00f56f9c,0,DAT_00e38da8,
               DAT_00e38da4);
  iVar2 = (&DAT_00ea5c94)[DAT_00eaac28 * 0x1276];
  if (iVar2 == 1) {
    cVar3 = ((&DAT_00ea5b88)[DAT_00eaac28 * 0x1276] != 0) + '\x13';
  }
  else if (iVar2 == 2) {
    cVar3 = ((&DAT_00ea5b8c)[DAT_00eaac28 * 0x1276] != 0) + '\x15';
  }
  else if (iVar2 == 3) {
    cVar3 = '\x17';
  }
  else if ((&DAT_00ea5b88)[DAT_00eaac28 * 0x1276] == 0) {
    cVar3 = '\x18';
  }
  else {
    if ((&DAT_00ea5b8c)[DAT_00eaac28 * 0x1276] == 0) {
      bVar1 = DAT_0078d604 < 100;
    }
    else {
      if (DAT_0078d604 <= DAT_0078d608) {
        cVar3 = '\x1a';
        goto LAB_00528904;
      }
      bVar1 = (DAT_0078d604 < DAT_0078d608) - 1U & 2;
    }
    cVar3 = bVar1 + 0x19;
  }
LAB_00528904: ;
  FUN_004d6160(0x39,cVar3,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xb4,0x200,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  return;
}
