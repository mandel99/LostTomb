#include "numeric_completion.h"
#include "menu_full_input_numeric.h"
#include "menu_full_input_routes.h"
#include "menu_route_capacity.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(a) ((int32_t)(int16_t)W(a))
typedef char numeric_completion_pointer32[sizeof(void *)==4?1:-1];
extern void *FUN_00561a1d(uint32_t bytes);
extern void FUN_00561a2b(uint32_t allocation);

/* Whole 407D40, 176 native instructions. Reuses the actual typed x87 route
   primitives and explicit-self 407F90. The serialized route domain has at
   most50 signed-WORD vertices. required_samples fixes the same insufficient
   (need+50)*6 allocation already repaired in the route-delete/controller
   paths. Original need, segment early-stop and healthy allocation sizes stay
   unchanged. Native allocation failure has no invented successful exit. */
void restore_numeric_completion_00407d40(void)
{
    uint32_t self=0x776fb0u+U(0x5d2a30u)*324u;
    uint32_t count=B(self+0x141u),point=self+8u,segment,need=0;
    uint32_t old,capacity,required,generated=0,length,xratio,yratio;
    uint32_t written,distance,offset;int planned,resize;
    int32_t x0,y0,x1,y1,dx,dy,ax,ay;uint8_t raw[10];
    for(segment=1;segment<count;++segment){
        dx=S(point)-S(point+6u);dy=S(point+2u)-S(point+8u);
        ax=dx>-dx?dx:-dx;ay=dy>-dy?dy:-dy;
        need+=(uint32_t)(ax>ay?ax:ay)+1u;point+=6u;
        /* Native writes a private direction byte which is never read. */
    }
    old=U(self+0x138u);capacity=need+50u;
    resize=(int32_t)need>(int32_t)old&&(int32_t)old<(int32_t)capacity;
    planned=restore_menu_route_required_samples((const uint8_t *)(uintptr_t)(self+8u),
                                               count,B(self),need,&required);
    if(!resize)capacity=old;
    if(planned&&required>capacity){capacity=required;resize=1;}
    if(resize){
        old=U(self+4u);
        if(old){FUN_00561a2b(old);U(self+4u)=0;}
        U(self+4u)=(uint32_t)(uintptr_t)FUN_00561a1d(capacity*6u);
        U(self+0x138u)=capacity;
    }
    count=B(self+0x141u);U(self+0x134u)=need;
    if(count>1u){
        segment=1;point=self+8u;
        do{
            if((int32_t)generated>(int32_t)U(self+0x134u))goto done;
            x0=S(point);x1=S(point+6u);y0=S(point+2u);y1=S(point+8u);
            dx=x1-x0;dy=y1-y0;written=0;
            length=restore_menu_input_length32(dx,dy);
            if((length&0x7fffffffu)!=0u&&B(self)!=0u){
                yratio=restore_menu_input_ratio32(dy,length);
                xratio=restore_menu_input_ratio32(dx,length);distance=0;
                if(restore_menu_input_compare32(0,length)&0x4100u){
                    offset=generated*6u;
                    do{
                        restore_menu_input_coordinate80(x0,xratio,(int32_t)distance,raw);
                        W(U(self+4u)+offset)=(uint16_t)restore_menu_input_trunc80(raw);
                        restore_menu_input_coordinate80(y0,yratio,(int32_t)distance,raw);
                        W(U(self+4u)+offset+2u)=(uint16_t)restore_menu_input_trunc80(raw);
                        B(U(self+4u)+offset+4u)=(uint8_t)(segment-1u);
                        distance+=B(self);++written;offset+=6u;
                    }while(restore_menu_input_compare32((int32_t)distance,length)&0x4100u);
                }
            }
            generated+=written;++segment;point+=6u;
        }while((int32_t)segment<(int32_t)B(self+0x141u));
    }
    U(self+0x134u)=generated;
    (void)restore_menu_route_endpoints(self);
done:
    U(0x5d2a2cu)=0;
}

/* Whole 408120, 28 instructions: DWORD selected index, BYTE route flags. */
void restore_numeric_completion_00408120(void)
{
    int32_t selected=(int32_t)U(0x5d2a30u);uint32_t self;
    if(selected<1||selected>=20){U(0x5d2a30u)=1;return;}
    self=0x776fb0u+(uint32_t)selected*324u;
    if(B(self+0x141u)<=1u){B(self+0x142u)=0;selected=(int32_t)U(0x5d2a30u);}
    if(B(self+0x142u)==0u){
        self=0x776fb0u+(uint32_t)selected*324u;
        B(self+0x141u)=0;U(self+0x134u)=0;U(0x775928u)=14;
    }
}

/* Whole 4084B0, 10 instructions: 98-byte city records, unsigned BYTE clamp. */
void restore_numeric_completion_004084b0(void)
{
    uint32_t target=0x77f762u+U(0x77591cu)*98u;
    if(B(target)>19u)B(target)=19;
}

/* Whole 44C780, 29 instructions, native ECX receiver +3 stack DWORD slots,
   RET12. Only their signed low WORDs are consumed. No live callback return.
   This is a private explicit-self implementation, not a legacy global bind. */
static void range_pack(uint32_t self,int16_t first,int16_t second,int16_t third)
{
    if(first<0){
        if(second>=0){first=second;second=-1;}
        else if(third>=0){first=third;third=-1;}
    }
    if(second<0&&third>=0){second=third;third=-1;}
    W(self+2u)=(uint16_t)first;W(self+4u)=(uint16_t)second;W(self+6u)=(uint16_t)third;
}

static uint32_t selected_event(void)
{
    int32_t selected=(int32_t)U(0x78a61cu);
    if(selected<0||selected>=(int32_t)U(0x785d70u))return 0;
    return 0x785d74u+(uint32_t)selected*124u;
}

/* Whole four106-instruction callbacks. Their only differing instruction is
   the LEA receiver offset. Native scratch DWORD upper halves are not inputs:
   every decision/write and 44C780 consumes only the signed low WORD. */
static void edit_range(uint32_t field_offset)
{
    uint32_t record=selected_event(),field;int16_t first,low,high,value;int32_t mode;
    if(!record)return;
    field=record+field_offset;first=(int16_t)W(field+2u);
    if(first<0){low=(int16_t)W(field+4u);high=(int16_t)W(field+6u);}
    else if((int16_t)W(field+4u)<0){low=first;high=first;}
    else{low=-1;high=-1;}
    mode=S(0x78a638u);
    if(mode==1){
        value=(int16_t)W(0x78a636u);
        if(low>value)low=value;
        if(low<0||low==value){
            W(field+2u)=(uint16_t)value;W(field+4u)=0xffffu;W(field+6u)=0xffffu;
        }else if(value<0){range_pack(field,low,-1,-1);}
        else if(low>value){
            W(field+2u)=0xffffu;W(field+4u)=(uint16_t)value;W(field+6u)=(uint16_t)low;
        }else{
            W(field+2u)=0xffffu;W(field+4u)=(uint16_t)low;W(field+6u)=(uint16_t)value;
        }
    }else if(mode==0){
        value=(int16_t)W(0x78a636u);
        if(high<value)high=value;
        if(value<0||high==value){
            W(field+2u)=(uint16_t)high;W(field+4u)=0xffffu;W(field+6u)=0xffffu;
        }else if(high<0){
            W(field+2u)=(uint16_t)value;W(field+4u)=0xffffu;W(field+6u)=0xffffu;
        }else if(value>high){
            W(field+2u)=0xffffu;W(field+4u)=(uint16_t)high;W(field+6u)=(uint16_t)value;
        }else{
            W(field+2u)=0xffffu;W(field+4u)=(uint16_t)value;W(field+6u)=(uint16_t)high;
        }
    }
}
void restore_numeric_completion_00449c90(void){edit_range(0x14u);}
void restore_numeric_completion_00449e40(void){edit_range(0x0cu);}
void restore_numeric_completion_00449ff0(void){edit_range(0x58u);}
void restore_numeric_completion_0044a1a0(void){edit_range(0x1cu);}

/* Whole 44A350, 27 instructions. Four exact native targets in44A3B8. */
void restore_numeric_completion_0044a350(void)
{
    uint32_t record=selected_event();int32_t mode;
    if(!record)return;
    mode=S(0x78a638u);
    switch(mode){
    case 0:W(record+0x24u)=W(0x78a636u);break;
    case 1:W(record+0x26u)=W(0x78a636u);break;
    case 2:W(record+0x52u)=W(0x78a636u);break;
    case 3:W(record+0x54u)=W(0x78a636u);break;
    default:break;
    }
}
void restore_numeric_completion_0044a530(void)
{
    uint32_t record=selected_event();if(record)W(record+0x2cu)=W(0x78a636u);
}
void restore_numeric_completion_0044a5b0(void)
{
    uint32_t record=selected_event();if(record)B(record+0x36u)=B(0x78a636u);
}

int restore_numeric_completion_action(uint32_t token)
{
    switch(token){
    case 0x407d40u:restore_numeric_completion_00407d40();return 1;
    case 0x408120u:restore_numeric_completion_00408120();return 1;
    case 0x4084b0u:restore_numeric_completion_004084b0();return 1;
    case 0x449c90u:restore_numeric_completion_00449c90();return 1;
    case 0x449e40u:restore_numeric_completion_00449e40();return 1;
    case 0x449ff0u:restore_numeric_completion_00449ff0();return 1;
    case 0x44a1a0u:restore_numeric_completion_0044a1a0();return 1;
    case 0x44a350u:restore_numeric_completion_0044a350();return 1;
    case 0x44a530u:restore_numeric_completion_0044a530();return 1;
    case 0x44a5b0u:restore_numeric_completion_0044a5b0();return 1;
    default:return 0;
    }
}
