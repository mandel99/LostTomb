#include "map_surface_repair.h"
void restore_C_00476530(struct restore_menu_input_frame *f)
{uint32_t sp=f->r[4];restore_map_refresh_garden_tiles();f->r[4]=sp+4u;}
