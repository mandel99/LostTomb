#include "restore_types.h"
#define DAT_0078d7c4 (*(undefined *)0x0078d7c4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a7e (*(undefined4 *)0x00799a7eu)
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00bca348 (*(undefined1 *)0x00bca348u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00d386d4 (*(undefined *)0x00d386d4u)
#define DAT_00ea6204 (*(undefined4 *)0x00ea6204u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00455c30();
extern int FUN_00488030();
extern int FUN_004883d0();
extern void FUN_00488e60();
extern bool FUN_004890f0();
extern void FUN_00489180();
extern uint FUN_004894a0();
extern void FUN_004beeb0();

int FUN_00488400(int param_1)

{
  char cVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int in_ECX;
  int iVar5;
  int iVar6;
  
  if ((DAT_0078d7c4 == 0) || (DAT_0078d7c4 == 7)) {
    iVar5 = -1;
    cVar1 = FUN_00488030(param_1);
    if (cVar1 != '\0') {
      switch((&DAT_007999e8)[param_1 * 0x84]) {
      case 100: ;
        iVar5 = 0;
        break;
      case 0x65: ;
        iVar5 = 1;
        break;
      case 0x66: ;
        iVar5 = 2;
        break;
      case 0x67: ;
        iVar5 = 3;
        break;
      case 0x68: ;
        iVar5 = 4;
        break;
      case 0x69: ;
        iVar5 = 5;
        break;
      case 0xc3: ;
        iVar5 = 8;
        break;
      case 0xc4: ;
        iVar5 = 7;
        break;
      case 0xe0: ;
        iVar5 = 9;
      }
    }
    cVar1 = FUN_004894a0(param_1);
    if (cVar1 == '\0') {
      if ((*(char *)(in_ECX + iVar5) != '\0') && (DAT_0078d7c4 == 0)) goto LAB_004884bd;
    }
    else if (DAT_00d386d4 == 1) {
LAB_004884bd: ;
      iVar3 = FUN_004883d0(param_1);
      iVar5 = iVar3 << 3;
      FUN_00455c30(param_1);
      FUN_00488e60(param_1,0);
      cVar2 = FUN_004890f0(param_1);
      FUN_00489180(param_1,(-(uint)(cVar2 != '\0') & 0xffffffe2) + 0x50);
      if (cVar1 != '\0') {
        if (*(int *)(&DAT_00ea6204 + DAT_00eaac28 * 0x49d8) != 0) {
          iVar5 = iVar3 << 4;
          DAT_00bca348 = 1;
        }
        iVar6 = param_1 * 0x108;
        *(undefined1 *)((int)&DAT_00799a7e + iVar6 + 1) = 0;
        (&DAT_00799a18)[param_1 * 0x84] = 0;
        iVar3 = (int)*(short *)(&DAT_00799a8c + iVar6);
        if (iVar3 != 0) {
          iVar4 = iVar3 * 0x184;
          if (((((&DAT_00bcd606)[iVar4] != '\x02') && ((&DAT_00bcd602)[iVar4] == 'U')) &&
              ((&DAT_00bcd630)[iVar4] != '\x06')) && ((&DAT_00bcd630)[iVar4] != '\a')) {
            FUN_004beeb0(iVar3);
          }
          *(undefined2 *)(&DAT_00799a8c + iVar6) = 0;
        }
      }
      return iVar5;
    }
  }
  return 0;
}
