#include "pharaoh/rendering/building_stock_draw.h"
#include <stddef.h>

_Static_assert(offsetof(PharaohBuildingTypeRecord,building_type)==0x10 &&
    offsetof(PharaohBuildingTypeRecord,loads_stored_render)==0x3c &&
    offsetof(PharaohBuildingTypeRecord,industry_orientation)==0xaa &&
    offsetof(PharaohBuildingTypeRecord,secondary_input_stock)==0xcc,
    "native workshop-stock fields");

enum StockGroup {StockSchool=329,StockSenet=467,StockMortuary=468,
    StockPotter=469,StockRecruiter=470,StockWood=471,StockWeapons=472,
    StockReeds=473,StockMeat=474,StockPapyrus=475,StockClay=476,
    StockBarley=477,StockFlax=478,StockLinen=479,StockLampsOil=738,
    StockLampsClay=739,StockPaint=740,StockArtisans=744};

static void stock(PharaohBuildingStockDrawContext *c,int32_t level,
    int32_t maximum,int32_t group,int32_t x,int32_t y,int32_t dx,int32_t dy)
{
    if(level<0)return;if(level>maximum)level=maximum;
    (void)pharaoh_image_draw(c->draw,
        (int32_t)c->groups[group].first_image+level,x+dx,y+dy);
}

void pharaoh_building_draw_stock(PharaohBuildingStockDrawContext *c,
    int32_t id,int32_t x,int32_t y)
{
    const PharaohBuildingTypeRecord *b=&c->buildings[id];
    int32_t first=(b->loads_stored_render-100)/100;
    int32_t second=(b->secondary_input_stock-100)/100;
    switch(b->building_type){
    case 32:stock(c,first,2,StockSenet,x,y,107,5);break;
    case 47:stock(c,first,4,StockMortuary,x,y,49,16);break;
    case 51:stock(c,first,2,StockSchool,x,y,28,15);break;
    case 53:stock(c,first,2,StockSchool,x,y,61,-13);break;
    case 74:if(first>=0){static const int16_t places[4][2]={{70,25},{120,-25},{90,10},{40,-5}};int32_t view=b->industry_orientation-(int32_t)*c->orientation/2;if(view<0)view+=4;stock(c,first,4,StockWood,x,y,places[view][0],places[view][1]);}break;
    case 95:stock(c,first,1,StockRecruiter,x,y,131,2);stock(c,second,1,StockFlax,x,y,74,10);break;
    case 108:stock(c,first,4,StockWood,x,y,65,3);break;
    case 110:stock(c,first,2,StockBarley,x,y,43,-1);break;
    case 111:stock(c,first,1,StockLinen,x,y,41,1);break;
    case 112:stock(c,first,1,StockWeapons,x,y,69,19);break;
    case 113:stock(c,first,1,StockPotter,x,y,63,-1);break;
    case 114:stock(c,first,1,StockClay,x,y,65,20);break;
    case 115:stock(c,first,4,StockMeat,x,y,61,13);break;
    case 177:stock(c,first,4,StockWood,x,y,41,8);break;
    case 194:stock(c,first,1,StockReeds,x,y,38,7);break;
    case 195:stock(c,first,4,StockPapyrus,x,y,34,17);break;
    case 203:stock(c,first,4,StockPapyrus,x,y,35,4);break;
    case 204:stock(c,second,1,StockReeds,x,y,91,2);stock(c,first,1,StockClay,x,y,52,-17);break;
    case 205:stock(c,first,4,StockWood,x,y,166,-23);break;
    case 226:stock(c,first,2,StockMeat,x,y,200,-10);stock(c,second,2,StockReeds,x,y,220,10);break;
    case 227:stock(c,second,4,StockClay,x,y,1,7);stock(c,first,4,StockArtisans,x,y,46,12);break;
    case 228:stock(c,second,1,StockLampsOil,x,y,50,20);stock(c,first,1,StockLampsClay,x,y,15,8);break;
    case 229:stock(c,first,1,StockPaint,x,y,45,25);break;
    default:break;
    }
}
