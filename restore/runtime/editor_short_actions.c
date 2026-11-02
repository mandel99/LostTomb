#include "editor_short_actions.h"
#include "numeric_input.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Five complete three-instruction native callbacks differ only in the mode
   immediate. Table5763D8/count10 rows7/5/6/0 and5C5CD0/count12 row0;
   actual input calls4D8818/4D98DC. No argument/register/x87 input, RET0.
   Represent the identical mode-write/dirty-write bodies once. */
static const struct {uint32_t token;uint8_t mode;} mode_actions[]={
    {0x408c20u,0x19},{0x408cd0u,0x1b},{0x408f00u,0x1c},
    {0x409390u,0x25},{0x4093d0u,0x16}
};
int restore_editor_short_action(uint32_t token)
{
    uint32_t i;
    /* Whole4093A0..4093CB,13 instructions: table5C5CD0/count12 rows3..5.
       The selected DWORD is an index; native pointer arithmetic wraps. */
    if(token==0x4093a0u){
        uint32_t index=U(0xd3b198u);
        FUN_00403790(300,100,4,0,0,0x784c98u+index*4u,0);
        B(0xe38dcau)=1;return 1;
    }
    for(i=0;i<sizeof(mode_actions)/sizeof(mode_actions[0]);++i){
        if(token!=mode_actions[i].token)continue;
        B(0xe38ddau)=mode_actions[i].mode;
        B(0xe38dcau)=1;return 1;
    }
    return 0;
}
