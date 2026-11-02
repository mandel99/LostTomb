#include "region_gate.h"
#include "terrain_redraw.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_0047d530(int32_t size_in,int32_t x_in,int32_t y_in,uint32_t mask,uint8_t allow)
{
    uint32_t mode=U(0xbca2e0u),size=(uint32_t)size_in,x=(uint32_t)x_in,y=(uint32_t)y_in;
    uint32_t row=0,col,columns=size,rows,pointer_table=0x5cc8c0u,p,cell,flags;
    U(0xbc9738u)=size;U(0xbc973cu)=size;
    if(mode==6u)y+=1u-size;else if(mode==4u){x+=1u-size;y+=1u-size;}else if(mode==2u)x+=1u-size;
    if((int32_t)x<0||(int32_t)y<0)return 0;
    if((int32_t)(size+x-1u)>=(int32_t)U(0xbca2d0u))return 0;
    if((int32_t)(size+y-1u)>=(int32_t)U(0xbca2d4u))return 0;
    U(0xbc9768u)=U(0xbca2d8u)+y*228u+x;
    if(size_in>0)do{
        col=0;
        if((int32_t)columns>0)do{
            cell=U(pointer_table+col*8u);cell+=U(0xbc9768u);
            if((uint8_t)FUN_0046f9a0((int32_t)cell%228,(int32_t)cell/228,3))return 0;
            p=U(0xd05720u+cell*4u);flags=U(0xaf598cu+cell*4u)&mask;
            if((flags&0x10000u)&&!allow)return 0;
            if(flags&0xeefed77fu)return 0;
            if(p&&B(p+24u)==0u)return 0;
            columns=U(0xbc973cu);++col;
        }while((int32_t)col<(int32_t)columns);
        rows=U(0xbc9738u);++row;pointer_table+=48u;
    }while((int32_t)row<(int32_t)rows);
    return 1;
}
