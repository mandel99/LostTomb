#include "world_render.h"
#include "graphics_53f250.h"
#include "world_building_draw.h"
#include "world_special_buildings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(v) ((int32_t)(uint32_t)(v))
extern void FUN_00420a30(int32_t,int32_t);
extern int32_t FUN_0053e590(int32_t,int32_t,int32_t);
extern int32_t FUN_00488030(int32_t);
extern void FUN_0041fff0(int32_t,int32_t);
extern void FUN_004e5e90(int32_t,int32_t,int32_t);
extern void FUN_00420050(int32_t,int32_t);
extern void FUN_004206f0(int32_t,int32_t);
extern void FUN_00420d60(int32_t,int32_t);
extern void FUN_00421920(int32_t,int32_t,int32_t);
extern void FUN_00421060(int32_t,int32_t);
extern void FUN_00438e50(int32_t,int32_t,int32_t,uint32_t);
extern uint32_t FUN_00442280(int32_t,uint32_t,int32_t,int32_t);
extern uint32_t FUN_0040f2c0(int32_t,int32_t);
extern uint32_t FUN_00488050(uint32_t);
extern void FUN_00489500(int32_t,int32_t,int32_t,int32_t);
extern int32_t FUN_004381b0(int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_00438210(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern int32_t FUN_00438350(int32_t,int32_t,int32_t,int32_t,int32_t);
extern int32_t FUN_00438470(int32_t,int32_t,int32_t,int32_t);
extern int32_t FUN_004384d0(int32_t,int32_t,int32_t,int32_t);
extern int32_t FUN_00438590(int32_t,int32_t,int32_t,int32_t);
extern int32_t FUN_00438650(int32_t,int32_t,int32_t,int32_t);
extern int32_t FUN_004386b0(int32_t,int32_t,int32_t,int32_t);
extern int32_t FUN_00438710(int32_t,int32_t,int32_t,int32_t);
extern int32_t FUN_00438770(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00436310(int32_t,int32_t,int32_t);

static uint32_t building_offset(uint32_t tile)
{
    return (uint32_t)(int32_t)(int16_t)W(0x00b4e6fcu+tile*2u)*0x108u;
}
static int32_t building_id(uint32_t tile)
{
    return (int16_t)W(0x00b4e6fcu+tile*2u);
}
static int neighbor_occludes(uint32_t tile)
{
    uint32_t offset,actor;
    int32_t id;
    if(S(tile)>=0x0fff0000)return 0;
    offset=building_offset(tile);
    if(W(0x007999e8u+offset)==183 && B(0x00799a2eu+offset)!=0 &&
       U(0x00799a40u+offset)!=0 && B(0x00799a31u+offset)<=3)return 1;
    id=(int16_t)W(0x00b350dcu+tile*2u);
    while(id>0){
        actor=(uint32_t)id*0x184u;
        if(B(0x00bcd65du+actor)==0){
            switch(B(0x00bcd602u+actor)){
            case 13:case 45:case 50:case 56:case 57:return 1;
            default:break;
            }
        }
        id=(int16_t)W(0x00bcd600u+actor);
    }
    return 0;
}
static int upper_is_occluded(uint32_t grid_index,uint32_t alternate)
{
    uint32_t first,second;
    if(alternate){first=U(0x0070e0f4u+grid_index*4u);second=U(0x0070e0f8u+grid_index*4u);}
    else{first=U(0x0070e0f8u+grid_index*4u);second=U(0x0070e0fcu+grid_index*4u);}
    if(neighbor_occludes(first))return 1;
    return neighbor_occludes(second);
}
static int footprint_position(uint32_t footprint,uint32_t x,uint32_t y,uint32_t *out_x,uint32_t *out_y)
{
    switch(footprint){
    case 0:*out_x=x;*out_y=y;return 1;
    case 1:*out_y=y-U(0x70e44c);*out_x=x+U(0x70e45c)+1u;return 1;
    case 2:*out_y=y-U(0x70e448);*out_x=x+U(0x70e458)+2u;return 1;
    case 3:*out_y=y-U(0x70e444);*out_x=x+U(0x70e454)+3u;return 1;
    case 4:*out_y=y-U(0x70e440);*out_x=x+U(0x70e450)+4u;return 1;
    case 5:*out_y=y-U(0x70e440)-U(0x70e44c);*out_x=x+U(0x70e450)+U(0x70e45c)+5u;return 1;
    default:return 0;
    }
}
static void base_extras(int32_t building,uint32_t offset,uint32_t x,uint32_t y)
{
    uint32_t direction;
    uint16_t state,type;
    if(W(0x7999e8u+offset)==210){
        direction=U(0xbca2e0);
        if(direction==4||direction==2){state=W(0x7999eau+offset);if(state==0||state==3)FUN_0041fff0(x,y);}
        else if(direction==0||direction==6){state=W(0x7999eau+offset);if(state==2||state==5)FUN_0041fff0(x,y);}
    }
    type=W(0x7999e8u+offset);
    if(type==207||type==208)FUN_004e5e90(building,x,y);
    if(W(0x7999e8u+offset)==213&&W(0x7999eau+offset)==0){FUN_00542e10(building,x,y);FUN_00420050(x,y);}
    if(W(0x7999e8u+offset)==225){
        direction=U(0xbca2e0);
        if((direction==0&&W(0x7999eau+offset)==6)||(direction==2&&W(0x7999eau+offset)==5)||
           (direction==4&&W(0x7999eau+offset)==7)||(direction==6&&W(0x7999eau+offset)==8))FUN_004206f0(x,y);
    }
    if(W(0x7999e8u+offset)==227&&W(0x7999eau+offset)==0)FUN_00420d60(x,y);
    type=W(0x7999e8u+offset);
    if((type==229||type==234||type==236||type==235)&&U(0x799a3cu+offset)!=0&&
       B(0xbcd609u+U(0x799a34u+offset)*0x184u)==8)FUN_00421920(x,y,building);
    if(W(0x7999e8u+offset)==228&&W(0x7999eau+offset)==0)FUN_00421060(x,y);
}
static void draw_base(uint32_t tile,uint32_t index,uint32_t alternate,uint32_t x,uint32_t y,uint32_t *extra)
{
    int32_t building=building_id(tile);
    uint32_t offset=(uint32_t)building*0x108u,footprint,image,px,py,direction;
    uint16_t type;
    if(building!=0&&B(0x799a97u+offset)!=0)W(0x70e120)=0xf863;
    footprint=B(0xb285ccu+tile)&15u;image=U(0xb7482cu+tile*4u);U(0xd3b5c0)=image;
    type=W(0x7999e8u+offset);
    if(type==183){*extra=32;FUN_004ef550(image,x,y,building);}
    else if(type==230){
        *extra=80;
        if((footprint!=0||(U(0xaf598cu+tile*4u)&0xeefed77fu)!=0||!upper_is_occluded(index,alternate))&&
           footprint_position(footprint,x,y,&px,&py))FUN_0053f250(image,px,py);
        direction=U(0xbca2e0);
        if(((direction==0||direction==4)&&W(0x7999eau+offset)==0)||
           ((direction==2||direction==6)&&W(0x7999eau+offset)==1)){
            FUN_00401f70(building,x,y);FUN_00420a30(x,y);
        }
    }else{
        if(S(image)>=S(U(0xf572ec)+48u)&&S(image)<S(U(0xf572ec)+72u)){
            FUN_0053e590(image,x,y-150u);FUN_0053f250(U(0xd3b5c0),x,y-150u);
        }else{
            if((footprint!=0||(U(0xaf598cu+tile*4u)&0xeefed77fu)!=0||!upper_is_occluded(index,alternate))&&
               footprint_position(footprint,x,y,&px,&py)){
                FUN_0053f250(image,px,py);
                if(footprint==2&&(uint8_t)FUN_00488030(building)!=0){
                    footprint_position(2,x,y,&px,&py);restore_world_building_draw_00488c20(0x00bca350u,building,S(px),S(py),1u);
                }
            }
            if(B(0x7999ddu+offset)!=0){
                if(B(0x799a5fu+offset)==1)FUN_0053e590(U(0xf56724),x+18u,y-32u);
                if(B(0x7999ddu+offset)!=0&&B(0x799a5fu+offset)==2)FUN_0053e590(U(0xf572e4),x+18u,y-32u);
            }
        }
    }
    base_extras(building,offset,x,y);
    W(0x70e120)=0;
}

static void detail_animation(uint32_t image,uint32_t frames,int32_t building,uint32_t offset,uint32_t footprint,uint32_t x,uint32_t y)
{
    uint32_t base,variant,group,px,py,record,top,height,left,chosen;
    int32_t actor;
    static const int32_t offsets[16]={132,-7,115,25,60,20,49,-4,152,-13,123,16,65,14,38,-12};
    switch((int16_t)W(0x7999e8u+offset)){
    case 71:case 191:
        base=FUN_004381a0(624);FUN_004381b0(base-1u+frames,x,y,113,3);break;
    case 72:case 193:
        base=FUN_004381a0(628);FUN_004381b0(base-1u+frames,x,y,22,24);
        base=FUN_004381a0(351);FUN_0053e590(base+17u,x-4u,y-42u);break;
    case 90:
        base=FUN_004381a0(547);variant=(image-base)&3u;
        switch(variant){
        case 0:group=578;px=4;py=0;break;
        case 1:group=579;px=3;py=0xfffffffeu;break;
        case 2:group=580;px=12;py=0;break;
        default:group=581;px=15;py=6;break;
        }
        base=FUN_004381a0(group);FUN_004381b0(base-1u+frames,x,y,px-8u+U(0x70e45c)*2u,py+U(0x70e44c));break;
    case 75:case 192:
        actor=(int16_t)W(0x799a8cu+offset);base=FUN_004381a0(546);variant=(image-base)&3u;chosen=(variant-1u)&3u;
        group=632;
        if(actor!=0&&B(0xbcd630u+(uint32_t)actor*0x184u)==10){variant+=4;group=633;if(S(frames)>20)frames=20;}
        base=FUN_004381a0(group);
        FUN_004381b0(base+chosen-4u+frames*4u,x,y,offsets[variant*2u],offsets[variant*2u+1u]);break;
    case 74:FUN_00438210(image,frames,x,y,W(0x799a8cu+offset));break;
    case 76:FUN_00438350(image,frames,x,y,(int16_t)W(0x799a8cu+offset));break;
    case 161:case 162:case 217:FUN_00438470(image,frames,x,y);break;
    case 106:case 107:case 216:case 221:FUN_004384d0(image,frames,x,y);break;
    case 30:FUN_00438590(image,frames,x,y);break;
    case 34:FUN_00438650(image,frames,x,y);break;
    case 31:FUN_004386b0(image,frames,x,y);break;
    case 33:FUN_00438710(image,frames,x,y);break;
    case 35:FUN_00438770(image,frames,x,y);break;
    default:
        if(footprint>5)break;
        record=(uint32_t)FUN_00438180(image);top=(uint32_t)(int32_t)(int16_t)W(record+0x24u);
        record=(uint32_t)FUN_00438180(image);height=(uint32_t)(int32_t)(int16_t)W(record+0x16u);
        py=top-height;
        switch(footprint){
        case 0:py+=y+30u;break;
        case 1:py=py-U(0x70e44c)+y+60u;break;
        case 2:py=py-U(0x70e448)+y+90u;break;
        case 3:py=py-U(0x70e444)+y+120u;break;
        case 4:py=py-U(0x70e440)+y+150u;break;
        default:py=py-U(0x70e440)-U(0x70e44c)+y+180u;break;
        }
        record=(uint32_t)FUN_00438180(image);left=(uint32_t)(int32_t)(int16_t)W(record+0x22u);
        FUN_0053e590(image+frames,x+left,py);break;
    }
    W(0x70e120)=0;
    (void)building;
}
static void draw_detail(uint32_t tile,uint32_t x,uint32_t y)
{
    uint32_t image,index,offset,frames,footprint;
    int32_t building;
    uint16_t type,state;
    uint8_t edge;
    if((U(0xaf598cu+tile*4u)&0x44u)==0x44u&&B(0xadc36cu+tile)!=0){
        FUN_0053e590((uint32_t)B(0xadc36cu+tile)-1u+U(0xf56d34),x,y-U(0x70e448));return;
    }
    image=U(0xb7482cu+tile*4u);index=S(image)<S(U(0x126517c))?image:0;
    if(W(0x10d6a16u+index*64u)==0){
        building=building_id(tile);offset=(uint32_t)building*264u;edge=B(0xb67d1cu+tile);type=W(0x7999e8u+offset);
        if(type==57){
            if(edge&0x40){
                state=W(0x7999eau+offset);
                if(state==13){FUN_0053e590(U(0xf5662c)+3u,x+93u,y-20u);tile=U(0x70e488);}
                else if(state==12){FUN_0053e590(U(0xf5662c)+4u,x+93u,y-20u);tile=U(0x70e488);}
                else if(state==11){FUN_0053e590(U(0xf5662c)+2u,x+93u,y-20u);tile=U(0x70e488);}
            }
        }else if(type==202){if(edge&0x40){FUN_004c0160(building);tile=U(0x70e488);}}
        else if(type==71||type==191){FUN_00442280(building,tile,x,y);tile=U(0x70e488);}
    }else{
        if((B(0xb67d1cu+tile)&0x40)==0)return;
        building=building_id(tile);offset=(uint32_t)building*264u;
        if(building!=0&&B(0x799a97u+offset)!=0)W(0x70e120)=0xf863;
        type=W(0x7999e8u+offset);
        if(type==71||type==191){FUN_00442280(building,tile,x,y);tile=U(0x70e488);}
        type=W(0x7999e8u+offset);
        if(type==75)image=U(0xb7482cu+tile*4u);
        if(type==72||type==193){FUN_0053e590(U(0xf566ac)+17u,x-4u,y-42u);tile=U(0x70e488);}
        frames=FUN_0040f2c0(image,tile);
        if(S(frames)>0){
            index=S(image)<S(U(0x126517c))?image:0;
            if(S(frames)>(int16_t)W(0x10d6a16u+index*64u)){
                index=S(image)<S(U(0x126517c))?image:0;frames=(uint32_t)(int32_t)(int16_t)W(0x10d6a16u+index*64u);
            }
            tile=U(0x70e488);footprint=B(0xb285ccu+tile)&15u;
            if((uint8_t)FUN_00488050(building)!=0){FUN_00489500(building,frames,x,y);W(0x70e120)=0;}
            else detail_animation(image,frames,building,offset,footprint,x,y);
        }else W(0x70e120)=0;
        tile=U(0x70e488);
    }
    if(B(0xb67d1cu+tile)&0x40)FUN_00436310(building_id(tile),x,y);
}

void FUN_00436b60(uint32_t actor_parameter)
{
    uint32_t y=U(0x70e48c)-U(0x70e44c)*9u,alternate=0,rows=0,extra=4;
    uint32_t x,row,column,step,index,tile,iteration,actor;
    int32_t id;
    W(0x775688)=(uint16_t)(W(0x775688)+1u);
    U(0x70e47c)=U(0x70e474)-8u;
    if(S(U(0x70e460)+18u)<=0)return;
    do{
        if(S(U(0x70e47c))>=0&&S(U(0x70e47c))<457){
            x=U(0x70e490);if(alternate)x=x-U(0x70e45c)-1u;
            step=U(0x70e458);U(0x70e480)=U(0x70e478)-4u;x+=(0xfffffffeu-step)*4u;
            row=U(0x70e47c);iteration=0;
            if(S(U(0x70e464)+6u)>=0)do{
                column=U(0x70e480);
                if(S(column)>=0&&S(column)<231){
                    index=row*231u+column;tile=U(0x70e494u+index*4u);U(0x70e488)=tile;
                    if(S(tile)<0x0fff0000&&((U(0xe94e0c)!=51&&B(0x6de515)==0)||
                       (U(0xaf598cu+tile*4u)&0x20000002u)!=0x20000002u)&&(B(0xb67d1cu+tile)&0x40)){
                        draw_base(tile,index,alternate,x,y,&extra);row=U(0x70e47c);step=U(0x70e458);
                    }
                }
                ++iteration;U(0x70e480)=U(0x70e480)+1u;x+=step+2u;
            }while(S(iteration)<=S(U(0x70e464)+6u));

            /* Actors whose terrain has any bit in the native priority mask. */
            x=U(0x70e490);if(alternate)x=x-U(0x70e45c)-1u;
            x+=(0xfffffffeu-step)*4u;column=U(0x70e478)-4u;U(0x70e480)=column;iteration=0;
            if(S(U(0x70e464)+6u)>=0)do{
                if(S(column)>=0&&S(column)<231){
                    tile=U(0x70e494u+(row*231u+column)*4u);U(0x70e488)=tile;
                    if(S(tile)<0x0fff0000&&(U(0xaf598cu+tile*4u)&0xeefed77fu)!=0){
                        id=(int16_t)W(0xb350dcu+tile*2u);
                        while(id>0){
                            actor=(uint32_t)id*388u;
                            if(B(0xbcd65du+actor)==0&&W(0xbcd718u+actor)==0){
                                FUN_00438e50(id,x,y,actor_parameter);column=U(0x70e480);row=U(0x70e47c);
                            }
                            id=(int16_t)W(0xbcd600u+actor);
                        }
                    }
                }
                ++iteration;++column;U(0x70e480)=column;x+=U(0x70e458)+2u;
            }while(S(iteration)<=S(U(0x70e464)+6u));

            /* Building upper animation and state overlays. */
            x=U(0x70e490);if(alternate)x=x-U(0x70e45c)-1u;
            x+=(0xfffffffeu-U(0x70e458))*4u;U(0x70e480)=U(0x70e478)-4u;iteration=0;
            if(S(U(0x70e464)+6u)>=0)do{
                column=U(0x70e480);
                if(S(column)>=0&&S(column)<231){
                    tile=U(0x70e494u+(U(0x70e47c)*231u+column)*4u);U(0x70e488)=tile;
                    if(S(tile)<0x0fff0000)draw_detail(tile,x,y);
                }
                ++iteration;U(0x70e480)=U(0x70e480)+1u;x+=U(0x70e458)+2u;
            }while(S(iteration)<=S(U(0x70e464)+6u));

            /* Complementary actors follow the upper building pass. */
            x=U(0x70e490);if(alternate)x=x-U(0x70e45c)-1u;
            x+=(0xfffffffeu-U(0x70e458))*4u;column=U(0x70e478)-4u;row=U(0x70e47c);U(0x70e480)=column;iteration=0;
            if(S(U(0x70e464)+6u)>=0)do{
                if(S(column)>=0&&S(column)<231){
                    tile=U(0x70e494u+(row*231u+column)*4u);U(0x70e488)=tile;
                    if(S(tile)<0x0fff0000&&(U(0xaf598cu+tile*4u)&0xeefed77fu)==0){
                        id=(int16_t)W(0xb350dcu+tile*2u);
                        while(id>0){
                            actor=(uint32_t)id*388u;
                            if(B(0xbcd65du+actor)==0&&W(0xbcd718u+actor)==0){
                                FUN_00438e50(id,x,y,actor_parameter);column=U(0x70e480);row=U(0x70e47c);
                            }
                            id=(int16_t)W(0xbcd600u+actor);
                        }
                    }
                }
                x+=U(0x70e458)+2u;++iteration;++column;U(0x70e480)=column;
            }while(S(iteration)<=S(U(0x70e464)+6u));
        }
        alternate=!alternate;y+=U(0x70e44c);++rows;U(0x70e47c)=U(0x70e47c)+1u;
    }while(S(rows)<S(U(0x70e460)+extra+14u));
}
