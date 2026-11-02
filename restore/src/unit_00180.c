#include "restore_types.h"
#define DAT_00bca2b8 (*(undefined4 *)0x00bca2b8u)
#define DAT_00e36c70 (*(uint32_t *)0x00e36c70u)
#define DAT_00e36c74 (*(uint32_t *)0x00e36c74u)
#define DAT_00e36c80 (*(undefined *)0x00e36c80u)
#define DAT_00e38e58 (*(undefined *)0x00e38e58u)
#define DAT_00e92e1c (*(uint32_t *)0x00e92e1cu)
#define DAT_00f535bc (*(undefined *)0x00f535bcu)
#define s_OK__removed_manmades__0057925c (*(char (*)[22])0x0057925cu)
#define s_Pharaoh_inf_00579274 (*(char (*)[12])0x00579274u)
#define s_Pharaohmap_inf_0057924c (*(char (*)[15])0x0057924cu)
extern void FUN_004ce6e0();
extern undefined4 FUN_004ced90();
extern void FUN_00561a2b();
extern void FUN_00561bdc();

void FUN_00414750(void)

{
  FUN_004ced90(s_Pharaoh_inf_00579274,&DAT_00e38e58,0x234);
  FUN_004ced90(s_Pharaohmap_inf_0057924c,&DAT_00bca2b8,0x54);
  FUN_004ce6e0(s_OK__removed_manmades__0057925c,0,DAT_00f535bc + 1);
  if (DAT_00e92e1c != 0) {
    FUN_00561bdc(DAT_00e92e1c);
  }
  if (DAT_00e36c80 != 0) {
    FUN_00561bdc(DAT_00e36c80);
  }
  if (DAT_00e36c70 != 0) {
    FUN_00561a2b(DAT_00e36c70);
    DAT_00e36c70 = 0;
  }
  if (DAT_00e36c74 != 0) {
    FUN_00561a2b(DAT_00e36c74);
    DAT_00e36c74 = 0;
  }
  return;
}
