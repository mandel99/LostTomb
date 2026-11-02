#ifndef RESTORE_SIDEBAR_UNDO_H
#define RESTORE_SIDEBAR_UNDO_H
#include <stdint.h>
struct restore_sidebar_frame;
typedef void (*restore_sidebar_call)(uint32_t,uint32_t,struct restore_sidebar_frame *);
struct restore_sidebar_frame {uint32_t r[8],flags;restore_sidebar_call boundary;};
void restore_sidebar_C_00485230(struct restore_sidebar_frame *);
void restore_sidebar_C_00485890(struct restore_sidebar_frame *);
void restore_sidebar_C_004858e0(struct restore_sidebar_frame *);
void restore_sidebar_C_0040d890(struct restore_sidebar_frame *);
void restore_sidebar_C_00401480(struct restore_sidebar_frame *);
void restore_sidebar_C_00416fb0(struct restore_sidebar_frame *);
void restore_sidebar_C_004e6660(struct restore_sidebar_frame *);
void restore_sidebar_C_00545720(struct restore_sidebar_frame *);
void restore_sidebar_C_00558bc0(struct restore_sidebar_frame *);
void restore_sidebar_C_00555e60(struct restore_sidebar_frame *);
void restore_sidebar_C_0054f450(struct restore_sidebar_frame *);
void restore_sidebar_C_00546660(struct restore_sidebar_frame *);
void restore_sidebar_C_0045afe0(struct restore_sidebar_frame *);
void restore_sidebar_C_0045e220(struct restore_sidebar_frame *);
void restore_sidebar_C_0046b320(struct restore_sidebar_frame *);
void restore_sidebar_C_0046f030(struct restore_sidebar_frame *);
void restore_sidebar_C_0046a320(struct restore_sidebar_frame *);
void restore_sidebar_C_004e0c80(struct restore_sidebar_frame *);
void restore_sidebar_C_0046a280(struct restore_sidebar_frame *);
void restore_sidebar_undo_00485230(void);
void restore_sidebar_undo_00406010(void);
int restore_sidebar_undo_action(uint32_t token);
void restore_sidebar_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
#endif
