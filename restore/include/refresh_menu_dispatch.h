#ifndef RESTORE_REFRESH_MENU_DISPATCH_H
#define RESTORE_REFRESH_MENU_DISPATCH_H
#include "restore_types.h"
#include "initial_menu_dispatch.h"
#include <stdint.h>
#define RD_ADD(a,b) ((int32_t)((uint32_t)(a)+(uint32_t)(b)))
#define RD_MUL(a,b) ((int32_t)((uint32_t)(a)*(uint32_t)(b)))
int restore_refresh_hit(int x,int y,const void *buttons,int count,int origin_x,int origin_y);
int restore_refresh_hover(int x,int y,const void *buttons,int count,int hit);
void FUN_004d75d0(void);
#endif
