#include "restore_types.h"
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00ea6204 (*(undefined4 *)0x00ea6204u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
extern int FUN_00488030();
extern undefined4 FUN_00488050();
extern int FUN_00488140();
extern int FUN_004883d0();
extern void FUN_004e2970();

void FUN_00489510(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  int iVar5;
  int local_8;
  int local_4;
  
  local_8 = 0;
  local_4 = 0;
  iVar5 = 1;
  psVar4 = &DAT_00799af0;
  do {
    if ((char)psVar4[-8] == '\x01') {
      cVar1 = FUN_00488050(iVar5);
      if (cVar1 == '\0') {
        if ((*psVar4 == 0x47) || (*psVar4 == 0xbf)) {
          local_4 = local_4 + psVar4[0x23];
        }
      }
      else {
        iVar2 = FUN_00488140(iVar5,1);
        iVar3 = FUN_004883d0(iVar5);
        if ((iVar2 + iVar3) * 8 < 800) {
          iVar2 = 800;
        }
        else {
          iVar2 = FUN_00488140(iVar5,1);
          iVar3 = FUN_004883d0(iVar5);
          iVar2 = (iVar2 + iVar3) * 8;
          if (iVar2 == 0) goto switchD_004895b0_caseD_64;
        }
        cVar1 = FUN_00488030(iVar5);
        if (cVar1 != '\0') {
          switch(*psVar4) {
          case 100: ;
            goto switchD_004895b0_caseD_64;
          }
        }
        cVar1 = FUN_00488030(iVar5);
        if (cVar1 != '\0') {
          switch(*psVar4) {
          case 0x65: ;
            goto switchD_004895b0_caseD_64;
          }
        }
        cVar1 = FUN_00488030(iVar5);
        if (cVar1 != '\0') {
          switch(*psVar4) {
          case 0xe0: ;
            goto switchD_004895b0_caseD_64;
          }
        }
        if (*(int *)(&DAT_00ea6204 + DAT_00eaac28 * 0x49d8) != 0) {
          iVar2 = iVar2 * 2;
        }
        local_8 = local_8 + iVar2;
      }
    }
switchD_004895b0_caseD_64: ;
    psVar4 = psVar4 + 0x84;
    iVar5 = iVar5 + 1;
    if (0x89b6e7 < (int)psVar4) {
      if (0xc80 < local_8 - local_4) {
        FUN_004e2970(0xcd,0,0);
        return;
      }
      if (0x32 < local_8 - local_4) {
        FUN_004e2970(0xcc,0,0);
      }
      return;
    }
  } while( true );
}
