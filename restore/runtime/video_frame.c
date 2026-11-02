#include "video_open.h"
#include "map_file_load.h"
#include "media_bindings.h"
#include <windows.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
typedef int32_t (WINAPI *VideoLock)(void *,void *,void *,uint32_t,void *);
typedef int32_t (WINAPI *VideoRecover)(void *);
typedef int32_t (WINAPI *VideoUnlock)(void *,void *);
extern int32_t FUN_00427510(int32_t result);
extern uint32_t FUN_004cff20(void);
extern void FUN_00539220(uint32_t ignored);
static uint32_t video_method(uint32_t self,uint32_t offset){return U(U(self)+offset);}
uint32_t FUN_00538fa0(void)
{
    uint32_t i,front,self,handle,mask,pitch,pixels,height,x,y,frame_count,current,available;
    int32_t result;
    if(!U(0xf54054u))return 0;
    if(_SmackWait_4(U(0xf54114u)))return 0;
    for(i=0;i<27;++i)U(0xf540a8u+i*4u)=0;
    front=U(0xe92e04u)!=0;U(0xf540a8u)=108;
    self=U(front?0xe39178u:0xe39174u);
    result=((VideoLock)(uintptr_t)video_method(self,100))((void *)(uintptr_t)self,0,(void *)(uintptr_t)0xf540a8u,1,0);
    if(front&&result==(int32_t)0x887601c2u){
        do{self=U(0xe39178u);result=((VideoRecover)(uintptr_t)video_method(self,108))((void *)(uintptr_t)self);}while(!result);
    }else if(result){
        if(front){
            result=FUN_00427510(result);
            FUN_004ce6e0((char *)(uintptr_t)0x576364u,0,(int32_t)((uint32_t)result+1u));U(0xe92e04u)=0;
        }
    }else{
        mask=U(0xf53ff4u);pitch=U(0xf540ccu);
        if(front){handle=U(0xf54114u);pixels=U(0xf540b8u);}
        else{pixels=U(0xf540b8u);handle=U(0xf54114u);}
        height=U(handle+8u);y=U(0xf54004u);x=U(0xf54008u);
        (void)_SmackToBuffer_28(handle,x,y,pixels,height,pitch,mask);
        (void)_SmackDoFrame_4(U(0xf54114u));self=U(front?0xe39178u:0xe39174u);pixels=U(0xf540ccu);
        ((VideoUnlock)(uintptr_t)video_method(self,128))((void *)(uintptr_t)self,(void *)(uintptr_t)pixels);
    }
    handle=U(0xf54114u);frame_count=U(handle+12u)-1u;current=U(handle+0x374u);
    if(current<frame_count){(void)_SmackNextFrame_4(handle);return 1;}
    if(handle){(void)_SmackClose_4(handle);U(0xf54114u)=0;}
    FUN_004ce2d0();FUN_004ce6e0((char *)(uintptr_t)0x5f902cu,0xf5400c,0);
    available=FUN_004cff20();FUN_004ce6e0((char *)(uintptr_t)0x5f8fe4u,0,(int32_t)(available+1u));
    FUN_00539220(0);return 1;
}
