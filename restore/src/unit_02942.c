#include "crt_startup_arguments.h"
#include "crt_runtime_tail.h"
void *FUN_00561b68(size_t bytes);

void restore_crt_initialize_arguments(void)
{
 uint8_t *source,*command_line;
 uint8_t **arguments;
 uint32_t pointer_count,character_count;
 if(RESTORE_CRT_MB_INITIALIZED==0)FUN_00569bf5();
 (void)GetModuleFileNameA(0,(LPSTR)RESTORE_CRT_MODULE_NAME,0x104u);
 command_line=RESTORE_CRT_COMMAND_LINE;
 RESTORE_CRT_PROGRAM_NAME=RESTORE_CRT_MODULE_NAME;
 source=RESTORE_CRT_MODULE_NAME;
 if(*command_line!=0)source=command_line;
 FUN_00567ae6(source,0,0,&pointer_count,&character_count);
 arguments=(uint8_t **)FUN_00561b68(character_count+pointer_count*4u);
 if(arguments==0)(void)FUN_005630e0(8);
 FUN_00567ae6(source,arguments,(uint8_t *)arguments+pointer_count*4u,
  &pointer_count,&character_count);
 RESTORE_CRT_ARGUMENT_VECTOR=arguments;
 RESTORE_CRT_ARGUMENT_COUNT=pointer_count-1u;
}
void FUN_00567a4d(void){restore_crt_initialize_arguments();}
