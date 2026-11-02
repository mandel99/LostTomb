#include "restore_types.h"
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_0077de02 (*(undefined4 *)0x0077de02u)
#define DAT_00799b20 (*(undefined4 *)0x00799b20u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_00488030();
extern undefined4 FUN_00488050();
extern int FUN_00488140();
extern int FUN_00489880();
extern undefined4 FUN_004c5340();
extern int FUN_004cde40();
extern longlong FUN_0056133c();

int FUN_00447530(void)

{
  undefined4 uVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  char *pcVar6;
  int local_14;
  
  local_14 = 0;
  iVar4 = 1;
  psVar5 = &DAT_00799b20;
  do {
    if ((((char)psVar5[-0x20] == '\x01') &&
        ((cVar2 = FUN_00488030(iVar4), cVar2 != '\0' || (psVar5[-0x18] == 0xc2)))) &&
       ((cVar2 = FUN_00488030(iVar4), cVar2 == '\0' || (cVar2 = FUN_00489880(iVar4), cVar2 != '\0'))
       )) {
      cVar2 = FUN_00488050(iVar4);
      if (cVar2 == '\0') {
        cVar2 = FUN_00488030(iVar4);
        if (cVar2 == '\0') {
          if ((0 < *psVar5) && (0 < psVar5[10])) {
            local_14 = local_14 +
                       ((int)(*(int *)(&DAT_00705eb8 + psVar5[-0x18] * 0x28) * 0x10 +
                             (*(int *)(&DAT_00705eb8 + psVar5[-0x18] * 0x28) * 0x10 >> 0x1f & 3U))
                       >> 2);
          }
        }
        else {
          FUN_00488140(iVar4,1);
        }
      }
      else {
        FUN_00488140(iVar4,1);
      }
    }
    psVar5 = psVar5 + 0x84;
    iVar4 = iVar4 + 1;
  } while ((int)psVar5 < 0x89b718);
  FUN_004cde40(local_14,0x50);
  uVar1 = DAT_00eaac28;
  pcVar6 = &DAT_0077de02;
  do {
    if (((pcVar6[-2] != '\0') &&
        (((cVar2 = *pcVar6, cVar2 == '\x01' || (cVar2 == '\x03')) || (cVar2 == '\x05')))) &&
       (pcVar6[3] != '\0')) {
      iVar4 = 0;
      while (iVar4 = iVar4 + 1, iVar4 < 9) {
        while (iVar4 < 1) {
          iVar4 = iVar4 + 1;
          if (8 < iVar4) goto LAB_00447724;
        }
      }
    }
LAB_00447724: ;
    pcVar6 = pcVar6 + 0x6a;
    if (0x77f743 < (int)pcVar6) {
      iVar4 = FUN_0056133c();
      iVar3 = FUN_004c5340(uVar1);
      return (iVar4 * 100) / iVar3;
    }
  } while( true );
}
