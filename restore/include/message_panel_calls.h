#ifndef RESTORE_MESSAGE_PANEL_CALLS_H
#define RESTORE_MESSAGE_PANEL_CALLS_H
#include "message_panel.h"
#include "sprite_render.h"
#include "video_open.h"
#define MP_I(a) (*(int32_t *)(uintptr_t)(a))
#define MP_U(a) (*(uint32_t *)(uintptr_t)(a))
#define MP_W(a) (*(int16_t *)(uintptr_t)(a))
#define MP_B(a) (*(uint8_t *)(uintptr_t)(a))
#define MP_R (restore_message_record())
#define MP_E (restore_message_event())
extern const char *FUN_00447840(void);
extern void FUN_00448330(int,char *,int);
extern int FUN_0047d350(int,int);
extern void FUN_004c8040(void),FUN_004dddc0(void),FUN_0051e990(void);
extern void FUN_004c8070(const char *,int,int,int,int,int);
extern void FUN_004cc720(const char *,int,int,int,int,int);
extern void FUN_004cc770(const char *,int,int,int,int);
extern void FUN_004ccbb0(int,int,int,int,int,int);
extern void FUN_004ccc30(int,int,int,int,int,int,int);
extern void FUN_004cccb0(int,int,int,int,int,int);
extern void FUN_004cce00(int,char,const char *,int,int,int,int);
extern void FUN_004cd010(const char *,int,int,int,int,int);
extern void FUN_004cd9d0(int,int,int,int,int),FUN_004cd680(int,int,int,int,int);
extern void FUN_00536820(int,int,int,int),FUN_00536990(int,int,int,int),FUN_00536a30(int,int,int,int);
extern int FUN_00561c96(const char *);
static __inline int mp_unscaled_resource(int resource)
{return resource==24||resource==25||resource==26||resource==30||resource==35||resource==28||resource==10||resource==36;}
static __inline int mp_resource_plural(int resource,int amount)
{return resource==24||resource==25||resource==26||resource==30||resource==35||((resource==28||resource==10)&&amount>1);}
#endif
