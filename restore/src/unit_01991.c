#include "restore_types.h"
#define LAB_004e50f0 (*(undefined1 *)0x004e50f0u)
#define LAB_0056b6fe (*(undefined1 *)0x0056b6feu)
#define PTR_FUN_0056f6e8 (*(unsigned char * *)0x0056f6e8u)
#define s_LtHouse10_005e48c8 (*(char (*)[10])0x005e48c8u)
#define s_LtHouse1_005e4934 (*(char (*)[9])0x005e4934u)
#define s_LtHouse2_005e4928 (*(char (*)[9])0x005e4928u)
#define s_LtHouse3_005e491c (*(char (*)[9])0x005e491cu)
#define s_LtHouse4_005e4910 (*(char (*)[9])0x005e4910u)
#define s_LtHouse5_005e4904 (*(char (*)[9])0x005e4904u)
#define s_LtHouse6_005e48f8 (*(char (*)[9])0x005e48f8u)
#define s_LtHouse7_005e48ec (*(char (*)[9])0x005e48ecu)
#define s_LtHouse8_005e48e0 (*(char (*)[9])0x005e48e0u)
#define s_LtHouse9_005e48d4 (*(char (*)[9])0x005e48d4u)
extern void FUN_004e5120();
extern void FUN_004e5140();
extern void FUN_004e5210();
extern void FUN_0051f780();
extern void FUN_00561441();

void FUN_004e6400(void)

{
  undefined4 uVar1;
  undefined4 *in_ECX;
  CRect local_40 [52];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0056b6fe;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_004e5210();
  local_4 = 0;
  FUN_00561441(in_ECX + 0x13,0x34,2,&LAB_004e50f0,FUN_0051f780);
  RESTORE_SLICE1(local_4,0) = 1;
  *in_ECX = &PTR_FUN_0056f6e8;
  in_ECX[0x2d] = s_LtHouse1_005e4934;
  in_ECX[0x2e] = s_LtHouse2_005e4928;
  in_ECX[0x2f] = s_LtHouse3_005e491c;
  in_ECX[0x30] = s_LtHouse4_005e4910;
  in_ECX[0x31] = s_LtHouse5_005e4904;
  in_ECX[0x32] = s_LtHouse6_005e48f8;
  in_ECX[0x33] = s_LtHouse7_005e48ec;
  in_ECX[0x34] = s_LtHouse8_005e48e0;
  in_ECX[0x35] = s_LtHouse9_005e48d4;
  in_ECX[0x36] = s_LtHouse10_005e48c8;
  uVar1 = (FUN_004e5120(local_40,0,0,6,0), restore_pointer32_checked(local_40, 0x004e5120u));
  RESTORE_SLICE1(local_4,0) = 2;
  FUN_004e5140(uVar1);
  RESTORE_SLICE1(local_4,0) = 1;
  FUN_0051f780();
  uVar1 = (FUN_004e5120(local_40,2,6,1,1), restore_pointer32_checked(local_40, 0x004e5120u));
  RESTORE_SLICE1(local_4,0) = 3;
  FUN_004e5140(uVar1);
  local_4 = CONCAT31(RESTORE_SLICE3(local_4,1),1);
  FUN_0051f780();
  in_ECX[1] = 3;
  ExceptionList = local_c;
  return;
}
