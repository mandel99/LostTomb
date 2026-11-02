#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d844 (*(undefined4 *)0x0078d844u)
#define DAT_0078d8d4 (*(undefined4 *)0x0078d8d4u)
#define DAT_0078d9a0 (*(undefined *)0x0078d9a0u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)

void FUN_0045e040(void)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  short *psVar7;
  undefined2 *local_4;
  
  iVar4 = 1;
  puVar6 = (undefined4 *)&DAT_0078d8d4;
  for (iVar3 = 0x32; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar6 = 0;
    puVar6 = puVar6 + 1;
  }
  puVar6 = &DAT_0078d844;
  for (iVar3 = 0x24; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar6 = 0;
    puVar6 = puVar6 + 1;
  }
  DAT_0078d9a0 = 0;
  if (0 < DAT_0078d790) {
    iVar3 = 0x84;
    local_4 = &DAT_0078d8d4;
    psVar7 = &DAT_00799af0;
    do {
      if ((((char)psVar7[-8] == '\x01') &&
          (((*psVar7 == 0x47 || (*psVar7 == 0xbf)) && ((char)psVar7[0x1a] != '\0')))) &&
         (0 < psVar7[8])) {
        iVar5 = 0;
        bVar1 = *(byte *)((int)psVar7 + 0xb1);
        iVar2 = 0;
        while (iVar2 = iVar2 + 1, iVar2 < 9) {
          while (iVar2 < 1) {
            iVar2 = iVar2 + 1;
            if (8 < iVar2) goto LAB_0045e0fa;
          }
          if (*(char *)((int)&DAT_007900c2 + (uint)bVar1 * 0xc4 + iVar2) != '\x02') {
            iVar5 = iVar5 + (short)(&DAT_00799a2e)[iVar2 + iVar3];
            (&DAT_0078d844)[iVar2] =
                 (&DAT_0078d844)[iVar2] + (int)(short)(&DAT_00799a2e)[iVar2 + iVar3];
          }
        }
LAB_0045e0fa: ;
        if ((100 < iVar5) && (*local_4 = (short)iVar4, (int)local_4 < 0x78d998)) {
          DAT_0078d9a0 = DAT_0078d9a0 + 1;
          local_4 = local_4 + 1;
        }
      }
      iVar4 = iVar4 + 1;
      psVar7 = psVar7 + 0x84;
      iVar3 = iVar3 + 0x84;
    } while (iVar4 <= DAT_0078d790);
  }
  return;
}
