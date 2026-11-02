#include "restore_types.h"
#define DAT_005d06ec (*(undefined4 *)0x005d06ecu)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00ea43a4 (*(undefined4 *)0x00ea43a4u)
#define DAT_00ea4448 (*(undefined4 *)0x00ea4448u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00488050();
extern int FUN_004cde60();

void FUN_004595a0(void)

{
  int iVar1;
  char cVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  
  FUN_004cde60(100,*(undefined4 *)(&DAT_00ea4448 + DAT_00eaac28 * 0x49d8));
  iVar5 = 1;
  if (0 < DAT_0078d790) {
    psVar6 = &DAT_00799af0;
    do {
      if ((char)psVar6[-8] == '\x01') {
        iVar1 = *(int *)(&DAT_005d06ec + *psVar6 * 0x14);
        if (iVar1 == 0) {
          cVar2 = FUN_00488050(iVar5);
          if (cVar2 == '\0') goto LAB_00459610;
        }
        else if (iVar1 == 3) {
          if (psVar6[0x14] == 0) {
LAB_00459610: ;
            iVar4 = DAT_00eaac28 * 0x49d8;
            psVar6[5] = 0;
            if (psVar6[4] != 0) {
              sVar3 = FUN_004cde60(psVar6[4] * 100,
                                   *(undefined4 *)(&DAT_00ea43a4 + iVar1 * 0x14 + iVar4));
              psVar6[5] = sVar3;
            }
          }
        }
        else if (-1 < iVar1) goto LAB_00459610;
      }
      iVar5 = iVar5 + 1;
      psVar6 = psVar6 + 0x84;
    } while (iVar5 <= DAT_0078d790);
  }
  return;
}
