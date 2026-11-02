#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38e6c (*(undefined *)0x00e38e6cu)

void FUN_00403320(void)

{
  if (DAT_00605a3c != 1) {
    if (DAT_00d3b198 == 1) {
      DAT_00e38e6c = DAT_00e38e6c + -10;
    }
    else if (DAT_00d3b198 == 0) {
      DAT_00e38e6c = DAT_00e38e6c + 10;
    }
    if (DAT_00e38e6c < 10) {
      DAT_00e38e6c = 10;
      DAT_00e38dca = 1;
      return;
    }
    if (100 < DAT_00e38e6c) {
      DAT_00e38e6c = 100;
    }
    DAT_00e38dca = 1;
  }
  return;
}
