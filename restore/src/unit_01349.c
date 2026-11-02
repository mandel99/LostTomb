#include "restore_types.h"
#define DAT_00bcba54 (*(undefined4 *)0x00bcba54u)
#define DAT_00bcd5ec (*(undefined4 *)0x00bcd5ecu)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern int FUN_004ea9c0();

int FUN_004999e0(int param_1,int param_2,int param_3,int param_4)

{
  byte bVar1;
  short sVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  byte *pbVar7;
  
  iVar6 = param_4;
  param_4 = 0;
  sVar2 = (&DAT_00bcd658)[iVar6 * 0xc2];
  DAT_00bcd5ec = 10000;
  iVar6 = 1;
  pbVar7 = &DAT_00bcd786;
  do {
    if ((pbVar7[4] == 1) && (pbVar7[0x2e] != 4)) {
      bVar1 = *pbVar7;
      if (((bVar1 < 0x2b) || (0x39 < bVar1)) &&
         ((bVar1 != 99 && (cVar3 = FUN_004ea9c0(bVar1), cVar3 == '\0')))) {
        if ((*pbVar7 != 0x29) || (pbVar7[0x2e] != 0xb)) goto LAB_00499b26;
        iVar4 = (int)*(short *)(pbVar7 + 10);
        iVar5 = (int)*(short *)(pbVar7 + 0xc);
        if (iVar4 < param_1) {
          DAT_00d3b4fc = param_1 - iVar4;
        }
        else if (param_1 < iVar4) {
          DAT_00d3b4fc = iVar4 - param_1;
        }
        else {
          DAT_00d3b4fc = 0;
        }
        if (iVar5 < param_2) {
          iVar5 = param_2 - iVar5;
        }
        else if (param_2 < iVar5) {
          iVar5 = iVar5 - param_2;
        }
        else {
          iVar5 = 0;
        }
        _DAT_00d3b4f8 = iVar5;
        if (iVar5 <= DAT_00d3b4fc) goto LAB_00499ae8;
      }
      else {
        iVar4 = (int)*(short *)(pbVar7 + 10);
        iVar5 = (int)*(short *)(pbVar7 + 0xc);
        if (iVar4 < param_1) {
          DAT_00d3b4fc = param_1 - iVar4;
        }
        else if (param_1 < iVar4) {
          DAT_00d3b4fc = iVar4 - param_1;
        }
        else {
          DAT_00d3b4fc = 0;
        }
        if (iVar5 < param_2) {
          iVar5 = param_2 - iVar5;
        }
        else if (param_2 < iVar5) {
          iVar5 = iVar5 - param_2;
        }
        else {
          iVar5 = 0;
        }
        _DAT_00d3b4f8 = iVar5;
        if (iVar5 <= DAT_00d3b4fc) {
LAB_00499ae8: ;
          iVar5 = DAT_00d3b4fc;
          DAT_00d3b4fc = iVar5;
        }
      }
      if (((*(int *)(&DAT_00bcba54 + sVar2 * 0x90) < 1) ||
          ((int)*(short *)(pbVar7 + 0x56) == *(int *)(&DAT_00bcba54 + sVar2 * 0x90))) &&
         (iVar5 <= param_3)) {
        if (*(short *)(pbVar7 + 0x7a) != 0) {
          iVar5 = iVar5 * 2;
        }
        if (iVar5 < DAT_00bcd5ec) {
          DAT_00bcd5ec = iVar5;
          param_4 = iVar6;
        }
      }
    }
LAB_00499b26: ;
    pbVar7 = pbVar7 + 0x184;
    iVar6 = iVar6 + 1;
    if (0xc8ad41 < (int)pbVar7) {
      return param_4;
    }
  } while( true );
}
