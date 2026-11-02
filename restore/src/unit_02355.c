#include "restore_types.h"
#define DAT_005ce3bc (*(undefined4 *)0x005ce3bcu)
#define DAT_005cf0dc (*(undefined4 *)0x005cf0dcu)
#define DAT_005f89cc (*(undefined4 *)0x005f89ccu)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_007056e8 (*(undefined *)0x007056e8u)
#define DAT_007056ec (*(undefined *)0x007056ecu)
#define DAT_007056f0 (*(undefined *)0x007056f0u)
#define DAT_00705ea4 (*(undefined4 *)0x00705ea4u)
#define DAT_00784cb0 (*(undefined1 *)0x00784cb0u)
#define DAT_00784ddc (*(undefined *)0x00784ddcu)
#define DAT_00784de0 (*(undefined *)0x00784de0u)
#define DAT_00784de2 (*(undefined *)0x00784de2u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00ea5fa0 (*(undefined4 *)0x00ea5fa0u)
#define DAT_00ea5fa4 (*(undefined4 *)0x00ea5fa4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f60 (*(undefined *)0x00f53f60u)
#define DAT_00f53f64 (*(undefined *)0x00f53f64u)
#define DAT_00f53f78 (*(undefined *)0x00f53f78u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define _DAT_00784cb0 (*(undefined4 *)0x00784cb0u)
extern int FUN_00403f10();
extern int FUN_004d5ed0();
extern undefined4 FUN_004d5ee0();
extern void FUN_004d6250();
extern void FUN_004d6480();
extern void FUN_004d6860();

void FUN_00520000(void)

{
  int iVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  int iVar8;
  int iVar9;
  undefined4 uVar10;
  
  iVar6 = DAT_00f53f64;
  if (DAT_00f53f78 == 1) {
    iVar6 = DAT_00f53f60;
  }
  if (DAT_00605a3c == 1) {
    iVar6 = DAT_00f53f64;
  }
  iVar4 = -1;
  iVar8 = 0;
  if (0 < DAT_007056f0) {
    do {
      iVar4 = FUN_00403f10(DAT_007056ec,iVar4);
      iVar1 = iVar8 + 1;
      cVar3 = (DAT_007056c8 != iVar1) + '\x01';
      iVar9 = (-(uint)(DAT_007056c8 != iVar1) & 0xffff4439) + 0xfac1;
      uVar7 = DAT_00f56f94;
      if (DAT_007056c8 != iVar1) {
        uVar7 = DAT_00f56f8c;
      }
      if (DAT_007056ec == 0xe) {
        FUN_004d6480(6,iVar6 + -0xaa,iVar8 * 0x18 + 0x6e + DAT_007056e8,10,1,cVar3,DAT_00e38da8,
                     DAT_00e38da4);
        FUN_004d6250(0x30,*(int *)(&DAT_005ce3bc + (iVar4 + DAT_007056ec * 0x1e) * 4) + -1,
                     iVar6 + -0xaa,iVar8 * 0x18 + 0x71 + DAT_007056e8,0xa0,uVar7,iVar9,DAT_00e38da8,
                     DAT_00e38da4);
      }
      else if (DAT_007056ec == 0xf) {
        FUN_004d6480(6,iVar6 + -0xaa,iVar8 * 0x18 + 0x6e + DAT_007056e8,10,1,cVar3,DAT_00e38da8,
                     DAT_00e38da4);
        FUN_004d6250(0x30,*(int *)(&DAT_005ce3bc + (iVar4 + DAT_007056ec * 0x1e) * 4) + 4,
                     iVar6 + -0xaa,iVar8 * 0x18 + 0x71 + DAT_007056e8,0xa0,uVar7,iVar9,DAT_00e38da8,
                     DAT_00e38da4);
      }
      else if (DAT_007056ec == 0x11) {
        FUN_004d6480(6,iVar6 + -0xaa,iVar8 * 0x18 + 0x6e + DAT_007056e8,10,1,cVar3,DAT_00e38da8,
                     DAT_00e38da4);
        iVar5 = *(int *)(&DAT_005ce3bc + (iVar4 + DAT_007056ec * 0x1e) * 4);
        iVar8 = iVar8 * 0x18 + 0x71 + DAT_007056e8;
        if (iVar5 < 9) {
          FUN_004d6250(0x30,iVar5 + 9,iVar6 + -0xaa,iVar8,0xa0,uVar7,iVar9,DAT_00e38da8,DAT_00e38da4
                      );
        }
        else {
          FUN_004d6250(0x30,iVar5 + 0x27,iVar6 + -0xaa,iVar8,0xa0,uVar7,iVar9,DAT_00e38da8,
                       DAT_00e38da4);
        }
      }
      else if (DAT_007056ec == 0x10) {
        FUN_004d6480(6,iVar6 + -0xaa,iVar8 * 0x18 + 0x6e + DAT_007056e8,10,1,cVar3,DAT_00e38da8,
                     DAT_00e38da4);
        FUN_004d6250(0x30,*(int *)(&DAT_005ce3bc + (iVar4 + DAT_007056ec * 0x1e) * 4) + 6,
                     iVar6 + -0xaa,iVar8 * 0x18 + 0x71 + DAT_007056e8,0xa0,uVar7,iVar9,DAT_00e38da8,
                     DAT_00e38da4);
      }
      else if (DAT_007056ec == 0x12) {
        FUN_004d6480(6,iVar6 + -0xaa,iVar8 * 0x18 + 0x6e + DAT_007056e8,10,1,cVar3,DAT_00e38da8,
                     DAT_00e38da4);
        FUN_004d6250(0x30,*(int *)(&DAT_005ce3bc + (iVar4 + DAT_007056ec * 0x1e) * 4) + 0x11,
                     iVar6 + -0xaa,iVar8 * 0x18 + 0x71 + DAT_007056e8,0xa0,uVar7,iVar9,DAT_00e38da8,
                     DAT_00e38da4);
      }
      else if (DAT_007056ec == 0x13) {
        FUN_004d6480(6,iVar6 + -0xaa,iVar8 * 0x18 + 0x6e + DAT_007056e8,10,1,cVar3,DAT_00e38da8,
                     DAT_00e38da4);
        FUN_004d6250(0x30,*(int *)(&DAT_005ce3bc + (iVar4 + DAT_007056ec * 0x1e) * 4) + 0x13,
                     iVar6 + -0xaa,iVar8 * 0x18 + 0x71 + DAT_007056e8,0xa0,uVar7,iVar9,DAT_00e38da8,
                     DAT_00e38da4);
      }
      else if (DAT_007056ec == 0x1a) {
        FUN_004d6480(6,iVar6 + -0xaa,iVar8 * 0x18 + 0x6e + DAT_007056e8,10,1,cVar3,DAT_00e38da8,
                     DAT_00e38da4);
        FUN_004d6250(0x30,*(int *)(&DAT_005ce3bc + (iVar4 + DAT_007056ec * 0x1e) * 4) + 0x16,
                     iVar6 + -0xaa,iVar8 * 0x18 + 0x71 + DAT_007056e8,0xa0,uVar7,iVar9,DAT_00e38da8,
                     DAT_00e38da4);
      }
      else if (DAT_007056ec == 0x25) {
        FUN_004d6480(6,iVar6 + -0xaa,iVar8 * 0x18 + 0x6e + DAT_007056e8,10,1,cVar3,DAT_00e38da8,
                     DAT_00e38da4);
        if (iVar8 == 0) {
          if (DAT_00784cb0 == '\x02') {
            FUN_004d6250(0x40,0x53,iVar6 + -0xaa,DAT_007056e8 + 0x71,0xa0,uVar7,iVar9,DAT_00e38da8,
                         DAT_00e38da4);
          }
          else if (DAT_00784cb0 == '\x01') {
            FUN_004d6250(0x40,0x52,iVar6 + -0xaa,DAT_007056e8 + 0x71,0xa0,uVar7,iVar9,DAT_00e38da8,
                         DAT_00e38da4);
          }
          else {
            FUN_004d6250(0x40,0x54,iVar6 + -0xaa,DAT_007056e8 + 0x71,0xa0,uVar7,iVar9,DAT_00e38da8,
                         DAT_00e38da4);
          }
        }
        else {
          FUN_004d6250(0x40,(_DAT_00784cb0 & 0xff) + 0x66,iVar6 + -0xaa,
                       iVar8 * 0x18 + 0x71 + DAT_007056e8,0xa0,uVar7,iVar9,DAT_00e38da8,DAT_00e38da4
                      );
        }
      }
      else if (DAT_007056ec == 0x1b) {
        FUN_004d6480(6,iVar6 + -0xaa,iVar8 * 0x18 + 0x6e + DAT_007056e8,10,1,cVar3,DAT_00e38da8,
                     DAT_00e38da4);
        FUN_004d6250(0x30,*(int *)(&DAT_005ce3bc + (iVar4 + DAT_007056ec * 0x1e) * 4) + 0x22,
                     iVar6 + -0xaa,iVar8 * 0x18 + 0x71 + DAT_007056e8,0xa0,uVar7,iVar9,DAT_00e38da8,
                     DAT_00e38da4);
      }
      else if (DAT_007056ec == 0x1c) {
        iVar5 = (int)*(short *)((int)&DAT_00784ddc + *(int *)(&DAT_005cf0dc + iVar4 * 4) * 2 + 2);
        if (iVar4 == 1) {
          if (iVar5 == DAT_00784de0) {
            iVar2 = (&DAT_00ea5fa0)[DAT_00eaac28 * 0x1276];
joined_r0x005206b7: ;
            if (iVar2 == 0) goto LAB_0052089a;
          }
        }
        else if (iVar4 == 2) {
          if ((iVar5 == DAT_00784de0) && ((&DAT_00ea5fa0)[DAT_00eaac28 * 0x1276] == 0))
          goto LAB_0052089a;
          if (iVar5 == DAT_00784de2) {
            iVar2 = *(int *)(&DAT_00ea5fa4 + DAT_00eaac28 * 0x49d8);
            goto joined_r0x005206b7;
          }
        }
        if (iVar5 != 0) {
          FUN_004d6480(6,iVar6 + -0x12a,iVar8 * 0x18 + 0x6e + DAT_007056e8,0x12,1,cVar3,DAT_00e38da8
                       ,DAT_00e38da4);
          FUN_004d6250(0xc6,iVar5,iVar6 + -0x10a,iVar8 * 0x18 + 0x71 + DAT_007056e8,0xb0,uVar7,iVar9
                       ,DAT_00e38da8,DAT_00e38da4);
        }
      }
      else {
        iVar8 = iVar8 * 0x18;
        iVar5 = FUN_004d5ee0(1,cVar3,DAT_00e38da8,DAT_00e38da4);
        iVar5 = FUN_004d5ed0(iVar8 + 0x6e + DAT_007056e8,iVar5 + 7);
        FUN_004d6480(6,(iVar6 - iVar5) + -0x61);
        if (DAT_007056ec == 0x24) {
          iVar2 = (&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276];
          iVar9 = FUN_004d5ed0(iVar8 + 0x71 + DAT_007056e8,0xb0,uVar7,iVar9,DAT_00e38da8,
                               DAT_00e38da4);
          iVar5 = (iVar6 - iVar9) + -0x61;
          iVar9 = iVar4 + -0x82 + (short)(&DAT_007999e8)[iVar2 * 0x84] * 2;
          uVar10 = 0xbd;
        }
        else {
          iVar9 = FUN_004d5ed0(iVar8 + 0x71 + DAT_007056e8,0xb0,uVar7,iVar9,DAT_00e38da8,
                               DAT_00e38da4);
          iVar5 = (iVar6 - iVar9) + -0x59;
          iVar9 = *(int *)(&DAT_005ce3bc + (iVar4 + DAT_007056ec * 0x1e) * 4);
          uVar10 = 0x1c;
        }
        FUN_004d6250(uVar10,iVar9,iVar5);
        iVar9 = *(int *)(&DAT_005ce3bc + (iVar4 + DAT_007056ec * 0x1e) * 4);
        if (iVar9 == 7) {
          iVar9 = 0x5a;
        }
        if ((iVar9 != 0x39) && ((&DAT_00705ea4)[iVar9 * 10] != 0)) {
          FUN_004d6860((&DAT_00705ea4)[iVar9 * 10],0x40,&DAT_005f89cc,iVar6 + -0x52,
                       iVar8 + 0x72 + DAT_007056e8,uVar7,0,DAT_00e38da8,DAT_00e38da4);
        }
      }
LAB_0052089a: ;
      iVar8 = iVar1;
    } while (iVar1 < DAT_007056f0);
  }
  return;
}
