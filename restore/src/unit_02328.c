#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00f5695c (*(undefined *)0x00f5695cu)
#define DAT_00f56c44 (*(undefined *)0x00f56c44u)
#define DAT_00f56c94 (*(undefined *)0x00f56c94u)
#define DAT_00f56cd4 (*(undefined *)0x00f56cd4u)
extern uint FUN_0047b8e0();
extern undefined4 FUN_0047e090();
extern char FUN_0047e0d0();

undefined4 FUN_0051c630(int param_1)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  bVar1 = false;
  if ((*(short *)(&DAT_00b4e6fc + param_1 * 2) == 0) ||
     ((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + param_1 * 2) * 0x84] != 0x8a)) {
    if (((&DAT_00af598c)[param_1] & 0x10000) == 0) {
      if (((&DAT_00b285cc)[param_1] & 0x80) != 0) {
        if (2 < (&DAT_00b7482c)[param_1] - DAT_00f5695c) {
          return 0;
        }
        iVar3 = FUN_0047e0d0(param_1);
        if ((iVar3 < 3) && (cVar2 = FUN_0047e090(param_1), cVar2 == '\0')) {
          return 1;
        }
        return 0;
      }
      iVar4 = FUN_0047b8e0(param_1);
      iVar3 = (&DAT_00b7482c)[param_1];
      iVar5 = DAT_00f56c44;
      if (iVar4 == 0) {
        iVar5 = DAT_00f56c94;
      }
    }
    else {
      iVar3 = (&DAT_00b7482c)[param_1];
      iVar5 = DAT_00f56cd4;
    }
    switch(iVar3 - iVar5) {
    case 0: ;
    case 2: ;
    case 8: ;
    case 10: ;
      bVar1 = true;
      break;
    case 1: ;
    case 3: ;
    case 9: ;
    case 0xb: ;
      break;
default: ;
      goto switchD_0051c6ef_caseD_4;
    }
    if ((DAT_00bca2e0 == 2) || (DAT_00bca2e0 == 6)) {
      bVar1 = (bool)(bVar1 ^ 1);
    }
    if (bVar1) {
      if ((*(uint *)(&DAT_00af55fc + param_1 * 4) & 0x100) != 0) {
        return 0;
      }
      if ((*(uint *)(&DAT_00af5d1c + param_1 * 4) & 0x100) != 0) {
        return 0;
      }
    }
    else {
      if ((*(uint *)(&DAT_00af5990 + param_1 * 4) & 0x100) != 0) {
        return 0;
      }
      if ((*(uint *)(&DAT_00af5988 + param_1 * 4) & 0x100) != 0) {
switchD_0051c6ef_caseD_4: ;
        return 0;
      }
    }
  }
  return 1;
}
