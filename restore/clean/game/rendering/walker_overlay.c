#include "pharaoh/rendering/walker_overlay.h"
#include <string.h>
#include <stddef.h>

_Static_assert(offsetof(PharaohWalkerQueryRecord,building_id)==0x5a &&
    offsetof(PharaohWalkerQueryRecord,reserved_066)==0x66 &&
    offsetof(PharaohBuildingTypeRecord,reserved_100)==0x100,
    "native walker overlay field offsets");
static const PharaohScenarioPlayerRuntime *player(const PharaohWalkerOverlayContext *c)
{return &c->players[*c->current_player];}

/* Whole mixed Unit00560 actor predicate, native004387D0. Both descendants
 *004C5120 and TempleUpgradePresent use existing actual typed C owners. The
 * default/overlay51 returns precede all walker-array accesses in the EXE. */
int32_t pharaoh_walker_matches_overlay(const PharaohWalkerOverlayContext *c,int32_t id)
{
    const uint32_t mode=(uint32_t)*c->overlay;
    if(mode==51) return 1;
    switch(mode) {
    case 4:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:
    case 16:case 17:case 18:case 19:case 20:case 21:case 22:case 23:case 24:
    case 25:case 26:case 29:case 30:case 31:case 43:case 45:case 46:case 48:case 49:case 50:break;
    default:return 0;
    }
    const PharaohWalkerQueryRecord *w=&c->walkers[id];
    const uint8_t type=w->type;
    switch(mode) {
    case 4:return type==27;
    case 8:return type==10;
    case 9:return type==8;
    case 10:
        switch(type) {
        case 88:case 89:case 22:case 23:case 24:case 11:case 12:case 13:case 14:return 1;
        case 27: {
            const int32_t building=w->building_id;
            const int32_t selected=player(c)->temple_complex_building_id;
            if(player(c)->temple_complex_enabled==0) return 0;
            const int32_t first=pharaoh_building_group_index(c->buildings,selected);
            if(pharaoh_building_group_index(c->buildings,building)!=first) return 0;
            const int32_t category=pharaoh_building_group_index(c->buildings,selected);
            uint32_t desired;
            if(category==1) desired=1;
            else if(category==3) desired=2;
            else return 0;
            return pharaoh_temple_upgrade_present(c->buildings,player(c)->temple_complex_building_id,desired)!=0;
        }
        default:return 0;
        }
    case 11:return (type>=15 && type<=18) || type==105;
    case 12:return type==15;
    case 13:return type==16;
    case 14:return type==17;
    case 15:return type==18;
    case 16:return type==105;
    case 17:return type==30 || type==29;
    case 18:case 20:return type==29;
    case 19:return type==30;
    case 21:return type==31;
    case 22:return type==32;
    case 23:case 48:return type==33;
    case 24:return type==34;
    case 25:return type==7;
    case 26:
        if(type==26 || type==39 || type==66 || type==25) return 1;
        if(type!=4) return 0;
        return w->counting_action_00b>=1 && w->counting_action_00b<=8;
    case 29:return type==5;
    case 30:return type==41 || type==64;
    case 31:
        if(type==4) return w->action_state==8 && w->reserved_066[0]!=0;
        if(type==5) {
            uint32_t flags;memcpy(&flags,c->buildings[w->building_id].reserved_100,sizeof(flags));
            return (flags&0x1000u)!=0;
        }
        return type==106 || type==109 || type==107;
    case 43:return type==32 || type==98;
    case 45:return type==87;
    case 46:return type==1 || type==2;
    case 49:
        if((type>=11 && type<=14) || (type>=41 && type<=62) || type==77 || type==78 ||
            type==88 || type==92 || type==93 || type==99 || type==100 || type==101) return 1;
        if(type==27) {
            const int32_t building_type=c->buildings[w->building_id].building_type;
            if(building_type!=63 && building_type!=68) return 0;
            if(player(c)->temple_complex_enabled==0) return 0;
            return pharaoh_temple_upgrade_present(c->buildings,player(c)->temple_complex_building_id,2)!=0;
        }
        return 0;
    case 50:return type==89;
    default:return 0;
    }
}
static int32_t matches(void *data,int32_t id)
{return pharaoh_walker_matches_overlay(data,id);}
void pharaoh_minimap_bind_walker_overlay(PharaohMinimapWalkers *m,PharaohWalkerOverlayContext *c)
{m->walker_matches_overlay=matches;m->user_data=c;}
