#include "restore_types.h"
#define DAT_005d9a48 (*(undefined4 *)0x005d9a48u)
#define DAT_005d9a90 (*(undefined4 *)0x005d9a90u)
#define DAT_005d9be0 (*(undefined4 *)0x005d9be0u)
#define DAT_005d9bfc (*(undefined4 *)0x005d9bfcu)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_00785d74 (*(undefined4 *)0x00785d74u)
#define DAT_00785d76 (*(undefined4 *)0x00785d76u)
#define DAT_0078a61c (*(undefined *)0x0078a61cu)
#define DAT_0078a620 (*(undefined *)0x0078a620u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define s__2d__s__s_005da168 (*(char (*)[10])0x005da168u)
extern void FUN_00425730();
extern void FUN_004484b0();
extern undefined1 * FUN_00448f20();
extern undefined * FUN_00448f60();
extern void FUN_004cc770();
extern void FUN_004cd1a0();
extern void FUN_004cd240();
extern void FUN_00536820();
extern undefined4 FUN_00561a36();

void FUN_00448640(void)

{
  short *psVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  short *psVar6;
  undefined4 local_68;
  undefined1 local_64 [100];
  
  FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x1b);
  DAT_00e38dc9 = 1;
  FUN_004cd1a0(1,&DAT_005d9be0,DAT_007056c8 == 0xd,0,0);
  FUN_004cd240(0x2c,0x60,&DAT_005d9be0,DAT_00f56f8c,0,4,4);
  FUN_004cd1a0(1,&DAT_005d9bfc,DAT_007056c8 == 0xe,0,0);
  FUN_004cd240(0x2c,0x61,&DAT_005d9bfc,DAT_00f56f8c,0,4,4);
  iVar5 = 1;
  psVar6 = &DAT_005d9a90;
  do {
    iVar2 = DAT_0078a620 + -1 + iVar5;
    if ((-1 < iVar2) && (iVar2 < DAT_00785d70)) {
      psVar1 = &DAT_00785d74 + iVar2 * 0x3e;
      if (psVar1 != (short *)0x0) {
        if (DAT_0078a61c == *psVar1) {
          local_68 = DAT_00f56f9c;
        }
        else {
          local_68 = DAT_00f56f94;
        }
        uVar3 = FUN_00448f20((&DAT_00785d76)[iVar2 * 0x7c]);
        uVar4 = FUN_00448f60();
        FUN_00561a36(local_64,s__2d__s__s_005da168,(int)*psVar1,uVar3,uVar4);
        FUN_004cd1a0(1,psVar6,DAT_007056c8 == iVar5,0,0);
        FUN_004cc770(local_64,*psVar6 + 4 + DAT_00e38da8,psVar6[1] + 4 + DAT_00e38da4,local_68,0);
      }
    }
    psVar6 = psVar6 + 0xe;
    iVar5 = iVar5 + 1;
  } while ((int)psVar6 < 0x5d9bc5);
  FUN_00425730(DAT_00e38da8,DAT_00e38da4,&DAT_005d9a48,2);
  FUN_004484b0();
  return;
}
