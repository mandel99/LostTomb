#include "restore_types.h"
#define DAT_01266434 (*(undefined *)0x01266434u)
#define DAT_01266438 (*(undefined *)0x01266438u)
#define DAT_01266708 (*(undefined *)0x01266708u)
#define DAT_01266960 (*(uint32_t *)0x01266960u)
#define _DAT_01266430 (*(undefined4 *)0x01266430u)
extern undefined4 FUN_005647eb();
extern int FUN_005660ed();
extern DWORD FUN_005662e3();
extern int FUN_00566840();
extern undefined4 FUN_005668d5();
extern void FUN_005672c8();
extern int FUN_0056a711();

uint FUN_00568ec2(LPCSTR param_1,uint param_2,uint param_3,uint param_4)

{
  byte *pbVar1;
  uint uVar2;
  uint uVar3;
  HANDLE hFile;
  DWORD DVar4;
  int iVar5;
  int iVar6;
  bool bVar7;
  _SECURITY_ATTRIBUTES local_20;
  DWORD local_14;
  DWORD local_10;
  DWORD local_c;
  byte local_5;
  
  bVar7 = (param_2 & 0x80) == 0;
  local_20.nLength = 0xc;
  local_20.lpSecurityDescriptor = (LPVOID)0x0;
  if (bVar7) {
    local_5 = 0;
  }
  else {
    local_5 = 0x10;
  }
  local_20.bInheritHandle = (BOOL)bVar7;
  if (((param_2 & 0x8000) == 0) && (((param_2 & 0x4000) != 0 || (DAT_01266708 != 0x8000)))) {
    local_5 = local_5 | 0x80;
  }
  uVar2 = param_2 & 3;
  if (uVar2 == 0) {
    local_10 = 0x80000000;
  }
  else if (uVar2 == 1) {
    local_10 = 0x40000000;
  }
  else {
    if (uVar2 != 2) {
      _DAT_01266430 = 0x16;
      DAT_01266434 = 0;
      return 0xffffffff;
    }
    local_10 = 0xc0000000;
  }
  if (param_3 == 0x10) {
    local_14 = 0;
  }
  else if (param_3 == 0x20) {
    local_14 = 1;
  }
  else if (param_3 == 0x30) {
    local_14 = 2;
  }
  else {
    if (param_3 != 0x40) {
      _DAT_01266430 = 0x16;
      DAT_01266434 = 0;
      return 0xffffffff;
    }
    local_14 = 3;
  }
  uVar2 = param_2 & 0x700;
  if (uVar2 < 0x401) {
    if ((uVar2 == 0x400) || (uVar2 == 0)) {
      local_c = 3;
    }
    else if (uVar2 == 0x100) {
      local_c = 4;
    }
    else {
      if (uVar2 == 0x200) goto LAB_00568fe0;
      if (uVar2 != 0x300) {
        _DAT_01266430 = 0x16;
        DAT_01266434 = 0;
        return 0xffffffff;
      }
      local_c = 2;
    }
  }
  else {
    if (uVar2 != 0x500) {
      if (uVar2 == 0x600) {
LAB_00568fe0: ;
        local_c = 5;
        goto LAB_00568ff0;
      }
      if (uVar2 != 0x700) {
        _DAT_01266430 = 0x16;
        DAT_01266434 = 0;
        return 0xffffffff;
      }
    }
    local_c = 1;
  }
LAB_00568ff0: ;
  uVar2 = 0x80;
  if (((param_2 & 0x100) != 0) && ((~DAT_01266438 & param_4 & 0x80) == 0)) {
    uVar2 = 1;
  }
  if ((param_2 & 0x40) != 0) {
    uVar2 = uVar2 | 0x4000000;
    local_10 = CONCAT13(RESTORE_SLICE1(local_10,3),0x10000);
  }
  if ((param_2 & 0x1000) != 0) {
    uVar2 = uVar2 | 0x100;
  }
  if ((param_2 & 0x20) == 0) {
    if ((param_2 & 0x10) != 0) {
      uVar2 = uVar2 | 0x10000000;
    }
  }
  else {
    uVar2 = uVar2 | 0x8000000;
  }
  uVar3 = FUN_00566840();
  if (uVar3 == 0xffffffff) {
    DAT_01266434 = 0;
    _DAT_01266430 = 0x18;
  }
  else {
    hFile = CreateFileA(param_1,local_10,local_14,&local_20,local_c,uVar2,(HANDLE)0x0);
    if (hFile != (HANDLE)0xffffffff) {
      DVar4 = GetFileType(hFile);
      if (DVar4 != 0) {
        if (DVar4 == 2) {
          local_5 = local_5 | 0x40;
        }
        else if (DVar4 == 3) {
          local_5 = local_5 | 8;
        }
        FUN_005668d5(uVar3,hFile);
        iVar6 = (uVar3 & 0x1f) * 8;
        RESTORE_SLICE1(param_1,3) = local_5 & 0x48;
        *(byte *)((&DAT_01266960)[(int)uVar3 >> 5] + 4 + iVar6) = local_5 | 1;
        if ((((local_5 & 0x48) == 0) && ((local_5 & 0x80) != 0)) && ((param_2 & 2) != 0)) {
          local_14 = FUN_005662e3(uVar3,0xffffffff,2);
          if (local_14 == -1) {
            if (DAT_01266434 != 0x83) {
LAB_00569151: ;
              FUN_005647eb(uVar3);
              return 0xffffffff;
            }
          }
          else {
            param_3 = param_3 & 0xffffff;
            iVar5 = FUN_005660ed(uVar3,(int)&param_3 + 3,1);
            if ((((iVar5 == 0) && (RESTORE_SLICE1(param_3,3) == '\x1a')) &&
                (iVar5 = FUN_0056a711(uVar3,local_14), iVar5 == -1)) ||
               (iVar5 = FUN_005662e3(uVar3,0,0), iVar5 == -1)) goto LAB_00569151;
          }
        }
        if (RESTORE_SLICE1(param_1,3) != 0) {
          return uVar3;
        }
        if ((param_2 & 8) != 0) {
          pbVar1 = (byte *)((&DAT_01266960)[(int)uVar3 >> 5] + 4 + iVar6);
          *pbVar1 = *pbVar1 | 0x20;
          return uVar3;
        }
        return uVar3;
      }
      CloseHandle(hFile);
    }
    DVar4 = GetLastError();
    FUN_005672c8(DVar4);
  }
  return 0xffffffff;
}
