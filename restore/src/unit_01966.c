#include "restore_types.h"
#define LAB_004e50f0 (*(undefined1 *)0x004e50f0u)
#define PTR_LAB_0056f698 (*(unsigned char * *)0x0056f698u)
#define s_suntemple1_005e4578 (*(char (*)[11])0x005e4578u)
#define s_suntemple2_005e456c (*(char (*)[11])0x005e456cu)
#define s_suntemple3_005e4560 (*(char (*)[11])0x005e4560u)
extern void FUN_004e5120();
extern void FUN_004e5140();
extern void FUN_0051f780();
extern void FUN_00561441();

void FUN_004e5210(void)

{
  undefined4 uVar1;
  undefined4 *in_ECX;
  CRect local_34 [52];
  
  FUN_00561441(in_ECX + 3,0x34,1,&LAB_004e50f0,FUN_0051f780);
  *in_ECX = &PTR_LAB_0056f698;
  uVar1 = (FUN_004e5120(local_34,0,0,1,7), restore_pointer32_checked(local_34, 0x004e5120u));
  FUN_004e5140(uVar1);
  FUN_0051f780();
  in_ECX[0x10] = s_suntemple1_005e4578;
  in_ECX[0x11] = s_suntemple2_005e456c;
  in_ECX[0x12] = s_suntemple3_005e4560;
  in_ECX[1] = 1;
  in_ECX[2] = 0;
  return;
}
