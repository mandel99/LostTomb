#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00eadd58 (*(undefined *)0x00eadd58u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00eadd94 (*(undefined *)0x00eadd94u)
#define DAT_00f55c5c (*(undefined *)0x00f55c5cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00eadd54 (*(undefined4 *)0x00eadd54u)
#define s_wavs_oracle_wav_005f7c24 (*(char (*)[16])0x005f7c24u)
#define s_wavs_temple_comm_wav_005f7c60 (*(char (*)[21])0x005f7c60u)
#define s_wavs_temple_farm_wav_005f7c90 (*(char (*)[21])0x005f7c90u)
#define s_wavs_temple_love_wav_005f7c34 (*(char (*)[21])0x005f7c34u)
#define s_wavs_temple_ship_wav_005f7c78 (*(char (*)[21])0x005f7c78u)
#define s_wavs_temple_war_wav_005f7c4c (*(char (*)[20])0x005f7c4cu)
extern undefined4 FUN_0047d350();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d65b0();
extern void FUN_004fc5c0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00505560(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  char *pcVar4;
  
  iVar3 = 0;
  if (((short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] < 0x8c) ||
     (0x95 < (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84])) {
    iVar3 = 4;
    switch((int)(param_1)) {
    case 0: ;
      uVar2 = 0x5c;
      if (DAT_00eadd94 == 0) goto LAB_0050564b;
      pcVar4 = s_wavs_temple_farm_wav_005f7c90;
      break;
    case 1: ;
      uVar2 = 0x5d;
      if (DAT_00eadd94 == 0) goto LAB_0050564b;
      pcVar4 = s_wavs_temple_ship_wav_005f7c78;
      break;
    case 2: ;
      uVar2 = 0x5e;
      if (DAT_00eadd94 == 0) goto LAB_0050564b;
      pcVar4 = s_wavs_temple_comm_wav_005f7c60;
      break;
    case 3: ;
      uVar2 = 0x5f;
      if (DAT_00eadd94 == 0) goto LAB_0050564b;
      pcVar4 = s_wavs_temple_war_wav_005f7c4c;
      break;
default: ;
      uVar2 = 0x60;
      if (DAT_00eadd94 == 0) goto LAB_0050564b;
      pcVar4 = s_wavs_temple_love_wav_005f7c34;
    }
    FUN_0047d350(pcVar4,1,0);
    DAT_00eadd94 = 0;
  }
  else {
    if (DAT_00eadd94 != 0) {
      FUN_0047d350(s_wavs_oracle_wav_005f7c24,1,0);
      DAT_00eadd94 = 0;
    }
    uVar2 = 0xa1;
  }
LAB_0050564b: ;
  _DAT_00eadd54 = 0x43;
  FUN_00536820(DAT_00eadd90,DAT_00eadd8c,DAT_00eadd88,DAT_00eadd84);
  if (((short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] < 0x8c) ||
     (0x95 < (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84])) {
    iVar1 = 0;
  }
  else {
    iVar1 = param_1;
    if (3 < param_1) {
      iVar1 = 4;
    }
    iVar1 = iVar1 * 2;
    uVar2 = 0xa1;
  }
  FUN_004d6250(uVar2,iVar1,DAT_00eadd90,DAT_00eadd8c + 0xc,DAT_00eadd88 << 4,DAT_00f56fb4,0,
               DAT_00eadd90,DAT_00eadd8c);
  if (iVar3 != 0) {
    FUN_004d65b0(DAT_00eadd90 + 0x10,DAT_00eadd8c + 0x38,DAT_00eadd88 + -2,iVar3,DAT_00eadd90,
                 DAT_00eadd8c);
    FUN_004fc5c0(DAT_00eadd8c + 0x3e);
  }
  if (0 < DAT_00eadd58) {
    if (3 < param_1) {
      param_1 = 4;
    }
    FUN_0053e590(param_1 + 0x15 + DAT_00f55c5c,DAT_00eadd90 + 0xbe,
                 (DAT_00eadd84 + iVar3) * 0x10 + -0xb6 + DAT_00eadd8c);
    return;
  }
  FUN_004d6160(0x45,0x19,DAT_00eadd90 + 0x20,(DAT_00eadd84 + -8) * 0x10 + DAT_00eadd8c,
               (DAT_00eadd88 + -4) * 0x10,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  return;
}
