#ifndef RESTORE_DROPDOWN_MENU_H
#define RESTORE_DROPDOWN_MENU_H
#include <stdint.h>
#include <stddef.h>
#define DM_U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define DM_I(a) (*(volatile int32_t *)(uintptr_t)(a))
#define DM_W(a) (*(volatile uint16_t *)(uintptr_t)(a))
#define DM_S(a) (*(volatile int16_t *)(uintptr_t)(a))
#define DM_B(a) (*(volatile uint8_t *)(uintptr_t)(a))
typedef struct RestoreDropdownBar { int16_t left,right,top,group;uint32_t items;int16_t count,reserved; } RestoreDropdownBar;
typedef struct RestoreDropdownItem { int16_t top,entry;uint32_t callback,reserved,argument; } RestoreDropdownItem;
typedef struct RestoreSquareButton { int16_t left,top,image,size;uint32_t callback;uint8_t enabled,countdown,repeat,kind;uint32_t primary,secondary; } RestoreSquareButton;
typedef char dm_bar_size[sizeof(RestoreDropdownBar)==16?1:-1];
typedef char dm_item_size[sizeof(RestoreDropdownItem)==16?1:-1];
typedef char dm_square_size[sizeof(RestoreSquareButton)==24?1:-1];
static __inline int32_t dm_add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
uint32_t restore_dropdown_clock_ms(void);
void restore_dropdown_callback(uint32_t token);
int32_t FUN_00425c40(int32_t x,int32_t y,RestoreSquareButton *buttons,int32_t count);
void FUN_00425e80(int32_t x,int32_t y,const RestoreSquareButton *buttons,int32_t count,int32_t base);
int32_t FUN_00425f20(RestoreDropdownBar *bars,int32_t count);
int32_t FUN_00426030(RestoreDropdownBar *bars);
void FUN_00426090(void);
void FUN_004260d0(void);
int32_t FUN_00426100(const RestoreDropdownBar *bars,int32_t count);
int32_t FUN_00426150(const RestoreDropdownItem *items,int32_t count,int32_t x,int32_t y);
int32_t FUN_004d5f60(int32_t index);
void FUN_00425430(void);
void FUN_00403410(void);void FUN_00403490(void);void FUN_00403500(void);void FUN_00403540(void);void FUN_0040c7d0(void);
#endif
