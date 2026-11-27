#ifndef PHARAOH_UI_ADVISOR_OPEN_H
#define PHARAOH_UI_ADVISOR_OPEN_H
#include "pharaoh/ui/advisor_initialize.h"
#include "pharaoh/ui/message_dialog_live.h"
#include "pharaoh/runtime/city_warning.h"

typedef struct PharaohAdvisorOpenContext {
    const uint32_t *tutorial_override,*requested_advisor; /*E38EBC/D3B198*/
    const uint8_t *tutorial_advisors; /*6FDC88*/
    uint8_t *current_window,*redraw; /*E38DDA/E38DCA*/
    /* Preserve historical member aliases; native active/saved roles differ. */
    union { uint32_t *active_advisor,*last_advisor; }; /*605A44*/
    union { uint32_t *saved_advisor,*shown_advisor; }; /*E38EA4*/
    uint32_t *animation_step; /*F53F48*/
    uint8_t *gift_notification_banks; /*EA623C, player stride49D8*/
    const PharaohAdvisorInitializeContext *initialize;
    const PharaohMessageDialogLiveCloseContext *close_dialog;
    PharaohCityWarningContext *warnings;
} PharaohAdvisorOpenContext;

/* Complete0040A0E0. Shared window/redraw pointers alias close_dialog's fields.
   Same current player and actual pools as initialize. Requested advisor is
   the full native DWORD; its native valid-index domain is not clamped.
   Diagnostic divide status stops before redraw/animation on the allowed path. */
PharaohPercentageScaleStatus pharaoh_advisors_show_checked(
    const PharaohAdvisorOpenContext *);
/* Complete0040B520. Availability mutates shown_advisor in place. */
PharaohPercentageScaleStatus pharaoh_advisors_show_last(
    const PharaohAdvisorOpenContext *);
#endif
