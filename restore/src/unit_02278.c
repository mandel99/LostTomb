#include "restore_types.h"
#define DAT_005f8078 (*(undefined4 *)0x005f8078u)
#define DAT_005f8094 (*(undefined4 *)0x005f8094u)
#define DAT_005f80b0 (*(undefined4 *)0x005f80b0u)
#define DAT_005f80cc (*(undefined4 *)0x005f80ccu)
#define DAT_007056c8 (*(int32_t *)0x007056c8u)
#define DAT_00eadfac (*(int32_t *)0x00eadfacu)
#define DAT_00eadfb0 (*(int32_t *)0x00eadfb0u)
#define DAT_00eadfb8 (*(int32_t *)0x00eadfb8u)
#define DAT_00eadfc8 (*(int32_t *)0x00eadfc8u)
#define DAT_00eadfcc (*(int32_t *)0x00eadfccu)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
#define DAT_00f56f94 (*(int32_t *)0x00f56f94u)
#define DAT_00f56f9c (*(int32_t *)0x00f56f9cu)
#define PTR_DAT_005f7f28 (*(unsigned char * *)0x005f7f28u)
extern void FUN_004cd1a0();
extern void FUN_004cd240();
extern void FUN_004cd390();

void FUN_00516720(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined **ppuVar4;
  int iVar5;
  
  iVar5 = 1;
  ppuVar4 = &PTR_DAT_005f7f28;
  do {
    iVar1 = DAT_00eadfc8 + -1 + iVar5;
    if (((-1 < iVar1) && (iVar1 < DAT_00eadfb0)) &&
       (iVar2 = *(int *)(DAT_00eadfac + iVar1 * 4) + DAT_00eadfb8, iVar2 != 0)) {
      uVar3 = DAT_00f56f9c;
      if ((DAT_007056c8 != iVar5) && (iVar1 != DAT_00eadfcc)) {
        uVar3 = DAT_00f56f94;
      }
      FUN_004cd390(iVar2,ppuVar4,uVar3,0,0,0);
    }
    uVar3 = DAT_00f56f8c;
    ppuVar4 = ppuVar4 + 7;
    iVar5 = iVar5 + 1;
  } while ((int)ppuVar4 < 0x5f805d);
  FUN_004cd1a0(1,&DAT_005f8078,DAT_007056c8 == 0xd,0,0);
  FUN_004cd240(0x124,0,&DAT_005f8078,uVar3,0,4,7);
  FUN_004cd1a0(1,&DAT_005f8094,DAT_007056c8 == 0xe,0,0);
  FUN_004cd240(0x124,1,&DAT_005f8094,uVar3,0,4,7);
  FUN_004cd1a0(1,&DAT_005f80b0,DAT_007056c8 == 0xf,0,0);
  FUN_004cd240(0x124,2,&DAT_005f80b0,uVar3,0,4,7);
  FUN_004cd1a0(1,&DAT_005f80cc,DAT_007056c8 == 0x10,0,0);
  FUN_004cd240(0x124,4,&DAT_005f80cc,uVar3,0,4,7);
  return;
}
