#include "restore_types.h"
#define DAT_0070e12c (*(undefined *)0x0070e12cu)
#define DAT_0070e138 (*(undefined *)0x0070e138u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799afc (*(undefined4 *)0x00799afcu)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern int FUN_0042fca0();
extern undefined4 FUN_0042fe90();
#define _param_1 restore_param_1

undefined4 FUN_0042fcf0(unsigned int restore_param_1)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  short *local_8;
  int local_4;
  
  iVar2 = FUN_0042fca0();
  if ((RESTORE_SLICE1(restore_param_1,0) != '\0') && (iVar2 == 0)) {
    iVar6 = 0;
    _param_1 = 1;
    if (0 < DAT_0078d790) {
      local_8 = &DAT_00799afc;
      uVar3 = DAT_00d3b374;
      local_4 = iVar2;
      do {
        if ((((char)local_8[-0xe] != '\0') && (*local_8 != 0)) &&
           (*(char *)((int)local_8 + -0x17) != '\0')) {
          iVar6 = iVar6 + 1;
          (&DAT_00d3b1c8)[DAT_00d3b1c4] = uVar3;
          DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
          if (99 < DAT_00d3b1c4) {
            DAT_00d3b1c4 = 0;
          }
          iVar2 = 0x1f;
          do {
            uVar3 = DAT_00d3b384 >> 4;
            uVar4 = DAT_00d3b384 & 1;
            uVar5 = DAT_00d3b380 >> 4;
            uVar7 = DAT_00d3b380 & 1;
            DAT_00d3b384 = DAT_00d3b384 >> 1;
            DAT_00d3b380 = DAT_00d3b380 >> 1;
            if ((uVar3 & 1) != uVar4) {
              DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
            }
            if ((uVar5 & 1) != uVar7) {
              DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
            }
            iVar2 = iVar2 + -1;
          } while (iVar2 != 0);
          _DAT_00d3b378 = DAT_00d3b384 & 7;
          _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
          _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
          _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
          _DAT_00d3b36c = DAT_00d3b380 & 7;
          uVar3 = DAT_00d3b384 & 0x7f;
          iVar2 = local_4;
          DAT_00d3b374 = uVar3;
          if ((int)((longlong)(ulonglong)_DAT_00d3b370 % (longlong)iVar6) == 0) {
            local_4 = _param_1;
            iVar2 = _param_1;
          }
        }
        local_8 = local_8 + 0x84;
        _param_1 = _param_1 + 1;
      } while (_param_1 <= DAT_0078d790);
    }
  }
  cVar1 = FUN_0042fe90(iVar2);
  if (cVar1 == '\0') {
    return 0;
  }
  DAT_0070e12c = iVar2;
  DAT_0070e138 = 1;
  return 1;
}
