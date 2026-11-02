#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999f0 (*(undefined4 *)0x007999f0u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a1a (*(undefined4 *)0x00799a1au)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea43a0 (*(undefined4 *)0x00ea43a0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f5684c (*(undefined *)0x00f5684cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
extern void FUN_004ccc30();
extern void FUN_004d6160();
extern void FUN_004d62f0();
extern void FUN_004d6860();
extern int FUN_0053e590();

void FUN_004fc5c0(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  char cVar3;
  
  uVar2 = DAT_00f56f94;
  if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] <
      *(int *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] * 0x28)) {
    if (*(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8) < 1) {
      cVar3 = '\x10';
      goto LAB_004fc609;
    }
    if ((short)(&DAT_007999f0)[DAT_00eadd6c * 0x84] < 1) {
      cVar3 = '\x11';
      goto LAB_004fc609;
    }
    if (0x27 < (short)(&DAT_007999f0)[DAT_00eadd6c * 0x84]) {
      cVar3 = (0 < *(int *)(&DAT_00ea43a0 +
                           (uint)(byte)(&DAT_00799a1a)[DAT_00eadd6c * 0x108] * 0x14 +
                           DAT_00eaac28 * 0x49d8)) + '\x12';
      goto LAB_004fc609;
    }
  }
  else {
    cVar3 = '\0';
    if (0x27 < (short)(&DAT_007999f0)[DAT_00eadd6c * 0x84]) goto LAB_004fc609;
  }
  cVar3 = '\x14';
LAB_004fc609: ;
  FUN_0053e590(DAT_00f5684c + 0xe,DAT_00eadd90 + 0x28,param_1 + 6);
  DAT_00e20778 = 0;
  if (cVar3 != '\0') {
    iVar1 = param_1 + 10;
    FUN_004ccc30(8,0xc,(int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84],DAT_00eadd90 + 0x3c,iVar1,
                 uVar2,0);
    FUN_004d6860(*(undefined4 *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] * 0x28)
                 ,0x28,&DAT_0057c048,DAT_00e20778 + 0x46 + DAT_00eadd90,iVar1,uVar2,0,DAT_00eadd90,
                 DAT_00eadd8c);
    FUN_004d62f0(0x45,0,DAT_00e20778 + 0x46 + DAT_00eadd90,iVar1,uVar2,0,DAT_00eadd90,DAT_00eadd8c);
    DAT_00e20778 = 0;
    FUN_004d6160(0x45,cVar3,DAT_00eadd90 + 0x46,param_1 + 0x1a,400,uVar2,0,DAT_00eadd90,DAT_00eadd8c
                );
    return;
  }
  param_1 = param_1 + 0x10;
  FUN_004ccc30(8,0xc,(int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84],DAT_00eadd90 + 0x3c,param_1,
               uVar2,0);
  FUN_004d6860(*(undefined4 *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] * 0x28),
               0x28,&DAT_0057c048,DAT_00e20778 + 0x46 + DAT_00eadd90,param_1,uVar2,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d62f0(0x45,0,DAT_00e20778 + 0x46 + DAT_00eadd90,param_1,uVar2,0,DAT_00eadd90,DAT_00eadd8c);
  return;
}
