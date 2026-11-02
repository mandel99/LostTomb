#include "restore_types.h"
#include "sprite_libraries.h"
#define DAT_00601de4 (*(undefined4 *)0x00601de4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00f56b54 (*(undefined *)0x00f56b54u)
#define DAT_00f56d54 (*(undefined *)0x00f56d54u)
#define DAT_00f56d5c (*(undefined *)0x00f56d5cu)
#define DAT_00f56d64 (*(undefined *)0x00f56d64u)
#define DAT_00f56d74 (*(undefined *)0x00f56d74u)
#define DAT_00f5736c (*(undefined *)0x00f5736cu)
#define PTR_s_suntemple1_00603228 (*(unsigned char * *)0x00603228u)

uint FUN_005414b0(int param_1,undefined4 *param_2)

{
  int iVar1;
  int iVar2;
  
  *param_2 = 0;
  switch((&DAT_00601de4)[(short)(&DAT_007999ea)[param_1 * 0x84] * 0xd]) {
  case 0: ;
    if ((int)(&DAT_00799a30)[param_1 * 0x42] < 9) {
      iVar2 = 0;
    }
    else {
      iVar2 = (0xc < (int)(&DAT_00799a30)[param_1 * 0x42]) + 1;
    }
    iVar1 = *(int *)(&DAT_00799a2c + param_1 * 0x84);
    if (iVar1 != 0) {
      if (iVar1 == 1) {
        FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(&PTR_s_suntemple1_00603228)[iVar2],6);
        return (DAT_00bca2e0 / 2 & 1U) + DAT_00f56d5c;
      }
      if (iVar1 == 2) {
        FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(&PTR_s_suntemple1_00603228)[iVar2],7);
        return (DAT_00bca2e0 / 2 & 1U) + DAT_00f56d64;
      }
    }
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(&PTR_s_suntemple1_00603228)[iVar2],5);
    return (DAT_00bca2e0 / 2 & 1U) + DAT_00f56d54;
  case 1: ;
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return (DAT_00bca2e0 / 2 & 1U) + DAT_00f56d74;
    }
    break;
  case 2: ;
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return ((8 - DAT_00bca2e0) / 2 & 3U) + 2 + DAT_00f56d74;
    }
    break;
  case 3: ;
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return ((8 - DAT_00bca2e0) / 2 + 1U & 3) + 2 + DAT_00f56d74;
    }
    break;
  case 4: ;
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return ((8 - DAT_00bca2e0) / 2 - 2U & 3) + 2 + DAT_00f56d74;
    }
    break;
  case 5: ;
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return ((8 - DAT_00bca2e0) / 2 - 1U & 3) + 2 + DAT_00f56d74;
    }
    break;
  case 6: ;
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return (DAT_00bca2e0 / 2 & 1U) + 6 + DAT_00f56d74;
    }
    goto LAB_00541723;
  case 7: ;
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return (DAT_00bca2e0 / 2 - 1U & 1) + 6 + DAT_00f56d74;
    }
    goto LAB_00541723;
  case 8: ;
    *param_2 = 5;
    iVar2 = (&DAT_00799a30)[param_1 * 0x42];
    if (iVar2 == 2) {
      return DAT_00f56d74 + 8;
    }
    goto LAB_0054172f;
  case 9: ;
    *param_2 = 5;
    iVar2 = (&DAT_00799a30)[param_1 * 0x42];
    if (iVar2 == 2) {
      return DAT_00f56d74 + 9;
    }
    goto LAB_0054172f;
  case 10: ;
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return DAT_00f56d74 + 10;
    }
    goto LAB_00541723;
  case 0xb: ;
    if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
      return DAT_00f56d74 + 0xb;
    }
LAB_00541723: ;
    *param_2 = 5;
    iVar2 = (&DAT_00799a30)[param_1 * 0x42];
LAB_0054172f: ;
    return ~-(uint)(iVar2 != 1) & DAT_00f56b54;
  case 0xc: ;
    if ((&DAT_00799a30)[param_1 * 0x42] == 4) {
      return DAT_00f56d74 + 0xc;
    }
    *param_2 = 5;
    if ((int)(&DAT_00799a30)[param_1 * 0x42] < 1) {
      *param_2 = 7;
      return DAT_00f5736c;
    }
    return DAT_00f56b54;
default: ;
    return 0;
  }
  *param_2 = 5;
  if ((&DAT_00799a30)[param_1 * 0x42] != 1) {
    return DAT_00f5736c;
  }
  return DAT_00f56b54;
}
