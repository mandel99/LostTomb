#include "restore_types.h"
#define DAT_00706b70 (*(undefined4 *)0x00706b70u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a1e (*(undefined4 *)0x00799a1eu)
#define DAT_009ab9e0 (*(undefined4 *)0x009ab9e0u)
#define DAT_00a9cc1c (*(undefined4 *)0x00a9cc1cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern int FUN_00475b50();

int FUN_00482c10(int param_1,int param_2)

{
  int iVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int local_4;
  
  iVar3 = param_1 + 2;
  iVar1 = param_2 + 2;
  iVar4 = param_1 + -2;
  iVar5 = 1000;
  _DAT_00d3b4e0 = param_2 + -2;
  local_4 = 0;
  DAT_009ab9e0 = 1000;
  if (iVar4 < 0) {
    iVar4 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  param_1 = iVar3;
  if (DAT_00bca2d0 <= iVar3) {
    param_1 = DAT_00bca2d0 + -1;
  }
  param_2 = iVar1;
  if (DAT_00bca2d4 <= iVar1) {
    param_2 = DAT_00bca2d4 + -1;
  }
  DAT_00bc9768 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + iVar4;
  iVar3 = iVar4;
  uVar6 = DAT_00706b70;
  if (_DAT_00d3b4e0 <= param_2) {
    do {
      while (_DAT_00d3b4e4 = iVar3, _DAT_00d3b4e4 <= param_1) {
        if (*(short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2) < 1) {
          if (((&DAT_00af598c)[DAT_00bc9768] & 0x4000) != 0) {
            iVar3 = (int)*(short *)(&DAT_00a9cc1c + DAT_00bc9768 * 2);
            if (iVar5 < iVar3) {
              iVar5 = iVar3;
              DAT_009ab9e0 = iVar3;
            }
            sVar2 = *(short *)(&DAT_00a9cc1c + DAT_00bc9768 * 2) - (short)uVar6;
            *(short *)(&DAT_00a9cc1c + DAT_00bc9768 * 2) = sVar2;
            if (sVar2 < 0) {
              *(undefined2 *)(&DAT_00a9cc1c + DAT_00bc9768 * 2) = 0;
            }
          }
        }
        else {
          iVar3 = FUN_00475b50((int)*(short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2));
          uVar6 = DAT_00706b70;
          sVar2 = (&DAT_00799a1e)[iVar3 * 0x84];
          if (DAT_009ab9e0 < sVar2) {
            DAT_009ab9e0 = (int)sVar2;
          }
          iVar5 = DAT_009ab9e0;
          sVar2 = sVar2 - (short)DAT_00706b70;
          (&DAT_00799a1e)[iVar3 * 0x84] = sVar2;
          if (sVar2 < 0) {
            (&DAT_00799a1e)[iVar3 * 0x84] = 0;
          }
          if ((*(char *)((int)&DAT_007999dc + iVar3 * 0x108 + 1) != '\0') &&
             (0 < (short)(&DAT_007999f4)[iVar3 * 0x84])) {
            local_4 = local_4 + 1;
          }
        }
        DAT_00bc9768 = DAT_00bc9768 + 1;
        iVar3 = _DAT_00d3b4e4 + 1;
      }
      DAT_00bc9768 = DAT_00bc9768 + (iVar4 - param_1) + 0xe3;
      _DAT_00d3b4e0 = _DAT_00d3b4e0 + 1;
      iVar3 = iVar4;
    } while (_DAT_00d3b4e0 <= param_2);
    return local_4;
  }
  return 0;
}
