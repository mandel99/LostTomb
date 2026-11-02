#include "restore_types.h"
/* Native 4147F0 uses DWORD settings and WORD menu labels. A label DWORD
 * would overwrite the low half of the adjacent callback address. */
#define DAT_00e38e58 (*(uint32_t *)0x00e38e58u)
#define DAT_00e38e5c (*(uint32_t *)0x00e38e5cu)
#define DAT_00e38e60 (*(uint8_t *)0x00e38e60u)
#define DAT_00e38e61 (*(uint8_t *)0x00e38e61u)
#define DAT_00e38e62 (*(uint8_t *)0x00e38e62u)
#define DAT_00e38e63 (*(uint8_t *)0x00e38e63u)
#define DAT_00e38e64 (*(uint8_t *)0x00e38e64u)
#define DAT_00e38e65 (*(uint8_t *)0x00e38e65u)
#define DAT_00e38e68 (*(uint32_t *)0x00e38e68u)
#define DAT_00e38e6c (*(uint32_t *)0x00e38e6cu)
#define DAT_00e38e70 (*(uint32_t *)0x00e38e70u)
#define DAT_00e38eac (*(uint32_t *)0x00e38eacu)
#define DAT_00e38ec0 (*(uint8_t *)0x00e38ec0u)
#define DAT_00e38ec1 (*(uint8_t *)0x00e38ec1u)
#define DAT_00e38ec3 (*(uint8_t *)0x00e38ec3u)
#define DAT_00e38ec4 (*(uint32_t *)0x00e38ec4u)
#define DAT_00e38ec8 (*(uint32_t *)0x00e38ec8u)
#define DAT_00e38ecc (*(uint32_t *)0x00e38eccu)
#define DAT_00e38ed0 (*(uint32_t *)0x00e38ed0u)
#define DAT_00e38ed8 (*(uint32_t *)0x00e38ed8u)
#define DAT_00e38edc (*(uint32_t *)0x00e38edcu)
#define DAT_00e38ee4 (*(uint32_t *)0x00e38ee4u)
#define DAT_00e38eec (*(uint32_t *)0x00e38eecu)
#define DAT_00e39080 (*(uint32_t *)0x00e39080u)
#define DAT_00e39084 (*(uint32_t *)0x00e39084u)
#define DAT_00e39088 (*(uint8_t *)0x00e39088u)
#define DAT_00e39089 (*(uint8_t *)0x00e39089u)
#define _DAT_005c8fd2 (*(uint16_t *)0x005c8fd2u)
#define _DAT_005c8ff2 (*(uint16_t *)0x005c8ff2u)
#define _DAT_005c9002 (*(uint16_t *)0x005c9002u)
#define _DAT_005c9022 (*(uint16_t *)0x005c9022u)
#define _DAT_005c9032 (*(uint16_t *)0x005c9032u)
#define _DAT_005c9062 (*(uint16_t *)0x005c9062u)
#define _DAT_00e36c6c (*(uint32_t *)0x00e36c6cu)
#define _DAT_00e38ea4 (*(uint32_t *)0x00e38ea4u)
#define _DAT_00e38ed4 (*(uint32_t *)0x00e38ed4u)
#define _DAT_00e38ee0 (*(uint32_t *)0x00e38ee0u)
#define s_Pharaoh_inf_00579274 (*(char (*)[12])0x00579274u)
extern undefined4 FUN_00414a90();
extern void FUN_00417dd0();
extern undefined4 FUN_004ceb10();

undefined4 FUN_004147f0(void)

{
  int iVar1;
  
  DAT_00e38e58 = 0x10;
  DAT_00e38e5c = 1;
  DAT_00e38e60 = 0;
  DAT_00e38e61 = 0;
  _DAT_00e38ed4 = 2;
  DAT_00e38ed8 = 0xf;
  DAT_00e38edc = 0x42;
  _DAT_00e38ee0 = 0;
  DAT_00e38e62 = 0x44;
  DAT_00e38e63 = 1;
  DAT_00e38e64 = 1;
  DAT_00e38e65 = 1;
  DAT_00e38ec0 = 1;
  DAT_00e38ec1 = '\x01';
  DAT_00e38eac = 2;
  DAT_00e38e6c = 0x5a;
  DAT_00e38e70 = 0x46;
  DAT_00e39089 = 1;
  DAT_00e39080 = 2;
  DAT_00e39084 = 1;
  DAT_00e38ec4 = 100;
  DAT_00e38ec8 = 0x50;
  DAT_00e38ecc = 100;
  DAT_00e38ed0 = 100;
  DAT_00e38e68 = 1;
  FUN_00414a90();
  if ((int32_t)DAT_00e38ed8 < 0x18) {
    DAT_00e38e6c = 100;
    DAT_00e38e70 = 100;
    DAT_00e38e68 = 1;
  }
  iVar1 = GetSystemMetrics(0);
  if (iVar1 < 800) {
    DAT_00e38e68 = 1;
  }
  else if (iVar1 < 0x400) {
    DAT_00e38e68 = 2;
  }
  DAT_00e38ee4 = DAT_00e38e68;
  FUN_00417dd0(0);
  _DAT_00e38ea4 = 1;
  FUN_004ceb10(s_Pharaoh_inf_00579274,&DAT_00e38e58);
  DAT_00e39089 = 1;
  FUN_00414a90();
  _DAT_00e36c6c = 1;
  DAT_00e38e60 = 0;
  if (DAT_00e38eac == 2) {
    _DAT_005c9022 = 4;
    _DAT_005c9062 = 4;
  }
  else if (DAT_00e38eac == 1) {
    _DAT_005c9022 = 3;
    _DAT_005c9062 = 3;
  }
  if (DAT_00e38ec1 == '\x01') {
    _DAT_005c9032 = 6;
  }
  _DAT_005c8ff2 = (DAT_00e38eec != 0) + 9;
  _DAT_005c9002 = (DAT_00e38ec3 != '\0') + 4;
  _DAT_005c8fd2 = (DAT_00e39088 != '\0') + 7;
  return 1;
}
