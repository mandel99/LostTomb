#include "map_refresh_postpass8.h"
/* Whole 0047a220. Incoming ECX is scratch; the three arguments are cdecl.
   All index/addition arithmetic wraps at 32 bits as in the native LEAs. */
void FUN_0047a220(int32_t x,int32_t y,int32_t set)
{
    uint32_t oy=P8_R(0xbca2ec,4),ox=P8_R(0xbca2e8,4),tile,flags,saved,water,base,frame,sprite,variant,rot=0,level,ptr,other,mark;
    int32_t left,top,cols=3,rows=3,endy,endx,row,col,count,code;
    uint8_t inactive[8];uint32_t i;
    static const int32_t around[8]={-228,-227,1,229,228,227,-1,-229};
    if(!(uint8_t)p8_bounds((int32_t)((uint32_t)x+ox),(int32_t)((uint32_t)y+oy)))return;
    tile=P8_R(0xbca2d8,4)+(uint32_t)y*228u+(uint32_t)x;
    flags=P8_R(0xaf598cu+tile*4u,4);flags=set==1?flags|4u:flags&0xfbfffffbu;
    left=(int32_t)((uint32_t)x-1u);top=(int32_t)((uint32_t)y-1u);
    P8_W(0xaf598cu+tile*4u,4,flags);
    if(left<0){cols=(int32_t)((uint32_t)left+3u);left=0;}else{int32_t width=(int32_t)P8_R(0xbca2d0,4);if((int32_t)((uint32_t)left+3u)>width)cols=(int32_t)((uint32_t)width-(uint32_t)left);}
    if(top<0){rows=(int32_t)((uint32_t)top+3u);top=0;}else{int32_t height=(int32_t)P8_R(0xbca2d4,4);if((int32_t)((uint32_t)top+3u)>height)rows=(int32_t)((uint32_t)height-(uint32_t)top);}
    water=(uint8_t)p8_water(left,top,0);saved=P8_R(0xbc9768,4);
    endy=(int32_t)((uint32_t)top+(uint32_t)rows);tile=P8_R(0xbca2d8,4)+(uint32_t)top*228u+(uint32_t)left;
    for(row=top;row<endy;row=(int32_t)((uint32_t)row+1u)){
        endx=(int32_t)((uint32_t)left+(uint32_t)cols);
        for(col=left;col<endx;col=(int32_t)((uint32_t)col+1u),++tile){
            flags=P8_R(0xaf598cu+tile*4u,4);if(!(flags&4u)||(flags&8u))continue;
            P8_W(0xbc9768,4,tile);p8_gather(4);p8_invert();
            FUN_00484be0((void *)0x5ca658,48,(int32_t)P8_R(0xacf85cu+tile,1),0);
            flags=P8_R(0xaf598cu+tile*4u,4);
            if(flags&0x10000u){
                variant=P8_R(0xbc9748,4);if((int32_t)variant<8)goto fallback;
                if(water){base=P8_R(0xf5722c,4);frame=P8_R(0xbc9740,4);}else{frame=P8_R(0xbc9740,4);base=P8_R(0xf56bd4,4);}
                sprite=base+frame+variant;goto signcheck;
            }
            P8_W(0xbc9768,4,tile);p8_gather(0x10000);count=p8_count();
            if(!count){variant=P8_R(0xbc9748,4);goto fallback;}
            other=P8_R(0xbc9768,4);
            for(i=0;i<8;++i){ptr=P8_R(0xd05720u+(other+(uint32_t)around[i])*4u,4);inactive[i]=(uint8_t)(ptr!=0&&P8_R(ptr+24u,1)==0);}
            level=(uint32_t)(count<2?count:2)*2u;variant=P8_R(0xbc9748,4);code=-1;
            switch(variant){
            case 8:case 12:case 16:case 20:
                rot=P8_R(0xbca2e0,4);i=variant==8?rot-4u:variant==12?rot-2u:variant==16?rot:rot+2u;
                if(inactive[i&7u])code=variant==8?4:variant==12?6:variant==16?0:2;
                level=0;if(code<0)goto default_road;break;
            case 24:case 28:case 32:case 36:
                rot=P8_R(0xbca2e0,4);
                i=(variant==24||variant==28)?rot-2u:rot+2u;
                if(inactive[i&7u])code=(variant==24||variant==28)?6:2;
                else {i=(variant==24||variant==36)?rot-4u:rot;if(!inactive[i&7u])goto default_road;code=(variant==24||variant==36)?4:0;}
                if(level==2){other=P8_R(0xbc97d8,4);other|=P8_R(0xbc97d0,4);other|=P8_R(0xbc97c8,4);other|=P8_R(0xbc97c0,4);if(other)level=4;}break;
            default:goto default_road;
            }
            base=P8_R(0xf56c34,4);sprite=base+3u*(((uint32_t)code-4u)&7u)+60u+level;
            if(p8_context(rot+(uint32_t)code+1u))++sprite;else if(p8_context(rot+(uint32_t)code+2u))++sprite;
            if(water){other=sprite-base;if((int32_t)other>=60&&(int32_t)other<=83)sprite+=P8_R(0xf57234,4)-base-60u;}
            if((int32_t)sprite>=0)goto publish;
default_road:
            frame=P8_R(0xbc9740,4);base=P8_R(water?0xf5722cu:0xf56bd4u,4);sprite=base+frame+variant;
            P8_W(0xbc9768,4,tile);FUN_0047dc80(0x100,4);
            P8_W(0xbc97dc,4,0);P8_W(0xbc97d4,4,0);P8_W(0xbc97cc,4,0);P8_W(0xbc97c4,4,0);
            count=p8_count();variant=P8_R(0xbc9748,4);if(!count)goto signcheck;code=-1;
            switch(variant){
            case 8:case 12:case 16:case 20:
                rot=P8_R(0xbca2e0,4);i=variant==8?rot-4u:variant==12?rot-2u:variant==16?rot:rot+2u;
                if(!p8_context(i))goto signcheck;code=variant==8?3:variant==12?0:variant==16?1:2;break;
            default:
                rot=P8_R(0xbca2e0,4);other=p8_context(rot-2u);
                if(other&&p8_context(rot-4u))code=7;
                else if(other&&p8_context(rot))code=4;
                else if(p8_context(rot+2u)){if(p8_context(rot))code=5;else if(p8_context(rot-4u))code=6;}
                if(variant==24){if(p8_context(rot-4u))code=11;else if(other)code=8;}
                else if(variant==28){if(other)code=13;else if(p8_context(rot))code=10;}
                else if(variant==32){if(p8_context(rot+2u))code=12;else if(p8_context(rot))code=15;}
                else if(variant==36){if(p8_context(rot+2u))code=9;else if(p8_context(rot-4u))code=14;}
                if(code<0)goto signcheck;break;
            }
            sprite=P8_R(water?0xf57264u:0xf56c9cu,4)+(uint32_t)code;
signcheck:
            if((int32_t)sprite>=0)goto publish;
fallback:
            if(P8_R(0xaf598cu+tile*4u,4)&0x4000000u){
                p8_reset();P8_W(0xbc9768,4,tile);p8_gather(0x4000000);p8_invert();
                other=P8_R(0xbc9768,4);FUN_00484be0((void *)0x5cbfb8,26,(int32_t)P8_R(0xacf85cu+other,1),1);
                if(water){frame=P8_R(0xbc9740,4);base=P8_R(0xf57224,4);variant=P8_R(0xbc9748,4);sprite=base+frame*15u+variant;}
                else if((uint8_t)p8_water(left,top,1)){other=P8_R(0xbc9768,4);frame=P8_R(0xacf85cu+other,1)&3u;rot=P8_R(0xbca2e0,4);base=P8_R(0xf57324,4);sprite=base+frame+(uint32_t)((int32_t)rot/2)*4u;}
                else{frame=P8_R(0xbc9740,4);base=P8_R(0xf56d24,4);variant=P8_R(0xbc9748,4);sprite=base+frame*15u+variant;}
            }else{
                base=P8_R(variant?(water?0xf5721cu:0xf56b64u):(water?0xf5722cu:0xf56bd4u),4);frame=P8_R(0xbc9740,4);sprite=base+frame+variant;
            }
publish:
            other=P8_R(0xb285ccu+tile,1);mark=P8_R(0xb67d1cu+tile,1);
            P8_W(0xb67d1cu+tile,1,mark|0x40u);P8_W(0xb7482cu+tile*4u,4,sprite);P8_W(0xb285ccu+tile,1,other&0xf0u);
        }
        tile+=228u-(uint32_t)cols;
    }
    P8_W(0xbc9768,4,saved);
}
