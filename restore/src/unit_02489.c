#include "restore_types.h"
#include "sound_initialization.h"
#define DAT_00e38e63 (*(char *)0x00e38e63u)
#define DAT_00e38ec4 (*(int32_t *)0x00e38ec4u)
#define DAT_00e92da8 (*(undefined *)0x00e92da8u)
#define DAT_00f543b8 (*(undefined4 *)0x00f543b8u)
#define DAT_00f55b88 (*(uint32_t *)0x00f55b88u)
#define _DAT_00e92da8 (*(undefined4 *)0x00e92da8u)
#define _DAT_00e92dac (*(undefined4 *)0x00e92dacu)
#define _DAT_00e92db0 (*(undefined4 *)0x00e92db0u)
#define _DAT_00e92db4 (*(undefined4 *)0x00e92db4u)
#define _DAT_00e92db8 (*(undefined4 *)0x00e92db8u)

void FUN_0053b090(int param_1)

{
  if ((((DAT_00f55b88 != 0) && (param_1 < 0x13)) && (DAT_00e38e63 != '\0')) &&
     (-1 < (int)(&DAT_00f543b8)[param_1])) {
    _DAT_00e92db0 = DAT_00e38ec4;
    _DAT_00e92db4 = 0x40;
    _DAT_00e92da8 = 0xffffffff;
    _DAT_00e92db8 = 1;
    _DAT_00e92dac = (&DAT_00f543b8)[param_1];
    FUN_0055f0e8(RESTORE_SOUND,(const int32_t *)0x00e92da8u);
  }
  return;
}
