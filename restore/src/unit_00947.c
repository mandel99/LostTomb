#include "restore_types.h"
#define DAT_005f80fc (*(undefined4 *)0x005f80fcu)
#define DAT_00784cb0 (*(undefined1 *)0x00784cb0u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d7fc (*(undefined *)0x0078d7fcu)
#define DAT_0078d804 (*(undefined *)0x0078d804u)
#define DAT_0078d808 (*(undefined *)0x0078d808u)
#define DAT_0078dd90 (*(undefined4 *)0x0078dd90u)
#define DAT_00799a24 (*(undefined4 *)0x00799a24u)
#define DAT_00799b2a (*(undefined4 *)0x00799b2au)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define _DAT_0078d800 (*(undefined4 *)0x0078d800u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern void FUN_004695d0();
extern int FUN_004696b0();
extern void FUN_0046d100();
extern void FUN_0047c2b0();
extern void FUN_00517370();
extern void FUN_00517660();
extern void FUN_0053b090();

void FUN_00469730(void)

{
  bool bVar1;
  char cVar2;
  ushort uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  ushort *puVar9;
  int iVar10;
  int iVar11;
  uint uVar12;
  int local_c;
  int local_8;
  int local_4;
  
  bVar1 = false;
  _DAT_0078d800 = 0;
  DAT_0078d7fc = 0;
  DAT_0078d804 = 0;
  local_4 = 1;
  if (0 < DAT_0078d790) {
    puVar9 = &DAT_00799b2a;
    uVar7 = DAT_00d3b384;
    uVar12 = DAT_00d3b380;
    do {
      iVar10 = DAT_0078d7fc;
      if (((char)puVar9[-0x25] == '\x01') && (puVar9[-0x1d] == 99)) {
        if ((short)*puVar9 < 0) {
          *puVar9 = 0;
        }
        uVar3 = *puVar9 + 1;
        *puVar9 = uVar3;
        if ((short)uVar3 < 0x21) {
          if (*(char *)((int)puVar9 + 0x73) == '\0') {
            (&DAT_0078dd90)[iVar10] = (short)local_4;
            DAT_0078d804 = DAT_0078d804 + 1;
            DAT_0078d7fc = iVar10 + 1;
            if (499 < DAT_0078d7fc) {
              DAT_0078d7fc = 499;
            }
            if (DAT_00784cb0 == '\x02') {
              uVar3 = uVar3 & 3;
            }
            else {
              uVar3 = uVar3 & 7;
            }
            if (uVar3 == 0) {
              (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
              DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
              if (99 < DAT_00d3b1c4) {
                DAT_00d3b1c4 = 0;
              }
              iVar10 = 0x1f;
              do {
                uVar4 = uVar7 >> 4;
                uVar5 = uVar7 & 1;
                uVar6 = uVar12 >> 4;
                uVar8 = uVar12 & 1;
                uVar7 = uVar7 >> 1;
                uVar12 = uVar12 >> 1;
                if ((uVar4 & 1) != uVar5) {
                  uVar7 = uVar7 | 0x40000000;
                }
                if ((uVar6 & 1) != uVar8) {
                  uVar12 = uVar12 | 0x40000000;
                }
                iVar10 = iVar10 + -1;
              } while (iVar10 != 0);
              DAT_00d3b374 = uVar7 & 0x7f;
              _DAT_00d3b370 = uVar7 & 0x7fff;
              _DAT_00d3b378 = uVar7 & 7;
              _DAT_00d3b364 = uVar12 & 0x7fff;
              _DAT_00d3b368 = uVar12 & 0x7f;
              _DAT_00d3b36c = uVar12 & 7;
              DAT_00d3b380 = uVar12;
              DAT_00d3b384 = uVar7;
              if ((*(byte *)((int)puVar9 + 3) & 3) == (uVar7 & 3)) {
                local_c = DAT_0078d808 + -1;
                if (local_c < 0) {
                  local_c = 7;
                }
                local_8 = DAT_0078d808 + 1;
                if (7 < local_8) {
                  local_8 = 0;
                }
                iVar10 = *(int *)(puVar9 + -0x1f);
                iVar11 = (int)*(short *)(&DAT_00b4e6fc +
                                        ((&DAT_005f80fc)[DAT_0078d808] + iVar10) * 2);
                if (((((iVar11 != 0) && ((&DAT_00799a24)[iVar11 * 0x108] == '\0')) &&
                     (cVar2 = FUN_004696b0(iVar11), cVar2 != '\0')) ||
                    (((iVar11 = (int)*(short *)(&DAT_00b4e6fc +
                                               ((&DAT_005f80fc)[local_c] + iVar10) * 2), iVar11 != 0
                      && ((&DAT_00799a24)[iVar11 * 0x108] == '\0')) &&
                     (cVar2 = FUN_004696b0(iVar11), cVar2 != '\0')))) ||
                   (((iVar11 = (int)*(short *)(&DAT_00b4e6fc +
                                              ((&DAT_005f80fc)[local_8] + iVar10) * 2),
                     uVar7 = DAT_00d3b384, uVar12 = DAT_00d3b380, iVar11 != 0 &&
                     ((&DAT_00799a24)[iVar11 * 0x108] == '\0')) &&
                    (cVar2 = FUN_004696b0(iVar11), uVar7 = DAT_00d3b384, uVar12 = DAT_00d3b380,
                    cVar2 != '\0')))) {
                  FUN_0046d100(iVar11,0);
                  FUN_004695d0(iVar11,1);
                  FUN_0053b090(0x12);
                  goto LAB_004699f9;
                }
              }
            }
          }
        }
        else {
          *(undefined1 *)(puVar9 + -0x25) = 4;
          DAT_00bc9694 = 0;
          FUN_0047c2b0(local_4,(int)(short)puVar9[-0x22],(int)(short)puVar9[-0x21],
                       *(undefined1 *)((int)puVar9 + -0x47),*(undefined1 *)((int)puVar9 + -0x47));
LAB_004699f9: ;
          bVar1 = true;
          uVar7 = DAT_00d3b384;
          uVar12 = DAT_00d3b380;
        }
      }
      local_4 = local_4 + 1;
      puVar9 = puVar9 + 0x84;
    } while (local_4 <= DAT_0078d790);
    if (bVar1) {
      FUN_00517370();
      FUN_00517660();
    }
  }
  return;
}
