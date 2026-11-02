#include "restore_types.h"
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd646 (*(undefined4 *)0x00bcd646u)
#define DAT_00bcd648 (*(undefined4 *)0x00bcd648u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
extern void FUN_004b52f0();
extern void FUN_004b5420();

void FUN_004f2410(int param_1)

{
  int iVar1;
  int iVar2;
  
  FUN_004b5420(param_1);
  iVar1 = DAT_00d056f8 * 0x184;
  iVar2 = param_1 * 0x184;
  (&DAT_00bcd60c)[param_1 * 0xc2] = *(undefined2 *)(&DAT_00bcd61c + iVar1);
  (&DAT_00bcd60e)[param_1 * 0xc2] = *(undefined2 *)(&DAT_00bcd61e + iVar1);
  *(undefined4 *)(&DAT_00bcd618 + iVar2) = *(undefined4 *)(&DAT_00bcd620 + iVar1);
  FUN_004b52f0(param_1);
  *(ushort *)(&DAT_00bcd648 + iVar2) =
       (ushort)*(byte *)(&DAT_00799a2e + *(short *)(&DAT_00bcd646 + iVar2) * 0x84) +
       (ushort)*(byte *)((int)&DAT_00799a2e + *(short *)(&DAT_00bcd646 + iVar2) * 0x108 + 1) * 6;
  *(undefined2 *)(&DAT_00bcd62e + iVar2) = 0;
  return;
}
