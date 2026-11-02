#include <stdint.h>
#include <string.h>

/* Whole40F8F0: repeat the low argument byte for the complete DWORD byte
   count. The native REP STOSD/STOSB pair has no consumed return value.
   Avoid per-DWORD CONCAT helpers in this heavily used map-buffer fill. */
void FUN_0040f8f0(uint32_t *destination,uint32_t bytes,uint8_t value)
{
    if(bytes!=0)memset(destination,value,bytes);
}
