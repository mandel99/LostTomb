#include "mausoleum.h"
#include "city_footprint_validation.h"
#include "building_image.h"
#include "road_place.h"
#include "terrain_redraw.h"
#include "map_refresh_postpass1.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern void FUN_00475bf0(int32_t,int32_t,int32_t,int32_t);

/* Whole4DFBB0. The image mode is a DWORD output; geometry is the complete
   thirteen-DWORD record. Terrain/image maps use DWORDs, ownership WORDs,
   and the footprint/edge/noise maps use BYTEs. */
void FUN_004dfbb0(int32_t building)
{
    int32_t geometry[13],size,row,column;
    uint32_t mode=0,offset=(uint32_t)building*264u,image,base,tile,flags,x,y;
    restore_city_geometry_004ded50(geometry,(int32_t)(int16_t)W(0x7999eau+offset),
        I(U(0xbca2e0u)),I(U(0x799a48u+offset)));
    image=(uint32_t)FUN_004df360(building,&mode);size=geometry[2];
    x=(uint32_t)(int32_t)(int16_t)W(0x7999deu+offset);y=(uint32_t)(int32_t)(int16_t)W(0x7999e0u+offset);
    if(!(mode&1u)){FUN_0047bf50((uint16_t)building,I(x),I(y),size,size,image,8);return;}
    base=U(0xbca2d8u)+y*228u+x;U(0xbc9768u)=base;
    for(row=0;row<size;++row)for(column=0;column<size;++column){
        tile=base+U(0x5cc8c0u+(uint32_t)row*48u+(uint32_t)column*8u);
        flags=U(0xaf598cu+tile*4u)|0x10000000u;
        W(0xb4e6fcu+tile*2u)=(uint16_t)building;
        B(0xb285ccu+tile)&=0xe0u;B(0xb67d1cu+tile)|=0x40u;U(0xaf598cu+tile*4u)=flags;
        if(!image)U(0xb7482cu+tile*4u)=0;
        else{
            U(0xb7482cu+tile*4u)=image+(image==U(0xf56b54u)?B(0xacf85cu+tile)&7u:0u);
            U(0xaf598cu+tile*4u)=flags&((image==U(0xf57464u)||image==U(0xf5736cu))?0xfefed7feu:0xfefed77eu);
        }
    }
    if(!image){
        int32_t right=I(x+(uint32_t)size-1u),bottom=I(y+(uint32_t)size-1u);
        FUN_00475bf0(I(x),I(y),right,bottom);
        FUN_004767d0(I(x),I(y),right,bottom,0,1);
        FUN_00477500(I(x),I(y),right,bottom);
    }
}
