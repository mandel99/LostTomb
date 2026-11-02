#include "editor_city_detail_actions.h"
#include "editor_permissions_actions.h"
#include "selection_popup.h"
#include "numeric_input.h"
#include "map_validation.h"
#include "record_place.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
static uint32_t selected_city(void){return 0x77f748u+U(0x77591cu)*98u;}

void restore_editor_city_detail_004082d0(void)
{
    uint32_t city=selected_city(),y;
    if(!B(city+1u))return;
    y=U(0xe38dacu)-314u;
    B(city+51u)=10;
    U(0x5d2a2cu)=0;
    FUN_00403c00(443,y,7,39,0,city+24u,0,0,0,1,0,0,0,-1,0);
    B(0xe38dcau)=1;
}
void restore_editor_city_detail_00408340(void)
{
    uint32_t city=selected_city(),y;uint8_t alternate;
    if(!B(city+1u))return;
    alternate=B(0xe39088u);
    B(city+51u)=10;
    U(0x5d2a2cu)=0;
    y=U(0xe38dacu)-534u;
    FUN_00403c00(20,y,66,alternate?21u:195u,0,city+25u,0,0,0,1,0,0,0,-1,0);
    B(0xe38dcau)=1;
}
void restore_editor_city_detail_004083f0(void)
{
    uint32_t city=selected_city();uint8_t value;
    if(!B(city+1u))return;
    B(city+51u)=10;
    value=(uint8_t)(B(city+19u)+1u);
    U(0x5d2a2cu)=0;
    B(city+19u)=value;
    if(value>=4u)B(city+19u)=0;
    B(0xe38dcau)=1;
}
void restore_editor_city_detail_00408440(void)
{
    uint32_t city=selected_city(),y;
    if(!B(city+1u))return;
    y=U(0xe38dacu)-534u;
    B(city+51u)=10;
    U(0x5d2a2cu)=0;
    FUN_00403c00(373,y,17,196,0,city+25u,0,0,0,1,0,0,0,-1,0);
    B(0xe38dcau)=1;
}
void restore_editor_city_detail_004084e0(void)
{
    uint32_t city=selected_city(),y;
    if(!B(city+1u))return;
    y=U(0xe38dacu)-314u;
    B(city+51u)=10;
    U(0x5d2a2cu)=0;
    /*4084B0 is already the genuine numeric_completion provider and resolver. */
    restore_numeric_open_byte_004038e0(453,y,city+26u,0x4084b0u);
    B(0xe38dcau)=1;
}

/* Complete444A80,62 instructions. Existing queries retain their actual
   fullDWORD returns. Only the single shared typed setter mutates permissions. */
void restore_editor_city_detail_00444a80(void)
{
    uint32_t record=0,city=0x77f748u,kind,total;
    do {
        if(B(city+1u)!=0&&B(city)==1){
            total=0;
            for(kind=1;kind<36u;++kind)total+=FUN_00444a00(record,kind);
            if(total==0)
                for(kind=1;kind<36u;++kind)
                    restore_editor_permission_set_00444a40(record,kind,2);
            for(kind=1;kind<36u;++kind)
                if(FUN_00443000(record,kind)==0&&FUN_00443040(record,kind)==0)
                    restore_editor_permission_set_00444a40(record,kind,0);
        }
        city+=98u;++record;
    }while(city<0x7843d8u);
}
/* Complete408540,41 instructions. Native deliberately publishes EVERY
   eligible resource;16 is only the minimum fill length, not an output clamp. */
void restore_editor_city_detail_00408540(void)
{
    uint32_t selected,kind,count=0,output=0x705650u;
    B(0xe38ddau)=0x24;
    restore_editor_city_detail_00444a80();
    selected=U(0x77591cu);
    for(kind=1;kind<36u;++kind){
        if(FUN_00443000(selected,kind)!=0||FUN_00443040(selected,kind)!=0){
            U(output)=kind;output+=4u;++count;
        }
    }
    for(;count<16u;++count,output+=4u)U(output)=0xffffffffu;
    B(0xe38dcau)=1;
}
void restore_editor_city_detail_004085b0(void)
{
    uint32_t city=selected_city(),y;
    if(!B(city+1u))return;
    y=U(0xe38dacu)-314u;
    B(city+51u)=10;
    U(0x5d2a2cu)=0;
    FUN_00403790(653,y,2,0,city+28u,0,0);
    B(0xe38dcau)=1;
}
int restore_editor_city_detail_action(uint32_t token)
{
    switch(token){
    case 0x4082d0u:restore_editor_city_detail_004082d0();break;
    case 0x408340u:restore_editor_city_detail_00408340();break;
    case 0x4083f0u:restore_editor_city_detail_004083f0();break;
    case 0x408440u:restore_editor_city_detail_00408440();break;
    case 0x4084e0u:restore_editor_city_detail_004084e0();break;
    case 0x408540u:restore_editor_city_detail_00408540();break;
    case 0x4085b0u:restore_editor_city_detail_004085b0();break;
    default:return 0;
    }
    return 1;
}
