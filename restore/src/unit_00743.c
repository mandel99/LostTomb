#include "restore_types.h"
#include "town_trade.h"
#define DAT_007770f0 (*(undefined4 *)0x007770f0u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5ed5 (*(undefined4 *)0x00ea5ed5u)
#define DAT_00ea5ed6 (*(undefined4 *)0x00ea5ed6u)
#define DAT_00ea5ed8 (*(undefined4 *)0x00ea5ed8u)
#define DAT_00ea5ed9 (*(undefined4 *)0x00ea5ed9u)
#define DAT_00ea5eda (*(undefined4 *)0x00ea5edau)
#define DAT_00ea5edb (*(undefined4 *)0x00ea5edbu)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int * FUN_0044baf0();


int FUN_0044d520(int param_1,int *param_2)

{
  short sVar1;
  char cVar2;
  short *psVar3;
  int iVar4;
  
  iVar4 = 0;
  psVar3 = (short *)FUN_0044baf0(0);
  do {
    if (psVar3 == (short *)0x0) {
      return 0;
    }
    cVar2 = FUN_0044d4c0((const struct RestoreTownRecord *)(uintptr_t)psVar3);
    if (cVar2 != '\0') {
      if (iVar4 == param_1) {
        *param_2 = (int)*psVar3;
        sVar1 = psVar3[2];
        if (sVar1 == 0x24) {
          if (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) <= (int)psVar3[6]) {
            return -1;
          }
        }
        else {
          if (sVar1 == 0x25) {
            iVar4 = (int)psVar3[0xe];
            if (((iVar4 < 0) || (0x31 < iVar4)) || ((&DAT_007770f0)[iVar4 * 0x144] != '\x02')) {
              if ((&DAT_00ea5edb)[DAT_00eaac28 * 0x49d8] == '\0') {
                return -4;
              }
              if ((&DAT_00ea5ed6)[DAT_00eaac28 * 0x49d8] == '\0') {
                return -3;
              }
            }
            else {
              iVar4 = DAT_00eaac28 * 0x49d8;
              if (((&DAT_00ea5ed8)[iVar4] == '\0') && ((&DAT_00ea5eda)[iVar4] == '\0')) {
                return -6;
              }
              if (((&DAT_00ea5ed9)[iVar4] == '\0') && ((&DAT_00ea5ed5)[iVar4] == '\0')) {
                return -5;
              }
            }
            return -2;
          }
          if ((int)(&DAT_00ea6160)[DAT_00eaac28 * 0x1276 + (int)sVar1] < psVar3[6] * 100) {
            return -1;
          }
        }
        return *psVar3 + 1;
      }
      iVar4 = iVar4 + 1;
    }
    psVar3 = (short *)FUN_0044baf0(psVar3);
  } while( true );
}
