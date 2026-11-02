#include "restore_types.h"
#define DAT_006fdcb4 (*(undefined1 *)0x006fdcb4u)
#define DAT_006fdcb8 (*(undefined *)0x006fdcb8u)
#define DAT_006fdce0 (*(undefined *)0x006fdce0u)
#define DAT_006fdce4 (*(undefined *)0x006fdce4u)
#define DAT_006fdcec (*(char *)0x006fdcecu)
#define DAT_006fdcf0 (*(undefined *)0x006fdcf0u)
#define DAT_006fdcf8 (*(char *)0x006fdcf8u)
#define DAT_006fdcfc (*(undefined *)0x006fdcfcu)
#define DAT_006fdd04 (*(char *)0x006fdd04u)
#define DAT_0070278c (*(undefined *)0x0070278cu)
#define DAT_0070278e (*(undefined *)0x0070278eu)
#define DAT_00702790 (*(undefined *)0x00702790u)
#define DAT_00702792 (*(undefined *)0x00702792u)
#define DAT_00702794 (*(undefined *)0x00702794u)
#define DAT_00702796 (*(undefined *)0x00702796u)
#define DAT_00702798 (*(undefined *)0x00702798u)
#define DAT_0070279c (*(undefined *)0x0070279cu)
#define DAT_0070279e (*(undefined *)0x0070279eu)
#define DAT_007027a0 (*(undefined *)0x007027a0u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38eb4 (*(undefined *)0x00e38eb4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_006fdce8 (*(undefined4 *)0x006fdce8u)
#define _DAT_006fdcf4 (*(undefined4 *)0x006fdcf4u)
#define _DAT_006fdd00 (*(undefined4 *)0x006fdd00u)
extern void FUN_0040d7f0();
extern int FUN_0046a400();
extern void FUN_00470990();
extern void FUN_004cc110();
extern void FUN_004dea50();
extern void FUN_004df2c0();
extern void FUN_004e65a0();
extern void FUN_00547ad0();

void FUN_00418bf0(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  
  iVar1 = (int)DAT_00e38ea8;
  puVar4 = &DAT_006fdcb8;
  puVar5 = &DAT_006fdce0;
  for (iVar3 = 10; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar5 = *puVar4;
    puVar4 = puVar4 + 1;
    puVar5 = puVar5 + 1;
  }
  FUN_004cc110(0x133,iVar1);
  DAT_00e38eb4 = (*(undefined4 *)(DAT_00e20768) != '1') - 1 & DAT_006fdce0;
  DAT_006fdce0 = 0;
  if (((undefined4 *)&DAT_00e20768)[4] == 'A') {
    if (((short)DAT_00702798 != 0) && (RESTORE_SLICE2(DAT_00702798,2) != 0)) {
      uVar2 = FUN_0046a400(DAT_00eaac28,0xde,DAT_00702798 & 0x1fff,(int)RESTORE_SLICE2(DAT_00702798,2),0,0);
      FUN_00470990(DAT_00eaac28,0xde,uVar2,DAT_00702798 & 0x1fff,(int)RESTORE_SLICE2(DAT_00702798,2),1,1,1);
      FUN_004df2c0(uVar2);
    }
    if (((DAT_0070279c != 0) && (DAT_0070279e != 0)) && (DAT_00e38ea8 == '3')) {
      uVar2 = FUN_0046a400(DAT_00eaac28,0xe1,(int)DAT_0070279c,(int)DAT_0070279e,0,0);
      FUN_00470990(DAT_00eaac28,0xe1,uVar2,(int)DAT_0070279c,(int)DAT_0070279e,1,1,1);
      FUN_0040d7f0(uVar2);
    }
    if ((((short)DAT_007027a0 != 0) && (RESTORE_SLICE2(DAT_007027a0,2) != 0)) && (DAT_00e38ea8 == '3')) {
      uVar2 = FUN_0046a400(DAT_00eaac28,0xe4,(int)(short)DAT_007027a0,(int)RESTORE_SLICE2(DAT_007027a0,2),0,0);
      FUN_00470990(DAT_00eaac28,0xe4,uVar2,(int)(short)DAT_007027a0,(int)RESTORE_SLICE2(DAT_007027a0,2),1,1,1);
      FUN_004e65a0(uVar2);
    }
  }
  else if (((undefined4 *)&DAT_00e20768)[4] == 'V') {
    if ((DAT_0070278c != 0) && (DAT_0070278e != 0)) {
      uVar2 = FUN_0046a400(DAT_00eaac28,0xe5,(int)DAT_0070278c,(int)DAT_0070278e,0,0);
      FUN_00470990(DAT_00eaac28,0xe5,uVar2,(int)DAT_0070278c,(int)DAT_0070278e,1,1,1);
      FUN_00547ad0(uVar2);
    }
    if (((DAT_00702790 != 0) && (DAT_00702792 != 0)) && ('\'' < DAT_00e38ea8)) {
      uVar2 = FUN_0046a400(DAT_00eaac28,0xea,(int)DAT_00702790,(int)DAT_00702792,0,0);
      FUN_00470990(DAT_00eaac28,0xea,uVar2,(int)DAT_00702790,(int)DAT_00702792,1,1,1);
      FUN_00547ad0(uVar2);
    }
    if (((DAT_00702794 != 0) && (DAT_00702796 != 0)) && ('(' < DAT_00e38ea8)) {
      uVar2 = FUN_0046a400(DAT_00eaac28,0xeb,(int)DAT_00702794,(int)DAT_00702796,0,0);
      FUN_00470990(DAT_00eaac28,0xeb,uVar2,(int)DAT_00702794,(int)DAT_00702796,1,1,1);
      FUN_00547ad0(uVar2);
    }
  }
  if (((undefined4 *)&DAT_00e20768)[2] != '1') {
    _DAT_006fdce8 = 0xffffffff;
    RESTORE_SLICE1(DAT_006fdcec,0) = 0;
    RESTORE_SLICE1(DAT_006fdce4,0) = 0xff;
    _DAT_006fdcf4 = 0xffffffff;
    RESTORE_SLICE1(DAT_006fdcf8,0) = 0;
    RESTORE_SLICE1(DAT_006fdcf0,0) = 0xff;
    _DAT_006fdd00 = 0xffffffff;
    RESTORE_SLICE1(DAT_006fdd04,0) = 0;
    RESTORE_SLICE1(DAT_006fdcfc,0) = 0xff;
    DAT_006fdcb4 = 1;
    FUN_004dea50();
    return;
  }
  DAT_006fdcb4 = 0;
  FUN_004dea50();
  return;
}
