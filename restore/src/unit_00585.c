#include "restore_types.h"
#define DAT_005d2934 (*(undefined4 *)0x005d2934u)
#define DAT_005d2944 (*(undefined4 *)0x005d2944u)
#define PTR_DAT_005d2924 (*(unsigned char * *)0x005d2924u)
#define PTR_DAT_005d2928 (*(unsigned char * *)0x005d2928u)

undefined1 FUN_0043f3a0(int param_1,int param_2,undefined4 param_3)

{
  switch((int)(param_3)) {
  case 2: ;
    return *(undefined1 *)((int)&PTR_DAT_005d2924 + param_1 * 2 + param_2);
  case 3: ;
    return *(undefined1 *)((int)&PTR_DAT_005d2928 + param_2 + param_1 * 3);
  case 4: ;
    return (&DAT_005d2934)[param_1 * 4 + param_2];
  case 5: ;
    return (&DAT_005d2944)[param_2 + param_1 * 5];
default: ;
    return 1;
  }
}
