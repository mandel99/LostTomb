#include "restore_types.h"
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00f53f68 (*(undefined *)0x00f53f68u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define s_Network_info__F2__005f8f28 (*(char (*)[18])0x005f8f28u)
extern void FUN_004cc720();
extern void FUN_00536dc0();

void FUN_00536620(void)

{
  int iVar1;
  
  if (DAT_00f53f68 != 0) {
    iVar1 = DAT_00e38da4 * 2;
    FUN_00536dc0(6,0,iVar1 + 0xdc,0xc,0xf,1);
    FUN_004cc720(s_Network_info__F2__005f8f28,0,iVar1 + 0xe2,0xc0,DAT_00f56f8c,0xffff);
  }
  return;
}
