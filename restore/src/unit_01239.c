#include "restore_types.h"
#define DAT_006059e8 (*(undefined *)0x006059e8u)
#define DAT_00799ae0 (*(undefined4 *)0x00799ae0u)
#define DAT_009ab9ec (*(undefined4 *)0x009ab9ecu)
#define DAT_009b84fc (*(undefined4 *)0x009b84fcu)
#define DAT_009c500c (*(undefined4 *)0x009c500cu)
#define DAT_009d1b1c (*(undefined4 *)0x009d1b1cu)
#define DAT_009de62c (*(undefined4 *)0x009de62cu)
#define DAT_00a1126c (*(undefined4 *)0x00a1126cu)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00ba746c (*(undefined *)0x00ba746cu)
#define DAT_00ba7888 (*(undefined *)0x00ba7888u)
#define DAT_00bc95f0 (*(undefined *)0x00bc95f0u)
#define DAT_00bc95f4 (*(undefined4 *)0x00bc95f4u)
#define DAT_00bc9688 (*(undefined *)0x00bc9688u)
#define DAT_00bc968c (*(undefined *)0x00bc968cu)
#define DAT_00bc9690 (*(undefined *)0x00bc9690u)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)
#define _DAT_00bc9684 (*(undefined4 *)0x00bc9684u)
extern void FUN_0040f860();
extern void FUN_0040f8f0();

undefined4 FUN_00484f60(void)

{
  char *pcVar1;
  int iVar2;
  undefined4 *puVar3;
  
  puVar3 = &DAT_00bc95f4;
  for (iVar2 = 0x24; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  DAT_00bc968c = 0;
  DAT_00bc9694 = 1;
  DAT_00bc9688 = 0;
  _DAT_00bc9684 = 0;
  DAT_00bc95f0 = 0;
  DAT_00bc9690 = DAT_006059e8;
  FUN_0040f8f0(&DAT_00ba7888,0x21d68,0);
  FUN_0040f8f0(&DAT_00ba746c,0x41a,0);
  pcVar1 = &DAT_00799ae0;
  do {
    if (*pcVar1 == '\x02') {
      DAT_00bc9694 = 0;
      return 0;
    }
    if (*pcVar1 == '\x06') {
      DAT_00bc9694 = 0;
    }
    pcVar1 = pcVar1 + 0x108;
  } while ((int)pcVar1 < 0x89b6d8);
  FUN_0040f860(&DAT_00b7482c,&DAT_00a1126c,0x32c40);
  FUN_0040f860(&DAT_00af598c,&DAT_009de62c,0x32c40);
  FUN_0040f860(&DAT_00ae8e7c,&DAT_009d1b1c,0xcb10);
  FUN_0040f860(&DAT_00b285cc,&DAT_009b84fc,0xcb10);
  FUN_0040f860(&DAT_00b67d1c,&DAT_009ab9ec,0xcb10);
  FUN_0040f860(&DAT_00adc36c,&DAT_009c500c,0xcb10);
  return 1;
}
