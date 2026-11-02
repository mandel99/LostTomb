#include "restore_types.h"
#define DAT_00d3b40c (*(undefined *)0x00d3b40cu)
#define DAT_00d3b410 (*(undefined *)0x00d3b410u)
#define DAT_00d3b414 (*(undefined *)0x00d3b414u)
#define DAT_00d3b418 (*(undefined *)0x00d3b418u)
#define DAT_00d3b41c (*(undefined *)0x00d3b41cu)
#define DAT_00d3b420 (*(undefined *)0x00d3b420u)
#define DAT_00d3b424 (*(undefined *)0x00d3b424u)
#define DAT_00d3b428 (*(undefined *)0x00d3b428u)
#define DAT_00d3b42c (*(undefined *)0x00d3b42cu)
#define DAT_00d3b430 (*(undefined *)0x00d3b430u)

void FUN_004cdc70(void)

{
  DAT_00d3b430 = DAT_00d3b430 + 1;
  if (3 < DAT_00d3b430) {
    DAT_00d3b430 = 0;
  }
  DAT_00d3b42c = DAT_00d3b42c + 1;
  if (7 < DAT_00d3b42c) {
    DAT_00d3b42c = 0;
  }
  DAT_00d3b428 = DAT_00d3b428 + 1;
  if (0xf < DAT_00d3b428) {
    DAT_00d3b428 = 0;
  }
  DAT_00d3b424 = DAT_00d3b424 + 1;
  if (0x17 < DAT_00d3b424) {
    DAT_00d3b424 = 0;
  }
  DAT_00d3b420 = DAT_00d3b420 + 1;
  if (0x1f < DAT_00d3b420) {
    DAT_00d3b420 = 0;
  }
  DAT_00d3b41c = DAT_00d3b41c + 1;
  if (0x3f < DAT_00d3b41c) {
    DAT_00d3b41c = 0;
  }
  DAT_00d3b418 = DAT_00d3b418 + 1;
  if (0x7f < DAT_00d3b418) {
    DAT_00d3b418 = 0;
  }
  DAT_00d3b414 = DAT_00d3b414 + 1;
  if (0xff < DAT_00d3b414) {
    DAT_00d3b414 = 0;
  }
  DAT_00d3b410 = DAT_00d3b410 + 1;
  if (0x17f < DAT_00d3b410) {
    DAT_00d3b410 = 0;
  }
  DAT_00d3b40c = DAT_00d3b40c + 1;
  if (0x2ff < DAT_00d3b40c) {
    DAT_00d3b40c = 0;
  }
  return;
}
