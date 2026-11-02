#include "family_menu.h"
#include "text_selection.h"
#include <string.h>
extern void FUN_004cc500(int);
extern void FUN_004cc3c0(void);
extern void FUN_004cc3e0(void);
extern void FUN_004264d0(uint32_t,uint32_t,uint32_t);
/* Complete local40C890: button5C5AF8 enters the name-choice panel from
   new-family mode54. Native RET0 consumes no receiver or stack arguments. */
void restore_family_open_name_choices(void)
{
    *(volatile uint8_t *)(uintptr_t)0xe38ddau=81;
    *(volatile uint8_t *)(uintptr_t)0xe38dcau=1;
}
/* Whole local40C810: return from choices to the new-family text editor. */
void restore_family_close_name_choices(void)
{
    *(volatile uint32_t *)(uintptr_t)0xe20998u=0;
    *(volatile uint8_t *)(uintptr_t)0xe38ddau=54;
    *(volatile uint8_t *)(uintptr_t)0xe38dcau=1;
}
/* Whole local40C830: copy the selected native catalog name including NUL,
   prepare the real text editor, then reload the selected row for highlighting. */
void restore_family_choose_name(void)
{
    const char *name;
    FUN_004cc110(197,*(int32_t *)(uintptr_t)0xd3b198u);
    name=*(const char **)(uintptr_t)0xe20768u;
    memcpy((void *)0xe38e74u,name,strlen(name)+1u);
    FUN_004cc500(1);FUN_004cc3c0();FUN_004cc3e0();
    *(volatile uint32_t *)(uintptr_t)0x6059fcu=*(volatile uint32_t *)(uintptr_t)0xd3b198u;
}
/* Original local callbacks omitted from the exported procedure list. */
void FUN_00516c10(void)
{
    memset((void *)0xe38e74u,0,32);
    *(uint8_t *)0xe38ddau=0x36;*(uint32_t *)0xe20998u=0;
    FUN_004cc500(1);FUN_004cc3c0();FUN_004cc3e0();
    *(uint32_t *)0xe94dc0u=0x2f;*(uint8_t *)0xe38dcau=1;
}
void FUN_00516c60(void)
{FUN_004264d0(0x5a,0x516dd0,2);}
