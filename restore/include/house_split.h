#ifndef RESTORE_HOUSE_SPLIT_H
#define RESTORE_HOUSE_SPLIT_H
#include <stdint.h>
/* Native BE90/C1C0/C500/CB30/CCF0: one building DWORD, RET0, void. */
void restore_house_subdivide(uint32_t native_target,uint32_t building);
#endif
