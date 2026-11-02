#include "nearest_rect.h"
#include <stdlib.h>
#define NR_SENTRY UINT32_C(0x8e47b123)
static void nr_begin(struct restore_menu_input_frame *f,uint32_t stack[32])
{
    unsigned i;
    for(i=0;i<32;++i)stack[i]=NR_SENTRY;
    for(i=0;i<8;++i)f->r[i]=0;
    f->flags=2;f->boundary=0;f->r[4]=(uint32_t)(uintptr_t)(stack+16);
}
static void nr_end(const struct restore_menu_input_frame *f,const uint32_t stack[32])
{
    unsigned i;
    if(f->r[4]!=(uint32_t)(uintptr_t)(stack+17)||stack[16]!=NR_SENTRY)abort();
    for(i=0;i<7;++i)if(stack[i]!=NR_SENTRY)abort();
    for(i=19;i<32;++i)if(stack[i]!=NR_SENTRY)abort();
}
int32_t FUN_00407870(void)
{
    uint32_t stack[32];struct restore_menu_input_frame f;
    nr_begin(&f,stack);restore_C_00407870(&f);nr_end(&f,stack);
    return (int32_t)f.r[0];
}
void FUN_00407a10(int32_t *out_record,int32_t *out_rectangle)
{
    uint32_t stack[32];struct restore_menu_input_frame f;
    nr_begin(&f,stack);stack[17]=(uint32_t)(uintptr_t)out_record;stack[18]=(uint32_t)(uintptr_t)out_rectangle;
    restore_C_00407a10(&f);nr_end(&f,stack);
}
