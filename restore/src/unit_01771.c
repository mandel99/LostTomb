#include "camera_pan.h"
int32_t FUN_004cf660(void)
{
    int32_t coordinate;
    uint32_t left=0,right=0,up=0,down=0;
    if(FUN_004ddd60()==0)return 8;
    coordinate=(int32_t)CP_U(0xe38dc4);
    CP_U(0xe38dcc)=0;
    if(coordinate<=0){left=1;CP_U(0xe38dcc)=1;}
    if(coordinate>=(int32_t)(CP_U(0xe38db8)-1u)){right=1;CP_U(0xe38dcc)=1;}
    coordinate=(int32_t)CP_U(0xe38dc0);
    if(coordinate<=0){up=1;CP_U(0xe38dcc)=1;}
    if(coordinate>=(int32_t)(CP_U(0xe38db4)-1u)){down=1;CP_U(0xe38dcc)=1;}
    if(CP_U(0xe92e30)!=0){left=1;CP_U(0xe38dcc)=1;}
    if(CP_U(0xe92e2c)!=0){right=1;CP_U(0xe38dcc)=1;}
    if(CP_U(0xe92e38)!=0){up=1;CP_U(0xe38dcc)=1;}
    if(CP_U(0xe92e34)!=0){down=1;CP_U(0xe38dcc)=1;}
    CP_U(0xe92e2c)=0;CP_U(0xe92e30)=0;CP_U(0xe92e34)=0;CP_U(0xe92e38)=0;
    if(left){if(up)return 7;if(down)return 5;}
    if(right){if(up)return 1;if(down)return 3;}
    if(left)return 6;
    if(up)return 0;
    if(down)return 4;
    return right?2:8;
}
