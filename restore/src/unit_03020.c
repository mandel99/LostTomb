#include "restore_types.h"
#define DAT_01266458 (*(undefined *)0x01266458u)
extern int FUN_0056a857();

int FUN_0056b045(uchar *param_1,size_t param_2)

{
  uchar *_Str2;
  int iVar1;
  int *piVar2;
  
  _Str2 = (uchar *)*(undefined4 *)(DAT_01266458);
  piVar2 = DAT_01266458;
  while( true ) {
    if (_Str2 == (uchar *)0x0) {
      return -((int)piVar2 - (int)DAT_01266458 >> 2);
    }
    iVar1 = FUN_0056a857(param_1,_Str2,param_2);
    if ((iVar1 == 0) &&
       ((*(char *)(*piVar2 + param_2) == '=' || (*(char *)(*piVar2 + param_2) == '\0')))) break;
    _Str2 = (uchar *)piVar2[1];
    piVar2 = piVar2 + 1;
  }
  return (int)piVar2 - (int)DAT_01266458 >> 2;
}
