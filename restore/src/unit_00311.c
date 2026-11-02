#include "restore_types.h"
#define DAT_00704db8 (*(int32_t *)0x00704db8u)
#define DAT_00704dbc (*(int32_t *)0x00704dbcu)
#define DAT_00d3b198 (*(int32_t *)0x00d3b198u)
#define DAT_00d3b1b3 (*(char *)0x00d3b1b3u)
#define DAT_00d3b1c0 (*(undefined1 *)0x00d3b1c0u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define s_maps___map_00576260 (*(char (*)[11])0x00576260u)
extern void FUN_0041f670();
extern void FUN_0041f810();

void FUN_0041f870(void)

{
  DAT_00e38dda = 0x3d;
  FUN_0041f810(s_maps___map_00576260);
  DAT_00d3b1b3 = 0;
  DAT_00704db8 = 0;
  DAT_00d3b1c0 = 1;
  DAT_00d3b198 = DAT_00704dbc;
  FUN_0041f670();
  DAT_00e38dca = 1;
  return;
}
