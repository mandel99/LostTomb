#include "training_queries.h"
uint32_t TQ_PUBLIC(FUN_00443a20)(int resource)
{
    uint32_t row=0,header=0x77de02u,column=0x77de06u+(uint32_t)resource;uint8_t type;
    do{
        if(TQ_B(header-2u)!=0){
            type=TQ_B(header);
            if((type==1||type==3||type==5)&&TQ_B(column)==1){
                if(FUN_00447790((int)row,resource)!=0)return 1;
            }
        }
        header+=106u;column+=106u;++row;
    }while(header<0x77f744u);
    return 0;
}
