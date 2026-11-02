#include "restore_types.h"
#define DAT_007900c0 (*(undefined4 *)0x007900c0u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a7e (*(undefined4 *)0x00799a7eu)
#define DAT_00799a97 (*(undefined4 *)0x00799a97u)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_00799ae0 (*(undefined4 *)0x00799ae0u)
#define DAT_009783a4 (*(undefined4 *)0x009783a4u)
#define DAT_009d1b1c (*(undefined4 *)0x009d1b1cu)
#define DAT_009de62c (*(undefined4 *)0x009de62cu)
#define DAT_00ae8d98 (*(undefined4 *)0x00ae8d98u)
#define DAT_00ae8e7b (*(undefined4 *)0x00ae8e7bu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00ae8e7d (*(undefined4 *)0x00ae8e7du)
#define DAT_00ae8f60 (*(undefined4 *)0x00ae8f60u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)
#define DAT_00bc96a8 (*(undefined *)0x00bc96a8u)
#define DAT_00bc96ac (*(undefined *)0x00bc96acu)
#define DAT_00bc96b0 (*(undefined *)0x00bc96b0u)
#define DAT_00bc96b4 (*(undefined *)0x00bc96b4u)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
#define DAT_00bc9760 (*(undefined *)0x00bc9760u)
#define DAT_00bc9764 (*(undefined *)0x00bc9764u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eaaf68 (*(undefined4 *)0x00eaaf68u)
extern void FUN_0040f860();
extern void FUN_00440060();
extern void FUN_00440540();
extern void FUN_00459020();
extern undefined4 FUN_00474380();
extern void FUN_00475720();
extern int FUN_00475b50();
extern void FUN_00475cc0();
extern void FUN_00476530();
extern void FUN_004767d0();
extern void FUN_00477500();
extern void FUN_004776f0();
extern void FUN_004777c0();
extern void FUN_00477890();
extern void FUN_0047afd0();
extern void FUN_0047b480();
extern void FUN_0047b590();
extern void FUN_0047bae0();
extern void FUN_0047bd50();
extern void FUN_0047f810();
extern uint FUN_0047f9b0();
extern void FUN_00485080();
extern undefined4 FUN_00485100();
extern void FUN_00485830();
extern void FUN_004e2970();
extern void FUN_004f4930();
extern void FUN_004f5e70();
extern void FUN_00517370();
extern void FUN_00517660();
extern void FUN_005178b0();
extern void FUN_005178e0();
extern void FUN_0053b090();

void FUN_00474700(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  byte bVar1;
  byte bVar2;
  short sVar3;
  uint uVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  char *pcVar10;
  int iVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  int local_c;
  
  iVar6 = param_5;
  iVar7 = param_4;
  local_c = 0;
  DAT_00bc971c = 0;
  FUN_00485080();
  FUN_0040f860(&DAT_009de62c,&DAT_00af598c,0x32c40);
  FUN_0040f860(&DAT_009d1b1c,&DAT_00ae8e7c,0xcb10);
  FUN_00485830(0);
  if (param_2 <= param_4) {
    param_4 = param_2;
    param_2 = iVar7;
  }
  if (param_3 <= param_5) {
    param_5 = param_3;
    param_3 = iVar6;
  }
  if (param_4 < 0) {
    param_4 = 0;
  }
  if (DAT_00bca2d0 <= param_2) {
    param_2 = DAT_00bca2d0 + -1;
  }
  if (param_5 < 0) {
    param_5 = 0;
  }
  if (DAT_00bca2d4 <= param_3) {
    param_3 = DAT_00bca2d4 + -1;
  }
  iVar6 = (param_2 - param_4) + 1;
  iVar7 = (param_3 - param_5) + 1;
  DAT_00bc9768 = DAT_00bca2d8 + param_5 * 0xe4 + param_4;
  for (DAT_00bc9760 = param_5; iVar8 = param_4, DAT_00bc9760 <= param_3;
      DAT_00bc9760 = DAT_00bc9760 + 1) {
    while (DAT_00bc9764 = iVar8, DAT_00bc9764 <= param_2) {
      uVar4 = (&DAT_00af598c)[DAT_00bc9768];
      if ((uVar4 & 0x2040202) == 0) {
        iVar8 = (int)*(short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2);
        if ((((uVar4 & 8) == 0) && ((uVar4 & 0x40000000) == 0)) &&
           (((iVar8 == 0 || ((uVar4 & 0x10000000) == 0)) ||
            (((&DAT_007999e8)[iVar8 * 0x84] != 0xe1 && ((&DAT_007999e8)[iVar8 * 0x84] != 0xd5))))))
        {
          if ((uVar4 & 0x100) == 0) {
            if ((uVar4 & 4) == 0) {
              if ((uVar4 & 0xeefed77f) != 0) {
                bVar1 = (&DAT_00b67d1c)[DAT_00bc9768];
                (&DAT_00af598c)[DAT_00bc9768] = uVar4 & 0x3052e80;
                bVar2 = (&DAT_00b285cc)[DAT_00bc9768];
                (&DAT_00b285cc)[DAT_00bc9768] = bVar2 & 0xf0;
                (&DAT_00b67d1c)[DAT_00bc9768] = bVar1 | 0x40;
                if (param_1 == 0) {
                  (&DAT_00b285cc)[DAT_00bc9768] = bVar2 & 0x70;
                  (&DAT_00b67d1c)[DAT_00bc9768] = bVar1 | 0x40;
                }
                DAT_00bc971c = DAT_00bc971c + 1;
              }
            }
            else {
              if ((param_1 == 0) && (iVar8 = FUN_0047f9b0(DAT_00bc9768), 0 < iVar8)) {
                FUN_004e2970(0x37,0,0);
                goto LAB_00474934;
              }
              if (DAT_00bc96b0 == 1) {
                FUN_0047f810(DAT_00bc9768,param_1);
                DAT_00bc971c = DAT_00bc971c + 1;
              }
            }
          }
          else {
            (&DAT_00af598c)[DAT_00bc9768] = uVar4 & 0x3052e80;
            (&DAT_00ae8e7c)[DAT_00bc9768] = 0;
            DAT_00bc971c = DAT_00bc971c + 1;
            if ((&DAT_00ae8d98)[DAT_00bc9768] == '\x05') {
              (&DAT_00ae8d98)[DAT_00bc9768] = 1;
            }
            if ((&DAT_00ae8e7d)[DAT_00bc9768] == '\x06') {
              (&DAT_00ae8e7d)[DAT_00bc9768] = 2;
            }
            if ((&DAT_00ae8f60)[DAT_00bc9768] == '\x05') {
              (&DAT_00ae8f60)[DAT_00bc9768] = 3;
            }
            if ((&DAT_00ae8e7b)[DAT_00bc9768] == '\x06') {
              (&DAT_00ae8e7b)[DAT_00bc9768] = 4;
            }
          }
        }
        else if (iVar8 != 0) {
          iVar9 = iVar8 * 0x108;
          sVar3 = (&DAT_007999e8)[iVar8 * 0x84];
          if (((((((sVar3 == 99) || (sVar3 == 0x58)) || (sVar3 == 0x59)) ||
                ((sVar3 == 0x5d || (sVar3 == 0xda)))) ||
               ((sVar3 == 0xdb || ((sVar3 == 0xdc || (sVar3 == 0xd7)))))) ||
              (((&DAT_007999d8)[iVar9] == '\x06' ||
               (((((sVar3 == 0x39 || (sVar3 == 0x36)) && (param_1 == 0)) && (DAT_00bc96b4 != 1)) ||
                (((cVar5 = FUN_00474380((int)sVar3), cVar5 != '\0' && (param_1 == 0)) &&
                 (DAT_00bc96ac != 1)))))))) ||
             (((((sVar3 = (&DAT_007999e8)[iVar8 * 0x84], sVar3 == 0x41 || (sVar3 == 0x42)) ||
                (((sVar3 == 0x43 || (((sVar3 == 0x44 || (sVar3 == 0x45)) || (sVar3 == 0x61)))) ||
                 (((sVar3 == 0x9c || (sVar3 == 0x9d)) || (sVar3 == 0x9e)))))) ||
               ((sVar3 == 0x9f || (sVar3 == 0xa0)))) && ((param_1 == 0 && (DAT_00bc96a8 != 1))))))
          goto LAB_00474934;
          if ((((sVar3 == 0x39) || (sVar3 == 0x36)) && (param_1 == 0)) && (DAT_00bc96b4 == 1)) {
            DAT_00bc9694 = param_1;
          }
          if (sVar3 == 0xb7) {
            FUN_004f4930(iVar8);
            FUN_004f5e70(iVar8,1);
          }
          bVar1 = *(byte *)((int)&DAT_00799a98 + iVar9 + 1);
          if (bVar1 != 0) {
            (&DAT_007900c0)[(uint)bVar1 * 0xc4] = 0;
          }
          bVar1 = *(byte *)((int)&DAT_00799aa0 + iVar9 + 3);
          if (bVar1 != 0) {
            (&DAT_009783a4)[(uint)bVar1 * 0x1c] = 0;
          }
          bVar1 = *(byte *)((int)&DAT_00799aa0 + iVar9 + 2);
          if (bVar1 != 0) {
            (&DAT_00eaaf68)[(uint)bVar1 * 0x38] = 0;
          }
          if (((*(char *)((int)&DAT_007999dc + iVar9 + 1) != '\0') &&
              ((&DAT_007999f4)[iVar8 * 0x84] != 0)) && (param_1 == 0)) {
            FUN_00459020(DAT_00eaac28,(int)*(short *)((int)&DAT_007999dc + iVar9 + 2),
                         (int)*(short *)(&DAT_007999e0 + iVar9),
                         (int)(short)(&DAT_007999f4)[iVar8 * 0x84]);
            (&DAT_007999f4)[iVar8 * 0x84] = 0;
          }
          sVar3 = (&DAT_007999e8)[iVar8 * 0x84];
          if (((sVar3 == 0x6c) || (sVar3 == 0xc3)) || (sVar3 == 0x73)) {
            *(undefined1 *)(&DAT_00799a7e + iVar8 * 0x84) = 0;
          }
          if ((&DAT_007999d8)[iVar9] != '\x06') {
            DAT_00bc971c = DAT_00bc971c + 1;
            FUN_00485100(iVar8);
          }
          (&DAT_007999d8)[iVar9] = 6;
          FUN_0053b090(2);
          (&DAT_00799a97)[iVar9] = 1;
          iVar11 = 0;
          iVar9 = iVar8;
          do {
            iVar9 = (int)*(short *)(&DAT_00799a10 + iVar9 * 0x108);
            if (iVar9 < 1) break;
            FUN_00485100(iVar9);
            iVar11 = iVar11 + 1;
            (&DAT_007999d8)[iVar9 * 0x108] = 6;
            (&DAT_00799a97)[iVar9 * 0x108] = 1;
          } while (iVar11 < 0x20d);
          iVar9 = 0;
          do {
            iVar8 = (int)*(short *)(&DAT_00799a12 + iVar8 * 0x108);
            if (iVar8 < 1) break;
            FUN_00485100(iVar8);
            iVar9 = iVar9 + 1;
            (&DAT_007999d8)[iVar8 * 0x108] = 6;
            (&DAT_00799a97)[iVar8 * 0x108] = 1;
          } while (iVar9 < 0x20d);
        }
        if ((param_1 == 0) && ((uVar4 & 0x148) != 0)) {
          local_c = 5;
        }
      }
LAB_00474934: ;
      DAT_00bc9768 = DAT_00bc9768 + 1;
      iVar8 = DAT_00bc9764 + 1;
    }
    DAT_00bc9768 = DAT_00bc9768 + (0xe4 - iVar6);
  }
  if (iVar7 < iVar6) {
    iVar7 = iVar6;
  }
  FUN_004767d0(param_4,param_5,param_2,param_3,0,1);
  if (local_c != 0) {
    FUN_004767d0(param_4 - local_c,param_5 - local_c,param_2 + local_c,param_3 + local_c,1,0);
  }
  FUN_00477500(param_4,param_5,param_2,param_3);
  FUN_004776f0(param_4,param_5,param_2,param_3);
  FUN_004777c0(param_4,param_5,param_2,param_3);
  FUN_0047bae0(param_4,param_5,param_2,param_3,0);
  FUN_0047b480(param_4,param_5,iVar7 + 2);
  FUN_00477890(param_4,param_5,param_2,param_3);
  FUN_00476530();
  FUN_00475720(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,1);
  iVar6 = 1;
  pcVar10 = &DAT_00799ae0;
  do {
    if ((*pcVar10 != '\0') && (*pcVar10 != '\x06')) {
      sVar3 = *(short *)(pcVar10 + 0x10);
      if (sVar3 == 0x1f) {
        iVar8 = FUN_00475b50(iVar6);
        if ((iVar8 == iVar6) && (*(int *)(pcVar10 + 0xd8) != 0)) {
          uVar13 = 0x1f;
LAB_00474eb3: ;
          uVar12 = *(undefined4 *)(pcVar10 + 0xd0);
LAB_00474eba: ;
          FUN_00440060(uVar12,uVar13,iVar6);
        }
      }
      else if (sVar3 == 0x1e) {
        if ((&DAT_007999e8)[*(short *)(pcVar10 + 0x3a) * 0x84] == 0x1e) {
          FUN_00440540(iVar6,(int)*(short *)(pcVar10 + 6),(int)*(short *)(pcVar10 + 8));
        }
        iVar8 = FUN_00475b50(iVar6);
        if ((iVar8 == iVar6) && (*(int *)(pcVar10 + 0xd8) != 0)) {
          uVar12 = *(undefined4 *)(pcVar10 + 0xd0);
          uVar13 = 0x1e;
          goto LAB_00474eba;
        }
      }
      else if (sVar3 == 0x21) {
        iVar8 = FUN_00475b50(iVar6);
        if ((iVar8 == iVar6) && (*(int *)(pcVar10 + 0xd8) != 0)) {
          uVar12 = *(undefined4 *)(pcVar10 + 0xd0);
          uVar13 = 0x21;
          goto LAB_00474eba;
        }
      }
      else if (((sVar3 == 0xd1) && (iVar8 = FUN_00475b50(iVar6), iVar8 == iVar6)) &&
              (*(int *)(pcVar10 + 0xd8) != 0)) {
        uVar13 = 0xd1;
        goto LAB_00474eb3;
      }
    }
    pcVar10 = pcVar10 + 0x108;
    iVar6 = iVar6 + 1;
    if (0x89b6d7 < (int)pcVar10) {
      FUN_0047afd0(param_4,param_5,iVar7 + 2);
      FUN_00475cc0(param_4,param_5,param_2,param_3,0);
      if (param_1 == 0) {
        FUN_0047bd50(param_4 + -1,param_5 + -1,param_2 + 1,param_3 + 1);
        FUN_0047b590(param_4 + -1,param_5 + -1,param_2 + 1,param_3 + 1);
        FUN_005178b0();
        FUN_00517370();
        FUN_00517660();
        FUN_005178e0();
      }
      return;
    }
  } while( true );
}
