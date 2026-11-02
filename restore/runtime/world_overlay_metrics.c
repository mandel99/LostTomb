#include "world_overlay_metrics.h"
#include "world_metric_helpers_model.h"
#include "world_support_model.h"
#include <stdlib.h>

extern void restore_world_detail_dispatch(uint32_t site,uint32_t target,
                                         struct restore_building_world_frame *frame);

static void metric_call(void (*body)(struct restore_building_world_frame *),
                        const uint32_t *args,uint32_t count)
{
    /* Only owned locals, saved registers and callee call slots. These
       providers never consume the granary's inherited food-coordinate frame. */
    uint32_t stack[128],sp=(uint32_t)(uintptr_t)(stack+112),i;
    struct restore_building_world_frame f={{0},2,restore_world_detail_dispatch};
    stack[112]=0;
    for(i=0;i<count;++i)stack[113+i]=args[i];
    f.r[4]=sp;body(&f);
    if(f.r[4]!=sp+4u||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
}

#define METRIC2(token) \
void FUN_##token(int32_t x,int32_t y) \
{const uint32_t a[]={(uint32_t)x,(uint32_t)y};metric_call(restore_building_world_##token,a,2);}
METRIC2(00432320)
METRIC2(00432820)
METRIC2(00435050)
#undef METRIC2
#define METRIC3(token) \
void FUN_##token(int32_t building,int32_t x,int32_t y) \
{const uint32_t a[]={(uint32_t)building,(uint32_t)x,(uint32_t)y};metric_call(restore_building_world_##token,a,3);}
METRIC3(00435290)
METRIC3(004353a0)
METRIC3(004354b0)
METRIC3(00435510)
METRIC3(00435570)
METRIC3(004355d0)
METRIC3(00435630)
METRIC3(004356a0)
METRIC3(00435700)
METRIC3(00435760)
METRIC3(004357c0)
METRIC3(00435820)
METRIC3(00435880)
METRIC3(00435a10)
METRIC3(00435a70)
METRIC3(00435ac0)
METRIC3(00435b20)
METRIC3(00435b80)
METRIC3(00435be0)
METRIC3(00435c40)
METRIC3(00435ca0)
METRIC3(00435d00)
METRIC3(00435d60)
METRIC3(00435de0)
METRIC3(004360a0)
METRIC3(00436140)
#undef METRIC3
#define METRIC4(token) \
void FUN_##token(int32_t level,int32_t x,int32_t y,int32_t palette) \
{const uint32_t a[]={(uint32_t)level,(uint32_t)x,(uint32_t)y,(uint32_t)palette};metric_call(restore_building_world_##token,a,4);}
METRIC4(00436180)
METRIC4(004361a0)
#undef METRIC4
