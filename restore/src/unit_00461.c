#include "ambient_sound.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
static int32_t add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}
static int32_t mul(int32_t a,int32_t b){return (int32_t)((uint32_t)a*(uint32_t)b);}

int32_t FUN_00430960(int32_t x,int32_t y,int32_t unused)
{
    int32_t row,column,horizontal,vertical,half,column_odd,row_odd,parity,tile,index;
    (void)unused;
    if(x<I(0x0070e490u))return 0;
    if(x>=add(mul(add(I(0x0070e458u),2),I(0x0070e464u)),I(0x0070e490u)))return 0;
    if(y<I(0x0070e48cu))return 0;
    if(y>=add(mul(I(0x0070e44cu),I(0x0070e460u)),I(0x0070e48cu)))return 0;
    x=sub(x,I(0x0070e490u));y=sub(y,I(0x0070e48cu));
    row=y/I(0x0070e44cu);I(0x0070e404u)=row;
    horizontal=add(I(0x0070e45cu),1);
    column=x/horizontal;
    column_odd=column&1;row_odd=row&1;parity=add(row,column)&1;
    half=(x%horizontal)/2;vertical=y%I(0x0070e44cu);
    column/=2;I(0x0070e408u)=column;
    if(parity==0){
        if(vertical>half){row=add(row,1);I(0x0070e404u)=row;}
        else if(column_odd&&row_odd){column=add(column,1);I(0x0070e408u)=column;}
    }else if(parity==1&&add(vertical,half)>=add(I(0x0070e44cu),-1)){
        row=add(row,1);I(0x0070e404u)=row;
        if(column_odd&&!row_odd){column=add(column,1);I(0x0070e408u)=column;}
    }
    index=add(add(mul(add(I(0x0070e474u),row),231),I(0x0070e478u)),column);
    tile=I(0x0070e494u+(uint32_t)index*4u);I(0x0070e3f8u)=tile;
    if(tile>=0x0fff0000)return 0;
    I(0x0070e3fcu)=0;I(0x0070e400u)=0;
    if(!row)I(0x0070e3fcu)=2;
    else if(row>=I(0x0070e460u))I(0x0070e3fcu)=1;
    if(row&1){
        if(!column)I(0x0070e400u)=2;
        else if(column>=I(0x0070e464u))I(0x0070e400u)=1;
    }
    return tile;
}
