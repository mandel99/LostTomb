#include "map_hover_geometry.h"
int32_t FUN_00430790(uint32_t ignored)
{
    uint32_t pixel0,origin0,pixel1,origin1,bound,span,step,delta0,delta1;
    uint32_t row,half_column,column,parity_row,parity_column,index,value;
    int32_t remainder0,remainder1,half_remainder;
    MHE(ignored);(void)ignored;
    pixel0=MHR(0xe38e34u);origin0=MHR(0x70e490u);
    if((int32_t)pixel0<(int32_t)origin0)return 0;
    bound=MHR(0x70e458u)+2u;bound=bound*MHR(0x70e464u)+origin0;
    if((int32_t)pixel0>=(int32_t)bound)return 0;
    pixel1=MHR(0xe38e30u);origin1=MHR(0x70e48cu);
    if((int32_t)pixel1<(int32_t)origin1)return 0;
    span=MHR(0x70e44cu);bound=span*MHR(0x70e460u)+origin1;
    if((int32_t)pixel1>=(int32_t)bound)return 0;
    delta1=pixel1-origin1;delta0=pixel0-origin0;
    row=(uint32_t)((int32_t)delta1/(int32_t)span);
    step=MHR(0x70e45cu)+1u;MHW(0x70e404u,row);
    column=(uint32_t)((int32_t)delta0/(int32_t)step);
    parity_column=column&1u;parity_row=row&1u;
    remainder0=(int32_t)delta0%(int32_t)step;
    remainder1=(int32_t)delta1%(int32_t)MHR(0x70e44cu);
    half_remainder=remainder0/2;half_column=(uint32_t)((int32_t)column/2);
    MHW(0x70e408u,half_column);
    if(((row+column)&1u)==0u){
        if(remainder1>half_remainder){++row;MHW(0x70e404u,row);}
        else if(parity_column&&parity_row){++half_column;MHW(0x70e408u,half_column);}
    }else{
        span=MHR(0x70e44cu);
        if((int32_t)((uint32_t)remainder1+(uint32_t)half_remainder)>=(int32_t)(span-1u)){
            ++row;MHW(0x70e404u,row);
            if(parity_column&&!parity_row){++half_column;MHW(0x70e408u,half_column);}
        }
    }
    index=(MHR(0x70e474u)+row)*231u;index+=MHR(0x70e478u)+half_column;
    value=MHR(0x70e494u+index*4u);MHW(0x70e3f8u,value);
    /* Invalid lookup still publishes the raw value; old border flags survive. */
    if((int32_t)value>=0x0fff0000)return 0;
    MHW(0x70e3fcu,0);MHW(0x70e400u,0);
    if(row==0u)MHW(0x70e3fcu,2);
    else if((int32_t)row>=(int32_t)MHR(0x70e460u))MHW(0x70e3fcu,1);
    if(row&1u){
        if(half_column==0u){MHW(0x70e400u,2);return (int32_t)value;}
        if((int32_t)half_column>=(int32_t)MHR(0x70e464u))MHW(0x70e400u,1);
    }
    return (int32_t)value;
}
