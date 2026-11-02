#include "restore_types.h"
#define DAT_01266458 (*(undefined *)0x01266458u)
#define DAT_0126645c (*(undefined *)0x0126645cu)
#define DAT_01266460 (*(undefined *)0x01266460u)
extern void * FUN_00561b68();
extern void FUN_00561bdc();
extern size_t FUN_00563380();
extern void * FUN_005645a0();
extern uint * FUN_00566750();
extern undefined4 FUN_0056a896();
extern int FUN_0056b045();
extern undefined4 * FUN_0056b09d();
extern byte * FUN_0056b104();

undefined4 FUN_0056aebe(char *param_1,int param_2)

{
  char *pcVar1;
  int iVar2;
  int *piVar3;
  size_t sVar4;
  LPCSTR lpName;
  int *piVar5;
  bool bVar6;
  
  if (param_1 == (char *)0x0) {
    return 0xffffffff;
  }
  pcVar1 = (char *)FUN_0056b104(param_1,0x3d);
  if (pcVar1 == (char *)0x0) {
    return 0xffffffff;
  }
  if (param_1 == pcVar1) {
    return 0xffffffff;
  }
  bVar6 = pcVar1[1] == '\0';
  if (DAT_01266458 == DAT_0126645c) {
    DAT_01266458 = (int *)FUN_0056b09d(DAT_01266458);
  }
  if (DAT_01266458 == (int *)0x0) {
    if ((param_2 == 0) || (DAT_01266460 == (undefined4 *)0x0)) {
      if (bVar6) {
        return 0;
      }
      DAT_01266458 = FUN_00561b68(4);
      if (DAT_01266458 == (int *)0x0) {
        return 0xffffffff;
      }
      *(undefined4 *)(DAT_01266458) = 0;
      if (DAT_01266460 == (undefined4 *)0x0) {
        DAT_01266460 = FUN_00561b68(4);
        if (DAT_01266460 == (undefined4 *)0x0) {
          return 0xffffffff;
        }
        *(undefined4 *)(DAT_01266460) = 0;
      }
    }
    else {
      iVar2 = FUN_0056a896();
      if (iVar2 != 0) {
        return 0xffffffff;
      }
    }
  }
  piVar3 = DAT_01266458;
  iVar2 = FUN_0056b045(param_1,(int)pcVar1 - (int)param_1);
  if ((iVar2 < 0) || (*piVar3 == 0)) {
    if (bVar6) {
      return 0;
    }
    if (iVar2 < 0) {
      iVar2 = -iVar2;
    }
    piVar3 = (int *)FUN_005645a0(piVar3,iVar2 * 4 + 8);
    if (piVar3 == (int *)0x0) {
      return 0xffffffff;
    }
    piVar3[iVar2] = (int)param_1;
    piVar3[iVar2 + 1] = 0;
  }
  else {
    if (!bVar6) {
      piVar3[iVar2] = (int)param_1;
      goto LAB_0056aff2;
    }
    piVar5 = piVar3 + iVar2;
    FUN_00561bdc(piVar3[iVar2]);
    for (; *piVar5 != 0; piVar5 = piVar5 + 1) {
      iVar2 = iVar2 + 1;
      *piVar5 = piVar5[1];
    }
    piVar3 = (int *)FUN_005645a0(piVar3,iVar2 << 2);
    if (piVar3 == (int *)0x0) goto LAB_0056aff2;
  }
  DAT_01266458 = piVar3;
LAB_0056aff2: ;
  if (param_2 != 0) {
    sVar4 = FUN_00563380(param_1);
    lpName = FUN_00561b68(sVar4 + 2);
    if (lpName != (LPCSTR)0x0) {
      FUN_00566750(lpName,param_1);
      pcVar1[(int)lpName - (int)param_1] = '\0';
      SetEnvironmentVariableA
                (lpName,(LPCSTR)(~-(uint)bVar6 & (uint)(pcVar1 + ((int)lpName - (int)param_1) + 1)))
      ;
      FUN_00561bdc(lpName);
    }
  }
  return 0;
}
