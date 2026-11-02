#include "restore_types.h"
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc96c8 (*(undefined4 *)0x00bc96c8u)
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)

void FUN_0047fb70(void)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  char cVar4;
  int iVar5;
  byte *pbVar6;
  int iVar7;
  
  iVar2 = DAT_00bca2d4;
  iVar1 = DAT_00bc96c8;
  iVar7 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      if ((int)(&DAT_00bc9f24)[iVar7] < (int)(&DAT_00bc9b94)[iVar7]) {
        iVar5 = (&DAT_00bc9b94)[iVar7] - (&DAT_00bc9f24)[iVar7];
        pcVar3 = &DAT_00adc36c + (&DAT_00bc9804)[iVar7];
        pbVar6 = (byte *)(&DAT_00af598c + (&DAT_00bc9804)[iVar7]);
        do {
          if (((*pbVar6 & 4) != 0) && (cVar4 = *pcVar3, cVar4 != '\0')) {
            if (cVar4 == '\x01') {
              cVar4 = (-(iVar1 != 1) & 0xfeU) + 6;
            }
            else {
              if (cVar4 == '\x02') {
                *pcVar3 = '\x05';
                goto LAB_0047fc26;
              }
              if (cVar4 == '\x03') {
                cVar4 = (-(iVar1 != 1) & 2U) + 4;
              }
              else {
                if (cVar4 != '\x04') {
                  if (cVar4 == '\x05') {
                    *pcVar3 = '\x02';
                  }
                  else if (cVar4 == '\x06') {
                    cVar4 = (-(iVar1 != 1) & 0xfeU) + 3;
                    goto LAB_0047fc24;
                  }
                  goto LAB_0047fc26;
                }
                cVar4 = (-(iVar1 != 1) & 2U) + 1;
              }
            }
LAB_0047fc24: ;
            *pcVar3 = cVar4;
          }
LAB_0047fc26: ;
          pbVar6 = pbVar6 + 4;
          pcVar3 = pcVar3 + 1;
          iVar5 = iVar5 + -1;
        } while (iVar5 != 0);
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < iVar2);
  }
  return;
}
