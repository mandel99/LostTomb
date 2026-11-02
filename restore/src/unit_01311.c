#include "restore_types.h"
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00bcd608 (*(undefined4 *)0x00bcd608u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00bcd672 (*(undefined4 *)0x00bcd672u)
#define DAT_00bcd778 (*(undefined4 *)0x00bcd778u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
#define DAT_00f55f54 (*(undefined4 *)0x00f55f54u)
#define DAT_00f5603c (*(undefined4 *)0x00f5603cu)
extern undefined4 FUN_0048b320();

void FUN_0048b4a0(int param_1)

{
  char cVar1;
  char cVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  
  param_1 = param_1 * 0x184;
  sVar3 = *(short *)(&DAT_00bcd66c + param_1);
  if (sVar3 == 0) {
    iVar5 = -1;
  }
  else if (sVar3 < 0xc9) {
    iVar5 = 0;
  }
  else {
    iVar5 = ((400 < sVar3) - 1 & 0xfffffff8) + 0x10;
  }
  cVar1 = (&DAT_00bcd603)[param_1];
  switch((int)(cVar1)) {
  case '\x18': ;
  case '\x19': ;
  case '\x1a': ;
  case '\x1e': ;
  case '#': ;
switchD_0048b4fc_caseD_18: ;
    if (iVar5 != -1) {
      iVar5 = 0;
    }
    break;
default: ;
    if (cVar1 == '\f') goto switchD_0048b4fc_caseD_18;
  }
  if ((&DAT_00bcd602)[param_1] == '&') {
    cVar2 = (&DAT_00bcd630)[param_1];
    if (((cVar2 != '\x10') && (cVar2 != '\f')) &&
       ((cVar2 != '\a' ||
        (((&DAT_00bcd608)[param_1] != '\x10' && ((&DAT_00bcd608)[param_1] != '\f')))))) {
      iVar5 = 0;
LAB_0048b545: ;
      iVar4 = FUN_0048b320(cVar1);
      *(int *)(&DAT_00bcd778 + param_1) = (&DAT_00f55bb4)[iVar4 * 2] + iVar5;
      return;
    }
  }
  else if (-1 < iVar5) goto LAB_0048b545;
  switch((int)(cVar1)) {
  case '\x18': ;
  case '\x19': ;
  case '\x1a': ;
  case '\x1e': ;
  case '#': ;
    break;
default: ;
    if (cVar1 != '\f') {
      if (cVar1 != '\0') {
LAB_0048b5b6: ;
        *(undefined4 *)(&DAT_00bcd778 + param_1) = DAT_00f55f54;
        return;
      }
      switch((&DAT_00bcd672)[param_1]) {
      case 0x18: ;
      case 0x19: ;
      case 0x1a: ;
      case 0x1e: ;
      case 0x23: ;
        break;
default: ;
        if ((&DAT_00bcd672)[param_1] != '\f') goto LAB_0048b5b6;
      }
      *(undefined4 *)(&DAT_00bcd778 + param_1) = DAT_00f5603c;
      return;
    }
  }
  *(undefined4 *)(&DAT_00bcd778 + param_1) = DAT_00f5603c;
  return;
}
