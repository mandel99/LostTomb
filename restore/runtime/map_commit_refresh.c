#include "map_commit_refresh.h"
#include "map_commit_fields.h"
#include <stdint.h>
extern void FUN_00475dc0(void);
#include "terrain_redraw.h"
extern void FUN_00477500(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004777c0(int32_t,int32_t,int32_t,int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(a))
extern void FUN_0051f780(void); /* Complete native function is one RET. */
void FUN_004795e0(void)
{
 int32_t last_x,last_y;
 FUN_00479670();FUN_00479670();FUN_00479670();FUN_00479670();
 last_y=(int32_t)(U(0xbca2d4u)-1u);last_x=(int32_t)(U(0xbca2d0u)-1u);
 /* The four original arguments are ignored by this genuine empty callee. */
 (void)last_y;(void)last_x;FUN_0051f780();
 FUN_00475dc0();
 last_y=(int32_t)(U(0xbca2d4u)-1u);last_x=(int32_t)(U(0xbca2d0u)-1u);
 FUN_004767d0(0,0,last_x,last_y,0,0);
 last_y=(int32_t)(U(0xbca2d4u)-1u);last_x=(int32_t)(U(0xbca2d0u)-1u);
 FUN_00477500(0,0,last_x,last_y);
 last_y=(int32_t)(U(0xbca2d4u)-1u);last_x=(int32_t)(U(0xbca2d0u)-1u);
 FUN_004777c0(0,0,last_x,last_y);
}
