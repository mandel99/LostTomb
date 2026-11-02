#include "restore_types.h"
#include "city_prepare.h"
#define DAT_005ca658 (*(undefined4 *)0x005ca658u)
#define DAT_005ca958 (*(undefined4 *)0x005ca958u)
#define DAT_005cac58 (*(undefined4 *)0x005cac58u)
#define DAT_005cb9d8 (*(undefined4 *)0x005cb9d8u)
#define DAT_005cbcd8 (*(undefined4 *)0x005cbcd8u)
#define DAT_005cbea8 (*(undefined4 *)0x005cbea8u)
#define DAT_005cc158 (*(undefined4 *)0x005cc158u)
#define DAT_005cc268 (*(undefined4 *)0x005cc268u)
#define DAT_005cc378 (*(undefined4 *)0x005cc378u)
#define DAT_005cc678 (*(undefined4 *)0x005cc678u)
#define DAT_00a43eac (*(undefined4 *)0x00a43eacu)
#define DAT_00a509bc (*(undefined4 *)0x00a509bcu)
#define DAT_00a5d4cc (*(undefined4 *)0x00a5d4ccu)
#define DAT_00a69fdc (*(undefined4 *)0x00a69fdcu)
#define DAT_00a76aec (*(undefined4 *)0x00a76aecu)
#define DAT_00a835fc (*(undefined4 *)0x00a835fcu)
#define DAT_00a9010c (*(undefined4 *)0x00a9010cu)
#define DAT_00a9cc1c (*(undefined4 *)0x00a9cc1cu)
#define DAT_00ab623c (*(undefined4 *)0x00ab623cu)
#define DAT_00ac2d4c (*(undefined4 *)0x00ac2d4cu)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define PTR_DAT_005cbdc8 (*(unsigned char * *)0x005cbdc8u)
extern void FUN_0040f8f0();
extern void FUN_0046fc30();
extern void FUN_0046fc80();
extern void FUN_00475b80();
extern void FUN_00484ee0();


void FUN_0046fa40(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  FUN_0040f8f0(&DAT_00b7482c,0x32c40,0);
  FUN_0040f8f0(&DAT_00b67d1c,0xcb10,0);
  FUN_0040f8f0(&DAT_00b4e6fc,0x19620,0);
  FUN_0040f8f0(&DAT_00af598c,0x32c40,0);
  FUN_0040f8f0(&DAT_00a5d4cc,0xcb10,0);
  FUN_0040f8f0(&DAT_00ae8e7c,0xcb10,0);
  FUN_0040f8f0(&DAT_00b350dc,0x19620,0);
  FUN_0040f8f0(&DAT_00b285cc,0xcb10,0);
  FUN_0040f8f0(&DAT_00adc36c,0xcb10,0);
  FUN_0040f8f0(&DAT_00acf85c,0xcb10,0);
  FUN_0040f8f0(&DAT_00ac2d4c,0xcb10,0);
  FUN_0040f8f0(&DAT_00ab623c,0xcb10,0);
  FUN_0040f8f0(&DAT_00a9cc1c,0x19620,0);
  FUN_0040f8f0(&DAT_00a9010c,0xcb10,0);
  FUN_0040f8f0(&DAT_00a835fc,0xcb10,0);
  FUN_0040f8f0(&DAT_00a76aec,0xcb10,0);
  FUN_0040f8f0(&DAT_00a69fdc,0xcb10,0);
  FUN_0040f8f0(&DAT_00a509bc,0xcb10,0);
  puVar2 = &DAT_00a43eac;
  for (iVar1 = 0x32c4; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0xffffffff;
    puVar2 = puVar2 + 1;
  }
  FUN_004be0f0(RESTORE_CITY_WORLD);
  FUN_00484ee0(&DAT_005ca658,0x30);
  FUN_00484ee0(&DAT_005ca958,0x30);
  FUN_00484ee0(&DAT_005cac58,0x18);
  FUN_00484ee0(&DAT_005cb9d8,0x30);
  FUN_00484ee0(&DAT_005cbcd8,0xf);
  FUN_00484ee0(&PTR_DAT_005cbdc8,0xe);
  FUN_00484ee0(&DAT_005cbea8,0x11);
  FUN_00484ee0(&DAT_005cc158,0x11);
  FUN_00484ee0(&DAT_005cc268,0x11);
  FUN_00484ee0(&DAT_005cc378,0x30);
  FUN_00484ee0(&DAT_005cc678,0x10);
  FUN_0046fc30();
  FUN_0046fc80();
  FUN_00475b80();
  return;
}
