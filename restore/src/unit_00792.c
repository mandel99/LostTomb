#include "restore_types.h"
#include "sound_streams.h"
#define DAT_0078d79c (*(undefined *)0x0078d79cu)
#define DAT_0078d7a0 (*(undefined *)0x0078d7a0u)
#define DAT_0078d7a4 (*(undefined *)0x0078d7a4u)
#define DAT_0078d7a8 (*(undefined *)0x0078d7a8u)
#define DAT_0078d7ac (*(undefined *)0x0078d7acu)
#define DAT_0078d7b0 (*(undefined *)0x0078d7b0u)
#define DAT_0078d7cc (*(undefined *)0x0078d7ccu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea3e34 (*(undefined4 *)0x00ea3e34u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f0c (*(undefined4 *)0x00ea5f0cu)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00ea6200 (*(undefined4 *)0x00ea6200u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eaaf5c (*(undefined1 *)0x00eaaf5cu)
#define DAT_00f53fd0 (*(undefined *)0x00f53fd0u)
#define DAT_00f55b94 (*(undefined *)0x00f55b94u)
extern void FUN_00443330();
extern void FUN_00444c20();
extern void FUN_004460b0();
extern void FUN_00452150();
extern void FUN_00452370();
extern void FUN_004525f0();
extern void FUN_004528b0();
extern void FUN_004539e0();
extern void FUN_00454e20();
extern void FUN_00454fa0();
extern void FUN_004550e0();
extern void FUN_00455430();
extern void FUN_004555d0();
extern void FUN_004558b0();
extern void FUN_00455900();
extern void FUN_00457a40();
extern void FUN_00457af0();
extern void FUN_00458440();
extern void FUN_00458540();
extern void FUN_00459070();
extern void FUN_00459100();
extern void FUN_00459350();
extern void FUN_004595a0();
extern void FUN_00459670();
extern void FUN_004599a0();
extern void FUN_0045aa30();
extern void FUN_0045aaa0();
extern void FUN_0045d5d0();
extern void FUN_0045e040();
extern void FUN_0045e6c0();
extern void FUN_00462180();
extern void FUN_004677c0();
extern void FUN_00468170();
extern void FUN_004684e0();
extern void FUN_00468860();
extern void FUN_00468b70();
extern void FUN_00469730();
extern void FUN_00469b90();
extern void FUN_0046a060();
extern void FUN_0046a0c0();
extern void FUN_0046a140();
extern void FUN_0046d840();
extern void FUN_0046f200();
extern void FUN_00475cc0();
extern void FUN_004776f0();
extern void FUN_0047e290();
extern void FUN_004860d0();
extern void FUN_004b52f0();
extern void FUN_004b7790();
extern undefined4 FUN_004c5120();
extern void FUN_004e9890();
extern void FUN_004f3890();
extern void FUN_0051f780();

void FUN_00451c50(void)

{
  short sVar1;
  int iVar2;
  short *psVar3;
  int iVar4;
  char *pcVar5;
  int iVar6;
  int iVar7;
  
  iVar7 = DAT_00eaac28;
  if (DAT_0078d7cc == 0) {
    FUN_0051f780();
  }
  else if (DAT_0078d7cc == 1) {
    FUN_004e9890(1);
  }
  else if (DAT_0078d7cc == 2) {
    FUN_00443330();
  }
  else if (DAT_0078d7cc == 3) {
    DAT_00f53fd0 = 1;
  }
  else if (DAT_0078d7cc == 4) {
    FUN_004460b0();
  }
  else if (DAT_0078d7cc == 5) {
    FUN_004b7790(0);
  }
  else if (DAT_0078d7cc == 6) {
    FUN_0051f780();
  }
  else if (DAT_0078d7cc == 7) {
    FUN_00468860();
  }
  else if (DAT_0078d7cc == 8) {
    FUN_0045e040();
  }
  else if (DAT_0078d7cc == 9) {
    FUN_00452150();
  }
  else if (DAT_0078d7cc == 10) {
    FUN_0045aa30();
  }
  else if (DAT_0078d7cc == 0xb) {
    iVar4 = 100;
    iVar2 = DAT_00eaac28 * 0x49d8;
    *(undefined4 *)(&DAT_00ea1894 + iVar2) = 0;
    psVar3 = (short *)(&DAT_00ea3e34 + iVar2);
    do {
      sVar1 = *psVar3;
      psVar3 = psVar3 + 1;
      iVar6 = *(int *)(&DAT_00ea1894 + iVar2) + (int)sVar1;
      iVar4 = iVar4 + -1;
      *(int *)(&DAT_00ea1894 + iVar2) = iVar6;
    } while (iVar4 != 0);
    if ((int)(&DAT_00ea5f0c)[iVar7 * 0x1276] < iVar6) {
      (&DAT_00ea5f0c)[iVar7 * 0x1276] = iVar6;
    }
  }
  else if (DAT_0078d7cc == 0xc) {
    FUN_004558b0();
  }
  else if (DAT_0078d7cc == 0x10) {
    FUN_0045aaa0();
  }
  else if (DAT_0078d7cc == 0x11) {
    FUN_0045d5d0();
  }
  else if (DAT_0078d7cc == 0x12) {
    FUN_0045e6c0();
  }
  else if (DAT_0078d7cc == 0x13) {
    FUN_00457a40();
  }
  else if (DAT_0078d7cc == 0x14) {
    FUN_00455900();
    FUN_004860d0();
  }
  else if (DAT_0078d7cc == 0x15) {
    FUN_00457af0();
  }
  else if (DAT_0078d7cc == 0x16) {
    FUN_00458440();
  }
  else if (DAT_0078d7cc == 0x17) {
    FUN_00458540();
  }
  else if (DAT_0078d7cc == 0x18) {
    FUN_00459070();
  }
  else if (DAT_0078d7cc == 0x19) {
    FUN_00459100();
    FUN_00459350();
    FUN_004595a0();
    FUN_0051f780();
    FUN_0051f780();
    FUN_00459670();
  }
  else if (DAT_0078d7cc == 0x1b) {
    FUN_004555d0();
  }
  else if (DAT_0078d7cc == 0x1c) {
    FUN_00455430();
  }
  else if (DAT_0078d7cc == 0x1d) {
    FUN_004b7790(1);
  }
  else if (DAT_0078d7cc == 0x1e) {
    DAT_00f53fd0 = 1;
  }
  else if (DAT_0078d7cc == 0x1f) {
    FUN_00462180();
  }
  else if (DAT_0078d7cc == 0x20) {
    FUN_00444c20();
    FUN_0046f200();
  }
  else if (DAT_0078d7cc == 0x21) {
    FUN_004677c0();
    FUN_00468170();
  }
  else if (DAT_0078d7cc == 0x22) {
    FUN_004684e0();
  }
  else if (DAT_0078d7cc == 0x23) {
    if (((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] == 0) ||
       (iVar7 = FUN_004c5120((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276]), iVar7 != 4)) {
      FUN_00454e20();
    }
    else if (*(int *)(&DAT_00ea6200 + DAT_00eaac28 * 0x49d8) == 0) {
      FUN_00454e20();
      *(undefined4 *)(&DAT_00ea6200 + DAT_00eaac28 * 0x49d8) = 10;
    }
    else {
      *(undefined4 *)(&DAT_00ea6200 + DAT_00eaac28 * 0x49d8) = 0;
    }
  }
  else if (DAT_0078d7cc == 0x24) {
    FUN_00454fa0();
  }
  else if (DAT_0078d7cc == 0x25) {
    FUN_0046a0c0();
    FUN_0046a140();
  }
  else if (DAT_0078d7cc == 0x26) {
    FUN_004539e0();
  }
  else if (DAT_0078d7cc == 0x27) {
    FUN_004528b0();
  }
  else if (DAT_0078d7cc == 0x28) {
    FUN_004525f0();
  }
  else if (DAT_0078d7cc == 0x2b) {
    FUN_00469730();
  }
  else if (DAT_0078d7cc == 0x2c) {
    FUN_00468b70();
  }
  else if (DAT_0078d7cc == 0x2d) {
    FUN_00469b90();
    FUN_0047e290();
  }
  else if (DAT_0078d7cc == 0x2e) {
    FUN_0051f780();
  }
  else if (DAT_0078d7cc == 0x2f) {
    FUN_0046a060();
  }
  else if (DAT_0078d7cc == 0x30) {
    FUN_004599a0();
  }
  else if (DAT_0078d7cc == 0x31) {
    FUN_004550e0();
  }
  else if (DAT_0078d7cc == 0x32) {
    FUN_0051f780();
  }
  if ((0 < DAT_0078d79c) && (DAT_0078d79c = DAT_0078d79c + -1, DAT_0078d79c == 0)) {
    FUN_0055f561(RESTORE_SOUND,2);
    DAT_00f55b94 = 0;
  }
  if ((0 < DAT_0078d7a8) && (DAT_0078d7a8 = DAT_0078d7a8 + -1, DAT_0078d7a8 == 0)) {
    FUN_0055f561(RESTORE_SOUND,2);
    DAT_00f55b94 = 0;
  }
  if (((DAT_00e38dda == '\x01') && (0 < DAT_0078d7a0)) &&
     (DAT_0078d7a0 = DAT_0078d7a0 + -1, DAT_0078d7a0 == 0)) {
    FUN_0055f561(RESTORE_SOUND,2);
    DAT_00f55b94 = 0;
  }
  if (0 < DAT_0078d7b0) {
    DAT_0078d7b0 = DAT_0078d7b0 + -1;
    if (DAT_0078d7b0 == 0) {
      FUN_00475cc0(0,0,DAT_00bca2d4 + -1,DAT_00bca2d0 + -1,1);
      FUN_004776f0(0,0,DAT_00bca2d4 + -1,DAT_00bca2d0 + -1);
      FUN_0046d840();
      iVar7 = 1;
      pcVar5 = &DAT_00bcd786;
      do {
        if ((pcVar5[4] != '\0') && (*pcVar5 == '_')) {
          FUN_004b52f0(iVar7);
        }
        pcVar5 = pcVar5 + 0x184;
        iVar7 = iVar7 + 1;
      } while ((int)pcVar5 < 0xc8ad42);
    }
    else {
      iVar7 = (DAT_0078d7ac - DAT_0078d7b0) / 2;
      if (iVar7 < 0xe4) {
        FUN_00475cc0(0xe5 - iVar7,0,0xe6 - iVar7,DAT_00bca2d4 + -1,1);
        FUN_004776f0(0xe5 - iVar7,0,0xe6 - iVar7,DAT_00bca2d4 + -1);
        FUN_0046d840();
      }
    }
    DAT_00e38dca = 1;
  }
  if ((0 < DAT_0078d7a4) && (DAT_0078d7a4 = DAT_0078d7a4 + -1, DAT_0078d7a4 == 0)) {
    DAT_00eaaf5c = '\0';
  }
  DAT_0078d7cc = DAT_0078d7cc + 1;
  if (0x32 < DAT_0078d7cc) {
    DAT_0078d7cc = 0;
    FUN_00452370();
  }
  if (DAT_00eaaf5c == '\0') {
    return;
  }
  FUN_004f3890();
  return;
}
