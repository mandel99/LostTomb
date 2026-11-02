#include "restore_types.h"
#define DAT_00d3b4bc (*(undefined *)0x00d3b4bcu)
#define DAT_00d3b4c0 (*(undefined *)0x00d3b4c0u)
#define DAT_00d3b4c4 (*(undefined *)0x00d3b4c4u)
#define DAT_00d3b4c8 (*(undefined *)0x00d3b4c8u)
#define DAT_00d3b4cc (*(undefined *)0x00d3b4ccu)
#define DAT_00d3b4d0 (*(undefined *)0x00d3b4d0u)
#define DAT_00d3b4d4 (*(undefined *)0x00d3b4d4u)
#define DAT_00d3b4d8 (*(undefined *)0x00d3b4d8u)
#define DAT_00d3b4dc (*(undefined *)0x00d3b4dcu)
extern void FUN_004cd3e0();
extern void FUN_004cd490();
extern void FUN_004cd4e0();

void FUN_004cd510(int param_1,int param_2,int param_3,int param_4,undefined4 param_5)

{
  int iVar1;
  int iVar2;
  
  FUN_004cd3e0(param_1,param_2,param_3,param_4);
  if (param_1 == param_3) {
    iVar1 = DAT_00d3b4cc;
    if (DAT_00d3b4cc <= DAT_00d3b4c4) {
      do {
        FUN_004cd4e0(param_1,iVar1,param_5);
        iVar1 = iVar1 + 1;
      } while (iVar1 <= DAT_00d3b4c4);
      return;
    }
  }
  else if (param_2 == param_4) {
    iVar1 = DAT_00d3b4c8;
    if (DAT_00d3b4c8 <= DAT_00d3b4c0) {
      do {
        FUN_004cd4e0(iVar1,param_2,param_5);
        iVar1 = iVar1 + 1;
      } while (iVar1 <= DAT_00d3b4c0);
      return;
    }
  }
  else if (DAT_00d3b4d0 < DAT_00d3b4d4) {
    DAT_00d3b4bc = DAT_00d3b4d0 * 2 - DAT_00d3b4d4;
    iVar1 = DAT_00d3b4cc;
    iVar2 = DAT_00d3b4c8;
    if (DAT_00d3b4cc <= DAT_00d3b4c4) {
      do {
        FUN_004cd4e0(iVar2,iVar1,param_5);
        FUN_004cd490(1);
        DAT_00d3b4d4 = DAT_00d3b4d4 + -1;
        if (0 < DAT_00d3b4bc) {
          DAT_00d3b4d0 = DAT_00d3b4d0 + -1;
          iVar2 = iVar2 + DAT_00d3b4d8;
        }
        iVar1 = iVar1 + 1;
      } while (iVar1 <= DAT_00d3b4c4);
      return;
    }
  }
  else {
    DAT_00d3b4bc = DAT_00d3b4d4 * 2 - DAT_00d3b4d0;
    iVar1 = DAT_00d3b4cc;
    iVar2 = DAT_00d3b4c8;
    if (DAT_00d3b4c8 <= DAT_00d3b4c0) {
      do {
        FUN_004cd4e0(iVar2,iVar1,param_5);
        FUN_004cd490(0);
        DAT_00d3b4d0 = DAT_00d3b4d0 + -1;
        if (0 < DAT_00d3b4bc) {
          DAT_00d3b4d4 = DAT_00d3b4d4 + -1;
          iVar1 = iVar1 + DAT_00d3b4dc;
        }
        iVar2 = iVar2 + 1;
      } while (iVar2 <= DAT_00d3b4c0);
    }
  }
  return;
}
