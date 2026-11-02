#include "restore_types.h"
#define DAT_00d3a6b4 (*(undefined *)0x00d3a6b4u)
#define DAT_00d3a6b8 (*(undefined *)0x00d3a6b8u)
#define DAT_00d3a6bc (*(undefined *)0x00d3a6bcu)
#define DAT_00d3a6e4 (*(undefined *)0x00d3a6e4u)
#define DAT_00d3b130 (*(undefined *)0x00d3b130u)
#define DAT_00d3b138 (*(undefined *)0x00d3b138u)
#define DAT_00d3b13c (*(undefined *)0x00d3b13cu)
#define DAT_00e38dca (*(byte *)0x00e38dcau)

undefined4 FUN_004ca930(void)

{
  if (DAT_00d3a6e4 < 1) {
    return 0;
  }
  DAT_00d3b130 = DAT_00d3a6bc;
  DAT_00d3a6e4 = 0;
  DAT_00d3b13c = DAT_00d3a6b8;
  DAT_00d3b138 = DAT_00d3a6b4;
  if (DAT_00e38dca == '\0') {
    DAT_00e38dca = '\x02';
  }
  return 1;
}
