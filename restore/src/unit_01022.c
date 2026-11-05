#include "map_commit.h"
#include "drag_leaves.h"
#include "editor_brush_validation.h"
#include "placement_context.h"
#include "placement_support.h"
#include "building_allocator.h"
#include "building_rect_commit.h"
#include "small_constructor_model.h"
#include "construction_controller.h"
#include "map_commit_placement.h"
#include "map_commit_route_actions.h"
#include "map_commit_state.h"
#include "map_commit_warnings.h"
#include "map_commit_costs.h"
#include "map_commit_selection.h"
#include "map_commit_connect.h"
#include "map_commit_proximity.h"
#include "map_commit_resource_warnings.h"
#include "map_commit_resource_gate.h"
#include "map_commit_town_warning.h"
#include "map_commit_population.h"
#include "map_commit_refresh.h"
#include "terrain_redraw.h"
#include "paving_region_model.h"
#include "footprint.h"
#include "actor_predicate.h"
#include "animation_state.h"
#include "menu_prepare_integer.h"
#include "region_validation.h"
#include "city_footprint_validation.h"
#include "monument_placement_queries.h"
#include "remaining_warnings.h"
#include "pending_message.h"

extern code *restore_placement_rule(uint32_t token);
extern void FUN_0047bae0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_0051f780(void);
extern void restore_placement_create_00470990(uint32_t,int32_t,int32_t,int32_t,
                                             int32_t,int32_t,uint32_t,uint32_t);

#define COMMIT_U32(address) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(address))
#define COMMIT_U16(address) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(address))
#define COMMIT_I16(address) (*(volatile int16_t *)(uintptr_t)(uint32_t)(address))
#define COMMIT_U8(address) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(address))
#define COMMIT_COST COMMIT_U32(0x00bc9728u)
#define COMMIT_CREDIT COMMIT_U32(0x00bc9718u)
#define COMMIT_COUNT COMMIT_U32(0x00bc971cu)
#define COMMIT_SIZE COMMIT_U32(0x00bc96d0u)
#define COMMIT_CITY COMMIT_U32(0x00eaac28u)
#define COMMIT_EDITOR COMMIT_U32(0x00605a0cu)
#define COMMIT_CELL COMMIT_U32(0x00bca300u)

static int32_t commit_add(int32_t a,int32_t b)
{
    return (int32_t)((uint32_t)a+(uint32_t)b);
}

static int32_t commit_sub(int32_t a,uint32_t b)
{
    return (int32_t)((uint32_t)a-b);
}

static uint32_t commit_city_offset(void)
{
    return COMMIT_CITY*0x49d8u;
}

static void commit_clear_preview(void)
{
    FUN_0040f920((void *)(uintptr_t)0x00b285ccu,0xcb10,0xaf);
}

static void commit_reject(int32_t message)
{
    FUN_004e2970(message);
    commit_clear_preview();
}

static void commit_redraw_area(int32_t x,int32_t y)
{
    uint32_t radius=COMMIT_SIZE;
    FUN_00516f90(commit_sub(x,radius),commit_sub(y,radius),
                (int32_t)((uint32_t)x+radius),(int32_t)((uint32_t)y+radius),1);
    /* The first callback can change the radius used by the second call. */
    radius=COMMIT_SIZE;
    FUN_005173a0(commit_sub(x,radius),commit_sub(y,radius),
                (int32_t)((uint32_t)x+radius),(int32_t)((uint32_t)y+radius));
}

/* Preserve the native warning sequence. Its callbacks can change live globals;
 * the footprint argument is the caller's cached local, not COMMIT_SIZE. */
static void commit_warnings(int32_t tool,int32_t x,int32_t y,int32_t footprint)
{
    FUN_004e3330(tool);
    FUN_004e2d30(tool);
    FUN_004e3100(tool);
    FUN_004e3140(tool);
    FUN_004e3180(tool);
    FUN_004e31c0(tool);
    FUN_004e3200(tool);
    FUN_004e2e20(tool);
    FUN_004e3240(tool);
    FUN_004e3280(tool,x,y,footprint);
    FUN_004e32e0(tool,x,y);
    FUN_0051f780();
    FUN_004e33a0(tool);
    FUN_004e3460(tool);
    FUN_004e3520(tool);
    FUN_004e35e0(tool);
    FUN_004e36a0(tool);
    FUN_004e3760(tool);
    FUN_004e3aa0(tool);
    FUN_004e39d0(tool);
    FUN_004e3cf0(tool);
    FUN_004e3da0(tool);
    FUN_004e3f60(tool);
    FUN_004e4120(tool);
    FUN_004e42d0(tool);
    FUN_004e4490(tool);
    FUN_004e4550(tool);
    FUN_004e47d0(tool);
    FUN_004e4e10(tool);
    FUN_004e4ed0(tool);
    FUN_004e5070(tool);
    FUN_004e2b10(tool,x,y,(uint32_t)footprint);
}

void restore_map_commit(uint32_t player,int32_t orientation,int32_t first_x,
                        int32_t first_y,int32_t last_x,int32_t last_y,int32_t tool)
{
    uint8_t allow_floodplain=(uint8_t)FUN_0046fde0((uint32_t)tool);
    uint32_t city_at_dispatch,terrain_mask=UINT32_MAX,query_result,city_offset;
    uint32_t amount,resource,spent_address,required,rule,cell;
    int32_t footprint,building=0,query,warning,kind,contiguous;
    int skip_initialization=0;

    COMMIT_U32(0x00e94e20u)=0;
    COMMIT_COST=0;
    COMMIT_CREDIT=0;
    if(tool==0)return;
    city_at_dispatch=COMMIT_CITY;
    if((int32_t)COMMIT_U32(0x00ea187cu+city_at_dispatch*0x49d8u)<=-5000) {
        commit_clear_preview();
        FUN_004e2970(3);
        return;
    }
    if(tool!=9&&FUN_004b5830(first_x,first_y,last_x,last_y)!=0) {
        switch(tool) {
        case 6:case 0xa8:case 0xa9:case 5:case 8:
        case 0x26:case 0x27:
            FUN_0040f860((void *)(uintptr_t)0x009de62cu,
                        (void *)(uintptr_t)0x00af598cu,0x32c40);
            FUN_0040f860((void *)(uintptr_t)0x009d1b1cu,
                        (void *)(uintptr_t)0x00ae8e7cu,0xcb10);
            if(tool==0x26||tool==0x27) {
                FUN_0040f860((void *)(uintptr_t)0x009b84fcu,
                            (void *)(uintptr_t)0x00b285ccu,0xcb10);
                FUN_0040f860((void *)(uintptr_t)0x009ab9ecu,
                            (void *)(uintptr_t)0x00b67d1cu,0xcb10);
            }
            FUN_00485830();
            break;
        case 0x52:case 0x53:COMMIT_U32(0x00bc96f8u)=0;break;
        default:commit_clear_preview();break;
        }
        FUN_004e2970(0x32);
        return;
    }
    if(tool!=9)city_at_dispatch=COMMIT_CITY;
    if(tool==0xcf||tool==0xd0) {
        if(FUN_004e59c0()>0) {
            commit_reject(0x58);
            return;
        }
        city_at_dispatch=COMMIT_CITY;
    }
    COMMIT_U32(0x00bc9720u)=0;
    COMMIT_COST=COMMIT_U32(0x00705ea4u+(uint32_t)tool*40u);

    switch(tool) {
    case 9:
        FUN_004743f0(0,first_x,first_y,last_x,last_y);
        goto multiply_count;
    case 6:case 0xa8:case 0xa9:
        if((uint8_t)FUN_00470900(first_x,first_y,last_x,last_y))goto protected_region;
        terrain_mask=tool==6?0x4000u:tool==0xa8?0x404000u:0x804000u;
        FUN_00475220(0,first_x,first_y,last_x,last_y,terrain_mask);
        goto multiply_count;
    case 5:
        if((uint8_t)FUN_00470900(first_x,first_y,last_x,last_y))goto protected_region;
        FUN_00474f50(0,first_x,first_y,last_x,last_y);
        goto multiply_count;
    case 0x26:
        FUN_004754a0(0,first_x,first_y,last_x,last_y);
        goto multiply_count;
    case 0x1f:case 0x1e:case 0x21:case 0xd1:
        if(tool==0xd1&&COMMIT_U16(0x00ea6084u+city_at_dispatch*0x49d8u)!=0) {
            FUN_004e2970(4);
            return;
        }
        footprint=tool==0x1f?1:tool==0x1e?2:tool==0x21?3:4;
        if((uint8_t)FUN_00470900(first_x,first_y,commit_add(first_x,footprint),
                                commit_add(first_y,footprint)))goto protected_region;
        if(tool==0x1f)query_result=FUN_00440700(0,(uint32_t)first_x,(uint32_t)first_y,last_x,last_y);
        else if(tool==0x1e)query_result=FUN_00440850(0,(uint32_t)first_x,(uint32_t)first_y,last_x,last_y);
        else if(tool==0x21)query_result=FUN_00440b10(0,(uint32_t)first_x,(uint32_t)first_y,last_x,last_y);
        else query_result=FUN_00440ed0(0,(uint32_t)first_x,(uint32_t)first_y,last_x,last_y);
        if(query_result==0) {
            if(tool==0x1f)commit_clear_preview();
            FUN_004e2970(tool==0xd1?0xd7:0x4d);
            return;
        }
        if(tool!=0xd1)FUN_004e5070(tool);
        goto finish;
    case 0x27:
        if((uint8_t)FUN_00470900(first_x,first_y,last_x,last_y))goto protected_region;
        FUN_004759e0(0,first_x,first_y,last_x,last_y);
        COMMIT_COST=COMMIT_COUNT*COMMIT_COST;
        commit_redraw_area(last_x,last_y);
        goto finish;
    case 0x52:case 0x53:
        FUN_0047f530(last_x,last_y,tool==0x53);
        if((int32_t)COMMIT_U32(0x00bc96f8u)<2) {
            FUN_004e2970(0x19);
            return;
        }
        COMMIT_COST=COMMIT_U32(0x00bc96f8u)*COMMIT_COST;
        goto finish;
    case 8:
        FUN_00475090(0,first_x,first_y,last_x,last_y);
        goto multiply_count;
    case 10:
        /* The pinned original performs this guard as well as the housing
         * commit's individual-cell checks. */
        if((uint8_t)FUN_00470900(first_x,first_y,last_x,last_y))goto protected_region;
        restore_building_rect_00474130(0,first_x,first_y,last_x,last_y);
        goto multiply_count;
    default:break;
    }

    footprint=(int32_t)COMMIT_U32(0x005d06dcu+(uint32_t)tool*20u);
    COMMIT_SIZE=(uint32_t)footprint;
    switch(tool) {
    case 0x3a:case 0xaa:case 0xab:case 200:case 0xc9:case 0xca:case 0x38:
        terrain_mask=0xffffffbfu;break;
    case 0x3b:case 0xac:case 0xad:terrain_mask=0xff3fbfffu;break;
    case 0x8a:terrain_mask=8;break;
    case 0x48:footprint=3;break;
    default:break;
    }
    if((uint8_t)FUN_00470900(first_x,first_y,
            commit_add(first_x,commit_add(footprint,-1)),
            commit_add(first_y,commit_add(footprint,-1))))goto protected_region;
    if(tool!=0xab&&tool!=0xca) {
        if(orientation==6)last_y=commit_add(last_y,commit_sub(1,(uint32_t)footprint));
        else if(orientation==4) {
            last_x=commit_add(last_x,commit_sub(1,(uint32_t)footprint));
            last_y=commit_add(last_y,commit_sub(1,(uint32_t)footprint));
        } else if(orientation==2)last_x=commit_add(last_x,commit_sub(1,(uint32_t)footprint));
    }

    switch(tool) {
    case 0x4c:case 0xb5:
        if(FUN_0047cc70(last_x,last_y,0,0)) {
            FUN_004e2970(0x19);return;
        }
        goto extra_checks;
    case 7:
        if(FUN_0047cc70(last_x,last_y,0,1)) {
            commit_reject(0x19);return;
        }
        tool=0x5a;
        cell=COMMIT_U32(0x00bca2d8u)+(uint32_t)last_y*228u+(uint32_t)last_x;
        building=restore_building_allocate_0046a400(player,0x5a,last_x,last_y,0,0);
        FUN_004e2d30(0x5a);
        FUN_004e32e0(0x5a,last_x,last_y);
        (void)FUN_00485100(building);
        {
            uint32_t height=COMMIT_U32(0x00bca2d4u)-1u;
            uint32_t width=COMMIT_U32(0x00bca2d0u)-1u;
            COMMIT_U8(0x00ae8e7cu+cell)=0;
            FUN_0047bae0(0,0,(int32_t)width,(int32_t)height,0);
        }
        commit_redraw_area(last_x,last_y);
        goto allocate;
    case 0xab:case 0xca:
        if(!(uint8_t)FUN_004bff60(last_x,last_y,(int32_t)COMMIT_U32(0x00bc9714u))) {
            commit_clear_preview();FUN_004e2970(2);return;
        }
        goto extra_checks;
    case 0xb6:case 0x4a:case 0x4b:case 0xc0:
        if((tool==0x4b||tool==0xc0)&&(int32_t)COMMIT_U32(0x00705c1cu)>=10) {
            FUN_004e2970(0xce);return;
        }
        if(FUN_0047d060(last_x,last_y,0)) {FUN_004e2970(0x19);return;}
        if(!FUN_00457ac0(last_x,last_y)) {FUN_004e2970(0x38);return;}
        if(tool!=0xb6)goto extra_checks;
        if((int32_t)COMMIT_U32(0x00bcb9b0u)>=6) {FUN_004e2970(0x54);return;}
        goto allocate;
    case 0x88:
        if(!COMMIT_U8(0x00bca314u)) {
            if(!COMMIT_U8(0x00bca315u))FUN_004e2970(0x44);
            return;
        }
        if(FUN_0047cc70(last_x,last_y,0,0)) {FUN_004e2970(0x19);return;}
        goto allocate;
    case 0xe5:
        query_result=restore_placement_check_005456c0(0x006f9320u,COMMIT_CELL);
        if(!(uint8_t)query_result)goto invalid_complex;
        query_result=restore_placement_existing_00545700(0x006f9320u,query_result);
        goto existing_large_complex;
    case 0xea:
        query_result=restore_placement_check_00558b60(0x006f5648u,COMMIT_CELL);
        if(!(uint8_t)query_result)goto invalid_complex;
        query_result=restore_placement_existing_00558ba0(0x006f5648u,query_result);
        goto existing_large_complex;
    case 0xeb:
        query_result=restore_placement_check_00555e00(0x006de9e0u,COMMIT_CELL);
        if(!(uint8_t)query_result)goto invalid_complex;
        query_result=restore_placement_existing_00555e40(0x006de9e0u,query_result);
        goto existing_large_complex;
    case 0xec:
        query_result=restore_placement_check_0054f3f0(0x006e9188u,COMMIT_CELL);
        if(!(uint8_t)query_result)goto invalid_complex;
        query_result=restore_placement_existing_0054f430(0x006e9188u,query_result);
        goto existing_large_complex;
    case 0xe4:
        query_result=restore_placement_check_004e65f0(0x006de548u,COMMIT_CELL);
        if(!(uint8_t)query_result) {warning=0xf9;goto invalid_complex_message;}
        query_result=restore_placement_existing_004e6650(0x006de548u,query_result);
        goto existing_small_complex;
    case 0xe6:
        query_result=restore_placement_check_00401400(0x006de650u,COMMIT_CELL);
        if(!(uint8_t)query_result)goto invalid_complex;
        /* 00401430 forwards to the shared existing-building query. Its AL is
         * real; the upper EAX bits are those from this exact preceding check. */
        query_result=restore_placement_existing_00401430(0x006de650u,query_result);
        goto existing_small_complex;
    default:break;
    }
    if(!FUN_0047c880(last_x,last_y,footprint,footprint,(int32_t)terrain_mask,0,allow_floodplain)) {
        commit_reject(2);return;
    }
    if(tool==0x35||tool==0xcf||tool==0xd0) {
        if(COMMIT_EDITOR!=1) {
            uint32_t city=COMMIT_CITY;
            amount=tool==0x35?500u:tool==0xcf?10000u:20000u;
            resource=tool==0x35?0x17u:0x1au;
            spent_address=tool==0x35?0x00bc9650u:0x00bc965cu;
            cell=tool==0x35?0x00ea61bcu:0x00ea61c8u;
            if((int32_t)COMMIT_U32(cell+city*0x49d8u)<(int32_t)amount) {
                commit_reject(tool==0x35?0x4f:tool==0xcf?0x55:0x56);return;
            }
            (void)FUN_0045b4d0((int32_t)city,(int32_t)resource,(int32_t)amount);
            COMMIT_U32(spent_address)=COMMIT_U32(spent_address)+amount;
        }
        goto allocate;
    }
    if(tool==0xa1) {
        rule=COMMIT_U32(0x00605a48u);
        if(rule&&!FUN_00482720(last_x,last_y,2,1,restore_placement_rule(rule))) {
            FUN_004e2970(0x16);return;
        }
        if(!(uint8_t)FUN_00459fa0())FUN_004e2970(0x3c);
        goto allocate;
    }
    /* Exactly one requirement branch runs; the order is the native priority. */
    warning=0;
    if(COMMIT_U32(0x006059ccu)) {
        if(!FUN_00482650(last_x,last_y,3,1,0x10800u))warning=0x14;
    } else if((rule=COMMIT_U32(0x00605a48u))!=0) {
        if(!FUN_00482720(last_x,last_y,2,1,restore_placement_rule(rule)))warning=0x16;
    } else if(COMMIT_U32(0x006059c8u)) {
        if(!FUN_00482650(last_x,last_y,2,1,1))warning=0x17;
    } else if(COMMIT_U32(0x006059c0u)) {
        if(!FUN_00482650(last_x,last_y,2,1,0x40000u))warning=0x3a;
    } else if(COMMIT_U32(0x006059c4u)) {
        if(!FUN_00482650(last_x,last_y,2,3,0x10004u))warning=0x15;
    } else if(COMMIT_U32(0x006059bcu)) {
        if(!FUN_004828e0(last_x,last_y,2,0,0x4000u))warning=0x31;
    } else if(COMMIT_U32(0x006059b8u)) {
        /* Cache the cell before the callback, as EBX does in the original. */
        cell=COMMIT_U32(0x00bca2d8u)+(uint32_t)last_y*228u+(uint32_t)last_x;
        if(!FUN_004828e0(last_x,last_y,1,0,0x40u))warning=0x42;
        else if(COMMIT_U8(0x00adc36cu+cell))warning=2;
    } else if((required=COMMIT_U32(0x006059b4u))!=0) {
        int32_t size=(int32_t)COMMIT_U32(0x005d06dcu+(uint32_t)tool*20u);
        if(!FUN_004829b0(last_x,last_y,size,0,0x80u,(int32_t)required))warning=0x4c;
    }
    if(warning) {FUN_004e2970(warning);return;}

extra_checks:
    if(tool==0x28||tool==0x2c||tool==0x2d) {
        if(!FUN_0047c880(commit_add(last_x,3),commit_add(last_y,-1),4,4,
                         (int32_t)terrain_mask,0,allow_floodplain)) {
            FUN_004e2970(2);return;
        }
        if((int32_t)COMMIT_U32(0x00bcb9b4u)>=6) {FUN_004e2970(0x34);return;}
    }
    switch(tool) {
    case 0xd2:
        if(COMMIT_U16(0x00ea5baau+commit_city_offset())) {FUN_004e2970(4);return;}
        if(!FUN_0047c880(commit_add(last_x,6),last_y,6,6,(int32_t)terrain_mask,0,allow_floodplain)) {
            FUN_004e2970(2);return;
        }
        if(!FUN_0047c880(commit_add(last_x,12),last_y,6,6,(int32_t)terrain_mask,0,allow_floodplain)) {
            FUN_004e2970(2);return;
        }
        if((uint8_t)FUN_00470900(commit_add(last_x,6),last_y,commit_add(last_x,11),commit_add(first_y,5)))
            goto protected_region;
        if((uint8_t)FUN_00470900(commit_add(last_x,12),last_y,commit_add(last_x,17),commit_add(first_y,5)))
            goto protected_region;
        goto allocate;
    case 0xd5:
        query_result=FUN_00541ae0(COMMIT_CELL);
        if(!(uint8_t)query_result) {FUN_004e2970(2);return;}
        query_result=restore_monument_ready_00542ee0(query_result);
        if((uint8_t)query_result) {FUN_004e2970(0x5b);return;}
        amount=22000;
        warning=0x5a;
        goto special_resource;
    case 0xe1:
        query_result=restore_placement_check_0040d850(0x006fd5d8u,COMMIT_CELL);
        if(!(uint8_t)query_result) {FUN_004e2970(2);return;}
        query_result=restore_placement_existing_0040d880(0x006fd5d8u,query_result);
        if((uint8_t)query_result) {FUN_004e2970(0xf3);return;}
        goto allocate;
    case 0xe3:
        query_result=restore_placement_check_00416f70(0x006fd368u,COMMIT_CELL);
        if(!(uint8_t)query_result) {FUN_004e2970(2);return;}
        query_result=restore_placement_existing_00416fa0(0x006fd368u,query_result);
        goto existing_large_complex;
    case 0xde:
        cell=COMMIT_U32(0x00bca2d8u)+(uint32_t)first_y*228u+(uint32_t)first_x;
        if(!FUN_004def00((int32_t)cell)) {FUN_004e2970(2);return;}
        amount=24000;
        warning=0xbe;
        goto special_resource;
    default:break;
    }
    if(tool>=0x41&&tool<=0x45) {
        if(COMMIT_U16(0x00ea5ce8u+commit_city_offset()))goto existing_unique;
        query=FUN_004c3730(last_x,last_y);
        if(query==-1) {commit_reject(2);return;}
        if(query==-2)goto protected_region;
    }
    if(tool==0xd3||tool==0xd4) {
        query=FUN_00475b50(COMMIT_I16(0x00b4e6fcu+COMMIT_CELL*2u));
        city_offset=commit_city_offset();
        if(!COMMIT_U16(0x00ea5ce8u+city_offset)) {commit_reject(0x49);return;}
        kind=COMMIT_I16(0x007999e8u+(uint32_t)query*264u);
        if(kind<0x41||kind>0x45) {commit_reject(0x4b);return;}
        if(COMMIT_I16(0x00ea5cfau+city_offset)<2) {commit_reject(0x4e);return;}
        if(FUN_004c52d0((int32_t)COMMIT_U32(0x00ea5f9cu+city_offset),tool==0xd3?1u:2u)) {
            commit_reject(0x4a);return;
        }
        goto allocate;
    }
    city_offset=commit_city_offset();
    if((tool==0xbd||tool==0xbc||tool==0xbb)&&COMMIT_U16(0x00ea5cf6u+city_offset))goto existing_unique;
    if((tool==0x4f||tool==0x4e||tool==0x4d)&&COMMIT_U32(0x00ea620cu+city_offset))goto existing_unique;
    if(tool==0x56) {
        if(!COMMIT_U16(0x00ea5cf6u+city_offset)) {commit_reject(0x59);return;}
    } else if(tool==0x5f) {
        if((int32_t)COMMIT_U32(0x00705c6cu)>0)goto existing_unique;
        COMMIT_U32(0x00705c6cu)=1;
    } else if(tool==0x5e) {
        if(COMMIT_U32(0x00ea6240u+city_offset))goto existing_unique;
    } else if(tool==0xb7) {
        uint32_t failure_reason=(uint32_t)first_x;
        cell=COMMIT_U32(0x00bca2d8u)+(uint32_t)last_y*228u+(uint32_t)last_x;
        if(!(uint8_t)FUN_004ecec0((int32_t)cell,(int32_t)COMMIT_U32(0x005e6970u),&failure_reason)) {
            FUN_004e2970(failure_reason==2?0xd6:0xd5);return;
        }
    }
    goto allocate;

special_resource:
    if(COMMIT_EDITOR==0) {
        uint32_t city=COMMIT_CITY;
        if((int32_t)COMMIT_U32(0x00ea61d8u+city*0x49d8u)<(int32_t)amount) {
            FUN_004e2970(warning);return;
        }
        (void)FUN_0045b4d0((int32_t)city,0x1e,(int32_t)amount);
        COMMIT_U32(0x00bc966cu)=COMMIT_U32(0x00bc966cu)+amount;
    }
    goto allocate;

existing_large_complex:
    if((uint8_t)query_result) {FUN_004e2970(0xf5);return;}
    goto allocate;
existing_small_complex:
    if((uint8_t)query_result) {FUN_004e2970(0xf4);return;}
    goto allocate;
invalid_complex:
    warning=0x110;
invalid_complex_message:
    FUN_004e2970(warning);
    FUN_004e2970(0x111);
    return;
existing_unique:
    commit_reject(4);
    return;
protected_region:
    commit_reject(0x3d);
    return;

allocate:
    commit_warnings(tool,last_x,last_y,footprint);
    COMMIT_U8(0x00605a5cu)=0;
    kind=tool;
    contiguous=0;
    switch(tool) {
    case 0x28:case 0x2c:case 0x2d:kind=0x39;break;
    case 0xb7:contiguous=0x90;break;
    case 0xd5:contiguous=100;skip_initialization=1;break;
    case 0xe6:contiguous=0x10;skip_initialization=1;break;
    case 0xe1:contiguous=0x19;skip_initialization=1;break;
    case 0xe3:contiguous=9;skip_initialization=1;break;
    case 0xe5:contiguous=0xa7;skip_initialization=1;break;
    case 0xea:contiguous=0x9e;skip_initialization=1;break;
    case 0xeb:contiguous=0x1b5;skip_initialization=1;break;
    case 0xec:contiguous=0x201;skip_initialization=1;break;
    case 0xe4:contiguous=2;skip_initialization=1;break;
    case 0xde:contiguous=0x74;skip_initialization=1;break;
    default:break;
    }
    if(tool!=0x5a)
        building=restore_building_allocate_0046a400(player,(uint32_t)kind,last_x,last_y,contiguous,0);
    if(building==0)return;
    if(!skip_initialization)(void)FUN_00485100(building);
    if(building<1)return;
    restore_placement_create_00470990(player,tool,building,last_x,last_y,(int32_t)COMMIT_SIZE,1,0);
    if(tool==0x88)FUN_00486020();
    goto finish;

multiply_count:
    COMMIT_COST=COMMIT_COUNT*COMMIT_COST;
finish:
    FUN_00517370();
    FUN_00517660();
    FUN_005178e0();
    FUN_004b9ee0(last_x,last_y);
    query_result=tool==0x88&&COMMIT_U32(0x00bca310u)==0?0:COMMIT_COST;
    query_result-=COMMIT_CREDIT;
    COMMIT_COST=query_result;
    FUN_004e9680((int32_t)player,(int32_t)query_result);
    if(tool!=0x38&&tool!=9)FUN_00485200();
    FUN_004f61e0();
}
