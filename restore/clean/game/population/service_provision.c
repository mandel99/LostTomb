#include "pharaoh/population/service_provision.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohNativeHouseServiceRecord)==264,"native building stride");
_Static_assert(offsetof(PharaohNativeHouseServiceRecord,house_size)==5,"occupied house BYTE");
_Static_assert(offsetof(PharaohNativeHouseServiceRecord,population)==0x1c,"population WORD");
_Static_assert(offsetof(PharaohNativeHouseServiceRecord,juggler)==0x6c,"coverage BYTEs");
_Static_assert(offsetof(PharaohNativeHouseServiceRecord,religion)==0x79,"deity BYTEs");
_Static_assert(offsetof(PharaohNativeHouseServiceRecord,zoo)==0x86,"zoo BYTE");
_Static_assert(offsetof(PharaohNativeHouseServiceRecord,bast_flag)==0x87,"Bast BYTE");
_Static_assert(offsetof(PharaohNativeHouseServiceRecord,missionary_flag)==0xc2,"missionary BYTE");
_Static_assert(offsetof(PharaohNativeHouseServiceRecord,crime)==0xe4,"crime DWORD");
_Static_assert(offsetof(PharaohNativeHouseServiceRecord,apothecary_health)==0xe8,"health DWORD");
_Static_assert(offsetof(PharaohNativeHouseServiceRecord,physician_health)==0xfc,"physician DWORD");
_Static_assert(sizeof(PharaohHouseServiceModel)==88,"model stride");

enum ServiceEffect { STAMP,HEALTH,BAST,CRIME,MISSIONARY,TAX };

/* Whole effects reused from ActorServices/HouseServices/BuildingServiceEffects
 * and TaxCollector C. Select actual algorithms directly, without callbacks. */
static uint32_t service_cell_effect(const PharaohHouseServiceContext *c,int32_t id,
    enum ServiceEffect effect,uint32_t field,uint32_t amount)
{
    volatile PharaohNativeHouseServiceRecord *h;
    if(effect==STAMP||effect==TAX){
        if(id<=0)return 0;
        h=&c->buildings[id];
        if(!h->house_size||h->population<=0)return 0;
        if(effect==TAX){
            const int32_t metric=c->models[h->subtype].tax_metric;
            if(metric>*c->maximum_tax_metric)*c->maximum_tax_metric=metric;
            h->tax_coverage=50;
        }else if(field!=UINT32_MAX){*((volatile uint8_t *)h+field)=96;}
        return 1;
    }
    if(effect==MISSIONARY){
        if(id>0){h=&c->buildings[id];if(h->type==88||h->type==89)h->missionary_flag=0;}
        return 0;
    }
    if(id==0)return 0;
    h=&c->buildings[id];
    if(effect==CRIME){
        uint32_t value=(uint32_t)h->crime;
        if((int32_t)value>*c->maximum_crime)*c->maximum_crime=(int32_t)value;
        value-=amount;h->crime=(int32_t)value;
        const int32_t minimum=c->models[h->subtype].minimum_crime;
        if((int32_t)value<minimum)h->crime=minimum;
        if(h->crime>1000)h->crime=1000;
        return 0;
    }
    if(h->type>=10&&h->type<=29&&h->state){
        if(effect==BAST)h->bast_flag=0;
        else{
            volatile int32_t *health=(volatile int32_t *)((volatile uint8_t *)h+field);
            const uint32_t value=(uint32_t)*health-amount;
            *health=(int32_t)value;if((int32_t)value<0)*health=0;
        }
    }
    return 0;
}

/* Reuse the whole ordinary RestoreHouseServiceScan algorithm. Native loops
 * publish endpoint coordinates before cells; cursor publication varies. */
static int32_t service_scan(const PharaohHouseServiceContext *c,int32_t x,int32_t y,
    uint32_t radius,enum ServiceEffect effect,uint32_t field,uint32_t amount,uint32_t publication)
{
    uint32_t left=(uint32_t)x-radius,top=(uint32_t)y-radius;
    uint32_t right=(uint32_t)x+radius,bottom=(uint32_t)y+radius;
    uint32_t width,height,tile,gap,rows,cells,scan,count=0;
    if((int32_t)left<0)left=0;if((int32_t)top<0)top=0;
    width=(uint32_t)*c->map_width;if((int32_t)right>=(int32_t)width)right=width-1u;
    height=(uint32_t)*c->map_height;if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    *c->row=top;tile=*c->grid_base+top*228u+left;gap=227u+left-right;*c->cursor=tile;
    if((int32_t)top>(int32_t)bottom)return 0;
    rows=bottom-top+1u;*c->row=bottom+1u;
    do{
        *c->column=left;
        if((int32_t)left<=(int32_t)right){
            cells=right-left+1u;scan=tile;tile+=cells;
            if(publication&1u)*c->cursor=tile;
            *c->column=right+1u;
            do{
                count+=service_cell_effect(c,c->building_grid[scan],effect,field,amount);
                ++scan;
            }while(--cells);
        }
        tile+=gap;if(publication&2u)*c->cursor=tile;
    }while(--rows);
    if(!(publication&2u))*c->cursor=tile;
    return (int32_t)count;
}

/* Whole004833C0. */
int32_t pharaoh_house_service_juggler(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    return service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,juggler),0,2);
}

/* Whole004834D0. */
int32_t pharaoh_house_service_musician(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    return service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,musician),0,2);
}

/* Whole004835E0. */
int32_t pharaoh_house_service_dancer(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    return service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,dancer),0,2);
}

/* Whole004836F0. */
int32_t pharaoh_house_service_senet(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    return service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,senet),0,2);
}

/* Whole00483800. */
int32_t pharaoh_house_service_zoo(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    return service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,zoo),0,2);
}

/* Whole00484120. */
int32_t pharaoh_house_service_school(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    return service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,school),0,2);
}

/* Whole00484230. */
int32_t pharaoh_house_service_library(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    return service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,library),0,2);
}

/* Whole00484340. */
int32_t pharaoh_house_service_dentist(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    return service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,dentist),0,2);
}

/* Whole00484450. */
int32_t pharaoh_house_service_physician(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    return service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,physician),0,2);
}

/* Whole00484560. */
int32_t pharaoh_house_service_apothecary(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    return service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,apothecary),0,2);
}

/* Whole00484780. */
int32_t pharaoh_house_service_mortuary(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    return service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,mortuary),0,2);
}

/* Whole00482DD0. */
uint32_t pharaoh_house_service_apothecary_health(const PharaohHouseServiceContext *c,int32_t x,int32_t y,int32_t amount)
{
    (void)service_scan(c,x,y,2,HEALTH,offsetof(PharaohNativeHouseServiceRecord,apothecary_health),(uint32_t)amount,2);return 0;
}

/* Whole00482EF0. */
void pharaoh_house_service_physician_health(const PharaohHouseServiceContext *c,int32_t x,int32_t y,int32_t amount)
{
    (void)service_scan(c,x,y,2,HEALTH,offsetof(PharaohNativeHouseServiceRecord,physician_health),(uint32_t)amount,0);
}

/* Whole00483160. */
uint32_t pharaoh_house_service_bast(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    (void)service_scan(c,x,y,2,BAST,0,0,2);return 0;
}

/* Whole00483260. */
void pharaoh_house_service_crime(const PharaohHouseServiceContext *c,int32_t x,int32_t y,int32_t amount)
{
    *c->maximum_crime=0;
    (void)service_scan(c,x,y,2,CRIME,0,(uint32_t)amount,3);
}

/* Whole00483E90. */
int32_t pharaoh_house_service_water(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    if(*c->water_service_blocked>0)return 0;
    return service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,water),0,0);
}

/* Whole00483FB0. */
int32_t pharaoh_house_service_religion(const PharaohHouseServiceContext *c,int32_t x,int32_t y,int32_t deity)
{
    const uint32_t field=(uint32_t)deity<=4u?(uint32_t)offsetof(PharaohNativeHouseServiceRecord,religion)+(uint32_t)deity:UINT32_MAX;
    return service_scan(c,x,y,2,STAMP,field,0,2);
}

/* Whole00484670. */
void pharaoh_house_service_magistrate(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    (void)service_scan(c,x,y,2,STAMP,offsetof(PharaohNativeHouseServiceRecord,magistrate),0,0);
}

/* Whole00484890. */
uint32_t pharaoh_house_service_missionary(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    (void)service_scan(c,x,y,4,MISSIONARY,0,0,2);return 1;
}

/* Whole00484990. */
int32_t pharaoh_house_service_labor_seeker(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    return service_scan(c,x,y,2,STAMP,UINT32_MAX,0,2);
}

/* Whole00484A90. */
int32_t pharaoh_house_service_tax(const PharaohHouseServiceContext *c,int32_t x,int32_t y)
{
    *c->maximum_tax_metric=0;
    return service_scan(c,x,y,2,TAX,0,0,0);
}
