#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799adc (*(undefined4 *)0x00799adcu)
#define DAT_00799ade (*(undefined4 *)0x00799adeu)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00bcd787 (*(undefined4 *)0x00bcd787u)

void FUN_004bc7a0(void)

{
  short sVar1;
  short sVar2;
  int iVar3;
  short *psVar4;
  char *pcVar5;
  
  psVar4 = &DAT_00799af0;
  do {
    if ((char)psVar4[-8] == '\x01') {
      sVar1 = *psVar4;
      if ((sVar1 < 0x6e) || (0x72 < sVar1)) {
        switch((int)(sVar1)) {
        case 0x20: ;
        case 0x2f: ;
        case 0x33: ;
        case 0x35: ;
        case 0x4a: ;
        case 0x71: ;
        case 0xb1: ;
        case 0xb2: ;
        case 0xbb: ;
        case 0xbc: ;
        case 0xbd: ;
        case 0xc2: ;
        case 0xcb: ;
        case 0xcc: ;
        case 0xcd: ;
        case 0xe2: ;
        case 0xe7: ;
        case 0xe8: ;
        case 0xe9: ;
          break;
default: ;
          if ((sVar1 != 0x47) && (sVar1 != 0x5f)) goto LAB_004bc7f6;
        }
      }
      psVar4[0x7a] = 0;
      psVar4[0x7b] = 0;
    }
LAB_004bc7f6: ;
    psVar4 = psVar4 + 0x84;
  } while ((int)psVar4 < 0x89b6e8);
  pcVar5 = &DAT_00bcd787;
  do {
    if ((pcVar5[3] != '\0') && (sVar1 = *(short *)(pcVar5 + 0x69), sVar1 != 0)) {
      iVar3 = *(short *)(pcVar5 + 0x53) * 0x108;
      sVar2 = (&DAT_007999e8)[*(short *)(pcVar5 + 0x53) * 0x84];
      if ((sVar2 < 0x6e) || (0x72 < sVar2)) {
        switch((int)(sVar2)) {
        case 0x20: ;
        case 0x2f: ;
        case 0x33: ;
        case 0x35: ;
        case 0x4a: ;
        case 0x71: ;
        case 0xb1: ;
        case 0xb2: ;
        case 0xbb: ;
        case 0xbc: ;
        case 0xbd: ;
        case 0xc2: ;
        case 0xcb: ;
        case 0xcc: ;
        case 0xcd: ;
        case 0xe2: ;
        case 0xe7: ;
        case 0xe8: ;
        case 0xe9: ;
          break;
default: ;
          if (sVar2 == 0x47) {
            *(short *)(&DAT_00799adc + iVar3) = *(short *)(&DAT_00799adc + iVar3) + 1;
            *(short *)(&DAT_00799ade + iVar3) = *(short *)(&DAT_00799ade + iVar3) + sVar1;
          }
          goto LAB_004bc8c5;
        }
      }
      if (sVar2 == 0x5f) {
        if (*pcVar5 == '\x1c') {
          *(short *)(&DAT_00799ade + iVar3) = *(short *)(&DAT_00799ade + iVar3) + 1;
        }
        else if (*pcVar5 == '\n') {
          *(short *)(&DAT_00799adc + iVar3) = *(short *)(&DAT_00799adc + iVar3) + 1;
        }
      }
      else if (sVar2 == 0xcc) {
        if (*pcVar5 == '\t') {
          *(short *)(&DAT_00799ade + iVar3) = *(short *)(&DAT_00799ade + iVar3) + 1;
        }
        else if (*pcVar5 == '\v') {
          *(short *)(&DAT_00799adc + iVar3) = *(short *)(&DAT_00799adc + iVar3) + 1;
        }
      }
      else {
        *(short *)(&DAT_00799adc + iVar3) = *(short *)(&DAT_00799adc + iVar3) + 1;
      }
    }
LAB_004bc8c5: ;
    pcVar5 = pcVar5 + 0x184;
    if (0xc8ad42 < (int)pcVar5) {
      return;
    }
  } while( true );
}
