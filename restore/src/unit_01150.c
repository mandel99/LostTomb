#include "restore_types.h"
#define DAT_00af55f8 (*(undefined4 *)0x00af55f8u)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5600 (*(undefined4 *)0x00af5600u)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d18 (*(undefined4 *)0x00af5d18u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00af5d20 (*(undefined4 *)0x00af5d20u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bc97c0 (*(undefined *)0x00bc97c0u)
#define DAT_00bc97c4 (*(undefined *)0x00bc97c4u)
#define DAT_00bc97c8 (*(undefined *)0x00bc97c8u)
#define DAT_00bc97cc (*(undefined *)0x00bc97ccu)
#define DAT_00bc97d0 (*(undefined *)0x00bc97d0u)
#define DAT_00bc97d4 (*(undefined *)0x00bc97d4u)
#define DAT_00bc97d8 (*(undefined *)0x00bc97d8u)
#define DAT_00bc97dc (*(undefined *)0x00bc97dcu)

void FUN_0047e750(void)

{
  DAT_00bc97c0 = DAT_00bc97c0 + (*(uint *)(&DAT_00af55fc + DAT_00bc9768 * 4) >> 0xe & 1);
  DAT_00bc97c4 = DAT_00bc97c4 + (*(uint *)(&DAT_00af5600 + DAT_00bc9768 * 4) >> 0xe & 1);
  DAT_00bc97c8 = DAT_00bc97c8 + (*(uint *)(&DAT_00af5990 + DAT_00bc9768 * 4) >> 0xe & 1);
  DAT_00bc97cc = DAT_00bc97cc + (*(uint *)(&DAT_00af5d20 + DAT_00bc9768 * 4) >> 0xe & 1);
  DAT_00bc97d0 = DAT_00bc97d0 + (*(uint *)(&DAT_00af5d1c + DAT_00bc9768 * 4) >> 0xe & 1);
  DAT_00bc97d4 = DAT_00bc97d4 + (*(uint *)(&DAT_00af5d18 + DAT_00bc9768 * 4) >> 0xe & 1);
  DAT_00bc97d8 = DAT_00bc97d8 + (*(uint *)(&DAT_00af5988 + DAT_00bc9768 * 4) >> 0xe & 1);
  DAT_00bc97dc = DAT_00bc97dc + (*(uint *)(&DAT_00af55f8 + DAT_00bc9768 * 4) >> 0xe & 1);
  return;
}
