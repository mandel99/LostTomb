#include "restore_types.h"
#define DAT_005d0050 (*(undefined4 *)0x005d0050u)
#define DAT_005d0070 (*(undefined4 *)0x005d0070u)
#define DAT_005d00b8 (*(undefined4 *)0x005d00b8u)
#define DAT_005d0138 (*(undefined4 *)0x005d0138u)
#define DAT_005d0200 (*(undefined4 *)0x005d0200u)
#define DAT_005d1ff8 (*(undefined4 *)0x005d1ff8u)
#define DAT_005d1ffc (*(undefined4 *)0x005d1ffcu)
#define DAT_005d267c (*(undefined4 *)0x005d267cu)
#define DAT_005d268c (*(undefined4 *)0x005d268cu)
#define DAT_005d26b0 (*(undefined4 *)0x005d26b0u)
#define DAT_005d26f0 (*(undefined4 *)0x005d26f0u)
#define DAT_005d2754 (*(undefined4 *)0x005d2754u)
#define DAT_0070e440 (*(undefined *)0x0070e440u)
#define DAT_0070e444 (*(undefined *)0x0070e444u)
#define DAT_0070e448 (*(undefined *)0x0070e448u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e450 (*(undefined *)0x0070e450u)
#define DAT_0070e454 (*(undefined *)0x0070e454u)
#define DAT_0070e458 (*(undefined *)0x0070e458u)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00799a9a (*(undefined4 *)0x00799a9au)
#define DAT_00799ad8 (*(undefined4 *)0x00799ad8u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
#define DAT_00e94e0c (*(undefined *)0x00e94e0cu)
#define DAT_00f56bdc (*(undefined *)0x00f56bdcu)
extern uint FUN_00431690();
extern void FUN_00432920();
extern int FUN_00475b50();
extern void FUN_004f0ba0();
extern int FUN_0053e590();

void FUN_00432d30(int param_1,int param_2,int param_3,int param_4)

{
  short sVar1;
  short sVar2;
  int iVar3;
  bool bVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  byte bVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  int *piVar13;
  undefined4 local_8;
  undefined4 local_4;
  
  if (param_1 == 0) {
    return;
  }
  if (DAT_00e94e0c == 0x1f) {
    FUN_00432920(param_1);
  }
  iVar6 = FUN_00475b50(param_1);
  if ((*(int *)(&DAT_00799ad8 + iVar6 * 0x108) == 0) || (DAT_00e94e0c != 0x1f)) {
    bVar4 = false;
  }
  else {
    bVar4 = true;
  }
  cVar5 = FUN_00431690(iVar6);
  if (cVar5 == '\0') {
    iVar6 = param_1 * 0x108;
    sVar1 = (&DAT_007999e8)[param_1 * 0x84];
    if ((sVar1 == 0xd1) || (bVar4)) goto LAB_0043366b;
    bVar8 = (&DAT_00b285cc)[DAT_0070e488] & 0xf;
    if (((sVar1 == 0x1f) || ((sVar1 == 0x1e || (sVar1 == 0x21)))) &&
       ((*(byte *)(&DAT_00af598c + DAT_0070e488) & 0x60) != 0)) {
      DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
      switch((int)(bVar8)) {
      case 1: ;
        FUN_0053e590(DAT_00d3b5c0,DAT_0070e45c + 1 + param_2,param_3 - DAT_0070e44c);
        return;
      case 2: ;
        FUN_0053e590(DAT_00d3b5c0,DAT_0070e458 + 2 + param_2,param_3 - DAT_0070e448);
        return;
      case 3: ;
        FUN_0053e590(DAT_00d3b5c0,DAT_0070e454 + 3 + param_2,param_3 - DAT_0070e444);
        return;
      case 4: ;
        FUN_0053e590(DAT_00d3b5c0,DAT_0070e450 + 4 + param_2,param_3 - DAT_0070e440);
        return;
      case 5: ;
        param_2 = DAT_0070e450 + DAT_0070e45c + 5 + param_2;
        param_3 = (param_3 - DAT_0070e440) - DAT_0070e44c;
      }
      FUN_0053e590(DAT_00d3b5c0,param_2,param_3);
      return;
    }
    if (sVar1 == 0x48) {
      if (DAT_00bca2e0 == 0) {
        param_2 = param_2 + -0x3c;
        param_3 = param_3 + 0x1e;
      }
      else if (DAT_00bca2e0 == 4) {
        param_2 = param_2 + -0x3c;
        param_3 = param_3 + -0x1e;
      }
      else if (DAT_00bca2e0 == 6) {
        param_2 = param_2 + -0x78;
      }
      piVar12 = &DAT_005d0070;
      piVar13 = &DAT_005d268c;
      do {
        DAT_00d3b5c0 = DAT_00f56bdc + *piVar13;
        if (*(char *)((int)&DAT_007999dc + iVar6 + 1) != '\0') {
          DAT_00d3b5c0 = DAT_00d3b5c0 + 4;
        }
        FUN_0053e590(DAT_00d3b5c0,param_2 + *piVar12,piVar12[1] + param_3);
        piVar13 = piVar13 + 1;
        piVar12 = piVar12 + 2;
      } while ((int)piVar13 < 0x5d26b0);
      return;
    }
    if (sVar1 == 0x49) {
      return;
    }
    if ((sVar1 < 0x41) || (0x45 < sVar1)) {
      if (((&DAT_00b285cc)[DAT_0070e488] & 0xf) != 0) {
        if (bVar8 == 1) {
          if (DAT_00e94e0c == 0x1f) {
            iVar6 = FUN_00475b50(param_1);
            iVar6 = iVar6 * 0x108;
            if (*(int *)(&DAT_00799ad8 + iVar6) != 0) {
              DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
              FUN_0053e590(DAT_00d3b5c0,DAT_0070e45c + 1 + param_2,param_3 - DAT_0070e44c);
              return;
            }
          }
          piVar12 = &DAT_005d0050;
          piVar13 = &DAT_005d267c;
          do {
            DAT_00d3b5c0 = DAT_00f56bdc + *piVar13 + param_4;
            if (*(char *)((int)&DAT_007999dc + iVar6 + 1) != '\0') {
              DAT_00d3b5c0 = DAT_00d3b5c0 + 4;
            }
            FUN_0053e590(DAT_00d3b5c0,*piVar12 + param_2,piVar12[1] + param_3);
            piVar13 = piVar13 + 1;
            piVar12 = piVar12 + 2;
          } while ((int)piVar13 < 0x5d268c);
          return;
        }
        if (bVar8 == 2) {
          if (DAT_00e94e0c == 0x1f) {
            iVar6 = FUN_00475b50(param_1);
            iVar6 = iVar6 * 0x108;
            if (*(int *)(&DAT_00799ad8 + iVar6) != 0) {
              DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
              FUN_0053e590(DAT_00d3b5c0,DAT_0070e458 + 2 + param_2,param_3 - DAT_0070e448);
              return;
            }
          }
          piVar12 = &DAT_005d0070;
          piVar13 = &DAT_005d268c;
          do {
            DAT_00d3b5c0 = *piVar13 + DAT_00f56bdc + param_4;
            if (*(char *)((int)&DAT_007999dc + iVar6 + 1) != '\0') {
              DAT_00d3b5c0 = DAT_00d3b5c0 + 4;
            }
            FUN_0053e590(DAT_00d3b5c0,*piVar12 + param_2,piVar12[1] + param_3);
            piVar13 = piVar13 + 1;
            piVar12 = piVar12 + 2;
          } while ((int)piVar13 < 0x5d26b0);
          return;
        }
        if (bVar8 == 3) {
          if ((DAT_00e94e0c == 0x1f) && (*(int *)(&DAT_00799ad8 + iVar6) != 0)) {
            FUN_00475b50(param_1);
            DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
            FUN_0053e590(DAT_00d3b5c0,DAT_0070e454 + 3 + param_2,param_3 - DAT_0070e444);
            return;
          }
          piVar12 = &DAT_005d00b8;
          piVar13 = &DAT_005d26b0;
          do {
            DAT_00d3b5c0 = *piVar13 + DAT_00f56bdc + param_4;
            if (*(char *)((int)&DAT_007999dc + iVar6 + 1) != '\0') {
              DAT_00d3b5c0 = DAT_00d3b5c0 + 4;
            }
            FUN_0053e590(DAT_00d3b5c0,*piVar12 + param_2,piVar12[1] + param_3);
            piVar13 = piVar13 + 1;
            piVar12 = piVar12 + 2;
          } while ((int)piVar13 < 0x5d26f0);
          return;
        }
        if (bVar8 != 4) {
          if (bVar8 != 5) {
            return;
          }
          if ((DAT_00e94e0c == 0x1f) && (*(int *)(&DAT_00799ad8 + iVar6) != 0)) {
            FUN_00475b50(param_1);
            DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
            FUN_0053e590(DAT_00d3b5c0,DAT_0070e450 + DAT_0070e45c + 5 + param_2,
                         (param_3 - DAT_0070e440) - DAT_0070e44c);
            return;
          }
          piVar12 = &DAT_005d0200;
          piVar13 = &DAT_005d2754;
          do {
            DAT_00d3b5c0 = DAT_00f56bdc + *piVar13 + param_4;
            if (*(char *)((int)&DAT_007999dc + iVar6 + 1) != '\0') {
              DAT_00d3b5c0 = DAT_00d3b5c0 + 4;
            }
            FUN_0053e590(DAT_00d3b5c0,*piVar12 + param_2,piVar12[1] + param_3);
            piVar13 = piVar13 + 1;
            piVar12 = piVar12 + 2;
          } while ((int)piVar13 < 0x5d27e4);
          return;
        }
        if ((DAT_00e94e0c == 0x1f) && (*(int *)(&DAT_00799ad8 + iVar6) != 0)) {
          FUN_00475b50(param_1);
          DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
          FUN_0053e590(DAT_00d3b5c0,DAT_0070e450 + 4 + param_2,param_3 - DAT_0070e440);
          return;
        }
        piVar13 = &DAT_005d0138;
        piVar12 = &DAT_005d26f0;
        do {
          DAT_00d3b5c0 = DAT_00f56bdc + param_4 + *piVar12;
          if (*(char *)((int)&DAT_007999dc + iVar6 + 1) != '\0') {
            DAT_00d3b5c0 = DAT_00d3b5c0 + 4;
          }
          FUN_0053e590(DAT_00d3b5c0,param_2 + *piVar13,piVar13[1] + param_3);
          piVar12 = piVar12 + 1;
          piVar13 = piVar13 + 2;
        } while ((int)piVar12 < 0x5d2754);
        return;
      }
      DAT_00d3b5c0 = DAT_00f56bdc;
      if (*(char *)((int)&DAT_007999dc + iVar6 + 1) == '\0') {
        if ((DAT_00e94e0c == 0x1f) &&
           (iVar6 = FUN_00475b50(param_1), *(int *)(&DAT_00799ad8 + iVar6 * 0x108) != 0)) {
          DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
          goto LAB_00433192;
        }
      }
      else if ((DAT_00e94e0c == 0x1f) && (*(char *)((int)&DAT_00799a9a + iVar6 + 1) != '\0')) {
        DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
      }
      else {
        DAT_00d3b5c0 = DAT_00f56bdc + 4;
      }
      DAT_00d3b5c0 = DAT_00d3b5c0 + param_4;
LAB_00433192: ;
      FUN_0053e590(DAT_00d3b5c0,param_2,param_3);
      return;
    }
    if (*(short *)(&DAT_00799a10 + iVar6) != 0) {
      return;
    }
    sVar1 = *(short *)((int)&DAT_007999dc + iVar6 + 2);
    iVar7 = (uint)(*(byte *)(&DAT_00799a82 + param_1 * 0x84) >> 1) * 8;
    iVar3 = *(int *)(&DAT_005d1ff8 + iVar7);
    iVar7 = *(int *)(&DAT_005d1ffc + iVar7);
    sVar2 = *(short *)(&DAT_007999e0 + iVar6);
    switch(*(byte *)(&DAT_00799a82 + param_1 * 0x84)) {
    case 0: ;
      iVar6 = 0xd;
      iVar9 = 7;
      param_4 = 0xd;
      break;
default: ;
      return;
    case 2: ;
      iVar6 = 7;
      iVar9 = 0xd;
      param_4 = 7;
      param_1 = 5;
      goto LAB_00432fb4;
    case 4: ;
      iVar6 = 0xd;
      iVar9 = 7;
      param_4 = 0xd;
      param_1 = 5;
      goto LAB_00432fb4;
    case 6: ;
      iVar6 = 7;
      iVar9 = 0xd;
      param_4 = 7;
    }
    param_1 = 1;
LAB_00432fb4: ;
    iVar10 = 0;
    if (iVar6 == 0) {
      return;
    }
    do {
      iVar6 = 0;
      if (iVar9 != 0) {
        do {
          if (DAT_00bca2e0 == 0) {
            if (iVar10 == 0) {
              if (iVar6 == 0) {
                iVar11 = 0;
              }
              else {
                if (iVar6 < 1) goto LAB_004330b2;
                iVar11 = 1;
              }
            }
            else if ((iVar10 < 1) || (iVar6 != 0)) {
LAB_004330b2: ;
              iVar11 = 3;
            }
            else {
              iVar11 = 2;
            }
          }
          else if (DAT_00bca2e0 == 2) {
            if (iVar10 == param_4 + -1) {
              if (iVar6 == 0) {
                iVar11 = 0;
              }
              else {
                if ((iVar10 != param_4 + -1) || (iVar6 < 1)) goto LAB_00433081;
                iVar11 = 2;
              }
            }
            else {
LAB_00433081: ;
              if ((iVar10 < 1) || (iVar6 != 0)) goto LAB_004330b2;
              iVar11 = 1;
            }
          }
          else if (DAT_00bca2e0 == 4) {
            if (iVar10 == param_4 + -1) {
              if (iVar6 == iVar9 + -1) {
                iVar11 = 0;
              }
              else {
                if ((iVar10 != param_4 + -1) || (iVar6 < 0)) goto LAB_00433051;
                iVar11 = 1;
              }
            }
            else {
LAB_00433051: ;
              if ((iVar10 < 0) || (iVar6 != iVar9 + -1)) goto LAB_004330b2;
              iVar11 = 2;
            }
          }
          else if (iVar10 == 0) {
            if (iVar6 == iVar9 + -1) {
              iVar11 = 0;
            }
            else {
              if (iVar6 < 0) goto LAB_0043301b;
              iVar11 = 2;
            }
          }
          else {
            if (iVar10 < 0) goto LAB_004330b2;
LAB_0043301b: ;
            if (iVar6 != iVar9 + -1) goto LAB_004330b2;
            iVar11 = 1;
          }
          FUN_004f0ba0(iVar10 * param_1 + sVar1 + iVar3,iVar6 * param_1 + sVar2 + iVar7,&local_4,
                       &local_8);
          DAT_00d3b5c0 = DAT_00f56bdc + iVar11;
          FUN_0053e590(DAT_00d3b5c0,local_4,local_8);
          iVar6 = iVar6 + 1;
        } while (iVar6 < iVar9);
      }
      iVar10 = iVar10 + 1;
      if (param_4 <= iVar10) {
        return;
      }
    } while( true );
  }
LAB_0043366b: ;
  DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
  switch((&DAT_00b285cc)[DAT_0070e488] & 0xf) {
  case 1: ;
    iVar6 = DAT_0070e45c + 1;
    goto LAB_004336fc;
  case 2: ;
    param_2 = DAT_0070e458 + 2 + param_2;
    iVar6 = DAT_0070e448;
    break;
  case 3: ;
    param_2 = DAT_0070e454 + 3 + param_2;
    iVar6 = DAT_0070e444;
    break;
  case 4: ;
    param_2 = DAT_0070e450 + 4 + param_2;
    iVar6 = DAT_0070e440;
    break;
  case 5: ;
    param_3 = param_3 - DAT_0070e440;
    iVar6 = DAT_0070e450 + DAT_0070e45c + 5;
LAB_004336fc: ;
    param_2 = iVar6 + param_2;
    iVar6 = DAT_0070e44c;
    break;
default: ;
    goto switchD_00433699_default;
  }
  param_3 = param_3 - iVar6;
switchD_00433699_default: ;
  FUN_0053e590(DAT_00d3b5c0,param_2,param_3);
  return;
}
