#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_00d386e0 (*(undefined4 *)0x00d386e0u)
#define DAT_00d38700 (*(undefined *)0x00d38700u)
#define DAT_00d38710 (*(undefined4 *)0x00d38710u)
#define DAT_00d3b568 (*(undefined *)0x00d3b568u)
#define DAT_00d3b56c (*(undefined *)0x00d3b56cu)
#define DAT_00d3b570 (*(undefined *)0x00d3b570u)
#define DAT_00d3b574 (*(undefined *)0x00d3b574u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e20774 (*(undefined *)0x00e20774u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00e38dd9 (*(undefined1 *)0x00e38dd9u)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea5b3c (*(undefined4 *)0x00ea5b3cu)
#define DAT_00ea5b40 (*(undefined4 *)0x00ea5b40u)
#define DAT_00ea5b48 (*(undefined4 *)0x00ea5b48u)
#define DAT_00ea6018 (*(undefined4 *)0x00ea6018u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53db4 (*(undefined *)0x00f53db4u)
#define DAT_00f53db8 (*(undefined *)0x00f53db8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern undefined4 FUN_004181a0();
extern int FUN_004192b0();
extern int FUN_0041c380();
extern void FUN_00448370();
extern void FUN_004cc110();
extern void FUN_004ccb30();
extern void FUN_004ccbb0();
extern void FUN_004cce00();
extern void FUN_004ccfd0();
extern void FUN_004cd010();
extern undefined4 FUN_004ea880();
extern void FUN_00536820();
extern void FUN_00536a30();

void FUN_004194c0(void)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  char *pcVar8;
  char *pcVar9;
  char local_e8 [32];
  undefined1 local_c8 [200];
  
  DAT_00f53db8 = DAT_00e38da8 + 0x30;
  DAT_00f53db4 = DAT_00e38da4 + 0x10;
  DAT_00d3b56c = (0x400 - DAT_00e38db0) / 2;
  DAT_00d3b574 = DAT_00e38db0;
  DAT_00d3b570 = DAT_00e38dac;
  DAT_00d3b568 = (0x300 - DAT_00e38dac) / 2;
  if (DAT_00e38dd9 != '\x01') {
    iVar5 = FUN_004ea880();
    if (2 < iVar5) {
      FUN_00536820(DAT_00f53db8,DAT_00f53db4 + -7,0x22,0x10);
      FUN_004ccb30(0x3e,0x26,DAT_00f53db8,DAT_00f53db4 + 0x10,0x220,DAT_00f56fb4,0);
      DAT_00e20774 = 0;
      FUN_004ccfd0(0x3e,0x27,DAT_00f53db8 + 0x19,DAT_00f53db4 + 0x30,0x200,DAT_00f56f8c,0);
      return;
    }
    FUN_00536820(DAT_00f53db8,DAT_00f53db4 + -7,0x22,0x10);
    FUN_004ccb30(0x3e,1,DAT_00f53db8,DAT_00f53db4 + 0x10,0x220,DAT_00f56fb4,0);
    DAT_00e20774 = 0;
    FUN_004ccfd0(0x3e,0x10,DAT_00f53db8 + 0x19,DAT_00f53db4 + 0x30,0x200,DAT_00f56f8c,0);
    return;
  }
  uVar7 = DAT_007848d8 - DAT_0078d7bc >> 0x1f;
  iVar5 = DAT_0078d7c0 + ((DAT_007848d8 - DAT_0078d7bc ^ uVar7) - uVar7) * 0xc;
  uVar2 = FUN_0041c380(iVar5);
  FUN_00536820(DAT_00f53db8,DAT_00f53db4,0x22,0x1b);
  FUN_004ccb30(0x3e,0,DAT_00f53db8,DAT_00f53db4 + 0x10,0x220,DAT_00f56fb4,0);
  FUN_00536a30(DAT_00f53db8 + 0x10,DAT_00f53db4 + 0x38,0x20,7);
  uVar3 = FUN_004181a0(DAT_00f53db8 + 0x20,DAT_00f53db4 + 0x40,0x1f0,DAT_00f56f9c,0);
  FUN_004ccfd0(0x93,uVar3);
  DAT_00e20778 = 0;
  FUN_004ccbb0(0x94,0,DAT_00f53db8 + 0x28,DAT_00f53db4 + 0xb4,DAT_00f56f8c,0);
  FUN_004cce00((&DAT_00ea5b3c)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,
               DAT_00e20778 + 0x28 + DAT_00f53db8,DAT_00f53db4 + 0xb4,DAT_00f56f8c,0);
  DAT_00e20778 = 0;
  FUN_004ccbb0(0x94,1,DAT_00f53db8 + 0x28,DAT_00f53db4 + 200,DAT_00f56f8c,0);
  FUN_004cce00((&DAT_00ea5b40)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,
               DAT_00e20778 + 0x28 + DAT_00f53db8,DAT_00f53db4 + 200,DAT_00f56f8c,0);
  DAT_00e20778 = 0;
  FUN_004ccbb0(0x94,3,DAT_00f53db8 + 0x28,DAT_00f53db4 + 0xdc,DAT_00f56f8c,0);
  FUN_004cce00((&DAT_00ea5b48)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,
               DAT_00e20778 + 0x28 + DAT_00f53db8,DAT_00f53db4 + 0xdc,DAT_00f56f8c,0);
  DAT_00e20778 = 0;
  FUN_004ccbb0(0x94,5,DAT_00f53db8 + 0x28,DAT_00f53db4 + 0xf0,DAT_00f56f8c,0);
  FUN_004cce00(*(undefined4 *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048,
               DAT_00e20778 + 0x28 + DAT_00f53db8,DAT_00f53db4 + 0xf0,DAT_00f56f8c,0);
  DAT_00e20778 = 0;
  FUN_004ccbb0(0x94,4,DAT_00f53db8 + 0x28,DAT_00f53db4 + 0x104,DAT_00f56f8c,0);
  FUN_004cce00(*(undefined4 *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048,
               DAT_00e20778 + 0x28 + DAT_00f53db8,DAT_00f53db4 + 0x104,DAT_00f56f8c,0);
  iVar4 = FUN_004192b0();
  if (iVar4 != 0) {
    FUN_004ccbb0(0x94,6,DAT_00f53db8 + 0x113,DAT_00f53db4 + 0xb4,DAT_00f56f8c,0);
  }
  DAT_00e20778 = 0;
  FUN_004ccbb0(0x94,7,DAT_00f53db8 + 0x78,DAT_00f53db4 + 0x118,DAT_00f56f8c,0);
  FUN_004cce00(iVar5,0x40,&DAT_0057c048,DAT_00e20778 + 0x78 + DAT_00f53db8,DAT_00f53db4 + 0x118,
               DAT_00f56f8c,0);
  FUN_004ccbb0(0x94,0xf,DAT_00e20778 + 0x78 + DAT_00f53db8,DAT_00f53db4 + 0x118,DAT_00f56f8c,0);
  DAT_00e20778 = 0;
  FUN_004ccbb0(0x94,8,DAT_00f53db8 + 0xa0,DAT_00f53db4 + 300,DAT_00f56f8c,0);
  FUN_004ccbb0(0x99,*(int *)(&DAT_00ea6018 + DAT_00eaac28 * 0x49d8) + 1,
               DAT_00e20778 + 0xa0 + DAT_00f53db8,DAT_00f53db4 + 300,DAT_00f56f8c,0);
  DAT_00e20778 = 0;
  FUN_004ccbb0(0x94,9,DAT_00f53db8 + 200,DAT_00f53db4 + 0x140,DAT_00f56f8c,0);
  FUN_004cce00(uVar2,0x40,&DAT_0057c048,DAT_00e20778 + 200 + DAT_00f53db8,DAT_00f53db4 + 0x140,
               DAT_00f56f8c,0);
  switch((int)(DAT_00d38700)) {
  case 1: ;
    FUN_004ccfd0(0x94,0x10,DAT_00f53db8 + 0x28,DAT_00f53db4 + 0x154,0x220,DAT_00f56f8c,0);
    break;
  case 2: ;
    FUN_004ccfd0(0x94,10,DAT_00f53db8 + 0x28,DAT_00f53db4 + 0x154,0x220,DAT_00f56f8c,0);
    break;
  case 3: ;
    FUN_004ccfd0(0x94,0xb,DAT_00f53db8 + 0x28,DAT_00f53db4 + 0x154,0x220,DAT_00f56f8c,0);
    break;
  case 4: ;
    FUN_004ccfd0(0x94,0xc,DAT_00f53db8 + 0x28,DAT_00f53db4 + 0x154,0x220,DAT_00f56f8c,0);
    break;
  case 5: ;
    FUN_004cc110(0x94,0xe);
    uVar7 = 0xffffffff;
    pcVar8 = &DAT_00e38e74;
    do {
      pcVar9 = pcVar8;
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      pcVar9 = pcVar8 + 1;
      cVar1 = *pcVar8;
      pcVar8 = pcVar9;
    } while (cVar1 != '\0');
    uVar7 = ~uVar7;
    pcVar8 = pcVar9 + -uVar7;
    pcVar9 = local_e8;
    for (uVar6 = uVar7 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
      pcVar8 = pcVar8 + 4;
      pcVar9 = pcVar9 + 4;
    }
    for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
      *pcVar9 = *pcVar8;
      pcVar8 = pcVar8 + 1;
      pcVar9 = pcVar9 + 1;
    }
    uVar7 = 0xffffffff;
    pcVar8 = &DAT_00d386e0;
    do {
      pcVar9 = pcVar8;
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      pcVar9 = pcVar8 + 1;
      cVar1 = *pcVar8;
      pcVar8 = pcVar9;
    } while (cVar1 != '\0');
    uVar7 = ~uVar7;
    pcVar8 = pcVar9 + -uVar7;
    pcVar9 = &DAT_00e38e74;
    for (uVar6 = uVar7 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
      pcVar8 = pcVar8 + 4;
      pcVar9 = pcVar9 + 4;
    }
    for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
      *pcVar9 = *pcVar8;
      pcVar8 = pcVar8 + 1;
      pcVar9 = pcVar9 + 1;
    }
    FUN_00448370(DAT_00e20768,local_c8,200);
    uVar2 = DAT_00f56f8c;
    uVar7 = 0xffffffff;
    pcVar8 = local_e8;
    do {
      pcVar9 = pcVar8;
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      pcVar9 = pcVar8 + 1;
      cVar1 = *pcVar8;
      pcVar8 = pcVar9;
    } while (cVar1 != '\0');
    uVar7 = ~uVar7;
    pcVar8 = pcVar9 + -uVar7;
    pcVar9 = &DAT_00e38e74;
    for (uVar6 = uVar7 >> 2; iVar4 = DAT_00f53db8, uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
      pcVar8 = pcVar8 + 4;
      pcVar9 = pcVar9 + 4;
    }
    for (uVar7 = uVar7 & 3; iVar5 = DAT_00f53db4, uVar7 != 0; uVar7 = uVar7 - 1) {
      *pcVar9 = *pcVar8;
      pcVar8 = pcVar8 + 1;
      pcVar9 = pcVar9 + 1;
    }
    goto LAB_00419c4e;
  case 6: ;
    FUN_004cc110(0x94,0xd);
    uVar2 = DAT_00e20768;
    uVar7 = 0xffffffff;
    pcVar8 = &DAT_00e38e74;
    do {
      pcVar9 = pcVar8;
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      pcVar9 = pcVar8 + 1;
      cVar1 = *pcVar8;
      pcVar8 = pcVar9;
    } while (cVar1 != '\0');
    uVar7 = ~uVar7;
    pcVar8 = pcVar9 + -uVar7;
    pcVar9 = local_e8;
    for (uVar6 = uVar7 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
      pcVar8 = pcVar8 + 4;
      pcVar9 = pcVar9 + 4;
    }
    for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
      *pcVar9 = *pcVar8;
      pcVar8 = pcVar8 + 1;
      pcVar9 = pcVar9 + 1;
    }
    uVar7 = 0xffffffff;
    pcVar8 = &DAT_00d38710 + DAT_00e38ea8 * 0x4c;
    do {
      pcVar9 = pcVar8;
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      pcVar9 = pcVar8 + 1;
      cVar1 = *pcVar8;
      pcVar8 = pcVar9;
    } while (cVar1 != '\0');
    uVar7 = ~uVar7;
    pcVar8 = pcVar9 + -uVar7;
    pcVar9 = &DAT_00e38e74;
    for (uVar6 = uVar7 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
      pcVar8 = pcVar8 + 4;
      pcVar9 = pcVar9 + 4;
    }
    for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
      *pcVar9 = *pcVar8;
      pcVar8 = pcVar8 + 1;
      pcVar9 = pcVar9 + 1;
    }
    FUN_00448370(uVar2,local_c8,200);
    iVar5 = DAT_00f53db4;
    uVar7 = 0xffffffff;
    pcVar8 = local_e8;
    do {
      pcVar9 = pcVar8;
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      pcVar9 = pcVar8 + 1;
      cVar1 = *pcVar8;
      pcVar8 = pcVar9;
    } while (cVar1 != '\0');
    uVar7 = ~uVar7;
    pcVar8 = pcVar9 + -uVar7;
    pcVar9 = &DAT_00e38e74;
    for (uVar6 = uVar7 >> 2; iVar4 = DAT_00f53db8, uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
      pcVar8 = pcVar8 + 4;
      pcVar9 = pcVar9 + 4;
    }
    for (uVar7 = uVar7 & 3; uVar2 = DAT_00f56f8c, uVar7 != 0; uVar7 = uVar7 - 1) {
      *pcVar9 = *pcVar8;
      pcVar8 = pcVar8 + 1;
      pcVar9 = pcVar9 + 1;
    }
LAB_00419c4e: ;
    FUN_004cd010(local_c8,iVar4 + 0x28,iVar5 + 0x154,0x220,uVar2,0);
  }
  FUN_004ccb30(0xd,1,DAT_00f53db8 + 0x10,DAT_00f53db4 + 0x18c,0x200,DAT_00f56f8c,0);
  return;
}
