#include "restore_types.h"
#define LAB_004e50f0 (*(undefined1 *)0x004e50f0u)
#define LAB_0056b626 (*(undefined1 *)0x0056b626u)
#define PTR_FUN_0056f4dc (*(unsigned char * *)0x0056f4dcu)
#define s_caesareum10_00579370 (*(char (*)[12])0x00579370u)
#define s_caesareum11_00579364 (*(char (*)[12])0x00579364u)
#define s_caesareum12_00579358 (*(char (*)[12])0x00579358u)
#define s_caesareum13_0057934c (*(char (*)[12])0x0057934cu)
#define s_caesareum14_00579340 (*(char (*)[12])0x00579340u)
#define s_caesareum15_00579334 (*(char (*)[12])0x00579334u)
#define s_caesareum16_00579328 (*(char (*)[12])0x00579328u)
#define s_caesareum17_0057931c (*(char (*)[12])0x0057931cu)
#define s_caesareum18_00579310 (*(char (*)[12])0x00579310u)
#define s_caesareum19_00579304 (*(char (*)[12])0x00579304u)
#define s_caesareum1_005793dc (*(char (*)[11])0x005793dcu)
#define s_caesareum20_005792f8 (*(char (*)[12])0x005792f8u)
#define s_caesareum2_005793d0 (*(char (*)[11])0x005793d0u)
#define s_caesareum3_005793c4 (*(char (*)[11])0x005793c4u)
#define s_caesareum4_005793b8 (*(char (*)[11])0x005793b8u)
#define s_caesareum5_005793ac (*(char (*)[11])0x005793acu)
#define s_caesareum6_005793a0 (*(char (*)[11])0x005793a0u)
#define s_caesareum7_00579394 (*(char (*)[11])0x00579394u)
#define s_caesareum8_00579388 (*(char (*)[11])0x00579388u)
#define s_caesareum9_0057937c (*(char (*)[11])0x0057937cu)
extern void FUN_004e5120();
extern void FUN_004e5140();
extern void FUN_004e5210();
extern void FUN_0051f780();
extern void FUN_00561441();

void FUN_00416c20(void)

{
  undefined4 uVar1;
  undefined4 *in_ECX;
  CRect local_40 [52];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0056b626;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_004e5210();
  local_4 = 0;
  FUN_00561441(in_ECX + 0x13,0x34,9,&LAB_004e50f0,FUN_0051f780);
  RESTORE_SLICE1(local_4,0) = 1;
  *in_ECX = &PTR_FUN_0056f4dc;
  in_ECX[0x88] = s_caesareum1_005793dc;
  in_ECX[0x89] = s_caesareum2_005793d0;
  in_ECX[0x8a] = s_caesareum3_005793c4;
  in_ECX[0x8b] = s_caesareum4_005793b8;
  in_ECX[0x8c] = s_caesareum5_005793ac;
  in_ECX[0x8d] = s_caesareum6_005793a0;
  in_ECX[0x8e] = s_caesareum7_00579394;
  in_ECX[0x8f] = s_caesareum8_00579388;
  in_ECX[0x90] = s_caesareum9_0057937c;
  in_ECX[0x91] = s_caesareum10_00579370;
  in_ECX[0x92] = s_caesareum11_00579364;
  in_ECX[0x93] = s_caesareum12_00579358;
  in_ECX[0x94] = s_caesareum13_0057934c;
  in_ECX[0x95] = s_caesareum14_00579340;
  in_ECX[0x96] = s_caesareum15_00579334;
  in_ECX[0x97] = s_caesareum16_00579328;
  in_ECX[0x98] = s_caesareum17_0057931c;
  in_ECX[0x99] = s_caesareum18_00579310;
  in_ECX[0x9a] = s_caesareum19_00579304;
  in_ECX[0x9b] = s_caesareum20_005792f8;
  uVar1 = (FUN_004e5120(local_40,-7,-7,5,0), restore_pointer32_checked(local_40, 0x004e5120u));
  RESTORE_SLICE1(local_4,0) = 2;
  FUN_004e5140(uVar1);
  RESTORE_SLICE1(local_4,0) = 1;
  FUN_0051f780();
  uVar1 = (FUN_004e5120(local_40,-7,-2,5,1), restore_pointer32_checked(local_40, 0x004e5120u));
  RESTORE_SLICE1(local_4,0) = 3;
  FUN_004e5140(uVar1);
  RESTORE_SLICE1(local_4,0) = 1;
  FUN_0051f780();
  uVar1 = (FUN_004e5120(local_40,-7,3,5,2), restore_pointer32_checked(local_40, 0x004e5120u));
  RESTORE_SLICE1(local_4,0) = 4;
  FUN_004e5140(uVar1);
  RESTORE_SLICE1(local_4,0) = 1;
  FUN_0051f780();
  uVar1 = (FUN_004e5120(local_40,-2,-7,5,3), restore_pointer32_checked(local_40, 0x004e5120u));
  RESTORE_SLICE1(local_4,0) = 5;
  FUN_004e5140(uVar1);
  RESTORE_SLICE1(local_4,0) = 1;
  FUN_0051f780();
  uVar1 = (FUN_004e5120(local_40,-2,-2,5,4), restore_pointer32_checked(local_40, 0x004e5120u));
  RESTORE_SLICE1(local_4,0) = 6;
  FUN_004e5140(uVar1);
  RESTORE_SLICE1(local_4,0) = 1;
  FUN_0051f780();
  uVar1 = (FUN_004e5120(local_40,-2,3,5,5), restore_pointer32_checked(local_40, 0x004e5120u));
  RESTORE_SLICE1(local_4,0) = 7;
  FUN_004e5140(uVar1);
  RESTORE_SLICE1(local_4,0) = 1;
  FUN_0051f780();
  uVar1 = (FUN_004e5120(local_40,3,-7,5,6), restore_pointer32_checked(local_40, 0x004e5120u));
  RESTORE_SLICE1(local_4,0) = 8;
  FUN_004e5140(uVar1);
  RESTORE_SLICE1(local_4,0) = 1;
  FUN_0051f780();
  uVar1 = (FUN_004e5120(local_40,3,-2,5,7), restore_pointer32_checked(local_40, 0x004e5120u));
  RESTORE_SLICE1(local_4,0) = 9;
  FUN_004e5140(uVar1);
  RESTORE_SLICE1(local_4,0) = 1;
  FUN_0051f780();
  uVar1 = (FUN_004e5120(local_40,3,3,5,8), restore_pointer32_checked(local_40, 0x004e5120u));
  RESTORE_SLICE1(local_4,0) = 10;
  FUN_004e5140(uVar1);
  local_4 = CONCAT31(RESTORE_SLICE3(local_4,1),1);
  FUN_0051f780();
  in_ECX[1] = 2;
  ExceptionList = local_c;
  return;
}
