#ifndef RESTORE_CONSTRUCTION_TILES_H
#define RESTORE_CONSTRUCTION_TILES_H
#include <stdint.h>
/* Native cdecl RET0; cell and kind DWORD, only the low WORD of building is read. */
void FUN_00440060(uint32_t cell,int32_t kind,uint16_t building);
#endif
