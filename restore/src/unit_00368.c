#include "restore_types.h"
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a68 (*(undefined4 *)0x00799a68u)
#define PTR_DAT_005be510 (*(unsigned char * *)0x005be510u)
#define PTR_DAT_005be540 (*(unsigned char * *)0x005be540u)
#define PTR_DAT_005be570 (*(unsigned char * *)0x005be570u)
#define PTR_DAT_005be5a0 (*(unsigned char * *)0x005be5a0u)
#define PTR_DAT_005be5d0 (*(unsigned char * *)0x005be5d0u)
#define PTR_DAT_005be600 (*(unsigned char * *)0x005be600u)
#define PTR_DAT_005be630 (*(unsigned char * *)0x005be630u)
#define PTR_DAT_005be660 (*(unsigned char * *)0x005be660u)
#define PTR_DAT_005be690 (*(unsigned char * *)0x005be690u)
#define PTR_DAT_005be6c0 (*(unsigned char * *)0x005be6c0u)
#define PTR_DAT_005be6f0 (*(unsigned char * *)0x005be6f0u)
#define PTR_DAT_005be720 (*(unsigned char * *)0x005be720u)
#define PTR_DAT_005be750 (*(unsigned char * *)0x005be750u)
#define PTR_DAT_005be780 (*(unsigned char * *)0x005be780u)
#define PTR_DAT_005be7b0 (*(unsigned char * *)0x005be7b0u)
#define PTR_DAT_005be7e0 (*(unsigned char * *)0x005be7e0u)
#define PTR_DAT_005be810 (*(unsigned char * *)0x005be810u)
#define PTR_DAT_005be840 (*(unsigned char * *)0x005be840u)
#define PTR_DAT_005be870 (*(unsigned char * *)0x005be870u)
#define PTR_DAT_005be8a0 (*(unsigned char * *)0x005be8a0u)
#define PTR_DAT_005be8d0 (*(unsigned char * *)0x005be8d0u)
#define PTR_DAT_005be900 (*(unsigned char * *)0x005be900u)
#define PTR_DAT_005be930 (*(unsigned char * *)0x005be930u)
#define PTR_DAT_005be960 (*(unsigned char * *)0x005be960u)
#define PTR_DAT_005be990 (*(unsigned char * *)0x005be990u)
extern void FUN_0041fad0();
extern int FUN_0053e590();

void FUN_00421920(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  undefined **ppuVar3;
  int iVar4;
  
  iVar2 = param_2;
  iVar1 = param_1;
  iVar4 = param_3 * 0x108;
  switch(*(undefined4 *)(&DAT_00799a40 + param_3 * 0x84)) {
  case 0: ;
    ppuVar3 = &PTR_DAT_005be600;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be630);
    return;
  case 1: ;
  case 0x1d: ;
    ppuVar3 = &PTR_DAT_005be510;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be540);
    return;
  case 2: ;
    ppuVar3 = &PTR_DAT_005be540;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be570);
    return;
  case 3: ;
    ppuVar3 = &PTR_DAT_005be570;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be5a0);
    return;
  case 4: ;
    ppuVar3 = &PTR_DAT_005be750;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be780);
    return;
  case 5: ;
    ppuVar3 = &PTR_DAT_005be780;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be7b0);
    return;
  case 6: ;
    ppuVar3 = &PTR_DAT_005be630;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be660);
    return;
  case 7: ;
    ppuVar3 = &PTR_DAT_005be7b0;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be7e0);
    return;
  case 8: ;
    ppuVar3 = &PTR_DAT_005be7e0;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be810);
    return;
  case 9: ;
    ppuVar3 = &PTR_DAT_005be660;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be690);
    return;
  case 10: ;
    ppuVar3 = &PTR_DAT_005be690;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be6c0);
    return;
  case 0xb: ;
    ppuVar3 = &PTR_DAT_005be8a0;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be8d0);
    return;
  case 0xc: ;
    ppuVar3 = &PTR_DAT_005be810;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be840);
    return;
  case 0xd: ;
    ppuVar3 = &PTR_DAT_005be8d0;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be900);
    return;
  case 0xe: ;
    ppuVar3 = &PTR_DAT_005be900;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be930);
    return;
  case 0xf: ;
    ppuVar3 = &PTR_DAT_005be5a0;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be5d0);
    return;
  case 0x10: ;
    ppuVar3 = &PTR_DAT_005be5d0;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be600);
    return;
  case 0x13: ;
    ppuVar3 = &PTR_DAT_005be6c0;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be6f0);
    return;
  case 0x14: ;
    ppuVar3 = &PTR_DAT_005be6f0;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be720);
    return;
  case 0x15: ;
    ppuVar3 = &PTR_DAT_005be720;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be750);
    return;
  case 0x16: ;
    ppuVar3 = &PTR_DAT_005be840;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be870);
    return;
  case 0x17: ;
    ppuVar3 = &PTR_DAT_005be870;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be8a0);
    return;
  case 0x18: ;
    ppuVar3 = &PTR_DAT_005be930;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be960);
    return;
  case 0x19: ;
    ppuVar3 = &PTR_DAT_005be960;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be990);
    return;
  case 0x1a: ;
    ppuVar3 = &PTR_DAT_005be990;
    do {
      if ((*(undefined **)(&DAT_00799a38 + iVar4) == ppuVar3[-2]) &&
         ((FUN_0041fad0(&param_3,&param_1,&param_2,*(undefined4 *)(&DAT_00799a68 + iVar4)), restore_unresolved_return(0x0041fad0u)),
         param_2 != 0)) {
        FUN_0053e590(param_2,param_3 + iVar1,param_1 + iVar2);
      }
      ppuVar3 = ppuVar3 + 4;
    } while ((int)ppuVar3 < 0x5be9c0);
  }
  return;
}
