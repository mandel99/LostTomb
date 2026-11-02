#include "restore_types.h"
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_0078d79c (*(undefined *)0x0078d79cu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a7e (*(undefined4 *)0x00799a7eu)
#define DAT_00799a80 (*(undefined4 *)0x00799a80u)
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
extern int FUN_00488030();

void FUN_004bf920(int param_1)

{
  short sVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  
  iVar4 = (int)(short)(&DAT_00bcd656)[param_1 * 0xc2];
  sVar1 = (&DAT_00bcd652)[param_1 * 0xc2];
  cVar3 = FUN_00488030(iVar4);
  iVar2 = DAT_0078d79c;
  if (cVar3 == '\0') {
    *(undefined1 *)(&DAT_00799a80 + sVar1 * 0x84) = 0x20;
  }
  else {
    (&DAT_00bcd606)[param_1 * 0x184] = 2;
    if (iVar2 == 0) {
      *(undefined1 *)((int)&DAT_00799a7e + sVar1 * 0x108 + 1) = 0x20;
      *(undefined1 *)((int)&DAT_00799a7e + iVar4 * 0x108 + 1) = 0x60;
      *(char *)(&DAT_00799a7e + iVar4 * 0x84) = *(char *)(&DAT_00799a7e + iVar4 * 0x84) + '\x01';
      *(undefined2 *)(&DAT_00799a8c + iVar4 * 0x108) = 0;
      (&DAT_00799a18)[iVar4 * 0x84] =
           *(undefined2 *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[iVar4 * 0x84] * 0x28);
      return;
    }
  }
  return;
}
