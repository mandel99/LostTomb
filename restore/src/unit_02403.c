#include "chief_storage_menu.h"
#include "campaign_score_numeric.h"
#include "menu_building_overlay.h"
#include "menu_training_render.h"
#include "monument_base_queries.h"
#include "monument_complex_queries.h"
#include "monument_progress.h"
#include "monument_unlink.h"
#include "pyramid_summary.h"
#include "sprite_render.h"
#include <stdint.h>

extern int32_t FUN_0040cc40(int32_t index);
extern void FUN_004cd1a0(uint32_t kind,int16_t *point,uint32_t color,int32_t dx,int32_t dy);
extern void FUN_004d62f0(uint32_t group,uint32_t text,int32_t x,int32_t y,
                        uint32_t font,uint32_t color,int32_t origin_x,int32_t origin_y);
extern void FUN_004d6860(uint32_t value,uint32_t separator,const char *suffix,
                        int32_t x,int32_t y,uint32_t font,uint32_t color,
                        int32_t origin_x,int32_t origin_y);

#define STORAGE_U32(address) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(address))
#define STORAGE_U16(address) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(address))
#define STORAGE_U8(address) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(address))

static int16_t *storage_row_point(uint32_t row)
{
    return (int16_t *)(uintptr_t)(0x005c5798u+row*0x1cu);
}

static uint32_t storage_city_offset(void)
{
    return STORAGE_U32(0x00eaac28u)*0x49d8u;
}

/* Tables 0052C4B4 and 0052C4DC/C4F0/C504/C518, decoded from the
 * pinned native ROM. The latter four maps are byte-identical. */
static int storage_resource_visible(uint32_t resource)
{
    switch(resource) {
    case 1:case 8:case 10:case 13:case 15:case 17:case 18:case 19:
    case 20:case 23:case 24:case 25:case 26:case 28:case 30:return 1;
    default:return 0;
    }
}

static int storage_resource_counts_units(uint32_t resource)
{
    switch(resource) {
    case 24:case 25:case 26:case 30:case 35:return 1;
    default:return 0;
    }
}

static void storage_resource_text(uint32_t group,uint32_t text,uint32_t x,uint32_t y)
{
    FUN_004d62f0(group,text,(int32_t)x,(int32_t)y,STORAGE_U32(0x00f56f9cu),0,
                (int32_t)STORAGE_U32(0x00e38da8u),(int32_t)STORAGE_U32(0x00e38da4u));
}

static void storage_resource_number(uint32_t value,uint32_t x,uint32_t y)
{
    FUN_004d6860(value,0x40u,(const char *)(uintptr_t)0x0057c048u,(int32_t)x,
                (int32_t)y,STORAGE_U32(0x00f56f9cu),0,
                (int32_t)STORAGE_U32(0x00e38da8u),(int32_t)STORAGE_U32(0x00e38da4u));
}

static void storage_draw_resources(void)
{
    uint32_t resource,row=0,y_offset=0;
    for(resource=0;resource<36u;++resource) {
        uint32_t current,required,unit_text;
        int count_units=storage_resource_counts_units(resource);
        uint32_t total_address=0x00784de8u+resource*4u;
        if(!storage_resource_visible(resource)||(int32_t)STORAGE_U32(total_address)<=0)
            continue;
        STORAGE_U32(0x00e20778u)=0;
        (void)restore_sprite_draw((RestoreSpriteLibraries *)(uintptr_t)0x00f55bb0u,
            (int32_t)(STORAGE_U32(0x00f571fcu)+resource),
            (int32_t)(STORAGE_U32(0x00f53f0cu)+0x28u),
            (int32_t)(STORAGE_U32(0x00f53f08u)+y_offset+0x42u));
        current=STORAGE_U32(total_address)-STORAGE_U32(total_address+0x90u);
        if(!count_units&&resource!=28u&&resource!=10u&&resource!=36u)
            current*=100u;
        FUN_004cd1a0(0,storage_row_point(row),
                     STORAGE_U32(0x007056c8u)==row+1u,0,0);
        STORAGE_U32(0x00e20778u)=0;
        if(current==0)
            storage_resource_text(199u,2u,STORAGE_U32(0x00f53f0cu)+0x3cu,
                                  STORAGE_U32(0x00f53f08u)+y_offset+0x42u);
        else
            storage_resource_number(current,STORAGE_U32(0x00f53f0cu)+0x3cu,
                                    STORAGE_U32(0x00f53f08u)+y_offset+0x42u);
        unit_text=resource;
        if(count_units||((resource==28u||resource==10u)&&(int32_t)current>1))
            unit_text=resource+0x36u;
        storage_resource_text(23u,unit_text,
            STORAGE_U32(0x00f53f0cu)+STORAGE_U32(0x00e20778u)+0x3cu,
            STORAGE_U32(0x00f53f08u)+y_offset+0x42u);
        STORAGE_U32(0x00e20778u)=0;
        required=STORAGE_U32(0x00ea6160u+storage_city_offset()+resource*4u);
        if(count_units||resource==28u||resource==10u||resource==36u)
            required=(uint32_t)((int32_t)required/100);
        storage_resource_number(required,STORAGE_U32(0x00f53f0cu)+0x12cu,
                                STORAGE_U32(0x00f53f08u)+y_offset+0x42u);
        storage_resource_text(199u,count_units?(required==1u?0x39u:0x3au):1u,
            STORAGE_U32(0x00f53f0cu)+STORAGE_U32(0x00e20778u)+0x12cu,
            STORAGE_U32(0x00f53f08u)+y_offset+0x42u);
        ++row;
        y_offset+=0x2au;
    }
}

static void storage_highlight_incomplete(uint32_t row)
{
    if(STORAGE_U32(0x007056c8u)==row+1u)
        FUN_004cd1a0(0,storage_row_point(row),0xf800u,0,0);
}

static void storage_selected_text(uint32_t text,const int16_t *point,uint32_t font,
                                  uint32_t dx,int32_t dy)
{
    FUN_004d6200(199u,text,point,font,0,(int32_t)dx,dy,
                 (int32_t)STORAGE_U32(0x00e38da8u),(int32_t)STORAGE_U32(0x00e38da4u));
}

static void storage_draw_selected_row(uint32_t row,uint32_t title,uint32_t subtitle,
                                     int32_t percent,uint32_t cost,uint32_t supplied)
{
    const int16_t *point=storage_row_point(row);
    uint32_t font=STORAGE_U32(0x00f56f9cu);
    STORAGE_U32(0x00e20778u)=0;
    STORAGE_U32(0x00e20774u)=0;
    storage_selected_text(title,point,font,8u,8);
    if(percent<100) {
        FUN_004cd290((uint32_t)percent,0x40u,(const char *)(uintptr_t)0x005e03fcu,
                     point,font,0,(int32_t)(STORAGE_U32(0x00e20778u)+8u),8);
        storage_selected_text(2u,point,font,STORAGE_U32(0x00e20778u)+8u,8);
    }
    if(cost!=0)
        FUN_004cd290((uint32_t)((int32_t)cost/100),0x40u,
            (const char *)(uintptr_t)0x0057c048u,point,font,0,
            (int32_t)(STORAGE_U32(0x00e20778u)+8u),8);
    STORAGE_U32(0x00e20778u)=0;
    if(subtitle!=0)
        storage_selected_text(subtitle,point,font,8u,0x18);
    if(cost!=0) {
        FUN_004cd290((uint32_t)((int32_t)supplied/100),0x40u,
            (const char *)(uintptr_t)0x0057c048u,point,font,0,
            (int32_t)(STORAGE_U32(0x00e20778u)+8u),0x18);
        storage_selected_text(supplied==100u?0x39u:0x3au,point,font,
                              STORAGE_U32(0x00e20778u)+8u,0x18);
    }
}

/* Original 0052B6D0: two drawing phases. All arithmetic used for mapped
 * addresses and quantities is modulo 2^32; native comparisons/division are
 * explicitly signed. Existing providers own every child algorithm. */
void restore_chief_storage_menu_draw(void)
{
    uint32_t slot,row=5;
    if(FUN_0040cc40(0)!=0)
        storage_draw_resources();
    FUN_004d65b0((int32_t)(STORAGE_U32(0x00f53f0cu)+0x26u),
                 (int32_t)(STORAGE_U32(0x00f53f08u)+0x11au),0x23,8,
                 (int32_t)STORAGE_U32(0x00e38da8u),(int32_t)STORAGE_U32(0x00e38da4u));
    for(slot=0;slot<3u;++slot) {
        int32_t selected=(int16_t)STORAGE_U16(0x00784de0u+slot*2u);
        uint32_t descriptor,city,entry,title=0,subtitle=0,cost=0,supplied=0;
        int32_t type,percent=100;
        if(selected<=0)continue;
        descriptor=0x005d19c8u+(uint32_t)selected*16u;
        type=(int16_t)STORAGE_U16(descriptor+2u);
        if(STORAGE_U16(descriptor)==1u) {
            int32_t summary[28];
            uint32_t base;
            /* Native table 0052C524 is deliberately nonsequential. */
            switch(type) {
            case 1:base=15u;break;
            case 2:base=5u;break;
            case 3:base=10u;break;
            case 4:base=0u;break;
            default:base=20u;break;
            }
            entry=storage_city_offset()+slot*4u;
            if(STORAGE_U32(0x00ea5fa0u+entry)==0) {
                title=base+14u;subtitle=base+15u;
            } else {
                (void)FUN_004f1310(STORAGE_U32(0x00ea5facu+entry),summary);
                percent=summary[13];
                if(percent<100) {
                    title=base+16u;subtitle=13u;
                    storage_highlight_incomplete(row);
                } else {
                    percent=100;
                    city=storage_city_offset();
                    title=base+((STORAGE_U32(0x00ea6090u+city)==0&&
                                  (int32_t)STORAGE_U32(0x00ea6098u+city)>=12)?17u:18u);
                }
            }
        } else {
            entry=storage_city_offset()+slot*4u;
            switch(type) {
            case 1:
                if(STORAGE_U32(0x00ea5fa0u+entry)==0) {
                    title=0x27u;subtitle=0x28u;
                } else {
                    city=storage_city_offset();
                    percent=((int32_t)(int16_t)STORAGE_U16(0x00ea5baau+city)*100)/16;
                    if(percent<100) {
                        title=0x29u;subtitle=13u;storage_highlight_incomplete(row);
                    } else {percent=100;title=0x2au;}
                }
                break;
            case 2:
                if(STORAGE_U32(0x00ea5fa0u+entry)==0) {
                    title=0x2bu;subtitle=0x2cu;
                    cost=FUN_004f52b0((int32_t)slot)==0xcf?10000u:20000u;
                    supplied=STORAGE_U32(0x00ea61c8u+storage_city_offset());
                } else {
                    uint32_t building=STORAGE_U32(0x00ea5facu+entry);
                    percent=STORAGE_U8(0x00799a2du+building*264u);
                    if(percent<100) {
                        title=0x2du;subtitle=13u;storage_highlight_incomplete(row);
                    } else {percent=100;title=0x2eu;}
                }
                break;
            case 3:
                if(STORAGE_U32(0x00ea5fa0u+entry)==0) {
                    title=0x2fu;subtitle=0x30u;cost=22000u;
                    supplied=STORAGE_U32(0x00ea61d8u+storage_city_offset());
                } else {
                    percent=FUN_00542ad0(STORAGE_U32(0x00ea5facu+entry),0);
                    if(percent<100) {
                        title=0x31u;subtitle=13u;storage_highlight_incomplete(row);
                    } else {percent=100;title=0x32u;}
                }
                break;
            case 5:case 6:case 7:case 10:case 11:case 12:case 13:case 14:
                /* These cases share the native absent/incomplete/complete
                 * labels; receiver constants follow each native MOV ECX. */
                switch(type) {
                case 5:title=0x43u;break;
                case 6:title=0x3bu;break;
                case 7:title=0x3fu;break;
                case 10:title=0x4bu;break;
                case 11:title=0x4fu;break;
                case 12:title=0x53u;break;
                case 13:title=0x57u;break;
                default:title=0x47u;break;
                }
                if(STORAGE_U32(0x00ea5fa0u+entry)==0) {
                    subtitle=title+1u;
                } else {
                    uint32_t building=STORAGE_U32(0x00ea5facu+entry);
                    switch(type) {
                    case 5:percent=restore_monument_base_004e6710((void *)(uintptr_t)0x006de548u,building,0);break;
                    case 6:percent=restore_monument_base_0040d940((void *)(uintptr_t)0x006fd5d8u,building,0);break;
                    case 7:percent=restore_monument_base_00417050((void *)(uintptr_t)0x006fd368u,building,0);break;
                    case 10:percent=restore_monument_progress_00545740((void *)(uintptr_t)0x006f9320u,building,0);break;
                    case 11:percent=restore_monument_progress_00558be0((void *)(uintptr_t)0x006f5648u,building,0);break;
                    case 12:percent=restore_monument_progress_00555e80((void *)(uintptr_t)0x006de9e0u,building,0);break;
                    case 13:percent=restore_monument_progress_0054f470((void *)(uintptr_t)0x006e9188u,building,0);break;
                    default:percent=FUN_00401540(building,0);break;
                    }
                    if(percent<100) {
                        title+=2u;subtitle=13u;storage_highlight_incomplete(row);
                    } else {percent=100;title+=3u;}
                }
                break;
            default:
                if(STORAGE_U32(0x00ea5fa0u+entry)==0) {
                    title=0x33u;subtitle=0x34u;cost=24000u;
                    supplied=STORAGE_U32(0x00ea61d8u+storage_city_offset());
                } else {
                    percent=FUN_004df040(STORAGE_U32(0x00ea5facu+entry),0);
                    if(percent<100) {title=0x35u;subtitle=13u;}
                    else {
                        percent=100;city=storage_city_offset();
                        title=STORAGE_U32(0x00ea6090u+city)==0&&
                            (int32_t)STORAGE_U32(0x00ea6098u+city)>=12?0x36u:0x37u;
                    }
                }
                break;
            }
        }
        storage_draw_selected_row(row,title,subtitle,percent,cost,supplied);
        ++row;
    }
}

void FUN_0052b6d0(void)
{
    restore_chief_storage_menu_draw();
}
