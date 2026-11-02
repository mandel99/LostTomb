#include "restore_types.h"
#define DAT_01266408 (*(undefined *)0x01266408u)
#define PTR___exit_00604968 (*(unsigned char * *)0x00604968u)
extern void FUN_00567dcc();
extern void FUN_00567e05();

void __cdecl FUN_005630e0(int param_1)

{
  if (DAT_01266408 == 1) {
    FUN_00567dcc();
  }
  FUN_00567e05(param_1);
  (*(code *)PTR___exit_00604968)(0xff);
  return;
}
