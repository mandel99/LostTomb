#include "restore_types.h"
#define DAT_00ab6158 (*(undefined4 *)0x00ab6158u)
#define DAT_00ab623b (*(undefined4 *)0x00ab623bu)
#define DAT_00ab623c (*(undefined4 *)0x00ab623cu)
#define DAT_00ab623d (*(undefined4 *)0x00ab623du)
#define DAT_00ab6320 (*(undefined4 *)0x00ab6320u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
extern bool FUN_0047e830();

void FUN_00479670(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      DAT_00bc9768 = (&DAT_00bc9804)[iVar4];
      if ((int)(&DAT_00bc9f24)[iVar4] < (int)(&DAT_00bc9b94)[iVar4]) {
        iVar2 = (&DAT_00bc9b94)[iVar4] - (&DAT_00bc9f24)[iVar4];
        do {
          if (((&DAT_00ab623c)[DAT_00bc9768] != 0) &&
             (((((iVar3 = (byte)(&DAT_00ab623c)[DAT_00bc9768] - 1,
                 (int)(uint)(byte)(&DAT_00ab6158)[DAT_00bc9768] < iVar3 &&
                 (iVar1 = FUN_0047e830(DAT_00bc9768 + -0xe4), iVar1 == 0)) ||
                (((int)(uint)(byte)(&DAT_00ab623d)[DAT_00bc9768] < iVar3 &&
                 (iVar1 = FUN_0047e830(DAT_00bc9768 + 1), iVar1 == 0)))) ||
               (((int)(uint)(byte)(&DAT_00ab6320)[DAT_00bc9768] < iVar3 &&
                (iVar1 = FUN_0047e830(DAT_00bc9768 + 0xe4), iVar1 == 0)))) ||
              (((int)(uint)(byte)(&DAT_00ab623b)[DAT_00bc9768] < iVar3 &&
               (iVar3 = FUN_0047e830(DAT_00bc9768 + -1), iVar3 == 0)))))) {
            (&DAT_00ab623c)[DAT_00bc9768] = (&DAT_00ab623c)[DAT_00bc9768] + -1;
          }
          DAT_00bc9768 = DAT_00bc9768 + 1;
          iVar2 = iVar2 + -1;
        } while (iVar2 != 0);
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < DAT_00bca2d4);
  }
  return;
}
