#include "restore_types.h"
#define DAT_00a508d8 (*(undefined4 *)0x00a508d8u)
#define DAT_00a508d9 (*(undefined4 *)0x00a508d9u)
#define DAT_00a509bc (*(undefined4 *)0x00a509bcu)
#define DAT_00a509bd (*(undefined4 *)0x00a509bdu)
#define DAT_00a50aa1 (*(undefined4 *)0x00a50aa1u)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d18 (*(undefined4 *)0x00af5d18u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00af5d20 (*(undefined4 *)0x00af5d20u)
#define DAT_00bc97c0 (*(undefined *)0x00bc97c0u)
#define DAT_00bc97c4 (*(undefined *)0x00bc97c4u)
#define DAT_00bc97c8 (*(undefined *)0x00bc97c8u)
#define DAT_00bc97cc (*(undefined *)0x00bc97ccu)
#define DAT_00bc97d0 (*(undefined *)0x00bc97d0u)
#define DAT_00bc97d4 (*(undefined *)0x00bc97d4u)
#define DAT_00bc97d8 (*(undefined *)0x00bc97d8u)
#define DAT_00bc97dc (*(undefined *)0x00bc97dcu)
#define DAT_00d0571c (*(undefined4 *)0x00d0571cu)
#define DAT_00d05720 (*(undefined4 *)0x00d05720u)
#define DAT_00d05724 (*(undefined4 *)0x00d05724u)
#define DAT_00d05aac (*(undefined4 *)0x00d05aacu)
#define DAT_00d05ab0 (*(undefined4 *)0x00d05ab0u)
#define DAT_00d05ab4 (*(undefined4 *)0x00d05ab4u)
extern int FUN_00484f40();
extern undefined4 FUN_00487260();
extern undefined4 FUN_004872c0();
extern undefined1 FUN_00487300();
extern byte FUN_00487390();

undefined4 FUN_00486f20(int param_1)

{
  char cVar1;
  byte bVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  
  iVar4 = param_1 + -0xe4;
  if (((((*(byte *)(&DAT_00af598c + iVar4) & 4) == 0) || (((&DAT_00af5d1c)[param_1 * 4] & 4) == 0))
      && ((((&DAT_00af5988)[param_1 * 4] & 4) == 0 || (((&DAT_00af5990)[param_1 * 4] & 4) == 0))))
     && (cVar1 = FUN_00487260(param_1), cVar1 == '\0')) {
    bVar2 = (&DAT_00a509bc)[param_1];
    if ((bVar2 < 0x5b) && (0x28 < bVar2)) {
      cVar1 = FUN_004872c0(iVar4,4);
      if ((cVar1 == '\0') && (cVar1 = FUN_00487260(iVar4), cVar1 == '\0')) {
        bVar2 = (&DAT_00a508d8)[param_1];
      }
      else {
        bVar2 = 100;
      }
      if (((*(int *)(&DAT_00d05720 + iVar4 * 4) == 0) ||
          (*(char *)(*(int *)(&DAT_00d05720 + iVar4 * 4) + 0x18) != '\0')) &&
         (((&DAT_00af598c)[iVar4] & 0x40004) == 0)) {
        bVar2 = bVar2 & (0x7f < bVar2) - 1U;
      }
      else {
        bVar2 = 100;
      }
      iVar4 = param_1 + -0xe3;
      DAT_00bc97c0 = (uint)(bVar2 == 100);
      cVar1 = FUN_004872c0(iVar4,4);
      if ((cVar1 == '\0') && (cVar1 = FUN_00487260(iVar4), cVar1 == '\0')) {
        bVar2 = (&DAT_00a508d9)[param_1];
      }
      else {
        bVar2 = 100;
      }
      if (((*(int *)(&DAT_00d05720 + iVar4 * 4) == 0) ||
          (*(char *)(*(int *)(&DAT_00d05720 + iVar4 * 4) + 0x18) != '\0')) &&
         (((&DAT_00af598c)[iVar4] & 0x40004) == 0)) {
        bVar2 = (0x7f < bVar2) - 1U & bVar2;
      }
      else {
        bVar2 = 100;
      }
      DAT_00bc97c4 = (uint)(bVar2 == 100);
      cVar1 = FUN_004872c0(param_1 + 1,4);
      if ((cVar1 == '\0') && (cVar1 = FUN_00487260(param_1 + 1), cVar1 == '\0')) {
        bVar2 = (&DAT_00a509bd)[param_1];
      }
      else {
        bVar2 = 100;
      }
      if (((*(int *)(&DAT_00d05724 + param_1 * 4) == 0) ||
          (*(char *)(*(int *)(&DAT_00d05724 + param_1 * 4) + 0x18) != '\0')) &&
         ((*(uint *)(&DAT_00af5990 + param_1 * 4) & 0x40004) == 0)) {
        bVar2 = (0x7f < bVar2) - 1U & bVar2;
      }
      else {
        bVar2 = 100;
      }
      DAT_00bc97c8 = (uint)(bVar2 == 100);
      cVar1 = FUN_004872c0(param_1 + 0xe5,4);
      if ((cVar1 == '\0') && (cVar1 = FUN_00487260(param_1 + 0xe5), cVar1 == '\0')) {
        bVar2 = (&DAT_00a50aa1)[param_1];
      }
      else {
        bVar2 = 100;
      }
      if (((*(int *)(&DAT_00d05ab4 + param_1 * 4) == 0) ||
          (*(char *)(*(int *)(&DAT_00d05ab4 + param_1 * 4) + 0x18) != '\0')) &&
         ((*(uint *)(&DAT_00af5d20 + param_1 * 4) & 0x40004) == 0)) {
        bVar2 = (0x7f < bVar2) - 1U & bVar2;
      }
      else {
        bVar2 = 100;
      }
      DAT_00bc97cc = (uint)(bVar2 == 100);
      uVar3 = FUN_00487300(param_1 + 0xe4);
      if (((*(int *)(&DAT_00d05ab0 + param_1 * 4) == 0) ||
          (*(char *)(*(int *)(&DAT_00d05ab0 + param_1 * 4) + 0x18) != '\0')) &&
         ((*(uint *)(&DAT_00af5d1c + param_1 * 4) & 0x40004) == 0)) {
        uVar3 = uVar3 & (0x7f < (int)uVar3) - 1;
      }
      else {
        uVar3 = 100;
      }
      DAT_00bc97d0 = (uint)(uVar3 == 100);
      uVar3 = FUN_00487300(param_1 + 0xe3);
      if (((*(int *)(&DAT_00d05aac + param_1 * 4) == 0) ||
          (*(char *)(*(int *)(&DAT_00d05aac + param_1 * 4) + 0x18) != '\0')) &&
         ((*(uint *)(&DAT_00af5d18 + param_1 * 4) & 0x40004) == 0)) {
        uVar3 = uVar3 & (0x7f < (int)uVar3) - 1;
      }
      else {
        uVar3 = 100;
      }
      DAT_00bc97d4 = (uint)(uVar3 == 100);
      uVar3 = FUN_00487300(param_1 + -1);
      if (((*(int *)(&DAT_00d0571c + param_1 * 4) == 0) ||
          (*(char *)(*(int *)(&DAT_00d0571c + param_1 * 4) + 0x18) != '\0')) &&
         ((*(uint *)(&DAT_00af5988 + param_1 * 4) & 0x40004) == 0)) {
        uVar3 = uVar3 & (0x7f < (int)uVar3) - 1;
      }
      else {
        uVar3 = 100;
      }
      DAT_00bc97d8 = (uint)(uVar3 == 100);
      iVar4 = FUN_00487390(param_1,0xffffff1b);
      DAT_00bc97dc = (uint)(iVar4 == 100);
      uVar5 = FUN_00484f40();
      return uVar5;
    }
    if (bVar2 != 100) {
      return 0;
    }
  }
  (&DAT_00a509bc)[param_1] = 100;
  return 0;
}
