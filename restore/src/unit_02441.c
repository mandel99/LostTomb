#include "restore_types.h"
#define DAT_005e0400 (*(undefined4 *)0x005e0400u)
#define DAT_0078d7d4 (*(undefined *)0x0078d7d4u)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bca2c4 (*(undefined *)0x00bca2c4u)
#define DAT_00bcd5f0 (*(undefined *)0x00bcd5f0u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd607 (*(undefined4 *)0x00bcd607u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd654 (*(undefined4 *)0x00bcd654u)
#define DAT_00bcd65c (*(undefined4 *)0x00bcd65cu)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00d056f4 (*(undefined *)0x00d056f4u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f535cc (*(undefined *)0x00f535ccu)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define PTR_DAT_005f8ec0 (*(unsigned char * *)0x005f8ec0u)
#define s_Figs_005f8ef4 (*(char (*)[6])0x005f8ef4u)
#define s_Figure_info__F1__005f8f14 (*(char (*)[17])0x005f8f14u)
#define s_Not_watching_a_figure_005f8efc (*(char (*)[22])0x005f8efcu)
#define s___agent_for_005f8e98 (*(char (*)[14])0x005f8e98u)
#define s___dockers_005f8e60 (*(char (*)[12])0x005f8e60u)
#define s___ever_005f8c18 (*(char (*)[8])0x005f8c18u)
#define s___figure_no_005f8ed0 (*(char (*)[13])0x005f8ed0u)
#define s___ghost_005f8e6c (*(char (*)[10])0x005f8e6cu)
#define s___immigrant_to_005f8e84 (*(char (*)[17])0x005f8e84u)
#define s___people_005f8e78 (*(char (*)[11])0x005f8e78u)
#define s___ruler_005f8ec4 (*(char (*)[9])0x005f8ec4u)
#define s_free_route_slots_005f8ee0 (*(char (*)[18])0x005f8ee0u)
#define s_map_x_y_005f8eb4 (*(char (*)[9])0x005f8eb4u)
#define s_x_y_bits_005f8ea8 (*(char (*)[9])0x005f8ea8u)
extern int FUN_00430790();
extern int FUN_004b3320();
extern void FUN_004cc720();
extern void FUN_004d6860();
extern void FUN_00536dc0();

void FUN_00536120(void)

{
  int iVar1;
  undefined4 uVar2;
  char *pcVar3;
  int iVar4;
  
  if (DAT_0078d7d4 == 1) {
    iVar1 = DAT_00e38da4 * 2;
    FUN_00536dc0(6,0,iVar1 + 0xdc,0xc,0xf,1);
    DAT_00bca2c4 = FUN_00430790(0);
    if (DAT_00bca2c4 == 0) {
      DAT_00f535cc = 0;
    }
    else if (*(short *)(&DAT_00b350dc + DAT_00bca2c4 * 2) != 0) {
      DAT_00f535cc = (int)*(short *)(&DAT_00b350dc + DAT_00bca2c4 * 2);
    }
    FUN_004cc720(s_Figure_info__F1__005f8f14,0,iVar1 + 0xe2,0xc0,DAT_00f56f8c,0xffff);
    if (DAT_00f535cc == 0) {
      FUN_004cc720(s_Not_watching_a_figure_005f8efc,0,iVar1 + 0xf4,0xc0,DAT_00f56f8c,0xffff);
      FUN_004d6860(DAT_00d056f4,0x40,s_Figs_005f8ef4,8,iVar1 + 0x10c,DAT_00f56f84,0x3efa,
                   DAT_00e38da8,DAT_00e38da4);
      FUN_004d6860(DAT_00bcd5f0,0x40,s___ever_005f8c18,0x6e,iVar1 + 0x10c,DAT_00f56f84,0x3efa,
                   DAT_00e38da8,DAT_00e38da4);
      uVar2 = FUN_004b3320();
      FUN_004d6860(uVar2,0x40,s_free_route_slots_005f8ee0,8,iVar1 + 0x118,DAT_00f56f84,0x3efa,
                   DAT_00e38da8,DAT_00e38da4);
      return;
    }
    FUN_004d6860(DAT_00f535cc,0x40,s___figure_no_005f8ed0,8,iVar1 + 0xf4,DAT_00f56f84,
                 (-(uint)((&DAT_00bcd606)[DAT_00f535cc * 0x184] != '\x01') & 0xb906) + 0x3efa,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00bcd607)[DAT_00f535cc * 0x184],0x40,s___ruler_005f8ec4,0x6e,iVar1 + 0xf4,
                 DAT_00f56f84,0x3efa,DAT_00e38da8,DAT_00e38da4);
    iVar4 = iVar1 + 0x10c;
    DAT_00e20778 = 0;
    FUN_004d6860((int)(short)(&DAT_00bcd642)[DAT_00f535cc * 0xc2],0x40,&DAT_005e0400,8,iVar4,
                 DAT_00f56f84,0x3efa,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((int)(short)(&DAT_00bcd644)[DAT_00f535cc * 0xc2],0x40,&PTR_DAT_005f8ec0,
                 DAT_00e20778 + 8,iVar4,DAT_00f56f84,0x3efa,DAT_00e38da8,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d6860((int)(short)(&DAT_00bcd60c)[DAT_00f535cc * 0xc2],0x40,&DAT_005e0400,100,iVar4,
                 DAT_00f56f84,0x3efa,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((int)(short)(&DAT_00bcd60e)[DAT_00f535cc * 0xc2],0x40,s_map_x_y_005f8eb4,
                 DAT_00e20778 + 100,iVar4,DAT_00f56f84,0x3efa,DAT_00e38da8,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d6860(*(byte *)(&DAT_00bcd642 + DAT_00f535cc * 0xc2) & 0xf,0x40,&DAT_005e0400,8,
                 iVar1 + 0x124,DAT_00f56f84,0x3efa,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(*(byte *)(&DAT_00bcd644 + DAT_00f535cc * 0xc2) & 0xf,0x40,s_x_y_bits_005f8ea8,
                 DAT_00e20778 + 8,iVar1 + 0x124,DAT_00f56f84,0x3efa,DAT_00e38da8,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d6860((int)(short)(&DAT_00bcd652)[DAT_00f535cc * 0xc2],0x40,s___agent_for_005f8e98,8,
                 iVar1 + 0x130,DAT_00f56f84,0x3efa,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((int)(short)(&DAT_00bcd654)[DAT_00f535cc * 0xc2],0x40,s___immigrant_to_005f8e84,
                 DAT_00e20778 + 8,iVar1 + 0x130,DAT_00f56f84,0x3efa,DAT_00e38da8,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d6860((&DAT_00bcd65c)[DAT_00f535cc * 0x184],0x40,s___people_005f8e78,8,iVar1 + 0x13c,
                 DAT_00f56f84,0x3efa,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00bcd65d)[DAT_00f535cc * 0x184],0x40,s___ghost_005f8e6c,DAT_00e20778 + 8,
                 iVar1 + 0x13c,DAT_00f56f84,0x3efa,DAT_00e38da8,DAT_00e38da4);
    iVar4 = 0;
    pcVar3 = &DAT_00bcd786;
    do {
      if ((pcVar3[4] == '\x01') && (*pcVar3 == '&')) {
        iVar4 = iVar4 + 1;
      }
      pcVar3 = pcVar3 + 0x184;
    } while ((int)pcVar3 < 0xc8ad42);
    DAT_00e20778 = 0;
    FUN_004d6860(iVar4,0x40,s___dockers_005f8e60,8,iVar1 + 0x148,DAT_00f56f84,0x3efa,DAT_00e38da8,
                 DAT_00e38da4);
  }
  return;
}
