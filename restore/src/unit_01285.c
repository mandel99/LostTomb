#include <stdint.h>
#define DAT_005cc888 (*(uint8_t *)0x005cc888u)
#define DAT_005cc890 (*(uint8_t *)0x005cc890u)
#define DAT_005cc8b8 (*(uint8_t *)0x005cc8b8u)
#define DAT_005cc8c0 (*(int32_t *)0x005cc8c0u)
#define DAT_0070e448 (*(int32_t *)0x0070e448u)
#define DAT_0070e44c (*(int32_t *)0x0070e44cu)
#define DAT_0070e45c (*(int32_t *)0x0070e45cu)
#define DAT_0078d79c (*(int32_t *)0x0078d79cu)
#define DAT_0078d7c4 (*(uint32_t *)0x0078d7c4u)
#define DAT_0078d7cc (*(int32_t *)0x0078d7ccu)
#define DAT_007999db (*(uint8_t *)0x007999dbu)
#define DAT_007999e0 (*(int16_t *)0x007999e0u)
#define DAT_00799a18 (*(int16_t *)0x00799a18u)
#define DAT_00acf85c (*(uint8_t *)0x00acf85cu)
#define DAT_00adc36c (*(uint8_t *)0x00adc36cu)
#define DAT_00bca2d8 (*(uint32_t *)0x00bca2d8u)
#define DAT_00bca2e0 (*(uint32_t *)0x00bca2e0u)
#define DAT_00bca34c (*(int32_t *)0x00bca34cu)
#define DAT_00d386d4 (*(int32_t *)0x00d386d4u)
#define DAT_00f5609c (*(int32_t *)0x00f5609cu)
#define DAT_00f560a4 (*(int32_t *)0x00f560a4u)
#define DAT_00f560ac (*(int32_t *)0x00f560acu)
#define DAT_00f56544 (*(int32_t *)0x00f56544u)
#define DAT_010d6a0e (*(int16_t *)0x010d6a0eu)
#define DAT_010d6a1a (*(int16_t *)0x010d6a1au)
#define DAT_010d6a1c (*(int16_t *)0x010d6a1cu)
#define DAT_0126517c (*(int32_t *)0x0126517cu)

/* Native 488840: draw the nine-tile building footprint and its terrain overlay.
   Typed C callers use cdecl; the original six-argument entry returns with RET24.
   Sprite calls use the existing global-receiver C facade. */
extern void FUN_0047bf50(uint16_t,int32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern int32_t FUN_00489050(int32_t,int32_t,int32_t,int32_t);
extern int FUN_0053e590(int,int,int);
extern int64_t FUN_0056133c(void);

void FUN_00488840(int32_t building_id,int32_t screen_x,int32_t screen_y,
                  int32_t base_sprite,int32_t render_value,uint32_t detail)
{
    static const int8_t points[9][2]={
        {0,-2},{1,-1},{2,0},{-1,-1},{0,0},{1,1},{-2,0},{-1,1},{0,2}
    };
    uint32_t record_bytes=(uint32_t)building_id*264u;
    uint32_t record_words=(uint32_t)building_id*132u;
    uint32_t tile,terrain_type,overlay_mode=0,overlay_point=UINT32_MAX,point;
    int32_t scaled_value,ratio_denominator,frame_level,converted_ten;

    if((uint8_t)detail==0){
        int32_t image_base=DAT_00f56544;
        int32_t area_value=FUN_00489050(screen_x,screen_y,3,3);
        FUN_0047bf50((uint16_t)building_id,screen_x,screen_y,3,3,
                     (uint32_t)(area_value/14)+(uint32_t)image_base,8);
        return;
    }

    /* X is the signed WORD immediately before the Y bank at7999E0. */
    tile=DAT_00bca2d8+(uint32_t)(&DAT_007999e0)[record_words-1u]+
         (uint32_t)((&DAT_007999e0)[record_words]*228);
    switch(DAT_00bca2e0){
    case 0:
        terrain_type=(&DAT_007999db)[record_bytes];
        tile+=*(uint32_t *)(&DAT_005cc890+terrain_type*48u);break;
    case 2:tile+=(uint32_t)DAT_005cc8c0;break;
    case 4:
        terrain_type=(&DAT_007999db)[record_bytes];
        tile+=*(uint32_t *)(&DAT_005cc8b8+terrain_type*8u);break;
    case 6:
        terrain_type=(&DAT_007999db)[record_bytes];
        tile+=*(uint32_t *)(&DAT_005cc888+terrain_type*56u);break;
    default:tile=0;break;
    }
    terrain_type=(&DAT_00adc36c)[tile];

    /* Native 48893C/48894D: supply ST0 to the genuine truncating helper.
       No C floating-point intermediate changes inherited x87 PC/RC. */
    __asm { fld qword ptr ds:[0056f5a8h] }
    converted_ten=(int32_t)FUN_0056133c();
    scaled_value=(int32_t)((uint32_t)converted_ten*(uint32_t)render_value);
    if(terrain_type!=0 && (&DAT_00799a18)[record_words]>0){
        if(scaled_value<DAT_00bca34c/5){
            overlay_mode=1;
            overlay_point=4;
        }else if(DAT_00d386d4==1){
            overlay_mode=3;
            overlay_point=(&DAT_00acf85c)[tile+DAT_0078d7c4]%7u;
        }else{
            overlay_mode=2;
            ratio_denominator=(int32_t)((uint32_t)DAT_00bca34c+1u);
            /* Native 4889B9..4889D6: preserve both FILDs and FDIVP/FMUL. */
            __asm { fild scaled_value }
            __asm { fild ratio_denominator }
            __asm { fdivp st(1),st(0) }
            __asm { fmul qword ptr ds:[0056f658h] }
            overlay_point=(uint32_t)FUN_0056133c();
        }
    }

    frame_level=render_value/10;
    for(point=0;point<9u;++point){
        int32_t layer=frame_level<5?frame_level:5;
        int32_t image=(int32_t)((uint32_t)base_sprite+(uint32_t)layer);
        int32_t descriptor=image<DAT_0126517c?image:0;
        int32_t x=(int32_t)((uint32_t)points[point][0]*(uint32_t)DAT_0070e45c+
                           (uint32_t)screen_x);
        int32_t y=(int32_t)((uint32_t)points[point][1]*(uint32_t)DAT_0070e44c+
                           (uint32_t)screen_y+(uint32_t)DAT_0070e448*2u-
                           (uint32_t)(&DAT_010d6a0e)[(uint32_t)descriptor*32u]);
        (void)FUN_0053e590(image,x,y);
        frame_level-=2;
        if(frame_level<0)frame_level=0;
        if(overlay_mode!=0 && overlay_point==point && DAT_0078d79c<=0){
            uint32_t phase;
            int32_t overlay_base,overlay_image,overlay_descriptor;
            if(overlay_mode==1){
                phase=((uint32_t)(DAT_0078d7cc>>3)+(uint32_t)building_id)&7u;
                overlay_base=DAT_00f560a4;
            }else if(overlay_mode==2){
                phase=((&DAT_00acf85c)[tile]+point)%7u;
                overlay_base=DAT_00f5609c;
            }else{
                phase=(&DAT_00acf85c)[tile]%7u;
                overlay_base=DAT_00f560ac;
            }
            overlay_image=(int32_t)((uint32_t)overlay_base+phase-8u+terrain_type*8u);
            overlay_descriptor=overlay_image<DAT_0126517c?overlay_image:0;
            (void)FUN_0053e590(overlay_image,
                (int32_t)((uint32_t)x+(uint32_t)DAT_0070e45c-
                  (uint32_t)(&DAT_010d6a1a)[(uint32_t)overlay_descriptor*32u]),
                (int32_t)((uint32_t)y+(uint32_t)DAT_0070e44c*3u-5u-
                  (uint32_t)(&DAT_010d6a1c)[(uint32_t)overlay_descriptor*32u]));
        }
    }
}
