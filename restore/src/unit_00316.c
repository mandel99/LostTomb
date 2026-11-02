#include "audio_state.h"
#include <stdint.h>
extern uint32_t FUN_00562348(void);

void FUN_0041fca0(void *record,uint16_t state)
{
    uint8_t *object=(uint8_t *)record;
    int32_t kind=*(int32_t *)object,rows,row,choices,choice,index;
    uint8_t *table,*entries,*candidate;
    *(uint16_t *)(object+16)=state;
    if(kind<0||kind>=253)return;
    table=(uint8_t *)(uintptr_t)(0x5be9b8u+(uint32_t)kind*40u);
    *(int32_t *)(object+4)=-1;
    rows=*(int32_t *)(table+28);
    if(rows<=0)return;
    entries=*(uint8_t **)(table+24);
    for(row=0;row<rows;++row)if(*(int32_t *)(entries+(uint32_t)row*40u)<0)break;
    if(row==rows)return;
    candidate=entries+(uint32_t)row*40u+8;
    for(choices=0;choices<4;++choices)if(*(int32_t *)(candidate+choices*8)<0)break;
    if(!choices)return;
    choice=(int32_t)FUN_00562348()%choices;
    candidate+=choice*8;
    index=*(int32_t *)candidate;
    *(int32_t *)(object+4)=index;
    object[21]=candidate[4];
    if(index<0||index>=*(int32_t *)(table+20)) {*(int32_t *)(object+4)=-1;return;}
    *(uint32_t *)(object+8)=object[21]
        ?*(uint32_t *)(*(uint8_t **)(table+16)+(uint32_t)index*48u+12):0;
}
