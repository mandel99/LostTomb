#include "restore_types.h"
#include "city_prepare.h"
#include "mission_loaders.h"
#define DAT_0077de6f (*(undefined4 *)0x0077de6fu)
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_00785d74 (*(undefined4 *)0x00785d74u)
#define DAT_00a43eac (*(undefined4 *)0x00a43eacu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2c0 (*(undefined *)0x00bca2c0u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca30c (*(char *)0x00bca30cu)
#define DAT_00bcb9b0 (*(undefined *)0x00bcb9b0u)
#define DAT_00e945c0 (*(undefined *)0x00e945c0u)
#define DAT_00ea6010 (*(undefined4 *)0x00ea6010u)
#define DAT_00ea6014 (*(undefined4 *)0x00ea6014u)
#define DAT_00ea6082 (*(undefined4 *)0x00ea6082u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_0040f8f0();
extern void FUN_00442660();
extern void FUN_004426c0();
extern void FUN_00448a90();
extern int FUN_0046f190();
extern void FUN_004771c0();
extern void FUN_004860d0();
extern void FUN_00486e70();

extern void FUN_004d4040();
extern void FUN_004ddf90();
extern void FUN_004e59a0();
extern void FUN_0051f780();

void FUN_004d5980(void)

{
  short sVar1;
  char *pcVar2;
  undefined1 *puVar3;
  int iVar4;
  short *psVar5;
  undefined4 *puVar6;
  
  FUN_0040f8f0(&DAT_00b7482c,0x32c40,0);
  FUN_004771c0();
  FUN_004bd360(RESTORE_LOAD_SETTINGS);
  if (DAT_00e945c0 < 0x91) {
    DAT_00bca30c = 1;
    FUN_00486e70();
  }
  puVar6 = &DAT_00a43eac;
  for (iVar4 = 0x32c4; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar6 = 0xffffffff;
    puVar6 = puVar6 + 1;
  }
  FUN_0051f780(0,0,DAT_00bca2d0,DAT_00bca2c0);
  FUN_004860d0();
  FUN_004be160(RESTORE_CITY_WORLD,0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  iVar4 = 0;
  if (0 < DAT_00785d70) {
    do {
      FUN_00448a90();
      iVar4 = iVar4 + 1;
    } while (iVar4 < DAT_00785d70);
  }
  sVar1 = 0;
  psVar5 = &DAT_00785d74;
  iVar4 = 0x96;
  do {
    *psVar5 = sVar1;
    sVar1 = sVar1 + 1;
    psVar5 = psVar5 + 0x3e;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  FUN_004e59a0();
  iVar4 = DAT_00eaac28 * 0x49d8;
  *(undefined2 *)((int)&DAT_00ea6082 + iVar4 + 2) = 0;
  *(undefined4 *)(&DAT_00ea6010 + iVar4) = 0xffffffff;
  *(undefined4 *)(&DAT_00ea6014 + iVar4) = 0xffffffff;
  FUN_00442660();
  FUN_004426c0();
  pcVar2 = &DAT_0077f748;
  do {
    if ((pcVar2[1] != '\0') && (*pcVar2 == '\x01')) {
      pcVar2[0x1b] = '\0';
    }
    pcVar2 = pcVar2 + 0x62;
  } while ((int)pcVar2 < 0x7843d8);
  puVar3 = &DAT_0077de6f;
  do {
    *puVar3 = 0;
    puVar3 = puVar3 + 0x6a;
  } while ((int)puVar3 < 0x77f747);
  DAT_00e945c0 = 0xa0;
  FUN_004ddf90();
  DAT_00bcb9b0 = FUN_0046f190();
  FUN_004d4040();
  return;
}
