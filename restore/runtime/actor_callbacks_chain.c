#include "actor_callbacks_chain.h"
extern int32_t FUN_004b3ff0(int32_t actor,int32_t steps);
extern void FUN_004b22e0(int32_t actor,int32_t predecessor,int32_t steps);
extern uint32_t FUN_0047d350(void);
/* Explicit sound adapter preserves the discarded native event argument. */
extern void restore_actor_chain_sound(int32_t event);
static void next_frame(uint32_t a,uint8_t count)
{
    uint8_t next=(uint8_t)(AC_B(a-7u)+1u);AC_B(a-7u)=next;if(next>=count)AC_B(a-7u)=0;
}
/* Original004a2e90, type74 projectile. Home WORD names a shooter actor. */
void FUN_004a2e90(void)
{
    uint32_t id=ac_id(),a=ac_actor(id),target,prey,owner;int32_t arrived,direction;uint8_t age;
    AC_B(a+4u)=1;age=(uint8_t)(AC_B(a+0x31u)+1u);AC_B(a+0x31u)=age;if((int8_t)age>104)AC_B(a+6u)=2;
    next_frame(a,5);arrived=FUN_004b3ff0((int32_t)id,3);
    target=(uint32_t)FUN_004b57e0((int32_t)ac_id(),70);
    if(!target)target=(uint32_t)FUN_004b57e0((int32_t)ac_id(),68);
    if(!target)target=(uint32_t)FUN_004b57e0((int32_t)ac_id(),69);
    prey=ac_actor(target);
    if(target&&AC_B(prey+0x30u)!=12&&AC_B(prey+0x30u)!=4){
        AC_B(prey+0x30u)=12;AC_W(prey+0x2eu)=0;a=ac_current();owner=ac_actor((uint32_t)(int32_t)AC_S(a+0x52u));
        AC_W(owner+0x56u)=(uint16_t)target;AC_B(owner+0x30u)=11;
        AC_W(owner+0x1cu)=AC_W(prey+12u);AC_W(owner+0x1eu)=AC_W(prey+14u);
        restore_actor_chain_sound(24);id=ac_id();AC_B(ac_actor(id)+6u)=2;
    }else{
        id=ac_id();if(arrived){a=ac_actor(id);owner=ac_actor((uint32_t)(int32_t)AC_S(a+0x52u));AC_B(owner+0x30u)=8;AC_B(a+6u)=2;}
    }
    a=ac_actor(id);direction=(int32_t)((uint32_t)(int32_t)(int8_t)AC_B(a+9u)-(AC_U(0x00bca2e0u)<<2));
    AC_U(0x00e94e00u)=(uint32_t)direction;if(direction<0){direction=(int32_t)((uint32_t)direction+32u);AC_U(0x00e94e00u)=(uint32_t)direction;}
    if(!AC_B(0x00784cb0u)){AC_U(a-4u)=AC_U(0x00f55d44u)+AC_B(a-7u);AC_U(a+0x178u)=0;}
    else{AC_U(a-4u)=AC_U(0x00f55cecu)+(uint32_t)direction;AC_U(a+0x178u)=AC_U(0x00f55cf4u)+(uint32_t)direction;}
}
/* Original0049b400, type66 carrier. Cached WORD predecessor/home survive calls. */
void FUN_0049b400(void)
{
    uint32_t id=ac_id(),a=ac_actor(id),predecessor,root,root_state,stock;int32_t previous=AC_S(a+0x60u),home=AC_S(a+0x52u),direction;uint8_t value;
    root=(uint32_t)FUN_0049b3c0((int32_t)id);id=ac_id();a=ac_actor(id);AC_B(a+0x5du)=0;AC_B(a+0x6au)=1;
    next_frame(a,12);AC_U(a+0x178u)=0;predecessor=ac_actor((uint32_t)previous);
    if(previous<=0||AC_B(predecessor+0x30u)==4)AC_B(a+6u)=2;
    else if(AC_B(predecessor+6u)!=1){
        AC_B(a+6u)=2;stock=0x00799a2eu+(uint32_t)home*264u+(uint32_t)AC_B(a+0x72u)*2u;AC_W(stock)=(uint16_t)(AC_W(stock)+100u);
    }else if(AC_B(predecessor+2u)!=39&&AC_B(predecessor+2u)!=66)AC_B(a+6u)=2;
    else if(AC_B(ac_actor(root)+0x30u)!=7&&AC_B(ac_actor(root)+0x30u)!=6){
        FUN_004b22e0((int32_t)id,AC_S(a+0x60u),1);id=ac_id();
    }
    root_state=ac_actor(root)+0x30u;
    if(AC_B(root_state)==7)AC_B(ac_actor(id)-7u)=0;
    if(AC_B(root_state)==6)AC_B(ac_actor(id)+0x5du)=1;
    if(AC_B(predecessor+0x5du))AC_B(ac_actor(id)+0x5du)=1;
    a=ac_actor(id);value=AC_B(a+9u);direction=(int8_t)value<8?(int8_t)value:(int8_t)AC_B(a+10u);
    direction=(int32_t)((uint32_t)direction-AC_U(0x00bca2e0u));AC_U(0x00e94e00u)=(uint32_t)direction;
    if(direction<0){direction=(int32_t)((uint32_t)direction+8u);AC_U(0x00e94e00u)=(uint32_t)direction;}
    if(AC_B(a+0x30u)==4){int32_t frame=AC_S(a+0x2eu)/2;AC_U(a-4u)=AC_U(0x00f55d34u)+96u+AC_B(0x005d048cu+(uint32_t)frame);}
    else AC_U(a-4u)=AC_U(0x00f55d34u)+(uint32_t)AC_B(a-7u)*8u+(uint32_t)direction;
}
