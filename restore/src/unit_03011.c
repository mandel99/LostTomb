#include "restore_types.h"
#define DAT_01266434 (*(undefined *)0x01266434u)
#define DAT_01266960 (*(uint32_t *)0x01266960u)
#define DAT_01266a60 (*(uint32_t *)0x01266a60u)
#define _DAT_01266430 (*(undefined4 *)0x01266430u)
extern void FUN_00561f30();
extern void * FUN_00563240();
extern DWORD FUN_005662e3();
extern int FUN_005664d5();
extern undefined4 FUN_005669c6();
extern int FUN_0056aba0();

int FUN_0056a711(uint param_1,int param_2)

{
  int iVar1;
  HANDLE hFile;
  BOOL BVar2;
  int iVar3;
  undefined1 local_1004 [4060];
  undefined4 uStackY_28;
  uint uStackY_24;
  uint uStackY_20;
  
  FUN_00561f30();
  iVar3 = 0;
  if ((param_1 < DAT_01266a60) &&
     ((*(byte *)((&DAT_01266960)[(int)param_1 >> 5] + 4 + (param_1 & 0x1f) * 8) & 1) != 0)) {
    iVar1 = FUN_005662e3();
    if ((iVar1 != -1) && (iVar1 = FUN_005662e3(), iVar1 != -1)) {
      param_2 = param_2 - iVar1;
      if (param_2 < 1) {
        if (param_2 < 0) {
          uStackY_20 = 0x56a800;
          FUN_005662e3();
          uStackY_20 = param_1;
          uStackY_24 = 0x56a806;
          hFile = (HANDLE)FUN_005669c6();
          BVar2 = SetEndOfFile(hFile);
          iVar3 = (BVar2 != 0) - 1;
          if (iVar3 == -1) {
            _DAT_01266430 = 0xd;
            DAT_01266434 = GetLastError();
          }
        }
      }
      else {
        uStackY_20 = 0x56a796;
        FUN_00563240(local_1004,0,0x1000);
        uStackY_20 = 0x8000;
        uStackY_24 = param_1;
        uStackY_28 = 0x56a7a1;
        FUN_0056aba0();
        do {
          uStackY_20 = 0x56a7c0;
          iVar1 = FUN_005664d5();
          if (iVar1 == -1) {
            if (DAT_01266434 == 5) {
              _DAT_01266430 = 0xd;
            }
            iVar3 = -1;
            break;
          }
          param_2 = param_2 - iVar1;
        } while (0 < param_2);
        FUN_0056aba0();
      }
      uStackY_20 = 0x56a83e;
      FUN_005662e3();
      return iVar3;
    }
  }
  else {
    _DAT_01266430 = 9;
  }
  return -1;
}
