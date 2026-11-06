#include "restore_types.h"
#define DAT_0078d78c (*(undefined *)0x0078d78cu)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d794 (*(undefined *)0x0078d794u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_0078d7c4 (*(undefined *)0x0078d7c4u)
#define DAT_0078d7cc (*(undefined *)0x0078d7ccu)
#define DAT_0078d7dc (*(undefined *)0x0078d7dcu)
#define DAT_0078d7e0 (*(undefined *)0x0078d7e0u)
#define DAT_0078d7e8 (*(undefined *)0x0078d7e8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00eadfd4 (*(undefined *)0x00eadfd4u)
#define DAT_00eadfd8 (*(undefined *)0x00eadfd8u)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define s_Evolve_info__F3__005f8c58 (*(char (*)[17])0x005f8c58u)
#define s___barb_005f8bc8 (*(char (*)[8])0x005f8bc8u)
#define s___ever_005f8c18 (*(char (*)[8])0x005f8c18u)
#define s___evolve_error_005f8bdc (*(char (*)[16])0x005f8bdcu)
#define s___last_building_005f8c20 (*(char (*)[17])0x005f8c20u)
#define s___month_005f8c3c (*(char (*)[9])0x005f8c3cu)
#define s___no_of_connects_005f8c04 (*(char (*)[18])0x005f8c04u)
#define s___routings_005f8bd0 (*(char (*)[12])0x005f8bd0u)
#define s___tick_005f8c50 (*(char (*)[8])0x005f8c50u)
#define s___total_connections_005f8bec (*(char (*)[21])0x005f8becu)
#define s___week_005f8c48 (*(char (*)[8])0x005f8c48u)
#define s___year_005f8c34 (*(char (*)[8])0x005f8c34u)
extern void FUN_004cc720();
extern void FUN_004d6860();
extern void FUN_00536dc0();

void FUN_00534df0(void)

{
  int iVar1;
  int iVar2;
  
  if (DAT_0078d7dc == 1) {
    iVar1 = DAT_00e38da4 * 2;
    FUN_00536dc0(6,0,iVar1 + 0xdc,0xc,0xf,1);
    FUN_004cc720(s_Evolve_info__F3__005f8c58,0,iVar1 + 0xe2,0xc0,DAT_00f56f8c,0xffff);
    FUN_004d6860(DAT_0078d7cc,0x40,s___tick_005f8c50,8,iVar1 + 0xf6,DAT_00f56f84,0x3efa,DAT_00e38da8
                 ,DAT_00e38da4);
    FUN_004d6860(DAT_0078d7c4,0x40,s___week_005f8c48,0x6e,iVar1 + 0xf6,DAT_00f56f84,0x3efa,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(DAT_0078d7c0,0x40,s___month_005f8c3c,8,iVar1 + 0x104,DAT_00f56f84,0x3efa,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(DAT_0078d7bc,0x40,s___year_005f8c34,0x6e,iVar1 + 0x104,DAT_00f56f84,0x3efa,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(DAT_0078d790,0x40,s___last_building_005f8c20,8,iVar1 + 0x112,DAT_00f56f84,0x3efa,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(DAT_0078d78c,0x40,s___ever_005f8c18,0x6e,iVar1 + 0x112,DAT_00f56f84,0x3efa,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(DAT_0078d7e8 + -1,0x40,s___no_of_connects_005f8c04,8,iVar1 + 0x120,DAT_00f56f84,
                 0x3efa,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(DAT_0078d794,0x40,s___total_connections_005f8bec,8,iVar1 + 0x13c,DAT_00f56f84,
                 0x3efa,DAT_00e38da8,DAT_00e38da4);
    iVar2 = (-(uint)(DAT_0078d7e0 != 0) & 0xb906) + 0x3efa;
    FUN_004d6860(DAT_0078d7e0,0x40,s___evolve_error_005f8bdc,8,iVar1 + 0x14a,DAT_00f56f84,iVar2,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(DAT_00eadfd4,0x40,s___routings_005f8bd0,8,iVar1 + 0x158,DAT_00f56f84,iVar2,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(DAT_00eadfd8,0x40,s___barb_005f8bc8,8,iVar1 + 0x166,DAT_00f56f84,iVar2,DAT_00e38da8
                 ,DAT_00e38da4);
  }
  return;
}
