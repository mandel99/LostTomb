#include "restore_types.h"
#include "sprite_render.h"
#include <stdio.h>
#define DAT_005e06e0 (*(undefined4 *)0x005e06e0u)
#define DAT_006018f8 (*(int32_t *)0x006018f8u)
#define DAT_006018fc (*(int32_t *)0x006018fcu)
#define DAT_0070e120 (*(uint16_t *)0x0070e120u)
#define DAT_00d3b530 (*(undefined1 *)0x00d3b530u)
#define DAT_00d3b531 (*(undefined1 *)0x00d3b531u)
#define DAT_00d3b538 (*(int32_t *)0x00d3b538u)
#define DAT_00d3b53c (*(int32_t *)0x00d3b53cu)
#define DAT_00d3b540 (*(int32_t *)0x00d3b540u)
#define DAT_00d3b544 (*(int32_t *)0x00d3b544u)
#define DAT_00d3b54c (*(int32_t *)0x00d3b54cu)
#define DAT_00d3b568 (*(int32_t *)0x00d3b568u)
#define DAT_00d3b56c (*(int32_t *)0x00d3b56cu)
#define DAT_00d3b570 (*(int32_t *)0x00d3b570u)
#define DAT_00d3b574 (*(int32_t *)0x00d3b574u)
#define DAT_00d3b578 (*(int32_t *)0x00d3b578u)
#define DAT_00d3b57c (*(int32_t *)0x00d3b57cu)
#define DAT_00d3b580 (*(int32_t *)0x00d3b580u)
#define DAT_00d3b5b4 (*(undefined1 *)0x00d3b5b4u)
#define DAT_00d3b5b8 (*(int32_t *)0x00d3b5b8u)
#define DAT_00d3b5bc (*(int16_t *)0x00d3b5bcu)
#define DAT_00d3b5be (*(int16_t *)0x00d3b5beu)
#define DAT_00d3b5d4 (*(int32_t *)0x00d3b5d4u)
#define DAT_00d3b5dc (*(int32_t *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(int32_t *)0x00d3b5e0u)
#define DAT_00e38aa8 (*(undefined4 *)0x00e38aa8u)
#define DAT_00e38d9c (*(int32_t *)0x00e38d9cu)
#define DAT_00e38dac (*(int32_t *)0x00e38dacu)
#define DAT_00e38db0 (*(int32_t *)0x00e38db0u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e92e1c (*(uint32_t *)0x00e92e1cu)
#define DAT_00f56db4 (*(int32_t *)0x00f56db4u)
extern void FUN_0040fc40();
extern void FUN_0040fcf0();
extern void FUN_0040fdd0();
extern void FUN_0040fed0();
extern void FUN_0040ff50();
extern void FUN_004100f0();
extern void FUN_004101c0();
extern undefined4 FUN_004105c0();
extern undefined4 FUN_00410660();
extern void FUN_00410700();
extern void FUN_004109d0();
extern void FUN_00410a70();
extern undefined4 FUN_00410de0();
extern undefined4 FUN_00410e80();
extern void FUN_00410f70();
extern void FUN_00410fb0();
extern void FUN_004114a0();
extern void FUN_00411fd0();
extern undefined4 FUN_00427a20();
extern undefined4 FUN_0042a160();
extern undefined4 FUN_0042b440();
extern undefined4 FUN_0042c740();
extern undefined4 FUN_0042c9a0();
extern undefined4 FUN_0042cee0();
extern undefined4 FUN_0042d810();
extern undefined4 FUN_0042e680();
extern void FUN_004cdeb0();
extern void FUN_004cdf20();
extern void FUN_004ce310();
extern int FUN_004ce480();
extern void FUN_0051f780();
extern void FUN_0053f790();
extern int FUN_0053f7d0();

int restore_sprite_draw(RestoreSpriteLibraries *self,int param_1,int param_2,int param_3)

{
  int *piVar1;
  char cVar2;
  bool bVar3;
  int iVar4;
  uintptr_t in_ECX=(uintptr_t)self;
  uint uVar5;
  uint uVar6;
  undefined2 uVar7;
  int iVar8;
  int *piVar9;
  char *pcVar10;
  undefined4 *puVar11;
  char *pcVar12;
  char *pcVar13;
  char local_104[260];
  
  DAT_00d3b57c = param_2;
  DAT_00d3b580 = param_1;
  DAT_00d3b578 = param_3;
  bVar3 = false;
  iVar8 = param_1;
  if (*(int *)(in_ECX + 0x30f5cc) <= param_1) {
    iVar8 = 0;
  }
  piVar9 = (int *)(iVar8 * 0x40 + 0x180e48 + in_ECX);
  if (param_1 < 1) {
    return 0;
  }
  if (0x639b < param_1) {
    return 0;
  }
  if (piVar9 == (int *)0x0) {
    return 0;
  }
  if (*(char *)((int)piVar9 + 0x3e) != '\0') {
    iVar8 = piVar9[4] + param_1;
    if (*(int *)(in_ECX + 0x30f5cc) <= iVar8) {
      iVar8 = 0;
    }
    bVar3 = true;
    piVar9 = (int *)(iVar8 * 0x40 + 0x180e48 + in_ECX);
    if (piVar9 == (int *)0x0) {
      return 0;
    }
  }
  DAT_00d3b5b8 = *piVar9;
  if (DAT_00d3b5b8 < 1) {
    return 0;
  }
  DAT_00d3b5be = (short)piVar9[5];
  DAT_00d3b5bc = *(undefined2 *)((int)piVar9 + 0x16);
  DAT_00d3b5b4 = *(char *)((int)piVar9 + 0x32);
  DAT_00d3b5e0 = param_2;
  iVar8 = (int)DAT_00d3b5be;
  DAT_00d3b5dc = param_3;
  DAT_00d3b544 = iVar8;
  if (((char)piVar9[0xd] != '\0') && (iVar8 <= DAT_00e38d9c)) {
    iVar4=param_1<(int32_t)self->next_image?param_1:0;
    if(iVar4!=param_1)param_1=0;
    {
      RestoreSpriteImage *source=&self->images[iVar4];
      uint32_t library;
      memcpy(&library,source->bytes+12,4);
      if(library>=30||source->bytes[0x38]>=200)return 0;
      if(snprintf(local_104,sizeof(local_104),"data\\%s",self->libraries[library].files[source->bytes[0x38]])>=sizeof(local_104))return 0;
    }
    FUN_004ce310(local_104,&DAT_005e06e0);
    piVar1 = (int *)(in_ECX + 0x30f5c0);
    FUN_0053f790(piVar1,0);
    iVar8 = DAT_00e92e1c;
    *piVar1 = DAT_00e92e1c;
    iVar8 = FUN_004ce480(local_104,iVar8,piVar9[1],*piVar9 + -1);
    if (iVar8 == 0) {
      return 0;
    }
    if (*(char *)((int)piVar9 + 0x33) == '\0') {
      FUN_004cdf20(*piVar1,piVar9[1]);
    }
    else {
      FUN_004cdeb0(*piVar1,piVar9[1]);
    }
    FUN_004cdad0();
    FUN_004cdb80();
    iVar8 = (int)DAT_00d3b5be;
    if (DAT_00d3b530 == '\x05') {
      return 0;
    }
    if (DAT_00d3b5b4 == '\0') {
      if (*(char *)((int)piVar9 + 0x33) != '\0') {
        if (!bVar3) {
          if (DAT_00d3b531 == '\x01') {
            FUN_004105c0(*piVar1);
            return iVar8;
          }
          if (DAT_00d3b531 == '\x02') {
            FUN_00410660(*piVar1);
            return iVar8;
          }
          FUN_00410230((const void *)(uintptr_t)(uint32_t)*piVar1);
          return iVar8;
        }
        if (DAT_00d3b531 == '\x01') {
          FUN_0040fdd0(*piVar1);
          return iVar8;
        }
        if (DAT_00d3b531 == '\x02') {
          FUN_0040fcf0(*piVar1);
          return iVar8;
        }
        FUN_0040fc40(*piVar1);
        return iVar8;
      }
      DAT_00d3b5b8 = (iVar8 * DAT_00d3b538 + DAT_00d3b54c) * 2;
      if (!bVar3) {
        if (DAT_00d3b531 == '\x01') {
          FUN_00410090((const void *)(uintptr_t)(uint32_t)(*piVar1 + DAT_00d3b5b8));
          return iVar8;
        }
        if (DAT_00d3b531 == '\x02') {
          FUN_00410090((const void *)(uintptr_t)(uint32_t)(*piVar1 + DAT_00d3b5b8));
          return iVar8;
        }
        FUN_0040ffd0((const void *)(uintptr_t)(uint32_t)(*piVar1 + DAT_00d3b5b8));
        return iVar8;
      }
      if (DAT_00d3b531 == '\x01') {
        FUN_0040ff50(*piVar1 + DAT_00d3b5b8);
        return iVar8;
      }
      if (DAT_00d3b531 == '\x02') {
        FUN_0040ff50(*piVar1 + DAT_00d3b5b8);
        return iVar8;
      }
    }
    else {
      DAT_00d3b5b8 = (iVar8 * DAT_00d3b538 + DAT_00d3b54c) * 2;
      if (!bVar3) {
        if (DAT_00d3b531 == '\x01') {
          FUN_00410090((const void *)(uintptr_t)(uint32_t)(*piVar1 + DAT_00d3b5b8));
          return iVar8;
        }
        if (DAT_00d3b531 == '\x02') {
          FUN_00410090((const void *)(uintptr_t)(uint32_t)(*piVar1 + DAT_00d3b5b8));
          return iVar8;
        }
        FUN_0040ffd0((const void *)(uintptr_t)(uint32_t)(*piVar1 + DAT_00d3b5b8));
        return iVar8;
      }
      if (DAT_00d3b531 == '\x01') {
        FUN_0040ff50(*piVar1 + DAT_00d3b5b8);
        return iVar8;
      }
      if (DAT_00d3b531 == '\x02') {
        FUN_0040ff50(*piVar1 + DAT_00d3b5b8);
        return iVar8;
      }
    }
    FUN_0040fed0(*piVar1 + DAT_00d3b5b8);
    return iVar8;
  }
  if ((char)piVar9[0xd] != '\0') {
    DAT_00d3b574 = DAT_00e38db0;
    DAT_00d3b544 = DAT_00e38db0;
    DAT_00d3b568 = (0x300 - DAT_00e38dac) / 2;
    DAT_00d3b56c = (0x400 - DAT_00e38db0) / 2;
    iVar4 = iVar8 * DAT_00d3b568 + DAT_00d3b56c;
    DAT_00d3b570 = DAT_00e38dac;
    DAT_00d3b5d4 = DAT_00e38dac;
    iVar8 = param_1;
    if ((*(int *)(in_ECX + 0x30f5cc) <= param_1) &&
       (iVar8 = 0, *(int *)(in_ECX + 0x30f5cc) <= param_1)) {
      param_1 = 0;
    }
    {
      RestoreSpriteImage *source=&self->images[iVar8];
      uint32_t library;
      memcpy(&library,source->bytes+12,4);
      if(library>=30||source->bytes[0x38]>=200)return 0;
      strcpy((char *)&DAT_00e38aa8,self->libraries[library].files[source->bytes[0x38]]);
    }
    piVar9 = (int *)(in_ECX + 0x30f5c0);
    FUN_0053f790(piVar9,0);
    *piVar9 = DAT_00e92e1c;
    DAT_00d3b5dc = 0;
    DAT_00d3b5e0 = 0;
    FUN_0053f7d0(&DAT_00e38aa8,*piVar9,DAT_00d3b544 * 2,DAT_00d3b5d4,iVar4 * 2);
    DAT_00d3b540 = 0;
    DAT_00d3b53c = 0;
    FUN_0040fb60((const void *)(uintptr_t)(uint32_t)(*piVar9));
    return DAT_00d3b544;
  }
  if (((DAT_00e38dda == '\x15') || (DAT_00e38dda == '\x14')) && (param_1 == DAT_00f56db4)) {
    DAT_00d3b5d4 = DAT_00d3b570;
    DAT_00d3b53c = (DAT_00e38d9c - DAT_00d3b574) * 2;
    DAT_00d3b540 = (iVar8 - DAT_00d3b574) * 2;
    DAT_00d3b544 = DAT_00d3b574;
    iVar8 = DAT_00d3b5b8 + (iVar8 * DAT_00d3b568 + DAT_00d3b56c) * 2;
LAB_0053ec1d: ;
    FUN_0040fb60((void *)(uintptr_t)(uint32_t)iVar8);
  }
  else {
    if (DAT_00d3b5b4 == '\x1e') {
      if (DAT_00d3b5be == 0x3a) {
        DAT_00d3b5bc = 0x1e;
      }
      else if (DAT_00d3b5be == 0x1a) {
        DAT_00d3b5bc = 0xe;
      }
      else if (DAT_00d3b5be == 10) {
        DAT_00d3b5bc = 6;
      }
      else {
        if (DAT_00d3b5be == 0x76) {
          iVar8 = FUN_0042c740();
          return iVar8;
        }
        if (DAT_00d3b5be == 0xb2) {
          iVar8 = FUN_0042c9a0();
          return iVar8;
        }
        if (DAT_00d3b5be == 0xee) {
          iVar8 = FUN_0042cee0();
          return iVar8;
        }
        if (DAT_00d3b5be == 0x12a) {
          iVar8 = FUN_0042d810();
          return iVar8;
        }
        if (DAT_00d3b5be == 0x166) {
          iVar8 = FUN_0042e680();
          return iVar8;
        }
      }
    }
    FUN_004cdad0();
    FUN_004cdb80();
    if (DAT_00d3b530 == '\x05') {
      return 0;
    }
    if (DAT_00d3b5b4 == '\0') {
      if (*(char *)((int)piVar9 + 0x33) == '\0') {
        DAT_00d3b5b8 = DAT_00d3b5b8 + (DAT_00d3b5be * DAT_00d3b538 + DAT_00d3b54c) * 2;
        if (bVar3) {
          if (DAT_00d3b531 == '\x01') {
            FUN_0040ff50(DAT_00d3b5b8);
          }
          else if (DAT_00d3b531 == '\x02') {
            FUN_0040ff50(DAT_00d3b5b8);
          }
          else {
            FUN_0040fed0(DAT_00d3b5b8);
          }
        }
        else if (DAT_00d3b531 == '\x01') {
          FUN_00410090((const void *)(uintptr_t)(uint32_t)(DAT_00d3b5b8));
        }
        else if (DAT_00d3b531 == '\x02') {
          FUN_00410090((const void *)(uintptr_t)(uint32_t)(DAT_00d3b5b8));
        }
        else {
          FUN_0040ffd0((const void *)(uintptr_t)(uint32_t)(DAT_00d3b5b8));
        }
        if (DAT_0070e120 == 0) {
          return DAT_00d3b544;
        }
        if (DAT_00d3b531 == '\x01') {
          FUN_004101c0(DAT_00d3b5b8,(uint16_t)DAT_0070e120);
          return DAT_00d3b544;
        }
        if (DAT_00d3b531 == '\x02') {
          FUN_004101c0(DAT_00d3b5b8,(uint16_t)DAT_0070e120);
          return DAT_00d3b544;
        }
        FUN_004100f0(DAT_00d3b5b8,(uint16_t)DAT_0070e120);
        return DAT_00d3b544;
      }
      if (bVar3) {
        if (DAT_00d3b531 == '\x01') {
          FUN_0040fdd0(DAT_00d3b5b8);
        }
        else if (DAT_00d3b531 == '\x02') {
          FUN_0040fcf0(DAT_00d3b5b8);
        }
        else {
          FUN_0040fc40(DAT_00d3b5b8);
        }
      }
      else if (DAT_00d3b531 == '\x01') {
        FUN_004105c0(DAT_00d3b5b8);
      }
      else if (DAT_00d3b531 == '\x02') {
        FUN_00410660(DAT_00d3b5b8);
      }
      else {
          FUN_00410230((const void *)(uintptr_t)(uint32_t)DAT_00d3b5b8);
      }
      if (DAT_0070e120 == 0) {
        return DAT_00d3b544;
      }
      if (DAT_00d3b531 == '\x01') {
        FUN_004109d0(DAT_00d3b5b8,(uint16_t)DAT_0070e120);
        return DAT_00d3b544;
      }
      if (DAT_00d3b531 == '\x02') {
        FUN_00410a70(DAT_00d3b5b8,(uint16_t)DAT_0070e120);
        return DAT_00d3b544;
      }
      FUN_00410700(DAT_00d3b5b8,(uint16_t)DAT_0070e120);
      return DAT_00d3b544;
    }
    if (DAT_00d3b5b4 == '\x1e') {
      if (DAT_00d3b531 == '\x01') {
        if (DAT_00d3b5be == 0x3a) {
          FUN_0042a160(DAT_00d3b5b8,DAT_00d3b530);
          return DAT_00d3b544;
        }
        if (DAT_00d3b5be != 0x1a) {
          if (DAT_00d3b5be != 10) {
            FUN_00410090((const void *)(uintptr_t)(uint32_t)(DAT_00d3b5b8));
            return DAT_00d3b544;
          }
          FUN_0051f780(DAT_00d3b5b8,DAT_00d3b530);
          return DAT_00d3b544;
        }
        FUN_0051f780(DAT_00d3b5b8,DAT_00d3b530);
        return DAT_00d3b544;
      }
      if (DAT_00d3b531 == '\x02') {
        if (DAT_00d3b5be == 0x3a) {
          FUN_0042b440(DAT_00d3b5b8,DAT_00d3b530);
          return DAT_00d3b544;
        }
        if (DAT_00d3b5be != 0x1a) {
          if (DAT_00d3b5be != 10) {
            FUN_0040ff50(DAT_00d3b5b8);
            return DAT_00d3b544;
          }
          FUN_0051f780(DAT_00d3b5b8,DAT_00d3b530);
          return DAT_00d3b544;
        }
        FUN_0051f780(DAT_00d3b5b8,DAT_00d3b530);
        return DAT_00d3b544;
      }
      if (DAT_00d3b5be == 0x3a) {
        FUN_00427a20(DAT_00d3b5b8,DAT_00d3b530);
        return DAT_00d3b544;
      }
      if (DAT_00d3b5be == 0x1a) {
        FUN_0051f780(DAT_00d3b5b8,DAT_00d3b530);
        return DAT_00d3b544;
      }
      if (DAT_00d3b5be == 10) {
        FUN_0051f780(DAT_00d3b5b8,DAT_00d3b530);
        return DAT_00d3b544;
      }
    }
    else {
      if (DAT_00d3b5b4 == '\r') {
        if (DAT_00d3b531 != '\0') {
          FUN_00411fd0(DAT_00d3b5b8);
          return DAT_00d3b544;
        }
      }
      else if (DAT_00d3b5b4 == '\f') {
        if (DAT_00d3b531 != '\0') {
          FUN_004114a0(DAT_00d3b5b8);
          return DAT_00d3b544;
        }
      }
      else if (DAT_00d3b5b4 == '\n') {
        if (DAT_00d3b531 != '\0') {
          FUN_00410fb0(DAT_00d3b5b8);
          return DAT_00d3b544;
        }
      }
      else if (DAT_00d3b5b4 == '\x02') {
        if (DAT_00d3b531 != '\0') {
          FUN_00410f70(DAT_00d3b5b8);
          return DAT_00d3b544;
        }
      }
      else if (DAT_00d3b5b4 == '\x14') {
        if (DAT_00d3b531 == '\x01') {
          FUN_00410de0(DAT_00d3b5b8);
          return DAT_00d3b544;
        }
        if (DAT_00d3b531 == '\x02') {
          FUN_00410e80(DAT_00d3b5b8);
          return DAT_00d3b544;
        }
        FUN_00410b10((const void *)(uintptr_t)(uint32_t)DAT_00d3b5b8);
        return DAT_00d3b544;
      }
      DAT_00d3b5b8 = DAT_00d3b5b8 + (DAT_00d3b5be * DAT_00d3b538 + DAT_00d3b54c) * 2;
      if (!bVar3) {
        if (DAT_00d3b531 == '\x01') {
          FUN_00410090((const void *)(uintptr_t)(uint32_t)(DAT_00d3b5b8));
          return DAT_00d3b544;
        }
        if (DAT_00d3b531 == '\x02') {
          FUN_00410090((const void *)(uintptr_t)(uint32_t)(DAT_00d3b5b8));
          return DAT_00d3b544;
        }
        iVar8 = DAT_00d3b5b8;
        if (DAT_00d3b530 != '\0') {
          FUN_0040ffd0((const void *)(uintptr_t)(uint32_t)(DAT_00d3b5b8));
          return DAT_00d3b544;
        }
        goto LAB_0053ec1d;
      }
      if (DAT_00d3b531 == '\x01') {
        FUN_0040ff50(DAT_00d3b5b8);
        return DAT_00d3b544;
      }
      if (DAT_00d3b531 == '\x02') {
        FUN_0040ff50(DAT_00d3b5b8);
        return DAT_00d3b544;
      }
    }
    FUN_0040fed0(DAT_00d3b5b8);
  }
  return DAT_00d3b544;
}

int FUN_0053e590(int image,int x,int y) {return restore_sprite_draw(RESTORE_SPRITE_LIBRARIES,image,x,y);}
