#include "restore_types.h"
#define DAT_005d06ec (*(undefined4 *)0x005d06ecu)
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af8 (*(undefined4 *)0x00799af8u)
#define DAT_00ea439c (*(undefined4 *)0x00ea439cu)
#define DAT_00ea43a4 (*(undefined4 *)0x00ea43a4u)
#define DAT_00ea43a8 (*(undefined4 *)0x00ea43a8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00459240();
extern undefined4 FUN_00488050();

void FUN_00459100(void)

{
  char cVar1;
  undefined4 *puVar2;
  int iVar3;
  short sVar4;
  int iVar5;
  int iVar6;
  short *psVar7;
  
  puVar2 = (undefined4 *)(&DAT_00ea43a4 + DAT_00eaac28 * 0x49d8);
  iVar5 = 10;
  do {
    puVar2[1] = 0;
    *puVar2 = 0;
    puVar2[-1] = 0;
    puVar2[-2] = 0;
    puVar2 = puVar2 + 5;
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  iVar5 = 1;
  if (0 < DAT_0078d790) {
    psVar7 = &DAT_00799af8;
    do {
      if ((char)psVar7[-0xc] == '\x01') {
        iVar6 = *(int *)(&DAT_005d06ec + psVar7[-4] * 0x14);
        if ((iVar6 == 0) && (cVar1 = FUN_00488050(iVar5), cVar1 != '\0')) {
          iVar6 = -1;
        }
        *(char *)(psVar7 + 0x15) = (char)iVar6;
        if (-1 < iVar6) {
          if (iVar6 == 3) {
            if (psVar7[0x10] == 0) {
              sVar4 = *psVar7;
joined_r0x004591a4: ;
              if (0 < sVar4) goto LAB_004591d0;
            }
          }
          else {
            if (iVar6 == 2) {
              sVar4 = *psVar7;
              goto joined_r0x004591a4;
            }
            if ((iVar6 == 1) || (iVar6 == 0)) {
              iVar3 = FUN_00459240(iVar5);
              if (iVar3 != 0) goto LAB_0045921c;
              sVar4 = *psVar7;
            }
            else {
              sVar4 = *psVar7;
            }
            if (sVar4 < 1) goto LAB_0045921c;
LAB_004591d0: ;
            iVar3 = DAT_00eaac28 * 0x49d8 + iVar6 * 0x14;
            (&DAT_00ea439c)[iVar6 * 5 + DAT_00eaac28 * 0x1276] =
                 (&DAT_00ea439c)[iVar6 * 5 + DAT_00eaac28 * 0x1276] +
                 *(int *)(&DAT_00705eb8 + psVar7[-4] * 0x28);
            *(int *)(&DAT_00ea43a4 + iVar3) = *(int *)(&DAT_00ea43a4 + iVar3) + (int)sVar4;
            *(int *)(&DAT_00ea43a8 + iVar3) = *(int *)(&DAT_00ea43a8 + iVar3) + 1;
          }
        }
      }
LAB_0045921c: ;
      iVar5 = iVar5 + 1;
      psVar7 = psVar7 + 0x84;
    } while (iVar5 <= DAT_0078d790);
  }
  return;
}
