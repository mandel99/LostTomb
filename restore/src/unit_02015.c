#include "restore_types.h"
#include "sound_streams.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d79c (*(undefined *)0x0078d79cu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_00799ae6 (*(undefined4 *)0x00799ae6u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd654 (*(undefined4 *)0x00bcd654u)
#define DAT_00bcd65c (*(undefined4 *)0x00bcd65cu)
#define DAT_00bcd66a (*(undefined4 *)0x00bcd66au)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f55b94 (*(undefined *)0x00f55b94u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
#define s_audio_ambient_locusts_mp3_005e4990 (*(char (*)[26])0x005e4990u)
extern int FUN_00488030();
extern int FUN_004b4800();
extern void FUN_004b5420();
extern int FUN_004becf0();

void FUN_004e7700(void)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int local_c;
  int local_8;
  short *local_4;
  
  if ((DAT_0078d79c == -1) &&
     ((iVar2 = FUN_004becf0(), iVar2 == DAT_0078d7c0 + 2 ||
      (iVar2 = FUN_004becf0(), iVar2 == DAT_0078d7c0 + -10)))) {
    DAT_0078d79c = 0xa48;
    local_8 = 1;
    if (0 < DAT_0078d790) {
      local_4 = &DAT_00799ae6;
      do {
        cVar1 = FUN_00488030(local_8);
        if (cVar1 != '\0') {
          local_c = 0;
          uVar4 = DAT_00d3b380;
          uVar3 = DAT_00d3b374;
          uVar8 = DAT_00d3b384;
          do {
            (&DAT_00d3b1c8)[DAT_00d3b1c4] = uVar3;
            DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
            if (99 < DAT_00d3b1c4) {
              DAT_00d3b1c4 = 0;
            }
            iVar2 = 0x1f;
            do {
              uVar3 = uVar8 >> 4;
              uVar5 = uVar8 & 1;
              uVar6 = uVar4 >> 4;
              uVar7 = uVar4 & 1;
              uVar8 = uVar8 >> 1;
              uVar4 = uVar4 >> 1;
              if ((uVar3 & 1) != uVar5) {
                uVar8 = uVar8 | 0x40000000;
              }
              if ((uVar6 & 1) != uVar7) {
                uVar4 = uVar4 | 0x40000000;
              }
              iVar2 = iVar2 + -1;
            } while (iVar2 != 0);
            DAT_00d3b374 = uVar8 & 0x7f;
            _DAT_00d3b36c = uVar4 & 7;
            _DAT_00d3b368 = uVar4 & 0x7f;
            _DAT_00d3b364 = uVar4 & 0x7fff;
            _DAT_00d3b378 = uVar8 & 7;
            _DAT_00d3b370 = uVar8 & 0x7fff;
            iVar2 = (int)((longlong)(ulonglong)_DAT_00d3b370 %
                         (longlong)((int)(DAT_00bca2d0 + (DAT_00bca2d0 >> 0x1f & 3U)) >> 2));
            DAT_00d3b380 = uVar4;
            DAT_00d3b384 = uVar8;
            iVar2 = FUN_004b4800(DAT_00eaac28,0x6b,
                                 ((int)(DAT_00bca2d0 * 7 + (DAT_00bca2d0 * 7 >> 0x1f & 7U)) >> 3) -
                                 iVar2,((int)(DAT_00bca2d0 * 3 + (DAT_00bca2d0 * 3 >> 0x1f & 7U)) >>
                                       3) - iVar2,0);
            local_4[0x14] = (short)iVar2;
            iVar9 = iVar2 * 0x184;
            (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
            DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
            (&DAT_00bcd630)[iVar9] = 8;
            (&DAT_00bcd654)[iVar2 * 0xc2] = (undefined2)local_8;
            if (99 < DAT_00d3b1c4) {
              DAT_00d3b1c4 = 0;
            }
            iVar2 = 0x1f;
            uVar4 = DAT_00d3b380;
            uVar8 = DAT_00d3b384;
            do {
              uVar3 = uVar8 >> 4;
              uVar5 = uVar8 & 1;
              uVar6 = uVar4 >> 4;
              uVar7 = uVar4 & 1;
              uVar8 = uVar8 >> 1;
              uVar4 = uVar4 >> 1;
              if ((uVar3 & 1) != uVar5) {
                uVar8 = uVar8 | 0x40000000;
              }
              if ((uVar6 & 1) != uVar7) {
                uVar4 = uVar4 | 0x40000000;
              }
              iVar2 = iVar2 + -1;
            } while (iVar2 != 0);
            _DAT_00d3b378 = uVar8 & 7;
            _DAT_00d3b370 = uVar8 & 0x7fff;
            _DAT_00d3b364 = uVar4 & 0x7fff;
            _DAT_00d3b368 = uVar4 & 0x7f;
            _DAT_00d3b36c = uVar4 & 7;
            uVar3 = uVar8 & 0x7f;
            DAT_00d3b374 = uVar3;
            DAT_00d3b380 = uVar4;
            DAT_00d3b384 = uVar8;
            *(short *)(&DAT_00bcd62e + iVar9) = (short)((ulonglong)_DAT_00d3b370 % 300);
            (&DAT_00bcd65c)[iVar9] = 0;
            (&DAT_00bcd66a)[iVar9] = 0xb;
            *(short *)(&DAT_00bcd61c + iVar9) =
                 ((short)((ulonglong)((longlong)local_c * 0x55555556) >> 0x20) -
                 (short)((longlong)local_c * 0x55555556 >> 0x3f)) + *local_4;
            iVar2 = local_c % 3;
            local_c = local_c + 1;
            *(short *)(&DAT_00bcd61e + iVar9) = (short)iVar2 + local_4[1];
          } while (local_c < 9);
          if (0 < local_4[0x1d]) {
            *(undefined1 *)((int)local_4 + 0xa1) = 0;
            local_4[0x1d] = 0;
            iVar2 = (int)local_4[0x57];
            if (iVar2 != 0) {
              iVar9 = iVar2 * 0x184;
              if (((&DAT_00bcd606)[iVar9] != '\x02') && ((&DAT_00bcd602)[iVar9] == 'U')) {
                (&DAT_00bcd606)[iVar9] = 2;
                FUN_004b5420(iVar2);
              }
              local_4[0x57] = 0;
            }
          }
        }
        local_8 = local_8 + 1;
        local_4 = local_4 + 0x84;
      } while (local_8 <= DAT_0078d790);
    }
    iVar2 = FUN_0055f5a1(RESTORE_SOUND,0);
    if (iVar2 != 0) {
      FUN_0055f561(RESTORE_SOUND,0);
    }
    DAT_00f55b94 = 1;
    iVar2 = FUN_0055f5a1(RESTORE_SOUND,2);
    if (iVar2 != 0) {
      FUN_0055f561(RESTORE_SOUND,2);
    }
    FUN_0055f449(RESTORE_SOUND,s_audio_ambient_locusts_mp3_005e4990,2,1);
  }
  return;
}
