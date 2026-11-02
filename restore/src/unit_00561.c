#include "actor_render.h"

void FUN_00438e50(int32_t actor_id,uint32_t x,uint32_t y,uint32_t selection)
{
    uint32_t actor=(uint32_t)actor_id*388u,upper=AR_U(0xbcd778u+actor);
    uint32_t horizontal=2,vertical,direction,lower,limit,index,base_x,base_y,upper_x,upper_y;
    uint32_t type,dx,dy,ship,height,image,value;
    int32_t first,second;
    type=AR_B(0xbcd602u+actor);
    switch(type){
    case 1:case 11:case 12:case 13:case 43:case 44:case 45:case 54:case 55:case 56:
    case 82:case 83:case 84:case 99:case 102:case 103:case 104:case 106:case 109:
        vertical=AR_B(0xbcd639u+actor)!=0?17u:12u;break;
    default:vertical=10;break;
    }
    direction=(uint32_t)AR_SB(0xbcd609u+actor);
    if(AR_S(direction)>=8){direction=(uint32_t)AR_SB(0xbcd60au+actor);if(AR_S(direction)>=8)direction=0;}
    direction-=AR_U(0xbca2e0);AR_U(0xe94e00)=direction;
    if(AR_S(direction)<0){direction+=8u;AR_U(0xe94e00)=direction;}
    index=AR_U(0x978d78);lower=AR_U(0xbcd5fcu+actor);
    if(index==1)return;
    if(AR_B(0xbcd604u+actor)==0){
        if(AR_SW(0xbcd718u+actor)>0)FUN_004f0d30(actor_id,&x,&y);
        else{
            index=direction*16u+(uint32_t)AR_SB(0xbcd631u+actor);
            horizontal=(uint32_t)AR_SB(0x5c99b8u+index)+2u;
            vertical+=(uint32_t)AR_SB(0x5c9a38u+index)-AR_B(0xbcd670u+actor);
        }
    }else{
        first=AR_SW(0xbcd642u+actor)%15;second=AR_SW(0xbcd644u+actor)%15;
        index=(uint32_t)first+(uint32_t)second*16u;
        switch(AR_U(0xbca2e0)){
        case 0:horizontal=(uint32_t)AR_SB(0x5c95b8u+index)+2u;vertical+=(uint32_t)AR_SB(0x5c96b8u+index);break;
        case 2:horizontal=(uint32_t)AR_SB(0x5c97b8u+index)+2u;vertical-=(uint32_t)AR_SB(0x5c98b8u+index);break;
        case 4:horizontal=2u-(uint32_t)AR_SB(0x5c95b8u+index);vertical-=(uint32_t)AR_SB(0x5c96b8u+index);break;
        case 6:horizontal=2u-(uint32_t)AR_SB(0x5c97b8u+index);vertical+=(uint32_t)AR_SB(0x5c98b8u+index);break;
        default:break;
        }
        type=AR_B(0xbcd602u+actor);vertical-=AR_B(0xbcd614u+actor);
        if(type==60){
            index=(uint32_t)AR_SW(0xbcd652u+actor)*388u;
            if(AR_B(0xbcd602u+index)==62)vertical-=18u+AR_B(0xbcd670u+index);
        }
    }
    vertical+=AR_U(0x70e44c);horizontal+=AR_U(0x70e45c);
    limit=AR_U(0x126517c);
    base_x=horizontal-actor_render_meta(lower,limit,0x10d6a1a);
    base_y=vertical-actor_render_meta(lower,limit,0x10d6a1c);
    upper_x=horizontal-2u-actor_render_meta(upper,limit,0x10d6a1a);
    upper_y=vertical-10u-actor_render_meta(upper,limit,0x10d6a1c);
    if(selection!=0){
        if(selection==9999){if(!FUN_004387d0(actor_id))return;limit=AR_U(0x126517c);}
        else{
            if((uint32_t)actor_id!=selection)return;
            AR_U(0x70e3e4)=base_x+x;AR_U(0x70e3e0)=base_y+y;
        }
    }
    if(upper==0){FUN_0053e590(lower,base_x+x,base_y+y);return;}
    type=AR_B(0xbcd602u+actor);
    if(type==4||type==38||type==9){
        direction=(uint32_t)AR_SB(0xbcd609u+actor)+AR_U(0xbca2e0);
        if(AR_S(direction)>=8)direction-=8u;
        if(direction<=7u&&direction!=3u){
            dx=(uint32_t)AR_SB(0xbcd664u+actor);dy=(uint32_t)AR_SB(0xbcd665u+actor);
            FUN_0053e590(upper,upper_x+dx+x,upper_y+dy+y);
            FUN_0053e590(lower,base_x+x,base_y+y);return;
        }
        FUN_0053e590(lower,base_x+x,base_y+y);
        dx=(uint32_t)AR_SB(0xbcd664u+actor);dy=(uint32_t)AR_SB(0xbcd665u+actor);
        FUN_0053e590(upper,upper_x+dx+x,upper_y+dy+y);return;
    }
    if(type==1||type==2||type==106||type==58||type==13||type==56||type==45){
        if(AR_SB(0xbcd665u+actor)<0){
            dx=(uint32_t)AR_SB(0xbcd664u+actor);dy=(uint32_t)AR_SB(0xbcd665u+actor);
            FUN_0053e590(upper,base_x+dx+x,base_y+dy+y);
            FUN_0053e590(lower,base_x+x,base_y+y);return;
        }
        FUN_0053e590(lower,base_x+x,base_y+y);
        dx=(uint32_t)AR_SB(0xbcd664u+actor);dy=(uint32_t)AR_SB(0xbcd665u+actor);
        FUN_0053e590(upper,base_x+dx+x,base_y+dy+y);return;
    }
    if(type==14){
        ship=(uint32_t)AR_SW(0xbcd658u+actor)*144u;
        if(AR_SW(0xbcba4au+ship)>=0)return;
        if(AR_U(0xbcba4cu+ship)==0){
            FUN_0053e590(lower,base_x+x,base_y+y);
            value=(uint32_t)((AR_SW(0xbcba42u+ship)+3)/5);
            FUN_0053e590(AR_U(0xf56b1c)-value+20u,base_x+x,base_y+y);
            height=actor_render_meta(upper,AR_U(0x126517c),0x10d6a0e);
            FUN_0053e590(upper,base_x+x,base_y-height+y);
            limit=AR_U(0x126517c);image=AR_B(0xbcb9cau+ship)+AR_U(0xf56814);
            value=actor_render_meta(image,limit,0x10d6a0e);
            FUN_0053e590(image,base_x+x,base_y-value-height+y);return;
        }
        height=actor_render_meta(upper,limit,0x10d6a0e);
        image=AR_B(0xbcb9cau+ship)+AR_U(0xf56814);
        FUN_0053e590(image,base_x+x,base_y-height+y);return;
    }
    if(type==36){
        if(AR_SW(0xbcd60cu+actor)<0||AR_SW(0xbcd60eu+actor)<0)return;
        if(AR_B(0xbcd5f9u+actor)&1u)--base_y;
        FUN_0053e590(lower,base_x+x,base_y+y);
        height=actor_render_meta(upper,AR_U(0x126517c),0x10d6a0e);
        /* PUSH y moves ESP before the native [ESP+14] image read; that
           operand therefore refers to the captured upper image. */
        FUN_0053e590(upper,base_x+x,base_y-height+y);
        value=AR_B(0xbcd603u+actor);
        if(value>=4&&value<=11)value-=3;
        else if(value>=14&&value<=21)value-=13;
        else if(value>=22&&value<=25)value-=21;
        else if(value>=26&&value<=29)value-=25;
        else if(value>=30&&value<=32)value-=29;
        else if(value>=50&&value<=65)value-=49;
        else return;
        if(value)FUN_004cce00(value,0x20,(const char *)(uintptr_t)0x57c048,base_x+x+9u,base_y+y+14u,0x6b,0);
        return;
    }
    if(type==74||type==60||type==71){
        FUN_0053e590(upper,base_x+x,base_y+y+10u);
        FUN_0053e590(lower,base_x+x,base_y+y);return;
    }
    if(type==77||type==78){
        FUN_0053e590(lower,base_x+x,base_y+y);
        dx=(uint32_t)AR_SB(0xbcd664u+actor);dy=(uint32_t)AR_SB(0xbcd665u+actor);
        FUN_0053e590(upper,upper_x+dx+x,upper_y+dy+y-24u);return;
    }
    FUN_0053e590(lower,base_x+x,base_y+y);
}
