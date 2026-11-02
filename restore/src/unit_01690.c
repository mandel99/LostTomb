#include "text_selection.h"
extern int FUN_004cbf30(const char *,uint32_t);
int FUN_004cc6c0(int group,int entry,uint32_t font)
{
    restore_text_select(group,entry,0xe20768u);
    return FUN_004cbf30(*(const char **)0xe20768u,font);
}
