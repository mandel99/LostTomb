#include "unit_button_actions.h"
#include "menu_group_selection.h"
#include "menu_list_scroll.h"
#include "map_commit_links.h"
#include "actor_walking.h"
#include "order_actions_finish.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(a) ((int32_t)(int16_t)W(a))
static uint32_t actor(uint32_t id){return 0xbcd5f8u+id*388u;}
static uint32_t group(uint32_t id){return 0xbcb9c8u+id*144u;}

/* Whole 4069E0, 23 instructions, cdecl1/RET0/full EAX. */
int32_t restore_unit_building_rank(uint32_t id)
{
    uint32_t record=0x7999d8u+id*264u,index;
    if(W(record+16u)!=74u)return -1;
    if(B(record)!=1u)return -1;
    index=B(record+203u);if(!index)return -1;
    return (int32_t)U(0x9783b0u+index*28u);
}

/* Whole 406A30, 78 instructions. IMUL/ADD distances wrap to signed DWORD;
   the tie-break deliberately compares that native wrapped result. */
uint32_t restore_unit_nearest_building(uint32_t id)
{
    uint32_t p=0x799af0u,index=1,best=0,best_distance=100000u,a=actor(id);
    int32_t best_rank=200,rank,by,ay,ax,bx;uint32_t dx,dy,distance;
    do{
        if(B(p-16u)==1u&&W(p)==74u&&W(p+48u)!=0u){
            rank=restore_unit_building_rank(index);
            if(rank!=-1&&rank<=best_rank){
                by=S(p-8u);ay=S(a+22u);ax=S(a+20u);bx=S(p-10u);
                dy=(uint32_t)by-(uint32_t)ay;dx=(uint32_t)bx-(uint32_t)ax;
                distance=dx*dx+dy*dy;
                if(rank<best_rank||(int32_t)distance<(int32_t)best_distance){
                    best=index;best_rank=rank;best_distance=distance;
                }
            }
        }
        p+=264u;++index;
    }while((int32_t)p<0x89b6e8);
    return best;
}

/* Whole 4A4300, 18 instructions, cdecl1/RET0. Only native AL is consumed.
   A negative nonzero WORD link is not changed into a successful guard. */
uint8_t restore_unit_actor_is_linked(uint32_t id)
{
    int32_t building=S(actor(id)+90u);
    if(building==0)return 0;
    return (uint8_t)(S(0x799a8cu+(uint32_t)building*264u)==(int32_t)id);
}

/* Whole 4A45D0, 74 instructions. Its search uses the global selected actor,
   while all resulting state changes and walking use its actual argument. */
void restore_unit_actor_assign_building(uint32_t id)
{
    uint32_t building=restore_unit_nearest_building(U(0xd056f8u)),a=actor(id),b,old_x,old_y;
    uint32_t layout;int32_t x,y;uint16_t previous_x,previous_y,target_x,target_y;
    uint8_t kind=B(a+10u),last_kind;
    if((int32_t)building<=0){if(kind==77u||kind==78u)B(a+56u)=8;return;}
    if(kind==77u)B(a+56u)=11;
    else if(kind==78u)B(a+56u)=13;
    old_y=U(0xd3b4e0u);old_x=U(0xd3b4e4u);b=0x7999d8u+building*264u;
    layout=B(b+3u);y=S(b+8u);x=S(b+6u);
    (void)FUN_004806c0(x,y,layout);
    previous_x=W(a+20u);W(a+44u)=previous_x;
    previous_y=W(a+22u);target_x=W(0xd3b4e4u);W(a+46u)=previous_y;
    last_kind=B(a+18u);target_y=W(0xd3b4e0u);B(a+17u)=last_kind;
    W(a+94u)=(uint16_t)building;W(a+36u)=target_x;W(a+38u)=target_y;
    U(0xd3b4e4u)=old_x;U(0xd3b4e0u)=old_y;
    FUN_004b3250((int32_t)id);
}

/* Whole 406B40: 77 instructions, including all four targets of406C7C
   omitted from the41-instruction cached body. */
void restore_unit_button_00406b40(void)
{
    uint32_t id=U(0xeadcc8u),a,maximum,value,mode;int32_t percentage;uint8_t state;
    U(0xd05700u)=id;
    if((int32_t)id<=0||B(0xe38e60u)!=0u)return;
    a=actor(id);maximum=U(a+300u);value=U(a+296u);
    percentage=FUN_004cde60((int32_t)value,(int32_t)maximum);state=B(a+56u);
    if((state==11u||state==15u)&&percentage<=15)return;
    mode=U(0xd3b198u);
    if(mode==3u&&(state==11u||state==15u||percentage==100))return;
    switch(mode){
    case 0:
        B(a+56u)=20;U(0xd05708u)=1;B(0xe38ddau)=31;B(0xe38dcau)=1;break;
    case 1:
        B(a+56u)=10;W(a+54u)=75;U(a+348u)=16;
        U(0xd05708u)=0;B(0xe38ddau)=1;B(0xe38dcau)=1;break;
    case 2:
        B(a+56u)=(uint8_t)(12u+(B(a+304u)!=0u));
        U(0xd05708u)=1;B(0xe38ddau)=31;B(0xe38dcau)=1;break;
    case 3:
        U(0xd05708u)=0;restore_unit_actor_assign_building(id);B(0xe38ddau)=31;B(0xe38dcau)=1;break;
    default:break;
    }
}

/* Whole 406E80, 73 instructions. There is no E38E60 lock test in this
   native callback. Preserve the mode/global reload after the linked query. */
void restore_unit_button_00406e80(void)
{
    uint32_t id=U(0xeadcc8u),a,maximum,value,b,old_x,old_y,layout,walking;
    int32_t percentage,building,x,y;uint8_t state;
    uint16_t previous_x,previous_y,target_x,target_y;
    U(0xd05700u)=id;if((int32_t)id<=0)return;
    a=actor(id);maximum=U(a+300u);value=U(a+296u);
    percentage=FUN_004cde60((int32_t)value,(int32_t)maximum);state=B(a+56u);
    if((state==11u||state==15u)&&percentage<=15)return;
    if(U(0xd3b198u)==4u){
        if(restore_unit_actor_is_linked(id))return;
        if(U(0xd3b198u)==4u){
            B(a+56u)=9;building=S(a+90u);
            old_x=U(0xd3b4e4u);old_y=U(0xd3b4e0u);b=0x7999d8u+(uint32_t)building*264u;
            U(0xd05708u)=0;layout=B(b+3u);y=S(b+8u);x=S(b+6u);
            (void)FUN_004806c0(x,y,layout);
            previous_x=W(a+20u);target_x=W(0xd3b4e4u);W(a+44u)=previous_x;
            previous_y=W(a+22u);target_y=W(0xd3b4e0u);W(a+46u)=previous_y;
            walking=U(0xd056f8u);W(a+36u)=target_x;W(a+38u)=target_y;
            U(0xd3b4e4u)=old_x;U(0xd3b4e0u)=old_y;FUN_004b3250((int32_t)walking);
        }
    }
    B(0xe38ddau)=1;B(0xe38dcau)=1;
}

/* Whole 406FB0, 85 instructions. 144-byte formations, sixteen signed-WORD
   actor slots,388-byte actors. 47D350 is the genuine XOR EAX,EAX;RET leaf;
   all three native sound arguments (57624C,1,0) are unused by that body. */
void restore_unit_button_00406fb0(void)
{
    uint32_t id=U(0xeadcccu),g=group(id),selected,a,i;int32_t member;
    uint8_t order_mode;
    if(S(g+130u)>=0||B(g+3u)==1u||B(0xe38e60u)!=0u)return;
    if(W(g+4u)==13u&&B(actor((uint32_t)S(g+8u))+56u)==19u&&U(0xd3b198u)!=1u){
        selected=U(0xd056f8u);
        for(i=0;i<16u;++i){
            member=S(g+8u+i*2u);if(!member)break;a=actor((uint32_t)member);
            if(B(a+56u)==4u||B(g+115u)!=0u)continue;
            order_mode=B(g+102u);if(order_mode>1u)continue;
            if(order_mode==1u){FUN_004b7b50((int32_t)id,10);selected=U(0xd056f8u);}
            B(a)=0;U(a+308u)=0;U(a+316u)=(uint32_t)S(a+20u);
            U(a+320u)=(uint32_t)S(a+22u);B(a+56u)=12;B(actor(selected)+1u)=0;
        }
    }
    FUN_004b6c20((int32_t)id,14);(void)FUN_0047d350();
    B(0xe38ddau)=1;B(0xe38dcau)=1;
}

/* Whole 407110, 37 instructions, including the exact15-byte remap4071AC
   and4-entry table40719C. Tail JMP406720 becomes an actual typed call. */
void restore_unit_button_00407110(void)
{
    uint32_t previous=U(0xbcb9c0u),current=U(0xeadcccu),chosen=previous,g;
    int32_t order;
    if(previous==0u||previous!=current)chosen=current;
    if(B(0xe38e60u)!=0u)return;
    g=group(chosen);B(g+119u)=(uint8_t)(1u-B(g+119u));order=S(g+42u);
    switch(order){
    case 1:case 2:U(0xd3b198u)=0;break;
    case 3:case 4:U(0xd3b198u)=1;break;
    case 15:U(0xd3b198u)=2;break;
    default:break;
    }
    if(U(0xd3b194u)==1u){U(0xeadcccu)=previous;FUN_00406720();}
}

int restore_unit_button_action(uint32_t token)
{
    switch(token){
    case 0x406b40u:restore_unit_button_00406b40();return 1;
    case 0x406e80u:restore_unit_button_00406e80();return 1;
    case 0x406fb0u:restore_unit_button_00406fb0();return 1;
    case 0x407110u:restore_unit_button_00407110();return 1;
    default:return 0;
    }
}
