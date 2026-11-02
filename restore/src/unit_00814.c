#include "production_updates.h"
#include "actor_teardown_model.h"
#include "menu_building_kind.h"
#include "actor_predicate.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
static void boost(uint32_t record)
{
    uint32_t player;int32_t group=0;
    switch(H(record+16u)){
    case 74:case 109:case 111:case 113:case 161:case 162:case 217:group=1;break;
    case 106:case 107:case 177:case 178:case 179:case 204:case 216:case 221:case 231:group=2;break;
    }
    if(!group)return;
    player=U(0xeaac28u)*0x49d8u;
    if(!W(0xea5ce8u+player)||FUN_004c5120((int32_t)U(0xea5f9cu+player))!=2u)return;
    if(group==2){
        player=U(0xeaac28u)*0x49d8u;
        if(!FUN_004c52d0((int32_t)U(0xea5f9cu+player),1))return;
    }
    W(record+84u)=(uint16_t)(W(record+84u)+H(record+64u)/2);
}
/* 455900: preserve the live DWORD record count,264-byte stride and
 * WORD production arithmetic. The schedule object is passed explicitly. */
void FUN_00455900(void)
{
    uint32_t id=1,record;int32_t limit;
    FUN_00488630((volatile uint8_t *)(uintptr_t)0xbca350u);
    while((int32_t)id<=(int32_t)U(0x78d790u)){
        record=0x7999d8u+id*264u;
        if(B(record)==1&&B(record+67u)){
            if(restore_actor_building_gate((int32_t)id)){
                if(!B(record+166u)||FUN_00459240((int32_t)id))W(record+64u)=0;
                else W(record+64u)=W(0x705eb8u+(uint32_t)(int32_t)H(record+16u)*40u);
            }
            if(H(record+24u)>=1||restore_actor_building_gate((int32_t)id)){
                if(H(record+64u)>0&&(!H(record+18u)||H(record+60u)>=100)&&
                   (!H(record+206u)||H(record+204u)>=(H(record+16u)==204?25:100))){
                    if(B(record+173u))--B(record+173u);
                    else{
                        if(B(record+169u))--B(record+169u);
                        if(!FUN_00488350((int32_t)id))W(record+84u)=(uint16_t)(W(record+84u)+W(record+64u));
                        boost(record);
                        if(!(uint8_t)FUN_00488030(id)){
                            limit=H(record+18u)?400:200;
                            if(H(record+84u)>limit)W(record+84u)=(uint16_t)limit;
                        }
                    }
                }
            }
        }else if(H(record+16u)==177&&B(record)&&H(record+64u)>0&&
                 (!H(record+18u)||H(record+60u)>0)){
            W(record+84u)=(uint16_t)(W(record+84u)+W(record+64u));
            if(H(record+84u)>400)W(record+84u)=400;
        }
        ++id;
    }
}
