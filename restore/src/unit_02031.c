#include "restore_types.h"
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea1898 (*(undefined4 *)0x00ea1898u)
#define DAT_00ea3e34 (*(undefined4 *)0x00ea3e34u)
#define DAT_00ea3f54 (*(undefined4 *)0x00ea3f54u)
#define DAT_00ea3f70 (*(undefined4 *)0x00ea3f70u)
#define DAT_00ea3f74 (*(undefined4 *)0x00ea3f74u)
#define DAT_00ea3f80 (*(undefined4 *)0x00ea3f80u)
#define DAT_00ea3f84 (*(undefined4 *)0x00ea3f84u)
#define DAT_00ea5f0c (*(undefined4 *)0x00ea5f0cu)

void FUN_004e87a0(int param_1)

{
  short sVar1;
  short *psVar2;
  unsigned char *city;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = param_1 * 0x49d8;
  city = (unsigned char *)(0x00ea0000u + (unsigned int)iVar3);
  *(undefined4 *)(city + 0x3f84) = 0;
  *(undefined4 *)(city + 0x1898) = *(undefined4 *)(city + 0x1894);
  *(undefined4 *)(city + 0x1894) = 0;
  psVar2 = (short *)(city + 0x3e34);
  iVar4 = 100;
  do {
    sVar1 = *psVar2;
    psVar2 = psVar2 + 1;
    iVar5 = *(int *)(city + 0x1894) + (int)sVar1;
    iVar4 = iVar4 + -1;
    *(int *)(city + 0x1894) = iVar5;
  } while (iVar4 != 0);
  if (*(int *)(city + 0x5f0c) < iVar5) {
    *(int *)(city + 0x5f0c) = iVar5;
  }
  *(undefined4 *)(city + 0x3f54) = 0;
  *(int *)(city + 0x3f74) = *(int *)(city + 0x3f74) + *(int *)(city + 0x1894);
  iVar4 = *(int *)(city + 0x3f80);
  *(int *)(city + 0x3f80) = iVar4 + 1;
  *(int *)(city + 0x3f70) = *(int *)(city + 0x3f74) / (iVar4 + 1);
  return;
}
