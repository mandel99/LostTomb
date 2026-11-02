#include "restore_types.h"
#include "sound_streams.h"
#define DAT_0078d7a8 (*(undefined *)0x0078d7a8u)
#define DAT_0078d83c (*(undefined *)0x0078d83cu)
#define DAT_0078d840 (*(undefined *)0x0078d840u)
#define DAT_0078e178 (*(undefined4 *)0x0078e178u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00f55b94 (*(undefined *)0x00f55b94u)
#define s_audio_ambient_frogs_mp3_005e4940 (*(char (*)[24])0x005e4940u)
extern void FUN_004583f0();
extern void FUN_004e7300(int32_t building);
extern void FUN_004e8e80();

void FUN_004e70b0(int param_1)

{
  short sVar1;
  int iVar2;
  short *psVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  iVar5 = 0;
  iVar6 = 0;
  DAT_0078d7a8 = param_1 * 0x330;
  iVar7 = 0;
  FUN_004583f0();
  DAT_0078d840 = 0;
  if (0 < DAT_0078d83c) {
    psVar3 = &DAT_0078e178;
    DAT_0078d840 = DAT_0078d83c;
    iVar4 = DAT_0078d83c;
    do {
      sVar1 = *psVar3;
      psVar3 = psVar3 + 1;
      if (iVar6 < (short)(&DAT_007999e8)[sVar1 * 0x84]) {
        iVar6 = (int)(short)(&DAT_007999e8)[sVar1 * 0x84];
      }
      iVar7 = iVar7 + (short)(&DAT_007999f4)[sVar1 * 0x84];
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
  }
  if (9 < iVar6) {
    iVar7 = (iVar7 * 0x41) / 100;
    do {
      if (iVar7 <= iVar5) break;
      DAT_0078d840 = 0;
      iVar4 = 0;
      if (0 < DAT_0078d83c) {
        do {
          iVar2 = (int)(short)(&DAT_0078e178)[iVar4];
          DAT_0078d840 = iVar4 + 1;
          if (((short)(&DAT_007999e8)[iVar2 * 0x84] == iVar6) &&
             (iVar5 = iVar5 + (short)(&DAT_007999f4)[iVar2 * 0x84], iVar5 <= iVar7)) {
            FUN_004e7300(iVar2);
          }
          iVar4 = DAT_0078d840;
        } while (DAT_0078d840 < DAT_0078d83c);
      }
      iVar6 = iVar6 + -1;
    } while (9 < iVar6);
  }
  iVar6 = FUN_0055f5a1(RESTORE_SOUND,0);
  if (iVar6 != 0) {
    FUN_0055f561(RESTORE_SOUND,0);
  }
  DAT_00f55b94 = 1;
  iVar6 = FUN_0055f5a1(RESTORE_SOUND,2);
  if (iVar6 != 0) {
    FUN_0055f561(RESTORE_SOUND,2);
  }
  FUN_0055f449(RESTORE_SOUND,s_audio_ambient_frogs_mp3_005e4940,2,1);
  FUN_004e8e80(0xfffffff1);
  return;
}
