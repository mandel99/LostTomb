#include "restore_types.h"
#define DAT_00604e64 (*(undefined *)0x00604e64u)
#define DAT_0126648c (*(undefined *)0x0126648cu)
#define DAT_0126649c (*(undefined *)0x0126649cu)
#define _DAT_01266430 (*(undefined4 *)0x01266430u)

int FUN_005691e5(LPSTR param_1,WCHAR param_2)

{
  LPSTR lpMultiByteStr;
  int iVar1;
  
  lpMultiByteStr = param_1;
  if (param_1 == (LPSTR)0x0) {
    return 0;
  }
  if (DAT_0126648c == 0) {
    if ((ushort)param_2 < 0x100) {
      *param_1 = (CHAR)param_2;
      return 1;
    }
  }
  else {
    param_1 = (LPSTR)0x0;
    iVar1 = WideCharToMultiByte(DAT_0126649c,0x220,&param_2,1,lpMultiByteStr,DAT_00604e64,
                                (LPCSTR)0x0,(LPBOOL)&param_1);
    if ((iVar1 != 0) && (param_1 == (LPSTR)0x0)) {
      return iVar1;
    }
  }
  _DAT_01266430 = 0x2a;
  return -1;
}
