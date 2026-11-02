#include "map_refresh_postpass5.h"
void FUN_0047b670(void)
{
    uint32_t tile=P5_R(0xbc9768u,4),flags,mode,frame,base,direction,sprite,saved,old,instance,attribute,a,b;
    if(P5_R(0xb4e6fcu+tile*2u,2)!=0)return;
    if(P5_R(0xb285ccu+tile,1)&0x80u)return;
    mode=P5_R(0x978d74u,4);
    if(mode==1){
        FUN_00484be0((void *)0x5cc378,48,-1,0);p5_normalize();frame=P5_R(0xbc9740u,4);base=P5_R(0xf56c44u,4);direction=P5_R(0xbc9748u,4);tile=P5_R(0xbc9768u,4);sprite=base+frame+direction;
    }else{
        flags=P5_R(0xaf598cu+tile*4u,4);
        if(flags&0x100u){
            old=P5_R(0xb7482cu+tile*4u,4);base=P5_R(0xf56b84u,4);saved=(int32_t)(old-base)>=48?48u:0u;
            FUN_0047e870();FUN_00484be0((void *)0x5cc678,16,-1,0);attribute=P5_R(0xbc9744u,4);
            if(attribute==0){tile=P5_R(0xbc9768u,4);a=P5_R(0xaf598cu+(tile-1u)*4u,4);if(a&0x100u)P5_W(0xbc9748u,4,0);else{a=P5_R(0xaf598cu+(tile+1u)*4u,4);P5_W(0xbc9748u,4,1);if(a&0x100u)P5_W(0xbc9748u,4,0);}}
            if(FUN_0047b920()){
                direction=P5_R(0xbc9748u,4);base=P5_R(0xf56b84u,4);tile=P5_R(0xbc9768u,4);sprite=base+direction+saved+42u;
            }else{
                direction=P5_R(0xbc9748u,4);base=P5_R(0xf56b84u,4);sprite=base+direction+saved+15u;tile=P5_R(0xbc9768u,4);
                if(P5_R(0xaf598cu+tile*4u,4)&0x10000u){sprite=(uint32_t)FUN_0051c620((int32_t)sprite);tile=P5_R(0xbc9768u,4);}
            }
        }else if(!(flags&0x10000u)&&(!(instance=P5_R(0xd05720u+tile*4u,4))||P5_R(instance+24u,1)!=0)){
            if(FUN_0047b920()){
                FUN_00484be0((void *)0x5cc378,48,-1,0);p5_normalize();frame=P5_R(0xbc9740u,4);base=P5_R(0xf56c44u,4);direction=P5_R(0xbc9748u,4);sprite=base+frame+direction;tile=P5_R(0xbc9768u,4);
            }else{
                FUN_00484be0((void *)0x5cc268,17,-1,0);p5_normalize();frame=P5_R(0xbc9740u,4);base=P5_R(0xf56c94u,4);direction=P5_R(0xbc9748u,4);tile=P5_R(0xbc9768u,4);sprite=base+frame+direction;
            }
        }else{
            old=P5_R(0xb7482cu+tile*4u,4);base=P5_R(0xf56c34u,4);saved=old-base;sprite=(uint32_t)FUN_0051c5f0(tile,(int32_t)saved);
            if(sprite!=saved||((int32_t)saved>=84&&(int32_t)saved<=87)){base=P5_R(0xf56c34u,4);sprite+=base;}
            else{FUN_00484be0((void *)0x5cc268,17,-1,0);p5_normalize();frame=P5_R(0xbc9740u,4);base=P5_R(0xf56cd4u,4);sprite=frame+base;direction=P5_R(0xbc9748u,4);sprite+=direction;}
            tile=P5_R(0xbc9768u,4);
        }
    }
    P5_W(0xb7482cu+tile*4u,4,sprite);a=P5_R(0xb285ccu+tile,1);b=P5_R(0xb67d1cu+tile,1);P5_W(0xb285ccu+tile,1,a&0xf0u);P5_W(0xb67d1cu+tile,1,b|0x40u);
}
