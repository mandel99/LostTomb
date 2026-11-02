#include "menu_keyboard_modal.h"
#include <stdio.h>
extern int FUN_004cbf30(const char *text,int font);
extern void FUN_00536820(int x,int y,int width,int height);
extern void FUN_004d65b0(int x,int y,int width,int height,int screen_x,int screen_y);
extern void FUN_004cc720(uint32_t text,int x,int y,int width,uint32_t font,int flags);
extern void FUN_004d6250(int group,int label,int x,int y,int width,uint32_t font,int flags,int screen_x,int screen_y);
extern void FUN_004d63d0(int kind,int x,int y,int width,int height,uint32_t color,int screen_x,int screen_y);
static int add(int a,int b){return (int)((uint32_t)a+(uint32_t)b);}
static int sub(int a,int b){return (int)((uint32_t)a-(uint32_t)b);}
int FUN_00538940(uint32_t title,uint32_t body,uint32_t flags)
{
    int mid_x=MM_I(0xe38db0)/2,mid_y=MM_I(0xe38dac)/2;
    uint32_t title_font=MM_U(0xf56fb4);int body_width,title_width,width,span,left,top,units,count,i;
    int labels[3]={0,0,0},results[3]={0,0,0};
    body_width=FUN_004cbf30((const char *)(uintptr_t)body,MM_I(0xf56f8c));
    title_width=FUN_004cbf30((const char *)(uintptr_t)title,(int)title_font);
    if(title_width>body_width)width=FUN_004cbf30((const char *)(uintptr_t)title,MM_I(0xf56fb4));
    else width=FUN_004cbf30((const char *)(uintptr_t)body,MM_I(0xf56f8c));
    span=add(width,20);left=sub(mid_x,span/2);top=add(mid_y,-50);units=span/16;
    FUN_00536820(left,top,add(units,1),7);
    FUN_004d65b0(add(left,8),add(top,5),units,2,MM_I(0xe38da8),MM_I(0xe38da4));
    switch(flags&15u){
    case 1:count=2;labels[0]=3;labels[1]=2;results[0]=1;results[1]=2;break;
    case 2:count=3;labels[0]=10;labels[1]=9;labels[2]=11;results[0]=3;results[1]=4;results[2]=5;break;
    case 3:count=3;labels[0]=1;labels[1]=0;labels[2]=2;results[0]=6;results[1]=7;results[2]=2;break;
    case 4:count=2;labels[0]=1;labels[1]=0;results[0]=6;results[1]=7;break;
    case 5:count=2;labels[0]=9;labels[1]=2;results[0]=4;results[1]=2;break;
    default:count=1;labels[0]=3;results[0]=1;break;
    }
    MM_I(0xf535c8)=count;
    FUN_004cc720(title,add(left,16),add(top,10),span,MM_U(0xf56fbc),0);
    FUN_004cc720(body,add(left,5),add(top,48),add(span,-10),MM_U(0xf56f8c),0);
    {int button_width=span/3,button_y=add(top,70),height=add(sub(top,add(top,70)),95);
     int button_x=sub(mid_x,(int)((uint32_t)(button_width/2)*(uint32_t)count));
     for(i=0;i<count;++i){
        RestoreMenuButton *b=(RestoreMenuButton *)(uintptr_t)(0x5f8910u+(uint32_t)i*28u);
        FUN_004d6250(0x12,labels[i],button_x,add(button_y,8),button_width,MM_U(0xf56f8c),0,MM_I(0xe38da8),MM_I(0xe38da4));
        FUN_004d63d0(0,button_x,button_y,button_width,height,MM_I(0x7056c8)==i+1?0xf800u:0,MM_I(0xe38da8),MM_I(0xe38da4));
        b->left=(int16_t)button_x;b->top=(int16_t)button_y;
        b->right=(int16_t)add(button_x,button_width);b->bottom=(int16_t)add(button_y,height);
        button_x=add(button_x,button_width);
     }}
    if(!MM_B(0xf535c0))return -1;
    {uint32_t index=MM_U(0xf535c4);
     if(index>=(uint32_t)count){fprintf(stderr,"Invalid native modal selection %lu of %d.\n",(unsigned long)index,count);ExitProcess(3);}
     return results[index];}
}
