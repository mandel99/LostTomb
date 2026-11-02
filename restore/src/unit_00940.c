#include "restore_types.h"
#define DAT_005cd678 (*(undefined4 *)0x005cd678u)
#define DAT_0078c400 (*(undefined *)0x0078c400u)
#define DAT_0078c404 (*(undefined *)0x0078c404u)
#define DAT_0078c408 (*(undefined *)0x0078c408u)
#define DAT_0078c40c (*(undefined *)0x0078c40cu)
#define DAT_0078c410 (*(undefined *)0x0078c410u)
#define DAT_0078c414 (*(undefined4 *)0x0078c414u)
#define DAT_00a69fdc (*(undefined4 *)0x00a69fdcu)
#define DAT_00a76aec (*(undefined4 *)0x00a76aecu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00ee0c50 (*(undefined4 *)0x00ee0c50u)
extern void FUN_0040f8f0();

void FUN_004689e0(int param_1,undefined1 param_2)

{
  char cVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int local_4;
  
  iVar5 = 0;
  FUN_0040f8f0(&DAT_0078c414,4000,0);
  DAT_0078c410 = 0;
  local_4 = 1;
  while( true ) {
    (&DAT_00a69fdc)[param_1] = (char)DAT_0078c408;
    (&DAT_00a76aec)[param_1] = param_2;
    iVar4 = -1;
    piVar6 = &DAT_005cd678;
    do {
      iVar3 = *piVar6 + param_1;
      cVar1 = *(char *)((int)&DAT_00ee0c50 + iVar3);
      if ((((-1 < cVar1) && (cVar1 < '\x03')) &&
          ((cVar1 != '\x02' || (((&DAT_00af598c)[iVar3] & 0x400) != 0)))) &&
         ((&DAT_00a76aec)[iVar3] == '\0')) {
        (&DAT_00a69fdc)[iVar3] = (char)DAT_0078c408;
        (&DAT_00a76aec)[iVar3] = param_2;
        DAT_0078c400 = DAT_0078c400 + 1;
        if (iVar4 == -1) {
          DAT_0078c404 = DAT_0078c404 + 1;
          iVar4 = iVar3;
        }
        else {
          (&DAT_0078c414)[iVar5] = iVar3;
          iVar5 = iVar5 + 1;
          if (999 < iVar5) {
            iVar5 = 0;
          }
        }
      }
      piVar6 = piVar6 + 1;
    } while ((int)piVar6 < 0x5cd688);
    if (iVar4 == -1) {
      if (DAT_0078c410 == iVar5) {
        DAT_0078c40c = iVar5;
        return;
      }
      iVar4 = (&DAT_0078c414)[DAT_0078c410];
      DAT_0078c410 = DAT_0078c410 + 1;
      if (999 < DAT_0078c410) {
        DAT_0078c410 = 0;
      }
    }
    if (0x1e < DAT_0078c404) {
      DAT_0078c408 = DAT_0078c408 + 1;
      DAT_0078c404 = 0;
    }
    if (iVar4 < 0) break;
    bVar2 = 0xcb0f < local_4;
    param_1 = iVar4;
    local_4 = local_4 + 1;
    if (bVar2) {
      DAT_0078c40c = iVar5;
      return;
    }
  }
  DAT_0078c40c = iVar5;
  return;
}
