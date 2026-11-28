#include "pharaoh/ui/building_info_draw_bindings.h"
#include <stddef.h>

/* Binder metadata from the already reconstructed4F8E60 catalogue. This does
 * not introduce another Native owner. A zero is the actual default branch;
 * a one requires a renderer and must never be treated as a default/no-op. */
static const uint8_t native_catalog_present[237]={
    [7]=1,[90]=1,[10]=1,[11]=1,[12]=1,[13]=1,[14]=1,[15]=1,[16]=1,[17]=1,
    [18]=1,[19]=1,[20]=1,[21]=1,[22]=1,[23]=1,[24]=1,[25]=1,[26]=1,[27]=1,
    [28]=1,[29]=1,[30]=1,[31]=1,[32]=1,[33]=1,[34]=1,[35]=1,[36]=1,[37]=1,
    [41]=1,[42]=1,[43]=1,[46]=1,[47]=1,[49]=1,[51]=1,[53]=1,[55]=1,[56]=1,
    [57]=1,[58]=1,[200]=1,[59]=1,[74]=1,[75]=1,[192]=1,[76]=1,[77]=1,[78]=1,
    [79]=1,[80]=1,[81]=1,[84]=1,[85]=1,[187]=1,[188]=1,[189]=1,[86]=1,[87]=1,
    [88]=1,[89]=1,[92]=1,[93]=1,[94]=1,[185]=1,[186]=1,[95]=1,[98]=1,[99]=1,
    [100]=1,[101]=1,[102]=1,[103]=1,[104]=1,[105]=1,[106]=1,[108]=1,[109]=1,
    [110]=1,[111]=1,[112]=1,[113]=1,[114]=1,[115]=1,[136]=1,[138]=1,[161]=1,
    [162]=1,[167]=1,[170]=1,[201]=1,[171]=1,[202]=1,[172]=1,[173]=1,[177]=1,
    [178]=1,[179]=1,[180]=1,[181]=1,[182]=1,[183]=1,[184]=1,[194]=1,[195]=1,
    [196]=1,[199]=1,[203]=1,[204]=1,[205]=1,[206]=1,[207]=1,[208]=1,[209]=1,
    [210]=1,[213]=1,[217]=1,[222]=1,[224]=1,[225]=1,[226]=1,[227]=1,[228]=1,
    [229]=1,[234]=1,[235]=1,[236]=1,[230]=1,[231]=1,[232]=1,[233]=1
};
static int storage_type(int32_t type)
{ return type==70||type==71||type==72||type==190||type==191||type==193; }
static int temple_type(int32_t type)
{ return (type>=60&&type<=69)||(type>=140&&type<=144); }
static int quarry_type(int32_t type)
{ return type==107||type==216||type==221; }
/* Exact4F93B0 selector/4F920C targets and their proved tail blocks. */
static int industry_type(int32_t type)
{
    switch(type) {
    case 7:case 90:case 92:case 100:case 101:case 102:case 103:case 106:case 107:
    case 109:case 110:case 111:case 112:case 115:case 161:case 162:case 177:
    case 178:case 179:case 180:case 194:case 195:case 199:case 216:case 217:
    case 221:case 224:case 231:return 1;
    default:return 0;
    }
}
static int industry_farm(int32_t type)
{ return type==100||type==101||type==102||type==103||type==224; }

static int services_type(int32_t type)
{
    if(temple_type(type))return 1;
    switch(type) {
    case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 46:case 47:case 49:case 51:case 53:case 55:case 77:case 78:case 79:case 80:case 81:case 84:case 85:case 86:case 87:case 88:case 89:case 93:case 98:case 167:case 184:case 187:case 188:case 189:case 206:case 209:case 226:return 1;
    default:return 0;
    }
}

static int monuments_type(int32_t type)
{
    switch(type){
    case 37:case 57:case 58:case 59:case 74:case 75:case 76:case 94:case 95:case 104:case 105:case 108:case 113:case 114:case 136:case 138:case 170:case 171:case 172:case 173:case 181:case 182:case 183:case 185:case 186:case 192:case 196:case 200:case 201:case 202:case 203:case 204:case 205:case 207:case 208:case 210:case 213:case 222:case 225:case 227:case 228:case 229:case 230:case 232:case 233:case 234:case 235:case 236:return 1;
    default:return 0;
    }
}

enum PharaohBuildingInfoDrawTypeSupport pharaoh_building_info_draw_type_support(int32_t type)
{
    if((uint32_t)type>=237)return PHARAOH_INFO_DRAW_OUTSIDE_TYPE_CATALOG;
    if((type>=10&&type<=29)||(type>=41&&type<=43)||type==56||type==99||storage_type(type)||industry_type(type)||services_type(type)||monuments_type(type))
        return PHARAOH_INFO_DRAW_CONNECTED_TYPE;
    if(native_catalog_present[type]||temple_type(type)||quarry_type(type))
        return PHARAOH_INFO_DRAW_MISSING_TYPE_PROVIDER;
    return PHARAOH_INFO_DRAW_NATIVE_DEFAULT;
}

static const PharaohStorageInfoDrawContext *drawing(void *data)
{ return ((PharaohBuildingInfoDrawBindings *)data)->providers.storage; }
/* LiveOperations' text consumers use exactly these five actual font IDs. */
static const PharaohBitmapFont *font(const PharaohStorageInfoDrawContext *d,int32_t id)
{
    if(id==*d->ui->font_black)return d->font_black;
    if(id==*d->ui->font_green)return d->font_green;
    if(id==*d->ui->font_white)return d->font_white;
    if(id==*d->ui->font_red)return d->font_red;
    if(id==*d->ui->font_title)return d->font_title;
    return NULL; /* outside the supported parent font domain */
}
static void border(void *p,int32_t style,int32_t x,int32_t y,int32_t w,int32_t h,
    int32_t flags,int32_t ox,int32_t oy)
{ pharaoh_info_layout_draw_border(drawing(p)->layout,style,x,y,w,h,flags,ox,oy); }
static void text(void *p,int32_t group,int32_t entry,int32_t x,int32_t y,
    int32_t id,int32_t color,int32_t ox,int32_t oy)
{
    const PharaohStorageInfoDrawContext *d=drawing(p);
    pharaoh_ui_text_draw_language(d->text,font(d,id),group,entry,x,y,id,(uint16_t)color,ox,oy);
}
static void centered(void *p,int32_t group,int32_t entry,int32_t x,int32_t y,
    int32_t w,int32_t id,int32_t color,int32_t ox,int32_t oy)
{
    const PharaohStorageInfoDrawContext *d=drawing(p);
    pharaoh_ui_text_draw_centered_language(d->text,font(d,id),group,entry,x,y,w,id,(uint16_t)color,ox,oy);
}
static void outer(void *p,int32_t x,int32_t y,int32_t w,int32_t h)
{ pharaoh_panel_frame_draw_native(drawing(p)->layout->outer_panel,x,y,w,h); }
static void inner(void *p,int32_t x,int32_t y,int32_t w,int32_t h,int32_t ox,int32_t oy)
{ pharaoh_info_layout_draw_inner_panel(drawing(p)->layout,x,y,w,h,ox,oy); }
static void image(void *p,int32_t id,int32_t x,int32_t y)
{ (void)pharaoh_image_draw(drawing(p)->layout->inner_panel->drawing,id,x,y); }
static void arrows(void *p,int32_t x,int32_t y,PharaohSquareButtonRecord *rows,int32_t n)
{
    PharaohBuildingInfoDrawBindings *b=p;
    pharaoh_info_square_buttons_draw(b->providers.square_buttons,x,y,rows,n);
}
static void buttons(void *p,int32_t x,int32_t y,PharaohImageButtonRecord *rows,int32_t n)
{
    PharaohBuildingInfoDrawBindings *b=p;
    PharaohImageButtonDrawState state=*b->providers.image_buttons;
    state.draw_sprite=image;state.user_data=b;
    pharaoh_image_buttons_draw(x,y,rows,n,&state);
}
static void city(void *p)
{ pharaoh_city_render_current_view(((PharaohBuildingInfoDrawBindings *)p)->providers.city); }
static void sample(void *p,int32_t file,int32_t volume,int32_t pan,int32_t owner,int32_t loops)
{
    const PharaohDigitalSoundDeviceContext *s=((PharaohBuildingInfoDrawBindings *)p)->providers.sample_sound;
    const PharaohDigitalSoundRequest request={owner,file,volume,pan,loops};
    (void)pharaoh_digital_sound_play_sample(s->state,s->operations,&request);
}
static void no_info(void *p)
{ pharaoh_info_draw_no_info(((PharaohBuildingInfoDrawBindings *)p)->providers.content); }
static void terrain(void *p)
{ pharaoh_info_draw_terrain(((PharaohBuildingInfoDrawBindings *)p)->providers.terrain); }
static void house(void *p)
{ pharaoh_info_draw_house(((PharaohBuildingInfoDrawBindings *)p)->providers.house); }
static void statue(void *p)
{ pharaoh_info_draw_statue(((PharaohBuildingInfoDrawBindings *)p)->providers.content); }
static void arch(void *p)
{ pharaoh_info_draw_triumphal_arch(((PharaohBuildingInfoDrawBindings *)p)->providers.content); }
static void burning(void *p)
{ pharaoh_info_draw_burning_ruin(((PharaohBuildingInfoDrawBindings *)p)->providers.content); }
static void bazaar(void *p) { pharaoh_info_draw_bazaar(drawing(p)); }
static void granary(void *p) { pharaoh_info_draw_granary(drawing(p)); }
static void storage(void *p) { pharaoh_info_draw_storage_yard(drawing(p)); }
static void legion(void *p,int32_t formation)
{
    PharaohBuildingInfoDrawBindings *b=p;
    b->arithmetic=pharaoh_unit_panel_draw_legion(b->providers.units,formation);
}
static void transport(void *p,int32_t formation)
{
    PharaohBuildingInfoDrawBindings *b=p;(void)formation;
    b->arithmetic=pharaoh_unit_panel_draw_transport(b->providers.units);
}
static void warship(void *p,int32_t formation)
{
    PharaohBuildingInfoDrawBindings *b=p;(void)formation;
    b->arithmetic=pharaoh_unit_panel_draw_warship(b->providers.units);
}
static void legion_foreground(void *p,int32_t formation)
{ pharaoh_unit_panel_draw_legion_foreground(((PharaohBuildingInfoDrawBindings *)p)->providers.units,formation); }

static const PharaohInfoIndustryLiveContext *industry(void *p)
{ return ((PharaohBuildingInfoDrawBindings *)p)->providers.industry; }
static void industry_water_lift(void *p) { pharaoh_info_industry_draw_water_lift(industry(p)); }
static void industry_water_supply(void *p) { pharaoh_info_industry_draw_water_supply(industry(p)); }
static void industry_well(void *p) { pharaoh_info_industry_draw_well(industry(p)); }
static void industry_work_camp(void *p) { pharaoh_info_industry_draw_work_camp(industry(p)); }
static void industry_barley(void *p) { pharaoh_info_industry_draw_barley_farm(industry(p)); }
static void industry_flax(void *p) { pharaoh_info_industry_draw_flax_farm(industry(p)); }
static void industry_grain(void *p) { pharaoh_info_industry_draw_grain_farm(industry(p)); }
static void industry_lettuce(void *p) { pharaoh_info_industry_draw_lettuce_farm(industry(p)); }
static void industry_henna(void *p) { pharaoh_info_industry_draw_henna_farm(industry(p)); }
static void industry_reeds(void *p) { pharaoh_info_industry_draw_reed_gatherer(industry(p)); }
static void industry_cattle(void *p) { pharaoh_info_industry_draw_cattle_ranch(industry(p)); }
static void industry_stone(void *p) { pharaoh_info_industry_draw_stone_quarry(industry(p)); }
static void industry_gold(void *p) { pharaoh_info_industry_draw_gold_mine(industry(p)); }
static void industry_copper(void *p) { pharaoh_info_industry_draw_copper_mine(industry(p)); }
static void industry_gems(void *p) { pharaoh_info_industry_draw_gemstone_mine(industry(p)); }
static void industry_clay(void *p) { pharaoh_info_industry_draw_clay_pit(industry(p)); }
static void industry_brewery(void *p) { pharaoh_info_industry_draw_brewery(industry(p)); }
static void industry_weaver(void *p) { pharaoh_info_industry_draw_weaver(industry(p)); }
static void industry_weapons(void *p) { pharaoh_info_industry_draw_weaponsmith(industry(p)); }
static void industry_carpenters(void *p) { pharaoh_info_industry_draw_carpenters_guild(industry(p)); }
static void industry_bricklayers(void *p) { pharaoh_info_industry_draw_bricklayers_guild(industry(p)); }
static void industry_stonemasons(void *p) { pharaoh_info_industry_draw_stonemasons_guild(industry(p)); }
static void industry_artisans(void *p) { pharaoh_info_industry_draw_artisans_guild(industry(p)); }
static void industry_hunting(void *p) { pharaoh_info_industry_draw_hunting_lodge(industry(p)); }
static void industry_quarry(void *p,int32_t resource)
{ pharaoh_info_industry_draw_quarry(industry(p),resource); }

static const PharaohInfoServicesLiveContext *services(void *p)
{ return ((PharaohBuildingInfoDrawBindings *)p)->providers.services; }
static void services_bandstand(void *p)
{ pharaoh_info_services_draw_bandstand(services(p)); }
static void services_booth(void *p)
{ pharaoh_info_services_draw_booth(services(p)); }
static void services_senet_house(void *p)
{ pharaoh_info_services_draw_senet_house(services(p)); }
static void services_pavilion(void *p)
{ pharaoh_info_services_draw_pavilion(services(p)); }
static void services_conservatory(void *p)
{ pharaoh_info_services_draw_conservatory(services(p)); }
static void services_dance_school(void *p)
{ pharaoh_info_services_draw_dance_school(services(p)); }
static void services_juggler_school(void *p)
{ pharaoh_info_services_draw_juggler_school(services(p)); }
static void services_apothecary(void *p)
{ pharaoh_info_services_draw_apothecary(services(p)); }
static void services_mortuary(void *p)
{ pharaoh_info_services_draw_mortuary(services(p)); }
static void services_dentist(void *p)
{ pharaoh_info_services_draw_dentist(services(p)); }
static void services_scribal_school(void *p)
{ pharaoh_info_services_draw_scribal_school(services(p)); }
static void services_library(void *p)
{ pharaoh_info_services_draw_library(services(p)); }
static void services_police_station(void *p)
{ pharaoh_info_services_draw_police_station(services(p)); }
static void services_mansion(void *p)
{ pharaoh_info_services_draw_mansion(services(p)); }
static void services_mission_post(void *p)
{ pharaoh_info_services_draw_mission_post(services(p)); }
static void services_architect_post(void *p)
{ pharaoh_info_services_draw_architect_post(services(p)); }
static void services_palace(void *p)
{ pharaoh_info_services_draw_palace(services(p)); }
static void services_tax_collector(void *p)
{ pharaoh_info_services_draw_tax_collector(services(p)); }
static void services_native_hut(void *p)
{ pharaoh_info_services_draw_native_hut(services(p)); }
static void services_native_meeting(void *p)
{ pharaoh_info_services_draw_native_meeting(services(p)); }
static void services_native_crops(void *p)
{ pharaoh_info_services_draw_native_crops(services(p)); }
static void services_oracle(void *p)
{ pharaoh_info_services_draw_oracle(services(p)); }
static void services_firehouse(void *p)
{ pharaoh_info_services_draw_firehouse(services(p)); }
static void services_courthouse(void *p)
{ pharaoh_info_services_draw_courthouse(services(p)); }
static void services_physician(void *p)
{ pharaoh_info_services_draw_physician(services(p)); }
static void services_festival_square(void *p)
{ pharaoh_info_services_draw_festival_square(services(p)); }
static void services_zoo(void *p)
{ pharaoh_info_services_draw_zoo(services(p)); }
static void services_temple(void *p,int32_t god)
{ pharaoh_info_services_draw_temple(services(p),god); }
static void services_mansion_foreground(void *p)
{ pharaoh_info_services_draw_mansion_foreground(services(p)); }

static const PharaohInfoMonumentsLiveContext *monuments(void *p)
{return ((PharaohBuildingInfoDrawBindings *)p)->providers.monuments;}
static void monuments_bullfight_school(void *p)
{pharaoh_info_monuments_draw_bullfight_school(monuments(p));}
static void monuments_fort(void *p)
{pharaoh_info_monuments_draw_fort(monuments(p));}
static void monuments_mud_gatehouse(void *p)
{pharaoh_info_monuments_draw_mud_gatehouse(monuments(p));}
static void monuments_mud_tower(void *p)
{pharaoh_info_monuments_draw_mud_tower(monuments(p));}
static void monuments_shipwright(void *p)
{pharaoh_info_monuments_draw_shipwright(monuments(p));}
static void monuments_dock(void *p)
{pharaoh_info_monuments_draw_dock(monuments(p));}
static void monuments_fishing_wharf(void *p)
{pharaoh_info_monuments_draw_fishing_wharf(monuments(p));}
static void monuments_military_academy(void *p)
{pharaoh_info_monuments_draw_military_academy(monuments(p));}
static void monuments_recruiter(void *p)
{pharaoh_info_monuments_draw_recruiter(monuments(p));}
static void monuments_pomegranate_farm(void *p)
{pharaoh_info_monuments_draw_pomegranate_farm(monuments(p));}
static void monuments_chickpea_farm(void *p)
{pharaoh_info_monuments_draw_chickpea_farm(monuments(p));}
static void monuments_wood_cutter(void *p)
{pharaoh_info_monuments_draw_wood_cutter(monuments(p));}
static void monuments_jeweler(void *p)
{pharaoh_info_monuments_draw_jeweler(monuments(p));}
static void monuments_potter(void *p)
{pharaoh_info_monuments_draw_potter(monuments(p));}
static void monuments_ferry(void *p)
{pharaoh_info_monuments_draw_ferry(monuments(p));}
static void monuments_roadblock(void *p)
{pharaoh_info_monuments_draw_roadblock(monuments(p));}
static void monuments_brick_gatehouse(void *p)
{pharaoh_info_monuments_draw_brick_gatehouse(monuments(p));}
static void monuments_clay_gatehouse(void *p)
{pharaoh_info_monuments_draw_clay_gatehouse(monuments(p));}
static void monuments_brick_tower(void *p)
{pharaoh_info_monuments_draw_brick_tower(monuments(p));}
static void monuments_clay_tower(void *p)
{pharaoh_info_monuments_draw_clay_tower(monuments(p));}
static void monuments_transport_wharf(void *p)
{pharaoh_info_monuments_draw_transport_wharf(monuments(p));}
static void monuments_warship_wharf(void *p)
{pharaoh_info_monuments_draw_warship_wharf(monuments(p));}
static void monuments_pyramid(void *p)
{pharaoh_info_monuments_draw_pyramid(monuments(p));}
static void monuments_fig_farm(void *p)
{pharaoh_info_monuments_draw_fig_farm(monuments(p));}
static void monuments_papyrus_maker(void *p)
{pharaoh_info_monuments_draw_papyrus_maker(monuments(p));}
static void monuments_brickworks(void *p)
{pharaoh_info_monuments_draw_brickworks(monuments(p));}
static void monuments_chariot_maker(void *p)
{pharaoh_info_monuments_draw_chariot_maker(monuments(p));}
static void monuments_obelisk(void *p)
{pharaoh_info_monuments_draw_obelisk(monuments(p));}
static void monuments_sphinx(void *p)
{pharaoh_info_monuments_draw_sphinx(monuments(p));}
static void monuments_sun_temple(void *p)
{pharaoh_info_monuments_draw_sun_temple(monuments(p));}
static void monuments_mausoleum(void *p)
{pharaoh_info_monuments_draw_mausoleum(monuments(p));}
static void monuments_great_library(void *p)
{pharaoh_info_monuments_draw_great_library(monuments(p));}
static void monuments_caesareum(void *p)
{pharaoh_info_monuments_draw_caesareum(monuments(p));}
static void monuments_lighthouse(void *p)
{pharaoh_info_monuments_draw_lighthouse(monuments(p));}
static void monuments_royal_tomb(void *p)
{pharaoh_info_monuments_draw_royal_tomb(monuments(p));}
static void monuments_abu_simbel(void *p)
{pharaoh_info_monuments_draw_abu_simbel(monuments(p));}
static void monuments_lamp_maker(void *p)
{pharaoh_info_monuments_draw_lamp_maker(monuments(p));}
static void monuments_paint_maker(void *p)
{pharaoh_info_monuments_draw_paint_maker(monuments(p));}

static void (*const connected_catalog[237])(void *)={
    [37]=monuments_bullfight_school,
    [57]=monuments_fort,
    [58]=monuments_mud_gatehouse,
    [59]=monuments_mud_tower,
    [74]=monuments_shipwright,
    [75]=monuments_dock,
    [76]=monuments_fishing_wharf,
    [94]=monuments_military_academy,
    [95]=monuments_recruiter,
    [104]=monuments_pomegranate_farm,
    [105]=monuments_chickpea_farm,
    [108]=monuments_wood_cutter,
    [113]=monuments_jeweler,
    [114]=monuments_potter,
    [136]=monuments_ferry,
    [138]=monuments_roadblock,
    [170]=monuments_brick_gatehouse,
    [171]=monuments_clay_gatehouse,
    [172]=monuments_brick_tower,
    [173]=monuments_clay_tower,
    [181]=monuments_transport_wharf,
    [182]=monuments_warship_wharf,
    [183]=monuments_pyramid,
    [185]=monuments_military_academy,
    [186]=monuments_military_academy,
    [192]=monuments_dock,
    [196]=monuments_fig_farm,
    [200]=monuments_mud_gatehouse,
    [201]=monuments_brick_gatehouse,
    [202]=monuments_clay_gatehouse,
    [203]=monuments_papyrus_maker,
    [204]=monuments_brickworks,
    [205]=monuments_chariot_maker,
    [207]=monuments_obelisk,
    [208]=monuments_obelisk,
    [210]=monuments_sphinx,
    [213]=monuments_sun_temple,
    [222]=monuments_mausoleum,
    [225]=monuments_great_library,
    [227]=monuments_caesareum,
    [228]=monuments_lighthouse,
    [229]=monuments_royal_tomb,
    [230]=monuments_abu_simbel,
    [232]=monuments_lamp_maker,
    [233]=monuments_paint_maker,
    [234]=monuments_royal_tomb,
    [235]=monuments_royal_tomb,
    [236]=monuments_royal_tomb,

    [30]=services_bandstand,
    [31]=services_booth,
    [32]=services_senet_house,
    [33]=services_pavilion,
    [34]=services_conservatory,
    [35]=services_dance_school,
    [36]=services_juggler_school,
    [46]=services_apothecary,
    [47]=services_mortuary,
    [49]=services_dentist,
    [51]=services_scribal_school,
    [53]=services_library,
    [55]=services_police_station,
    [77]=services_mansion,
    [78]=services_mansion,
    [79]=services_mansion,
    [80]=services_mission_post,
    [81]=services_architect_post,
    [84]=services_palace,
    [85]=services_palace,
    [86]=services_tax_collector,
    [87]=services_tax_collector,
    [88]=services_native_hut,
    [89]=services_native_meeting,
    [93]=services_native_crops,
    [98]=services_oracle,
    [167]=services_firehouse,
    [184]=services_courthouse,
    [187]=services_palace,
    [188]=services_palace,
    [189]=services_palace,
    [206]=services_physician,
    [209]=services_festival_square,
    [226]=services_zoo,

    [10]=house,[11]=house,[12]=house,[13]=house,[14]=house,[15]=house,[16]=house,
    [17]=house,[18]=house,[19]=house,[20]=house,[21]=house,[22]=house,[23]=house,
    [24]=house,[25]=house,[26]=house,[27]=house,[28]=house,[29]=house,
    [41]=statue,[42]=statue,[43]=statue,[56]=arch,[99]=burning,
    [7]=industry_water_lift,[90]=industry_water_lift,[92]=industry_well,
    [100]=industry_barley,[101]=industry_flax,[102]=industry_grain,[103]=industry_lettuce,
    [106]=industry_stone,[109]=industry_clay,[110]=industry_brewery,[111]=industry_weaver,
    [112]=industry_weapons,[115]=industry_hunting,[161]=industry_gold,[162]=industry_gems,
    [177]=industry_carpenters,[178]=industry_bricklayers,[179]=industry_stonemasons,
    [180]=industry_water_supply,[194]=industry_cattle,[195]=industry_reeds,[199]=industry_work_camp,
    [217]=industry_copper,[224]=industry_henna,[231]=industry_artisans
};

void pharaoh_building_info_draw_bindings_initialize(PharaohBuildingInfoDrawBindings *b,
    const PharaohBuildingInfoDrawProviders *providers)
{
    b->providers=*providers;b->arithmetic=PHARAOH_PERCENTAGE_SCALE_VALUE;
    b->operations=(PharaohBuildingInfoLiveOperations){0};
    PharaohBuildingInfoLiveOperations *o=&b->operations;o->user_data=b;
    if(providers->storage) {
        o->border=border;o->text=text;o->centered=centered;o->outer_panel=outer;
        o->inner_panel=inner;o->image=image;o->bazaar=bazaar;o->granary=granary;o->storage_yard=storage;
    }
    if(providers->square_buttons)o->arrow_draw=arrows;
    if(providers->image_buttons)o->image_draw=buttons;
    if(providers->city)o->city_background=city;
    if(providers->sample_sound)o->play_sample=sample;
    if(providers->content)o->no_info=no_info;
    if(providers->terrain)o->terrain=terrain;
    if(providers->units) {
        o->legion=legion;o->transport=transport;o->warship=warship;o->legion_foreground=legion_foreground;
    }
    if(providers->industry)o->quarry=industry_quarry;
    if(providers->services){o->temple=services_temple;o->mansion_foreground=services_mansion_foreground;}
    o->building_background=connected_catalog;
    if(providers->ui)providers->ui->operations=o;
}

static enum PharaohBuildingInfoDrawSupport common(const PharaohBuildingInfoDrawBindings *b)
{
    if(!b||!b->providers.ui||!b->providers.ui->window||!b->providers.storage)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(b->arithmetic!=PHARAOH_PERCENTAGE_SCALE_VALUE)return PHARAOH_INFO_DRAW_DIVIDE_FAULT;
    const PharaohBuildingInfoDrawProviders *p=&b->providers;
    const PharaohBuildingInfoLiveContext *u=p->ui;
    const PharaohStorageInfoDrawContext *d=p->storage;
    if(u->operations!=&b->operations||d->ui!=u)return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    const PharaohBuildingInfoLiveOperations *o=&b->operations;
    if(!o->border||!o->text||!o->centered||!o->outer_panel||!o->inner_panel||
       !o->image||!o->arrow_draw||!o->image_draw)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(!u->buildings||!u->walkers||!d->layout||!d->text||!d->text->drawing||!d->wrapped||
       !d->wrapped->drawing||!d->layout->inner_panel||!d->layout->outer_panel||
       !d->layout->border||!d->layout->layout||!d->layout->highlight||!d->layout->window_id||
       !d->layout->inner_panel->drawing||!p->square_buttons||!p->image_buttons||
       !u->font_black||!u->font_green||!u->font_white||!u->font_red||!u->font_title||
       !d->font_black||!d->font_green||!d->font_white||!d->font_red||!d->font_title)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(!d->font_black->draw_glyph||!d->font_green->draw_glyph||!d->font_white->draw_glyph||
       !d->font_red->draw_glyph||!d->font_title->draw_glyph)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    const PharaohImageDrawContext *images=d->layout->inner_panel->drawing;
    if(!images->clip||!images->state||!images->surface_pixels||!d->text->selection||
       !d->text->glyph_map||!d->text->number_text||!d->text->current_text||!d->text->center_offset||
       !d->text->reset_flag||!d->wrapped->selection||!d->wrapped->current_text_00e20768||
       !d->wrapped->drawing->word_metrics||!d->wrapped->drawing->line_buffer_00e3439c||
       !d->wrapped->drawing->maximum_width_00e2076c||!p->square_buttons->current_image||
       !p->square_buttons->draw_x||!p->square_buttons->draw_y||!p->square_buttons->ui_redraw||
       !p->image_buttons->sprite_group_bases||
       !p->image_buttons->draw_x||!p->image_buttons->draw_y||!p->image_buttons->active_sprite_image||
       !p->image_buttons->ui_redraw||!p->image_buttons->window_redraw_requested||!p->image_buttons->force_refresh)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(d->layout->outer_panel->drawing!=images||d->layout->border->drawing!=images||
       p->square_buttons->drawing!=images||
       d->text->layout!=d->layout->layout||d->text->highlight!=d->layout->highlight||
       d->text->window_id!=d->layout->window_id||d->text->window_seven_layout_base!=d->layout->window_seven_layout_base||
       u->text_width!=&d->text->drawing->cursor_x||d->wrapped->drawing->draw_state!=d->text->drawing||
       p->image_buttons->draw_x!=&images->clip->draw_x||p->image_buttons->draw_y!=&images->clip->draw_y)
        return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    return PHARAOH_INFO_DRAW_READY;
}
static int city_ready(const PharaohCityRenderContext *c)
{
    return pharaoh_city_render_bound_for_current_mode(c)!=0;
}
static enum PharaohBuildingInfoDrawSupport content_ready(const PharaohBuildingInfoDrawBindings *b)
{
    const PharaohInfoContentDrawContext *c=b->providers.content;
    if(!c)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    return c->drawing==b->providers.storage?PHARAOH_INFO_DRAW_READY:PHARAOH_INFO_DRAW_STATE_MISMATCH;
}
static enum PharaohBuildingInfoDrawSupport walkers_ready(const PharaohBuildingInfoDrawBindings *b)
{
    enum PharaohBuildingInfoDrawSupport s=content_ready(b);if(s!=PHARAOH_INFO_DRAW_READY)return s;
    const PharaohBuildingInfoDrawProviders *p=&b->providers;
    const PharaohInfoWalkerContentContext *w=p->walkers;
    const PharaohInfoPreviewContext *v=p->content->previews;
    if(!w||!v||!w->portrait_codes||!w->portrait_scripts||!w->portrait_images||!w->font_name||
       !w->font_name_id||!w->supply||!w->traders||!w->classical_names||!w->image_buttons||
       !v->view||!v->view->state||!v->view->projection||!v->render||!v->surface_pitch||
       !v->preview_base||!v->preview_offsets)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(!w->portrait_codes->portrait_codes||!w->portrait_codes->selected_scenario||
       !w->supply->cities||!w->supply->route_limits||!w->supply->players||!w->supply->current_player||
       !v->grid_origin||!v->panel_position_mode||!v->projected_x||!v->projected_y||!v->images||
       !v->image_count||!v->menu_bar_images||!v->capture_x||!v->capture_y||!v->draw_x||
       !v->draw_y||!v->draw_width||!v->draw_height)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    const PharaohImageDrawContext *images=p->storage->layout->inner_panel->drawing;
    if(w->content!=p->content||v->ui!=p->ui||v->render!=p->city||v->view->state!=p->city->view||
       v->surface_pixels!=images->surface_pixels||*v->surface_pitch!=images->pitch_bytes||
       w->portrait_codes->walkers!=p->ui->walkers||w->portrait_codes->buildings!=p->ui->buildings||
       w->portrait_codes->scripts!=w->portrait_scripts||w->image_buttons!=p->image_buttons)
        return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    return PHARAOH_INFO_DRAW_READY;
}
static enum PharaohBuildingInfoDrawSupport unit_ready(const PharaohBuildingInfoDrawBindings *b)
{
    const PharaohBuildingInfoDrawProviders *p=&b->providers;
    const PharaohUnitPanelLiveContext *u=p->units;
    if(!u||!u->pixels||!u->images||!u->pixel_format||!u->pixel_read_scratch||!u->formations)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(!u->normal.id||!u->title.id||!u->heading.id||!u->description.id||
       !u->focus_primary||!u->focus_secondary||!u->transport_button_base||!u->warship_button_base||
       !u->experience_icon_base||!u->legion_badge_base||!u->legion_type_base||!u->legion_morale_base||
       !u->legion_experience_base||!u->charioteer_button_base||!u->archer_button_base||
       !u->infantry_button_base||!u->map_orientation||!u->native_7058b8)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    const PharaohStorageInfoDrawContext *d=p->storage;
    if(u->window!=p->ui->window||u->buildings!=p->ui->buildings||u->walkers!=p->ui->walkers||
       u->formations!=p->ui->formations||u->layout!=d->layout||u->text!=d->text||u->wrapped!=d->wrapped||
       u->text_width!=p->ui->text_width||u->images!=d->layout->inner_panel->drawing||
       (const void *)u->pixels->rgb565_pixels!=u->images->surface_pixels||
       u->pixels->rgb565_pitch_bytes!=u->images->pitch_bytes||u->normal.font!=d->font_black||
       u->title.font!=d->font_title||u->heading.font!=d->font_white||u->description.font!=d->font_green||
       u->normal.id!=p->ui->font_black||u->title.id!=p->ui->font_title||
       u->heading.id!=p->ui->font_white||u->description.id!=p->ui->font_green)
        return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    return PHARAOH_INFO_DRAW_READY;
}
static int industry_percentage(int32_t type)
{
    return industry_farm(type)||quarry_type(type)||type==106||type==109||type==110||
        type==111||type==112||type==161||type==162||type==177||type==194||type==217;
}
static int industry_mothball(int32_t type)
{
    switch(type) {
    case 100:case 101:case 102:case 103:case 106:case 107:case 109:case 110:
    case 111:case 112:case 115:case 161:case 162:case 194:case 195:case 216:
    case 217:case 221:case 224:return 1;
    default:return 0;
    }
}
static enum PharaohBuildingInfoDrawSupport industry_employment_ready(const PharaohBuildingInfoDrawBindings *b)
{
    const PharaohStorageInfoDrawContext *d=b->providers.storage;
    const PharaohBuildingInfoLiveContext *u=b->providers.ui;
    const PharaohBuildingTypeRecord *selected=&u->buildings[u->window->building_id];
    if(!d->models||!d->context_icons||!d->number_suffix||!d->employee_suffix)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(selected->num_workers<d->models[selected->building_type].laborers) {
        if(!d->city_population)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        if(*d->city_population>0&&selected->houses_covered>=40&&!d->labor)
            return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    }
    return PHARAOH_INFO_DRAW_READY;
}
static enum PharaohBuildingInfoDrawSupport industry_ready(const PharaohBuildingInfoDrawBindings *b,int32_t type)
{
    enum PharaohBuildingInfoDrawSupport s=content_ready(b);if(s!=PHARAOH_INFO_DRAW_READY)return s;
    const PharaohBuildingInfoDrawProviders *p=&b->providers;
    const PharaohInfoIndustryLiveContext *i=p->industry;
    if(!i)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(i->content!=p->content)return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    if(quarry_type(type)) {
        if(!b->operations.quarry)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    } else if(!b->operations.building_background||!b->operations.building_background[type])
        return PHARAOH_INFO_DRAW_UNSUPPORTED_TYPE;
    const PharaohBuildingInfoLiveContext *u=p->ui;
    const PharaohBuildingTypeRecord *selected=&u->buildings[u->window->building_id];
    if(type==92) {
        const PharaohMapHouseWaterContext *water=i->house_water;
        if(!water||!water->building_ids||!water->result_x||!water->result_y)
            return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        return water->buildings==u->buildings?PHARAOH_INFO_DRAW_READY:PHARAOH_INFO_DRAW_STATE_MISMATCH;
    }
    if(industry_percentage(type)&&!i->percent_suffix)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(u->window->details.road_access>0&&industry_mothball(type)&&(!i->mothball_banks||!i->current_player))
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    /* The quarry adapter captures these actual calendar scalars when forming
     * its unchanged508E00 query context; it does not consume harvest rows. */
    if(quarry_type(type)&&(!i->month||!i->flood_season_month))return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(industry_farm(type)) {
        const PharaohOccupancyState *o=p->content->occupancy;
        if(!o||!o->offsets||!o->terrain||!o->objects||!o->buildings||
           !i->farm_progress_max||!i->month||!i->flood_season_month||!i->food)
            return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        /* The existing query writes only last_origin. A small descriptor
         * probe keeps that write local; its grids and objects remain borrowed.
         * Readiness must not advance the actual Native occupancy scratch. */
        PharaohOccupancyState probe=*o;
        const uint8_t active=(uint8_t)pharaoh_building_has_active_occupant(&probe,u->window->building_id);
        if(!active&&!i->harvest_months)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        const PharaohFoodSupplyQueryContext *food=i->food;
        if(!food->map_origin||!food->current_tile||!food->soil_values||!food->footprints)
            return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        if(food->buildings!=u->buildings)return PHARAOH_INFO_DRAW_STATE_MISMATCH;
        if(selected->farm_water_access!=0&&
           (uint8_t)pharaoh_building_type_is_00488080_group(u->buildings,u->window->building_id)&&food->occupancy!=o)
            return PHARAOH_INFO_DRAW_STATE_MISMATCH;
        if(active) {
            /*4FC4F0 draws its actual occupant branch instead of employment. */
            return p->storage->context_icons?PHARAOH_INFO_DRAW_READY:PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        }
    }
    return industry_employment_ready(b);
}
static enum PharaohBuildingInfoDrawSupport services_ready(const PharaohBuildingInfoDrawBindings *b)
{
    const PharaohInfoServicesLiveContext *s=b->providers.services;
    const PharaohStorageInfoDrawContext *d=b->providers.storage;
    if(!s||!s->drawing||!s->players||!s->current_player||!s->month||!s->space_suffix||!s->percent_suffix||
       !s->square_buttons||!s->tax_arrows||!d->labor||!d->city_population||!d->models)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(s->drawing!=d||s->square_buttons!=b->providers.square_buttons)
        return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    return industry_employment_ready(b);
}

static const uint64_t monuments_required[237]={
    [37]=UINT64_C(0x1000000),
    [57]=UINT64_C(0x100),
    [58]=UINT64_C(0x100),
    [59]=UINT64_C(0x100),
    [74]=UINT64_C(0x900),
    [75]=UINT64_C(0x100),
    [76]=UINT64_C(0x80900),
    [94]=UINT64_C(0x100),
    [95]=UINT64_C(0x100),
    [104]=UINT64_C(0x800),
    [105]=UINT64_C(0x800),
    [108]=UINT64_C(0x800),
    [113]=UINT64_C(0x800),
    [114]=UINT64_C(0x800),
    [136]=UINT64_C(0x800100),
    [138]=UINT64_C(0x100),
    [170]=UINT64_C(0x100),
    [171]=UINT64_C(0x100),
    [172]=UINT64_C(0x100),
    [173]=UINT64_C(0x100),
    [181]=UINT64_C(0x800100),
    [182]=UINT64_C(0x800100),
    [183]=UINT64_C(0x300702ff6),
    [185]=UINT64_C(0x100),
    [186]=UINT64_C(0x100),
    [192]=UINT64_C(0x100),
    [196]=UINT64_C(0x800),
    [200]=UINT64_C(0x100),
    [201]=UINT64_C(0x100),
    [202]=UINT64_C(0x100),
    [203]=UINT64_C(0x800),
    [204]=UINT64_C(0x800),
    [205]=UINT64_C(0x800),
    [207]=UINT64_C(0x38600c184),
    [208]=UINT64_C(0x38600c184),
    [210]=UINT64_C(0x981029a4),
    [213]=UINT64_C(0x3601003b6),
    [222]=UINT64_C(0x3001003b6),
    [225]=UINT64_C(0x3001331a6),
    [227]=UINT64_C(0x3001031ae),
    [228]=UINT64_C(0x3001431a6),
    [229]=UINT64_C(0x1f00003186),
    [230]=UINT64_C(0x300003187),
    [232]=UINT64_C(0x800),
    [233]=UINT64_C(0x800),
    [234]=UINT64_C(0x1f00003186),
    [235]=UINT64_C(0x1f00003186),
    [236]=UINT64_C(0x1f00003186),
};
static enum PharaohBuildingInfoDrawSupport monuments_ready(const PharaohBuildingInfoDrawBindings *b,int32_t type)
{
    const PharaohInfoMonumentsLiveContext *c=b->providers.monuments;
    if(!c)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    uint64_t available=0;
    if(c->abu_simbel_spots)available|=UINT64_C(1)<<0;
    if(c->building_count_active)available|=UINT64_C(1)<<1;
    if(c->building_count_total)available|=UINT64_C(1)<<2;
    if(c->caesareum_spots)available|=UINT64_C(1)<<3;
    if(c->complex)available|=UINT64_C(1)<<4;
    if(c->current_player)available|=UINT64_C(1)<<5;
    if(c->definitions)available|=UINT64_C(1)<<6;
    if(c->draw_y_accum)available|=UINT64_C(1)<<7;
    if(c->drawing)available|=UINT64_C(1)<<8;
    if(c->flood_state)available|=UINT64_C(1)<<9;
    if(c->goal_definition_ids)available|=UINT64_C(1)<<10;
    if(c->industry)available|=UINT64_C(1)<<11;
    if(c->kingdom_access)available|=UINT64_C(1)<<12;
    if(c->land_citizen)available|=UINT64_C(1)<<13;
    if(c->large_obelisk_counts)available|=UINT64_C(1)<<14;
    if(c->large_obelisk_spots)available|=UINT64_C(1)<<15;
    if(c->library_progress)available|=UINT64_C(1)<<16;
    if(c->library_spots)available|=UINT64_C(1)<<17;
    if(c->lighthouse_spots)available|=UINT64_C(1)<<18;
    if(c->map_sweep_countdown)available|=UINT64_C(1)<<19;
    if(c->players)available|=UINT64_C(1)<<20;
    if(c->pyramid)available|=UINT64_C(1)<<21;
    if(c->pyramid_report)available|=UINT64_C(1)<<22;
    if(c->road_access)available|=UINT64_C(1)<<23;
    if(c->services)available|=UINT64_C(1)<<24;
    if(c->small_obelisk_counts)available|=UINT64_C(1)<<25;
    if(c->small_obelisk_spots)available|=UINT64_C(1)<<26;
    if(c->sphinx_assignments)available|=UINT64_C(1)<<27;
    if(c->sphinx_percent_spots)available|=UINT64_C(1)<<28;
    if(c->sun_temple_report)available|=UINT64_C(1)<<29;
    if(c->sun_temple_spots)available|=UINT64_C(1)<<30;
    if(c->tile_building_ids)available|=UINT64_C(1)<<31;
    if(c->token_entries)available|=UINT64_C(1)<<32;
    if(c->token_operations)available|=UINT64_C(1)<<33;
    if(c->tomb_report)available|=UINT64_C(1)<<34;
    if(c->victory_required)available|=UINT64_C(1)<<35;
    if(c->victory_stored)available|=UINT64_C(1)<<36;
    if((available&monuments_required[type])!=monuments_required[type])return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(c->drawing!=b->providers.storage)return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    if(c->industry&&(c->industry!=b->providers.industry||c->industry->content!=b->providers.content))return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    if(c->services&&(c->services!=b->providers.services||c->services->drawing!=b->providers.storage))return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    if(c->current_player&&c->industry&&c->industry->current_player!=c->current_player)return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    if(c->current_player&&c->services&&c->services->current_player!=c->current_player)return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    if(c->draw_y_accum&&c->draw_y_accum!=&c->drawing->text->drawing->cursor_y)return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    return PHARAOH_INFO_DRAW_READY;
}

static enum PharaohBuildingInfoDrawSupport building_ready(const PharaohBuildingInfoDrawBindings *b,int32_t type)
{
    const enum PharaohBuildingInfoDrawTypeSupport type_support=pharaoh_building_info_draw_type_support(type);
    if(type_support==PHARAOH_INFO_DRAW_MISSING_TYPE_PROVIDER||type_support==PHARAOH_INFO_DRAW_OUTSIDE_TYPE_CATALOG)
        return PHARAOH_INFO_DRAW_UNSUPPORTED_TYPE;
    if(type_support==PHARAOH_INFO_DRAW_NATIVE_DEFAULT)return PHARAOH_INFO_DRAW_READY;
    if(industry_type(type))return industry_ready(b,type);
    if(monuments_type(type))return monuments_ready(b,type);
    if(services_type(type)) {
        if(temple_type(type)&&!b->operations.temple)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        return services_ready(b);
    }
    if(storage_type(type)) {
        const PharaohBuildingInfoLiveContext *u=b->providers.ui;
        const PharaohStorageInfoDrawContext *d=b->providers.storage;
        if(!u->storage||!u->focus_primary||!u->focus_secondary||!u->display_mode||!u->food_count||
           !u->foods||!u->resource_count||!u->resources||!d->models||!d->labor||!d->city_population||
           !d->kingdom_supplies_grain||!d->context_icons||!d->number_suffix||!d->employee_suffix)
            return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        if(!u->window->orders_shown&&
           (((type==70||type==190)&&!b->operations.bazaar)||
            ((type==71||type==191)&&!b->operations.granary)||
            ((type==72||type==193)&&!b->operations.storage_yard)))return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        return PHARAOH_INFO_DRAW_READY;
    }
    if(!b->operations.building_background||!b->operations.building_background[type])
        return PHARAOH_INFO_DRAW_UNSUPPORTED_TYPE;
    if(type==99)return content_ready(b);
    enum PharaohBuildingInfoDrawSupport s=walkers_ready(b);if(s!=PHARAOH_INFO_DRAW_READY)return s;
    if(type<10||type>29)return PHARAOH_INFO_DRAW_READY;
    const PharaohInfoHouseContentContext *h=b->providers.house;
    if(!h||!h->houses||!h->models||!h->roads||!h->trade||!h->list_or||!h->comma||!h->close_parenthesis)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(h->walkers!=b->providers.walkers||(const void *)h->houses!=b->providers.ui->buildings)
        return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    return PHARAOH_INFO_DRAW_READY;
}

enum PharaohBuildingInfoDrawSupport pharaoh_building_info_draw_background_support(
    const PharaohBuildingInfoDrawBindings *b)
{
    enum PharaohBuildingInfoDrawSupport s=common(b);if(s!=PHARAOH_INFO_DRAW_READY)return s;
    const PharaohBuildingInfoDrawProviders *p=&b->providers;
    if(!b->operations.city_background||!city_ready(p->city))return PHARAOH_INFO_DRAW_MISSING_CITY_PASS;
    const PharaohBuildingInfoLiveContext *u=p->ui;
    if(!u->sound_channels||!u->sound_enabled||!u->sound_volume)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    const PharaohBuildingTypeRecord *selected=&u->buildings[u->window->building_id];
    int32_t sound_type=selected->building_type;
    if(sound_type==57) {
        if(selected->subtype==13)sound_type=40;
        if(selected->subtype==11)sound_type=44;
        if(selected->subtype==12)sound_type=45;
    }
    if((uint32_t)sound_type>=237)return PHARAOH_INFO_DRAW_UNSUPPORTED_TYPE;
    if(u->window->mode==2&&u->window->first_draw&&*u->sound_enabled&&u->sound_channels[sound_type*5]>-1&&
       (!b->operations.play_sample||!p->sample_sound||!p->sample_sound->state||!p->sample_sound->operations))
        return PHARAOH_INFO_DRAW_MISSING_SAMPLE_SOUND;
    switch(u->window->mode) {
    case 0:return b->operations.no_info?content_ready(b):PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    case 1: {
        if(u->window->walker_count==0) {
            if(!b->operations.terrain||!p->terrain||!p->walkers||!p->content||!p->content->previews)
                return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
            if(p->terrain->walkers!=p->walkers||p->walkers->content!=p->content||
               p->content->drawing!=p->storage||p->content->previews->ui!=u)
                return PHARAOH_INFO_DRAW_STATE_MISMATCH;
            /*4FA3C0 returns before capture reads.4FA780 still draws its real
             * inner panel/no-walkers text and sets selection1; do not skip it. */
            return PHARAOH_INFO_DRAW_READY;
        }
        s=walkers_ready(b);if(s!=PHARAOH_INFO_DRAW_READY)return s;
        if(!b->operations.terrain||!p->terrain||!p->terrain->selection_voice)
            return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        const PharaohInfoWalkerSelectionVoiceContext *voice=p->terrain->selection_voice;
        if(!voice->sound||!voice->sound->operations||!voice->sound->device||
           !voice->voice||!voice->voice->stream||!voice->voice->voice_names||!voice->voice->special_voice_names||
           !voice->voice->stream->operations||!voice->voice->stream->device)
            return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        if(p->terrain->selection_voice->ui!=p->ui||p->terrain->selection_voice->portrait!=p->walkers->portrait_codes)
            return PHARAOH_INFO_DRAW_STATE_MISMATCH;
        return p->terrain->walkers==p->walkers?PHARAOH_INFO_DRAW_READY:PHARAOH_INFO_DRAW_STATE_MISMATCH;
    }
    case 2:return building_ready(b,selected->building_type);
    case 4:return b->operations.legion?unit_ready(b):PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    case 5:return b->operations.warship?unit_ready(b):PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    case 6:return b->operations.transport?unit_ready(b):PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    default:return PHARAOH_INFO_DRAW_READY;
    }
}
enum PharaohBuildingInfoDrawSupport pharaoh_building_info_draw_foreground_support(
    const PharaohBuildingInfoDrawBindings *b)
{
    enum PharaohBuildingInfoDrawSupport s=common(b);if(s!=PHARAOH_INFO_DRAW_READY)return s;
    const PharaohBuildingInfoLiveContext *u=b->providers.ui;
    if(u->window->mode==4)return b->operations.legion_foreground?unit_ready(b):PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(u->window->mode==1&&(!u->phrase_counts||!u->focus_secondary))return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(u->window->mode!=2)return PHARAOH_INFO_DRAW_READY;
    const int32_t type=u->buildings[u->window->building_id].building_type;
    if(type==77||type==78)
        return b->operations.mansion_foreground?services_ready(b):PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    /* Actual504630 is square-button input as well as focus. Its40A260 tax
     * action remains unbound; retain an explicit gap before any stores. */
    if(type==187||type==188||type==189||type==86||type==87)
        return PHARAOH_INFO_DRAW_UNSUPPORTED_TYPE;
    if(storage_type(type)&&(!u->focus_primary||!u->display_mode||!u->resource_count||!u->food_count))
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    return PHARAOH_INFO_DRAW_READY;
}
PharaohBuildingInfoDrawResult pharaoh_building_info_draw_bindings_background(PharaohBuildingInfoDrawBindings *b)
{
    const enum PharaohBuildingInfoDrawSupport s=pharaoh_building_info_draw_background_support(b);
    if(s==PHARAOH_INFO_DRAW_READY)pharaoh_building_info_live_draw_background(b->providers.ui);
    const PharaohPercentageScaleStatus a=b?b->arithmetic:PHARAOH_PERCENTAGE_SCALE_VALUE;
    const PharaohBuildingInfoDrawResult r={a==PHARAOH_PERCENTAGE_SCALE_VALUE?s:PHARAOH_INFO_DRAW_DIVIDE_FAULT,a};
    return r;
}
PharaohBuildingInfoDrawResult pharaoh_building_info_draw_bindings_foreground(PharaohBuildingInfoDrawBindings *b)
{
    const enum PharaohBuildingInfoDrawSupport s=pharaoh_building_info_draw_foreground_support(b);
    if(s==PHARAOH_INFO_DRAW_READY)pharaoh_building_info_live_draw_foreground(b->providers.ui);
    const PharaohPercentageScaleStatus a=b?b->arithmetic:PHARAOH_PERCENTAGE_SCALE_VALUE;
    const PharaohBuildingInfoDrawResult r={s,a};return r;
}
