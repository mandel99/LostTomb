#include "event_parameters.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
static void remember_random(void)
{
    uint32_t index=U(0xd3b1c4u),previous=U(0xd3b374u);
    U(0xd3b1c8u+index*4u)=previous;++index;U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
}
static void shift_random(uint32_t *first,uint32_t *second)
{
    uint32_t a=*first,b=*second,i;
    for(i=0;i<31u;++i){a=(a>>1)|((((a>>4)^a)&1u)<<30);b=(b>>1)|((((b>>4)^b)&1u)<<30);}
    *first=a;*second=b;
}
uint32_t restore_event_parameter_choose(uint32_t self,uint32_t incoming_eax)
{
    int32_t first=H(self+2u),second,third,divisor;uint32_t a,b,result=incoming_eax;
    if(first>=0){if(H(self+4u)<0){W(self)=(uint16_t)first;return result;}}
    else{
        second=H(self+4u);
        if(second>=0){third=H(self+6u);if(third>=0&&third==second){W(self)=(uint16_t)first;return 1;}}
    }
    remember_random();a=U(0xd3b384u);b=U(0xd3b380u);shift_random(&a,&b);
    U(0xd3b384u)=a;U(0xd3b378u)=a&7u;U(0xd3b380u)=b;
    U(0xd3b364u)=b&0x7fffu;U(0xd3b370u)=a&0x7fffu;U(0xd3b374u)=a&0x7fu;
    U(0xd3b368u)=b&0x7fu;U(0xd3b36cu)=b&7u;
    first=H(self+2u);
    if(first<0){
        second=H(self+4u);third=H(self+6u);divisor=third-second;
        /* Preserve the explicit failure for native zero-divisor #DE. */
        if(!divisor)abort();
        result=(uint32_t)((int32_t)(a&0x7fffu)/divisor);
        W(self)=(uint16_t)((uint32_t)((int32_t)(a&0x7fffu)%divisor)+(uint32_t)second);
        return result;
    }
    second=H(self+4u);divisor=second<0?1:(H(self+6u)>=0?3:2);
    result=(a&0x7fffu)/(uint32_t)divisor;
    switch((a&0x7fffu)%(uint32_t)divisor){
    case 0:W(self)=(uint16_t)first;break;
    case 1:W(self)=(uint16_t)second;break;
    default:result=(result&0xffff0000u)|W(self+6u);W(self)=(uint16_t)result;break;
    }
    return result;
}
uint32_t restore_event_record_reset(uint32_t self)
{
    uint32_t a,b,i;
    U(self+0x28u)&=0xfffffffbu;
    (void)restore_event_parameter_choose(self+4u,0);(void)restore_event_parameter_choose(self+12u,0);
    (void)restore_event_parameter_choose(self+20u,0);(void)restore_event_parameter_choose(self+28u,0);
    (void)restore_event_parameter_choose(self+88u,0);
    a=(uint16_t)(B(self+3u)+W(self+20u));
    B(self+0x57u)=0;W(self+0x30u)=0;W(self+0x2eu)=(uint16_t)a;
    W(self+0x34u)=0xffffu;W(self+0x64u)=0xffffu;a=B(self+2u);
    B(self+0x32u)=0;B(self+0x33u)=0;B(self+0x63u)=0;
    if(a==3u)for(i=0;i<4u;++i){W(self+0x40u+i*4u)=W(0x784c60u);W(self+0x42u+i*4u)=W(0x784c62u);}
    remember_random();a=U(0xd3b384u);b=U(0xd3b380u);shift_random(&a,&b);
    U(0xd3b384u)=a;U(0xd3b374u)=a&0x7fu;U(0xd3b380u)=b;
    B(self+0x61u)=(uint8_t)((a&0x7fffu)%5u+4u);
    remember_random();a=U(0xd3b384u);b=U(0xd3b380u);shift_random(&a,&b);
    U(0xd3b384u)=a;U(0xd3b378u)=a&7u;U(0xd3b364u)=b&0x7fffu;
    U(0xd3b370u)=a&0x7fffu;U(0xd3b380u)=b;U(0xd3b374u)=a&0x7fu;
    U(0xd3b368u)=b&0x7fu;U(0xd3b36cu)=b&7u;B(self+0x62u)=(uint8_t)(a&1u);
    return b&7u;
}
