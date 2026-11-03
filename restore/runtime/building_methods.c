#include <stdlib.h>
/* Native frame adapters delegate to the semantic C providers. */
#include "building_methods.h"
#include "monument_map.h"
#include "monument_create.h"
#include "sun_temple.h"
#include "monument_complex_create.h"
#include "monument_footprint.h"

#define MI_U32(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

void restore_building_method_C_004015a0(struct restore_menu_input_frame *f)
{restore_monument_create_frame(0x4015a0u,f);}

void restore_building_method_C_004017a0(struct restore_menu_input_frame *f)
{restore_monument_map_frame(0x004017a0u,f);}

void restore_building_method_C_0040da90(struct restore_menu_input_frame *f)
{restore_monument_create_frame(0x0040da90u,f);}

void restore_building_method_C_0040dca0(struct restore_menu_input_frame *f)
{restore_monument_map_frame(0x0040dca0u,f);}

void restore_building_method_C_00417180(struct restore_menu_input_frame *f)
{restore_monument_create_frame(0x00417180u,f);}

void restore_building_method_C_004e6840(struct restore_menu_input_frame *f)
{restore_monument_create_frame(0x004e6840u,f);}

void restore_building_method_C_004e69c0(struct restore_menu_input_frame *f)
{restore_monument_map_frame(0x004e69c0u,f);}

void restore_building_method_C_005417b0(struct restore_menu_input_frame *f)
{restore_sun_temple_frame(0x5417b0u,f);}

void restore_building_method_C_00541920(struct restore_menu_input_frame *f)
{restore_sun_temple_frame(0x541920u,f);}

void restore_building_method_C_00545c30(struct restore_menu_input_frame *f)
{restore_complex_create_frame(0x00545c30u,f);}

void restore_building_method_C_005468d0(struct restore_menu_input_frame *f)
{restore_monument_commit_footprint(f->r[1],(int32_t)MI_U32(f->r[4]+4u));f->r[4]+=8u;}

void restore_building_method_C_0054fba0(struct restore_menu_input_frame *f)
{restore_complex_create_frame(0x0054fba0u,f);}

void restore_building_method_C_00556480(struct restore_menu_input_frame *f)
{restore_complex_create_frame(0x00556480u,f);}

void restore_building_method_C_00559130(struct restore_menu_input_frame *f)
{restore_complex_create_frame(0x00559130u,f);}

int restore_building_method_has(uint32_t target)
{switch(target){case 0x004015a0u:case 0x004017a0u:case 0x0040da90u:case 0x0040dca0u:case 0x00417180u:case 0x004e6840u:case 0x004e69c0u:case 0x005417b0u:case 0x00541920u:case 0x00545c30u:case 0x005468d0u:case 0x0054fba0u:case 0x00556480u:case 0x00559130u:return 1;default:return 0;}}

void restore_building_method_dispatch(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{(void)site;switch(target){
case 0x004015a0u:restore_building_method_C_004015a0(f);return;
case 0x004017a0u:restore_building_method_C_004017a0(f);return;
case 0x0040da90u:restore_building_method_C_0040da90(f);return;
case 0x0040dca0u:restore_building_method_C_0040dca0(f);return;
case 0x00417180u:restore_building_method_C_00417180(f);return;
case 0x004e6840u:restore_building_method_C_004e6840(f);return;
case 0x004e69c0u:restore_building_method_C_004e69c0(f);return;
case 0x005417b0u:restore_building_method_C_005417b0(f);return;
case 0x00541920u:restore_building_method_C_00541920(f);return;
case 0x00545c30u:restore_building_method_C_00545c30(f);return;
case 0x005468d0u:restore_building_method_C_005468d0(f);return;
case 0x0054fba0u:restore_building_method_C_0054fba0(f);return;
case 0x00556480u:restore_building_method_C_00556480(f);return;
case 0x00559130u:restore_building_method_C_00559130(f);return;
default:abort();}}
