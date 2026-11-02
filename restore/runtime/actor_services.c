#include "actor_services.h"
#include "actor_routing.h"
#include "actor_predicate.h"
#include "building_services.h"
#include "house_services.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole47E230/47E250. x86 masks the shift count, including for channels
   outside0..4. The setter uses XOR FFFF, not a DWORD complement. */
uint32_t FUN_0047e230(uint32_t channel,int32_t tile)
{
    return (U(0x978d9cu+(uint32_t)tile*4u)>>((channel*3u)&31u))&7u;
}
void FUN_0047e250(uint32_t channel,int32_t tile)
{
    uint32_t shift=(channel*3u)&31u,address=0x978d9cu+(uint32_t)tile*4u;
    uint32_t packed=U(address),value=(packed>>shift)&7u;
    if(value<7u)U(address)=(packed&((7u<<shift)^0xffffu))|((value+1u)<<shift);
}

/* Shared native rectangle traversal. Keep the distinct publication points
   required by crime, water and ordinary service loops. By-value caller
   argument scratch is not part of the logical results. */
int32_t restore_house_service_scan(int32_t x_pos,int32_t y_pos,uint32_t radius,
    RestoreHouseCellEffect effect,uint32_t value,uint32_t publication)
{
    uint32_t left=(uint32_t)x_pos-radius,top=(uint32_t)y_pos-radius;
    uint32_t right=(uint32_t)x_pos+radius,bottom=(uint32_t)y_pos+radius;
    uint32_t width,height,tile,gap,rows,cells,scan,count=0;
    if(I(left)<0)left=0;
    if(I(top)<0)top=0;
    width=U(0xbca2d0u);if(I(right)>=I(width))right=width-1u;
    height=U(0xbca2d4u);if(I(bottom)>=I(height))bottom=height-1u;
    U(0xd3b4e0u)=top;tile=U(0xbca2d8u)+top*228u+left;
    gap=227u+left-right;U(0xbc9768u)=tile;
    if(I(top)>I(bottom))return 0;
    rows=bottom-top+1u;U(0xd3b4e0u)=bottom+1u;
    do{
        U(0xd3b4e4u)=left;
        if(I(left)<=I(right)){
            cells=right-left+1u;scan=tile;tile+=cells;
            if(publication&1u)U(0xbc9768u)=tile;
            U(0xd3b4e4u)=right+1u;
            do{
                count+=effect(H(0xb4e6fcu+scan*2u),value);
                ++scan;
            }while(--cells);
        }
        tile+=gap;if(publication&2u)U(0xbc9768u)=tile;
    }while(--rows);
    if(!(publication&2u))U(0xbc9768u)=tile;
    return I(count);
}
static uint32_t stamp_house(int32_t building,uint32_t field)
{
    uint32_t off=(uint32_t)building*264u;
    if(building<=0||!B(0x7999ddu+off)||H(0x7999f4u+off)<=0)return 0;
    B(field+off)=96u;return 1;
}
static int32_t house_visit(int32_t x_pos,int32_t y_pos,uint32_t field)
{
    return restore_house_service_scan(x_pos,y_pos,2u,stamp_house,field,2u);
}
#define VISIT(token,field) \
int32_t FUN_##token(int32_t x_pos,int32_t y_pos){return house_visit(x_pos,y_pos,field);}
VISIT(00484450,0x799a46u)
VISIT(00484120,0x799a4au)
VISIT(00484230,0x799a4bu)
VISIT(00484340,0x799a4du)
VISIT(00484560,0x799a4eu)
VISIT(00484780,0x799a50u)
VISIT(004833c0,0x799a44u)
VISIT(004834d0,0x799a45u)
VISIT(004835e0,0x799a47u)
VISIT(004836f0,0x799a49u)
VISIT(00483800,0x799a5eu)
#undef VISIT

static int32_t x(uint32_t off){return H(0xbcd60cu+off);}
static int32_t y(uint32_t off){return H(0xbcd60eu+off);}
static uint32_t city_offset(void){return U(0xeaac28u)*0x49d8u;}
static void provide_amount(uint32_t off,uint32_t amount)
{
    FUN_00483260(x(off),y(off),I(amount));B(0xbcd65eu+off)=B(0x9ab9dcu);
}
static void update_strength(uint32_t off,uint32_t value)
{
    uint32_t previous=B(0xbcd65eu+off);
    if(I(value)>(int32_t)previous)B(0xbcd65eu+off)=(uint8_t)value;
    else B(0xbcd65eu+off)=previous>10u?(uint8_t)(previous-10u):0;
}

/* Native priest service caches the home and major temple before coverage.
   Its gate and later major-temple lookup use the current city after queries.
   Classifier calls remain separate, ordered and full DWORD comparisons. */
static uint32_t provide_priest(uint32_t off)
{
    int32_t home=H(0xbcd652u+off);
    uint32_t major=U(0xea5f9cu+city_offset()),kind=W(0x7999e8u+(uint32_t)home*264u);
    uint32_t count=0,first,second,category,desired;
    if(kind>=60u&&kind<=69u)count=(uint32_t)FUN_00483fb0(x(off),y(off),(int32_t)((kind-60u)%5u));
    if(!W(0xea5ce8u+city_offset()))return count;
    first=FUN_004c5120(I(major));second=FUN_004c5120(home);
    if(first!=second)return count;
    category=FUN_004c5120(I(major));
    switch(category){
    case 1:desired=1;break;
    case 3:desired=2;break;
    case 4:desired=10;break;
    default:return count;
    }
    if(!FUN_004c52d0(I(U(0xea5f9cu+city_offset())),desired))return count;
    if(category==4u)(void)FUN_00483160(x(off),y(off));
    else provide_amount(off,U(category==1u?0x706918u:0x706968u));
    return count;
}

/* Whole4BAA10. The actor offset is fixed for the invocation, but coordinates,
   home, type and cell are reloaded at the native callback continuations.
   Coverage contributes its low WORD to the live home service counter. */
uint32_t FUN_004baa10(int32_t actor)
{
    uint32_t off=(uint32_t)actor*388u,kind=B(0xbcd602u+off),count=0,value;
    int32_t home,selected,mode;
    uint8_t action;
    if(kind==40u)return 0;
    switch(kind){
    case 5:count=(uint32_t)FUN_00484990(x(off),y(off));break;
    case 7:
        count=(uint32_t)FUN_00484a90(x(off),y(off));B(0xbcd65eu+off)=B(0x9ab9e8u);break;
    case 26:count=(uint32_t)FUN_00483910(H(0xbcd652u+off),x(off),y(off));break;
    case 87:count=(uint32_t)FUN_00483e90(x(off),y(off));break;
    case 32:
        count=(uint32_t)FUN_00484450(x(off),y(off));FUN_00482ef0(x(off),y(off),I(U(0x707ef8u)));break;
    case 29:count=(uint32_t)FUN_00484120(x(off),y(off));break;
    case 30:count=(uint32_t)FUN_00484230(x(off),y(off));break;
    case 31:count=(uint32_t)FUN_00484340(x(off),y(off));break;
    case 33:
        count=(uint32_t)FUN_00484560(x(off),y(off));
        value=FUN_00482dd0(x(off),y(off),I(U(0x7065f8u)));update_strength(off,value);break;
    case 98:FUN_0042ff20(x(off),y(off));break;
    case 34:count=(uint32_t)FUN_00484780(x(off),y(off));break;
    case 64:count=FUN_00484890(x(off),y(off));break;
    case 27:count=provide_priest(off);break;
    case 8:
        count=(uint32_t)FUN_00482c10(x(off),y(off));update_strength(off,U(0x9ab9e0u));break;
    case 10:
        count=(uint32_t)FUN_00483000(x(off),y(off));B(0xbcd65eu+off)=(uint8_t)count;break;
    case 88:provide_amount(off,U(0x706760u));break;
    case 89:
        FUN_00483260(x(off),y(off),I(U(0x707b88u)));FUN_00484670(x(off),y(off));
        B(0xbcd65eu+off)=B(0x9ab9dcu);break;
    case 15:count=(uint32_t)FUN_004833c0(x(off),y(off));break;
    case 16:case 17:
        action=B(0xbcd630u+off);
        selected=H((action==12u||action==13u?0xbcd652u:0xbcd656u)+off);
        mode=B(0x799a80u+(uint32_t)selected*264u)?2:1;
        /* Both native callees ignore the extra mode slot pushed by4BAA10. */
        (void)mode;
        count=(uint32_t)(kind==16u?FUN_004834d0(x(off),y(off)):FUN_004835e0(x(off),y(off)));break;
    case 18:count=(uint32_t)FUN_004836f0(x(off),y(off));break;
    case 105:count=(uint32_t)FUN_00483800(x(off),y(off));break;
    default:break;
    }
    home=H(0xbcd652u+off);kind=B(0xbcd602u+off);
    if(home){
        value=0x7999f0u+(uint32_t)home*264u;W(value)=(uint16_t)(W(value)+(uint16_t)count);
        if(H(value)>300)W(value)=300;
    }
    FUN_0047e250(U(0x5dbf70u+kind*4u),I(U(0xbcd618u+off)));
    return 0;
}
