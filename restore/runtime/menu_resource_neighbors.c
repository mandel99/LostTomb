/* Complete integer bodies from pinned Pharaoh SHA256 fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d.
 * Inclusive bounds:480180..4802ba,4802c0..48045b,480460..480695,4806a0..4806b4. */
#include "menu_resource_neighbors.h"
#include <stdio.h>
#include <stdlib.h>
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
static __inline uint32_t mi_alu(struct restore_resource_neighbors_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_resource_neighbors_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_resource_neighbors_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}


static __inline void mi_idiv32(struct restore_resource_neighbors_frame *f,uint32_t divisor_bits)
{
 int32_t divisor=(int32_t)divisor_bits;
 int64_t dividend=(int64_t)(int32_t)f->r[2]*INT64_C(4294967296)+(int64_t)f->r[0];
 int64_t quotient,remainder;
 /* Native #DE cases explicitly fail; no C division overflow or fake result. */
 if(!divisor||(dividend==INT64_MIN&&divisor==-1))abort();
 quotient=dividend/(int64_t)divisor;
 if(quotient<INT32_MIN||quotient>INT32_MAX)abort();
 remainder=dividend%(int64_t)divisor;
 f->r[0]=(uint32_t)(int32_t)quotient;f->r[2]=(uint32_t)(int32_t)remainder;
 /* IDIV leaves arithmetic flags undefined. Keep their previous frame bits. */
}

void restore_resource_C_00480180(struct restore_resource_neighbors_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00480180 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00480183 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00480187 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00480188 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00480189 */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
    /* 0048018d */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00480190 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00480191 */
    f->r[6]=MI_U32((0x0000001cu+f->r[4]));
    /* 00480195 */
    f->r[5]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 00480199 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0048019c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048019d */
    a=f->r[5];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 004801a0 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004801a3 */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 004801a8 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 004801b0 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004801b8 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 004801bb */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004801bd */
    a=f->r[5];b=0x005cc9e0u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004801c3 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
mi_004801c7:;
    /* 004801c7 */
    f->r[0]=MI_U32((0x00000000u+f->r[5]));
    /* 004801ca */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004801cc */
    if((f->flags&MI_ZF))goto mi_0048026a;
    /* 004801d2 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[2]*1u);
    /* 004801d5 */
    f->r[3]=MI_U32((0x00af598cu+f->r[6]*4u));
    /* 004801dc */
    a=(f->r[3]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 004801df */
    if((f->flags&MI_ZF))goto mi_00480206;
    /* 004801e1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[6]*2u));
    /* 004801e9 */
    f->r[6]=f->r[1];
    /* 004801eb */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004801ee */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004801f0 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[6]*8u)))&0xffffu)<<0);
    /* 004801f8 */
    a=(f->r[1]&0xffffu);b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 004801fd */
    if((f->flags&MI_ZF))goto mi_00480255;
    /* 004801ff */
    a=(f->r[1]&0xffffu);b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 00480204 */
    if((f->flags&MI_ZF))goto mi_00480255;
mi_00480206:;
    /* 00480206 */
    a=(f->r[3]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 00480209 */
    if((f->flags&MI_ZF))goto mi_00480255;
    /* 0048020b */
    a=(f->r[3]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 0048020e */
    if(!(f->flags&MI_ZF))goto mi_00480255;
    /* 00480210 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00480212 */
    f->r[6]=0x0000000bu;
    /* 00480217 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00a76aecu+f->r[0]+f->r[2]*1u)))&0xffu)<<0);
    /* 0048021e */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00480223 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00480229 */
    f->r[7]=f->r[1];
    /* 0048022b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048022d */
    a=f->r[0];b=0x00ea5f1cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_00480232:;
    /* 00480232 */
    a=MI_U32((0x00000000u+f->r[0]));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00480234 */
    if((f->flags&MI_ZF))goto mi_00480241;
    /* 00480236 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00480237 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048023a */
    a=f->r[1];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0048023d */
    if(mi_signed_less(f->flags))goto mi_00480232;
    /* 0048023f */
    goto mi_00480243;
mi_00480241:;
    /* 00480241 */
    f->r[6]=f->r[1];
mi_00480243:;
    /* 00480243 */
    a=f->r[6];b=MI_U32((0x00000014u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 00480247 */
    if(!mi_signed_less(f->flags))goto mi_00480255;
    /* 00480249 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0048024d */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00480251 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
mi_00480255:;
    /* 00480255 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00480259 */
    a=f->r[5];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0048025c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0048025d */
    a=f->r[0];b=0x00000018u;t=mi_alu(f,a,b,32,1);
    /* 00480260 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00480264 */
    if(mi_signed_less(f->flags))goto mi_004801c7;
mi_0048026a:;
    /* 0048026a */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0048026e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048026f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00480270 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00480271 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 00480274 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00480275 */
    if(!mi_signed_less(f->flags))goto mi_004802b5;
    /* 00480277 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0048027b */
    f->r[2]=MI_U32((0x00000008u+f->r[4]));
    /* 0048027f */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00480282 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*8u);
    /* 00480285 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 00480289 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048028c */
    f->r[1]=MI_U32((0x005cce98u+f->r[0]));
    /* 00480292 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00480294 */
    f->r[2]=MI_U32((0x005cce9cu+f->r[0]));
    /* 0048029a */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0048029e */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[1]);
    /* 004802a4 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004802a6 */
    f->r[0]=0x00000001u;
    /* 004802ab */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[2]);
    /* 004802b1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004802b4 */
    f->r[4]+=4;return;
mi_004802b5:;
    /* 004802b5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004802b7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004802ba */
    f->r[4]+=4;return;
}

void restore_resource_C_004802c0(struct restore_resource_neighbors_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004802c0 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004802c3 */
    f->r[0]=MI_U32((0x00000074u+f->r[4]));
    /* 004802c7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004802c8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004802c9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004802ca */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004802cd */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004802ce */
    a=f->r[7];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 004802d1 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004802d3 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 004802d6 */
    f->r[1]=0xfffffffeu;
    /* 004802db */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004802df */
    MI_U32((0x00000060u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004802e3 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004802e6 */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 004802eb */
    MI_U32((0x0000006cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004802ef */
    f->r[1]=(0x0000001cu+f->r[4]);
    /* 004802f3 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 004802f6 */
    f->r[0]=MI_U32((0x00000080u+f->r[4]));
    /* 004802fd */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004802ff */
    f->r[0]=0x00000001u;
    /* 00480304 */
    f->r[2]=0x00000002u;
    /* 00480309 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0048030d */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00480311 */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00480315 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0xffffff1cu);
    /* 0048031d */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x000000e4u);
    /* 00480325 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00480329 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(0xfffffe38u);
    /* 00480331 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00480335 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x000001c8u);
    /* 0048033d */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00480341 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00480345 */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00480349 */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0048034d */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00480351 */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00480355 */
    MI_U32((0x0000005cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00480359 */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0048035d */
    MI_U32((0x00000068u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00480361 */
    MI_U32((0x00000070u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00480365 */
    MI_U32((0x00000074u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00480369 */
    MI_U32((0x00000078u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0048036d */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 00480375 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00480379 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048037b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0048037f:;
    /* 0048037f */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 00480383 */
    f->r[1]=MI_U32((0x00000000u+f->r[2]));
    /* 00480385 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00480387 */
    if((f->flags&MI_ZF))goto mi_0048041c;
    /* 0048038d */
    f->r[2]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 00480390 */
    f->r[3]=MI_U32((0x00af598cu+f->r[2]*4u));
    /* 00480397 */
    a=(f->r[3]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0048039a */
    if((f->flags&MI_ZF))goto mi_004803c1;
    /* 0048039c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[2]*2u));
    /* 004803a4 */
    f->r[7]=f->r[2];
    /* 004803a6 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004803a9 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004803ab */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[7]*8u)))&0xffffu)<<0);
    /* 004803b3 */
    a=(f->r[2]&0xffffu);b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 004803b8 */
    if((f->flags&MI_ZF))goto mi_00480407;
    /* 004803ba */
    a=(f->r[2]&0xffffu);b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 004803bf */
    if((f->flags&MI_ZF))goto mi_00480407;
mi_004803c1:;
    /* 004803c1 */
    a=(f->r[3]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004803c4 */
    if((f->flags&MI_ZF))goto mi_00480407;
    /* 004803c6 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004803c8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004803ca */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00a76aecu+f->r[1]+f->r[6]*1u)))&0xffu)<<0);
    /* 004803d1 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004803d7 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004803dd */
    f->r[7]=0x0000000bu;
    /* 004803e2 */
    a=f->r[1];b=0x00ea5f1cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_004803e8:;
    /* 004803e8 */
    a=MI_U32((0x00000000u+f->r[1]));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004803ea */
    if((f->flags&MI_ZF))goto mi_004803f7;
    /* 004803ec */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004803ed */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004803f0 */
    a=f->r[2];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004803f3 */
    if(mi_signed_less(f->flags))goto mi_004803e8;
    /* 004803f5 */
    goto mi_004803f9;
mi_004803f7:;
    /* 004803f7 */
    f->r[7]=f->r[2];
mi_004803f9:;
    /* 004803f9 */
    a=f->r[7];b=MI_U32((0x00000014u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004803fd */
    if(!mi_signed_less(f->flags))goto mi_00480407;
    /* 004803ff */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00480403 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
mi_00480407:;
    /* 00480407 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0048040b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0048040c */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048040f */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00480412 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00480416 */
    if(mi_signed_less(f->flags))goto mi_0048037f;
mi_0048041c:;
    /* 0048041c */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00480420 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00480421 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00480422 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00480423 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 00480426 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00480427 */
    if(!mi_signed_less(f->flags))goto mi_00480456;
    /* 00480429 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 0048042d */
    f->r[2]=MI_U32((0x00000070u+f->r[4]));
    /* 00480431 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]+f->r[0]*8u));
    /* 00480435 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00480437 */
    f->r[2]=MI_U32((0x00000030u+f->r[4]+f->r[0]*8u));
    /* 0048043b */
    f->r[0]=MI_U32((0x00000074u+f->r[4]));
    /* 0048043f */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[1]);
    /* 00480445 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00480447 */
    f->r[0]=0x00000001u;
    /* 0048044c */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[2]);
    /* 00480452 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00480455 */
    f->r[4]+=4;return;
mi_00480456:;
    /* 00480456 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00480458 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048045b */
    f->r[4]+=4;return;
}

void restore_resource_C_00480460(struct restore_resource_neighbors_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00480460 */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 00480464 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00480467 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00480469 */
    if((f->flags&MI_ZF))goto mi_00480490;
    /* 0048046b */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0048046e */
    if((f->flags&MI_ZF))goto mi_00480490;
    /* 00480470 */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 00480474 */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 00480478 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(0x0000000fu);
    /* 00480480 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000009u);
    /* 00480488 */
    f->r[1]=(0xfffffffdu+f->r[0]);
    /* 0048048b */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 0048048e */
    goto mi_004804ae;
mi_00480490:;
    /* 00480490 */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 00480494 */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 00480498 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(0x00000009u);
    /* 004804a0 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x0000000fu);
    /* 004804a8 */
    f->r[1]=(0xffffffffu+f->r[0]);
    /* 004804ab */
    f->r[0]=(0xfffffffdu+f->r[2]);
mi_004804ae:;
    /* 004804ae */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004804b1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004804b2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004804b3 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 004804bb */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004804be */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 004804c4 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004804c7 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 004804ca */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 004804ce */
    f->r[6]=(0x00000000u+f->r[0]+f->r[1]*1u);
    /* 004804d1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004804d3 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004804d5 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004804d9 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004804dd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048068e;
    /* 004804e3 */
    f->r[1]=(0x00b4e6fcu+f->r[6]*2u);
    /* 004804ea */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004804eb */
    f->r[3]=MI_U32((0x0000003cu+f->r[4]));
    /* 004804ef */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004804f3 */
    f->r[1]=(0x00af598cu+f->r[6]*4u);
    /* 004804fa */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004804fb */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004804ff */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00480503:;
    /* 00480503 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00480505 */
    if((f->flags&MI_ZF))goto mi_00480511;
    /* 00480507 */
    f->r[1]=(0xffffffffu+f->r[2]);
    /* 0048050a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0048050c */
    f->r[1]=(0xffffffffu+f->r[3]);
    /* 0048050f */
    if(!(f->flags&MI_ZF))goto mi_00480516;
mi_00480511:;
    /* 00480511 */
    f->r[1]=0x00000001u;
mi_00480516:;
    /* 00480516 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00480518 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00480520 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00480619;
    /* 00480526 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00480529 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 0048052d */
    f->r[0]=(0x00000000u+f->r[1]+f->r[2]*2u);
    /* 00480530 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00480533 */
    f->r[5]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00480536 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00480539 */
    a=f->r[5];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 0048053c */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 0048053f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00480542 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00480545 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00480549 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[2]*2u);
    /* 0048054c */
    f->r[2]=MI_U32((0x00000048u+f->r[4]));
    /* 00480550 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00480553 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00480557 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048055a */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0048055e */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00480562 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
mi_00480566:;
    /* 00480566 */
    f->r[2]=MI_U32((0x00000000u+f->r[2]));
    /* 00480568 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0048056b */
    if((f->flags&MI_ZF))goto mi_0048058b;
    /* 0048056d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 00480570 */
    f->r[7]=f->r[0];
    /* 00480572 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00480575 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00480577 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[7]*8u)))&0xffffu)<<0);
    /* 0048057f */
    a=(f->r[0]&0xffffu);b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 00480583 */
    if((f->flags&MI_ZF))goto mi_004805d9;
    /* 00480585 */
    a=(f->r[0]&0xffffu);b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 00480589 */
    if((f->flags&MI_ZF))goto mi_004805d9;
mi_0048058b:;
    /* 0048058b */
    a=(f->r[2]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0048058e */
    if((f->flags&MI_ZF))goto mi_004805d9;
    /* 00480590 */
    a=(f->r[2]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 00480593 */
    if(!(f->flags&MI_ZF))goto mi_004805d9;
    /* 00480595 */
    f->r[0]=(0x00a76aecu+f->r[6]);
    /* 0048059b */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0048059d */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004805a1 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004805a3 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[0])))&0xffu)<<0);
    /* 004805a5 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004805aa */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004805b0 */
    f->r[7]=0x0000000bu;
    /* 004805b5 */
    a=f->r[0];b=0x00ea5f1cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004805ba:;
    /* 004805ba */
    a=MI_U32((0x00000000u+f->r[0]));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004805bc */
    if((f->flags&MI_ZF))goto mi_004805c9;
    /* 004805be */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004805bf */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004805c2 */
    a=f->r[2];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004805c5 */
    if(mi_signed_less(f->flags))goto mi_004805ba;
    /* 004805c7 */
    goto mi_004805cb;
mi_004805c9:;
    /* 004805c9 */
    f->r[7]=f->r[2];
mi_004805cb:;
    /* 004805cb */
    a=f->r[7];b=MI_U32((0x00000018u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004805cf */
    if(!mi_signed_less(f->flags))goto mi_004805d9;
    /* 004805d1 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004805d5 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[6]);
mi_004805d9:;
    /* 004805d9 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004805dd */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
    /* 004805e1 */
    f->r[3]=MI_U32((0x00000034u+f->r[4]));
    /* 004805e5 */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 004805e9 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004805eb */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004805ef */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004805f1 */
    f->r[3]=MI_U32((0x00000040u+f->r[4]));
    /* 004805f5 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004805f7 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004805f9 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004805fb */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004805ff */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00480603 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00480607 */
    if(mi_signed_less(f->flags))goto mi_00480566;
    /* 0048060d */
    f->r[2]=MI_U32((0x00000044u+f->r[4]));
    /* 00480611 */
    f->r[6]=MI_U32((0x00000028u+f->r[4]));
    /* 00480615 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
mi_00480619:;
    /* 00480619 */
    f->r[1]=MI_U32((0x00000048u+f->r[4]));
    /* 0048061d */
    f->r[5]=MI_U32((0x00000010u+f->r[4]));
    /* 00480621 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 00480625 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00480626 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00480629 */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0048062c */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0048062d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0048062f */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00480633 */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00480637 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0048063b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0048063f */
    if(mi_signed_less(f->flags))goto mi_00480503;
    /* 00480645 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 00480649 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048064a */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 0048064d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048064e */
    if(!mi_signed_less(f->flags))goto mi_0048068e;
    /* 00480650 */
    f->r[5]=MI_U32((0x00bca2d8u));
    /* 00480656 */
    f->r[1]=f->r[6];
    /* 00480658 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0048065a */
    f->r[6]=0x000000e4u;
    /* 0048065f */
    f->r[0]=f->r[1];
    /* 00480661 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00480662 */
    a=f->r[6];mi_idiv32(f,a);
    /* 00480664 */
    f->r[0]=0x8fb823efu;
    /* 00480669 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048066a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048066b */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[2]);
    /* 00480671 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00480673 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00480675 */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00480678 */
    f->r[0]=f->r[2];
    /* 0048067a */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0048067d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048067f */
    f->r[0]=0x00000001u;
    /* 00480684 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[2]);
    /* 0048068a */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048068d */
    f->r[4]+=4;return;
mi_0048068e:;
    /* 0048068e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048068f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00480691 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00480692 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00480695 */
    f->r[4]+=4;return;
}

void restore_resource_C_004806a0(struct restore_resource_neighbors_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004806a0 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004806a4 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004806a8 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004806aa */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004806ab */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004806ac */
    f->r[4]-=4;MI_U32(f->r[4])=0x004806b1u;f->boundary(0x004806acu,0x00480180u,f);
    /* 004806b1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004806b4 */
    f->r[4]+=4;return;
}

__declspec(noinline) __declspec(noreturn) void restore_resource_neighbors_failure(uint32_t target)
{fprintf(stderr,"Unsupported resource-neighbor boundary %08lx\n",(unsigned long)target);abort();}
void restore_resource_neighbors_dispatch(uint32_t site,uint32_t target,struct restore_resource_neighbors_frame *frame)
{
 (void)site;
 switch(target){
 case 0x480180u:restore_resource_C_00480180(frame);return;
 case 0x4802c0u:restore_resource_C_004802c0(frame);return;
 case 0x480460u:restore_resource_C_00480460(frame);return;
 case 0x4806a0u:restore_resource_C_004806a0(frame);return;
 default:restore_resource_neighbors_failure(target);
 }
}
static uint32_t resource_run(uint32_t target,int32_t x,int32_t y,uint32_t orientation,uint32_t count)
{
 uint32_t stack[128],i,entry;struct restore_resource_neighbors_frame frame={{0},2,restore_resource_neighbors_dispatch};
 typedef char pointer_is_32[sizeof(void*)==4?1:-1];(void)sizeof(pointer_is_32);
 for(i=0;i<128;++i)stack[i]=0xa5a55a5au;
 entry=(uint32_t)(uintptr_t)&stack[112];frame.r[4]=entry;
 stack[112]=0;stack[113]=(uint32_t)x;stack[114]=(uint32_t)y;if(count==3)stack[115]=orientation;
 switch(target){
 case 0x480180u:restore_resource_C_00480180(&frame);break;
 case 0x4802c0u:restore_resource_C_004802c0(&frame);break;
 case 0x480460u:restore_resource_C_00480460(&frame);break;
 case 0x4806a0u:restore_resource_C_004806a0(&frame);break;
 default:restore_resource_neighbors_failure(target);
 }
 if(frame.r[4]!=entry+4u)restore_resource_neighbors_failure(frame.r[4]);
 for(i=0;i<16;++i)if(stack[i]!=0xa5a55a5au)restore_resource_neighbors_failure(target);
 for(i=113+count;i<128;++i)if(stack[i]!=0xa5a55a5au)restore_resource_neighbors_failure(target);
 return frame.r[0];
}
uint32_t FUN_00480180(int32_t x,int32_t y,uint32_t orientation){return resource_run(0x480180u,x,y,orientation,3);}
uint32_t FUN_004802c0(int32_t x,int32_t y){return resource_run(0x4802c0u,x,y,0,2);}
uint32_t FUN_00480460(int32_t x,int32_t y,uint32_t orientation){return resource_run(0x480460u,x,y,orientation,3);}
uint32_t FUN_004806a0(int32_t x,int32_t y){return resource_run(0x4806a0u,x,y,0,2);}
