#include "restore_types.h"
#define DAT_00604e7c (*(undefined *)0x00604e7cu)
#define DAT_00604f10 (*(undefined *)0x00604f10u)
#define DAT_00604f14 (*(undefined *)0x00604f14u)
#define DAT_00604f18 (*(undefined *)0x00604f18u)
#define DAT_00604f20 (*(undefined *)0x00604f20u)
#define DAT_00604f24 (*(undefined *)0x00604f24u)
#define DAT_00604f28 (*(undefined *)0x00604f28u)
#define DAT_012664b0 (*(undefined *)0x012664b0u)
#define DAT_012664fc (*(undefined *)0x012664fcu)
#define DAT_012664fe (*(undefined *)0x012664feu)
#define DAT_01266500 (*(undefined *)0x01266500u)
#define DAT_01266502 (*(undefined *)0x01266502u)
#define DAT_01266504 (*(undefined *)0x01266504u)
#define DAT_01266506 (*(undefined *)0x01266506u)
#define DAT_01266508 (*(undefined *)0x01266508u)
#define DAT_0126650a (*(undefined *)0x0126650au)
#define DAT_01266550 (*(undefined *)0x01266550u)
#define DAT_01266552 (*(undefined *)0x01266552u)
#define DAT_01266554 (*(undefined *)0x01266554u)
#define DAT_01266556 (*(undefined *)0x01266556u)
#define DAT_01266558 (*(undefined *)0x01266558u)
#define DAT_0126655a (*(undefined *)0x0126655au)
#define DAT_0126655c (*(undefined *)0x0126655cu)
#define DAT_0126655e (*(undefined *)0x0126655eu)
extern void FUN_00566fcd();

bool FUN_00566e21(int *param_1)

{
  int iVar1;
  int iVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  
  if (DAT_00604e7c != 0) {
    iVar2 = param_1[5];
    if ((iVar2 != DAT_00604f10) || (iVar2 != DAT_00604f20)) {
      if (DAT_012664b0 == 0) {
        FUN_00566fcd(1,1,iVar2,4,1,0,0,2,0,0,0);
        FUN_00566fcd(0,1,param_1[5],10,5,0,0,2,0,0,0);
      }
      else {
        if (DAT_01266550 != 0) {
          uVar4 = 0;
          uVar3 = 0;
          uVar5 = DAT_01266556;
        }
        else {
          uVar4 = DAT_01266554;
          uVar3 = DAT_01266556;
          uVar5 = 0;
        }
        FUN_00566fcd(1,DAT_01266550 == 0,iVar2,DAT_01266552,uVar3,uVar4,uVar5,DAT_01266558,
                     DAT_0126655a,DAT_0126655c,DAT_0126655e);
        if (DAT_012664fc != 0) {
          uVar4 = 0;
          uVar3 = 0;
          iVar2 = param_1[5];
          uVar5 = DAT_01266502;
        }
        else {
          iVar2 = param_1[5];
          uVar4 = DAT_01266500;
          uVar3 = DAT_01266502;
          uVar5 = 0;
        }
        FUN_00566fcd(0,DAT_012664fc == 0,iVar2,DAT_012664fe,uVar3,uVar4,uVar5,DAT_01266504,
                     DAT_01266506,DAT_01266508,DAT_0126650a);
      }
    }
    iVar2 = param_1[7];
    if (DAT_00604f14 < DAT_00604f24) {
      if ((DAT_00604f14 <= iVar2) && (iVar2 <= DAT_00604f24)) {
        if ((DAT_00604f14 < iVar2) && (iVar2 < DAT_00604f24)) {
          return true;
        }
LAB_00566f99: ;
        iVar1 = ((param_1[2] * 0x3c + param_1[1]) * 0x3c + *param_1) * 1000;
        if (iVar2 == DAT_00604f14) {
          return DAT_00604f18 <= iVar1;
        }
        return iVar1 < DAT_00604f28;
      }
    }
    else {
      if (iVar2 < DAT_00604f24) {
        return true;
      }
      if (DAT_00604f14 < iVar2) {
        return true;
      }
      if ((iVar2 <= DAT_00604f24) || (DAT_00604f14 <= iVar2)) goto LAB_00566f99;
    }
  }
  return false;
}
