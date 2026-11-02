#include "restore_types.h"
#define DAT_01266458 (*(undefined *)0x01266458u)
#define DAT_01266460 (*(undefined *)0x01266460u)
#define DAT_01266a80 (*(undefined *)0x01266a80u)
extern size_t FUN_00563380();
extern int FUN_0056a857();
extern undefined4 FUN_0056a896();

int FUN_005696a2(uchar *param_1)

{
  int iVar1;
  size_t _MaxCount;
  size_t sVar2;
  int *piVar3;
  
  if (((DAT_01266a80 != 0) &&
      ((DAT_01266458 != (int *)0x0 ||
       (((DAT_01266460 != 0 && (iVar1 = FUN_0056a896(), iVar1 == 0)) && (DAT_01266458 != (int *)0x0)
        ))))) && (piVar3 = DAT_01266458, param_1 != (uchar *)0x0)) {
    _MaxCount = FUN_00563380((char *)param_1);
    for (; (char *)*piVar3 != (char *)0x0; piVar3 = piVar3 + 1) {
      sVar2 = FUN_00563380((char *)*piVar3);
      if (((_MaxCount < sVar2) && (((uchar *)*piVar3)[_MaxCount] == '=')) &&
         (iVar1 = FUN_0056a857((uchar *)*piVar3,param_1,_MaxCount), iVar1 == 0)) {
        return *piVar3 + 1 + _MaxCount;
      }
    }
  }
  return 0;
}
