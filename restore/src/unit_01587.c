#include "temple_upgrade.h"
/* Complete50-byte native4C0ED0, including its ten-entry jump table. */
int32_t restore_temple_complex_library_kind(int32_t tool)
{
 uint32_t index=(uint32_t)tool-60u;
 return index<10u?(int32_t)(index<5u?index:index-5u):-1;
}
int32_t FUN_004c0ed0(int32_t tool){return restore_temple_complex_library_kind(tool);}
