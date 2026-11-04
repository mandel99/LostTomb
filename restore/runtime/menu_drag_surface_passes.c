#include "map_surface_repair.h"
#include "map_region_repair.h"
void FUN_00475720(int32_t left,int32_t top,int32_t right,int32_t bottom,uint32_t clear)
{restore_map_refresh_paved_region(left,top,right,bottom,clear);}
void FUN_00476530(void){restore_map_refresh_garden_tiles();}
void FUN_0047b1a0(void){restore_map_refresh_canal_tiles();}
