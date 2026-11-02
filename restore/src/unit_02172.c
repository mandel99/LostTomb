#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
extern int FUN_004df040();
extern void FUN_004f1310();
extern int FUN_00542ad0();

undefined4 FUN_004f5bd0(int param_1,int param_2)

{
  short sVar1;
  bool bVar2;
  undefined1 local_d8 [32];
  int local_b8;
  int local_b4;
  undefined1 local_68 [28];
  int local_4c;
  undefined1 local_38 [44];
  int local_c;
  
  sVar1 = (&DAT_007999e8)[param_1 * 0x84];
  if (sVar1 != 0xb7) {
    if (sVar1 == 0xd5) {
      if (param_2 != 0x1e) {
        return 0;
      }
      FUN_00542ad0(param_1,local_38);
      if (local_c == 0) {
        return 0;
      }
      return 1;
    }
    if (sVar1 != 0xde) {
      return 0;
    }
    if (param_2 != 0x1e) {
      return 0;
    }
    FUN_004df040(param_1,local_68);
    if (local_4c == 0) {
      return 0;
    }
    return 1;
  }
  FUN_004f1310(param_1,local_d8);
  switch(DAT_005e4ef4 + -1) {
  case 0: ;
  case 3: ;
    if ((param_2 != 0x19) && (local_b4 = local_b8, param_2 != 0x18)) {
      return 0;
    }
    break;
  case 1: ;
    if ((param_2 != 0x19) && (local_b4 = local_b8, param_2 != 0xc)) {
      return 0;
    }
    break;
  case 2: ;
    bVar2 = param_2 == 0x18;
    goto LAB_004f5ccb;
  case 4: ;
    bVar2 = param_2 == 0xc;
LAB_004f5ccb: ;
    if (!bVar2) {
      return 0;
    }
    if (0 < local_b8) {
      return 1;
    }
    break;
default: ;
    goto switchD_004f5c84_default;
  }
  if (0 < local_b4) {
    return 1;
  }
switchD_004f5c84_default: ;
  return 0;
}
