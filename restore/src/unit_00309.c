#include "restore_types.h"
#include "file_list.h"
#define DAT_0057626c (*(undefined4 *)0x0057626cu)
#define DAT_00704db4 (*(int32_t *)0x00704db4u)
#define DAT_00704db8 (*(int32_t *)0x00704db8u)
#define DAT_00704dbc (*(int32_t *)0x00704dbcu)
#define DAT_00d3b198 (*(int32_t *)0x00d3b198u)
#define DAT_00e37078 (*(undefined4 *)0x00e37078u)
#define DAT_00e38aec (*(char *)0x00e38aecu)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define PTR_DAT_00576274 (*(unsigned char * *)0x00576274u)
extern void FUN_0040f8f0();


extern void FUN_004cbea0();
extern void FUN_004ce3a0();
extern void FUN_004ce3c0();
extern undefined4 FUN_004d5820();
extern undefined4 FUN_004d58b0();

void FUN_0041f670(void)

{
  char cVar1;
  undefined1 uVar2;
  char *pcVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  char *pcVar7;
  char *pcVar8;
  char local_64 [100];
  
  if (DAT_00d3b198 < DAT_00704db4) {
    DAT_00704dbc = DAT_00d3b198 + DAT_00704db8;
    pcVar3 = (char *)FUN_0041e010(RESTORE_MISSION_FILES,DAT_00704dbc);
    if (pcVar3 != (char *)0x0) {
      FUN_0040f8f0(&DAT_00e37078,0x40,0);
      uVar5 = 0xffffffff;
      pcVar7 = pcVar3;
      do {
        pcVar8 = pcVar7;
        if (uVar5 == 0) break;
        uVar5 = uVar5 - 1;
        pcVar8 = pcVar7 + 1;
        cVar1 = *pcVar7;
        pcVar7 = pcVar8;
      } while (cVar1 != '\0');
      uVar5 = ~uVar5;
      pcVar7 = pcVar8 + -uVar5;
      pcVar8 = (char *)&DAT_00e37078;
      for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
        *(undefined4 *)pcVar8 = *(undefined4 *)pcVar7;
        pcVar7 = pcVar7 + 4;
        pcVar8 = pcVar8 + 4;
      }
      for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
        *pcVar8 = *pcVar7;
        pcVar7 = pcVar7 + 1;
        pcVar8 = pcVar8 + 1;
      }
      FUN_004ce3a0(&DAT_00e37078);
      iVar4 = FUN_0041dfe0(RESTORE_MISSION_FILES,DAT_00704dbc);
      if (iVar4 == 0) {
        FUN_004cbea0(&PTR_DAT_00576274,&DAT_00e38aec,3);
        FUN_004ce3c0(&DAT_00e37078);
        uVar5 = 0xffffffff;
        pcVar3 = (char *)&DAT_0057626c;
        do {
          pcVar7 = pcVar3;
          if (uVar5 == 0) break;
          uVar5 = uVar5 - 1;
          pcVar7 = pcVar3 + 1;
          cVar1 = *pcVar3;
          pcVar3 = pcVar7;
        } while (cVar1 != '\0');
        uVar5 = ~uVar5;
        pcVar3 = pcVar7 + -uVar5;
        pcVar7 = local_64;
        for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
          *(undefined4 *)pcVar7 = *(undefined4 *)pcVar3;
          pcVar3 = pcVar3 + 4;
          pcVar7 = pcVar7 + 4;
        }
        for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
          *pcVar7 = *pcVar3;
          pcVar3 = pcVar3 + 1;
          pcVar7 = pcVar7 + 1;
        }
        uVar5 = 0xffffffff;
        pcVar3 = (char *)&DAT_00e37078;
        do {
          pcVar7 = pcVar3;
          if (uVar5 == 0) break;
          uVar5 = uVar5 - 1;
          pcVar7 = pcVar3 + 1;
          cVar1 = *pcVar3;
          pcVar3 = pcVar7;
        } while (cVar1 != '\0');
        uVar5 = ~uVar5;
        iVar4 = -1;
        pcVar3 = local_64;
        do {
          pcVar8 = pcVar3;
          if (iVar4 == 0) break;
          iVar4 = iVar4 + -1;
          pcVar8 = pcVar3 + 1;
          cVar1 = *pcVar3;
          pcVar3 = pcVar8;
        } while (cVar1 != '\0');
        pcVar3 = pcVar7 + -uVar5;
        pcVar7 = pcVar8 + -1;
        for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
          *(undefined4 *)pcVar7 = *(undefined4 *)pcVar3;
          pcVar3 = pcVar3 + 4;
          pcVar7 = pcVar7 + 4;
        }
        for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
          *pcVar7 = *pcVar3;
          pcVar3 = pcVar3 + 1;
          pcVar7 = pcVar7 + 1;
        }
        FUN_004d5820(local_64,1);
        FUN_004ce3a0(&DAT_00e37078);
        DAT_00e38dca = 1;
      }
      else if ((0 < iVar4) && (iVar4 < 3)) {
        uVar2 = FUN_0041dfc0(RESTORE_MISSION_FILES,DAT_00704dbc);
        DAT_00e38ea8 = uVar2;
        FUN_004d58b0(0,1);
        uVar5 = 0xffffffff;
        do {
          pcVar7 = pcVar3;
          if (uVar5 == 0) break;
          uVar5 = uVar5 - 1;
          pcVar7 = pcVar3 + 1;
          cVar1 = *pcVar3;
          pcVar3 = pcVar7;
        } while (cVar1 != '\0');
        uVar5 = ~uVar5;
        pcVar3 = pcVar7 + -uVar5;
        pcVar7 = (char *)&DAT_00e37078;
        DAT_00e38ea8 = uVar2;
        for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
          *(undefined4 *)pcVar7 = *(undefined4 *)pcVar3;
          pcVar3 = pcVar3 + 4;
          pcVar7 = pcVar7 + 4;
        }
        for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
          *pcVar7 = *pcVar3;
          pcVar3 = pcVar3 + 1;
          pcVar7 = pcVar7 + 1;
        }
        FUN_004ce3a0(&DAT_00e37078);
        DAT_00e38dca = 1;
        return;
      }
    }
  }
  return;
}
