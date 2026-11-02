#ifndef RESTORE_PLACEMENT_QUERIES_H
#define RESTORE_PLACEMENT_QUERIES_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_MENU_INPUT_FRAME_DEFINED
#define RESTORE_RESTORE_MENU_INPUT_FRAME_DEFINED
struct restore_menu_input_frame;
typedef void (*restore_menu_input_frame_call)(uint32_t,uint32_t,struct restore_menu_input_frame *);
struct restore_menu_input_frame {uint32_t r[8],flags;restore_menu_input_frame_call boundary;};
#endif
/* Genuine native frame entries: image queries ECX+twoDWORD/RET8,
   position queries ECX+threeDWORD/RET12, activity547BA0 ECX+oneDWORD/RET4.
   The frame retains full EAX and native ordered DWORD output writes. */
void restore_placement_queries_dispatch(uint32_t site,uint32_t target,struct restore_menu_input_frame *frame);
/* These facades resolve actual vtable data tokens, never native code pointers.
   Output pointers are physical32-bit addresses; aliasing remains native. */
uint32_t restore_placement_image_query(uint32_t self,int32_t building,uint32_t out_flags);
uint32_t restore_placement_position_query(uint32_t self,int32_t building,uint32_t out_x,uint32_t out_y);
uint32_t restore_placement_activity_00547ba0(uint32_t self,int32_t building);
#endif
