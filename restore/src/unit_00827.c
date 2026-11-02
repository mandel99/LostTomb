#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define _DAT_00784c8c (*(undefined4 *)0x00784c8cu)
#define _DAT_00784c8e (*(undefined4 *)0x00784c8eu)
extern undefined4 FUN_00481420();
extern undefined4 FUN_0051ce90();

void FUN_00457a40(void)

{
  int iVar1;
  short *psVar2;
  int iVar3;
  
  FUN_0051ce90((int)_DAT_00784c8c,(int)_DAT_00784c8e);
  iVar3 = 1;
  if (0 < DAT_0078d790) {
    psVar2 = &DAT_00799af0;
    do {
      if ((((char)psVar2[-8] == '\x01') && (*(char *)((int)psVar2 + -0xb) == '\0')) &&
         ((*psVar2 == 0x4b || (*psVar2 == 0xc0)))) {
        iVar1 = FUN_00481420((int)psVar2[-5],(int)psVar2[-4],3);
        if (iVar1 == 0) {
          *(undefined1 *)((int)psVar2 + 0x25) = 0;
        }
        else {
          *(undefined1 *)((int)psVar2 + 0x25) = 1;
        }
      }
      iVar3 = iVar3 + 1;
      psVar2 = psVar2 + 0x84;
    } while (iVar3 <= DAT_0078d790);
  }
  return;
}
