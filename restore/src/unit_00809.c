#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d7f0 (*(undefined *)0x0078d7f0u)
#define DAT_0078d7f4 (*(undefined *)0x0078d7f4u)
#define DAT_0078d9a8 (*(undefined4 *)0x0078d9a8u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
extern void FUN_0047fc40();
extern undefined4 FUN_004816b0();
extern int FUN_00488030();

void FUN_00455430(void)

{
  char cVar1;
  int iVar2;
  short *psVar3;
  int iVar4;
  
  DAT_0078d7f0 = 0;
  iVar4 = 1;
  if (0 < DAT_0078d790) {
    psVar3 = &DAT_00799af0;
    do {
      if ((char)psVar3[-8] == '\x01') {
        if (*psVar3 == 0x5c) {
          (&DAT_0078d9a8)[DAT_0078d7f0] = (short)iVar4;
          DAT_0078d7f0 = DAT_0078d7f0 + 1;
          if (499 < DAT_0078d7f0) {
            DAT_0078d7f0 = 499;
          }
        }
        else if (*(char *)((int)psVar3 + -0xb) == '\0') {
          cVar1 = FUN_00488030(iVar4);
          if (cVar1 != '\0') {
            iVar2 = FUN_004816b0((int)psVar3[-5],(int)psVar3[-4],*(undefined1 *)((int)psVar3 + -0xd)
                                 ,0x1000000);
            if (iVar2 != 0) goto LAB_004554d2;
            *(undefined1 *)((int)psVar3 + 0x25) = 0;
          }
        }
        else {
          *(undefined1 *)((int)psVar3 + 0x25) = 0;
          *(undefined1 *)((int)psVar3 + 0x2f) = 0;
          if (*(char *)((int)psVar3 + 0x67) != '\0') {
LAB_004554d2: ;
            *(undefined1 *)((int)psVar3 + 0x25) = 1;
          }
        }
      }
      iVar4 = iVar4 + 1;
      psVar3 = psVar3 + 0x84;
    } while (iVar4 <= DAT_0078d790);
    if (0 < DAT_0078d7f0) {
      DAT_0078d7f4 = 0;
      iVar4 = 0;
      if (0 < DAT_0078d7f0) {
        do {
          DAT_0078d7f4 = iVar4 + 1;
          FUN_0047fc40((int)(short)(&DAT_0078d9a8)[iVar4],2);
          iVar4 = DAT_0078d7f4;
        } while (DAT_0078d7f4 < DAT_0078d7f0);
      }
    }
  }
  return;
}
