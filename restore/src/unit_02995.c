#include "restore_types.h"
#include "crt_multibyte.h"
/* The four masks are BYTE-addressed. Built-in records remain DWORD-addressed
   at a 48-byte stride; each record's four range groups contain 8 BYTEs. */
#define RESTORE_CRT_BUILTIN_FIRST_PAGE (RESTORE_CRT_MB_CODEPAGE_RECORDS[0].codepage)

/* Reuse the recovered table and GetCPInfo branches, with native operand widths. */
uint32_t restore_crt_set_multibyte_codepage(int32_t param_1)

{
  BYTE *pBVar1;
  byte bVar3;
  byte bVar4;
  UINT CodePage;
  volatile const uint32_t *pUVar5;
  BOOL BVar6;
  uint uVar7;
  BYTE *pBVar8;
  int iVar9;
  volatile const uint8_t *pbVar10;
  int iVar11;
  volatile const uint8_t *pbVar12;
  volatile uint32_t *puVar13;
  uint32_t locale;
  _cpinfo local_1c;
  uint local_8;
  
  CodePage = FUN_005699ca(param_1);
  if (CodePage == RESTORE_CRT_CODEPAGE) {
    return 0;
  }
  if (CodePage != 0) {
    iVar11 = 0;
    pUVar5 = &RESTORE_CRT_BUILTIN_FIRST_PAGE;
    do {
      if (*pUVar5 == CodePage) {
        puVar13 = (volatile uint32_t *)RESTORE_CRT_MB_FLAGS;
        for (iVar9 = 0x40; iVar9 != 0; iVar9 = iVar9 + -1) {
          *puVar13 = 0;
          puVar13 = puVar13 + 1;
        }
        local_8 = 0;
        iVar11 = iVar11 * 0x30;
        *(volatile uint8_t *)puVar13 = 0;
        pbVar12 = (volatile const uint8_t *)(uintptr_t)((uint32_t)iVar11 + 0x6053b0u);
        do {
          bVar3 = *pbVar12;
          pbVar10 = pbVar12;
          while ((bVar3 != 0 && (bVar3 = pbVar10[1], bVar3 != 0))) {
            uVar7 = (uint)*pbVar10;
            if (uVar7 <= bVar3) {
              bVar4 = RESTORE_CRT_MB_CLASS_MASKS[local_8];
              do {
                RESTORE_CRT_MB_FLAGS[uVar7 + 1u] |= bVar4;
                uVar7 = uVar7 + 1;
              } while (uVar7 <= bVar3);
            }
            pbVar10 = pbVar10 + 2;
            bVar3 = *pbVar10;
          }
          local_8 = local_8 + 1;
          pbVar12 = pbVar12 + 8;
        } while (local_8 < 4);
        RESTORE_CRT_MB_ENABLED = 1;
        RESTORE_CRT_CODEPAGE = CodePage;
        locale = FUN_00569a14((int32_t)CodePage);
        RESTORE_CRT_MB_SYMBOLS[0] = *(volatile const uint32_t *)(uintptr_t)((uint32_t)iVar11 + 0x6053a4u);
        RESTORE_CRT_MB_SYMBOLS[1] = *(volatile const uint32_t *)(uintptr_t)((uint32_t)iVar11 + 0x6053a8u);
        RESTORE_CRT_MB_LOCALE = locale;
        RESTORE_CRT_MB_SYMBOLS[2] = *(volatile const uint32_t *)(uintptr_t)((uint32_t)iVar11 + 0x6053acu);
        goto LAB_005699b9;
      }
      pUVar5 = pUVar5 + 0xc;
      iVar11 = iVar11 + 1;
    } while ((int)pUVar5 < 0x605490);
    BVar6 = GetCPInfo(CodePage,&local_1c);
    if (BVar6 == 1) {
      puVar13 = (volatile uint32_t *)RESTORE_CRT_MB_FLAGS;
      RESTORE_CRT_CODEPAGE = CodePage;
      for (iVar11 = 0x40; iVar11 != 0; iVar11 = iVar11 + -1) {
        *puVar13 = 0;
        puVar13 = puVar13 + 1;
      }
      *(volatile uint8_t *)puVar13 = 0;
      RESTORE_CRT_MB_LOCALE = 0;
      if (local_1c.MaxCharSize < 2) {
        RESTORE_CRT_MB_ENABLED = 0;
      }
      else {
        if (local_1c.LeadByte[0] != '\0') {
          pBVar8 = local_1c.LeadByte + 1;
          do {
            bVar3 = *pBVar8;
            if (bVar3 == 0) break;
            for (uVar7 = (uint)pBVar8[-1]; uVar7 <= bVar3; uVar7 = uVar7 + 1) {
              RESTORE_CRT_MB_FLAGS[uVar7 + 1u] |= 4u;
            }
            pBVar1 = pBVar8 + 1;
            pBVar8 = pBVar8 + 2;
          } while (*pBVar1 != 0);
        }
        uVar7 = 1;
        do {
          RESTORE_CRT_MB_FLAGS[uVar7 + 1u] |= 8u;
          uVar7 = uVar7 + 1;
        } while (uVar7 < 0xff);
        RESTORE_CRT_MB_LOCALE = FUN_00569a14((int32_t)CodePage);
        RESTORE_CRT_MB_ENABLED = 1;
      }
      RESTORE_CRT_MB_SYMBOLS[0] = 0;
      RESTORE_CRT_MB_SYMBOLS[1] = 0;
      RESTORE_CRT_MB_SYMBOLS[2] = 0;
      goto LAB_005699b9;
    }
    if (RESTORE_CRT_DEFAULT_CODEPAGE == 0) {
      return 0xffffffff;
    }
  }
  FUN_00569a47();
LAB_005699b9: ;
  FUN_00569a70();
  return 0;
}

uint32_t FUN_00569831(int32_t codepage)
{
  return restore_crt_set_multibyte_codepage(codepage);
}
