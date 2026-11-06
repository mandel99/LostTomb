#include "restore_types.h"
#define DAT_005e03fc (*(undefined4 *)0x005e03fcu)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00ea4126 (*(undefined4 *)0x00ea4126u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
#define DAT_00eadd58 (*(undefined *)0x00eadd58u)
#define DAT_00eadd5c (*(undefined *)0x00eadd5cu)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00eadd94 (*(undefined *)0x00eadd94u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
#define _DAT_00eadd54 (*(undefined4 *)0x00eadd54u)
#define s_wavs_quarry_wav_005f7d34 (*(char (*)[16])0x005f7d34u)
extern undefined4 FUN_0047d350();
extern int FUN_004cde60();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern void FUN_004fc5c0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00508e00(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  char cVar3;
  char cVar4;
  
  if (param_1 == 0x19) {
    cVar3 = 'w';
  }
  else {
    cVar3 = (-(param_1 != 0x1a) & 2U) - 0x40;
  }
  _DAT_00eadd54 = 0x5f;
  if (DAT_00eadd94 != 0) {
    FUN_0047d350(s_wavs_quarry_wav_005f7d34,1,0);
    DAT_00eadd94 = 0;
  }
  FUN_00536820(DAT_00eadd90,DAT_00eadd8c,DAT_00eadd88,DAT_00eadd84);
  FUN_0053e590(DAT_00f571fc + param_1,DAT_00eadd90 + 10,DAT_00eadd8c + 10);
  FUN_004d6250(cVar3,0,DAT_00eadd90,DAT_00eadd8c + 10,DAT_00eadd88 << 4,DAT_00f56fb4,0,DAT_00eadd90,
               DAT_00eadd8c);
  uVar1 = FUN_004cde60((int)(short)(&DAT_00799a2c)[DAT_00eadd6c * 0x84],200);
  DAT_00e20778 = 0;
  FUN_004d62f0(cVar3,2,DAT_00eadd90 + 0x20,DAT_00eadd8c + 0x2c,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d6860(uVar1,0x40,&DAT_005e03fc,DAT_00e20778 + 0x20 + DAT_00eadd90,DAT_00eadd8c + 0x2c,
               DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  FUN_004d62f0(cVar3,3,DAT_00e20778 + 0x20 + DAT_00eadd90,DAT_00eadd8c + 0x2c,DAT_00f56f8c,0,
               DAT_00eadd90,DAT_00eadd8c);
  if (DAT_00eadd58 < 1) {
    iVar2 = (DAT_00eadd88 + -4) * 0x10;
    uVar1 = 0x19;
    cVar4 = 'E';
  }
  else {
    cVar4 = cVar3;
    if (*(short *)(&DAT_00ea4126 + (DAT_00eaac28 * 0x24ec + param_1) * 2) == 0) {
      if ((short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < 1) {
        iVar2 = (DAT_00eadd88 + -4) * 0x10;
        uVar1 = 5;
      }
      else if (DAT_00eadd5c < 100) {
        if (DAT_00eadd5c < 0x4b) {
          if (DAT_00eadd5c < 0x32) {
            if (DAT_00eadd5c < 0x19) {
              iVar2 = (DAT_00eadd88 + -4) * 0x10;
              uVar1 = 10;
            }
            else {
              iVar2 = (DAT_00eadd88 + -4) * 0x10;
              uVar1 = 9;
            }
          }
          else {
            iVar2 = (DAT_00eadd88 + -4) * 0x10;
            uVar1 = 8;
          }
        }
        else {
          iVar2 = (DAT_00eadd88 + -4) * 0x10;
          uVar1 = 7;
        }
      }
      else {
        iVar2 = (DAT_00eadd88 + -4) * 0x10;
        uVar1 = 6;
      }
    }
    else {
      iVar2 = (DAT_00eadd88 + -4) * 0x10;
      uVar1 = 4;
    }
  }
  FUN_004d6160(cVar4,uVar1,DAT_00eadd90 + 0x20,DAT_00eadd8c + 0x46,iVar2,DAT_00f56f8c,0,DAT_00eadd90
               ,DAT_00eadd8c);
  FUN_004d65b0(DAT_00eadd90 + 0x10,DAT_00eadd8c + 0x88,DAT_00eadd88 + -2,4,DAT_00eadd90,DAT_00eadd8c
              );
  FUN_004fc5c0(DAT_00eadd8c + 0x8e);
  FUN_004d6160(cVar3,1,DAT_00eadd90 + 0x20,DAT_00eadd84 * 0x10 + -0x71 + DAT_00eadd8c,
               (DAT_00eadd88 + -4) * 0x10,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  return;
}
