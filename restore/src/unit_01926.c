#include "briefing_draw.h"
#include "menu_record_queries.h"
#include "menu_text_glyph.h"
#include "sprite_render.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
static int32_t add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}

/* 4E2720: signed WORD coordinates and120-byte queued records. The row,
 * sprite and font providers consume full DWORD identifiers. */
void FUN_004e2720(void)
{
    int32_t offset=0,y,x,width,style;
    uint32_t record,counter;
    if(B(0xe38ddau)!=1&&B(0xe38ddau)!=18)FUN_004e2ab0();
    if(B(0xe38e60u))offset=70;
    if(B(0xe38ddau)==1||B(0xe38ddau)==18||B(0xe38ddau)==6){
        if(FUN_004e2530()){
            y=add(H(0xe94e2cu),offset);
            FUN_00537000(9,sub(H(0xe94e2au),229),y,29,1,1);
            FUN_0053e590((int32_t)(U(0xf5684cu)+16u),sub(H(0xe94e2au),228),add(y,2));
            FUN_0053e590((int32_t)(U(0xf5684cu)+16u),add(H(0xe94e2au),200),add(y,2));
            FUN_004cc720((const uint8_t *)(uintptr_t)U(0xe20768u),sub(H(0xe94e2au),229),
                add(y,4),460,(int32_t)U(0xf56f9cu),0);
            offset=add(offset,25);
        }
    }
    U(0xe950d8u)=0;
    record=0xe94e32u;counter=0xf53f34u;
    for(style=9;style<14;++style,record+=120u,counter+=4u){
        if(!H(record-4u))continue;
        ++U(0xe950d8u);
        y=add(H(record-6u),offset);width=H(record);x=H(record-8u);
        FUN_00537000(style,add(sub(x,width/2),1),y,width/16+1,0,1);
        if((int32_t)U(counter)>40){
            if(H(record)<460){
                FUN_0053e590((int32_t)(U(0xf5684cu)+15u),add(sub(x,H(record)/2),2),add(y,2));
                FUN_0053e590((int32_t)(U(0xf5684cu)+15u),add(add(H(record)/2,-30),x),add(y,2));
            }
            FUN_004cc720((const uint8_t *)(uintptr_t)(record+8u),add(sub(x,H(record)/2),1),
                add(y,4),H(record),(int32_t)U(0xf56f9cu),0);
        }
        if(FUN_00536fd0(style)){
            B(0xe38dc8u)=1;U(counter)+=4u;
        }
        if((int32_t)U(0xe92e0cu)>(int32_t)(U(record+2u)+15000u))H(record-4u)=0;
    }
}
