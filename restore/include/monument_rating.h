#ifndef RESTORE_MONUMENT_RATING_H
#define RESTORE_MONUMENT_RATING_H
#include <stdint.h>
/* Complete 4F78A0, including branches omitted by the original analysis.
   Explicit raw80 storage avoids MSVC's binary64 long double return type. */
void restore_monument_rating_raw80(int32_t selected,uint8_t result[10]);
int32_t restore_monument_rating_int32(int32_t selected);
/* Physical cdecl1/RET0 entry leaves its result in ST0. C code uses the
   explicit functions above; this void declaration prevents a compiler pop. */
void FUN_004f78a0(int32_t selected);
uint8_t FUN_004f53d0(uint32_t building);
void FUN_0046ef50(void);
#endif
