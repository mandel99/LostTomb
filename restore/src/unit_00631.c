#include "training_queries.h"
uint32_t TQ_PUBLIC(FUN_00443b90)(int resource)
{
    uint32_t header,column;
    if(resource==15){resource=14;if(FUN_004439c0(14)!=0)return 1;}
    else if(resource==17){resource=16;if(FUN_004439c0(16)!=0)return 1;}
    else if(resource==33){resource=32;if(FUN_004439c0(32)!=0)return 1;}
    else if(resource==34){
        if((FUN_00443b90(13)!=0||FUN_00443d90(13)!=0)&&FUN_00443950(31)!=0)return 1;
        if(FUN_004439c0(13)!=0&&FUN_00443950(31)!=0)return 1;
    }
    else if(resource==19){resource=18;if(FUN_004439c0(18)!=0)return 1;}
    else if(resource==13){resource=11;if(FUN_004439c0(11)!=0)return 1;}
    else if(resource==10){resource=29;if(TQ_W(0x784d32)==0)return 0;if(FUN_004439c0(29)!=0)return 1;}
    else if(resource==28){resource=20;if(TQ_W(0x784d34)==0)return 0;if(FUN_004439c0(20)!=0)return 1;}
    else if(resource==12){
        if((FUN_00443b90(11)!=0||FUN_00443d90(11)!=0)&&FUN_00443b90(1)!=0)return 1;
        if((FUN_00443b90(11)!=0||FUN_00443d90(11)!=0)&&FUN_004439c0(9)!=0)return 1;
        if(FUN_00443b90(1)!=0&&FUN_004439c0(11)!=0)return 1;
        if(FUN_004439c0(9)!=0&&FUN_004439c0(11)!=0)return 1;
    }
    else if(resource==23){resource=22;if(FUN_004439c0(22)!=0)return 1;}
    else if(resource==9){if(FUN_00443b90(1)!=0)return 1;}
    header=0x77de02u;column=0x77de2au+(uint32_t)resource;
    do{
        if(TQ_B(header-2u)!=0&&TQ_B(header)==0&&TQ_B(column)==1)return 1;
        header+=106u;column+=106u;
    }while(header<0x77f744u);
    return 0;
}
