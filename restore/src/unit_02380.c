#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00ea4508 (*(undefined4 *)0x00ea4508u)
#define DAT_00ea450c (*(undefined4 *)0x00ea450cu)
#define DAT_00ea5cbc (*(undefined4 *)0x00ea5cbcu)
#define DAT_00ea5cc0 (*(undefined4 *)0x00ea5cc0u)
#define DAT_00ea5ff4 (*(undefined4 *)0x00ea5ff4u)
#define DAT_00ea620c (*(undefined4 *)0x00ea620cu)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fbc (*(undefined *)0x00f56fbcu)
#define PTR_DAT_005e03ec (*(unsigned char * *)0x005e03ecu)
extern int FUN_0044d500();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();
extern void FUN_004d6860();

void FUN_00524840(void)

{
  int iVar1;
  bool bVar2;
  
  if (5 < DAT_00f53f48) {
    FUN_004d65b0(DAT_00f53f0c + 0x40,DAT_00f53f08 + 0x144,0x20,6,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x20,(&DAT_00ea5cbc)[DAT_00eaac28 * 0x1276],DAT_00f53f0c + 0x48,
                 DAT_00f53f08 + 0x152,DAT_00f56fbc,0,DAT_00e38da8,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d62f0(0x34,1,DAT_00f53f0c + 0x48,DAT_00f53f08 + 0x174,DAT_00f56f9c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276],0x40,&PTR_DAT_005e03ec,
                 DAT_00e20778 + 0x50 + DAT_00f53f0c,DAT_00f53f08 + 0x174,DAT_00f56f9c,0,DAT_00e38da8
                 ,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d6860(*(undefined4 *)(&DAT_00ea5ff4 + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048,
                 DAT_00f53f0c + 0xfa,DAT_00f53f08 + 0x174,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x34,0x4f,DAT_00e20778 + 0xfa + DAT_00f53f0c,DAT_00f53f08 + 0x174,DAT_00f56f9c,0,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d63d0(0,DAT_00f53f0c + 0x140,DAT_00f53f08 + 0x160,0xfa,0x14,DAT_007056c8 == 1,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6250(0x34,2,DAT_00f53f0c + 0x140,DAT_00f53f08 + 0x165,0xfa,DAT_00f56f9c,0,DAT_00e38da8,
                 DAT_00e38da4);
    if (*(int *)(&DAT_00ea620c + DAT_00eaac28 * 0x49d8) == 0) {
      FUN_004d62f0(0x34,0x4e,DAT_00f53f0c + 100,DAT_00f53f08 + 0x18e,DAT_00f56f9c,0,DAT_00e38da8,
                   DAT_00e38da4);
    }
    else {
      FUN_004d63d0(0,DAT_00f53f0c + 0x46,DAT_00f53f08 + 0x189,500,0x14,DAT_007056c8 == 2,
                   DAT_00e38da8,DAT_00e38da4);
      DAT_00e20778 = 0;
      FUN_004d62f0(0x34,(&DAT_00ea450c)[DAT_00eaac28 * 0x1276] + 4,DAT_00f53f0c + 0x78,
                   DAT_00f53f08 + 0x18e,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004d6860((&DAT_00ea4508)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,
                   DAT_00e20778 + 0x78 + DAT_00f53f0c,DAT_00f53f08 + 0x18e,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      FUN_004d62f0(0x34,3,DAT_00e20778 + 0x78 + DAT_00f53f0c,DAT_00f53f08 + 0x18e,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    FUN_004d63d0(0,DAT_00f53f0c + 0x140,DAT_00f53f08 + 0x14a,0xfa,0x14,DAT_007056c8 == 3,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6250(0x34,0x31,DAT_00f53f0c + 0x140,DAT_00f53f08 + 0x14f,0xfa,DAT_00f56f9c,0,
                 DAT_00e38da8,DAT_00e38da4);
    bVar2 = DAT_007056c8 == 4;
    iVar1 = FUN_0044d500(0);
    if (iVar1 != 0) {
      FUN_004d63d0(0,DAT_00f53f0c + 0x26,DAT_00f53f08 + 0x60,0x230,0x28,bVar2,DAT_00e38da8,
                   DAT_00e38da4);
    }
    bVar2 = DAT_007056c8 == 5;
    iVar1 = FUN_0044d500(1);
    if (iVar1 != 0) {
      FUN_004d63d0(0,DAT_00f53f0c + 0x26,DAT_00f53f08 + 0x8a,0x230,0x28,bVar2,DAT_00e38da8,
                   DAT_00e38da4);
    }
    bVar2 = DAT_007056c8 == 6;
    iVar1 = FUN_0044d500(2);
    if (iVar1 != 0) {
      FUN_004d63d0(0,DAT_00f53f0c + 0x26,DAT_00f53f08 + 0xb4,0x230,0x28,bVar2,DAT_00e38da8,
                   DAT_00e38da4);
    }
    bVar2 = DAT_007056c8 == 7;
    iVar1 = FUN_0044d500(3);
    if (iVar1 != 0) {
      FUN_004d63d0(0,DAT_00f53f0c + 0x26,DAT_00f53f08 + 0xde,0x230,0x28,bVar2,DAT_00e38da8,
                   DAT_00e38da4);
    }
    bVar2 = DAT_007056c8 == 8;
    iVar1 = FUN_0044d500(4);
    if (iVar1 != 0) {
      FUN_004d63d0(0,DAT_00f53f0c + 0x26,DAT_00f53f08 + 0x108,0x230,0x28,bVar2,DAT_00e38da8,
                   DAT_00e38da4);
    }
  }
  return;
}
