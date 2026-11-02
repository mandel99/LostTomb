#include "restore_types.h"
#define DAT_005df768 (*(undefined4 *)0x005df768u)
#define DAT_005df778 (*(undefined4 *)0x005df778u)
#define DAT_005df814 (*(undefined4 *)0x005df814u)
#define DAT_005df818 (*(undefined4 *)0x005df818u)
#define DAT_005df81c (*(undefined4 *)0x005df81cu)
#define DAT_00605994 (*(undefined *)0x00605994u)
#define DAT_0070e120 (*(undefined *)0x0070e120u)
#define DAT_00bc9714 (*(undefined *)0x00bc9714u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca304 (*(undefined *)0x00bca304u)
#define DAT_00bca308 (*(undefined *)0x00bca308u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
extern undefined4 FUN_004bff60();
extern void FUN_004c0040();
extern int FUN_004c53d0();
extern void FUN_004f0ba0();
extern int FUN_0053e590();
extern undefined4 FUN_0053f250();

void FUN_004bfcd0(void)

{
  int iVar1;
  int iVar2;
  char cVar3;
  SHORT SVar4;
  int iVar5;
  int iVar6;
  int iVar8;
  int *piVar9;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  int local_c;
  int local_8;
  int local_4;
  int iVar7;
  
  iVar5 = FUN_004c53d0(2);
  if ((iVar5 == -1) && (iVar5 = FUN_004c53d0(1), iVar5 == -1)) {
    iVar5 = 0;
  }
  iVar6 = DAT_00bca308;
  iVar2 = DAT_00bca304;
  local_4 = (&DAT_005df778)[iVar5];
  local_c = DAT_00bca308;
  SVar4 = GetAsyncKeyState(0x52);
  if (SVar4 == -0x7fff) {
    DAT_00bc9714 = (uint)(DAT_00bc9714 != 1);
  }
  cVar3 = FUN_004bff60(iVar2,iVar6,DAT_00bc9714);
  *(char *)((int)&local_18 + DAT_00bc9714) = cVar3;
  if (cVar3 == '\0') {
    DAT_00605994 = 0;
    FUN_004c0040(iVar2,iVar6,DAT_00bc9714);
    return;
  }
  local_18 = DAT_00bca2e0 + DAT_00bc9714;
  if (7 < local_18) {
    local_18 = local_18 + -8;
  }
  piVar9 = &DAT_005df768;
  local_8 = local_18 * 0x10 + 0x24;
  do {
    iVar5 = *(int *)(local_8 + (int)piVar9);
    switch((int)(local_18)) {
    case 0: ;
    case 3: ;
    case 4: ;
    case 7: ;
      iVar8 = 1;
      break;
default: ;
      iVar8 = 0;
    }
    if (DAT_00bc9714 == 0) {
      iVar1 = *(int *)(&DAT_005df81c + iVar5 * 0x10);
      iVar7 = *(int *)(&DAT_005df818 + iVar5 * 0x10);
LAB_004bfdfe: ;
      iVar6 = iVar7 + iVar6;
      iVar7 = iVar1 + iVar2;
    }
    else {
      iVar7 = iVar2;
      if (DAT_00bc9714 == 1) {
        iVar1 = *(int *)(&DAT_005df818 + iVar5 * 0x10);
        iVar7 = *(int *)(&DAT_005df81c + iVar5 * 0x10);
        goto LAB_004bfdfe;
      }
    }
    if (DAT_00bca2e0 != 0) {
      if (DAT_00bca2e0 == 2) {
        iVar7 = iVar7 + -1 + (&DAT_005df814)[iVar5 * 4];
      }
      else {
        if (DAT_00bca2e0 == 4) {
          iVar5 = (&DAT_005df814)[iVar5 * 4];
          iVar7 = iVar7 + -1 + iVar5;
        }
        else {
          iVar5 = (&DAT_005df814)[iVar5 * 4];
        }
        iVar6 = iVar6 + -1 + iVar5;
      }
    }
    iVar5 = *piVar9;
    if (*piVar9 == 0x1ae) {
      iVar5 = local_4;
    }
    iVar5 = (&DAT_00f55bb4)[iVar5 * 2];
    FUN_004f0ba0(iVar7,iVar6,&local_10,&local_14);
    DAT_0070e120 = 0x1fe3;
    FUN_0053e590(iVar5 + iVar8,local_10,local_14);
    FUN_0053f250(iVar5 + iVar8,local_10,local_14);
    piVar9 = piVar9 + 1;
    DAT_0070e120 = 0;
    iVar6 = local_c;
    if (0x5df777 < (int)piVar9) {
      return;
    }
  } while( true );
}
