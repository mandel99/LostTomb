#include "city_disaster_fields.h"
#include "actor_command.h"
#include "building_destruction.h"
#include "building_footprint.h"
#include "building_removal.h"
#include "canal_region_model.h"
#include "city_updates.h"
#include "construction_refresh.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "menu_list_scroll.h"
#include "menu_prepare_integer.h"
#include "placement_support.h"
#include "remaining_menu_actions.h"

#define DISASTER_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define DISASTER_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define DISASTER_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define DISASTER_H(a) ((int32_t)(int16_t)DISASTER_W(a))
#define DISASTER_I(a) ((int32_t)(uint32_t)(a))
extern uint32_t FUN_00474380(int32_t kind);

/* Whole469140. Repair the complete original algorithm in its original
   owner: DWORD city/player fields, 264-byte records, signed WORD coordinates
   and types, BYTE state/size. Native difficulty indexes the five local rates
   directly; retain that original index contract without a substitute rate. */
void restore_city_apply_disaster_at_cell(int32_t x,int32_t y,uint32_t cell)
{
    const int32_t rates[5]={0,15,30,60,100};
    uint32_t building,record,owner_offset,difficulty,amount,size;
    int32_t linked,occupant,kind,coord_x,coord_y;
    uint16_t type;

    occupant=DISASTER_H(0xb4e6fcu+cell*2u);
    if(occupant<=0){
        if((DISASTER_U(0xaf598cu+cell*4u)&0x4000u)!=0u)
            restore_city_disaster_actors_004b7510(x,y);
        FUN_0047c2b0(0,x,y,1,1);
    }else{
        building=FUN_00475b50((uint32_t)occupant);
        record=building*264u;
        kind=DISASTER_H(0x7999e8u+record);
        if((uint8_t)FUN_00474380(kind)!=0u)return;
        type=DISASTER_W(0x7999e8u+record);
        if(type==202u){
            FUN_004c03d0(DISASTER_I(building));
        }else if(type==31u||type==30u||type==33u||type==209u){
            linked=DISASTER_I(FUN_0046d0f0(building));
            if(linked>0){
                DISASTER_U(0x77567cu)=1u;
                kind=DISASTER_H(0x7999e8u+(uint32_t)linked*264u);
                FUN_0043fd00(linked,kind);
            }
        }else{
            coord_y=DISASTER_H(0x7999e0u+record);
            coord_x=DISASTER_H(0x7999deu+record);
            size=DISASTER_B(0x7999dbu+record);
            FUN_0047c2b0(DISASTER_I(building),coord_x,coord_y,DISASTER_I(size),DISASTER_I(size));
        }
        if(DISASTER_B(0x7999d8u+record)==1u){
            type=DISASTER_W(0x7999e8u+record);
            if(type!=10u&&type!=11u&&type!=55u&&type!=81u&&type!=92u&&
               type!=57u&&type!=54u&&type!=58u&&type!=170u&&type!=171u&&
               type!=200u&&type!=201u&&type!=202u&&type!=59u&&type!=172u&&type!=173u){
                if(restore_building_is_kind_77_to_79(DISASTER_I(building))!=0u){
                    owner_offset=DISASTER_U(0xeaac28u)*0x49d8u;
                    difficulty=DISASTER_U(0xe39080u);
                    DISASTER_U(0xea5ec8u+owner_offset)=DISASTER_U(0xea5ec8u+owner_offset)+1u;
                    amount=(uint32_t)FUN_004cde40(DISASTER_I(DISASTER_U(0xea5cc0u+owner_offset)),rates[difficulty]);
                    if(amount!=0u){
                        owner_offset=DISASTER_U(0xeaac28u)*0x49d8u;
                        DISASTER_U(0xea5cc0u+owner_offset)=DISASTER_U(0xea5cc0u+owner_offset)-amount;
                        DISASTER_U(0xea5ff4u+owner_offset)=DISASTER_U(0xea5ff4u+owner_offset)+amount;
                        restore_remaining_message_4e2970(199,0u,0u);
                    }
                }else if(FUN_00459ef0(DISASTER_I(building))!=0u){
                    owner_offset=DISASTER_U(0xeaac28u)*0x49d8u;
                    difficulty=DISASTER_U(0xe39080u);
                    DISASTER_U(0xea5ec8u+owner_offset)=DISASTER_U(0xea5ec8u+owner_offset)+1u;
                    amount=(uint32_t)FUN_004cde40(DISASTER_I(DISASTER_U(0x799a90u+record)),rates[difficulty]);
                    if(amount!=0u){
                        FUN_004684e0();
                        owner_offset=DISASTER_U(0xeaac28u);
                        DISASTER_U(0xe950dcu)=1u;
                        owner_offset*=0x49d8u;
                        DISASTER_W(0xea5cfcu+owner_offset)=(uint16_t)(DISASTER_W(0xea5cfcu+owner_offset)+1u);
                        DISASTER_U(0xea44f4u+owner_offset)=DISASTER_U(0xea44f4u+owner_offset)+amount;
                        DISASTER_U(0xea5fd4u+owner_offset)=DISASTER_U(0xea5fd4u+owner_offset)+amount;
                        DISASTER_U(0xea187cu+owner_offset)=DISASTER_U(0xea187cu+owner_offset)-amount;
                        kind=DISASTER_H(0x7999e8u+record);
                        switch(kind){
                        case 84:case 85:case 86:case 87:
                            restore_remaining_message_4e2970(201,0u,0u);break;
                        case 184:
                            restore_remaining_message_4e2970(202,0u,0u);break;
                        case 187:case 188:case 189:
                            restore_remaining_message_4e2970(200,0u,0u);break;
                        default:break;
                        }
                    }
                }else{
                    owner_offset=DISASTER_U(0xeaac28u)*0x49d8u;
                    DISASTER_U(0xea5ec8u+owner_offset)=DISASTER_U(0xea5ec8u+owner_offset)+1u;
                }
            }
            owner_offset=DISASTER_U(0xeaac28u)*0x49d8u;
            if(DISASTER_I(DISASTER_U(0xea5ec8u+owner_offset))>=12)
                DISASTER_U(0xea5ec8u+owner_offset)=12u;
            DISASTER_B(0x7999d8u+record)=4u;
            coord_y=DISASTER_H(0x7999e0u+record);
            size=DISASTER_B(0x7999dbu+record);
            coord_x=DISASTER_H(0x7999deu+record);
            FUN_0048c0e0(coord_x,coord_y,size,1u);
            FUN_004695d0(DISASTER_I(building),0);
        }
    }
    restore_city_disaster_refresh_00497b90();
    FUN_0047afd0(x,y,3);
    FUN_0047bae0(DISASTER_I((uint32_t)x-2u),DISASTER_I((uint32_t)y-2u),
                 DISASTER_I((uint32_t)x+2u),DISASTER_I((uint32_t)y+2u),0u);
    FUN_00517370();
    FUN_00517660();
    FUN_005178e0();
}

void FUN_00469140(int32_t x,int32_t y,int32_t cell)
{restore_city_apply_disaster_at_cell(x,y,(uint32_t)cell);}
