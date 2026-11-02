#include "restore_types.h"
#define DAT_00af55f8 (*(undefined4 *)0x00af55f8u)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5600 (*(undefined4 *)0x00af5600u)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d18 (*(undefined4 *)0x00af5d18u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00af5d20 (*(undefined4 *)0x00af5d20u)

int FUN_004775f0(uint param_1,uint param_2,int param_3)

{
  uint mask_55fc = *(uint *)(0x00af55fcu + (unsigned int)param_3 * 4u);
  uint mask_5600 = *(uint *)(0x00af5600u + (unsigned int)param_3 * 4u);
  uint mask_5990 = *(uint *)(0x00af5990u + (unsigned int)param_3 * 4u);
  uint mask_5d20 = *(uint *)(0x00af5d20u + (unsigned int)param_3 * 4u);
  uint mask_5d1c = *(uint *)(0x00af5d1cu + (unsigned int)param_3 * 4u);
  uint mask_5d18 = *(uint *)(0x00af5d18u + (unsigned int)param_3 * 4u);
  uint mask_5988 = *(uint *)(0x00af5988u + (unsigned int)param_3 * 4u);
  uint mask_55f8 = *(uint *)(0x00af55f8u + (unsigned int)param_3 * 4u);
  int iVar1;
  
  iVar1 = 0;
  if (((param_1 & mask_55fc) != 0) && ((param_2 & mask_55fc) == 0)) {
    iVar1 = 1;
  }
  if (((param_1 & mask_5600) != 0) && ((param_2 & mask_5600) == 0)) {
    iVar1 = iVar1 + 1;
  }
  if (((param_1 & mask_5990) != 0) && ((param_2 & mask_5990) == 0)) {
    iVar1 = iVar1 + 1;
  }
  if (((param_1 & mask_5d20) != 0) && ((param_2 & mask_5d20) == 0)) {
    iVar1 = iVar1 + 1;
  }
  if (((param_1 & mask_5d1c) != 0) && ((param_2 & mask_5d1c) == 0)) {
    iVar1 = iVar1 + 1;
  }
  if (((param_1 & mask_5d18) != 0) && ((param_2 & mask_5d18) == 0)) {
    iVar1 = iVar1 + 1;
  }
  if (((param_1 & mask_5988) != 0) && ((param_2 & mask_5988) == 0)) {
    iVar1 = iVar1 + 1;
  }
  if (((param_1 & mask_55f8) != 0) && ((param_2 & mask_55f8) == 0)) {
    iVar1 = iVar1 + 1;
  }
  return iVar1;
}
