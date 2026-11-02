#include "restore_types.h"
#define DAT_005deb9c (*(undefined4 *)0x005deb9cu)
#define DAT_005deebc (*(undefined4 *)0x005deebcu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799aec (*(undefined4 *)0x00799aecu)
#define DAT_00a835fc (*(undefined4 *)0x00a835fcu)
#define DAT_00bcb9f6 (*(undefined4 *)0x00bcb9f6u)
#define DAT_00bcb9f8 (*(undefined4 *)0x00bcb9f8u)
#define DAT_00bcba02 (*(undefined4 *)0x00bcba02u)
#define DAT_00bcba04 (*(undefined4 *)0x00bcba04u)
#define DAT_00bcba06 (*(undefined4 *)0x00bcba06u)
#define DAT_00bcba0c (*(undefined4 *)0x00bcba0cu)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern bool FUN_0051b040();
extern uint FUN_00562348();

uint FUN_004ba3b0(int param_1)

{
  undefined2 uVar1;
  int *piVar2;
  short *psVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  int local_10;
  int local_c;
  uint local_8;
  uint local_4;
  
  iVar9 = param_1 * 0x90;
  uVar7 = (uint)*(short *)(&DAT_00bcba0c + iVar9);
  if (uVar7 == 4) {
    uVar7 = FUN_00562348();
    uVar7 = uVar7 & 3;
  }
  local_10 = 100;
  local_8 = 0;
  local_c = 10000;
  FUN_0051b040((int)*(short *)(&DAT_00bcb9f6 + iVar9),(int)*(short *)(&DAT_00bcb9f8 + iVar9),
               0xfffffffe,0xfffffffe);
  local_4 = 1;
  piVar2 = &DAT_00799aec;
  do {
    if ((((char)piVar2[-3] == '\x01') && ((&DAT_00a835fc)[*piVar2] == '\0')) &&
       (0 < (int)(&DAT_00eed760)[*piVar2])) {
      iVar6 = 0;
      do {
        if ((local_10 < iVar6) || (*(short *)(&DAT_005deb9c + (iVar6 + uVar7 * 100) * 2) == 0))
        break;
        if ((short)piVar2[1] == *(short *)(&DAT_005deb9c + (iVar6 + uVar7 * 100) * 2)) {
          iVar4 = (int)*(short *)((int)piVar2 + -6);
          iVar5 = (int)*(short *)(&DAT_00bcb9f6 + iVar9);
          iVar8 = (int)(short)piVar2[-1];
          _DAT_00d3b4f8 = (int)*(short *)(&DAT_00bcb9f8 + iVar9);
          if (iVar4 < iVar5) {
            DAT_00d3b4fc = iVar5 - iVar4;
          }
          else if (iVar5 < iVar4) {
            DAT_00d3b4fc = iVar4 - iVar5;
          }
          else {
            DAT_00d3b4fc = 0;
          }
          if (iVar8 < _DAT_00d3b4f8) {
            _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar8;
          }
          else if (_DAT_00d3b4f8 < iVar8) {
            _DAT_00d3b4f8 = iVar8 - _DAT_00d3b4f8;
          }
          else {
            _DAT_00d3b4f8 = 0;
          }
          iVar4 = DAT_00d3b4fc;
          if (DAT_00d3b4fc < _DAT_00d3b4f8) {
            iVar4 = _DAT_00d3b4f8;
          }
          if ((iVar6 < local_10) || (iVar6 = local_10, iVar4 < local_c)) {
            local_10 = iVar6;
            local_8 = local_4;
            local_c = iVar4;
          }
          break;
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < 100);
    }
    piVar2 = piVar2 + 0x42;
    local_4 = local_4 + 1;
  } while ((int)piVar2 < 0x89b6e4);
  if ((int)local_8 < 1) {
    uVar7 = 1;
    piVar2 = &DAT_00799aec;
    do {
      if ((((char)piVar2[-3] == '\x01') && ((&DAT_00a835fc)[*piVar2] == '\0')) &&
         (0 < (int)(&DAT_00eed760)[*piVar2])) {
        iVar6 = 0;
        psVar3 = &DAT_005deebc;
        do {
          if ((local_10 < iVar6) || (*psVar3 == 0)) break;
          if ((short)piVar2[1] == *psVar3) {
            iVar4 = (int)*(short *)((int)piVar2 + -6);
            iVar5 = (int)*(short *)(&DAT_00bcb9f6 + iVar9);
            iVar8 = (int)(short)piVar2[-1];
            _DAT_00d3b4f8 = (int)*(short *)(&DAT_00bcb9f8 + iVar9);
            if (iVar4 < iVar5) {
              DAT_00d3b4fc = iVar5 - iVar4;
            }
            else if (iVar5 < iVar4) {
              DAT_00d3b4fc = iVar4 - iVar5;
            }
            else {
              DAT_00d3b4fc = 0;
            }
            if (iVar8 < _DAT_00d3b4f8) {
              _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar8;
            }
            else if (_DAT_00d3b4f8 < iVar8) {
              _DAT_00d3b4f8 = iVar8 - _DAT_00d3b4f8;
            }
            else {
              _DAT_00d3b4f8 = 0;
            }
            iVar4 = DAT_00d3b4fc;
            if (DAT_00d3b4fc < _DAT_00d3b4f8) {
              iVar4 = _DAT_00d3b4f8;
            }
            if ((iVar6 < local_10) || (iVar6 = local_10, iVar4 < local_c)) {
              local_10 = iVar6;
              local_c = iVar4;
              local_8 = uVar7;
            }
            break;
          }
          psVar3 = psVar3 + 1;
          iVar6 = iVar6 + 1;
        } while ((int)psVar3 < 0x5def84);
      }
      piVar2 = piVar2 + 0x42;
      uVar7 = uVar7 + 1;
    } while ((int)piVar2 < 0x89b6e4);
    if ((int)local_8 < 1) {
      return local_8 & 0xffffff00;
    }
  }
  iVar9 = local_8 * 0x108;
  if (((&DAT_007999e8)[local_8 * 0x84] != 0x48) && ((&DAT_007999e8)[local_8 * 0x84] != 0xc1)) {
    uVar1 = *(undefined2 *)(&DAT_007999e0 + iVar9);
    (&DAT_00bcba02)[param_1 * 0x48] = *(undefined2 *)((int)&DAT_007999dc + iVar9 + 2);
    (&DAT_00bcba04)[param_1 * 0x48] = uVar1;
    (&DAT_00bcba06)[param_1 * 0x48] = (short)local_8;
    return CONCAT31((int3)(local_8 >> 8),1);
  }
  uVar1 = *(undefined2 *)(&DAT_007999e0 + iVar9);
  (&DAT_00bcba02)[param_1 * 0x48] = *(short *)((int)&DAT_007999dc + iVar9 + 2) + 1;
  (&DAT_00bcba04)[param_1 * 0x48] = uVar1;
  (&DAT_00bcba06)[param_1 * 0x48] = (short)(local_8 + 1);
  return CONCAT31((int3)(local_8 + 1 >> 8),1);
}
