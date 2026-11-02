#include "restore_types.h"
#define DAT_01266710 (*(undefined *)0x01266710u)
#define DAT_01266944 (*(undefined *)0x01266944u)
extern int FUN_0056ac16();

int __cdecl FUN_0056a857(uchar *_Str1,uchar *_Str2,size_t _MaxCount)

{
  int iVar1;
  
  if (_MaxCount == 0) {
    return 0;
  }
  iVar1 = FUN_0056ac16(DAT_01266944,1,_Str1,_MaxCount,_Str2,_MaxCount,DAT_01266710);
  if (iVar1 == 0) {
    return 0x7fffffff;
  }
  return iVar1 + -2;
}
