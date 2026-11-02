#ifndef RESTORE_DISPLAY_CONFIGURATION_H
#define RESTORE_DISPLAY_CONFIGURATION_H
#include <stdint.h>
void FUN_004cfd60(int mode);
/* Shared, non-UI portion of the original 004cfdc0 display-mode switch. */
void restore_display_configure(uint32_t mode);
#endif
