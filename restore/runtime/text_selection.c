#include "text_selection.h"
#define U(address) (*(uint32_t *)(uintptr_t)(address))
void restore_text_select(int group,int entry,uintptr_t output_slot)
{
    uint8_t *text=(uint8_t *)(uintptr_t)(U(0xe36c70)+U(0xe34c38u+(uint32_t)group*8u));
    U(output_slot)=(uint32_t)(uintptr_t)text;
    while(entry>0) {
        if(!*text&&(text[-1]>=32||text[-1]==0))--entry;
        ++text;U(output_slot)=(uint32_t)(uintptr_t)text;
    }
    while(*text<32){++text;U(output_slot)=(uint32_t)(uintptr_t)text;}
}
