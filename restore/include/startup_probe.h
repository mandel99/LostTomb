#ifndef RESTORE_STARTUP_PROBE_H
#define RESTORE_STARTUP_PROBE_H
#include <stdint.h>
typedef struct RestoreProbeFrame {
    uint32_t magic;
    volatile uint32_t ready;
    uint32_t width,height,pitch,format,mode;
    void *pixels;
} RestoreProbeFrame;
extern RestoreProbeFrame restore_probe_frame;
extern RestoreProbeFrame restore_probe_menu_frame;
extern RestoreProbeFrame restore_probe_family_frame;
extern RestoreProbeFrame restore_probe_dialog_frame;
extern RestoreProbeFrame restore_probe_return_frame;
extern RestoreProbeFrame restore_probe_campaign_frame;
extern RestoreProbeFrame restore_probe_mission_frame;
extern RestoreProbeFrame restore_probe_load_save_frame;
extern RestoreProbeFrame restore_probe_load_save_selected_frame;
extern RestoreProbeFrame restore_probe_high_scores_frame;
extern RestoreProbeFrame restore_probe_high_scores_scrolled_frame;
extern RestoreProbeFrame restore_probe_high_scores_dragged_frame;
extern RestoreProbeFrame restore_probe_high_scores_return_frame;
extern RestoreProbeFrame restore_probe_city_frame;
extern RestoreProbeFrame restore_probe_gameplay_frame;
extern uint32_t restore_probe_high_scores_state[4][2];
void restore_startup_probe_input(void);
void restore_startup_probe_frame_complete(void);
#endif
