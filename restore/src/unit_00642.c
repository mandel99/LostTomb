#include "restore_types.h"
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
#define DAT_0077f760 (*(undefined4 *)0x0077f760u)
#define DAT_0077f786 (*(undefined4 *)0x0077f786u)

byte FUN_00444a00(int param_1,int param_2)

{
  param_1 = param_1 * 0x62;
  if ((((&DAT_0077f748)[param_1] == '\x01') && ((&DAT_0077f760)[param_1] != '\0')) &&
     ((byte)(&DAT_0077f786)[param_2 + param_1] < 0x29)) {
    return (&DAT_0077f786)[param_2 + param_1];
  }
  return 0;
}
