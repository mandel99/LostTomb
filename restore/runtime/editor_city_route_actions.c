#include "editor_city_route_actions.h"
#include "editor_permissions_actions.h"
#include "editor_validation.h"
#include "numeric_input.h"
#include "city_numeric_confirm.h"
#include "map_record_refresh.h"
#include "map_view_commit.h"
#include "record_place.h"
#include "menu_dialog.h"
#include "menu_keyboard_modal.h"
#include "remaining_menu_actions.h"
#include "event_publisher.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f820(const void *source,void *destination,int32_t bytes);
extern void FUN_0040f8f0(void *destination,uint32_t bytes,uint8_t value);
extern void FUN_00442500(void);

/* Whole442540,44 instructions. Delete a98-byte city record and shift each
   following active record until the first inactive record or native slot199.
   No new index/active-list guards are added. */
static void delete_city(int32_t index)
{
    uint32_t record=0x77f748u+(uint32_t)index*98u,cursor;
    if(!B(record+1u))return;
    FUN_0040f8f0((void *)(uintptr_t)record,98,0);
    if(index>=199)return;
    cursor=record+98u;
    do{
        if(!B(cursor+1u))return;
        FUN_0040f820((const void *)(uintptr_t)cursor,(void *)(uintptr_t)(cursor-98u),98);
        FUN_0040f8f0((void *)(uintptr_t)cursor,98,0);
        cursor+=98u;index=(int32_t)((uint32_t)index+1u);
    }while(index<199);
}

/* Whole407340,14 instructions. Routes1..19; native returns full EAX=-1
   when none is available. Its caller keeps that original value and behavior. */
static uint32_t find_route(void)
{
    uint32_t index=1,p=0x777236u;
    do{
        if(B(p-1u)<=1u)B(p)=0;
        if(!B(p))return index;
        p+=324u;++index;
    }while((int32_t)p<0x778a42);
    return UINT32_MAX;
}

/* Whole4075A0,34 instructions, a genuine entry reached by tail JMP407409.
   The original two sequential signed clamps are retained even for count0. */
static void select_city(void)
{
    uint32_t mode,index,count,record;
    if(B(0xe38ddau)!=21u)return;
    mode=U(0xd3b198u);index=U(0x77591cu);
    if(mode==1u){--index;U(0x77591cu)=index;}
    else if(mode==0u){++index;U(0x77591cu)=index;}
    if((int32_t)index<0){index=0;U(0x77591cu)=0;}
    count=U(0x775918u);
    if((int32_t)index>=(int32_t)count){index=count-1u;U(0x77591cu)=index;}
    record=0x77f748u+index*98u;
    if(B(record+1u))U(0x775924u)=W(record+12u);
    B(0xe38dcau)=1;
}

/* Whole409FA0,57 instructions. DWORD modes/flags, signed-BYTE campaign
   selection, genuine text/modal/event and editor-map providers. */
void restore_editor_city_enter_00409fa0(void)
{
    uint32_t mode=U(0x605a3cu),blocked,body,title;
    U(0x5d2e88u)=UINT32_MAX;U(0xf53fdcu)=0;
    if(mode==1u){
        if(U(0xe38e68u)==1u){
            FUN_004cc110(5,123);FUN_004cc160(5,124);
            body=U(0xe20764u);title=U(0xe20768u);
            (void)FUN_00538cd0(title,body,0);B(0xe38dcau)=1;return;
        }
        B(0xe38ddau)=21;U(0x775928u)=4;U(0x775920u)=1;
        FUN_00442800();B(0xe38dcau)=1;return;
    }
    blocked=U(0xe38ebcu);
    if(blocked==0u&&U(0x78d828u)!=0u){restore_remaining_message_4e2970(13,0,0);return;}
    B(0xe38ddau)=20;
    if(blocked==0u&&B(0x7027c6u)==0u&&
       (int32_t)(int8_t)B(0xe38ea8u)==(int32_t)U(0x579638u)){
        B(0xe38ddau)=1;FUN_00417ae0(14);U(0xd3b17cu)=20;
    }
    B(0xe38dcau)=1;
}

/* Whole40B590,20 instructions, including its two actual function tail calls. */
void restore_editor_city_return_0040b590(void)
{
    if(U(0x605a3cu)!=1u&&U(0xe38ebcu)==0u&&U(0x78d828u)!=0u){
        restore_remaining_message_4e2970(13,0,0);return;
    }
    if(B(0xe38ddau)==1u)restore_editor_city_enter_00409fa0();else FUN_0040a090();
}

/* Whole407370,87 instructions. Native mode2 alone checks city count>0;
   other modes branch past that guard. Tail targets are complete helpers. */
void restore_editor_city_route_00407370(void)
{
    uint32_t mode=U(0xd3b198u),index,record,image,limit;uint8_t kind;
    if(mode==8u){
        if(!FUN_00409820()){FUN_004264d0(125,0x51f780u,1);return;}
        restore_editor_city_return_0040b590();return;
    }
    if(mode==2u){if((int32_t)U(0x775918u)<=0)return;}
    else if(mode==3u){
        delete_city((int32_t)U(0x77591cu));FUN_00442500();FUN_00444720();
        U(0xd3b198u)=0;select_city();return;
    }
    U(0x775928u)=mode;
    if(mode==14u){
        index=find_route();U(0x5d2a30u)=index;record=0x776fb0u+index*324u;
        B(record+321u)=0;U(record+308u)=0;
    }else if(mode==15u){U(0x5d2a30u)=1;B(0xe38dcau)=1;return;}
    if(U(0x775928u)==2u){
        index=U(0x77591cu);record=0x77f748u+index*98u;
        if(B(record+1u)){
            kind=B(record);
            if(kind==1u){
                image=FUN_004425c0(index);W(record+12u)=(uint16_t)image;
                U(0x775924u)=image&0xffffu;B(0xe38dcau)=1;return;
            }
            if(kind==0u){
                image=U(0xf56a0cu);limit=U(0xf56a14u);image+=(uint32_t)W(record+14u);
                U(0x775924u)=(int32_t)image<(int32_t)limit?image:limit;
            }else U(0x775924u)=W(record+12u);
        }
    }
    B(0xe38dcau)=1;
}
void restore_editor_city_route_004074c0(void)
{restore_city_numeric_00444270(0);B(0xe38dcau)=1;}

void restore_editor_city_route_00408190(void)
{restore_numeric_open_dword_00403800(453,U(0xe38dacu)-314u,0x5d2a30u,0x408120u);}
void restore_editor_city_route_004081c0(void)
{
    uint32_t target=0x7770f0u+U(0x5d2a30u)*324u;
    B(target)=(uint8_t)(1u+(B(target)==1u));U(0x5d2a2cu)=0;B(0xe38dcau)=1;
}
void restore_editor_city_route_004080a0(void)
{
    uint32_t index=U(0x5d2a30u),y=U(0xe38dacu)-314u;
    restore_numeric_open_byte_004038e0(453,y,0x776fb0u+index*324u,0x407d40u);
}
void restore_editor_city_route_004080e0(void)
{
    uint32_t record=0x776fb0u+U(0x5d2a30u)*324u,index;
    B(record+321u)=0;U(record+308u)=0;index=U(0x5d2a30u);
    if((int32_t)index>1)U(0x5d2a30u)=index-1u;
    B(0xe38dcau)=1;
}

/* Whole4081F0,26 instructions. Exact four-target40827C switch plus default. */
void restore_editor_city_route_004081f0(void)
{
    uint32_t mode=U(0xd3b198u),image;
    B(0xe38dcau)=1;U(0x775920u)=mode;
    switch(mode){
    case 0:image=U(0xf56a14u);break;
    case 1:image=U(0xf5682cu);break;
    case 2:image=U(0xf569bcu);break;
    case 3:image=U(0xf569a4u);break;
    default:image=U(0xf569acu);break;
    }
    U(0x5d2a2cu)=0;U(0x775924u)=image;
}
void restore_editor_city_route_00408290(void)
{
    uint32_t mode=U(0xd3b198u),record=U(0x77591cu),kind=U(0x705650u+mode*4u),state;
    state=FUN_00444a00(record,kind)+1u;if((int32_t)state>3)state=0;
    record=U(0x77591cu);restore_editor_permission_set_00444a40(record,kind,(int32_t)state);
    B(0xe38dcau)=1;
}

int restore_editor_city_route_action(uint32_t token)
{
    switch(token){
    case 0x40b590u:restore_editor_city_return_0040b590();return 1;
    case 0x407370u:restore_editor_city_route_00407370();return 1;
    case 0x4074c0u:restore_editor_city_route_004074c0();return 1;
    case 0x408190u:restore_editor_city_route_00408190();return 1;
    case 0x4081c0u:restore_editor_city_route_004081c0();return 1;
    case 0x4080a0u:restore_editor_city_route_004080a0();return 1;
    case 0x4080e0u:restore_editor_city_route_004080e0();return 1;
    case 0x4081f0u:restore_editor_city_route_004081f0();return 1;
    case 0x408290u:restore_editor_city_route_00408290();return 1;
    default:return 0;
    }
}
