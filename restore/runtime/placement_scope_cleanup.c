#include "placement_scope_cleanup.h"
#include "array_lifecycle.h"
void FUN_0051f780(void);
void FUN_00561a2b(void *allocation);
static void destroy_empty_element(void *object){(void)object;FUN_0051f780();}
static void destroy_array(uint32_t frame,uint32_t offset,uint32_t pieces,int32_t count)
{
 uint32_t object=*(volatile uint32_t *)(uintptr_t)(frame+offset);
 restore_array_destroy((void *)(uintptr_t)(object+pieces),52u,count,destroy_empty_element);
}

void restore_placement_scope_0056b3e8(uint32_t frame)
{
 destroy_array(frame,0xffffffbcu,0x4cu,16);
}

void restore_placement_scope_0056b4b8(uint32_t frame)
{
 destroy_array(frame,0xffffffbcu,0x4cu,25);
}

void restore_placement_scope_0056b5c8(uint32_t frame)
{
 destroy_array(frame,0xffffffbcu,0x4cu,9);
}

void restore_placement_scope_0056b6d8(uint32_t frame)
{
 destroy_array(frame,0xffffffbcu,0x4cu,2);
}

void restore_placement_scope_0056b818(uint32_t frame)
{
 destroy_array(frame,0xffffffbcu,0x1e5cu,167);
}

void restore_placement_scope_0056bda8(uint32_t frame)
{
 destroy_array(frame,0xffffffbcu,0x5c88u,513);
}

void restore_placement_scope_0056ce08(uint32_t frame)
{
 destroy_array(frame,0xffffffbcu,0x4ee0u,437);
}

void restore_placement_scope_0056dc08(uint32_t frame)
{
 destroy_array(frame,0xffffffbcu,0x1cbcu,158);
}

void restore_placement_scope_0056b6b0(uint32_t frame)
{
 FUN_00561a2b((void *)(uintptr_t)*(volatile uint32_t *)(uintptr_t)(frame+0x4u));
}

void restore_placement_scope_0056e1aa(uint32_t frame)
{
 FUN_00561a2b((void *)(uintptr_t)*(volatile uint32_t *)(uintptr_t)(frame+0x8u));
}
