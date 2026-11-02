#include "restore_types.h"
#define DAT_005d1960 (*(undefined4 *)0x005d1960u)
#define DAT_00784c94 (*(undefined *)0x00784c94u)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5b48 (*(undefined4 *)0x00ea5b48u)
#define DAT_00ea5cbc (*(undefined4 *)0x00ea5cbcu)
#define DAT_00ea5cc8 (*(undefined4 *)0x00ea5cc8u)
#define DAT_00ea5d08 (*(undefined4 *)0x00ea5d08u)
#define DAT_00ea5d0c (*(undefined4 *)0x00ea5d0cu)
#define DAT_00ea609c (*(undefined4 *)0x00ea609cu)
#define DAT_00ea60a0 (*(undefined4 *)0x00ea60a0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int * FUN_0044bb90();
extern void FUN_0044c490();
extern void FUN_0044ef90();
extern void FUN_0045a7b0();
extern void FUN_0045a990();
extern void FUN_0046e840();
extern int FUN_004cde40();
extern void FUN_004e0cf0();

void FUN_004460b0(void)

{
  int iVar1;
  short *psVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  
  iVar4 = DAT_00eaac28;
  if (-1 < *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8)) {
    (&DAT_00ea609c)[DAT_00eaac28 * 0x1276] = 0;
    (&DAT_00ea60a0)[iVar4 * 0x1276] = 0;
    (&DAT_00ea5d0c)[iVar4 * 0x1276] = 0;
    goto LAB_004461cd;
  }
  if (*(int *)(&DAT_00ea5d08 + DAT_00eaac28 * 0x49d8) == 0) {
    iVar3 = DAT_00eaac28 * 0x49d8;
    iVar1 = FUN_004cde40(DAT_00784c94,*(undefined4 *)(&DAT_005d1960 + DAT_00e39080 * 4));
    iVar4 = DAT_00eaac28;
    *(int *)(&DAT_00ea187c + iVar3) = *(int *)(&DAT_00ea187c + iVar3) + iVar1;
    iVar1 = FUN_004cde40(DAT_00784c94,*(undefined4 *)(&DAT_005d1960 + DAT_00e39080 * 4));
    (&DAT_00ea5cc8)[iVar4 * 0x1276] = (&DAT_00ea5cc8)[iVar4 * 0x1276] + iVar1;
    FUN_0045a990();
    FUN_0045a7b0();
    iVar4 = DAT_00eaac28;
    iVar1 = DAT_00784c94;
    *(undefined4 *)(&DAT_00ea5d08 + DAT_00eaac28 * 0x49d8) = 1;
    if (iVar1 < 1) goto LAB_004461cd;
    DAT_00e950dc = 1;
    if ((&DAT_00ea5cbc)[iVar4 * 0x1276] != 10) {
      FUN_004e0cf0(0x73,0,0,0,0);
      FUN_0046e840();
      iVar4 = DAT_00eaac28;
      goto LAB_004461cd;
    }
    uVar5 = 0x75;
  }
  else {
    if ((&DAT_00ea5d0c)[DAT_00eaac28 * 0x1276] != 0) goto LAB_004461cd;
    (&DAT_00ea5d0c)[DAT_00eaac28 * 0x1276] = 1;
    uVar5 = 0x74;
  }
  DAT_00e950dc = 1;
  FUN_004e0cf0(uVar5,0,0,0,0);
  iVar4 = DAT_00eaac28;
LAB_004461cd: ;
  if (((&DAT_00ea5b48)[iVar4 * 0x1276] == 0) &&
     (psVar2 = (short *)FUN_0044bb90(0,0), psVar2 != (short *)0x0)) {
    while ((*(uint *)(psVar2 + 0x14) >> 4 & 1) == 0) {
      psVar2 = (short *)FUN_0044bb90(psVar2,0);
      if (psVar2 == (short *)0x0) {
        return;
      }
    }
    if ((psVar2 != (short *)0x0) && ((*(uint *)(psVar2 + 0x14) >> 2 & 1) == 0)) {
      psVar2[0xb] = 0;
      psVar2[0xc] = -1;
      psVar2[0xd] = -1;
      FUN_0044c490((int)*psVar2,(-(uint)(psVar2[2] != 3) & 10) + 0xab);
      FUN_0044ef90();
      *(undefined1 *)((int)psVar2 + 3) = 0xc;
    }
  }
  return;
}
