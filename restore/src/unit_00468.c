#include "viewport_controller.h"

void FUN_00430d30(uint32_t camera_x,uint32_t camera_y,uint32_t camera_mode)
{
    uint32_t clip_left,clip_right,clip_top,clip_bottom,now,previous,delta,mode,counter;
    uint32_t screen_width,screen_height;
    int overlay_pass;

    /* Preserve the original interleaving of fixed-address reads and writes. */
    viewport_write32(0x0070e478u,camera_x);
    clip_left=viewport_read32(0x0070e43cu);
    viewport_write32(0x0070e474u,camera_y);
    clip_right=viewport_read32(0x0070e438u);
    viewport_write32(0x0070e484u,camera_mode);
    clip_top=viewport_read32(0x0070e434u);
    viewport_write32(0x00d3b55cu,clip_left);
    clip_bottom=viewport_read32(0x0070e430u);
    viewport_write32(0x00d3b558u,clip_right);
    now=viewport_read32(0x00e92e0cu);
    viewport_write32(0x00d3b554u,clip_top);
    previous=viewport_read32(0x0070e3c0u);
    viewport_write32(0x00d3b550u,clip_bottom);
    viewport_write32(0x0070e3c4u,0);

    /* Native SUB wraps at 32 bits; signed JG/JGE interpret the result. */
    delta=now-previous;
    if((delta&0x80000000u)!=0 || delta>60u){
        viewport_write32(0x0070e3c0u,now);
        viewport_write32(0x0070e3c4u,1);
    }

    mode=viewport_read32(0x00e94e0cu);
    if(mode==0){
        overlay_pass=0;
        if(viewport_read8(0x006de515u)==0)FUN_00430ef0();
        else FUN_004316f0();
    }else{
        /* Mode51 skips the byte read. The branch is fixed before deep calls. */
        overlay_pass=mode!=51 && viewport_read8(0x006de515u)==0;
        FUN_004316f0();
    }
    if(overlay_pass){
        FUN_0051f780();
        FUN_00439f10();
        FUN_0043b630();
        FUN_00439740(9999);
    }else{
        FUN_00436b60(0);
        FUN_0051f780();
        FUN_0043b630();
        FUN_00439740(0);
    }
    FUN_0043e300();

    /* These values are loaded after all passes, so endpoint mutations apply. */
    counter=viewport_read32(0x00775610u);
    if(counter!=0)viewport_write32(0x00775610u,counter-1u);
    screen_width=viewport_read32(0x00e38db0u);
    screen_height=viewport_read32(0x00e38dacu);
    viewport_write32(0x00d3b55cu,0);
    viewport_write32(0x00d3b558u,screen_width);
    viewport_write32(0x00d3b554u,0);
    viewport_write32(0x00d3b550u,screen_height);
}
