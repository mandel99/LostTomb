#include "restore_types.h"
#define DAT_00605330 (*(undefined4 *)0x00605330u)
#define DAT_00605364 (*(undefined4 *)0x00605364u)
#define DAT_01266570 (*(undefined *)0x01266570u)
#define _DAT_01266570 (*(undefined4 *)0x01266570u)
#define _DAT_01266574 (*(undefined4 *)0x01266574u)
#define _DAT_01266578 (*(undefined4 *)0x01266578u)
#define _DAT_0126657c (*(undefined4 *)0x0126657cu)
#define _DAT_01266580 (*(undefined4 *)0x01266580u)
#define _DAT_01266584 (*(undefined4 *)0x01266584u)
#define _DAT_01266588 (*(undefined4 *)0x01266588u)
#define _DAT_0126658c (*(undefined4 *)0x0126658cu)
#define _DAT_01266590 (*(undefined4 *)0x01266590u)

undefined * FUN_0056710d(int *param_1)

{
  int iVar1;
  bool bVar2;
  undefined *puVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int *piVar7;
  
  bVar2 = false;
  iVar1 = *param_1;
  if (iVar1 < 0) {
    puVar3 = (undefined *)0x0;
  }
  else {
    iVar4 = iVar1 % 0x7861f80;
    iVar1 = (iVar1 / 0x7861f80) * 4;
    _DAT_01266584 = iVar1 + 0x46;
    iVar5 = iVar4;
    if (0x1e1337f < iVar4) {
      iVar5 = iVar4 + -0x1e13380;
      _DAT_01266584 = iVar1 + 0x47;
      if (0x1e1337f < iVar5) {
        iVar5 = iVar4 + -0x3c26700;
        _DAT_01266584 = iVar1 + 0x48;
        if (iVar5 < 0x1e28500) {
          bVar2 = true;
        }
        else {
          _DAT_01266584 = iVar1 + 0x49;
          iVar5 = iVar4 + -0x5a4ec00;
        }
      }
    }
    _DAT_0126658c = iVar5 / 0x15180;
    piVar6 = (int *)&DAT_00605330;
    if (!bVar2) {
      piVar6 = (int *)&DAT_00605364;
    }
    _DAT_01266580 = 1;
    piVar7 = piVar6;
    while (piVar7 = piVar7 + 1, *piVar7 < _DAT_0126658c) {
      _DAT_01266580 = _DAT_01266580 + 1;
    }
    _DAT_01266580 = _DAT_01266580 + -1;
    _DAT_0126657c = _DAT_0126658c - piVar6[_DAT_01266580];
    _DAT_01266588 = (*param_1 / 0x15180 + 4) % 7;
    _DAT_01266578 = (iVar5 % 0x15180) / 0xe10;
    _DAT_01266570 = (iVar5 % 0x15180) % 0xe10;
    _DAT_01266574 = _DAT_01266570 / 0x3c;
    _DAT_01266570 = _DAT_01266570 % 0x3c;
    _DAT_01266590 = 0;
    puVar3 = &DAT_01266570;
  }
  return puVar3;
}
