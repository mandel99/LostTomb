#include "restore_types.h"
#include "sound_streams.h"
#define DAT_0078d7a0 (*(undefined *)0x0078d7a0u)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define DAT_00f55b94 (*(undefined *)0x00f55b94u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
#define s_audio_ambient_hailstorm_mp3_005c92f8 (*(char (*)[28])0x005c92f8u)
extern void FUN_004e8e80();
extern int FUN_004ea9c0();
extern int FUN_004eada0();

void FUN_004e6ed0(void)

{
  byte bVar1;
  char cVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  byte *pbVar8;
  uint uVar9;
  uint uVar10;
  
  pbVar8 = &DAT_00bcd786;
  uVar3 = DAT_00d3b374;
  uVar9 = DAT_00d3b384;
  uVar10 = DAT_00d3b380;
  do {
    if (pbVar8[4] != 0) {
      (&DAT_00d3b1c8)[DAT_00d3b1c4] = uVar3;
      DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
      if (99 < DAT_00d3b1c4) {
        DAT_00d3b1c4 = 0;
      }
      iVar6 = 0x1f;
      do {
        uVar3 = uVar9 >> 4;
        uVar4 = uVar9 & 1;
        uVar5 = uVar10 >> 4;
        uVar7 = uVar10 & 1;
        uVar9 = uVar9 >> 1;
        uVar10 = uVar10 >> 1;
        if ((uVar3 & 1) != uVar4) {
          uVar9 = uVar9 | 0x40000000;
        }
        if ((uVar5 & 1) != uVar7) {
          uVar10 = uVar10 | 0x40000000;
        }
        iVar6 = iVar6 + -1;
      } while (iVar6 != 0);
      _DAT_00d3b378 = uVar9 & 7;
      _DAT_00d3b370 = uVar9 & 0x7fff;
      _DAT_00d3b364 = uVar10 & 0x7fff;
      _DAT_00d3b368 = uVar10 & 0x7f;
      _DAT_00d3b36c = uVar10 & 7;
      DAT_00d3b374 = uVar9 & 0x7f;
      uVar4 = _DAT_00d3b370 % 100;
      DAT_00d3b380 = uVar10;
      DAT_00d3b384 = uVar9;
      if (pbVar8[0x2e] == 8) {
        iVar6 = 0x19;
      }
      else {
        bVar1 = *pbVar8;
        if ((((bVar1 == 0xb) || (bVar1 == 0xc)) || (bVar1 == 0xd)) ||
           (((0x2a < bVar1 && (bVar1 < 0x3a)) || (bVar1 == 99)))) {
          iVar6 = 0x32;
        }
        else {
          cVar2 = FUN_004ea9c0(bVar1);
          if (cVar2 == '\0') {
            cVar2 = FUN_004eada0(*pbVar8);
            iVar6 = (-(uint)(cVar2 != '\0') & 0xffffffe7) + 0x4b;
          }
          else {
            iVar6 = 0x32;
          }
        }
      }
      uVar9 = DAT_00d3b384;
      uVar10 = DAT_00d3b380;
      uVar3 = DAT_00d3b374;
      if ((int)uVar4 <= iVar6) {
        pbVar8[0x2e] = 4;
      }
    }
    pbVar8 = pbVar8 + 0x184;
  } while ((int)pbVar8 < 0xc8ad42);
  iVar6 = FUN_0055f5a1(RESTORE_SOUND,0);
  if (iVar6 != 0) {
    FUN_0055f561(RESTORE_SOUND,0);
  }
  DAT_00f55b94 = 1;
  iVar6 = FUN_0055f5a1(RESTORE_SOUND,2);
  if (iVar6 != 0) {
    FUN_0055f561(RESTORE_SOUND,2);
  }
  FUN_0055f449(RESTORE_SOUND,s_audio_ambient_hailstorm_mp3_005c92f8,2,1);
  FUN_004e8e80(0xfffffff1);
  DAT_0078d7a0 = 0x198;
  return;
}
