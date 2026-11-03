#include "preview_parent_model.h"
#include "small_constructor_model.h"
#include "connection_plan_model.h"
#include "map_commit_route_actions.h"
#include "building_rect_commit.h"
#include "drag_commit_model.h"
#include "drag_leaves.h"
#include "map_commit_placement.h"
#include "paving_region_model.h"
#include "canal_region_model.h"
#include "footprint.h"
#include "region_gate.h"
#include "preview_geometry.h"
#include "route_query_model.h"
#include "map_commit_warnings.h"
#include "region_validation.h"
extern void FUN_004743f0(int32_t,int32_t,int32_t,int32_t,int32_t);
extern code *restore_placement_rule(uint32_t token);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

void restore_construction_preview(int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t tool)
{
 uint32_t base_cost,size,width,mask,value,discount,cost,rule,stage,cell,row,record,source,source_record;
 uint32_t source_orientation,target_orientation,source_size;int32_t source_x,source_y,count;
 uint8_t allow=0;
 U(0xbc9724u)=0;U(0xbc9718u)=0;
 if(!tool||I(U(0xea187cu+U(0xeaac28u)*0x49d8u))<=-5000)return;
 FUN_0040f920((void *)0xb285ccu,0xcb10,0xaf);
 base_cost=U(0x705ea4u+(uint32_t)tool*40u);size=U(0x5d06dcu+(uint32_t)tool*20u);
 U(0xbc9724u)=base_cost;U(0xbc96d0u)=size;
 width=size;mask=UINT32_MAX;
 switch(tool){
 case 5:FUN_00474f50(1,x1,y1,x2,y2);goto scale_cost;
 case 6:FUN_00475220(1,x1,y1,x2,y2,0x4000);goto scale_cost;
 case 7:
  (void)FUN_0047eda0(1,x1,y1,x2,y2);value=U(0xbca2d4u)-1u;cost=U(0xbca2d0u)-1u;U(0xbca328u)=1;
  FUN_0047bae0(0,0,I(cost),I(value),0);U(0x605994u)=0;break;
 case 8:FUN_00475090(1,x1,y1,x2,y2);goto scale_cost;
 case 9:FUN_004743f0(1,x1,y1,x2,y2);goto scale_cost;
 case 10:restore_building_rect_00474130(1,x1,y1,x2,y2);goto scale_cost;
 case 30:(void)FUN_00440850(1,(uint32_t)x1,(uint32_t)y1,x2,y2);goto scale_cost;
 case 31:(void)FUN_00440700(1,(uint32_t)x1,(uint32_t)y1,x2,y2);goto scale_cost;
 case 33:(void)FUN_00440b10(1,(uint32_t)x1,(uint32_t)y1,x2,y2);goto scale_cost;
 case 38:FUN_004754a0(1,x1,y1,x2,y2);goto scale_cost;
 case 39:FUN_004759e0(1,x1,y1,x2,y2);goto scale_cost;
 case 40:case 44:case 45:
  if(I(U(0xbcb9b4u))>=6)return;
  (void)FUN_0047d530(3,x2,y2,UINT32_MAX,1);break;
 case 50:if(U(0xea5e70u+U(0xeaac28u)*0x49d8u))break;goto draw;
 case 56:width=3;mask=0xffffffbfu;goto draw;
 case 58:case 170:case 200:case 201:width=2;mask=0xffffffbfu;goto draw;
 case 65:case 66:case 67:case 68:case 69:case 156:case 157:case 158:case 159:case 160:
  (void)FUN_0047d530(3,x2,y2,UINT32_MAX,1);break;
 case 72:case 193:width=3;goto draw;
 case 74:case 75:case 182:
  value=(uint32_t)FUN_0047d060(x2,y2,1);if(!value)U(0x605994u)=1;
  if(tool==182){
   if(I(U(0xbcb9b0u))>=6)return;
   /* This second query is intentional: genuine inputs may change in between. */
   if(!FUN_0047d060(x2,y2,1))U(0x605994u)=1;
  }
  break;
 case 76:case 90:case 181:if(!FUN_0047cc70(x2,y2,1,0))U(0x605994u)=1;break;
 case 77:case 78:case 79:
  if(U(0xea620cu+U(0xeaac28u)*0x49d8u))break;
  rule=U(0x6059b4u);
  if(rule){if(!FUN_004829b0(x2,y2,I(size),0,0x80,I(rule)))return;width=U(0xbc96d0u);}
  goto draw;
 case 82:case 83:value=U(0xbc96f8u);if(I(value)>1)U(0xbc9724u)=value*base_cost;break;
 case 84:case 85:case 86:case 87:case 187:case 188:case 189:
  if(W(0xea5cf6u+U(0xeaac28u)*0x49d8u))break;goto draw;
 case 94:if(U(0xea6240u+U(0xeaac28u)*0x49d8u))break;goto draw;
 case 95:if(I(U(0x705c6cu))>0)break;goto draw;
 case 136:
  if(!FUN_0047cc70(x2,y2,1,0))U(0x605994u)=1;
  row=(uint32_t)y2*228u;cell=U(0xbca2d8u)+row+(uint32_t)x2;
  if(!FUN_00486cf0(cell))break;
  stage=U(0xbca310u);
  if(!stage){
   B(0xbca315u)=0;record=(uint32_t)H(0xb4e6fcu+cell*2u)*264u;
   if(I(U(0x799a9cu+record))<0){
    U(0xbca310u)=2;B(0xbca315u)=1;
    U(0xbc9800u)=(uint32_t)H(0xb4e6fcu+(U(0xbca2d8u)+row+(uint32_t)x2)*2u);
   }
  }else if(stage==1){
   record=(uint32_t)H(0xb4e6fcu+cell*2u)*264u;source=U(0xbc9800u);B(0xbca315u)=0;
   if(I(U(0x799a9cu+record))<0){
    source_record=source*264u;
    /* BYTE orientation makes the native signed-remainder repair unreachable. */
    source_orientation=((uint32_t)B(0x799a82u+source_record)*2u+4u)&7u;
    target_orientation=(uint32_t)B(0x799a82u+record)*2u;source_size=B(0x7999dbu+source_record);
    source_y=H(0x7999e0u+source_record);source_x=H(0x7999deu+source_record);count=0;
    if(FUN_00486a30(x2,y2,source_x,source_y,source_size,I(target_orientation),I(source_orientation),0,&count)){
     value=(uint32_t)H(0xb4e6fcu+cell*2u);source=U(0xbc9800u);
     U(0xbca310u)=3;B(0xbca315u)=1;U(0x799a9cu+value*264u)=source;U(0x799a9cu+source*264u)=value;
     FUN_00486020(); /* The native caller pushes value, but the callee is nullary. */
    }
   }
  }
  break;
 case 168:FUN_00475220(1,x1,y1,x2,y2,0x404000);goto scale_cost;
 case 169:FUN_00475220(1,x1,y1,x2,y2,0x804000);goto scale_cost;
 case 171:case 202:FUN_004bfa50(x2,y2,U(0xbc9714u));break;
 case 207:width=3;mask=0xffffffbfu;goto draw;
 case 208:width=5;mask=0xffffffbfu;goto draw;
 case 209:(void)FUN_00440ed0(1,(uint32_t)x1,(uint32_t)y1,x2,y2);goto scale_cost;
 case 210:(void)FUN_0047d530(6,x2,y2,UINT32_MAX,1);break;
 case 213:case 222:case 225:case 227:case 228:case 229:case 230:case 234:case 235:case 236:break;
 default:
  allow=(uint8_t)FUN_0046fde0((uint32_t)tool);
  if(!allow&&FUN_004828e0(x2,y2,I(U(0xbc96d0u)),0,0x10000))return;
  if(U(0x6059ccu)){if(!FUN_00482650(x2,y2,3,1,0x10800))return;break;}
  rule=U(0x605a48u);if(rule){if(!FUN_00482720(x2,y2,2,1,restore_placement_rule(rule)))return;break;}
  if(U(0x6059c8u)){if(!FUN_00482650(x2,y2,2,1,1))return;break;}
  if(U(0x6059c4u)){if(!FUN_00482650(x2,y2,2,3,4))return;break;}
  if(U(0x6059bcu)){if(!FUN_004828e0(x2,y2,2,0,0x4000))return;break;}
  if(U(0x6059b8u)){
   if(!FUN_004828e0(x2,y2,1,0,0x40))return;
   allow=0;mask=0xffffffbfu;width=U(0xbc96d0u);goto draw;
  }
  rule=U(0x6059b4u);if(rule){if(!FUN_004829b0(x2,y2,I(U(0xbc96d0u)),0,0x80,I(rule)))return;break;}
  if(U(0x6059c0u)){if(!FUN_00482650(x2,y2,2,1,0x40000))return;break;}
  if(tool==183)break;
  width=U(0xbc96d0u);goto draw;
 }
 goto finish;
scale_cost:
 value=U(0xbc971cu);if(I(value)>=0)U(0xbc9724u)=value*U(0xbc9724u);
 goto finish;
draw:
 FUN_0047d360(I(width),x2,y2,mask,0,allow);
finish:
 discount=U(0xbc9718u);cost=U(0xbc9724u);U(0xbc9724u)=cost-discount;
}

void restore_construction_drag_update(void)
{
 uint32_t cell,mode,tool,x,y,first_x,first_y;
 if(!U(0x6059e4u))return;
 cell=U(0xbca300u);if(!cell)return;
 mode=B(0x605a5cu);B(0x605798u)=1;
 if(!mode){
  y=U(0xbca308u);U(0xbca2e4u)=cell;x=U(0xbca304u);U(0x6059d4u)=y;U(0x6059dcu)=x;
 }else{x=U(0x6059dcu);y=U(0x6059d4u);}
 mode=U(0x605a3cu);tool=U(0x6059e8u);
 if(!mode||tool==5u||tool==88u||tool==89u||tool==93u){
  if(mode&&(tool==5u||tool==93u)){first_y=U(0x6059d8u);first_x=U(0x6059e0u);}
  else{first_x=U(0x6059e0u);first_y=U(0x6059d8u);}
  FUN_0046fef0(I(first_x),I(first_y),I(x),I(y),I(tool));
 }else FUN_00478440(I(x),I(y));
}
void FUN_0046fef0(int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t tool)
{restore_construction_preview(x1,y1,x2,y2,tool);}
void FUN_00405d90(void){restore_construction_drag_update();}
