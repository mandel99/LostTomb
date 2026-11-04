#include "sound_banks.h"
#include "sound_samples.h"
#include "sound_initialization.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
typedef struct RestoreBankLoad { uint32_t bank,path,variant; } RestoreBankLoad;
#include "sound_bank_loads.inc"

void restore_sound_load_building_sample(int32_t bank,const char *filename,int32_t mode)
{
    uint32_t row=(uint32_t)bank*20u;int32_t id;
    if(mode==0){
        id=FUN_00539c60(filename);U(0xf5467cu+row)=(uint32_t)id;
    }else if(mode==1&&(int32_t)U(0xf54680u+row)<3){
        id=FUN_00539c60(filename);
        U(0xf54670u+row+U(0xf54680u+row)*4u)=(uint32_t)id;
        U(0xf54680u+row)=U(0xf54680u+row)+1u;
    }
}
void restore_sound_initialize_building_samples(void)
{
    uint32_t i,row;
    for(i=0;i<237u;++i){
        row=0xf54670u+i*20u;
        U(row)=UINT32_MAX;U(row+4u)=UINT32_MAX;U(row+8u)=UINT32_MAX;
        U(row+12u)=UINT32_MAX;U(row+16u)=0;
    }
    for(i=0;i<sizeof(building_loads)/sizeof(building_loads[0]);++i)
        FUN_00539cf0((int32_t)building_loads[i].bank,(const char *)(uintptr_t)building_loads[i].path,(int32_t)building_loads[i].variant);
}
void restore_sound_load_ui_sample(int32_t index,const char *filename)
{
    int32_t id=FUN_00539c60(filename);
    U(0xf543b8u+(uint32_t)index*4u)=(uint32_t)id;
}
void restore_sound_initialize_ui_samples(void)
{
    uint32_t i;
    for(i=0;i<100u;++i)U(0xf543b8u+i*4u)=UINT32_MAX;
    for(i=0;i<sizeof(ui_loads)/sizeof(ui_loads[0]);++i)
        FUN_0053af70((int32_t)ui_loads[i].bank,(const char *)(uintptr_t)ui_loads[i].path);
}
void restore_sound_play_ui_sample(int32_t index)
{
    int32_t id;
    if(!U(0xf55b88u)||index>=19||!B(0xe38e63u))return;
    id=(int32_t)U(0xf543b8u+(uint32_t)index*4u);
    if(id<0)return;
    /* ID must survive aliases with any of the following request fields. */
    U(0xe92db0u)=U(0xe38ec4u);U(0xe92db4u)=64;
    U(0xe92dacu)=(uint32_t)id;U(0xe92da8u)=UINT32_MAX;U(0xe92db8u)=1;
    (void)FUN_0055f0e8(RESTORE_SOUND,(const int32_t *)(uintptr_t)0xe92da8u);
}
void restore_sound_load_actor_sample(int32_t bank,const char *filename,int32_t variant)
{
    int32_t id=FUN_00539c60(filename);
    U(0xf54120u+((uint32_t)bank*5u+(uint32_t)variant)*4u)=(uint32_t)id;
}
void restore_sound_initialize_actor_samples(void)
{
    uint32_t i;
    for(i=0;i<165u;++i)U(0xf54120u+i*4u)=UINT32_MAX;
    for(i=0;i<sizeof(actor_loads)/sizeof(actor_loads[0]);++i)
        FUN_0053b100((int32_t)actor_loads[i].bank,(const char *)(uintptr_t)actor_loads[i].path,(int32_t)actor_loads[i].variant);
}
