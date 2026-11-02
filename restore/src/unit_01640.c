#include "restore_types.h"
#define DAT_005e0350 (*(undefined4 *)0x005e0350u)
#define DAT_005e0351 (*(undefined4 *)0x005e0351u)
#define DAT_005e0352 (*(undefined4 *)0x005e0352u)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0070cdf4 (*(undefined4 *)0x0070cdf4u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f0 (*(undefined4 *)0x007999f0u)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a17 (*(undefined4 *)0x00799a17u)
#define DAT_00799a1e (*(undefined4 *)0x00799a1eu)
#define DAT_00799a20 (*(undefined4 *)0x00799a20u)
#define DAT_00799a26 (*(undefined4 *)0x00799a26u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a44 (*(undefined4 *)0x00799a44u)
#define DAT_00799a46 (*(undefined4 *)0x00799a46u)
#define DAT_00799a47 (*(undefined4 *)0x00799a47u)
#define DAT_00799a48 (*(undefined4 *)0x00799a48u)
#define DAT_00799a49 (*(undefined4 *)0x00799a49u)
#define DAT_00799a4a (*(undefined4 *)0x00799a4au)
#define DAT_00799a4b (*(undefined4 *)0x00799a4bu)
#define DAT_00799a4d (*(undefined4 *)0x00799a4du)
#define DAT_00799a4e (*(undefined4 *)0x00799a4eu)
#define DAT_00799a4f (*(undefined4 *)0x00799a4fu)
#define DAT_00799a50 (*(undefined4 *)0x00799a50u)
#define DAT_00799a58 (*(undefined4 *)0x00799a58u)
#define DAT_00799a59 (*(undefined4 *)0x00799a59u)
#define DAT_00799a5b (*(undefined4 *)0x00799a5bu)
#define DAT_00799a5e (*(undefined4 *)0x00799a5eu)
#define DAT_00799a5f (*(undefined4 *)0x00799a5fu)
#define DAT_00799abc (*(undefined4 *)0x00799abcu)
#define DAT_00a5d4cc (*(undefined4 *)0x00a5d4ccu)
#define DAT_00ac2d4c (*(undefined4 *)0x00ac2d4cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2c4 (*(undefined *)0x00bca2c4u)
#define DAT_00d3a4c0 (*(undefined4 *)0x00d3a4c0u)
#define DAT_00d3a6b4 (*(undefined *)0x00d3a6b4u)
#define DAT_00d3a6b8 (*(undefined *)0x00d3a6b8u)
#define DAT_00d3a6bc (*(undefined *)0x00d3a6bcu)
#define DAT_00d3a6c0 (*(undefined *)0x00d3a6c0u)
#define DAT_00d3a6c4 (*(undefined *)0x00d3a6c4u)
#define DAT_00d3a6e4 (*(undefined *)0x00d3a6e4u)
#define DAT_00d3b130 (*(undefined *)0x00d3b130u)
#define DAT_00d3b138 (*(undefined *)0x00d3b138u)
#define DAT_00d3b13c (*(undefined *)0x00d3b13cu)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e34238 (*(undefined *)0x00e34238u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38df4 (*(char *)0x00e38df4u)
#define DAT_00e38eac (*(undefined *)0x00e38eacu)
#define DAT_00e94e0c (*(undefined *)0x00e94e0cu)
#define _DAT_00d3a6dc (*(undefined4 *)0x00d3a6dcu)
#define _DAT_00d3a6e0 (*(undefined4 *)0x00d3a6e0u)
extern undefined4 FUN_004439c0();
extern undefined4 FUN_00443b90();
extern int FUN_0046f1c0();
extern int FUN_00475b50();
extern void FUN_004ca3d0();
extern void FUN_004ca600();
extern void FUN_004ca6d0();
extern undefined4 FUN_004ca770();
extern void FUN_004cc110();
extern int FUN_004cc5c0();
extern int FUN_004cde60();

undefined4 FUN_004c9750(void)

{
  byte bVar1;
  short sVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  char *pcVar10;
  char *pcVar11;
  int iVar12;
  undefined4 uVar13;
  int local_4;
  
  iVar5 = DAT_00bca2c4;
  if ((((((DAT_00e38eac == 0) || (DAT_00e38df4 != '\0')) || (DAT_00e38dda != 1)) ||
       ((DAT_00605a3c == 1 || (DAT_00bca2c4 == 0)))) ||
      ((DAT_00e94e0c == 0 || ((DAT_00e94e0c == 2 || (DAT_00e94e0c == 1)))))) ||
     ((DAT_00e94e0c == 3 || (((DAT_00e94e0c == 5 || (DAT_00e94e0c == 6)) || (DAT_00e94e0c == 7))))))
  {
    return 0;
  }
  if (((*(byte *)(&DAT_00af598c + DAT_00bca2c4) & 0x40) == 0) &&
     ((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + DAT_00bca2c4 * 2) * 0x84] != 0xd1)) {
    iVar4 = FUN_00475b50((int)*(short *)(&DAT_00b4e6fc + DAT_00bca2c4 * 2));
  }
  else {
    iVar4 = 0;
  }
  if (((&DAT_005e0351)[DAT_00e94e0c * 3] != '\0') && (iVar4 == 0)) {
    return 0;
  }
  if (((&DAT_005e0350)[DAT_00e94e0c * 3] == '\x01') &&
     (*(char *)((int)&DAT_007999dc + iVar4 * 0x108 + 1) == '\0')) {
    return 0;
  }
  if (((&DAT_005e0350)[DAT_00e94e0c * 3] == '\x02') &&
     (*(char *)((int)&DAT_007999dc + iVar4 * 0x108 + 1) != '\0')) {
    return 0;
  }
  if (((&DAT_005e0352)[DAT_00e94e0c * 3] != '\0') && ((&DAT_00a5d4cc)[iVar5] == '\0')) {
    return 0;
  }
  local_4 = 0;
  FUN_004cc110(0x42,0);
  if (DAT_00e94e0c == 0x2d) {
    if ((iVar4 == 0) || (iVar4 = iVar4 * 0x108, *(char *)((int)&DAT_007999dc + iVar4 + 1) == '\0'))
    {
      uVar7 = (&DAT_00af598c)[iVar5];
      if ((uVar7 & 0x80) == 0) {
        if ((uVar7 & 0x2000) == 0) {
          return 0;
        }
        uVar13 = 3;
      }
      else if ((uVar7 & 0x2000) == 0) {
        uVar13 = 1;
      }
      else {
        uVar13 = 3;
      }
    }
    else {
      bVar1 = (&DAT_00799a4f)[iVar4];
      if (bVar1 < 0x50) {
        if (bVar1 < 0x11) {
          if (bVar1 == 0) {
            if (((&DAT_00af598c)[iVar5] & 0x2000) == 0) {
              if ((&DAT_00799a17)[iVar4] == '\0') {
                uVar13 = 2;
              }
              else {
                uVar13 = 3;
              }
            }
            else {
              uVar13 = 3;
            }
          }
          else {
            uVar13 = 0xb;
          }
        }
        else {
          uVar13 = 10;
        }
      }
      else {
        uVar13 = 9;
      }
    }
LAB_004ca2d9: ;
    FUN_004cc110(0x42,uVar13);
  }
  else {
    if (DAT_00e94e0c == 4) {
      cVar3 = (&DAT_00799a5b)[iVar4 * 0x108];
      if (cVar3 < '\x01') {
        uVar13 = 0xc;
      }
      else if (cVar3 < '\x02') {
        uVar13 = 0xd;
      }
      else if (cVar3 < '\x03') {
        uVar13 = 0xe;
      }
      else if (cVar3 < '\x04') {
        uVar13 = 0xf;
      }
      else if (cVar3 < '\x05') {
        uVar13 = 0x10;
      }
      else if (cVar3 < '\x06') {
        uVar13 = 0x11;
      }
      else {
        uVar13 = 0x12;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 8) {
      sVar2 = *(short *)(&DAT_00799a20 + iVar4 * 0x108);
      if (sVar2 < 1) {
        uVar13 = 0x2e;
      }
      else if (sVar2 < 0xc9) {
        uVar13 = 0x2f;
      }
      else if (sVar2 < 0x191) {
        uVar13 = 0x30;
      }
      else if (sVar2 < 0x259) {
        uVar13 = 0x31;
      }
      else if (sVar2 < 0x321) {
        uVar13 = 0x32;
      }
      else {
        uVar13 = 0x33;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 9) {
      sVar2 = (&DAT_00799a1e)[iVar4 * 0x84];
      if (sVar2 < 1) {
        uVar13 = 0x34;
      }
      else if (sVar2 < 0xc9) {
        uVar13 = 0x35;
      }
      else if (sVar2 < 0x191) {
        uVar13 = 0x36;
      }
      else if (sVar2 < 0x259) {
        uVar13 = 0x37;
      }
      else if (sVar2 < 0x321) {
        uVar13 = 0x38;
      }
      else {
        uVar13 = 0x39;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 10) {
      iVar5 = (&DAT_00799abc)[iVar4 * 0x42];
      if (iVar5 < 1) {
        uVar13 = 0x3a;
      }
      else if (iVar5 < 0x65) {
        uVar13 = 0x3b;
      }
      else if (iVar5 < 0xc9) {
        uVar13 = 0x3c;
      }
      else if (iVar5 < 0x12d) {
        uVar13 = 0x3d;
      }
      else if (iVar5 < 500) {
        uVar13 = 0x3e;
      }
      else {
        uVar13 = 0x3f;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0xb) {
      cVar3 = (&DAT_00799a58)[iVar4 * 0x108];
      if (cVar3 < '\x01') {
        uVar13 = 0x40;
      }
      else if (cVar3 < '\n') {
        uVar13 = 0x41;
      }
      else if (cVar3 < '\x14') {
        uVar13 = 0x42;
      }
      else if (cVar3 < '\x1e') {
        uVar13 = 0x43;
      }
      else if (cVar3 < '(') {
        uVar13 = 0x44;
      }
      else if (cVar3 < '2') {
        uVar13 = 0x45;
      }
      else if (cVar3 < '<') {
        uVar13 = 0x46;
      }
      else if (cVar3 < 'F') {
        uVar13 = 0x47;
      }
      else if (cVar3 < 'P') {
        uVar13 = 0x48;
      }
      else if (cVar3 < 'Z') {
        uVar13 = 0x49;
      }
      else {
        uVar13 = 0x4a;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0xc) {
      bVar1 = *(byte *)(&DAT_00799a44 + iVar4 * 0x84);
      if (bVar1 == 0) {
        uVar13 = 0x4b;
      }
      else if (bVar1 < 0x50) {
        if (bVar1 < 0x14) {
          uVar13 = 0x4e;
        }
        else {
          uVar13 = 0x4d;
        }
      }
      else {
        uVar13 = 0x4c;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0xd) {
      bVar1 = *(byte *)((int)&DAT_00799a44 + iVar4 * 0x108 + 1);
      if (bVar1 == 0) {
        uVar13 = 0x4f;
      }
      else if (bVar1 < 0x50) {
        if (bVar1 < 0x14) {
          uVar13 = 0x52;
        }
        else {
          uVar13 = 0x51;
        }
      }
      else {
        uVar13 = 0x50;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0xe) {
      bVar1 = (&DAT_00799a47)[iVar4 * 0x108];
      if (bVar1 == 0) {
        uVar13 = 0x53;
      }
      else if (bVar1 < 0x50) {
        if (bVar1 < 0x14) {
          uVar13 = 0x56;
        }
        else {
          uVar13 = 0x55;
        }
      }
      else {
        uVar13 = 0x54;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0xf) {
      bVar1 = (&DAT_00799a49)[iVar4 * 0x108];
      if (bVar1 == 0) {
        uVar13 = 0x57;
      }
      else if (bVar1 < 0x50) {
        if (bVar1 < 0x14) {
          uVar13 = 0x5a;
        }
        else {
          uVar13 = 0x59;
        }
      }
      else {
        uVar13 = 0x58;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0x10) {
      bVar1 = (&DAT_00799a5e)[iVar4 * 0x108];
      if (bVar1 == 0) {
        uVar13 = 0xa8;
      }
      else if (bVar1 < 0x50) {
        if (bVar1 < 0x14) {
          uVar13 = 0xab;
        }
        else {
          uVar13 = 0xaa;
        }
      }
      else {
        uVar13 = 0xa9;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0x11) {
      cVar3 = (&DAT_00799a59)[iVar4 * 0x108];
      if (cVar3 == '\0') {
        uVar13 = 100;
      }
      else if (cVar3 == '\x01') {
        uVar13 = 0x65;
      }
      else {
        if (cVar3 != '\x02') goto LAB_004ca2e3;
        uVar13 = 0x66;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0x12) {
      bVar1 = (&DAT_00799a4a)[iVar4 * 0x108];
      if (bVar1 == 0) {
        uVar13 = 0x13;
      }
      else if (bVar1 < 0x50) {
        if (bVar1 < 0x14) {
          uVar13 = 0x16;
        }
        else {
          uVar13 = 0x15;
        }
      }
      else {
        uVar13 = 0x14;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0x13) {
      bVar1 = (&DAT_00799a4b)[iVar4 * 0x108];
      if (bVar1 == 0) {
        uVar13 = 0x17;
      }
      else if (bVar1 < 0x50) {
        if (bVar1 < 0x14) {
          uVar13 = 0x1a;
        }
        else {
          uVar13 = 0x19;
        }
      }
      else {
        uVar13 = 0x18;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0x15) {
      bVar1 = (&DAT_00799a4d)[iVar4 * 0x108];
      if (bVar1 == 0) {
        uVar13 = 0x1f;
      }
      else if (bVar1 < 0x50) {
        if (bVar1 < 0x14) {
          uVar13 = 0x22;
        }
        else {
          uVar13 = 0x21;
        }
      }
      else {
        uVar13 = 0x20;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0x17) {
      bVar1 = (&DAT_00799a4e)[iVar4 * 0x108];
      if (bVar1 == 0) {
        uVar13 = 0x23;
      }
      else if (bVar1 < 0x50) {
        if (bVar1 < 0x14) {
          uVar13 = 0x26;
        }
        else {
          uVar13 = 0x25;
        }
      }
      else {
        uVar13 = 0x24;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0x16) {
      bVar1 = (&DAT_00799a46)[iVar4 * 0x108];
      if (bVar1 == 0) {
        uVar13 = 0x84;
      }
      else if (bVar1 < 0x50) {
        if (bVar1 < 0x14) {
          uVar13 = 0x85;
        }
        else {
          uVar13 = 0x86;
        }
      }
      else {
        uVar13 = 0x87;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0x18) {
      bVar1 = (&DAT_00799a50)[iVar4 * 0x108];
      if (bVar1 == 0) {
        uVar13 = 0x27;
      }
      else if (bVar1 < 0x50) {
        if (bVar1 < 0x14) {
          uVar13 = 0x2a;
        }
        else {
          uVar13 = 0x29;
        }
      }
      else {
        uVar13 = 0x28;
      }
      goto LAB_004ca2d9;
    }
    if (DAT_00e94e0c == 0x19) {
      iVar5 = FUN_0046f1c0(iVar4);
      if (iVar5 < 1) {
        if ((&DAT_00799a26)[iVar4 * 0x108] == '\0') {
          uVar13 = 0x2b;
        }
        else {
          uVar13 = 0x2c;
        }
        goto LAB_004ca2d9;
      }
      iVar5 = FUN_004cc5c0(iVar5,0,0);
      local_4 = local_4 + iVar5;
      uVar7 = 0xffffffff;
      pcVar10 = &DAT_00e34238;
      do {
        pcVar11 = pcVar10;
        if (uVar7 == 0) break;
        uVar7 = uVar7 - 1;
        pcVar11 = pcVar10 + 1;
        cVar3 = *pcVar10;
        pcVar10 = pcVar11;
      } while (cVar3 != '\0');
      uVar7 = ~uVar7;
      pcVar10 = pcVar11 + -uVar7;
      pcVar11 = (char *)&DAT_00d3a4c0;
      for (uVar8 = uVar7 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
        *(undefined4 *)pcVar11 = *(undefined4 *)pcVar10;
        pcVar10 = pcVar10 + 4;
        pcVar11 = pcVar11 + 4;
      }
      for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
        *pcVar11 = *pcVar10;
        pcVar10 = pcVar10 + 1;
        pcVar11 = pcVar11 + 1;
      }
      FUN_004cc110(0x42,0x2d);
    }
    else if (DAT_00e94e0c == 0x2b) {
      FUN_004ca6d0(iVar4);
    }
    else if (DAT_00e94e0c == 0x30) {
      FUN_004ca600(iVar4);
    }
    else if (DAT_00e94e0c == 0x21) {
      FUN_004ca3d0(iVar4,1,0,&local_4);
    }
    else if (DAT_00e94e0c == 0x22) {
      FUN_004ca3d0(iVar4,3,2,&local_4);
    }
    else if (DAT_00e94e0c == 0x23) {
      FUN_004ca3d0(iVar4,6,5,&local_4);
    }
    else if (DAT_00e94e0c == 0x24) {
      FUN_004ca3d0(iVar4,2,1,&local_4);
    }
    else if (DAT_00e94e0c == 0x25) {
      FUN_004ca3d0(iVar4,4,3,&local_4);
    }
    else if (DAT_00e94e0c == 0x27) {
      FUN_004ca3d0(iVar4,0xd,9,&local_4);
    }
    else if (DAT_00e94e0c == 0x28) {
      FUN_004ca3d0(iVar4,0x13,0xb,&local_4);
    }
    else if (DAT_00e94e0c == 0x29) {
      FUN_004ca3d0(iVar4,0x11,10,&local_4);
    }
    else {
      if (DAT_00e94e0c != 0x2a) {
        if (DAT_00e94e0c == 0x1a) {
          if ((short)(&DAT_007999f4)[iVar4 * 0x84] < 1) {
            return 0;
          }
          iVar5 = 0;
          if (*(int *)(&DAT_0070cdf4 + (short)(&DAT_007999ea)[iVar4 * 0x84] * 0x58) != 0) {
            iVar5 = (int)(short)(&DAT_007999f4)[iVar4 * 0x84];
          }
          iVar9 = 0;
          iVar12 = 1;
          do {
            iVar6 = FUN_00443b90(iVar12);
            if ((iVar6 != 0) || (iVar6 = FUN_004439c0(iVar12), iVar6 != 0)) {
              switch((int)(iVar12)) {
              case 1: ;
                iVar6 = 0;
                break;
              case 2: ;
                iVar6 = 1;
                break;
              case 3: ;
                iVar6 = 2;
                break;
              case 4: ;
                iVar6 = 3;
                break;
              case 5: ;
                iVar6 = 4;
                break;
              case 6: ;
                iVar6 = 5;
                break;
              case 7: ;
                iVar6 = 6;
                break;
              case 8: ;
                iVar6 = 7;
                break;
default: ;
                iVar6 = -1;
                break;
              case 0xd: ;
                iVar6 = 9;
                break;
              case 0xf: ;
                iVar6 = 8;
                break;
              case 0x11: ;
                iVar6 = 10;
                break;
              case 0x13: ;
                iVar6 = 0xb;
              }
              iVar9 = iVar9 + (short)(&DAT_00799a2c)[iVar6 + iVar4 * 0x84];
            }
            iVar12 = iVar12 + 1;
          } while (iVar12 < 9);
          if ((iVar5 == 0) || (iVar4 = FUN_004cde60(iVar9,iVar5), iVar5 < 1)) {
            uVar13 = 0x68;
          }
          else if (iVar4 < 0x3e9) {
            if (iVar4 < 500) {
              if (iVar4 < 1) {
                uVar13 = 4;
              }
              else {
                uVar13 = 5;
              }
            }
            else {
              uVar13 = 6;
            }
          }
          else {
            uVar13 = 7;
          }
        }
        else if (DAT_00e94e0c == 0x1b) {
          cVar3 = (&DAT_00ac2d4c)[iVar5];
          if (cVar3 < -10) {
            uVar13 = 0xa2;
          }
          else if (cVar3 < '\0') {
            uVar13 = 0xa3;
          }
          else if (cVar3 < '\x10') {
            uVar13 = 0xa4;
          }
          else if (cVar3 < ')') {
            uVar13 = 0xa5;
          }
          else if (cVar3 < 'G') {
            uVar13 = 0xa6;
          }
          else {
            uVar13 = 0xa7;
          }
        }
        else if (DAT_00e94e0c == 0x1c) {
          bVar1 = (&DAT_00a5d4cc)[iVar5];
          if (bVar1 == 0) {
            uVar13 = 0x6d;
          }
          else if (bVar1 < 0x19) {
            uVar13 = 0x6c;
          }
          else if (bVar1 < 0x32) {
            uVar13 = 0x6b;
          }
          else if (bVar1 < 0x4b) {
            uVar13 = 0x6a;
          }
          else {
            uVar13 = 0x69;
          }
        }
        else if (DAT_00e94e0c == 0x1d) {
          iVar5 = FUN_004cde60((int)(short)(&DAT_007999f0)[iVar4 * 0x84],300);
          if ((short)(&DAT_007999f0)[iVar4 * 0x84] < 1) {
            uVar13 = 0x5e;
          }
          else if (iVar5 < 0x15) {
            uVar13 = 0x5f;
          }
          else if (iVar5 < 0x29) {
            uVar13 = 0x60;
          }
          else if (iVar5 < 0x3d) {
            uVar13 = 0x61;
          }
          else if (iVar5 < 0x51) {
            uVar13 = 0x62;
          }
          else {
            uVar13 = 99;
          }
        }
        else if (DAT_00e94e0c == 0x2c) {
          iVar4 = iVar4 * 0x108;
          cVar3 = *(char *)((int)&DAT_007999dc + iVar4 + 1);
          if (cVar3 == '\0') {
            return 0;
          }
          if ((&DAT_00799a5f)[iVar4] == '\x01') {
            uVar13 = 0x83;
          }
          else {
            if (cVar3 == '\0') {
              return 0;
            }
            if ((&DAT_00799a5f)[iVar4] != '\x02') {
              return 0;
            }
            uVar13 = 0xac;
          }
        }
        else if (DAT_00e94e0c == 0x2e) {
          if (*(char *)((int)&DAT_007999dc + iVar4 * 0x108 + 1) == '\0') {
            return 0;
          }
          if ((&DAT_007999f4)[iVar4 * 0x84] != 0) {
            return 0;
          }
          uVar13 = 0x9c;
        }
        else {
          if (DAT_00e94e0c == 0x1f) {
            cVar3 = FUN_004ca770(iVar4);
            if (cVar3 == '\0') {
              return 0;
            }
            DAT_00e20768 = (char *)0x0;
            goto LAB_004ca316;
          }
          if (DAT_00e94e0c == 0x1e) {
            return 0;
          }
          if (DAT_00e94e0c == 0x31) {
            return 0;
          }
          if (DAT_00e94e0c == 0x33) {
            return 0;
          }
          if (DAT_00e94e0c != 0x32) goto LAB_004ca2e3;
          bVar1 = (&DAT_00799a48)[iVar4 * 0x108];
          if (bVar1 == 0) {
            uVar13 = 0x9e;
          }
          else if (bVar1 < 0x50) {
            if (bVar1 < 0x14) {
              uVar13 = 0xa1;
            }
            else {
              uVar13 = 0xa0;
            }
          }
          else {
            uVar13 = 0x9f;
          }
        }
        goto LAB_004ca2d9;
      }
      FUN_004ca3d0(iVar4,0xf,8,&local_4);
    }
  }
LAB_004ca2e3: ;
  if (DAT_00e20768 != (char *)0x0) {
    uVar7 = 0xffffffff;
    pcVar10 = DAT_00e20768;
    do {
      pcVar11 = pcVar10;
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      pcVar11 = pcVar10 + 1;
      cVar3 = *pcVar10;
      pcVar10 = pcVar11;
    } while (cVar3 != '\0');
    uVar7 = ~uVar7;
    pcVar10 = pcVar11 + -uVar7;
    pcVar11 = (char *)((int)&DAT_00d3a4c0 + local_4);
    for (uVar8 = uVar7 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
      *(undefined4 *)pcVar11 = *(undefined4 *)pcVar10;
      pcVar10 = pcVar10 + 4;
      pcVar11 = pcVar11 + 4;
    }
    for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
      *pcVar11 = *pcVar10;
      pcVar10 = pcVar10 + 1;
      pcVar11 = pcVar11 + 1;
    }
  }
LAB_004ca316: ;
  DAT_00d3a6b8 = DAT_00d3b13c;
  DAT_00d3a6bc = DAT_00d3b130;
  DAT_00d3a6b4 = DAT_00d3b138;
  _DAT_00d3a6e0 = 0;
  _DAT_00d3a6dc = DAT_00bca2c4;
  DAT_00d3a6c4 = (uint)DAT_00e38dda;
  DAT_00d3a6e4 = 1;
  DAT_00d3a6c0 = 1;
  DAT_00e38dca = 1;
  return 1;
}
