#include "restore_types.h"
#define DAT_005cdb78 (*(undefined4 *)0x005cdb78u)
#define DAT_005cdb7c (*(undefined4 *)0x005cdb7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern undefined4 FUN_004eaac0();
extern uint FUN_004ec410();
extern bool FUN_00517f20();
extern bool FUN_005181c0();
extern bool FUN_00519be0();

int FUN_004eab10(int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
                undefined4 param_6)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int local_4;
  
  local_4 = 0;
  do {
    iVar4 = (&DAT_005cdb7c)[param_3 * 2] + param_2;
    iVar5 = (&DAT_005cdb78)[param_3 * 2] + param_1;
    iVar3 = DAT_00bca2d8 + iVar4 * 0xe4 + iVar5;
    cVar1 = FUN_004eaac0(param_6);
    if (cVar1 == '\0') {
      iVar4 = FUN_005181c0(param_4,param_5,iVar5,iVar4);
LAB_004eab9a: ;
      if (iVar4 != 0) goto LAB_004eaba1;
    }
    else {
      iVar2 = FUN_00519be0(param_4,param_5);
      if (iVar2 == 0) {
        iVar4 = FUN_00517f20(param_4,param_5,iVar5,iVar4);
        goto LAB_004eab9a;
      }
LAB_004eaba1: ;
      if (*(short *)(&DAT_00b350dc + iVar3 * 2) == 0) {
        cVar1 = FUN_004ec410((&DAT_00af598c)[iVar3],param_6);
        if (cVar1 != '\0') {
          return param_3;
        }
      }
    }
    param_3 = param_3 + 1;
    if (0xf < param_3) {
      param_3 = 0;
    }
    local_4 = local_4 + 1;
    if (0xf < local_4) {
      return -1;
    }
  } while( true );
}
