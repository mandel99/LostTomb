#include "restore_types.h"
#define DAT_005be574 (*(undefined4 *)0x005be574u)
#define PTR_DAT_005be510 (*(unsigned char * *)0x005be510u)
#define PTR_DAT_005be540 (*(unsigned char * *)0x005be540u)
#define PTR_DAT_005be570 (*(unsigned char * *)0x005be570u)
#define PTR_DAT_005be5a0 (*(unsigned char * *)0x005be5a0u)
#define PTR_DAT_005be5d0 (*(unsigned char * *)0x005be5d0u)
#define PTR_DAT_005be600 (*(unsigned char * *)0x005be600u)
#define PTR_DAT_005be630 (*(unsigned char * *)0x005be630u)
#define PTR_DAT_005be660 (*(unsigned char * *)0x005be660u)
#define PTR_DAT_005be690 (*(unsigned char * *)0x005be690u)
#define PTR_DAT_005be6c0 (*(unsigned char * *)0x005be6c0u)
#define PTR_DAT_005be6f0 (*(unsigned char * *)0x005be6f0u)
#define PTR_DAT_005be720 (*(unsigned char * *)0x005be720u)
#define PTR_DAT_005be750 (*(unsigned char * *)0x005be750u)
#define PTR_DAT_005be780 (*(unsigned char * *)0x005be780u)
#define PTR_DAT_005be7b0 (*(unsigned char * *)0x005be7b0u)
#define PTR_DAT_005be7e0 (*(unsigned char * *)0x005be7e0u)
#define PTR_DAT_005be810 (*(unsigned char * *)0x005be810u)
#define PTR_DAT_005be840 (*(unsigned char * *)0x005be840u)
#define PTR_DAT_005be870 (*(unsigned char * *)0x005be870u)
#define PTR_DAT_005be8a0 (*(unsigned char * *)0x005be8a0u)
#define PTR_DAT_005be8d0 (*(unsigned char * *)0x005be8d0u)
#define PTR_DAT_005be900 (*(unsigned char * *)0x005be900u)
#define PTR_DAT_005be930 (*(unsigned char * *)0x005be930u)
#define PTR_DAT_005be960 (*(unsigned char * *)0x005be960u)
#define PTR_DAT_005be990 (*(unsigned char * *)0x005be990u)
#include "audio_state.h"

void FUN_004212f0(void)

{
  undefined **ppuVar1;
  int *piVar2;
  undefined4 *puVar3;
  undefined *puVar4;
  undefined **ppuVar5;
  int iVar6;
  
  ppuVar5 = &PTR_DAT_005be510;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be540);
  ppuVar5 = &PTR_DAT_005be540;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be570);
  ppuVar5 = &PTR_DAT_005be570;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be5a0);
  ppuVar5 = &PTR_DAT_005be750;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be780);
  iVar6 = 0;
  do {
    *(undefined2 *)(*(int *)((int)&PTR_DAT_005be780 + iVar6) + 0x12) = 0;
    FUN_0041fca0(*(void **)((uintptr_t)&PTR_DAT_005be780+iVar6),3);
    piVar2 = (int *)((int)&PTR_DAT_005be780 + iVar6);
    puVar3 = (undefined4 *)((int)&DAT_005be574 + iVar6);
    iVar6 = iVar6 + 0x10;
    *(undefined4 *)(*piVar2 + 0xc) = *puVar3;
  } while (iVar6 < 0x30);
  ppuVar5 = &PTR_DAT_005be5a0;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be5d0);
  ppuVar5 = &PTR_DAT_005be5d0;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be600);
  ppuVar5 = &PTR_DAT_005be600;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be630);
  ppuVar5 = &PTR_DAT_005be630;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be660);
  ppuVar5 = &PTR_DAT_005be7b0;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be7e0);
  ppuVar5 = &PTR_DAT_005be7e0;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be810);
  ppuVar5 = &PTR_DAT_005be660;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be690);
  ppuVar5 = &PTR_DAT_005be690;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be6c0);
  ppuVar5 = &PTR_DAT_005be8a0;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be8d0);
  ppuVar5 = &PTR_DAT_005be810;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be840);
  ppuVar5 = &PTR_DAT_005be8d0;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be900);
  ppuVar5 = &PTR_DAT_005be900;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be930);
  ppuVar5 = &PTR_DAT_005be6c0;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be6f0);
  ppuVar5 = &PTR_DAT_005be6f0;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be720);
  ppuVar5 = &PTR_DAT_005be720;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be750);
  ppuVar5 = &PTR_DAT_005be840;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be870);
  ppuVar5 = &PTR_DAT_005be870;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be8a0);
  ppuVar5 = &PTR_DAT_005be930;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be960);
  ppuVar5 = &PTR_DAT_005be960;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be990);
  ppuVar5 = &PTR_DAT_005be990;
  do {
    *(undefined2 *)(*ppuVar5 + 0x12) = 0;
    FUN_0041fca0(*ppuVar5,3);
    puVar4 = *ppuVar5;
    ppuVar1 = ppuVar5 + 1;
    ppuVar5 = ppuVar5 + 4;
    *(undefined **)(puVar4 + 0xc) = *ppuVar1;
  } while ((int)ppuVar5 < 0x5be9c0);
  return;
}
