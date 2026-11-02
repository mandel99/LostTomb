#include "event_description.h"
#include "legacy_format.h"
#include "menu_text_glyph.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004cd1a0(uint32_t,int16_t *,uint32_t,int32_t,int32_t);
extern void FUN_004cd240(uint32_t,uint32_t,int16_t *,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_00425730(int32_t,int32_t,int16_t *,int32_t);
extern void FUN_004484b0(void);

/* Whole448640/429 bytes. Twelve28-byte row rectangles and124-byte events,
   signed WORD IDs/coordinates, DWORD origins/scroll/selection/font tokens. */
void restore_editor_event_list_draw(void)
{
    uint32_t rectangle,row,index,record,font,x,y;const char *kind,*description;
    char text[100];
    FUN_00536820((int32_t)U(0xe38da8u),(int32_t)U(0xe38da4u),40,27);
    B(0xe38dc9u)=1;
    FUN_004cd1a0(1,(int16_t *)0x5d9be0u,U(0x7056c8u)==13,0,0);
    FUN_004cd240(44,96,(int16_t *)0x5d9be0u,U(0xf56f8cu),0,4,4);
    FUN_004cd1a0(1,(int16_t *)0x5d9bfcu,U(0x7056c8u)==14,0,0);
    FUN_004cd240(44,97,(int16_t *)0x5d9bfcu,U(0xf56f8cu),0,4,4);
    for(row=1,rectangle=0x5d9a90u;rectangle<=0x5d9bc4u;++row,rectangle+=28u){
        index=U(0x78a620u)+row-1u;
        if((int32_t)index<0||(int32_t)index>=(int32_t)U(0x785d70u))continue;
        record=0x785d74u+index*124u;if(!record)continue;
        font=U(0x78a61cu)==(uint32_t)S16(record)?U(0xf56f9cu):U(0xf56f94u);
        kind=FUN_00448f20(B(record+2u));
        description=FUN_00448f60((const RestoreTownRecord *)(uintptr_t)record);
        FUN_00561a36(text,(const char *)0x5da168u,S16(record),kind,description);
        FUN_004cd1a0(1,(int16_t *)(uintptr_t)rectangle,U(0x7056c8u)==row,0,0);
        y=(uint32_t)S16(rectangle+2u)+U(0xe38da4u)+4u;
        x=(uint32_t)S16(rectangle)+U(0xe38da8u)+4u;
        FUN_004cc770((const uint8_t *)text,(int32_t)x,(int32_t)y,(int32_t)font,0);
    }
    FUN_00425730((int32_t)U(0xe38da8u),(int32_t)U(0xe38da4u),(int16_t *)0x5d9a48u,2);
    FUN_004484b0();
}
void FUN_00448640(void){restore_editor_event_list_draw();}
