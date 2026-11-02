#ifndef RESTORE_MONUMENT_FOOTPRINT_H
#define RESTORE_MONUMENT_FOOTPRINT_H
#include <stdint.h>
/* Explicit receiver replaces the lost native ECX. These public C facades
   are cdecl; native frame graphs keep their original RET4 contract. */
void restore_monument_commit_footprint(uint32_t self,int32_t building);
void FUN_005468d0(uint32_t self,int32_t building);
void restore_monument_refresh_all_tiles(void);
void FUN_00547a60(void);
void restore_monument_consume_resource(void);
void FUN_00492fc0(void);
static __inline uint32_t restore_monument_receiver(uint16_t kind)
{
 switch(kind){case 229:return 0x6f9320u;case 234:return 0x6f5648u;
 case 235:return 0x6de9e0u;case 236:return 0x6e9188u;default:return 0;}
}
#endif
