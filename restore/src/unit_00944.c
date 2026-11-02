#include "restore_types.h"
#define DAT_0077567c (*(undefined *)0x0077567cu)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a90 (*(undefined4 *)0x00799a90u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea44f4 (*(undefined4 *)0x00ea44f4u)
#define DAT_00ea5cc0 (*(undefined4 *)0x00ea5cc0u)
#define DAT_00ea5cfc (*(undefined4 *)0x00ea5cfcu)
#define DAT_00ea5ec8 (*(undefined4 *)0x00ea5ec8u)
#define DAT_00ea5fd4 (*(undefined4 *)0x00ea5fd4u)
#define DAT_00ea5ff4 (*(undefined4 *)0x00ea5ff4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_0043fd00();
extern undefined4 FUN_00459ec0();
extern undefined4 FUN_00459ef0();
extern void FUN_004684e0();
extern void FUN_004695d0();
extern int FUN_0046d0f0();
extern undefined4 FUN_00474380();
extern int FUN_00475b50();
extern void FUN_0047afd0();
extern void FUN_0047bae0();
extern void FUN_0047c2b0();
extern void FUN_0048c0e0();
extern void FUN_00497b90();
extern void FUN_004b7510();
extern void FUN_004c03d0();
extern int FUN_004cde40();
extern void FUN_004e2970();
extern void FUN_00517370();
extern void FUN_00517660();
extern void FUN_005178e0();

void FUN_00469140(int param_1,int param_2,int param_3)

{
  short sVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 local_14 [5];
  
  local_14[0] = 0;
  local_14[1] = 0xf;
  local_14[2] = 0x1e;
  local_14[3] = 0x3c;
  local_14[4] = 100;
  if (*(short *)(&DAT_00b4e6fc + param_3 * 2) < 1) {
    if (((&DAT_00af598c)[param_3] & 0x4000) != 0) {
      FUN_004b7510(param_1,param_2);
    }
    FUN_0047c2b0(0,param_1,param_2,1,1);
  }
  else {
    iVar3 = FUN_00475b50((int)*(short *)(&DAT_00b4e6fc + param_3 * 2));
    iVar7 = iVar3 * 0x108;
    cVar2 = FUN_00474380((int)(short)(&DAT_007999e8)[iVar3 * 0x84]);
    if (cVar2 != '\0') {
      return;
    }
    sVar1 = (&DAT_007999e8)[iVar3 * 0x84];
    if (sVar1 == 0xca) {
      FUN_004c03d0(iVar3);
    }
    else if ((((sVar1 == 0x1f) || (sVar1 == 0x1e)) || (sVar1 == 0x21)) || (sVar1 == 0xd1)) {
      iVar5 = FUN_0046d0f0(iVar3);
      if (0 < iVar5) {
        DAT_0077567c = 1;
        FUN_0043fd00(iVar5,(int)(short)(&DAT_007999e8)[iVar5 * 0x84]);
      }
    }
    else {
      FUN_0047c2b0(iVar3,(int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                   (int)*(short *)(&DAT_007999e0 + iVar7),(&DAT_007999db)[iVar7],
                   (&DAT_007999db)[iVar7]);
    }
    if ((&DAT_007999d8)[iVar7] == '\x01') {
      sVar1 = (&DAT_007999e8)[iVar3 * 0x84];
      if ((((((sVar1 != 10) && (sVar1 != 0xb)) &&
            ((sVar1 != 0x37 && ((sVar1 != 0x51 && (sVar1 != 0x5c)))))) && (sVar1 != 0x39)) &&
          ((((sVar1 != 0x36 && (sVar1 != 0x3a)) && (sVar1 != 0xaa)) &&
           ((sVar1 != 0xab && (sVar1 != 200)))))) &&
         (((sVar1 != 0xc9 && ((sVar1 != 0xca && (sVar1 != 0x3b)))) &&
          ((sVar1 != 0xac && (sVar1 != 0xad)))))) {
        iVar4 = FUN_00459ec0(iVar3);
        iVar6 = DAT_00eaac28;
        iVar5 = DAT_00e39080;
        if (iVar4 == 0) {
          iVar6 = FUN_00459ef0(iVar3);
          iVar5 = DAT_00e39080;
          if (iVar6 == 0) {
            (&DAT_00ea5ec8)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5ec8)[DAT_00eaac28 * 0x1276] + 1;
          }
          else {
            (&DAT_00ea5ec8)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5ec8)[DAT_00eaac28 * 0x1276] + 1;
            iVar5 = FUN_004cde40((&DAT_00799a90)[iVar3 * 0x42],local_14[iVar5]);
            if (iVar5 != 0) {
              FUN_004684e0();
              iVar6 = DAT_00eaac28;
              DAT_00e950dc = 1;
              iVar4 = DAT_00eaac28 * 0x49d8;
              *(short *)(&DAT_00ea5cfc + DAT_00eaac28 * 0x1276) =
                   *(short *)(&DAT_00ea5cfc + DAT_00eaac28 * 0x1276) + 1;
              (&DAT_00ea44f4)[iVar6 * 0x1276] = (&DAT_00ea44f4)[iVar6 * 0x1276] + iVar5;
              *(int *)(&DAT_00ea5fd4 + iVar4) = *(int *)(&DAT_00ea5fd4 + iVar4) + iVar5;
              *(int *)(&DAT_00ea187c + iVar4) = *(int *)(&DAT_00ea187c + iVar4) - iVar5;
              switch((&DAT_007999e8)[iVar3 * 0x84]) {
              case 0x54: ;
              case 0x55: ;
              case 0x56: ;
              case 0x57: ;
                FUN_004e2970(0xc9,0,0);
                break;
              case 0xb8: ;
                FUN_004e2970(0xca,0,0);
                break;
              case 0xbb: ;
              case 0xbc: ;
              case 0xbd: ;
                FUN_004e2970(200,0,0);
              }
            }
          }
        }
        else {
          (&DAT_00ea5ec8)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5ec8)[DAT_00eaac28 * 0x1276] + 1;
          iVar5 = FUN_004cde40((&DAT_00ea5cc0)[iVar6 * 0x1276],local_14[iVar5]);
          if (iVar5 != 0) {
            iVar6 = DAT_00eaac28 * 0x49d8;
            (&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276] - iVar5;
            *(int *)(&DAT_00ea5ff4 + iVar6) = *(int *)(&DAT_00ea5ff4 + iVar6) + iVar5;
            FUN_004e2970(199,0,0);
          }
        }
      }
      if (0xb < (int)(&DAT_00ea5ec8)[DAT_00eaac28 * 0x1276]) {
        (&DAT_00ea5ec8)[DAT_00eaac28 * 0x1276] = 0xc;
      }
      (&DAT_007999d8)[iVar7] = 4;
      FUN_0048c0e0((int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                   (int)*(short *)(&DAT_007999e0 + iVar7),(&DAT_007999db)[iVar7],1);
      FUN_004695d0(iVar3,0);
    }
  }
  FUN_00497b90();
  FUN_0047afd0(param_1,param_2,3);
  FUN_0047bae0(param_1 + -2,param_2 + -2,param_1 + 2,param_2 + 2,0);
  FUN_00517370();
  FUN_00517660();
  FUN_005178e0();
  return;
}
