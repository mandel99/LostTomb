#include "restore_types.h"
#define DAT_00605398 (*(undefined4 *)0x00605398u)
#define DAT_006053a0 (*(undefined4 *)0x006053a0u)
#define DAT_012666e4 (*(undefined *)0x012666e4u)
#define DAT_01266710 (*(undefined *)0x01266710u)
#define DAT_01266720 (*(undefined4 *)0x01266720u)
#define DAT_01266724 (*(undefined4 *)0x01266724u)
#define DAT_01266728 (*(undefined4 *)0x01266728u)
#define DAT_0126672c (*(undefined *)0x0126672cu)
#define DAT_01266840 (*(undefined4 *)0x01266840u)
#define DAT_01266944 (*(undefined *)0x01266944u)
extern int FUN_005699ca();
extern undefined4 FUN_00569a14();
extern void FUN_00569a47();
extern void FUN_00569a70();

undefined4 FUN_00569831(undefined4 param_1)

{
  BYTE *pBVar1;
  byte *pbVar2;
  byte bVar3;
  byte bVar4;
  UINT CodePage;
  UINT *pUVar5;
  BOOL BVar6;
  uint uVar7;
  BYTE *pBVar8;
  int iVar9;
  byte *pbVar10;
  int iVar11;
  byte *pbVar12;
  undefined4 *puVar13;
  _cpinfo local_1c;
  uint local_8;
  
  CodePage = FUN_005699ca(param_1);
  if (CodePage == DAT_01266710) {
    return 0;
  }
  if (CodePage != 0) {
    iVar11 = 0;
    pUVar5 = &DAT_006053a0;
    do {
      if (*pUVar5 == CodePage) {
        puVar13 = &DAT_01266840;
        for (iVar9 = 0x40; iVar9 != 0; iVar9 = iVar9 + -1) {
          *puVar13 = 0;
          puVar13 = puVar13 + 1;
        }
        local_8 = 0;
        iVar11 = iVar11 * 0x30;
        *(undefined1 *)puVar13 = 0;
        pbVar12 = (byte *)(iVar11 + 0x6053b0);
        do {
          bVar3 = *pbVar12;
          pbVar10 = pbVar12;
          while ((bVar3 != 0 && (bVar3 = pbVar10[1], bVar3 != 0))) {
            uVar7 = (uint)*pbVar10;
            if (uVar7 <= bVar3) {
              bVar4 = (&DAT_00605398)[local_8];
              do {
                pbVar2 = (byte *)((int)&DAT_01266840 + uVar7 + 1);
                *pbVar2 = *pbVar2 | bVar4;
                uVar7 = uVar7 + 1;
              } while (uVar7 <= bVar3);
            }
            pbVar10 = pbVar10 + 2;
            bVar3 = *pbVar10;
          }
          local_8 = local_8 + 1;
          pbVar12 = pbVar12 + 8;
        } while (local_8 < 4);
        DAT_0126672c = 1;
        DAT_01266710 = CodePage;
        DAT_01266944 = FUN_00569a14(CodePage);
        DAT_01266720 = *(undefined4 *)(iVar11 + 0x6053a4);
        DAT_01266724 = *(undefined4 *)(iVar11 + 0x6053a8);
        DAT_01266728 = *(undefined4 *)(iVar11 + 0x6053ac);
        goto LAB_005699b9;
      }
      pUVar5 = pUVar5 + 0xc;
      iVar11 = iVar11 + 1;
    } while ((int)pUVar5 < 0x605490);
    BVar6 = GetCPInfo(CodePage,&local_1c);
    if (BVar6 == 1) {
      puVar13 = &DAT_01266840;
      DAT_01266710 = CodePage;
      for (iVar11 = 0x40; iVar11 != 0; iVar11 = iVar11 + -1) {
        *puVar13 = 0;
        puVar13 = puVar13 + 1;
      }
      *(undefined1 *)puVar13 = 0;
      DAT_01266944 = 0;
      if (local_1c.MaxCharSize < 2) {
        DAT_0126672c = 0;
      }
      else {
        if (local_1c.LeadByte[0] != '\0') {
          pBVar8 = local_1c.LeadByte + 1;
          do {
            bVar3 = *pBVar8;
            if (bVar3 == 0) break;
            for (uVar7 = (uint)pBVar8[-1]; uVar7 <= bVar3; uVar7 = uVar7 + 1) {
              pbVar12 = (byte *)((int)&DAT_01266840 + uVar7 + 1);
              *pbVar12 = *pbVar12 | 4;
            }
            pBVar1 = pBVar8 + 1;
            pBVar8 = pBVar8 + 2;
          } while (*pBVar1 != 0);
        }
        uVar7 = 1;
        do {
          pbVar12 = (byte *)((int)&DAT_01266840 + uVar7 + 1);
          *pbVar12 = *pbVar12 | 8;
          uVar7 = uVar7 + 1;
        } while (uVar7 < 0xff);
        DAT_01266944 = FUN_00569a14(CodePage);
        DAT_0126672c = 1;
      }
      DAT_01266720 = 0;
      DAT_01266724 = 0;
      DAT_01266728 = 0;
      goto LAB_005699b9;
    }
    if (DAT_012666e4 == 0) {
      return 0xffffffff;
    }
  }
  FUN_00569a47();
LAB_005699b9: ;
  FUN_00569a70();
  return 0;
}
