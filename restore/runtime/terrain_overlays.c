#include "ground_render.h"
#include "menu_prepare_integer.h"
#include "menu_building_kind.h"
#include "actor_transport.h"
#include "actor_order_los.h"
#include "actor_teardown_model.h"
#include "city_updates.h"
#include "production_updates.h"
extern int32_t FUN_00459ff0(int32_t building);
extern void FUN_00433980(uint32_t image,int32_t x,int32_t y);
extern void FUN_004339a0(uint32_t image,int32_t x,int32_t y);
extern void FUN_004f0ba0(int32_t x,int32_t y,int32_t *screen_x,int32_t *screen_y);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
typedef void (*OverlayDraw)(uint32_t,int32_t,int32_t);

/* Whole431690: preserve native full EAX, including the partial-AL exits. */
uint32_t FUN_00431690(int32_t building)
{
    uint32_t value=U(0xe94e0cu),cursor;
    if(value==51u)return (value&0xffffff00u)|1u;
    if(value==8u&&H(0x799a22u+(uint32_t)building*264u)!=0)return (value&0xffffff00u)|1u;
    cursor=U(0x5d25acu+value*4u);
    if(cursor){
        value=U(cursor);
        while(value){
            if(I(value)==H(0x7999e8u+(uint32_t)building*264u))return (value&0xffffff00u)|1u;
            cursor+=4u;value=U(cursor);
        }
    }
    return value&0xffffff00u;
}

static int cart_waiting(uint32_t offset)
{
    int32_t actor=H(0x799a02u+offset);uint32_t a=(uint32_t)actor*388u;
    return actor!=0&&B(0xbcd602u+a)==4u&&B(0xbcd630u+a)==8u&&B(0xbcd65eu+a)!=0;
}
static int stock_problem_kind(int32_t kind)
{
    if(kind>=110&&kind<=114)return 1;
    switch(kind){
    case 32:case 47:case 51:case 53:case 74:case 113:case 177:case 178:
    case 187:case 188:case 189:case 194:case 203:case 204:case 205:
    case 226:case 231:case 232:case 233:return 1;
    default:return 0;
    }
}

/* Whole432920: status DWORD and sticky marker BYTE are distinct. Required
   employment0 returns before the final latch, even with hazard bits set. */
void FUN_00432920(int32_t building)
{
    uint32_t offset=(uint32_t)building*264u,tag,field=0x799ad8u+offset;
    int32_t kind,required,spare;int no_goods;
    if(H(0x7999e8u+offset)==92){U(field)=0;return;}
    if(building==0||H(0x799a10u+offset)!=0)return;
    U(field)=0;
    if(H(0x799a1eu+offset)>800)U(field)=1;
    if(H(0x799a20u+offset)>800)U(field)|=2u;
    if(B(0x7999ddu+offset)){
        if(!B(0x7999d8u+offset))goto latch;
        if(H(0x7999f4u+offset)==0)U(field)|=8u;
        else if(U(0x799ac4u+offset)!=0&&I(U(0x799ac4u+offset))<3)U(field)|=16u;
        if(B(0x799a5fu+offset)==1u)U(field)|=32u;
        else if(B(0x799a5fu+offset)==2u)U(field)|=0x2000u;
        else if(I(U(0x799ad4u+offset))>800){
            if(H(0x7999f4u+offset)==0)goto latch;
            U(field)|=4u;
        }
        if(H(0x7999f4u+offset)==0)goto latch;
        if(I(U(0x799abcu+offset))>800)U(field)|=64u;
        (void)FUN_00454190((uint32_t)building);
        if(B(0x799a5du+offset)<=29u||B(0x799a5du+offset)==72u||B(0x799a5du+offset)==70u)U(field)|=128u;
    }else{
        if((uint8_t)FUN_00488030((uint32_t)building)!=0){
            if(cart_waiting(offset))B(0x799a9bu+offset)=1;
        }else{
            kind=H(0x7999e8u+offset);
            if(kind>=106&&kind<=109){
                if(cart_waiting(offset))B(0x799a9bu+offset)=1;
            }else if(stock_problem_kind(kind)&&FUN_00459ff0(building)==0){
                if(cart_waiting(offset))B(0x799a9bu+offset)=1;
                else if(H(0x799a14u+offset)<=0){
                    no_goods=1;
                    if(H(0x7999e8u+offset)==74){
                        tag=(uint32_t)(uint16_t)H(0x799a8cu+offset);
                        if(!(tag&0xf000u)){spare=building;no_goods=FUN_00460fb0(&spare)!=0;}
                        else if((tag&0xf000u)==0x1000u&&(tag&0xfffu)==25u)no_goods=0;
                    }
                    if(no_goods){U(field)|=0x100u;B(0x799a9bu+offset)=1;}
                }
            }
        }
        kind=H(0x7999e8u+offset);required=I(U(0x705eb8u+(uint32_t)kind*40u));
        if(required==0)return;
        if(FUN_00459240(building)!=0)U(field)|=0x800u;
        else if(H(0x7999f2u+offset)<=0&&(uint8_t)restore_actor_building_gate(building)==0)U(field)|=0x1000u;
        else if(H(0x799a18u+offset)==0)U(field)|=0x200u;
        else if(H(0x799a18u+offset)<required)U(field)|=0x400u;
    }
latch:
    if(U(field)!=0&&!B(0x799a9bu+offset))B(0x799a9bu+offset)=1;
}

static int footprint_position(uint32_t footprint,uint32_t *x,uint32_t *y)
{
    switch(footprint){
    case 0:return 1;
    case 1:*x+=U(0x70e45cu)+1u;*y-=U(0x70e44cu);return 1;
    case 2:*x+=U(0x70e458u)+2u;*y-=U(0x70e448u);return 1;
    case 3:*x+=U(0x70e454u)+3u;*y-=U(0x70e444u);return 1;
    case 4:*x+=U(0x70e450u)+4u;*y-=U(0x70e440u);return 1;
    case 5:*x+=U(0x70e450u)+U(0x70e45cu)+5u;*y-=U(0x70e440u);*y-=U(0x70e44cu);return 1;
    default:return 0;
    }
}
static void base_image(uint32_t x,uint32_t y,uint32_t footprint,OverlayDraw draw,int draw_unknown)
{
    uint32_t image=U(0xb7482cu+U(0x70e488u)*4u);
    U(0xd3b5c0u)=image;
    if(footprint_position(footprint,&x,&y)||draw_unknown)draw(image,I(x),I(y));
}
static void sprite(uint32_t image,int32_t x,int32_t y)
{(void)FUN_0053e590(I(image),x,y);}
static void publish_sprite(uint32_t image,uint32_t x,uint32_t y)
{U(0xd3b5c0u)=image;sprite(image,I(x),I(y));}

/* Whole434E10: fertility is unsigned BYTE; image bases and terrain are DWORD. */
void FUN_00434e10(int32_t x,int32_t y)
{
    uint32_t tile=U(0x70e488u),fertility=B(0xa5d4ccu+tile),level,flags;
    if(fertility){
        level=fertility/10u+1u;
        if(level>9u)level=9u;
        publish_sprite(U(0xf56ca4u)+level,(uint32_t)x,(uint32_t)y);return;
    }
    flags=U(0xaf598cu+tile*4u);
    if(!(flags&0x26341677u)||(flags&8u))
        base_image((uint32_t)x,(uint32_t)y,0,sprite,0);
    else if(B(0xb67d1cu+tile)&64u)
        base_image((uint32_t)x,(uint32_t)y,B(0xb285ccu+tile)&15u,sprite,0);
}

/* Whole433760. The renderer selector is the low BYTE, and the appeal is
   signed BYTE; the column-image branch does not publish D3B5C0. */
void FUN_00433760(int32_t x,int32_t y,uint32_t upper)
{
    uint32_t tile=U(0x70e488u),flags=U(0xaf598cu+tile*4u),image,group,level;
    int32_t appeal=(int8_t)B(0xac2d4cu+tile),building;
    OverlayDraw draw=(uint8_t)upper?FUN_004339a0:FUN_00433980;
    if((flags&0x26341777u)!=0u&&!(flags&8u)){
        if(B(0xb67d1cu+tile)&64u)base_image((uint32_t)x,(uint32_t)y,B(0xb285ccu+tile)&15u,draw,0);
        return;
    }
    if(flags&0x4000u){
        image=U(0xf56b8cu)+(B(0xacf85cu+tile)&7u);U(0xd3b5c0u)=image;draw(image,x,y);return;
    }
    if(!(flags&0x28u)&&appeal==0){base_image((uint32_t)x,(uint32_t)y,0,draw,0);return;}
    building=H(0xb4e6fcu+tile*2u);
    group=building!=0&&B(0x7999ddu+(uint32_t)building*264u)!=0?555u:554u;
    level=0;
    if(appeal>=-10)level=appeal<70?(uint32_t)(appeal/10+2):9u;
    draw(U(0xf55bb4u+group*8u)+level,x,y);
}

static int32_t resource_level(int32_t value,int construction)
{
    uint8_t raw[10];uint32_t level;
    /* Retain the exact native extended FILD/FMUL order and raw80 FTOL.
       A C double spill here changes values at quantity/color boundaries. */
    __asm { fild value }
    if(construction){
        __asm {
            mov eax,056f550h
            fmul dword ptr[eax]
        }
    }
    __asm {
        mov eax,056f558h
        fmul dword ptr[eax]
        mov eax,056f554h
        fmul dword ptr[eax]
        lea eax,raw
        fstp tbyte ptr[eax]
    }
    level=4u-(uint32_t)restore_los_trunc80(raw);
    return I(level)>9?9:I(level);
}

/* Whole434AA0, with both original resource selectors and signed quantities. */
void FUN_00434aa0(int32_t x,int32_t y,uint32_t primary,uint32_t secondary)
{
    uint32_t tile=U(0x70e488u),flags=U(0xaf598cu+tile*4u),offset,image;
    int32_t building=H(0xb4e6fcu+tile*2u),kind,value,level;
    if((flags&0x26341677u)!=0u&&!(flags&8u)){
        if(B(0xb67d1cu+tile)&64u)base_image((uint32_t)x,(uint32_t)y,B(0xb285ccu+tile)&15u,sprite,0);
        return;
    }
    if(flags&0x4100u){publish_sprite(U(0xf56b8cu)+(B(0xacf85cu+tile)&7u),(uint32_t)x,(uint32_t)y);return;}
    if(!(flags&8u)){base_image((uint32_t)x,(uint32_t)y,0,sprite,0);return;}
    offset=(uint32_t)building*264u;kind=H(0x7999e8u+offset);
    switch(kind){
    case 70:case 190:value=H(0x799a2eu+offset+secondary*2u);break;
    case 71:case 191:value=H(0x799a2eu+offset+primary*2u);break;
    case 72:case 193:value=FUN_0045d510(building,I(primary));break;
    case 73:value=FUN_0045d510(I(FUN_00475b50((uint32_t)building)),I(primary));break;
    default:
        level=B(0x7999ddu+offset)?resource_level(H(0x799a2cu+offset+secondary*2u),1):0;
        image=U(0xf56ca4u)+(uint32_t)level;publish_sprite(image,(uint32_t)x,(uint32_t)y);return;
    }
    image=U(0xf56ca4u)+(uint32_t)resource_level(value,0);
    publish_sprite(image,(uint32_t)x,(uint32_t)y);
}

static void tile_grid(uint32_t points,uint32_t colors,uint32_t end,uint32_t offset,
                      uint32_t x,uint32_t y,uint32_t tint)
{
    do{
        uint32_t image=U(0xf56bdcu)+U(colors)+tint;
        if(B(0x7999ddu+offset))image+=4u;
        publish_sprite(image,x+U(points),y+U(points+4u));
        colors+=4u;points+=8u;
    }while(colors<end);
}

static void pyramid_grid(uint32_t offset,uint32_t variant)
{
    uint32_t rows,columns,step,row,column,color,direction;
    int32_t x=H(0x7999deu+offset),y=H(0x7999e0u+offset),px,py;
    uint32_t dx=U(0x5d1ff8u+(variant>>1)*8u),dy=U(0x5d1ffcu+(variant>>1)*8u);
    switch(variant){
    case 0:rows=13;columns=7;step=1;break;
    case 2:rows=7;columns=13;step=5;break;
    case 4:rows=13;columns=7;step=5;break;
    case 6:rows=7;columns=13;step=1;break;
    default:return;
    }
    for(row=0;row<rows;++row)for(column=0;column<columns;++column){
        direction=U(0xbca2e0u);color=3;
        if(direction==0){if(row==0)color=column==0?0:1;else if(column==0)color=2;}
        else if(direction==2){if(row==rows-1)color=column==0?0:2;else if(row>=1&&column==0)color=1;}
        else if(direction==4){if(row==rows-1)color=column==columns-1?0:1;else if(column==columns-1)color=2;}
        else {if(row==0)color=column==columns-1?0:2;else if(column==columns-1)color=1;}
        FUN_004f0ba0(I(row*step+(uint32_t)x+dx),I(column*step+(uint32_t)y+dy),&px,&py);
        publish_sprite(U(0xf56bdcu)+color,(uint32_t)px,(uint32_t)py);
    }
}

/* Whole432D30: original table runs and pyramid projection, including the
   asymmetric problem-marker branches for footprints1/2 versus3/4/5. */
void FUN_00432d30(int32_t building,int32_t x,int32_t y,uint32_t tint)
{
    static const uint32_t points[6]={0,0x5d0050u,0x5d0070u,0x5d00b8u,0x5d0138u,0x5d0200u};
    static const uint32_t colors[6]={0,0x5d267cu,0x5d268cu,0x5d26b0u,0x5d26f0u,0x5d2754u};
    static const uint32_t ends[6]={0,0x5d268cu,0x5d26b0u,0x5d26f0u,0x5d2754u,0x5d27e4u};
    uint32_t root,offset,tile,footprint,px=(uint32_t)x,py=(uint32_t)y,image,direction;
    int32_t kind;int problem;
    if(building==0)return;
    if(U(0xe94e0cu)==31u)FUN_00432920(building);
    root=FUN_00475b50((uint32_t)building);
    problem=U(0x799ad8u+root*264u)!=0&&U(0xe94e0cu)==31u;
    if((uint8_t)FUN_00431690(I(root))!=0)goto original_image;
    offset=(uint32_t)building*264u;kind=H(0x7999e8u+offset);
    if(kind==209||problem)goto original_image;
    tile=U(0x70e488u);footprint=B(0xb285ccu+tile)&15u;
    if((kind==31||kind==30||kind==33)&&(U(0xaf598cu+tile*4u)&0x60u))goto original_image;
    if(kind==72){
        direction=U(0xbca2e0u);
        if(direction==0){px-=60u;py+=30u;}else if(direction==4){px-=60u;py-=30u;}else if(direction==6)px-=120u;
        tile_grid(points[2],colors[2],ends[2],offset,px,py,0);return;
    }
    if(kind==73)return;
    if(kind>=65&&kind<=69){
        if(!H(0x799a10u+offset))pyramid_grid(offset,B(0x799a82u+offset));
        return;
    }
    if(footprint){
        if(footprint>5u)return;
        if(U(0xe94e0cu)==31u){
            if(footprint<=2u){
                root=FUN_00475b50((uint32_t)building);
                /* Native EBP switches to the returned root here; subsequent
                   construction-palette reads use that root for1/2 only. */
                offset=root*264u;
                if(U(0x799ad8u+offset))goto original_image;
            }else if(U(0x799ad8u+offset)){
                (void)FUN_00475b50((uint32_t)building);goto original_image;
            }
        }
        tile_grid(points[footprint],colors[footprint],ends[footprint],offset,px,py,tint);return;
    }
    image=U(0xf56bdcu);U(0xd3b5c0u)=image;
    if(!B(0x7999ddu+offset)){
        if(U(0xe94e0cu)==31u){
            root=FUN_00475b50((uint32_t)building);
            if(U(0x799ad8u+root*264u)){
                publish_sprite(U(0xb7482cu+U(0x70e488u)*4u),px,py);return;
            }
        }
    }else if(U(0xe94e0cu)==31u&&B(0x799a9bu+offset))image=U(0xb7482cu+U(0x70e488u)*4u);
    else image=U(0xf56bdcu)+4u;
    publish_sprite(image+tint,px,py);return;
original_image:
    tile=U(0x70e488u);base_image(px,py,B(0xb285ccu+tile)&15u,sprite,1);
}
