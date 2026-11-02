#include "restore_types.h"
#include "campaign_profile.h"
#include "sound_streams.h"
#define DAT_005761cc (*(undefined4 *)0x005761ccu)
#define DAT_005761d0 (*(undefined4 *)0x005761d0u)
#define DAT_00700998 (*(int32_t *)0x00700998u)
#define DAT_007027b4 (*(undefined1 *)0x007027b4u)
#define DAT_007056b8 (*(int32_t *)0x007056b8u)
#define DAT_007056c0 (*(int32_t *)0x007056c0u)
#define DAT_007056c4 (*(int32_t *)0x007056c4u)
#define DAT_007056c8 (*(int32_t *)0x007056c8u)
#define DAT_00d3b198 (*(int32_t *)0x00d3b198u)
#define DAT_00d3b1b3 (*(char *)0x00d3b1b3u)
#define DAT_00d3b1bc (*(undefined1 *)0x00d3b1bcu)
#define DAT_00d3b1c0 (*(undefined1 *)0x00d3b1c0u)
#define DAT_00e20764 (*(uint32_t *)0x00e20764u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e370bc (*(undefined4 *)0x00e370bcu)
#define DAT_00e37100 (*(undefined4 *)0x00e37100u)
#define DAT_00e38aec (*(char *)0x00e38aecu)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define PTR_DAT_005761d4 (*(unsigned char * *)0x005761d4u)

extern void FUN_004cbea0();
extern void FUN_004cc110();
extern void FUN_004cc160();
extern void FUN_004ce3a0();
extern void FUN_004ce3c0();
extern bool FUN_004ce3f0();
extern undefined4 FUN_004d4d80();
extern undefined4 FUN_004d4e20();
extern undefined4 FUN_004d5100();
extern undefined4 * FUN_005169c0();
extern undefined4 * FUN_00516a20();
extern int FUN_00538cd0();
extern undefined4 FUN_00561994();
extern undefined4 * FUN_00561a0a();

void FUN_00402720(void)

{
  char cVar1;
  char *pcVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  char *pcVar7;
  char *pcVar8;
  bool bVar9;
  char local_64 [100];
  
  DAT_007056c4 = 0;
  DAT_007056c8 = 0;
  DAT_007056b8 = 0;
  DAT_007056c0 = 0;
  if (DAT_00d3b198 == 0) {
    DAT_00e38dda = DAT_00d3b1bc;
    DAT_00e38dca = 1;
    return;
  }
  if (DAT_00d3b198 != 1) {
    DAT_007056b8 = 0;
    DAT_007056c0 = 0;
    DAT_007056c4 = 0;
    DAT_007056c8 = 0;
    return;
  }
  FUN_004ce3a0(&DAT_00e37100);
  FUN_004cbea0(&PTR_DAT_005761d4,&DAT_00e38aec,3);
  FUN_004ce3c0(&DAT_00e37100);
  pcVar2 = (char *)FUN_005169c0();
  uVar4 = 0xffffffff;
  do {
    pcVar8 = pcVar2;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar8 = pcVar2 + 1;
    cVar1 = *pcVar2;
    pcVar2 = pcVar8;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  pcVar2 = pcVar8 + -uVar4;
  pcVar8 = local_64;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pcVar8 = pcVar8 + 4;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar8 = *pcVar2;
    pcVar2 = pcVar2 + 1;
    pcVar8 = pcVar8 + 1;
  }
  uVar4 = 0xffffffff;
  pcVar2 = &DAT_005761d0;
  do {
    pcVar8 = pcVar2;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar8 = pcVar2 + 1;
    cVar1 = *pcVar2;
    pcVar2 = pcVar8;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  iVar6 = -1;
  pcVar2 = local_64;
  do {
    pcVar7 = pcVar2;
    if (iVar6 == 0) break;
    iVar6 = iVar6 + -1;
    pcVar7 = pcVar2 + 1;
    cVar1 = *pcVar2;
    pcVar2 = pcVar7;
  } while (cVar1 != '\0');
  pcVar2 = pcVar8 + -uVar4;
  pcVar8 = pcVar7 + -1;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pcVar8 = pcVar8 + 4;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar8 = *pcVar2;
    pcVar2 = pcVar2 + 1;
    pcVar8 = pcVar8 + 1;
  }
  uVar4 = 0xffffffff;
  pcVar2 = (char *)&DAT_00e37100;
  do {
    pcVar8 = pcVar2;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar8 = pcVar2 + 1;
    cVar1 = *pcVar2;
    pcVar2 = pcVar8;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  iVar6 = -1;
  pcVar2 = local_64;
  do {
    pcVar7 = pcVar2;
    if (iVar6 == 0) break;
    iVar6 = iVar6 + -1;
    pcVar7 = pcVar2 + 1;
    cVar1 = *pcVar2;
    pcVar2 = pcVar7;
  } while (cVar1 != '\0');
  pcVar2 = pcVar8 + -uVar4;
  pcVar8 = pcVar7 + -1;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pcVar8 = pcVar8 + 4;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar8 = *pcVar2;
    pcVar2 = pcVar2 + 1;
    pcVar8 = pcVar8 + 1;
  }
  if (DAT_00d3b1c0 != 0) {
    iVar6 = FUN_004ce3f0(local_64);
    if (iVar6 == 0) {
      FUN_004ce3a0(&DAT_00e37100);
      DAT_00d3b1b3 = 100;
      return;
    }
    if (DAT_00d3b1c0 == 1) {
      FUN_0055f561(RESTORE_SOUND,0);
      FUN_004d5100(local_64);
      goto LAB_0040292e;
    }
  }
  if (DAT_00d3b1c0 == 2) {
    FUN_004d4d80(local_64);
  }
  else {
    iVar6 = FUN_00561a0a(local_64,&DAT_005761cc);
    if (iVar6 != 0) {
      FUN_00561994(iVar6);
      FUN_004cc110(5,0x6e);
      FUN_004cc160(5,0x6f);
      iVar6 = FUN_00538cd0(DAT_00e20768,DAT_00e20764,4);
      if (iVar6 == 7) {
        FUN_004ce3a0(&DAT_00e37100);
        return;
      }
    }
    FUN_004d4e20(local_64);
    if (DAT_007027b4 == '\x01') {
      uVar4 = 0xffffffff;
      pcVar2 = local_64;
      do {
        pcVar8 = pcVar2;
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        pcVar8 = pcVar2 + 1;
        cVar1 = *pcVar2;
        pcVar2 = pcVar8;
      } while (cVar1 != '\0');
      uVar4 = ~uVar4;
      pcVar2 = pcVar8 + -uVar4;
      pcVar8 = (char *)&DAT_00700998;
      for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(undefined4 *)pcVar8 = *(undefined4 *)pcVar2;
        pcVar2 = pcVar2 + 4;
        pcVar8 = pcVar8 + 4;
      }
      for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
        *pcVar8 = *pcVar2;
        pcVar2 = pcVar2 + 1;
        pcVar8 = pcVar8 + 1;
      }
      uVar3 = FUN_00516a20();
      FUN_0041b8a0(RESTORE_CAMPAIGN_PROFILE,(const char *)(uintptr_t)uVar3);
    }
  }
LAB_0040292e: ;
  FUN_004ce3a0(&DAT_00e37100);
  uVar4 = 0xffffffff;
  pcVar2 = (char *)&DAT_00e37100;
  do {
    pcVar8 = pcVar2;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar8 = pcVar2 + 1;
    cVar1 = *pcVar2;
    pcVar2 = pcVar8;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  pcVar2 = pcVar8 + -uVar4;
  pcVar8 = &DAT_00e370bc;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pcVar8 = pcVar8 + 4;
  }
  bVar9 = DAT_00d3b1c0 < 2;
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar8 = *pcVar2;
    pcVar2 = pcVar2 + 1;
    pcVar8 = pcVar8 + 1;
  }
  if (bVar9) {
    DAT_00e38dda = 1;
  }
  DAT_00e38dca = 1;
  return;
}
