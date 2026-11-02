#include "restore_types.h"
#define DAT_00bc96e0 (*(undefined *)0x00bc96e0u)
#define DAT_00bc96e4 (*(undefined *)0x00bc96e4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define PTR_LAB_0047f50c (*(unsigned char * *)0x0047f50cu)
#define _DAT_00bc96e8 (*(undefined4 *)0x00bc96e8u)
#define _DAT_00bc96ec (*(undefined4 *)0x00bc96ecu)
#define _DAT_00bc96f0 (*(undefined4 *)0x00bc96f0u)
extern int FUN_0047cbd0();

void FUN_0047f3e0(int param_1,int param_2)

{
  int iVar1;
  
  _DAT_00bc96e8 = 0;
  DAT_00bc96e4 = 0;
  _DAT_00bc96ec = 0;
  DAT_00bc96e0 = 0;
  _DAT_00bc96f0 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  iVar1 = FUN_0047cbd0(param_1,param_2,1);
  if ((uint)(iVar1 * 2) < 7) {
                    /* WARNING: Could not recover jumptable at 0x0047f437. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(&PTR_LAB_0047f50c)[iVar1 * 2])();
    return;
  }
  return;
}
