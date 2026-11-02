/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "road_check_model.h"

#define MI_U8(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define MI_U16(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MI_U32(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
enum {MI_CF=1,MI_PF=4,MI_AF=16,MI_ZF=64,MI_SF=128,MI_OF=2048};
static __inline uint32_t mi_mask(uint32_t width){return width==32?UINT32_MAX:((1u<<width)-1u);}
static __inline uint32_t mi_szp(uint32_t value,uint32_t width)
{
 uint32_t parity=value&255u;parity^=parity>>4;parity&=15;
 return (value==0?MI_ZF:0)|((value>>(width-1u))&1u?MI_SF:0)|
        (((0x6996u>>parity)&1u)?0:MI_PF);
}
static __inline uint32_t mi_alu(struct restore_menu_input_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
{
 uint32_t mask=mi_mask(width),sign=1u<<(width-1u),result,flags,carry;
 uint64_t wide;a&=mask;b&=mask;carry=operation==2?(f->flags&MI_CF):0;
 if(operation==0){wide=(uint64_t)a+b;result=(uint32_t)wide&mask;
  flags=(wide>mask?MI_CF:0)|((~(a^b)&(a^result)&sign)?MI_OF:0)|((a^b^result)&MI_AF);
 }else if(operation<=2){wide=(uint64_t)b+carry;result=(a-b-carry)&mask;
  flags=((uint64_t)a<wide?MI_CF:0)|(((a^b)&(a^result)&sign)?MI_OF:0)|((a^b^result)&MI_AF);
 }else{result=(operation==3?(a&b):operation==4?(a|b):(a^b))&mask;flags=f->flags&MI_AF;}
 f->flags=(f->flags&~(MI_CF|MI_PF|MI_AF|MI_ZF|MI_SF|MI_OF))|flags|mi_szp(result,width);
 return result;
}
static __inline uint32_t mi_shift(struct restore_menu_input_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
{
 uint32_t mask=mi_mask(width),sign=1u<<(width-1u),result,carry,flags;
 count&=31u;a&=mask;if(!count)return a;
 if(arithmetic){carry=count>=width?(a&sign)!=0:((a>>(count-1u))&1u);
  result=count>=width?((a&sign)?mask:0):((a>>count)|((a&sign)?(mask^(mask>>count)):0));
 }else{carry=count>width?0:((a>>(width-count))&1u);result=(uint32_t)((uint64_t)a<<count)&mask;}
 flags=carry|mi_szp(result,width);
 if(count==1){if(!arithmetic&&(((result&sign)!=0)^carry))flags|=MI_OF;}
 else flags|=f->flags&MI_OF;
 f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|flags;return result;
}
static __inline uint32_t mi_imul(struct restore_menu_input_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

void restore_C_00441130(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00441130 */
    a=f->r[4];b=0x00000090u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00441136 */
    f->r[0]=MI_U32((0x000000a4u+f->r[4]));
    /* 0044113d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044113e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044113f */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00441141 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00441142 */
    a=f->r[0];b=0x0000001fu;t=mi_alu(f,a,b,32,1);
    /* 00441145 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00441146 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0044114a */
    f->r[1]=0x00000005u;
    /* 0044114f */
    if(!(f->flags&MI_ZF))goto mi_0044115b;
    /* 00441151 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000002u);
    /* 00441159 */
    goto mi_00441184;
mi_0044115b:;
    /* 0044115b */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 0044115e */
    if(!(f->flags&MI_ZF))goto mi_0044116a;
    /* 00441160 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000003u);
    /* 00441168 */
    goto mi_00441184;
mi_0044116a:;
    /* 0044116a */
    a=f->r[0];b=0x00000021u;t=mi_alu(f,a,b,32,1);
    /* 0044116d */
    if(!(f->flags&MI_ZF))goto mi_00441179;
    /* 0044116f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000004u);
    /* 00441177 */
    goto mi_00441184;
mi_00441179:;
    /* 00441179 */
    a=f->r[0];b=0x000000d1u;t=mi_alu(f,a,b,32,1);
    /* 0044117e */
    if(!(f->flags&MI_ZF))goto mi_00441184;
    /* 00441180 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00441184:;
    /* 00441184 */
    f->r[0]=MI_U32((0x000000b8u+f->r[4]));
    /* 0044118b */
    f->r[6]=0x00000001u;
    /* 00441190 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00441193 */
    f->r[7]=f->r[6];
    /* 00441195 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00441199 */
    if((f->flags&MI_ZF))goto mi_004411c1;
    /* 0044119b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044119c */
    if((f->flags&MI_ZF))goto mi_004411b5;
    /* 0044119e */
    f->r[2]=0x00000002u;
    /* 004411a3 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004411a5 */
    if(!(f->flags&MI_ZF))goto mi_004411d2;
    /* 004411a7 */
    f->r[6]=f->r[2];
    /* 004411a9 */
    f->r[7]=f->r[2];
    /* 004411ab */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004411af */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004411b3 */
    goto mi_004411d2;
mi_004411b5:;
    /* 004411b5 */
    f->r[7]=f->r[6];
    /* 004411b7 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004411bb */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004411bf */
    goto mi_004411d2;
mi_004411c1:;
    /* 004411c1 */
    f->r[7]=0x00000002u;
    /* 004411c6 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004411ca */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004411ce */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
mi_004411d2:;
    /* 004411d2 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 004411d6 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004411da */
    f->r[0]=(0x00000020u+f->r[4]);
    /* 004411de */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 004411e2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004411e3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004411e4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004411e5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004411e6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004411e7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004411ecu;f->boundary(0x004411e7u,0x00441040u,f);
    /* 004411ec */
    f->r[5]=MI_U32((0x0000002cu+f->r[4]));
    /* 004411f0 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004411f3 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004411f5 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00441264;
    /* 004411f7 */
    a=MI_U32((0x000000b8u+f->r[4]));b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004411ff */
    if(!(f->flags&MI_ZF))goto mi_00441226;
    /* 00441201 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 00441205 */
    f->r[0]=(0x00000020u+f->r[4]);
    /* 00441209 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 0044120d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044120e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044120f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00441210 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00441211 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00441212 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 0044121a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044121fu;f->boundary(0x0044121au,0x00441040u,f);
    /* 0044121f */
    f->r[5]=MI_U32((0x0000002cu+f->r[4]));
    /* 00441223 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00441226:;
    /* 00441226 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00441228 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00441264;
    /* 0044122a */
    f->r[0]=MI_U32((0x000000a4u+f->r[4]));
    /* 00441231 */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 00441235 */
    f->r[2]=MI_U32((0x000000acu+f->r[4]));
    /* 0044123c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044123e */
    f->r[0]=MI_U32((0x000000a8u+f->r[4]));
    /* 00441245 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00441246 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00441248 */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 0044124c */
    f->r[2]=MI_U32((0x000000acu+f->r[4]));
    /* 00441253 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00441255 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00441256 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00441257 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00441259 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044125c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044125d */
    a=f->r[4];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00441263 */
    f->r[4]+=4;return;
mi_00441264:;
    /* 00441264 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00441266 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00441268 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004412b9;
    /* 0044126a */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 0044126e */
    f->r[1]=(0x00000024u+f->r[4]);
mi_00441272:;
    /* 00441272 */
    f->r[0]=f->r[6];
    /* 00441274 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00441277 */
    if((f->flags&MI_ZF))goto mi_004412b9;
    /* 00441279 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044127a */
    if((f->flags&MI_ZF))goto mi_00441294;
    /* 0044127c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044127d */
    if((f->flags&MI_ZF))goto mi_00441283;
    /* 0044127f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00441281 */
    goto mi_004412a5;
mi_00441283:;
    /* 00441283 */
    f->r[0]=MI_U32((0xfffffffcu+f->r[1]));
    /* 00441286 */
    f->r[7]=MI_U32((0x00000000u+f->r[1]));
    /* 00441288 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[0]*4u);
    /* 0044128b */
    f->r[0]=MI_U32((0x005d2984u+f->r[0]*4u));
    /* 00441292 */
    goto mi_004412a5;
mi_00441294:;
    /* 00441294 */
    f->r[0]=MI_U32((0xfffffffcu+f->r[1]));
    /* 00441297 */
    f->r[7]=MI_U32((0x00000000u+f->r[1]));
    /* 00441299 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0044129c */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044129e */
    f->r[0]=MI_U32((0x005d2960u+f->r[0]*4u));
mi_004412a5:;
    /* 004412a5 */
    a=f->r[0];b=MI_U32((0x0000001cu+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004412a9 */
    if(!mi_signed_less(f->flags))goto mi_004412b1;
    /* 004412ab */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004412af */
    f->r[3]=f->r[2];
mi_004412b1:;
    /* 004412b1 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004412b2 */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004412b5 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004412b7 */
    if(mi_signed_less(f->flags))goto mi_00441272;
mi_004412b9:;
    /* 004412b9 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]+f->r[3]*8u));
    /* 004412bd */
    f->r[2]=MI_U32((0x000000a4u+f->r[4]));
    /* 004412c4 */
    f->r[6]=MI_U32((0x000000acu+f->r[4]));
    /* 004412cb */
    f->r[1]=MI_U32((0x00000024u+f->r[4]+f->r[3]*8u));
    /* 004412cf */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004412d1 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004412d3 */
    f->r[2]=MI_U32((0x000000a8u+f->r[4]));
    /* 004412da */
    f->r[6]=MI_U32((0x000000b0u+f->r[4]));
    /* 004412e1 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004412e3 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004412e5 */
    f->r[2]=MI_U32((0x000000b8u+f->r[4]));
    /* 004412ec */
    a=f->r[2];b=0x0000001eu;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004412ef */
    if((f->flags&MI_ZF))goto mi_0044134c;
    /* 004412f1 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004412f2 */
    if((f->flags&MI_ZF))goto mi_004412f9;
    /* 004412f4 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004412f7 */
    if((f->flags&MI_ZF))goto mi_00441318;
mi_004412f9:;
    /* 004412f9 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 004412fc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004412fd */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004412ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00441300 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00441301 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00441302 */
    MI_U32((0x00775614u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 0044130d */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00441311 */
    a=f->r[4];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00441317 */
    f->r[4]+=4;return;
mi_00441318:;
    /* 00441318 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 0044131b */
    f->r[1]=0x00000001u;
    /* 00441320 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00441322 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00441323 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00441326 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00441327 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00441328 */
    MI_U32((0x00775614u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0044132e */
    MI_U32((0x00775628u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00441334 */
    MI_U32((0x00775618u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0044133a */
    MI_U32((0x0077562cu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00441340 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 00441344 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00441345 */
    a=f->r[4];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044134b */
    f->r[4]+=4;return;
mi_0044134c:;
    /* 0044134c */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 00441350 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00441352 */
    if((f->flags&MI_ZF))goto mi_0044137c;
    /* 00441354 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 00441357 */
    f->r[1]=0x00000001u;
    /* 0044135c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044135e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044135f */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00441362 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00441363 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00441364 */
    MI_U32((0x00775614u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0044136a */
    MI_U32((0x00775628u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00441370 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 00441374 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00441375 */
    a=f->r[4];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044137b */
    f->r[4]+=4;return;
mi_0044137c:;
    /* 0044137c */
    f->r[1]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 0044137f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00441380 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00441382 */
    f->r[1]=0x00000001u;
    /* 00441387 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0044138a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044138b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044138c */
    MI_U32((0x00775614u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00441392 */
    MI_U32((0x00775618u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00441398 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 0044139c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044139d */
    a=f->r[4];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004413a3 */
    f->r[4]+=4;return;
}
