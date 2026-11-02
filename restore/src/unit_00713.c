#include "restore_types.h"
#define DAT_007770f0 (*(undefined4 *)0x007770f0u)
extern int FUN_00447430();
extern undefined4 FUN_0044c110();

uint FUN_00449750(int *param_1,int *param_2,undefined1 *param_3,undefined4 *param_4)

{
  int *piVar1;
  int *piVar2;
  undefined1 *puVar3;
  uint3 uVar6;
  int iVar4;
  uint uVar5;
  int in_ECX;
  
  puVar3 = param_3;
  piVar2 = param_2;
  piVar1 = param_1;
  *param_1 = -1;
  *param_2 = 0;
  *param_3 = 0;
  *param_4 = 0;
  uVar6 = (uint3)((uint)param_4 >> 8);
  if (*(char *)(in_ECX + 2) != '\x01') {
    return (uint)uVar6 << 8;
  }
  uVar5 = CONCAT31(uVar6,*(char *)(in_ECX + 0x32));
  if (*(char *)(in_ECX + 0x32) == '\0') {
    return (uint)uVar6 << 8;
  }
  if (*(short *)(in_ECX + 4) != 0x25) {
LAB_004497a3: ;
    return uVar5 & 0xffffff00;
  }
  FUN_00447430((int)*(short *)(in_ECX + 0x1c),&param_1);
  *piVar1 = (int)*(short *)(in_ECX + 0x1c);
  iVar4 = FUN_0044c110();
  if (iVar4 < 1) {
    iVar4 = 1;
  }
  uVar5 = (uint)*(short *)(in_ECX + 0x30);
  if (uVar5 == 5) {
    *piVar2 = 100 - (*(short *)(in_ECX + 0x2e) * 100) / iVar4;
  }
  else {
    if (((int)uVar5 < 6) || (7 < (int)uVar5)) goto LAB_004497a3;
    *piVar2 = (*(short *)(in_ECX + 0x2e) * 100) / iVar4;
  }
  if (*piVar2 < 0) {
    *piVar2 = 0;
  }
  else if (100 < *piVar2) {
    *piVar2 = 100;
  }
  if ((char)param_1 != '\0') {
    *piVar2 = 100 - *piVar2;
  }
  iVar4 = *piVar1;
  *puVar3 = (&DAT_007770f0)[iVar4 * 0x144] == '\x01';
  *param_4 = 0;
  return CONCAT31((int3)((uint)(iVar4 * 0x51) >> 8),1);
}
