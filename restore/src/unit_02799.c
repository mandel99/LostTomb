#include "crt_runtime_init.h"
int32_t restore_crt_register_exit_callback(uint32_t callback)
{return restore_crt_append_exit_callback(callback)!=0?0:-1;}
int32_t FUN_00561953(uint32_t callback)
{return restore_crt_register_exit_callback(callback);}
