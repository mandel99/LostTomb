/* Find whole-map inputs for queue rollover using the actual compiled C. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "standalone_data.h"
#include "route_seed.h"
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
enum {COUNT=51984};
static const int32_t delta[4]={-228,1,228,-1};
int main(void)
{
 uint32_t kind,i,j,d,origin,last,previous,found,seen,head,tail,rng,row,n,incomplete;int result;
 if(!restore_data_open())return 2;
 for(kind=0;kind<3;++kind){
  seen=0;incomplete=0;
  for(i=0;i<256&&seen!=15;++i){
   origin=8192u+(i*997u)%(COUNT-8704u);
   memset((void *)0xaf598cu,0,COUNT*4u);
   memset((void *)0xee0c50u,kind?0:4,COUNT);
   if(kind==1)memset((void *)(0xeed760u-228u*4u),254,228u*4u);
   if(kind==2){
    /* Input-only one-way entrances: every row has a north entrance, while
       the bottom row connects horizontally. This keeps all cells reachable. */
    rng=i+1u;origin=COUNT-228u+i%228u;
    for(n=0;n<COUNT;++n){rng=rng*1664525u+1013904223u;U(0xaf598cu+n*4u)=8;B(0xee0c50u+n)=(rng>>30)==0?250:252;}
    for(row=0;row<227;++row)B(0xee0c50u+row*228u+(row*17u+i*13u)%228u)=250;
    memset((void *)(0xee0c50u+COUNT-228u),252,228u);
   }
   memset((void *)(0xeed760u+COUNT*4u),255,228u*4u);U(0xbca2d8u)=0;
   result=kind?(int)FUN_0051b680(1,(int32_t)(origin%228u),(int32_t)(origin/228u),kind==2):
    (int)FUN_0051c790(1,(int32_t)(origin%228u),(int32_t)(origin/228u));
   head=U(0xeae00cu);tail=U(0xeae008u);
   if(result!=1||head||tail){++incomplete;continue;}
   last=U(0xf203a0u+(COUNT-1u)*4u);found=4;previous=0;
   /* The first processed neighbour is the parent; all four incoming edges
      are allowed in these input-only, full-map terrain configurations. */
   for(j=0;j<COUNT-1u&&found==4;++j){
    previous=U(0xf203a0u+j*4u);
    for(d=0;d<4;++d)if(previous+(uint32_t)delta[d]==last&&
     (kind!=2||(B(0xee0c50u+last)==250?d==0:d!=0))){found=d;break;}
   }
   if(found==4)return 4;
   if(!(seen&(1u<<found))){
    printf("CANDIDATE kind=%u seed=%u origin=%u last=%u parent=%u direction=%u\n",kind,i,origin,last,previous,found);
    fflush(stdout);seen|=1u<<found;
   }
  }
  printf("SEARCH kind=%u directions=%u cases=%u incomplete=%u\n",kind,seen,i,incomplete);fflush(stdout);
 }
 restore_data_close();return 0;
}
