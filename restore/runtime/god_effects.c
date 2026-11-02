#include "god_effects.h"
#include "disease_updates.h"
#include "event_publisher.h"
#include "god_building_effects.h"
#include "monument_effects.h"
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_00562348(void);
extern void FUN_004e96c0(int32_t amount);

static uint32_t player_offset(void){return U(0xeaac28u)*0x49d8u;}
static int32_t random_mod(int32_t divisor){return (int32_t)FUN_00562348()%divisor;}
static void publish(int32_t message){FUN_004e0cf0(message,0,0,0,0);}

static void monument_message(int32_t god,int16_t kind,uint8_t scratch[30])
{
    int16_t word=0x251;
    memcpy(scratch+2,&word,2);
    memcpy(scratch,&kind,2);
    /* Negative god values pass the native signed outer guard. Its unsigned
       switch skips WORD8 for them; preserve the supplied scratch unchanged. */
    if((uint32_t)god<=4u){word=(int16_t)(0x254+god);memcpy(scratch+16,&word,2);}
    U(0xe950dcu)=1;
    FUN_004e0cf0(0x83,0,0,(const int16_t *)(const void *)scratch,0);
}

/* Whole 4C47A0..4C50A4, 764 native instructions and six five-way tables.
   Effect helpers use unique genuine APIs; no route can bind legacy 1601. */
void restore_god_effect_core_004c47a0(int32_t god,uint32_t source,int32_t effect,uint8_t scratch[30])
{
    uint32_t player,offset,count_address,g=(uint32_t)god;
    int8_t level;
    uint8_t mood;
    (void)source;
    if(god>4)return;
    if(W(0x7848e2u+g*2u)==0)return;
    player=U(0xeaac28u);
    offset=player*0x49d8u+g;
    count_address=0xea5c50u+(player*0x1276u+g)*4u;
    level=(int8_t)B(0xea6086u+offset);
    if((level==50&&(int32_t)U(count_address)<15&&effect==-1)||effect==1){
        B(0xea6086u+offset)=0;
        mood=(uint8_t)(B(0xea5c0au+offset)-30u);
        U(0xe950dcu)=1;
        B(0xea5c0au+offset)=mood;
        if(restore_monument_effect_level_004f5340()!=0&&random_mod(2)==0){
            monument_message(god,0x252,scratch);
            restore_monument_effect_004f3aa0(restore_monument_effect_level_004f5340()==2u?2:1);
            return;
        }
        switch(god){
        case 0:
            if(random_mod(2)==0){publish(0x127);U(0xea6204u+player_offset())=1;}
            else{
                int32_t amount;
                publish(0x154);amount=(random_mod(3)*5+10)*2;
                restore_god_settings_004bd5e0(0xd38360u,amount);
            }
            return;
        case 1:
            if(random_mod(2)==0){publish(0x128);U(0xea5e3cu+player_offset())=12;}
            else{publish(0x149);FUN_004e96c0(15);}
            return;
        case 2:publish(restore_god_building_004c3950()?0x129:0x91);return;
        case 3:publish(0x12a);U(0xea5e40u+player_offset())=10;return;
        case 4:publish(0x12b);restore_god_building_004c42b0();return;
        default:return;
        }
    }
    if((level>=20&&(int32_t)U(count_address)<15&&effect==-1)||effect==2){
        B(0xea6086u+offset)=0;
        mood=(uint8_t)(B(0xea5c0au+offset)-12u);
        U(0xe950dcu)=1;
        B(0xea5c0au+offset)=mood;
        if(restore_monument_effect_level_004f5340()==2u&&random_mod(2)==0){
            monument_message(god,0x253,scratch);
            restore_monument_effect_004f3aa0(1);
            return;
        }
        switch(god){
        case 0:
            publish(0x141);
            restore_god_settings_004bd5e0(0xd38360u,random_mod(4)*5+5);
            return;
        case 1:
            if(random_mod(2)==0){publish(0x14b);W(0xea61f4u+player_offset())=12;}
            else{publish(0x142);FUN_004e96c0(5);}
            return;
        case 2:publish(0x143);restore_god_building_004c3ac0();return;
        case 3:publish(0x144);U(0xea61fcu+player_offset())=10;return;
        case 4:
            publish(0x145);offset=player_offset();
            U(0xea5d68u+offset)=4;U(0xea5d6cu+offset)=1;
            U(0xea5d98u+offset)=1;U(0xea5d70u+offset)=0;
            restore_god_city_004ea000();
            offset=player_offset();
            U(0xea5c54u+offset)=0;U(0xea5c58u+offset)=0;U(0xea5c5cu+offset)=0;
            return;
        default:return;
        }
    }
    level=(int8_t)B(0xea5c14u+offset);
    if((level==50&&(int32_t)U(count_address)>3&&effect==-1)||effect==3){
        B(0xea5c14u+offset)=0;
        B(0xea5c0au+offset)=(uint8_t)(B(0xea5c0au+offset)+30u);
        U(0xe950dcu)=1;
        switch(god){
        case 0:
            if(random_mod(2)==0){
                int32_t amount;
                publish(0x8c);amount=(-2-random_mod(3))*10;
                restore_god_settings_004bd5e0(0xd38360u,amount);
            }else{publish(0x93);restore_god_city_004e6eb0();}
            return;
        case 1:
            if(random_mod(3)==0){publish(0x14c);W(0xea61f6u+player_offset())=12;return;}
            if(random_mod(3)==1){publish(0x118);FUN_004e96c0(-15);}
            else{publish(0x14d);U(0xea6024u+player_offset())=12;}
            return;
        case 2:
            if(random_mod(2)==0)publish(restore_god_building_004c3ed0()?0x8e:0x8d);
            else{publish(0x94);restore_god_city_004e70b0(6);}
            return;
        case 3:
            if(random_mod(2)==0){publish(0x119);restore_god_city_004b4710();}
            else{publish(0x95);restore_god_city_004e6ed0();}
            return;
        case 4:publish(0x90);restore_god_building_004c45e0();return;
        default:return;
        }
    }
    if((level>=20&&(int32_t)U(count_address)>3&&effect==-1)||effect==4){
        B(0xea5c14u+offset)=0;
        mood=(uint8_t)(B(0xea5c0au+offset)+12u);
        U(0xe950dcu)=1;
        B(0xea5c0au+offset)=mood;
        switch(god){
        case 0:
            if(random_mod(2)==0){publish(0x122);U(0xea6208u+player_offset())=1;}
            else{
                int32_t amount;
                publish(0x155);amount=(-1-random_mod(4))*5;
                restore_god_settings_004bd5e0(0xd38360u,amount);
            }
            return;
        case 1:
            if(random_mod(2)==0){publish(0x14e);W(0xea61f8u+player_offset())=12;}
            else{publish(0x123);FUN_004e96c0(-5);}
            return;
        case 2:
            {
                uint32_t first=U(0x705df4u),second=U(0x705c10u);
                if(first+second){publish(0x124);restore_god_building_004c4020(1);}
                else publish(0x8d);
            }
            return;
        case 3:publish(restore_god_building_004c41b0()?0x125:0x8f);return;
        case 4:publish(0x126);(void)FUN_0042fcf0(1);return;
        default:return;
        }
    }
}

void restore_god_effect_004c47a0(int32_t god,uint32_t source,int32_t effect)
{
    uint8_t scratch[30];
    restore_god_effect_core_004c47a0(god,source,effect,scratch);
}

