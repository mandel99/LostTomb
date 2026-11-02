#include "mausoleum.h"
#include "city_footprint_validation.h"
#include "building_allocator.h"
#include "building_support.h"
#include "editor_brush_validation.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole4DF9D0: 116 geometry records, a root and 115 linked allocations.
   Geometry size is a BYTE, piece/link indices are WORDs; progress, origin
   and saved orientation occupy DWORDs in each 264-byte building record. */
void FUN_004df9d0(int32_t origin_x,int32_t origin_y,int32_t building,uint8_t loaded)
{
    int32_t geometry[13],piece;
    uint32_t slot=0,orientation,offset,previous=(uint32_t)building,current,x,y;
    if(!loaded){slot=(uint32_t)FUN_004f4f80(building);orientation=U(0xbca2e0u);}
    else orientation=(U(0x702798u)&0xffffu)>>13;
    restore_city_geometry_004ded50(geometry,0,I(U(0xbca2e0u)),I(orientation));
    offset=(uint32_t)building*264u;x=(uint32_t)origin_x+(uint32_t)geometry[0];y=(uint32_t)origin_y+(uint32_t)geometry[1];
    W(0x7999deu+offset)=(uint16_t)x;W(0x7999e0u+offset)=(uint16_t)y;
    U(0x7999e4u+offset)=U(0xbca2d8u)+y*228u+x;
    W(0x7999eau+offset)=0;B(0x7999dbu+offset)=(uint8_t)geometry[2];
    W(0x799a10u+offset)=0;U(0x799a2cu+offset)=slot;U(0x799a30u+offset)=0;
    U(0x799a38u+offset)=1;U(0x799a34u+offset)=0;
    U(0x799a40u+offset)=(uint32_t)origin_x;U(0x799a44u+offset)=(uint32_t)origin_y;U(0x799a48u+offset)=orientation;
    FUN_004dfbb0(building);(void)FUN_00485100(building);
    for(piece=1;piece<116;++piece){
        restore_city_geometry_004ded50(geometry,piece,I(U(0xbca2e0u)),I(orientation));
        current=(uint32_t)restore_building_allocate_0046a400(U(0xeaac28u),222,
            I((uint32_t)origin_x+(uint32_t)geometry[0]),I((uint32_t)origin_y+(uint32_t)geometry[1]),0,I(previous+1u));
        offset=current*264u;
        W(0x799a10u+offset)=(uint16_t)previous;W(0x799a12u+offset)=0;
        W(0x799a12u+previous*264u)=(uint16_t)current;
        W(0x7999eau+offset)=(uint16_t)piece;B(0x7999dbu+offset)=(uint8_t)geometry[2];
        U(0x799a2cu+offset)=slot;U(0x799a30u+offset)=0;
        U(0x799a38u+offset)=0;U(0x799a34u+offset)=0;
        U(0x799a40u+offset)=(uint32_t)origin_x;U(0x799a44u+offset)=(uint32_t)origin_y;U(0x799a48u+offset)=orientation;
        previous=current;FUN_004dfbb0(I(current));(void)FUN_00485100(I(current));
    }
}
