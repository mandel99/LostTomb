#include "actor_panel_native_context.h"
#include "actor_panel_actions.h"

typedef char actor_panel_context_pointer32[sizeof(void *)==4?1:-1];

void restore_actor_panel_with_native_outslot(uint32_t native_entry_minus4)
{
    uint32_t stack[512],registers[8]={0},flags=2;
    /* Only the caller-proved output slot is seeded. 521AA0 writes the
       other four locals; any later successful44D520 replaces this slot.
       Preserve it between all five queries, including misses. */
    stack[383]=native_entry_minus4;
    stack[384]=0; /* Synthetic return slot; the model only pops it. */
    registers[4]=(uint32_t)(uintptr_t)(stack+384);
    restore_actor_panel_raw(0x0040a450u,registers,&flags);
}

static __declspec(thread) RestoreActorPanelNativeScope native_scope;
RestoreActorPanelNativeScope restore_actor_panel_native_enter(void)
{
    RestoreActorPanelNativeScope previous=native_scope;
    native_scope.valid=0;native_scope.hover=0;native_scope.buttons=0;
    return previous;
}
void restore_actor_panel_native_leave(RestoreActorPanelNativeScope previous)
{native_scope=previous;}
void restore_actor_panel_native_small(uint32_t site,uint32_t table,int32_t count)
{
    /* Native425990->425B80 PUSH ESI@425B8C, before any callback.
       In both main paths this is the later40A450 entrySP-4 word. */
    if(site==0x4dbfd4u&&table==0x5c7b80u&&count==1){
        native_scope.word=table;native_scope.valid=1;
    }
}
static int actor_table(uint32_t table,uint32_t count)
{return count==20u&&(table==0x5c4cd0u||table==0x5c4f00u);}
uint32_t restore_actor_panel_native_hover_begin(uint32_t site,uint32_t table,uint32_t count)
{
    uint32_t previous=native_scope.hover;
    native_scope.hover=native_scope.valid&&actor_table(table,count)&&
                        (site==0x4dc0f9u||site==0x4dc153u);
    return previous;
}
void restore_actor_panel_native_hover_end(uint32_t previous)
{native_scope.hover=previous;}
void restore_actor_panel_native_highlight(uint32_t x)
{
    /* Called at the actual draw branch, including its node/hit/mode gates.
       Native PUSH ECX@4D6850 writes this exact x;536C30 leaves arg2 intact. */
    if(native_scope.hover)native_scope.word=x;
}
uint32_t restore_actor_panel_native_buttons_begin(uint32_t site,uint32_t table,uint32_t count)
{
    uint32_t previous=native_scope.buttons;
    native_scope.buttons=native_scope.valid&&actor_table(table,count)&&
                          (site==0x4dcc9au||site==0x4dc171u);
    return previous;
}
void restore_actor_panel_native_buttons_end(uint32_t previous)
{native_scope.buttons=previous;}
int restore_actor_panel_native_action(uint32_t token)
{
    if(token!=0x40a450u&&token!=0x40a550u)return 0;
    if(!native_scope.valid||!native_scope.buttons)return 0;
    restore_actor_panel_with_native_outslot(native_scope.word);
    return 1;
}
