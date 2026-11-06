#include "restore_types.h"
#include "sprite_render.h"
#include <string.h>
#define DAT_005e05ac (*(uint32_t *)0x005e05acu)
#define DAT_005e06e0 (*(undefined4 *)0x005e06e0u)
#define DAT_006018f8 (*(uint32_t *)0x006018f8u)
#define DAT_006018fc (*(uint16_t *)0x006018fcu)
#define DAT_00d3b5be (*(int16_t *)0x00d3b5beu)
#define DAT_00e38e62 (*(undefined1 *)0x00e38e62u)
extern void FUN_004cdf20();
extern bool FUN_004ce100();
extern void FUN_004ce2d0();
extern void FUN_004ce310();
extern int FUN_004ce9e0(void);
extern undefined4 FUN_005647eb();
extern int FUN_005660ed();
extern DWORD FUN_005662e3();
extern uint FUN_00568eab();

int32_t restore_sprite_fetch_clipped(RestoreSpriteLibraries *self,char *param_1,int32_t param_2,int32_t param_3,int32_t param_4,uint32_t param_5)

{
  char cVar1;
  int iVar2;
  uintptr_t in_ECX=(uintptr_t)self;
  int iVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  char *pcVar7;
  char *pcVar9;
  char *pcVar10;
  int local_110;
  int local_10c;
  char native_path[260];
  uint32_t path_prefix = DAT_006018f8;
  uint16_t path_suffix = DAT_006018fc;

  /* 53F7F1..53F811: one pathname object, including the final zero WORD. */
  memcpy(native_path, &path_prefix, sizeof(path_prefix));
  memcpy(native_path + 4, &path_suffix, sizeof(path_suffix));
  memset(native_path + 6, 0, sizeof(native_path) - 6);
  uVar4 = 0xffffffff;
  iVar3 = DAT_00d3b5be * 2 - param_3;
  do {
    pcVar7 = param_1;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar7 = param_1 + 1;
    cVar1 = *param_1;
    param_1 = pcVar7;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  iVar5 = -1;
  pcVar10 = native_path;
  do {
    pcVar9 = pcVar10;
    if (iVar5 == 0) break;
    iVar5 = iVar5 + -1;
    pcVar9 = pcVar10 + 1;
    cVar1 = *pcVar10;
    pcVar10 = pcVar9;
  } while (cVar1 != '\0');
  pcVar7 = pcVar7 - uVar4;
  pcVar10 = pcVar9 + -1;
  for (uVar6 = uVar4 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
    *(undefined4 *)pcVar10 = *(undefined4 *)pcVar7;
    pcVar7 = pcVar7 + 4;
    pcVar10 = pcVar10 + 4;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar10 = *pcVar7;
    pcVar7 = pcVar7 + 1;
    pcVar10 = pcVar10 + 1;
  }
  local_10c = 0;
  local_110 = 1;
  FUN_004ce310(native_path,&DAT_005e06e0);
  iVar5 = FUN_00568eab(native_path,0x8000);
  if (iVar5 == -1) {
    uVar4 = 0xffffffff;
    pcVar7 = native_path;
    do {
      pcVar10 = pcVar7;
      if (uVar4 == 0) break;
      uVar4 = uVar4 - 1;
      pcVar10 = pcVar7 + 1;
      cVar1 = *pcVar7;
      pcVar7 = pcVar10;
    } while (cVar1 != '\0');
    uVar4 = ~uVar4;
    RESTORE_SLICE1(DAT_005e05ac,0) = DAT_00e38e62;
    iVar5 = -1;
    RESTORE_SLICE1(DAT_005e05ac,3) = 0;
    pcVar7 = (char *)&DAT_005e05ac;
    do {
      pcVar9 = pcVar7;
      if (iVar5 == 0) break;
      iVar5 = iVar5 + -1;
      pcVar9 = pcVar7 + 1;
      cVar1 = *pcVar7;
      pcVar7 = pcVar9;
    } while (cVar1 != '\0');
    pcVar7 = pcVar10 - uVar4;
    pcVar10 = pcVar9 + -1;
    for (uVar6 = uVar4 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar10 = *(undefined4 *)pcVar7;
      pcVar7 = pcVar7 + 4;
      pcVar10 = pcVar10 + 4;
    }
    for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
      *pcVar10 = *pcVar7;
      pcVar7 = pcVar7 + 1;
      pcVar10 = pcVar10 + 1;
    }
    iVar5 = FUN_00568eab(&DAT_005e05ac,0x8000);
    while (iVar5 == -1) {
      if (local_110 != 1) goto LAB_0053f987;
      local_110 = FUN_004ce9e0();
      iVar5 = FUN_00568eab(&DAT_005e05ac,0x8000);
    }
  }
  iVar2 = FUN_005662e3(iVar5,param_5,0);
  if ((iVar2 != -1) && (0 < param_4)) {
    local_110 = param_4;
    do {
      iVar2 = FUN_005660ed(iVar5,param_2,param_3);
      local_10c = local_10c + iVar2;
      FUN_005662e3(iVar5,iVar3,1);
      param_2 = param_2 + param_3;
      local_110 = local_110 + -1;
    } while (local_110 != 0);
  }
  FUN_005647eb(iVar5);
  if (local_10c < 1) {
LAB_0053f987: ;
    FUN_004ce100(native_path);
    iVar5 = FUN_00568eab(native_path,0x8000);
    if (iVar5 != -1) {
      iVar2 = FUN_005662e3(iVar5,param_5,0);
      if ((iVar2 != -1) && (0 < param_4)) {
        local_110 = param_4;
        do {
          iVar2 = FUN_005660ed(iVar5,param_2,param_3);
          local_10c = local_10c + iVar2;
          FUN_005662e3(iVar5,iVar3,1);
          param_2 = param_2 + param_3;
          local_110 = local_110 + -1;
        } while (local_110 != 0);
      }
      FUN_005647eb(iVar5);
    }
    FUN_004ce2d0();
    if (local_10c == 0) {
      return 0;
    }
  }
  FUN_004cdf20(*(undefined4 *)(in_ECX + 0x30f5c0),local_10c);
  return local_10c;
}

int FUN_0053f7d0(char *buffer,int destination,int width,int rows,uint32_t offset)
{return restore_sprite_fetch_clipped(RESTORE_SPRITE_LIBRARIES,buffer,destination,width,rows,offset);}
