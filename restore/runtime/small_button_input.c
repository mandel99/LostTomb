#include "small_button_input.h"
#include "menu_input.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define S(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)U(a))
static int32_t add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
extern uint32_t FUN_0047d350(void); /* Native XOR EAX,EAX; RET. */

/* Whole 425B50, 16 instructions: clear only active kind-2 records. */
void FUN_00425b50(uint32_t buttons,int32_t count)
{
    uint32_t p=buttons;
    while(count>0){
        if(B(p+25u)!=0&&W(p+8u)==2u){B(p+25u)=0;B(0xe38dc9u)=1;}
        p+=36u;--count;
    }
}
/* Whole 425B80, 25 instructions: one BYTE countdown for kinds 4 and 6. */
void FUN_00425b80(uint32_t buttons,int32_t count)
{
    uint32_t p=buttons;
    while(count>0){
        uint8_t delay=B(p+25u);
        if(delay){
            uint16_t kind=W(p+8u);
            if(kind==4u||kind==6u){
                delay=(uint8_t)(delay-1u);B(p+25u)=delay;
                if(!delay){B(p+25u)=0;B(0xe38dc9u)=1;}
            }
        }
        p+=36u;--count;
    }
}

/* Whole 425990, 159 instructions. The pre-dispatch kind is deliberately
   read AFTER the hover loop, at buttons+36*count (or buttons for count<=0).
   That read is in the original; replacing it with the hovered record or a
   synthetic sentinel would change input gating. Integer addresses preserve
   native wrap and avoid forming a C array pointer beyond its declared span. */
int32_t FUN_00425990(int32_t x,int32_t y,const void *buttons,int32_t count)
{
    uint32_t base=(uint32_t)(uintptr_t)buttons,p=base;
    int32_t remaining=count,index,changed=0;uint16_t kind;uint8_t left_down;
    FUN_00425b80(base,count);
    while(remaining>0){
        uint8_t hover=B(p+26u);
        if(hover){hover=(uint8_t)(hover-1u);B(p+26u)=hover;if(!hover)changed=1;}
        if(B(p+24u)){
            int32_t left=S(p),top=S(p+2u),mx=I(0xe38e34u),my;
            left=add(left,x);top=add(top,y);
            if(left>mx)goto next;
            if(add(S(p+4u),left)<=I(0xe38e34u))goto next;
            my=I(0xe38e30u);if(top>my)goto next;
            if(add(S(p+6u),top)<=my)goto next;
            mx=I(0xe38e34u);U(0xe38e28u)=(uint32_t)my;U(0xe38e2cu)=(uint32_t)mx;
            if(!B(p+26u))changed=1;
            B(p+26u)=2;
        }
next:  p+=36u;--remaining;
    }
    kind=W(p+8u);left_down=B(0xe38e03u);
    if(kind==2u||kind==4u){if(!left_down&&!B(0xe38df8u))return changed;}
    else if(kind==6u){if(!left_down&&!B(0xe38e00u))return changed;}
    p=base;
    for(index=0;index<count;++index,p+=36u){
        if(!B(p+24u)||!B(p+26u))continue;
        if(W(p+8u)==2u){FUN_00425b50(base,count);left_down=B(0xe38e03u);}
        U(0xd3b198u)=U(p+28u);U(0xd3b194u)=U(p+32u);
        if(left_down){
            (void)FUN_0047d350();
            B(p+25u)=20;B(p+27u)=0;
            restore_menu_callback(U(p+16u));
        }else if(B(0xe38df8u))restore_menu_callback(U(p+20u));
        else if(W(p+8u)==6u&&B(0xe38e00u)){
            B(p+25u)=20;restore_menu_callback(U(p+16u));
        }
        return index+1;
    }
    return changed;
}
