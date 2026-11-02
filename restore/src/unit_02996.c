#include "restore_types.h"
#define DAT_0126649c (*(undefined *)0x0126649cu)
#define DAT_012666e4 (*(undefined *)0x012666e4u)

int FUN_005699ca(int param_1)

{
  int iVar1;
  bool bVar2;
  
  if (param_1 == -2) {
    DAT_012666e4 = 1;
                    /* WARNING: Could not recover jumptable at 0x005699e4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar1 = GetOEMCP();
    return iVar1;
  }
  if (param_1 == -3) {
    DAT_012666e4 = 1;
                    /* WARNING: Could not recover jumptable at 0x005699f9. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar1 = GetACP();
    return iVar1;
  }
  bVar2 = param_1 == -4;
  if (bVar2) {
    param_1 = DAT_0126649c;
  }
  DAT_012666e4 = (uint)bVar2;
  return param_1;
}
