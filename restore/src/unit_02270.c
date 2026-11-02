#include "restore_types.h"
#define DAT_006fdc60 (*(undefined4 *)0x006fdc60u)
#define DAT_00e38dc8 (*(char *)0x00e38dc8u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00eadce8 (*(undefined4 *)0x00eadce8u)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define PTR_DAT_005f78f0 (*(unsigned char * *)0x005f78f0u)
extern int FUN_00425990();
extern void FUN_00425bc0();

void FUN_00515c60(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar4 = DAT_00eadd84 * 0x10 + -0x28;
  iVar3 = 0;
  iVar5 = 0;
  DAT_00e38dc9 = 1;
  iVar2 = 0x28;
  do {
    if ((*(int *)((int)&DAT_00eadce8 + iVar5) != 0) && ((&DAT_006fdc60)[iVar2] != '\0')) {
      FUN_00425bc0(DAT_00eadd90 + 0x28 + iVar3,DAT_00eadd8c + iVar4,
                   *(undefined4 *)((int)&PTR_DAT_005f78f0 + iVar5),1,iVar2);
      iVar1 = FUN_00425990(DAT_00eadd90 + 0x28 + iVar3,DAT_00eadd8c + iVar4,
                           *(undefined4 *)((int)&PTR_DAT_005f78f0 + iVar5),1);
      if (iVar1 != 0) {
        return;
      }
      iVar3 = iVar3 + 0x1b;
    }
    iVar5 = iVar5 + 4;
    iVar1 = iVar2 + -0x27;
    iVar2 = iVar2 + 1;
  } while (iVar1 < 0xf);
  DAT_00e38dc8 = 0;
  return;
}
