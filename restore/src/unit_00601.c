#include "restore_types.h"
#define DAT_00799a2e (*(uint16_t *)0x00799a2eu)
#define DAT_00b67d1c (*(uint8_t *)0x00b67d1cu)
#define DAT_00f53dd8 (*(int32_t *)0x00f53dd8u)
#define DAT_00f53ddc (*(undefined4 *)0x00f53ddcu)
#define DAT_00f5648c (*(uint32_t *)0x00f5648cu)
extern int FUN_0053e590();
extern void FUN_005596d0();

undefined4 FUN_00442280(int param_1,int *param_2,int param_3,int param_4)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  int local_6c;
  int local_68 [26];
  
  iVar6 = param_1;
  bVar1 = (&DAT_00b67d1c)[(int)param_2];
  FUN_005596d0();
  iVar2 = -0x2f;
  local_68[10] = 0x6e;
  local_68[0xb] = 0xffffffb7;
  local_68[0xc] = 0x7e;
  local_68[0xd] = 0xffffffc0;
  local_68[0xe] = 0x91;
  local_68[0xf] = 0xffffffc9;
  local_68[0x10] = 0xa1;
  local_68[0x11] = 0xffffffd1;
  local_68[0x12] = 0x5e;
  local_68[0x13] = 0xffffffbe;
  local_68[0x14] = 0x6f;
  local_68[0x15] = 0xffffffc7;
  local_68[0x16] = 0x82;
  local_68[0x17] = 0xffffffd1;
  local_68[0x18] = 0x93;
  local_68[0x19] = 0xffffffda;
  local_68[0] = 0;
  local_68[1] = 0;
  local_68[2] = 0;
  local_68[3] = 0;
  local_68[4] = 0;
  local_68[5] = 0;
  local_68[6] = 0;
  local_68[7] = 0;
  local_68[8] = 0;
  local_68[9] = 0;
  local_6c = 0;
  if ((bVar1 & 0x40) != 0) {
    if (0 < DAT_00f53dd8) {
      param_2 = &DAT_00f53ddc;
      param_1 = DAT_00f53dd8;
      do {
        iVar4 = 0;
        iVar2 = 600;
        do {
          if ((&DAT_00799a2e)[iVar6 * 0x84 + *param_2] != 0) {
            iVar3 = 0xc80;
            if (iVar2 < 0xc81) {
              iVar3 = iVar2;
            }
            if ((short)(&DAT_00799a2e)[iVar6 * 0x84 + *param_2] <= iVar3) {
              if (iVar4 < 8) {
                local_68[*param_2] = iVar4 + 1;
              }
              break;
            }
          }
          iVar2 = iVar2 + 400;
          iVar4 = iVar4 + 1;
        } while (iVar2 < 0xed8);
        param_2 = param_2 + 1;
        param_1 = param_1 + -1;
      } while (param_1 != 0);
      iVar2 = 0;
    }
    param_1 = 0;
    if (0 < DAT_00f53dd8) {
      param_2 = &DAT_00f53ddc;
      do {
        iVar2 = *param_2;
        iVar6 = local_68[iVar2];
        if (0 < iVar6) {
          piVar5 = local_68 + local_6c * 2 + 10;
          local_6c = local_6c + iVar6;
          do {
            FUN_0053e590(iVar2 + -1 + DAT_00f5648c,*piVar5 + param_3,piVar5[1] + param_4);
            piVar5 = piVar5 + 2;
            iVar6 = iVar6 + -1;
          } while (iVar6 != 0);
        }
        iVar2 = param_1 + 1;
        param_2 = param_2 + 1;
        param_1 = iVar2;
      } while (iVar2 < DAT_00f53dd8);
    }
  }
  return CONCAT31((int3)((uint)iVar2 >> 8),1);
}
