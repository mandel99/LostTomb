#include "ground_render.h"

static int special_building(uint16_t type)
{
    return type==229||type==234||type==235||type==236||type==230;
}
static void overlay_tile(uint32_t tile,uint32_t x,uint32_t y)
{
    uint32_t mode,offset,flags,foot,px,py,a=0,b=0;
    int32_t building=(int16_t)GR_W(0xb4e6fcu+tile*2u);
    int special,prefix=1;
    offset=(uint32_t)building*264u;special=special_building(GR_W(0x7999e8u+offset));mode=GR_U(0xe94e0c);
    switch(mode){
    case 27:if(!special){FUN_00433760(x,y,0);return;}break;
    case 28:if(!special){FUN_00434e10(x,y);return;}break;
    case 33:a=1;b=0;break;
    case 34:a=3;b=2;break;
    case 35:a=6;b=5;break;
    case 36:a=2;b=1;break;
    case 37:a=4;b=3;break;
    case 38:a=5;b=4;break;
    case 39:a=13;b=9;break;
    case 40:a=19;b=11;break;
    case 41:a=17;b=10;break;
    case 42:a=15;b=8;break;
    default:prefix=0;break;
    }
    if(prefix&&!special){FUN_00434aa0(x,y,a,b);return;}
    if((GR_B(0xb67d1cu+tile)&0x40u)==0)return;
    if(special_building(GR_W(0x7999e8u+offset))){
        GR_U(0xd3b5c0)=GR_U(0xb7482cu+tile*4u);foot=GR_B(0xb285ccu+tile)&15u;
        if(ground_footprint(foot,x,y,&px,&py))FUN_0053e590(GR_U(0xd3b5c0),px,py);
        return;
    }
    mode=GR_U(0xe94e0c);
    if(mode==45){FUN_00431ce0(x,y);return;}
    if(mode==30){restore_ground_empty_at(x,y);return;}
    if(mode==51||GR_B(0x6de515)!=0){FUN_00432620(x,y);return;}
    foot=GR_B(0xb285ccu+tile)&15u;flags=GR_U(0xaf598cu+tile*4u);
    if(flags&0x100u){
        GR_U(0xd3b5c0)=GR_U(0xb7482cu+tile*4u);
        if(foot!=0)return;
    }else if((flags&0x40u)!=0&&(flags&8u)==0){
        GR_U(0xd3b5c0)=GR_U(0xb7482cu+tile*4u);
        if(foot>1)return;
    }else{
        if(flags&0x40000000u){FUN_00432d30(building,x,y,0);return;}
        if(mode!=49&&(flags&0xc04000u)!=0){GR_U(0xd3b5c0)=GR_U(0xf56bdc);foot=0;}
        else{
            if(flags&8u){FUN_00432d30(building,x,y,0);return;}
            GR_U(0xd3b5c0)=GR_U(0xb7482cu+tile*4u);
        }
    }
    if(ground_footprint(foot,x,y,&px,&py))FUN_0053e590(GR_U(0xd3b5c0),px,py);
}

void FUN_004316f0(void)
{
    uint32_t ystep=GR_U(0x70e44c),row=GR_U(0x70e474)-8u,y=GR_U(0x70e48c)-ystep*9u,rows=0,alternate=0;
    uint32_t height=GR_U(0x70e460)+14u,origin,x,column,tile,columns;
    GR_U(0x70e47c)=row;
    if(GR_S(height)<=0)return;
    origin=GR_U(0x70e490);
    do{
        x=origin;if(alternate)x=x-GR_U(0x70e45c)-1u;alternate=!alternate;
        x+=(0xfffffffeu-GR_U(0x70e458))*4u;GR_U(0x70e480)=GR_U(0x70e478)-4u;columns=0;
        if(GR_S(GR_U(0x70e464)+6u)>=0){
            do{
                column=GR_U(0x70e480);
                if(GR_S(column)>=0&&GR_S(column)<231&&GR_S(GR_U(0x70e47c))>=0&&GR_S(GR_U(0x70e47c))<457){
                    tile=GR_U(0x70e494u+(GR_U(0x70e47c)*231u+column)*4u);GR_U(0x70e488)=tile;
                    if(tile==GR_U(0x6059d0)){GR_U(0x70e3ec)=x;GR_U(0x70e3e8)=y;}
                    if(GR_S(tile)>=0x0fff0000){GR_U(0xd3b5c0)=tile-0x0fff0000u;FUN_0053e590(GR_U(0xd3b5c0),x,y);}
                    else overlay_tile(tile,x,y);
                }
                ++columns;x+=GR_U(0x70e458)+2u;GR_U(0x70e480)=GR_U(0x70e480)+1u;
            }while(GR_S(columns)<=GR_S(GR_U(0x70e464)+6u));
            ystep=GR_U(0x70e44c);origin=GR_U(0x70e490);
        }
        y+=ystep;++rows;GR_U(0x70e47c)=GR_U(0x70e47c)+1u;
    }while(GR_S(rows)<GR_S(GR_U(0x70e460)+14u));
}
