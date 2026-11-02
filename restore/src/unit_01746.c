#include "restore_types.h"
#define DAT_00e38aec (*(char *)0x00e38aecu)
#define DAT_00e38aed (*(char *)0x00e38aedu)
#define DAT_00e38aee (*(char *)0x00e38aeeu)

void FUN_004ce3c0(char *param_1)

{
  char *pcVar1;
  
  do {
    pcVar1 = param_1;
    param_1 = pcVar1 + 1;
  } while (*pcVar1 != '\0');
  *pcVar1 = '.';
  pcVar1[1] = DAT_00e38aec;
  pcVar1[2] = DAT_00e38aed;
  pcVar1[3] = DAT_00e38aee;
  pcVar1[4] = '\0';
  return;
}
