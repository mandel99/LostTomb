#include "building_destruction.h"
#include "building_allocator.h"
#include "building_release_model.h"
#include "building_removal.h"
#include "clear_footprint.h"
#include "displaced_population.h"
#include "menu_prepare_integer.h"
#include "road_place.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))
extern void FUN_005178b0(void);

uint32_t restore_building_destruction_root(uint32_t building)
{return FUN_00475b50(building);}

/* Whole4BFEE0 caches the record coordinates before all four children, but
   reloads its BYTE orientation for each part. Occupancy dimensions are
   published in two DWORD globals before the three-argument clear child. */
void restore_building_clear_four_part_occupancy(int32_t building)
{
 uint32_t record=(uint32_t)building*264u,x=(uint32_t)H(0x7999deu+record),y=(uint32_t)H(0x7999e0u+record),part,at,px,py,width;
 uint8_t angle;
 for(part=0;part<4u;++part){
  at=0x5df818u+part*16u;angle=B(0x799a82u+record);px=x;py=y;
  if(angle==0u){px+=U(at+4u);py+=U(at);}else if(angle==1u){px+=U(at);py+=U(at+4u);}
  width=U(at-4u);U(0xbc9738u)=width;U(0xbc973cu)=width;FUN_0047c480(building,I(px),I(py));
 }
}

void restore_population_remove_destroyed(int32_t player,int32_t people)
{
 uint32_t off=(uint32_t)player*0x49d8u,index,total=0;
 U(0xea3f54u+off)=U(0xea3f54u+off)+(uint32_t)people;FUN_004e9350(player,people);U(0xea1894u+off)=0;
 for(index=0;index<100u;++index){total=(uint32_t)H(0xea3e34u+off+index*2u)+U(0xea1894u+off);U(0xea1894u+off)=total;}
 if(I(total)>I(U(0xea5f0cu+off)))U(0xea5f0cu+off)=total;
}

static uint32_t rubble_image(uint32_t record,uint32_t house_variant)
{
 uint32_t cell=U(0x7999e4u+record),variant=B(0xacf85cu+cell)&3u;
 if(house_variant)return U(0xf56c54u);
 return U(0xf56c5cu)+variant*9u;
}
static uint16_t rubble_frame(uint32_t record)
{return (uint16_t)((B(0x799a25u+record)&7u)+1u);}
static void publish_rubble(uint32_t id,uint32_t record,uint32_t image)
{FUN_0047bf50((uint16_t)id,H(0x7999deu+record),H(0x7999e0u+record),1,1,image,8);}

/* Whole46D100. A special type202 delegates immediately. Other records are
   released before their type, house variant and size are reloaded. Failed
   additional allocations still address record0, exactly as in native code. */
void restore_building_destroy(int32_t building,uint32_t mode)
{
 uint32_t id=(uint32_t)building,record=id*264u,refresh=0,house_variant=0,count,index,cell,x,y,new_id,new_record,image;
 uint16_t kind;uint8_t size,house;
 if(W(0x7999e8u+record)==202u){restore_building_destroy_four_parts(building);return;}
 W(0x799a20u+record)=0;W(0x799a1eu+record)=0;house=B(0x7999ddu+record);U(0xbc9694u)=0;
 if(house&&W(0x7999f4u+record))restore_population_remove_destroyed(I(U(0xeaac28u)),H(0x7999f4u+record));
 W(0x7999f4u+record)=0;B(0x7999ddu+record)=0;B(0x799a1bu+record)=0;W(0x7999f8u+record)=0;FUN_0046ac50(building);
 kind=W(0x7999e8u+record);
 if(kind==31u||kind==30u||kind==33u||kind==209u){
  new_id=restore_building_destruction_root(id);
  if(I(new_id)>0&&new_id==id)restore_building_clear_farm_tiles(I(new_id),H(0x7999e8u+new_id*264u));
 }
 kind=W(0x7999e8u+record);
 if(kind==75u||kind==76u||kind==74u||kind==181u||kind==182u||kind==192u)refresh=1;
 if(B(0x7999ddu+record)&&H(0x7999eau+record)<=1)house_variant=1;
 size=B(0x7999dbu+record);
 switch(size){case 2:count=4;break;case 3:count=9;break;case 4:count=16;break;case 5:count=25;break;case 6:count=36;break;default:count=0;break;}
 FUN_0047c480(building,H(0x7999deu+record),H(0x7999e0u+record));cell=U(0x7999e4u+record);
 if(B(0xaf598cu+cell*4u)&4u)B(0x7999d8u+record)=5;
 else{
  W(0x7999e8u+record)=99;W(0x799a08u+record)=0;W(0x799a22u+record)=rubble_frame(record);
  B(0x799a24u+record)=1;B(0x7999dbu+record)=1;B(0x799a95u+record)=(uint8_t)mode;
  image=rubble_image(record,house_variant);publish_rubble(id,record,image);
 }
 for(index=1;index<count;++index){
  x=(uint32_t)H(0x7999deu+record)+U(0x5ccd10u+index*8u);y=(uint32_t)H(0x7999e0u+record)+U(0x5ccd14u+index*8u);
  cell=U(0xbca2d8u)+y*228u+x;if(B(0xaf598cu+cell*4u)&4u)continue;
  new_id=(uint32_t)restore_building_allocate_0046a400(U(0xeaac28u),99,I(x),I(y),0,0);new_record=new_id*264u;
  image=rubble_image(new_record,house_variant);publish_rubble(new_id,new_record,image);
  W(0x799a22u+new_record)=rubble_frame(new_record);W(0x799a08u+new_record)=0;B(0x799a24u+new_record)=1;B(0x799a95u+new_record)=(uint8_t)mode;
 }
 if(refresh)FUN_005178b0();
}

/* Whole4C04A0. Each part uses its template count directly, not its square.
   The count is reloaded after every publisher and before owned stores, while
   the part origin and initial combined orientation remain cached. */
void restore_building_destroy_four_parts(int32_t building)
{
 uint32_t record=(uint32_t)building*264u,angle,table,part,entry,width_address,index,limit,px,py,x,y,new_id,new_record,image;
 uint16_t frame;
 U(0xbc9694u)=0;W(0x799a20u+record)=0;W(0x799a1eu+record)=0;B(0x799a1bu+record)=0;W(0x7999f8u+record)=0;
 FUN_0046ac50(building);restore_building_clear_four_part_occupancy(building);
 W(0x7999e8u+record)=99;W(0x799a08u+record)=0;W(0x799a22u+record)=rubble_frame(record);
 angle=U(0xbca2e0u);B(0x799a24u+record)=1;B(0x7999dbu+record)=1;angle+=B(0x799a82u+record);
 if(I(angle)>=8)angle-=8u;table=0x5df78cu+angle*16u;
 for(part=0;part<4u;++part){
  entry=U(table+part*4u);px=(uint32_t)H(0x7999deu+record);py=(uint32_t)H(0x7999e0u+record);
  if(angle==0u){px+=U(0x5df81cu+entry*16u);py+=U(0x5df818u+entry*16u);}
  else if(angle==1u){px+=U(0x5df818u+entry*16u);py+=U(0x5df81cu+entry*16u);}
  width_address=0x5df814u+entry*16u;limit=U(width_address);if(I(limit)<=0)continue;index=0;
  do{
   y=py+U(0x5ccd14u+index*8u);x=px+U(0x5ccd10u+index*8u);
   new_id=(uint32_t)restore_building_allocate_0046a400(U(0xeaac28u),99,I(x),I(y),0,0);new_record=new_id*264u;
   image=rubble_image(new_record,0);publish_rubble(new_id,new_record,image);
   frame=rubble_frame(new_record);limit=U(width_address);
   W(0x799a22u+new_record)=frame;++index;W(0x799a08u+new_record)=0;B(0x799a24u+new_record)=1;
  }while(I(index)<I(limit));
 }
}
