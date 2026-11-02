#include "map_cells_phase.h"
/* Whole 00487300. Growth is byte-addressed; native zero-extends full EAX. */
uint32_t FUN_00487300(int32_t tile) {return mc_growth((uint32_t)tile);}
