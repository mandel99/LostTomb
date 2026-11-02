#include "restore_types.h"
#define DAT_00e38aec (*(char *)0x00e38aecu)
#define PTR_DAT_00576274 (*(unsigned char * *)0x00576274u)
extern void FUN_004cbea0();
extern void FUN_004ce3a0();
extern void FUN_004ce3c0();
extern bool FUN_004ce3f0();

bool FUN_005409d0(undefined4 param_1)

{
  int iVar1;
  
  FUN_004ce3a0(param_1);
  FUN_004cbea0(&PTR_DAT_00576274,&DAT_00e38aec,3);
  FUN_004ce3c0(param_1);
  iVar1 = FUN_004ce3f0(param_1);
  FUN_004ce3a0(param_1);
  return iVar1 != 0;
}
