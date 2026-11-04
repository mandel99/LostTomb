#include "canal_region_model.h"
#include "map_region_repair.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void restore_C_0047bae0(struct restore_menu_input_frame *f)
{uint32_t sp=f->r[4];restore_map_refresh_channel_region((int32_t)U(sp+4u),(int32_t)U(sp+8u),(int32_t)U(sp+12u),(int32_t)U(sp+16u),U(sp+20u));f->r[4]=sp+4u;}
