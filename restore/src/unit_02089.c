#include "editor_brush_validation.h"
static uint32_t flags(uint32_t cell){return EBV_U(0xaf598cu+cell*4u);}
static int outside(uint32_t x,uint32_t y,uint32_t extra)
{
    uint32_t wy=EBV_U(0xbca2ecu)+y+extra,wx=EBV_U(0xbca2e8u)+x+extra;
    return (uint8_t)FUN_0046f9a0((int32_t)wx,(int32_t)wy,3)!=0;
}
static void draw(uint32_t x,uint32_t y,uint32_t samples,uint32_t mask)
{
    int32_t sx,sy;uint32_t i;
    FUN_004f0ba0((int32_t)x,(int32_t)y,&sx,&sy);
    for(i=0;(int32_t)i<(int32_t)samples;++i){
        uint32_t yy=EBV_U(0x5cfcf4u+i*8u)+(uint32_t)sy,xx=EBV_U(0x5cfcf0u+i*8u)+(uint32_t)sx;
        FUN_0053f4f0((int32_t)EBV_U(0xf56be4u),(int32_t)xx,(int32_t)yy,mask);
    }
}
static void terrain_or(uint32_t cell)
{
    EBV_W(0xaf598cu+cell*4u,flags(cell)|0x10000000u);
    EBV_W(0xaf5990u+cell*4u,EBV_U(0xaf5990u+cell*4u)|0x10000000u);
    EBV_W(0xaf5d1cu+cell*4u,EBV_U(0xaf5d1cu+cell*4u)|0x10000000u);
    EBV_W(0xaf5d20u+cell*4u,EBV_U(0xaf5d20u+cell*4u)|0x10000000u);
}
static void terrain_and(uint32_t cell)
{
    EBV_W(0xaf598cu+cell*4u,flags(cell)&0x03052e84u);
    EBV_W(0xaf5990u+cell*4u,EBV_U(0xaf5990u+cell*4u)&0x03052e84u);
    EBV_W(0xaf5d1cu+cell*4u,EBV_U(0xaf5d1cu+cell*4u)&0x03052e84u);
    EBV_W(0xaf5d20u+cell*4u,EBV_U(0xaf5d20u+cell*4u)&0x03052e84u);
}
static int missing(uint32_t cell,uint32_t code)
{
    if(EBV_S(0xb4e6fcu+cell*2u))return 0;
    EBV_W(0xeaae90u,cell);EBV_W(0xeaae8cu,code);return 1;
}
static int matches(uint32_t cell)
{
    if((uint32_t)(int32_t)EBV_S(0xb4e6fcu+cell*2u)!=EBV_U(0xeaae88u))return 0;
    EBV_WB(0xeaae84u,1);return 1;
}
void FUN_004ed360(int32_t input_x,int32_t input_y,int32_t mode,uint32_t mask)
{
    uint32_t x=(uint32_t)input_x,y=(uint32_t)input_y,base=EBV_U(0xbca2d8u),cell=base+y*228u+x;
    uint32_t reject=0xeeffd76eu,terminal=0xfbffff7bu,samples,value,record;
    EBV_WB(0xeaae84u,0);EBV_W(0xeaaea0u,1);EBV_W(0xeaae94u,2);
    EBV_W(0xeaae9cu,0);EBV_W(0xeaae98u,0);EBV_W(0xeaae90u,0);
    EBV_W(0xeaaea4u,cell);EBV_W(0xeaae8cu,0);
    if(mode==3||mode==4||mode==5||mode==6){reject=0xeeffd766u;terminal=0xebffff73u;}
    if(outside(x,y,0)||outside(x,y,1))return;
    if((EBV_U(0xaf5990u+cell*4u)&reject)||(EBV_U(0xaf5d1cu+cell*4u)&reject)||(EBV_U(0xaf5d20u+cell*4u)&reject))return;
    samples=EBV_U(0x5cfcdcu);
    if(mode==1)draw(x,y,samples,mask);
    else if(mode==2){
        terrain_or(cell);value=EBV_U(0xf5736cu);
        EBV_W(0xb7482cu+cell*4u,value);EBV_W(0xb74830u+cell*4u,value);
        EBV_W(0xb74bbcu+cell*4u,value);EBV_W(0xb74bc0u+cell*4u,value);
    }else if(mode==3){if(missing(cell,0xda))return;}
    else if(mode==4){
        if(!EBV_S(0xb4e6fcu+cell*2u)){
            EBV_W(0xaf598cu+cell*4u,flags(cell)|0x10000000u);
            EBV_W(0xaf5990u+cell*4u,EBV_U(0xaf5990u+cell*4u)|0x10000000u);
            EBV_W(0xaf5d1cu+cell*4u,EBV_U(0xaf5d1cu+cell*4u)|0x10000000u);
            record=EBV_U(0xaf5d20u+cell*4u)|0x10000000u;value=EBV_U(0xf5736cu);
            EBV_W(0xb7482cu+cell*4u,value);EBV_W(0xb74830u+cell*4u,value);EBV_W(0xb74bbcu+cell*4u,value);
            EBV_W(0xaf5d20u+cell*4u,record);EBV_W(0xb74bc0u+cell*4u,value);
        }
    }else if(mode==5){
        record=(uint32_t)(int32_t)EBV_S(0xb4e6fcu+cell*2u);
        if(record){FUN_00485100((int32_t)record);EBV_W(0xbc971cu,EBV_U(0xbc971cu)+1u);EBV_WB(0x7999d8u+record*264u,6);}
        else terrain_and(cell);
    }else if(mode==6){if(matches(cell))return;}
    cell+=EBV_U(0xeaaea0u)*2u;x+=2u;
    if(outside(x,y,0))return;
    for(;;){
        if(outside(x,y,1))return;
        if((EBV_U(0xaf5990u+cell*4u)&reject)||(EBV_U(0xaf5d1cu+cell*4u)&reject)||(EBV_U(0xaf5d20u+cell*4u)&reject))break;
        if(mode==1)draw(x,y,samples,mask);
        else if(mode==2)terrain_or(cell);
        else if(mode==4){if(!EBV_S(0xb4e6fcu+cell*2u))terrain_or(cell);}
        else if(mode==3){if(missing(cell,0xdb))return;}
        else if(mode==5){
            record=(uint32_t)(int32_t)EBV_S(0xb4e6fcu+cell*2u);
            if(record){FUN_00485100((int32_t)record);EBV_W(0xbc971cu,EBV_U(0xbc971cu)+1u);EBV_WB(0x7999d8u+record*264u,6);}
            else terrain_and(cell);
        }else if(mode==6){if(matches(cell))return;}
        value=EBV_U(0xeaae98u)+1u;EBV_W(0xeaae98u,value);
        if((int32_t)value>20)return;
        if((int32_t)(EBV_U(0xbca2ecu)+y)>=(int32_t)EBV_U(0xbca2f4u))return;
        cell+=EBV_U(0xeaaea0u)*2u;x+=2u;
        if(outside(x,y,0))return;
    }
    if(!EBV_U(0xeaae98u))return;
    value=EBV_U(0xaf5990u+cell*4u);
    if((value&terminal)||(EBV_U(0xaf5d1cu+cell*4u)&reject))return;
    record=EBV_U(0xaf5d20u+cell*4u);
    if((record&terminal)||!(value&4u)||!(record&4u))return;
    if(outside(x,y,0)||outside(x,y,1))return;
    if(mode==1){samples=EBV_U(0x5cfcdcu);draw(x,(uint32_t)input_y,samples,mask);EBV_W(0xeaae9cu,cell);return;}
    if(mode==2||(mode==4&&!EBV_S(0xb4e6fcu+cell*2u))){
        value=flags(cell);EBV_W(0xeaae9cu,cell);EBV_W(0xaf598cu+cell*4u,value|0x10000000u);
        EBV_W(0xaf5990u+cell*4u,EBV_U(0xaf5990u+cell*4u)|0x10000000u);
        EBV_W(0xaf5d1cu+cell*4u,EBV_U(0xaf5d1cu+cell*4u)|0x10000000u);
        EBV_W(0xaf5d20u+cell*4u,EBV_U(0xaf5d20u+cell*4u)|0x10000000u);
        value=EBV_U(0xf5736cu);EBV_W(0xb7482cu+cell*4u,value);EBV_W(0xb74bbcu+cell*4u,value);return;
    }
    if(mode==3){if(missing(cell,0xdc))return;}
    else if(mode==5){
        record=(uint32_t)(int32_t)EBV_S(0xb4e6fcu+cell*2u);
        if(record){
            FUN_00485100((int32_t)record);value=EBV_U(0xbc971cu)+1u;
            EBV_W(0xeaae9cu,cell);EBV_W(0xbc971cu,value);EBV_WB(0x7999d8u+record*264u,6);return;
        }
        value=flags(cell)&0x03052e84u;EBV_W(0xeaae9cu,cell);EBV_W(0xaf598cu+cell*4u,value);
        EBV_W(0xaf5990u+cell*4u,EBV_U(0xaf5990u+cell*4u)&0x03052e84u);
        EBV_W(0xaf5d1cu+cell*4u,EBV_U(0xaf5d1cu+cell*4u)&0x03052e84u);
        EBV_W(0xaf5d20u+cell*4u,EBV_U(0xaf5d20u+cell*4u)&0x03052e84u);return;
    }else if(mode==6){if(matches(cell))return;}
    EBV_W(0xeaae9cu,cell);
}
