#include "restore_types.h"
#define DAT_005e1e28 (*(undefined4 *)0x005e1e28u)
#define DAT_00bca2b8 (*(undefined4 *)0x00bca2b8u)
#define DAT_00e94dd4 (*(undefined4 *)0x00e94dd4u)
#define DAT_00e94dd8 (*(undefined4 *)0x00e94dd8u)
#define s_Pharaohmaps_bmp_005e2ac4 (*(char (*)[16])0x005e2ac4u)
extern void FUN_0046fa40();
extern void FUN_004cbea0();
extern undefined4 FUN_004ceb60();
extern undefined4 FUN_004cede0();
extern undefined4 FUN_004cee00();
extern void FUN_004de200();
extern void FUN_004dea50();
extern void * FUN_00561b68();
extern void FUN_00561bdc();

void FUN_004de160(int param_1)

{
  int iVar1;
  
  FUN_004cbea0(&DAT_005e1e28 + param_1 * 0x54,&DAT_00bca2b8,0x54);
  DAT_00e94dd8 = FUN_00561b68(1500000);
  if (DAT_00e94dd8 != (void *)0x0) {
    iVar1 = FUN_004ceb60(s_Pharaohmaps_bmp_005e2ac4,DAT_00e94dd8,0);
    if (iVar1 == 0) {
      FUN_00561bdc(DAT_00e94dd8);
      return;
    }
    DAT_00e94dd4 = FUN_004cede0(DAT_00e94dd8);
    FUN_004cee00(DAT_00e94dd8);
    FUN_0046fa40();
    FUN_004de200(param_1);
    FUN_004dea50();
    FUN_00561bdc(DAT_00e94dd8);
  }
  return;
}
