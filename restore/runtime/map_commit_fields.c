#include "map_commit_fields.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f8f0(uint32_t *,uint32_t,uint8_t);
/* Existing accepted C bounds API returns a BYTE predicate. Its original entry
 * produces EAX0/1; converting the real C result explicitly preserves that
 * caller-visible predicate without depending on unused C return-register bits. */
extern uint8_t FUN_0047e830(int32_t cell);

/* 4824B0: OR terrain in a clipped square. Native scratch coordinates and
 * the final cell cursor are DWORDs, including empty/inverted rectangles. */
void FUN_004824b0(int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t flags)
{
 int32_t left=(int32_t)((uint32_t)x-(uint32_t)radius);
 int32_t top=(int32_t)((uint32_t)y-(uint32_t)radius);
 int32_t right=(int32_t)((uint32_t)x+(uint32_t)size+(uint32_t)radius-1u);
 int32_t bottom=(int32_t)((uint32_t)y+(uint32_t)size+(uint32_t)radius-1u);
 uint32_t cell,step,rows,columns,i;
 if(left<0)left=0;
 if(top<0)top=0;
 if(right>=(int32_t)U(0xbca2d0u))right=(int32_t)(U(0xbca2d0u)-1u);
 if(bottom>=(int32_t)U(0xbca2d4u))bottom=(int32_t)(U(0xbca2d4u)-1u);
 cell=U(0xbca2d8u)+(uint32_t)top*228u+(uint32_t)left;
 step=(uint32_t)left-(uint32_t)right+227u;
 U(0xd3b4e0u)=(uint32_t)top;U(0xbc9768u)=cell;
 if(top>bottom)return;
 rows=(uint32_t)bottom-(uint32_t)top+1u;
 U(0xd3b4e0u)=(uint32_t)bottom+1u;
 do{
  U(0xd3b4e4u)=(uint32_t)left;
  if(left<=right){
   columns=(uint32_t)right-(uint32_t)left+1u;
   U(0xd3b4e4u)=(uint32_t)right+1u;
   for(i=0;i<columns;++i)U(0xaf598cu+(cell+i)*4u)|=flags;
   cell+=columns;
  }
  cell+=step;
 }while(--rows);
 U(0xbc9768u)=cell;
}

void FUN_00517690(int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 uint32_t cell,tail;int32_t width,height,x,y;uint32_t i,flags;uint8_t value;
 static const int32_t neighbors[8]={-228,-227,1,229,228,227,-1,-229};
 if(x1<0)x1=0;if(y1<0)y1=0;
 width=(int32_t)U(0xbca2d0u);if(x2>=width)x2=(int32_t)((uint32_t)width-1u);
 height=(int32_t)U(0xbca2d4u);if(y2>=height)y2=(int32_t)((uint32_t)height-1u);
 tail=(uint32_t)width-(uint32_t)x2+(uint32_t)x1-1u;
 cell=U(0xbca2d8u)+(uint32_t)y1*228u+(uint32_t)x1;
 for(y=y1;y<=y2;++y){
  for(x=x1;x<=x2;++x,++cell){
   flags=U(0xaf598cu+cell*4u)&0xeefed77fu;
   if(!(flags&4u))value=0xfc;
   else if(flags&0x80000u)value=0xff;
   else{
    for(i=0;i<8;++i)if(!(B(0xaf598cu+(cell+(uint32_t)neighbors[i])*4u)&4u))break;
    if(i<8)value=0xff;
    else if(y<1||x<1)value=0xfe;
    else if(y>(int32_t)(U(0xbca2d4u)-2u))value=0xfe;
    else if(x>(int32_t)(U(0xbca2d0u)-2u))value=0xfe;
    else{
     value=B(0xadc36cu+cell);
     value=value==13?0xff:(value==5||value==2)?0xfd:0;
    }
   }
   B(0xed4140u+cell)=value;
  }
  cell+=U(0xbca2dcu)+tail;
 }
}
void FUN_005178b0(void)
{
 int32_t last_x,last_y;
 FUN_0040f8f0((uint32_t *)0xed4140u,0xcb10u,0xff);
 last_y=(int32_t)(U(0xbca2d4u)-1u);last_x=(int32_t)(U(0xbca2d0u)-1u);
 FUN_00517690(0,0,last_x,last_y);
}
void FUN_00479670(void)
{
 uint32_t row=0,cell,remaining,limit;int32_t height=(int32_t)U(0xbca2d4u),first,last;uint8_t level,result;
 if(height<=0)return;
 do{
  first=(int32_t)U(0xbc9f24u+row*4u);last=(int32_t)U(0xbc9b94u+row*4u);cell=U(0xbc9804u+row*4u);
  U(0xbc9768u)=cell;
  if(first<last){
   remaining=(uint32_t)last-(uint32_t)first;
   do{
    level=B(0xab623cu+cell);
    if(level){
     limit=(uint32_t)level-1u;
     if(B(0xab6158u+cell)<limit){result=FUN_0047e830((int32_t)(cell-228u));cell=U(0xbc9768u);if(!result)goto decrease;}
     if(B(0xab623du+cell)<limit){result=FUN_0047e830((int32_t)(cell+1u));cell=U(0xbc9768u);if(!result)goto decrease;}
     if(B(0xab6320u+cell)<limit){result=FUN_0047e830((int32_t)(cell+228u));cell=U(0xbc9768u);if(!result)goto decrease;}
     if(B(0xab623bu+cell)<limit){result=FUN_0047e830((int32_t)(cell-1u));cell=U(0xbc9768u);if(!result)goto decrease;}
    }
    goto advance;
decrease:
    B(0xab623cu+cell)=(uint8_t)(B(0xab623cu+cell)-1u);
advance:
    ++cell;--remaining;U(0xbc9768u)=cell;
   }while(remaining);
  }
  height=(int32_t)U(0xbca2d4u);++row;
 }while((int32_t)row<height);
}
