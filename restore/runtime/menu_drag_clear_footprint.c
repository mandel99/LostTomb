#include "clear_footprint.h"
#include "clear_helpers.h"
#include "road_place.h"
#include "terrain_redraw.h"
#include "menu_building_kind.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0047a220(int32_t,int32_t,int32_t);
extern void FUN_00477500(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004777c0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_004776f0(int32_t,int32_t,int32_t,int32_t);
void FUN_0047c480(int32_t building,int32_t x_in,int32_t y_in)
{
    uint32_t id=(uint32_t)building,x=(uint32_t)x_in,y=(uint32_t)y_in,record=0x7999e8u+id*264u,origin,mask=0x03052e84u;
    uint32_t rows,cols,row=0,col,table,cell,access,sprite,flags,variation,base;
    if(x_in<0){U(0xbc9734u)=1;return;}if(y_in<0){U(0xbc9734u)=1;return;}
    if(x_in>=(int32_t)U(0xbca2d0u)){U(0xbc9734u)=1;return;}
    if(y_in>=(int32_t)U(0xbca2d4u)){U(0xbc9734u)=1;return;}
    if(W(record)==183u){origin=U(record-4u);U(0xbc973cu)=2;U(0xbc9738u)=2;}
    else origin=FUN_0047c7b0(x_in,y_in);
    if(U(0xaf598cu+origin*4u)==2u)return;
    if((uint8_t)FUN_00488030(id)){U(0xbc973cu)=3;U(0xbc9738u)=3;}
    else if(W(record)==213u){rows=FUN_00541ab0(id);U(0xbc9738u)=rows;U(0xbc973cu)=rows;}
    if(W(record)==138u)mask|=U(0xaf598cu+origin*4u)&0x140u;
    rows=U(0xbc9738u);
    if((int32_t)rows>0){
        cols=U(0xbc973cu);table=0x5cc8c0u;
        do{
            if((int32_t)cols>0){
                col=0;
                do{
                    cell=origin+U(table+col*8u);
                    if(!building||(int32_t)(int16_t)W(0xb4e6fcu+cell*2u)==building){
                        if(building&&W(record)!=99u)B(0xa9010cu+cell)=B(record);
                        access=B(0xb285ccu+cell);W(0xb4e6fcu+cell*2u)=0;B(0xb285ccu+cell)=(uint8_t)(access&0xe0u);
                        sprite=B(0xb67d1cu+cell);W(0xa9cc1cu+cell*2u)=0;B(0xae8e7cu+cell)=0;B(0xb67d1cu+cell)=(uint8_t)((sprite&0xc0u)|64u);
                        flags=U(0xaf598cu+cell*4u);B(0xadc36cu+cell)=0;
                        if((flags&4u)&&!(flags&0x10000u)){
                            U(0xaf598cu+cell*4u)=flags&4u;FUN_0047a220((int32_t)(x+col),(int32_t)(y+row),1);
                        }else{
                            variation=B(0xacf85cu+cell)&7u;base=U(0xf56b54u);U(0xb7482cu+cell*4u)=variation+base;U(0xaf598cu+cell*4u)=flags&mask;
                        }
                    }
                    cols=U(0xbc973cu);++col;
                }while((int32_t)col<(int32_t)cols);
                rows=U(0xbc9738u);
            }
            ++row;table+=48u;
        }while((int32_t)row<(int32_t)rows);
    }else cols=U(0xbc973cu);
    FUN_004767d0(x_in,y_in,(int32_t)(x+cols),(int32_t)(y+rows),0,0);
    cols=U(0xbc973cu);FUN_004746d0(x_in,y_in,(int32_t)cols);
    rows=U(0xbc9738u);cols=U(0xbc973cu);FUN_00477500(x_in,y_in,(int32_t)(x+cols),(int32_t)(y+rows));
    rows=U(0xbc9738u);cols=U(0xbc973cu);FUN_004777c0(x_in,y_in,(int32_t)(x+cols),(int32_t)(y+rows));
    rows=U(0xbc9738u);cols=U(0xbc973cu);FUN_00475cc0(x_in,y_in,(int32_t)(x+cols),(int32_t)(y+rows),0);
    rows=U(0xbc9738u);cols=U(0xbc973cu);FUN_004776f0(x_in,y_in,(int32_t)(x+cols),(int32_t)(y+rows));
    rows=U(0xbc9738u);cols=U(0xbc973cu);FUN_00477890(x_in,y_in,(int32_t)(x+cols),(int32_t)(y+rows));
}
