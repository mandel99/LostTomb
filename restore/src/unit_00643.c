#include "restore_types.h"
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
#define DAT_0077f760 (*(undefined4 *)0x0077f760u)
#define DAT_0077f786 (*(undefined4 *)0x0077f786u)

void FUN_00444a40(int param_1,int param_2,int param_3)

{
  param_1 = param_1 * 0x62;
  if ((((&DAT_0077f748)[param_1] == '\x01') && ((&DAT_0077f760)[param_1] != '\0')) && (-1 < param_3)
     ) {
    (&DAT_0077f786)[param_2 + param_1] = (char)param_3;
  }
  return;
}
