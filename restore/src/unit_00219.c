#include "restore_types.h"
#define DAT_0057bd88 (*(undefined4 *)0x0057bd88u)
#define DAT_0057be14 (*(undefined4 *)0x0057be14u)
#define DAT_007056c8 (*(int32_t *)0x007056c8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
extern void FUN_004cd240();
extern void FUN_00536710();

void FUN_004180c0(void)

{
  int iVar1;
  undefined4 *puVar2;
  short *psVar3;
  int iVar4;
  
  iVar4 = 0;
  puVar2 = &DAT_0057be14;
  psVar3 = &DAT_0057bd88;
  do {
    iVar4 = iVar4 + 1;
    iVar1 = (int)psVar3[2] - (int)*psVar3;
    FUN_00536710(6,*psVar3 + DAT_00e38da8,psVar3[1] + DAT_00e38da4,
                 (int)(iVar1 + (iVar1 >> 0x1f & 0xfU)) >> 4,DAT_007056c8 == iVar4);
    FUN_004cd240(0x1e,*puVar2,psVar3,DAT_00f56f8c,0,0,6);
    psVar3 = psVar3 + 0xe;
    puVar2 = puVar2 + 1;
  } while ((int)psVar3 < 0x57be14);
  return;
}
