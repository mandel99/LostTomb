#include "restore_types.h"
#include "town_menu.h"
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define _DAT_007848d8 (*(undefined4 *)0x007848d8u)
extern void FUN_00448a90();
extern int * FUN_00448d10();
extern uint FUN_00449390();
extern undefined4 FUN_00449550();
extern undefined2 * FUN_0044c0d0();
extern uint FUN_0044c430();
extern void FUN_0044c660();
extern int FUN_0044c960();
extern void FUN_00451540();

void FUN_0044c490(undefined4 param_1,short param_2)

{
  short sVar1;
  char cVar2;
  char cVar3;
  short sVar4;
  undefined4 *puVar5;
  short *psVar6;
  short *in_ECX;
  int iVar7;
  short sVar8;
  uint uVar9;
  short *psVar10;
  
  puVar5 = (undefined4 *)FUN_0044c0d0(param_1);
  if (puVar5 != (undefined4 *)0x0) {
    psVar6 = (short *)FUN_00448d10(0);
    if (psVar6 != (short *)0x0) {
      sVar4 = *psVar6;
      psVar10 = psVar6;
      for (iVar7 = 0x1f; iVar7 != 0; iVar7 = iVar7 + -1) {
        *(undefined4 *)psVar10 = *puVar5;
        puVar5 = puVar5 + 1;
        psVar10 = psVar10 + 2;
      }
      *psVar6 = sVar4;
      FUN_00448a90();
      psVar6[0x14] = 8;
      psVar6[0x15] = 0;
      cVar2 = FUN_00449390();
      cVar3 = FUN_00449390();
      if (cVar2 == cVar3) {
        if (psVar6[0xf] < 0) {
          sVar4 = psVar6[0x10];
          sVar8 = psVar6[0x11];
        }
        else {
          iVar7 = FUN_0044c960();
          if (iVar7 == 1) {
            sVar4 = psVar6[0xf];
            sVar8 = sVar4;
          }
          else {
            sVar8 = -1;
            sVar4 = -1;
          }
        }
        sVar1 = in_ECX[0xe];
        if ((sVar4 <= sVar1) && (sVar1 <= sVar8)) {
          psVar6[0xe] = sVar1;
        }
      }
      cVar2 = FUN_004493c0((const RestoreTownRecord *)psVar6);
      if (cVar2 != '\0') {
        cVar2 = FUN_004493c0((const RestoreTownRecord *)in_ECX);
        if (cVar2 != '\0') {
          cVar2 = FUN_00449550((int)in_ECX[2]);
          if (cVar2 != '\0') {
            if (psVar6[3] < 0) {
              sVar4 = psVar6[4];
              sVar8 = psVar6[5];
            }
            else {
              iVar7 = FUN_0044c960();
              if (iVar7 == 1) {
                sVar4 = psVar6[3];
                sVar8 = sVar4;
              }
              else {
                sVar8 = -1;
                sVar4 = -1;
              }
            }
            sVar1 = in_ECX[2];
            if ((sVar4 <= sVar1) && (sVar1 <= sVar8)) {
              psVar6[2] = sVar1;
            }
          }
        }
      }
      sVar4 = *in_ECX;
      psVar6[0x32] = param_2;
      psVar6[0x1a] = sVar4;
      cVar2 = FUN_00449390();
      if (cVar2 == '\0') {
        *(undefined1 *)((int)psVar6 + 99) = 0;
      }
      else {
        *(char *)((int)psVar6 + 99) = (char)in_ECX[0xe];
      }
      sVar4 = psVar6[10];
      psVar6[0x34] = in_ECX[6];
      psVar6[0x33] = in_ECX[2];
      *(char *)((int)psVar6 + 0x37) = (char)in_ECX[0x1b];
      uVar9 = (DAT_0078d7c0 + sVar4) % 0xc;
      sVar8 = sVar4 / 0xc;
      *(char *)((int)psVar6 + 3) = (char)uVar9;
      if ((int)(uVar9 & 0xff) < DAT_0078d7c0) {
        sVar8 = sVar8 + 1;
      }
      psVar6[10] = ((short)DAT_0078d7bc - (short)_DAT_007848d8) + sVar8;
      if (sVar4 == 0) {
        cVar2 = FUN_0044c430((int)*psVar6);
        if (cVar2 == '\0') {
          if ((char)psVar6[1] == '\x02') {
            FUN_00451540();
            return;
          }
          FUN_0044c660();
        }
      }
    }
  }
  return;
}
