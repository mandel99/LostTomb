#include "startup_probe.h"
#include "startup_trace.h"
#include "window_messages.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
static unsigned int name_character;
static unsigned int family_dialog_stage;
static unsigned int mission_back;
static unsigned int load_save_stage;
static unsigned int high_scores_stage;
static unsigned int message_dismiss_stage;
RestoreProbeFrame restore_probe_frame;
RestoreProbeFrame restore_probe_menu_frame;
RestoreProbeFrame restore_probe_family_frame;
RestoreProbeFrame restore_probe_dialog_frame;
RestoreProbeFrame restore_probe_return_frame;
RestoreProbeFrame restore_probe_campaign_frame;
RestoreProbeFrame restore_probe_mission_frame;
RestoreProbeFrame restore_probe_load_save_frame;
RestoreProbeFrame restore_probe_load_save_selected_frame;
RestoreProbeFrame restore_probe_high_scores_frame;
RestoreProbeFrame restore_probe_high_scores_scrolled_frame;
RestoreProbeFrame restore_probe_high_scores_dragged_frame;
RestoreProbeFrame restore_probe_high_scores_return_frame;
RestoreProbeFrame restore_probe_city_frame;
RestoreProbeFrame restore_probe_gameplay_frame;
uint32_t restore_probe_high_scores_state[4][2];
/* Publish immutable completed frames only during optional menu diagnostics. */
static void copy_frame(RestoreProbeFrame *frame,uint32_t mode)
{
    uint32_t width,height,pitch;
    void *pixels;
    if(frame->ready)return;
    width=*(uint32_t *)0xe38d9cu;height=*(uint32_t *)0xe38d98u;pitch=*(uint32_t *)0xe38d94u;
    pixels=*(void **)0xe92e18u;
    if(!pixels||!width||width>4096||!height||height>4096||pitch<width*2||pitch>16384) return;
    frame->pixels=malloc(pitch*height);
    if(!frame->pixels) return;
    memcpy(frame->pixels,pixels,pitch*height);
    frame->magic=0x52504652;frame->width=width;frame->height=height;
    frame->pitch=pitch;frame->format=*(uint32_t *)0xe3914cu;frame->mode=mode;
    MemoryBarrier();frame->ready=1;
    restore_startup_trace("STARTUP diagnostic completed frame copied after rendering: mode=%u\n",mode);
}
static void copy_scores_frame(RestoreProbeFrame *frame,unsigned slot,uint32_t mode)
{
    if(frame->ready)return;
    restore_probe_high_scores_state[slot][0]=*(uint32_t *)0xd3a4bcu;
    restore_probe_high_scores_state[slot][1]=*(uint32_t *)0xd3a4b8u;
    copy_frame(frame,mode);
}
void restore_startup_probe_frame_complete(void)
{
    static unsigned int dialog_frames;
    static unsigned int load_save_selected_frames;
    static unsigned int city_frames;
    static unsigned int gameplay_frames;
    char enabled[8];uint32_t mode=*(uint8_t *)0xe38ddau;
    int select=GetEnvironmentVariableA("RESTORE_FAMILY_SELECT_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1';
    int submit=GetEnvironmentVariableA("RESTORE_MENU_SUBMIT_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1';
    int dialog=GetEnvironmentVariableA("RESTORE_FAMILY_DIALOG_TEST",enabled,sizeof(enabled))==1&&enabled[0]>='1'&&enabled[0]<='3';
    /* A renderer can open mode5 during its last calls. Skip that transition
       frame and copy only a subsequent completed city/modal rendering. */
    if(GetEnvironmentVariableA("RESTORE_LOAD_SAVE_CONFIRM_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1'&&load_save_stage==3&&(mode==1||mode==5)&&city_frames++>=1)
        copy_frame(&restore_probe_city_frame,mode);
    if(GetEnvironmentVariableA("RESTORE_MESSAGE_DISMISS_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1'&&
       message_dismiss_stage>=1&&mode==1&&!restore_probe_gameplay_frame.ready&&gameplay_frames++>=1) {
        copy_frame(&restore_probe_gameplay_frame,mode);
        if(restore_probe_gameplay_frame.ready)
            restore_startup_trace("STARTUP diagnostic message: completed gameplay frame copied after OK\n");
    }
    if(GetEnvironmentVariableA("RESTORE_HIGH_SCORES_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1') {
        if(mode==83&&high_scores_stage==0)copy_scores_frame(&restore_probe_high_scores_frame,0,mode);
        if(mode==83&&high_scores_stage==1&&*(int32_t *)0xd3a4b8u==1)
            copy_scores_frame(&restore_probe_high_scores_scrolled_frame,1,mode);
        if(mode==83&&high_scores_stage==2&&*(int32_t *)0xd3a4b8u==*(int32_t *)0xd3a4bcu-20)
            copy_scores_frame(&restore_probe_high_scores_dragged_frame,2,mode);
        if(mode==0&&high_scores_stage==3)copy_scores_frame(&restore_probe_high_scores_return_frame,3,mode);
    }
    if(name_character==7&&mode==0x36)copy_frame(&restore_probe_frame,mode);
    if((select||dialog)&&mode==0x2f)copy_frame(&restore_probe_family_frame,mode);
    if((select||submit)&&mode==0x30)copy_frame(&restore_probe_menu_frame,mode);
    if(GetEnvironmentVariableA("RESTORE_FAMILY_ACTION_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='0'&&mode==23)
        copy_frame(&restore_probe_campaign_frame,mode);
    if(GetEnvironmentVariableA("RESTORE_FAMILY_ACTION_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='3') {
        if(mode==61)copy_frame(&restore_probe_mission_frame,mode);
        if(mission_back&&mode==48)copy_frame(&restore_probe_return_frame,mode);
    }
    if(GetEnvironmentVariableA("RESTORE_FAMILY_ACTION_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='2') {
        if(mode==13&&load_save_stage==0)copy_frame(&restore_probe_load_save_frame,mode);
        /* Input is dispatched after this hook. Skip the frame preceding the
           row callback, then retain the first actual rendering of its text. */
        if(mode==13&&load_save_stage==1&&load_save_selected_frames++>=1)copy_frame(&restore_probe_load_save_selected_frame,mode);
        if(mode==48&&load_save_stage==2)copy_frame(&restore_probe_return_frame,mode);
    }
    /* Modal input marks icon redraw after its first frame. Capture the next
       completed frame so the one-time diagnostic includes the real buttons. */
    if(dialog&&mode==0x3f&&dialog_frames++>=1)copy_frame(&restore_probe_dialog_frame,mode);
    if(dialog&&family_dialog_stage==2&&(mode==0x2f||mode==0x36))copy_frame(&restore_probe_return_frame,mode);
}
static void test_click(int x,int y)
{
    *(int32_t *)0xe38e34u=x;*(int32_t *)0xe38e30u=y;
    *(uint8_t *)0xe38e00u=1;*(uint8_t *)0xe38e03u=1;
    *(uint8_t *)0xe38e02u=0;*(uint8_t *)0xe38e04u=0;
    *(uint8_t *)0xe38df4u=0;*(uint8_t *)0xe38df7u=0;*(uint8_t *)0xe38df8u=0;
}
/* Optional integration-test input after a real title frame. It exercises the
   recovered input dispatcher without sending any desktop/window input. */
void restore_startup_probe_input(void)
{
    static unsigned int title_frames,menu_frames;static int replayed,menu_replayed;
    static unsigned int name_frames;
    static int name_submitted;
    static int family_selected;
    static int family_action;
    char enabled[8];
    if(GetEnvironmentVariableA("RESTORE_MESSAGE_DISMISS_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1') {
        if(message_dismiss_stage==0&&restore_probe_city_frame.ready&&*(uint8_t *)0xe38ddau==5) {
            uint32_t record=0xe209b8u+*(uint32_t *)0xd3b124u*80u;
            int32_t columns=*(int16_t *)(uintptr_t)(record+10u),rows=*(int16_t *)(uintptr_t)(record+12u);
            /* Center of the native24x24 OK button, using the current layout. */
            test_click(*(int32_t *)0xd3b160u+columns*16-26,*(int32_t *)0xd3b15cu+rows*16-24);
            message_dismiss_stage=1;
            restore_startup_trace("STARTUP diagnostic message: dismiss OK through C small buttons\n");return;
        }
        if(message_dismiss_stage>=1) {
            *(uint8_t *)0xe38e00u=0;*(uint8_t *)0xe38e03u=0;*(uint8_t *)0xe38e04u=0;*(uint8_t *)0xe38df8u=0;
            if(message_dismiss_stage==1&&restore_probe_gameplay_frame.ready) {
                message_dismiss_stage=2;
                restore_startup_trace("STARTUP diagnostic message: returned to gameplay through C input\n");
            }
        }
    }
    if(GetEnvironmentVariableA("RESTORE_HIGH_SCORES_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1') {
        int32_t x=*(int32_t *)0xe38da8u,y=*(int32_t *)0xe38da4u;
        if(high_scores_stage==0&&restore_probe_high_scores_frame.ready&&*(uint8_t *)0xe38ddau==83) {
            test_click(x+593,y+441);high_scores_stage=1;
            restore_startup_trace("STARTUP diagnostic high scores: down through C small buttons\n");return;
        }
        if(high_scores_stage==1&&restore_probe_high_scores_scrolled_frame.ready&&*(uint8_t *)0xe38ddau==83) {
            test_click(x+584,y+401);high_scores_stage=2;
            restore_startup_trace("STARTUP diagnostic high scores: drag to bottom through C input\n");return;
        }
        if(high_scores_stage==2&&restore_probe_high_scores_dragged_frame.ready&&*(uint8_t *)0xe38ddau==83) {
            *(uint8_t *)0xe38e00u=0;*(uint8_t *)0xe38e03u=0;*(uint8_t *)0xe38e04u=0;
            *(uint8_t *)0xe38df8u=1;high_scores_stage=3;
            restore_startup_trace("STARTUP diagnostic high scores: Escape back through C input\n");return;
        }
        if(high_scores_stage==3&&restore_probe_high_scores_return_frame.ready&&*(uint8_t *)0xe38ddau==0) {
            *(uint8_t *)0xe38df8u=0;*(uint8_t *)0xe38e00u=0;*(uint8_t *)0xe38e03u=0;*(uint8_t *)0xe38e04u=0;
        }
    }
    if(!family_action&&restore_probe_menu_frame.ready&&*(uint8_t *)0xe38ddau==48&&
       GetEnvironmentVariableA("RESTORE_FAMILY_ACTION_TEST",enabled,sizeof(enabled))==1&&(enabled[0]=='0'||enabled[0]=='2'||enabled[0]=='3')) {
        const int16_t *button=(const int16_t *)(0x5e0408u+(enabled[0]-'0')*28u);
        test_click(*(int32_t *)0xe38da8u+(button[0]+button[2])/2,*(int32_t *)0xe38da4u+(button[1]+button[3])/2);
        family_action=1;restore_startup_trace("STARTUP diagnostic family action: %s through C buttons\n",enabled[0]=='0'?"history":enabled[0]=='2'?"load game":"custom missions");
    }
    if(restore_probe_load_save_frame.ready&&*(uint8_t *)0xe38ddau==13&&
       ((GetEnvironmentVariableA("RESTORE_LOAD_SAVE_BACK_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1')||
        (GetEnvironmentVariableA("RESTORE_LOAD_SAVE_CONFIRM_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1'))) {
        if(load_save_stage==0) {
            const int16_t *button=(const int16_t *)0x5c3118u;
            test_click(*(int32_t *)0xe38da8u+(button[0]+button[2])/2,*(int32_t *)0xe38da4u+(button[1]+button[3])/2);
            load_save_stage=1;restore_startup_trace("STARTUP diagnostic load/save: select first row through C buttons\n");
        } else if(load_save_stage==1&&restore_probe_load_save_selected_frame.ready) {
            int confirm=GetEnvironmentVariableA("RESTORE_LOAD_SAVE_CONFIRM_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1';
            const int16_t *button=(const int16_t *)(0x5c79f0u+(confirm?36:0));
            test_click(*(int32_t *)0xe38da8u+button[0]+button[2]/2,*(int32_t *)0xe38da4u+button[1]+button[3]/2);
            load_save_stage=confirm?3:2;restore_startup_trace("STARTUP diagnostic load/save: %s through C buttons\n",confirm?"confirm":"cancel");
        }
    }
    if(!mission_back&&restore_probe_mission_frame.ready&&*(uint8_t *)0xe38ddau==61&&
       GetEnvironmentVariableA("RESTORE_MISSION_BACK_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1') {
        const int16_t *button=(const int16_t *)(0x57cc30u+2*36u);
        test_click(*(int32_t *)0xe38da8u+button[0]+button[2]/2,*(int32_t *)0xe38da4u+button[1]+button[3]/2);
        mission_back=1;restore_startup_trace("STARTUP diagnostic mission selector: back through C buttons\n");
    }
    if(GetEnvironmentVariableA("RESTORE_FAMILY_DIALOG_TEST",enabled,sizeof(enabled))==1&&enabled[0]>='1'&&enabled[0]<='3') {
        if(!family_dialog_stage&&restore_probe_family_frame.ready&&*(uint8_t *)0xe38ddau==0x2f) {
            unsigned int index=enabled[0]=='1'?12:13;
            const int16_t *b=(const int16_t *)(0x5f7f28u+index*28u);
            test_click(*(int32_t *)0xe38da8u+(b[0]+b[2])/2,*(int32_t *)0xe38da4u+(b[1]+b[3])/2);
            family_dialog_stage=1;
            restore_startup_trace("STARTUP diagnostic family dialog: open action %u through C buttons\n",index);
        } else if(family_dialog_stage==1&&restore_probe_dialog_frame.ready&&*(uint8_t *)0xe38ddau==0x3f) {
            unsigned int index=enabled[0]=='2'?1:0;
            const int16_t *b=(const int16_t *)(0x5c8168u+index*36u);
            test_click(*(int32_t *)0x705718u+b[0]+b[2]/2,*(int32_t *)0x705714u+b[1]+b[3]/2);
            family_dialog_stage=2;
            restore_startup_trace("STARTUP diagnostic family dialog: respond %u through C buttons\n",index==0);
        }
    }
    if(!family_selected&&restore_probe_family_frame.ready&&*(uint8_t *)0xe38ddau==0x2f&&
       GetEnvironmentVariableA("RESTORE_FAMILY_SELECT_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1') {
        const int16_t *button=(const int16_t *)(0x5f7f28u+14*28u);
        *(int32_t *)0xe38e34u=*(int32_t *)0xe38da8u+(button[0]+button[2])/2;
        *(int32_t *)0xe38e30u=*(int32_t *)0xe38da4u+(button[1]+button[3])/2;
        *(uint8_t *)0xe38e00u=1;*(uint8_t *)0xe38e03u=1;
        *(uint8_t *)0xe38e02u=0;*(uint8_t *)0xe38e04u=0;
        *(uint8_t *)0xe38df4u=0;*(uint8_t *)0xe38df7u=0;*(uint8_t *)0xe38df8u=0;
        family_selected=1;restore_startup_trace("STARTUP diagnostic family selection: confirm through C buttons\n");
    }
    if(!name_submitted&&restore_probe_frame.ready&&
       GetEnvironmentVariableA("RESTORE_MENU_SUBMIT_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1'&&
       *(uint8_t *)0xe38ddau==0x36) {
        name_submitted=1;
        restore_window_message(restore_window_platform(),*(HWND *)0xe39140u,WM_KEYDOWN,VK_RETURN,0);
        restore_startup_trace("STARTUP diagnostic name input: submit through C WM_KEYDOWN Return\n");
    }
    if(GetEnvironmentVariableA("RESTORE_MENU_NAME_TEST",enabled,sizeof(enabled))==1&&enabled[0]=='1'&&
       *(uint8_t *)0x00e38ddau==0x36&&name_frames++>=2&&name_character<7) {
        static const char name[]="Restore";
        restore_window_message(restore_window_platform(),*(HWND *)0x00e39140u,WM_CHAR,(WPARAM)name[name_character++],0);
        restore_startup_trace("STARTUP diagnostic name input: C WM_CHAR %u/7\n",name_character);
    }
    if(!menu_replayed&&GetEnvironmentVariableA("RESTORE_MENU_ACTION_TEST",enabled,sizeof(enabled))==1&&
       (enabled[0]=='0'||enabled[0]=='2'||enabled[0]=='3')&&*(uint8_t *)0x00e38ddau==0) {
        if(menu_frames++>0) {
            unsigned int action=(unsigned int)(enabled[0]-'0');
            *(int32_t *)0x00e38e34u=*(int32_t *)0x00e38da8u+320;
            *(int32_t *)0x00e38e30u=*(int32_t *)0x00e38da4u+112+40*action;
            *(uint8_t *)0x00e38e00u=1;*(uint8_t *)0x00e38e03u=1;
            *(uint8_t *)0x00e38e02u=0;*(uint8_t *)0x00e38e04u=0;
            *(uint8_t *)0x00e38df4u=0;*(uint8_t *)0x00e38df7u=0;*(uint8_t *)0x00e38df8u=0;
            menu_replayed=1;
            restore_startup_trace("STARTUP diagnostic menu input: dispatch action %u through C buttons\n",action);
        }
    }
    if(replayed||GetEnvironmentVariableA("RESTORE_STARTUP_CONTINUE_TEST",enabled,sizeof(enabled))!=1||enabled[0]!='1') return;
    if(*(uint8_t *)0x00e38ddau!=0x3c) return;
    if(title_frames++==0) return;
    *(uint8_t *)0x00e38e04u=1;
    replayed=1;
    restore_startup_trace("STARTUP diagnostic title input: dispatch one left-release flag in C\n");
}
