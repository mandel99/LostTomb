#include "list_panel_input.h"
#include "message_panel_input.h"
#include "menu_list_scroll.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
static int32_t signed_half_cells(uint32_t cells){return (int32_t)(cells*16u)/2;}
void FUN_004e1e10(void)
{
    uint32_t a,b,c,d,x,y;int32_t value;
    a=U(0xe95104u);b=U(0xe9510cu);y=a*16u+b-42u;x=U(0xe95110u)+16u;
    FUN_00425bc0((int32_t)x,(int32_t)y,(const void *)(uintptr_t)0x5c7e98u,1,10);
    a=U(0xe95104u);b=U(0xe9510cu);y=a*16u+b-42u;x=U(0xe95110u)+16u;
    if(FUN_00425990((int32_t)x,(int32_t)y,(const void *)(uintptr_t)0x5c7e98u,1))return;
    a=U(0xe95104u);b=U(0xe9510cu);y=a*16u+b-36u;c=U(0xe95108u);d=U(0xe95110u);x=c*16u+d-38u;
    FUN_00425bc0((int32_t)x,(int32_t)y,(const void *)(uintptr_t)0x5c80f0u,1,11);
    a=U(0xe95104u);b=U(0xe9510cu);y=a*16u+b-36u;c=U(0xe95108u);d=U(0xe95110u);x=c*16u+d-38u;
    if(FUN_00425990((int32_t)x,(int32_t)y,(const void *)(uintptr_t)0x5c80f0u,1))return;
    y=U(0xe950fcu);a=U(0xe950f8u);b=U(0xe95100u);x=a*16u+b;
    FUN_00425bc0((int32_t)x,(int32_t)y,(const void *)(uintptr_t)0x5c8118u,1,12);
    y=U(0xe950fcu);a=U(0xe950f8u);b=U(0xe95100u);x=a*16u+b;
    if(FUN_00425990((int32_t)x,(int32_t)y,(const void *)(uintptr_t)0x5c8118u,1))return;
    a=U(0xe950f4u);b=U(0xe950fcu);y=a*16u+b-26u;c=U(0xe950f8u);d=U(0xe95100u);x=c*16u+d;
    FUN_00425bc0((int32_t)x,(int32_t)y,(const void *)(uintptr_t)0x5c8140u,1,12);
    a=U(0xe950f4u);b=U(0xe950fcu);y=a*16u+b-26u;c=U(0xe950f8u);d=U(0xe95100u);x=c*16u+d;
    if(FUN_00425990((int32_t)x,(int32_t)y,(const void *)(uintptr_t)0x5c8140u,1))return;
    a=U(0xe950fcu);x=U(0xe95100u);y=a+4u;
    value=FUN_00426450((int32_t)x,(int32_t)y,(const RestoreMenuButton *)(uintptr_t)0x5c4830u,10);
    a=U(0xe950fcu);x=U(0xe95100u);y=a+4u;U(0x7056c8u)=(uint32_t)value;
    if(FUN_004261d0((int32_t)x,(int32_t)y,(RestoreMenuButton *)(uintptr_t)0x5c4830u,10))return;
    a=U(0xe95104u);b=U(0xe9510cu);y=a*16u+b-40u;c=U(0xe95108u);d=U(0xe95110u);x=(uint32_t)signed_half_cells(c)+d-140u;
    value=FUN_00426450((int32_t)x,(int32_t)y,(const RestoreMenuButton *)(uintptr_t)0x5c4810u,1);
    b=U(0xe9510cu);U(0x7056c4u)=(uint32_t)value;a=U(0xe95104u);y=a*16u+b-40u;
    c=U(0xe95108u);d=U(0xe95110u);x=(uint32_t)signed_half_cells(c)+d-140u;
    if(FUN_004261d0((int32_t)x,(int32_t)y,(RestoreMenuButton *)(uintptr_t)0x5c4810u,1))return;
    if(!FUN_004e2090()&&B(0xe38df8u)){B(0xe38ddau)=1;B(0xe38dcau)=1;}
}
int32_t FUN_004e2090(void)
{
    uint32_t a,b,c,d;int32_t height,distance,percent,result;
    if((int32_t)U(0xe9511cu)<=0)return 0;
    if(!B(0xe38e00u))return 0;
    a=U(0xe950f8u);b=U(0xe95100u);c=U(0xe950f4u);a=a*16u+b;d=U(0xe38e34u);height=(int32_t)(c*16u-52u);
    if((int32_t)d<(int32_t)(a+1u))return 0;
    if((int32_t)d>(int32_t)(a+41u))return 0;
    b=U(0xe950fcu);d=U(0xe38e30u);
    if((int32_t)d<(int32_t)(b+26u))return 0;
    if((int32_t)d>(int32_t)(b+(uint32_t)height+26u))return 0;
    distance=(int32_t)(d-b-11u);if(distance>height)distance=height;
    percent=FUN_004cde60(distance,height);
    result=FUN_004cde40((int32_t)U(0xe9511cu),percent);U(0xe95120u)=(uint32_t)result;
    a=(uint32_t)distance-25u;U(0xe95118u)=1;U(0xe95114u)=a;
    if((int32_t)a<0)U(0xe95114u)=0;
    B(0xe38dcau)=1;return 1;
}
