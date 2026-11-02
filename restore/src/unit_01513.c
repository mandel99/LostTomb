#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00bcb9f6 (*(undefined4 *)0x00bcb9f6u)
#define DAT_00bcb9f8 (*(undefined4 *)0x00bcb9f8u)
#define DAT_00bcba02 (*(undefined4 *)0x00bcba02u)
#define DAT_00bcba04 (*(undefined4 *)0x00bcba04u)
#define DAT_00bcba06 (*(undefined4 *)0x00bcba06u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define DAT_00ea5e10 (*(undefined4 *)0x00ea5e10u)
#define DAT_00ea5e14 (*(undefined4 *)0x00ea5e14u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern bool FUN_0051b040();

void FUN_004ba690(int param_1)

{
  short sVar1;
  undefined2 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  short *psVar8;
  int local_4;
  
  iVar3 = param_1;
  FUN_0051b040((int)*(short *)(&DAT_00bcb9f6 + param_1 * 0x90),
               (int)*(short *)(&DAT_00bcb9f8 + param_1 * 0x90),0xfffffffe,0xfffffffe);
  local_4 = 0;
  param_1 = 10000;
  iVar7 = 1;
  psVar8 = &DAT_00799af0;
  do {
    if (((((((char)psVar8[-8] == '\x01') && (sVar1 = *psVar8, sVar1 != 0x50)) && (sVar1 != 0x58)) &&
         ((sVar1 != 0x59 && (sVar1 != 0x5d)))) &&
        ((sVar1 != 0x48 && ((sVar1 != 0xc1 && (sVar1 != 0x39)))))) &&
       (0 < (int)(&DAT_00eed760)[*(int *)(psVar8 + -2)])) {
      iVar4 = (int)psVar8[-5];
      _DAT_00d3b4f8 = (&DAT_00ea5e14)[DAT_00eaac28 * 0x1276];
      iVar6 = (&DAT_00ea5e10)[DAT_00eaac28 * 0x1276];
      iVar5 = (int)psVar8[-4];
      if (iVar4 < iVar6) {
        DAT_00d3b4fc = iVar6 - iVar4;
      }
      else if (iVar6 < iVar4) {
        DAT_00d3b4fc = iVar4 - iVar6;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar5 < _DAT_00d3b4f8) {
        _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar5;
      }
      else if (_DAT_00d3b4f8 < iVar5) {
        _DAT_00d3b4f8 = iVar5 - _DAT_00d3b4f8;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar6 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar6 = DAT_00d3b4fc;
      }
      if (iVar6 < param_1) {
        param_1 = iVar6;
        local_4 = iVar7;
      }
    }
    psVar8 = psVar8 + 0x84;
    iVar7 = iVar7 + 1;
  } while ((int)psVar8 < 0x89b6e8);
  if (0 < local_4) {
    uVar2 = *(undefined2 *)(&DAT_007999e0 + local_4 * 0x108);
    (&DAT_00bcba02)[iVar3 * 0x48] = *(undefined2 *)((int)&DAT_007999dc + local_4 * 0x108 + 2);
    (&DAT_00bcba04)[iVar3 * 0x48] = uVar2;
    (&DAT_00bcba06)[iVar3 * 0x48] = (short)local_4;
  }
  return;
}
