#ifndef PHARAOH_SCENARIO_TUTORIAL_ADVISORS_H
#define PHARAOH_SCENARIO_TUTORIAL_ADVISORS_H
#include <stdint.h>

/* Complete004179A0, logical AL result. Actual6FDC88 flags and mutable saved
   selection required. Native wrap15->1 and initial-index domain retained;
   no invented iteration cap or repair of invalid initial selections. */
uint8_t pharaoh_tutorial_find_available_advisor(
    const uint8_t *flags,uint32_t *selection);
#endif
