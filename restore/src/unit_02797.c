#include "restore_types.h"
#define LAB_005641bf (*(undefined1 *)0x005641bfu)
#define LAB_0056420d (*(undefined1 *)0x0056420du)
#define PTR_FUN_00604c30 (*(unsigned char * *)0x00604c30u)
#define PTR_FUN_00604c34 (*(unsigned char * *)0x00604c34u)
#define PTR_FUN_00604c38 (*(unsigned char * *)0x00604c38u)
#define PTR_FUN_00604c3c (*(unsigned char * *)0x00604c3cu)
#define PTR_FUN_00604c40 (*(unsigned char * *)0x00604c40u)
#define PTR_FUN_00604c44 (*(unsigned char * *)0x00604c44u)
extern void FUN_00564165();
extern void FUN_00564225();
extern errno_t FUN_0056452a();

void FUN_005618ae(void)

{
  PTR_FUN_00604c34 = &LAB_005641bf;
  PTR_FUN_00604c30 = FUN_0056452a;
  PTR_FUN_00604c38 = FUN_00564225;
  PTR_FUN_00604c3c = FUN_00564165;
  PTR_FUN_00604c40 = &LAB_0056420d;
  PTR_FUN_00604c44 = FUN_0056452a;
  return;
}
