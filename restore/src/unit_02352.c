#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_00605a00 (*(undefined *)0x00605a00u)
#define DAT_00605a04 (*(undefined *)0x00605a04u)
#define DAT_00605a08 (*(undefined *)0x00605a08u)
#define DAT_00605a0c (*(undefined *)0x00605a0cu)
#define DAT_006de514 (*(char *)0x006de514u)
#define DAT_00784b60 (*(undefined *)0x00784b60u)
#define DAT_00784c48 (*(undefined *)0x00784c48u)
#define DAT_00784c50 (*(undefined *)0x00784c50u)
#define DAT_0078d7b8 (*(undefined *)0x0078d7b8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38e60 (*(undefined1 *)0x00e38e60u)
#define DAT_00e92dfc (*(undefined *)0x00e92dfcu)
#define DAT_00ea1880 (*(undefined4 *)0x00ea1880u)
#define DAT_00ea5d40 (*(undefined4 *)0x00ea5d40u)
#define DAT_00ea5f18 (*(undefined4 *)0x00ea5f18u)
#define DAT_00ea6018 (*(undefined4 *)0x00ea6018u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f70 (*(undefined *)0x00f53f70u)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define s_Allow_All_005f89c0 (*(char (*)[11])0x005f89c0u)
#define s_Event_005f896c (*(char (*)[6])0x005f896cu)
#define s_Happiness_005f89b4 (*(char (*)[11])0x005f89b4u)
#define s_Level_005f8964 (*(char (*)[6])0x005f8964u)
#define s_Major_Bad_005f8980 (*(char (*)[10])0x005f8980u)
#define s_Major_Good_005f8998 (*(char (*)[11])0x005f8998u)
#define s_Max_prosperity_005f89a4 (*(char (*)[16])0x005f89a4u)
#define s_Minor_Bad_005f8974 (*(char (*)[10])0x005f8974u)
#define s_Minor_Good_005f898c (*(char (*)[11])0x005f898cu)
extern int FUN_00430cc0();
extern void FUN_004cc770();
extern void FUN_004cda40();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d6860();
extern void FUN_0051f780();
extern void FUN_00536820();

void FUN_0051f790(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  short sVar5;
  char *pcVar6;
  int local_4;
  
  uVar2 = DAT_00f56f84;
  if (DAT_006de514 == '\0') {
    if ((&DAT_00ea5d40)[DAT_00eaac28 * 0x1276] == 0) {
      if (DAT_00784b60 == 0) {
        if (DAT_00784c48 == 0) {
          if (DAT_00784c50 != 0) {
            if (DAT_0078d7bc + 1 < DAT_0078d7b8) {
              iVar3 = (DAT_0078d7b8 - DAT_0078d7bc) + -1;
            }
            else {
              iVar3 = 0;
            }
            iVar3 = (iVar3 * 3 + 3) * 4 - DAT_0078d7c0;
            FUN_004cda40(1,0x2a,200,0x12,0x1fe3);
            DAT_00e20778 = 0;
            FUN_004d62f0(6,3,2,0x2e,DAT_00f56f84,0xf800,DAT_00e38da8,DAT_00e38da4);
            FUN_004d6860(iVar3,0x40,&DAT_0057c048,DAT_00e20778 + 2,0x2e,DAT_00f56f84,0xf800,
                         DAT_00e38da8,DAT_00e38da4);
          }
        }
        else {
          if (*(int *)(&DAT_00ea6018 + DAT_00eaac28 * 0x49d8) == 0) {
            iVar3 = DAT_0078d7b8 + 7;
          }
          else {
            iVar3 = DAT_0078d7b8;
            if (*(int *)(&DAT_00ea6018 + DAT_00eaac28 * 0x49d8) == 1) {
              iVar3 = DAT_0078d7b8 + 2;
            }
          }
          if (DAT_0078d7bc + 1 < iVar3) {
            iVar3 = (iVar3 - DAT_0078d7bc) + -1;
          }
          else {
            iVar3 = 0;
          }
          iVar3 = (iVar3 * 3 + 3) * 4 - DAT_0078d7c0;
          FUN_004cda40(1,0x28,0xe6,0x14,0x1fe3);
          uVar2 = DAT_00f56f84;
          local_4 = 0;
          do {
            iVar4 = 0x2d;
            do {
              DAT_00e20778 = 0;
              FUN_004d62f0(6,2,local_4 + 7,iVar4,uVar2,0,DAT_00e38da8,DAT_00e38da4);
              FUN_004d6860(iVar3,0x40,&DAT_0057c048,DAT_00e20778 + local_4 + 7,iVar4,uVar2,0,
                           DAT_00e38da8,DAT_00e38da4);
              iVar1 = iVar4 + -0x2c;
              iVar4 = iVar4 + 2;
            } while (iVar1 < 2);
            local_4 = local_4 + 1;
          } while (local_4 < 2);
          DAT_00e20778 = 0;
          FUN_004d62f0(6,2,7,0x2e,uVar2,0xf800,DAT_00e38da8,DAT_00e38da4);
          FUN_004d6860(iVar3,0x40,&DAT_0057c048,DAT_00e20778 + 7,0x2e,uVar2,0xf800,DAT_00e38da8,
                       DAT_00e38da4);
        }
      }
    }
    else {
      DAT_00e20778 = 0;
      sVar5 = 0x7f9a;
      if (DAT_00e92dfc == 2) {
        sVar5 = -0xc6;
      }
      FUN_004d62f0(6,5,8,0x2f,DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004d62f0(6,5,7,0x2e,uVar2,(int)sVar5,DAT_00e38da8,DAT_00e38da4);
    }
  }
  else {
    DAT_00e20778 = 0;
    sVar5 = 0x7f9a;
    if (DAT_00e92dfc == 2) {
      sVar5 = -0xc6;
    }
    FUN_004d62f0(6,6,8,0x2f,DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(6,6,7,0x2e,uVar2,(int)sVar5,DAT_00e38da8,DAT_00e38da4);
  }
  DAT_00e20778 = 0;
  FUN_0051f780();
  FUN_0051f780();
  if (DAT_00605a0c == 1) {
    FUN_004cc770(s_Allow_All_005f89c0,DAT_00e20778 + 10,0x3d,DAT_00f56f84,0xffff);
  }
  if (DAT_00f53f70 != 0) {
    FUN_004d6860(*(undefined4 *)(&DAT_00ea1880 + DAT_00eaac28 * 0x49d8),0x40,s_Happiness_005f89b4,2,
                 0x50,DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(*(undefined4 *)(&DAT_00ea1880 + DAT_00eaac28 * 0x49d8),0x40,s_Happiness_005f89b4,1,
                 0x51,DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea5f18)[DAT_00eaac28 * 0x1276],0x40,s_Max_prosperity_005f89a4,2,100,
                 DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea5f18)[DAT_00eaac28 * 0x1276],0x40,s_Max_prosperity_005f89a4,1,0x65,
                 DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4);
  }
  iVar3 = DAT_00e20778;
  if (DAT_00605a00 == 0) goto LAB_0051fcf0;
  DAT_00e20778 = 0;
  FUN_004d62f0(0x9d,DAT_00605a08,0,0x79,DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4);
  switch((int)(DAT_00605a00)) {
  case 1: ;
    pcVar6 = s_Major_Good_005f8998;
    break;
  case 2: ;
    pcVar6 = s_Minor_Good_005f898c;
    break;
  case 3: ;
    pcVar6 = s_Major_Bad_005f8980;
    break;
  case 4: ;
    pcVar6 = s_Minor_Bad_005f8974;
    break;
default: ;
    goto switchD_0051fc13_default;
  }
  FUN_004cc770(pcVar6,DAT_00e20778 + 1,0x79,DAT_00f56f84,0xffff);
switchD_0051fc13_default: ;
  FUN_004cc770(s_Event_005f896c,DAT_00e20778,0x65,DAT_00f56f84,0xffff);
  FUN_004d6860(DAT_00605a04,0x40,s_Level_005f8964,DAT_00e20778 + 2,0x79,DAT_00f56f84,0xffff,
               DAT_00e38da8,DAT_00e38da4);
LAB_0051fcf0: ;
  DAT_00e20778 = iVar3;
  if (DAT_00e38e60 != '\0') {
    iVar3 = FUN_00430cc0();
    iVar3 = (iVar3 + -0x1c0) / 2;
    FUN_00536820(iVar3,0x28,0x1c,3);
    FUN_004d6250(0xd,2,iVar3,0x3a,0x1c0,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  }
  return;
}
