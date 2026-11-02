#include "restore_types.h"
#include "sprite_libraries.h"
#define DAT_00784cbc (*(byte *)0x00784cbcu)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999ee (*(undefined4 *)0x007999eeu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799a48 (*(undefined4 *)0x00799a48u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00e94e18 (*(undefined *)0x00e94e18u)
#define DAT_00f56b54 (*(undefined *)0x00f56b54u)
#define DAT_00f56d54 (*(undefined *)0x00f56d54u)
#define DAT_00f56d5c (*(undefined *)0x00f56d5cu)
#define DAT_00f56d64 (*(undefined *)0x00f56d64u)
#define DAT_00f56d6c (*(undefined *)0x00f56d6cu)
#define DAT_00f56d74 (*(undefined *)0x00f56d74u)
#define DAT_00f5736c (*(undefined *)0x00f5736cu)
#define DAT_00f57444 (*(undefined *)0x00f57444u)
#define DAT_00f5744c (*(undefined *)0x00f5744cu)
#define DAT_00f57454 (*(undefined *)0x00f57454u)
#define DAT_00f5745c (*(undefined *)0x00f5745cu)
#define DAT_00f57464 (*(undefined *)0x00f57464u)
#define PTR_s_maus0_1a_005e4268 (*(unsigned char * *)0x005e4268u)
#define PTR_s_maus0_1b_005e426c (*(unsigned char * *)0x005e426cu)
#define PTR_s_maus0_1c_005e4270 (*(unsigned char * *)0x005e4270u)
#define PTR_s_maus0_1d_005e4274 (*(unsigned char * *)0x005e4274u)
extern void FUN_004ded50();

int FUN_004df360(int param_1,uint *param_2)

{
  int iVar1;
  int iVar2;
  undefined1 local_74 [12];
  uint local_68;
  int local_40 [16];
  
  local_40[5] = 2;
  local_40[7] = 2;
  local_40[0xd] = 2;
  local_40[0xf] = 2;
  *param_2 = 0;
  iVar2 = param_1 * 0x108;
  local_40[1] = 1;
  local_40[3] = 1;
  iVar1 = *(int *)(&DAT_00799a48 + iVar2);
  local_40[9] = 1;
  local_40[0xb] = 1;
  local_40[0] = 0;
  local_40[2] = 0;
  local_40[8] = 0;
  local_40[10] = 0;
  local_40[4] = 3;
  local_40[6] = 3;
  local_40[0xc] = 3;
  local_40[0xe] = 3;
  FUN_004ded50(local_74,(int)(short)(&DAT_007999ea)[param_1 * 0x84],DAT_00bca2e0,iVar1);
  switch(local_68 & 0xff) {
  case 0: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      if ((int)(&DAT_00799a30)[param_1 * 0x42] < 0xff) {
        iVar2 = 0;
      }
      else if ((*(byte *)(&DAT_00799a3c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84)
               & 0x10) == 0) {
        iVar2 = 0;
      }
      else {
        iVar2 = (0xfffe < (int)(&DAT_00799a30)[param_1 * 0x42]) + 1;
      }
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(&PTR_s_maus0_1a_005e4268)[(iVar2 + DAT_00e94e18 * 3) * 4],5);
      return (DAT_00f56d54 - ((iVar1 + DAT_00bca2e0) / 2 & 1U)) + 1;
    }
    return (DAT_00f57444 - ((DAT_00bca2e0 + iVar1) / 2 & 1U)) + 1;
  case 1: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      if ((int)(&DAT_00799a30)[param_1 * 0x42] < 0xff) {
        iVar2 = 0;
      }
      else if ((*(byte *)(&DAT_00799a3c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84)
               & 0x20) == 0) {
        iVar2 = 0;
      }
      else {
        iVar2 = (0xfffe < (int)(&DAT_00799a30)[param_1 * 0x42]) + 1;
      }
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(&PTR_s_maus0_1b_005e426c)[(iVar2 + DAT_00e94e18 * 3) * 4],6);
      return (DAT_00f56d5c - ((iVar1 + DAT_00bca2e0) / 2 & 1U)) + 1;
    }
    return (DAT_00f5744c - ((DAT_00bca2e0 + iVar1) / 2 & 1U)) + 1;
  case 2: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      if ((int)(&DAT_00799a30)[param_1 * 0x42] < 0xff) {
        iVar2 = 0;
      }
      else if ((*(byte *)(&DAT_00799a3c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84)
               & 0x40) == 0) {
        iVar2 = 0;
      }
      else {
        iVar2 = (0xfffe < (int)(&DAT_00799a30)[param_1 * 0x42]) + 1;
      }
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(&PTR_s_maus0_1c_005e4270)[(iVar2 + DAT_00e94e18 * 3) * 4],7);
      return (DAT_00f56d64 - ((iVar1 + DAT_00bca2e0) / 2 & 1U)) + 1;
    }
    return (DAT_00f57454 - ((DAT_00bca2e0 + iVar1) / 2 & 1U)) + 1;
  case 3: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      if ((int)(&DAT_00799a30)[param_1 * 0x42] < 0xff) {
        iVar2 = 0;
      }
      else if ((*(byte *)(&DAT_00799a3c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84)
               & 0x80) == 0) {
        iVar2 = 0;
      }
      else {
        iVar2 = (0xfffe < (int)(&DAT_00799a30)[param_1 * 0x42]) + 1;
      }
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(&PTR_s_maus0_1d_005e4274)[(iVar2 + DAT_00e94e18 * 3) * 4],8);
      return (DAT_00f56d6c - ((iVar1 + DAT_00bca2e0) / 2 & 1U)) + 1;
    }
    return (DAT_00f5745c - ((DAT_00bca2e0 + iVar1) / 2 & 1U)) + 1;
  case 4: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      *param_2 = 5;
    }
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return DAT_00f56d74;
    }
    if ((&DAT_00799a30)[param_1 * 0x42] == 1) {
      return DAT_00f56b54;
    }
    goto LAB_004df6ec;
  case 5: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      *param_2 = 5;
    }
    iVar1 = (&DAT_00799a30)[param_1 * 0x42];
    if (iVar1 == 2) {
      return DAT_00f56d74 + 1;
    }
    goto LAB_004df974;
  case 6: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      *param_2 = 5;
    }
    iVar1 = (&DAT_00799a30)[param_1 * 0x42];
    if (iVar1 == 2) {
      return DAT_00f56d74 + 2;
    }
    goto LAB_004df974;
  case 7: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      *param_2 = 5;
    }
    iVar1 = (&DAT_00799a30)[param_1 * 0x42];
    if (iVar1 == 2) {
      return DAT_00f56d74 + 3;
    }
    goto LAB_004df974;
  case 8: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      *param_2 = 1;
    }
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return local_40[DAT_00bca2e0 / 2 + (iVar1 / 2) * 4] + 4 + DAT_00f56d74;
    }
    break;
  case 9: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      *param_2 = 1;
    }
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return (local_40[DAT_00bca2e0 / 2 + (iVar1 / 2) * 4] - 2U & 3) + 4 + DAT_00f56d74;
    }
    break;
  case 10: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      *param_2 = 1;
    }
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return DAT_00f56d74 + 9 + (local_40[DAT_00bca2e0 / 2 + (iVar1 / 2) * 4] - 2U & 3) * 2;
    }
    break;
  case 0xb: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      *param_2 = 1;
    }
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return DAT_00f56d74 + 8 + (local_40[DAT_00bca2e0 / 2 + (iVar1 / 2) * 4] - 2U & 3) * 2;
    }
    break;
  case 0xc: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      *param_2 = 1;
    }
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return DAT_00f56d74 + 8 + local_40[DAT_00bca2e0 / 2 + (iVar1 / 2) * 4] * 2;
    }
    break;
  case 0xd: ;
    if (*(short *)(&DAT_007999ee + iVar2) != -1) {
      *param_2 = 1;
    }
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return DAT_00f56d74 + 9 + local_40[DAT_00bca2e0 / 2 + (iVar1 / 2) * 4] * 2;
    }
    break;
default: ;
    goto switchD_004df40b_default;
  }
  *param_2 = *param_2 | 4;
  iVar1 = (&DAT_00799a30)[param_1 * 0x42];
LAB_004df974: ;
  if (iVar1 == 1) {
    return DAT_00f56b54;
  }
LAB_004df6ec: ;
  if ((local_68 & 0x10000) != 0) {
    if ((char)DAT_00784cbc != '\x03') {
      return DAT_00f5736c;
    }
    return DAT_00f57464;
  }
switchD_004df40b_default: ;
  return 0;
}
