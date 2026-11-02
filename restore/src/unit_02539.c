#include "restore_types.h"
#define DAT_005d1960 (*(undefined4 *)0x005d1960u)
#define DAT_006059e8 (*(undefined *)0x006059e8u)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00784f10 (*(undefined *)0x00784f10u)
#define DAT_00e38dd7 (*(undefined1 *)0x00e38dd7u)
#define DAT_00e38dd8 (*(undefined1 *)0x00e38dd8u)
#define DAT_00e38dd9 (*(undefined1 *)0x00e38dd9u)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00ea184c (*(undefined4 *)0x00ea184cu)
#define DAT_00ea186d (*(undefined1 *)0x00ea186du)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea624a (*(undefined1 *)0x00ea624au)
#define DAT_00ea624b (*(undefined1 *)0x00ea624bu)
#define DAT_00ea6254 (*(undefined *)0x00ea6254u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00404330();
extern void FUN_0040f250();
extern void FUN_0041d730();
extern void FUN_0041df60();
extern void FUN_00443ea0();
extern void FUN_00444720();
extern void FUN_00444b10();
extern void FUN_00451be0();
extern void FUN_0046a230();
extern void FUN_0046a260();
extern void FUN_0046b0e0();
extern void FUN_0046efb0();
extern void FUN_0046f120();
extern void FUN_004b32a0();
extern void FUN_004b4760();
extern void FUN_004b58d0();
extern void FUN_004b8290();
extern void FUN_004b8c90();
extern void FUN_004bb090();
extern int FUN_004cde40();
extern void FUN_004de430();
extern void FUN_004de820();
extern void FUN_004e1770();
extern void FUN_004e7d10();
extern void FUN_004e7e10();
extern void FUN_00540b50();

void FUN_00540a10(undefined4 param_1)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  char *pcVar6;
  char *pcVar7;
  
  DAT_00e38dd9 = 0;
  DAT_00e38dd8 = 0;
  DAT_00e38dd7 = 0;
  DAT_006059e8 = 0;
  FUN_004e7d10();
  FUN_004e7e10();
  DAT_00eaac28 = 1;
  DAT_00ea624b = 1;
  DAT_00ea624a = 1;
  FUN_004e1770();
  FUN_00540b50();
  FUN_0040f250();
  FUN_0041d730();
  FUN_0041df60();
  FUN_00404330();
  FUN_0046a230();
  FUN_0046b0e0();
  FUN_0046efb0();
  FUN_0046f120();
  FUN_0046a260();
  FUN_004b4760();
  FUN_004b8c90();
  FUN_004b58d0();
  FUN_004b8290();
  FUN_004b32a0();
  FUN_00451be0();
  if (DAT_00605a3c == 1) {
    DAT_00784f10 = 0;
    FUN_004de430(3);
    FUN_00443ea0(1);
    FUN_00444720();
    FUN_00444b10();
    FUN_004bb090();
  }
  FUN_004de820(param_1);
  iVar2 = DAT_00e39080;
  uVar4 = 0xffffffff;
  DAT_00eaac28 = 1;
  pcVar6 = &DAT_00e38e74;
  do {
    pcVar7 = pcVar6;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar7 = pcVar6 + 1;
    cVar1 = *pcVar6;
    pcVar6 = pcVar7;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  DAT_00ea624b = 1;
  pcVar6 = pcVar7 + -uVar4;
  pcVar7 = (char *)&DAT_00ea184c;
  for (uVar5 = uVar4 >> 2; uVar3 = DAT_00ea6254, uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)pcVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    pcVar7 = pcVar7 + 4;
  }
  DAT_00ea624a = 1;
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar7 = *pcVar6;
    pcVar6 = pcVar6 + 1;
    pcVar7 = pcVar7 + 1;
  }
  DAT_00ea186d = 1;
  uVar3 = FUN_004cde40(uVar3,*(undefined4 *)(&DAT_005d1960 + iVar2 * 4));
  *(undefined4 *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) = uVar3;
  return;
}
