#include "building_state.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
int32_t FUN_004f3d70(uint32_t cell)
{
    uint32_t index=(uint32_t)(int32_t)(int16_t)W(0xb4e6fcu+cell*2u),record,origin,delta,mask,stage,category,mode,table,count,p,i,value;
    int32_t link;
    if(!index)return -1;
    record=0x7999e8u+index*264u;if(W(record)!=183u)return -1;
    origin=U(record-4u);delta=cell-origin;
    switch(delta){case 0:mask=1;break;case 1:mask=2;break;case 228:mask=4;break;case 229:mask=8;break;default:return 2;}
    stage=B(record+69u);
    if(stage==3u){
        category=B(record+68u);mode=U(0x5e4ef4u);table=mode==5u?0x5f6d58u:0x5f6cd0u;
        count=U(table+category*8u+4u);if((int32_t)count<=0)return 2;
        p=U(table+category*8u);link=(int16_t)W(record+2u);
        for(i=0;i<count;++i)if((uint32_t)link==U(p+i*8u)){value=U(p+i*8u+4u);return cell==origin+value?3:2;}
        return 2;
    }
    if(B(record+75u)&mask)return (int32_t)stage;
    --stage;return stage==3u?2:(int32_t)stage;
}
void FUN_004f60d0(int32_t building)
{
    uint32_t index=(uint32_t)building,record=0x7999e8u+index*264u,count,root,i;int32_t link;
    if(W(record)!=183u)return;
    link=(int16_t)W(record+2u);count=U(0xeaaf64u);root=index-(uint32_t)link;
    if((int32_t)count>0)for(i=0;i<count;++i)if(U(0xeaac30u+i*4u)==root)return;
    if((int32_t)count>3)return;
    U(0xeaac30u+count*4u)=root;U(0xeaaf64u)=count+1u;
}
