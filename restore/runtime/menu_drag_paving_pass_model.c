#include "surface_passes.h"
#include "map_region_repair.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void restore_C_00475720(struct restore_menu_input_frame *f)
{uint32_t sp=f->r[4];restore_map_refresh_paved_region((int32_t)U(sp+4u),(int32_t)U(sp+8u),(int32_t)U(sp+12u),(int32_t)U(sp+16u),U(sp+20u));f->r[4]=sp+4u;}
