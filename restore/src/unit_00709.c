#include "restore_types.h"

ushort FUN_00449660(void)

{
  ushort in_AX;
  int in_ECX;
  
  if (*(char *)(in_ECX + 2) == '\x02') {
    in_AX = *(ushort *)(in_ECX + 0x1e);
    if ((short)in_AX < 0) {
      in_AX = *(ushort *)(in_ECX + 0x22);
    }
    else if (-1 < *(short *)(in_ECX + 0x20)) goto LAB_00449685;
    if (8 < (short)in_AX) {
      return CONCAT11((char)(in_AX >> 8),1);
    }
  }
LAB_00449685: ;
  return in_AX & 0xff00;
}
