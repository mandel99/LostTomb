#include "restore_types.h"
#include "flood_simulation.h"
#define DAT_005cd6c8 (*(undefined *)0x005cd6c8u)
#define DAT_005cd6d8 (*(undefined *)0x005cd6d8u)
#define DAT_0078d790 (*(int *)0x0078d790u)
#define DAT_0078d7b0 (*(int *)0x0078d7b0u)
#define DAT_0078d83c (*(int *)0x0078d83cu)
#define DAT_0078d840 (*(int *)0x0078d840u)
#define DAT_0078e178 (*(short *)0x0078e178u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined *)0x007999e0u)
#define DAT_007999e4 (*(undefined *)0x007999e4u)
#define DAT_00799a0d (*(undefined *)0x00799a0du)
#define DAT_00799a18 (*(short *)0x00799a18u)
#define DAT_00799a82 (*(undefined2 *)0x00799a82u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00799b15 (*(undefined4 *)0x00799b15u)
#define DAT_00ac2d4c (*(undefined *)0x00ac2d4cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc96dc (*(uint *)0x00bc96dcu)
#define DAT_00f564d4 (*(int *)0x00f564d4u)
#define DAT_00f56644 (*(int *)0x00f56644u)
extern void FUN_0040f8f0();
extern void FUN_00455540();
extern void FUN_0047bf50();
extern int FUN_0047cc70();
extern void FUN_004824b0();
extern undefined4 FUN_00487e60();
extern void FUN_00487ec0();

void FUN_004555d0(void)

{
  char cVar1;
  short sVar2;
  bool bVar3;
  char cVar4;
  int iVar5;
  char *pcVar6;
  int iVar7;
  short *psVar8;
  int iVar9;
  
  FUN_00455540(&DAT_00af598c,0xcb10,0xfeffdfff);
  FUN_00468790();
  FUN_0040f8f0(&DAT_0078e178,8000,0);
  iVar9 = 1;
  DAT_0078d840 = 0;
  DAT_0078d83c = 0;
  if (0 < DAT_0078d790) {
    pcVar6 = &DAT_00799b15;
    do {
      if ((pcVar6[-0x35] == '\x01') && (*(short *)(pcVar6 + -0x25) == 0x5a)) {
        (&DAT_0078e178)[DAT_0078d83c] = (short)iVar9;
        DAT_0078d83c = DAT_0078d83c + 1;
        cVar1 = *pcVar6;
        if (DAT_0078d7b0 == 0) {
          cVar4 = FUN_00487e60(iVar9);
          if (cVar4 == '\0') goto LAB_00455666;
          *pcVar6 = '\x02';
        }
        else {
LAB_00455666: ;
          *pcVar6 = '\0';
        }
        if (*pcVar6 != cVar1) {
          FUN_00487ec0(iVar9);
        }
      }
      iVar9 = iVar9 + 1;
      pcVar6 = pcVar6 + 0x108;
    } while (iVar9 <= DAT_0078d790);
  }
  FUN_004beb90((uintptr_t)RESTORE_CITY_WORLD);
  iVar9 = DAT_0078d83c;
  do {
    bVar3 = false;
    DAT_0078d840 = 0;
    if (iVar9 < 1) break;
    do {
      iVar5 = (int)(short)(&DAT_0078e178)[DAT_0078d840];
      DAT_0078d840 = DAT_0078d840 + 1;
      iVar7 = iVar5 * 0x108;
      if (((&DAT_00799a0d)[iVar7] == '\x02') && (0 < (short)(&DAT_00799a18)[iVar5 * 0x84])) {
        (&DAT_00799a0d)[iVar7] = 1;
        bVar3 = true;
        FUN_0047cc70((int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                     (int)*(short *)(&DAT_007999e0 + iVar7),0,1);
        DAT_00bc96dc = (uint)*(byte *)(&DAT_00799a82 + iVar5 * 0x84);
        FUN_00468590(*(int *)(&DAT_005cd6c8 + (DAT_00bc96dc ^ 2) * 4) +
                     *(int *)(&DAT_007999e4 + iVar7));
        FUN_00468590(*(int *)(&DAT_005cd6d8 + (DAT_00bc96dc ^ 2) * 4) +
                     *(int *)(&DAT_007999e4 + iVar7));
        iVar9 = DAT_0078d83c;
      }
    } while (DAT_0078d840 < iVar9);
  } while (bVar3);
  DAT_0078d840 = 0;
  iVar5 = 0;
  if (0 < iVar9) {
    do {
      DAT_0078d840 = iVar5 + 1;
      iVar7 = (short)(&DAT_0078e178)[iVar5] * 0x108;
      if ((0 < (short)(&DAT_00799a18)[(short)(&DAT_0078e178)[iVar5] * 0x84]) &&
         ((&DAT_00799a0d)[iVar7] != '\0')) {
        FUN_004824b0((int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                     (int)*(short *)(&DAT_007999e0 + iVar7),2,2,0x1000000);
        iVar9 = DAT_0078d83c;
      }
      iVar5 = DAT_0078d840;
    } while (DAT_0078d840 < iVar9);
  }
  iVar9 = 1;
  if (0 < DAT_0078d790) {
    psVar8 = &DAT_00799af0;
    do {
      if (((char)psVar8[-8] == '\x01') && ((sVar2 = *psVar8, sVar2 == 0x5c || (sVar2 == 0xb4)))) {
        if (sVar2 == 0x5c) {
          FUN_0047bf50(iVar9,(int)psVar8[-5],(int)psVar8[-4],1,1,
                       (((char)(&DAT_00ac2d4c)[*(int *)(psVar8 + -2)] < '\x1e') - 1 & 2) +
                       DAT_00f564d4,8);
          FUN_004824b0((int)psVar8[-5],(int)psVar8[-4],1,2,0x2000);
        }
        else {
          FUN_0047bf50(iVar9,(int)psVar8[-5],(int)psVar8[-4],2,2,
                       (((char)(&DAT_00ac2d4c)[*(int *)(psVar8 + -2)] < '\x1e') - 1 & 2) +
                       DAT_00f56644,8);
        }
        *(undefined1 *)((int)psVar8 + 0x25) = 1;
      }
      iVar9 = iVar9 + 1;
      psVar8 = psVar8 + 0x84;
    } while (iVar9 <= DAT_0078d790);
  }
  return;
}
