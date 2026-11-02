#include "restore_types.h"
#define DAT_0078d788 (*(undefined *)0x0078d788u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007900bc (*(undefined4 *)0x007900bcu)
#define DAT_007900c0 (*(undefined4 *)0x007900c0u)
#define DAT_00790180 (*(undefined4 *)0x00790180u)
#define DAT_00790184 (*(undefined4 *)0x00790184u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
extern int FUN_0046b100();

void FUN_0046b320(void)

{
  byte bVar1;
  short sVar2;
  undefined1 uVar3;
  undefined2 *puVar4;
  char *pcVar5;
  short *psVar6;
  int iVar7;
  int iVar8;
  
  puVar4 = &DAT_00790180;
  do {
    *puVar4 = 0;
    iVar7 = DAT_0078d790;
    puVar4 = puVar4 + 0x62;
  } while ((int)puVar4 < 0x7999dc);
  if (0 < DAT_0078d790) {
    psVar6 = &DAT_00799af0;
    iVar8 = DAT_0078d790;
    do {
      if (((char)psVar6[-8] != '\0') &&
         (((((sVar2 = *psVar6, sVar2 == 0x47 || (sVar2 == 0x48)) || (sVar2 == 0x46)) ||
           (((sVar2 == 0xbf || (sVar2 == 0xc1)) || (sVar2 == 0xbe)))) &&
          (*(byte *)((int)psVar6 + 0xb1) != 0)))) {
        (&DAT_007900c0)[(uint)*(byte *)((int)psVar6 + 0xb1) * 0xc4] = 1;
      }
      psVar6 = psVar6 + 0x84;
      iVar8 = iVar8 + -1;
    } while (iVar8 != 0);
  }
  iVar8 = 1;
  if (0 < iVar7) {
    psVar6 = &DAT_00799af0;
    do {
      if (((char)psVar6[-8] != '\0') &&
         (((sVar2 = *psVar6, sVar2 == 0x47 || (sVar2 == 0x48)) ||
          ((sVar2 == 0x46 || (((sVar2 == 0xbf || (sVar2 == 0xc1)) || (sVar2 == 0xbe)))))))) {
        bVar1 = *(byte *)((int)psVar6 + 0xb1);
        if (bVar1 == 0) {
          uVar3 = FUN_0046b100(iVar8);
          *(undefined1 *)((int)psVar6 + 0xb1) = uVar3;
        }
        else if (*(short *)(&DAT_007900bc + (uint)bVar1 * 0x31) == 0) {
          *(short *)(&DAT_007900bc + (uint)bVar1 * 0x31) = (short)iVar8;
        }
        else {
          uVar3 = FUN_0046b100(iVar8);
          *(undefined1 *)((int)psVar6 + 0xb1) = uVar3;
        }
      }
      iVar8 = iVar8 + 1;
      psVar6 = psVar6 + 0x84;
    } while (iVar8 <= DAT_0078d790);
  }
  DAT_0078d788 = 0;
  iVar7 = 1;
  pcVar5 = &DAT_00790184;
  do {
    if (*pcVar5 != '\0') {
      DAT_0078d788 = iVar7;
    }
    pcVar5 = pcVar5 + 0xc4;
    iVar7 = iVar7 + 1;
  } while ((int)pcVar5 < 0x7999e0);
  return;
}
