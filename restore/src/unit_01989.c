#include "restore_types.h"
#include "sprite_frame_clock.h"
#define DAT_00d3b530 (*(undefined1 *)0x00d3b530u)
#define DAT_00d3b531 (*(undefined1 *)0x00d3b531u)
#define DAT_00d3b5bc (*(undefined *)0x00d3b5bcu)
#define DAT_00d3b5be (*(undefined *)0x00d3b5beu)
#define DAT_00d3b5dc (*(undefined *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(undefined *)0x00d3b5e0u)
#define DAT_00ea136c (*(undefined *)0x00ea136cu)
#define DAT_00ea1370 (*(undefined *)0x00ea1370u)
extern void FUN_00410230();
extern undefined4 FUN_004105c0();
extern undefined4 FUN_00410660();
extern void FUN_004cdad0();
extern void FUN_004cdb80();
extern int FUN_0055d6d0();
extern longlong FUN_0056133c();

void FUN_004e61f0(void)

{
  short sVar2;
  int iVar3;
  
  restore_sprite_frame_clock_update();
  if (DAT_00ea136c != (int *)0x0) {
    FUN_0055d6d0();
    (**(code **)(*(undefined4 *)(DAT_00ea136c) + 0x18))(0x15e,300);
  }
  if (((DAT_00ea1370 != 0) && (*(int *)(DAT_00ea1370 + 0x38) != 0)) &&
     (*(int *)(*(int *)(DAT_00ea1370 + 0x38) + 8) != 0)) {
    FUN_0055d6d0();
    iVar3 = DAT_00ea1370;
    sVar2 = FUN_0056133c();
    iVar3 = sVar2 * 0x10 + *(int *)(*(int *)(iVar3 + 0x44) + 4);
    if (*(char *)(iVar3 + 1) == '\0') {
      sVar2 = *(short *)(iVar3 + 8);
    }
    else {
      sVar2 = *(short *)(*(int *)(iVar3 + 8) + 2);
    }
    DAT_00d3b5be = *(undefined2 *)(iVar3 + 2);
    DAT_00d3b5bc = *(undefined2 *)(iVar3 + 4);
    DAT_00d3b5e0 = sVar2 + 0x1c2;
    DAT_00d3b5dc = 300;
    FUN_004cdad0();
    FUN_004cdb80();
    if (DAT_00d3b530 != '\x05') {
      if (DAT_00d3b531 == '\x01') {
        FUN_004105c0(*(undefined4 *)(iVar3 + 0xc));
        return;
      }
      if (DAT_00d3b531 == '\x02') {
        FUN_00410660(*(undefined4 *)(iVar3 + 0xc));
        return;
      }
      FUN_00410230(*(undefined4 *)(iVar3 + 0xc));
    }
  }
  return;
}
