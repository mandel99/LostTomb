#include "restore_types.h"
#include "file_list.h"
#define DAT_00704dbc (*(int32_t *)0x00704dbcu)


char FUN_0041e170(void)

{
  int iVar1;
  
  iVar1 = FUN_0041dfe0(RESTORE_MISSION_FILES,DAT_00704dbc);
  return (iVar1 != 2) + '\x03';
}
