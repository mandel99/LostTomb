#include "event_description.h"
#include "text_selection.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Whole448F20: invalid kinds leave the current text selection untouched. */
const char *restore_event_kind_label(int32_t kind)
{
    uint32_t saved,result;
    if(kind<0||kind>=30)return (const char *)0x7027d4u;
    saved=U(0xe20768u);FUN_004cc110(156,kind);
    result=U(0xe20768u);U(0xe20768u)=saved;
    return (const char *)(uintptr_t)result;
}
const char *FUN_00448f20(int32_t kind){return restore_event_kind_label(kind);}
