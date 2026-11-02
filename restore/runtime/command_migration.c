#include "command_migration.h"
#include "actor_create.h"
#include "text_selection.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Complete pinned-PE bodies: 41CCA0/469C60/469D50/469E40:4/77/62/94
   instructions. Genuine dependencies are canonical text selection and actor
   creation, plus the complete road query below (47FE60/47FEB0:36/132).
   DWORD arithmetic wraps before signed comparisons, as in x86. No new clamp. */
void FUN_0041cca0(void){FUN_00469c60(3);}
int restore_migration_command_action(uint32_t token)
{
    if(token!=0x0041cca0u)return 0;
    FUN_0041cca0();return 1;
}

uint32_t FUN_00469d50(void)
{
    uint32_t cursor,count,index;uint8_t text;int32_t kind;
    FUN_004cc110(0x133,(int32_t)(int8_t)B(0xe38ea8u));
    text=B(U(0xe20768u)+4u);
    if(text=='A'||text=='V'){
        count=U(0x78d790u);index=1u;cursor=0x799af6u;
        if((int32_t)count>=1)do {
            kind=H(cursor-6u);
            if((kind==222||kind==229||kind==234||kind==235||kind==236)&&H(cursor)==-1)
                return (cursor&0xffffff00u)|1u;
            ++index;cursor+=264u;
        }while((int32_t)index<=(int32_t)count);
    }
    /* The immutable PE switch has 30 bytes (469E18), with target0=469DF6
       and target1=469DFB. Its first count pointer is for index -1. */
    cursor=0x784e78u;
    for(kind=-1;kind<35;++kind,cursor+=4u){
        switch(kind){
        case 0:case 7:case 9:case 12:case 14:case 16:case 17:case 18:
        case 19:case 22:case 23:case 24:case 25:case 27:case 29:
            if((int32_t)U(cursor)>0)return 1u;
        }
    }
    return 0u;
}

void FUN_00469e40(int32_t building,uint8_t alternative)
{
    uint32_t offset=(uint32_t)building*264u,record,created,x,y;int32_t old;uint8_t kind,low;
    old=H(0x799a02u+offset);
    if(old>0){
        record=(uint32_t)old*388u;
        if(B(0xbcd606u+record)!=0){
            kind=B(0xbcd602u+record);
            if((kind==23||kind==0)&&H(0xbcd652u+record)==building)return;
        }
        W(0x799a02u+offset)=0;
    }
    /* Native reads the same field again. Keep its second validation intact. */
    old=H(0x799a02u+offset);
    if(old>0){
        record=(uint32_t)old*388u;
        if(B(0xbcd606u+record)!=0){
            kind=B(0xbcd602u+record);
            if((kind==24||kind==0)&&H(0xbcd652u+record)==building)return;
        }
        W(0x799a02u+offset)=0;
    }
    y=(uint32_t)H(0x7999e0u+offset);
    x=(uint32_t)H(0x7999deu+offset);
    kind=B(0x7999dbu+offset);
    if(FUN_0047fe60((int32_t)x,(int32_t)y,kind,2)==0)return;
    /* Native snapshots selected y before x, then reads the player BYTE. */
    y=U(0xd3b4e0u);x=U(0xd3b4e4u);
    kind=B(0x7999d9u+offset);
    created=(uint32_t)FUN_004b4800(kind,alternative?24:23,(int32_t)x,(int32_t)y,4);
    if(created==0)return;
    W(0x799a02u+offset)=(uint16_t)created;
    low=B(0x799a25u+offset);
    record=created*388u;
    B(0xbcd630u+record)=8;
    W(0xbcd652u+record)=(uint16_t)building;
    W(0xbcd62eu+record)=(uint16_t)((low&15u)+10u);
}

void FUN_00469c60(int32_t repetitions)
{
    uint32_t remaining=(uint32_t)repetitions,count,index,cursor,selected,best,value,record,answer,offset;
    int32_t old;uint8_t kind;
    if(repetitions<=0)return;
    count=U(0x78d790u);
    do {
        index=1;selected=0;best=0;
        if((int32_t)count>=1){
            cursor=0x799b0au;
            do {
                if(B(cursor-42u)==1&&B(cursor-37u)!=0&&W(cursor-14u)!=0){
                    old=H(cursor);
                    if(old>0){
                        record=(uint32_t)old*388u;
                        if(B(0xbcd606u+record)!=0){
                            kind=B(0xbcd602u+record);
                            if((kind==23||kind==24)&&H(0xbcd652u+record)==(int32_t)index)
                                goto next_building;
                        }
                        W(cursor)=0;
                    }
                    value=U(cursor+186u);
                    if((int32_t)value>(int32_t)best){best=value;selected=index;}
                }
next_building:
                ++index;cursor+=264u;
            }while((int32_t)index<=(int32_t)count);
            if((int32_t)selected>0){
                answer=FUN_00469d50();
                FUN_00469e40((int32_t)selected,(uint8_t)answer);
                /* Deliberately uses the post-scan index, NOT selected. */
                offset=index*264u;
                count=U(0x78d790u);
                value=U(0x70ce0cu+(uint32_t)H(0x7999eau+offset)*88u);
                U(0x799abcu+offset)=value;
            }
        }
        --remaining;
    }while(remaining!=0);
}

uint32_t FUN_0047fe60(int32_t x,int32_t y,uint32_t size,int32_t distance)
{
    uint32_t radius=1;
    if(distance<1)return 0;
    do {
        if(FUN_0047feb0(x,y,size,(int32_t)radius,0)!=0)return 1;
        ++radius;
    }while((int32_t)radius<=distance);
    return 0;
}

uint32_t FUN_0047feb0(int32_t x,int32_t y,uint32_t size,int32_t distance,uint32_t skip)
{
    uint32_t left=(uint32_t)x-(uint32_t)distance;
    uint32_t top=(uint32_t)y-(uint32_t)distance;
    uint32_t right=(uint32_t)x+size+(uint32_t)distance-1u;
    uint32_t bottom=(uint32_t)y+size+(uint32_t)distance-1u;
    uint32_t column,row,tile,terrain,row_step,best=12,best_x,best_y,best_tile,rank,network,list;
    if((int32_t)left<0)left=0;
    if((int32_t)top<0)top=0;
    column=U(0xbca2d0u);if((int32_t)right>=(int32_t)column)right=column-1u;
    row=U(0xbca2d4u);if((int32_t)bottom>=(int32_t)row)bottom=row-1u;
    best_x=left;best_y=top;
    tile=U(0xbca2d8u)+top*228u+left;best_tile=tile;
    U(0xd3b4e0u)=top;
    row_step=left-right+227u;
    U(0xbc9768u)=tile;
    row=top;
    if((int32_t)row>(int32_t)bottom)return 0;
    do {
        column=left;U(0xd3b4e4u)=column;
        if((int32_t)column<=(int32_t)right){
            terrain=0xaf598cu+tile*4u;
            do {
                if((U(terrain)&0x44u)==0x40u&&skip==0){
                    network=B(0xa76aecu+tile);rank=11;
                    list=0xea5f1cu+U(0xeaac28u)*0x49d8u;
                    for(uint32_t i=0;i<5u;++i,list+=8u)
                        if(U(list)==network){rank=i;break;}
                    if((int32_t)rank<(int32_t)best){
                        best=rank;best_tile=tile;best_x=column;best_y=row;
                    }
                }
                ++column;++tile;terrain+=4u;
            }while((int32_t)column<=(int32_t)right);
            U(0xd3b4e4u)=column;
        }
        ++row;tile+=row_step;
    }while((int32_t)row<=(int32_t)bottom);
    U(0xbc9768u)=tile;
    U(0xd3b4e0u)=row;
    if((int32_t)best>=12)return 0;
    U(0xbc9768u)=best_tile;
    U(0xd3b4e4u)=best_x;
    U(0xd3b4e0u)=best_y;
    return 1;
}
