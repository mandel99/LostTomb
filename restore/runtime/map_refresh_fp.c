#include "map_refresh_arithmetic.h"
#include <stdlib.h>
#define MR_TOP UINT64_C(0x8000000000000000)
typedef struct MRBig {uint32_t limb[8];} MRBig;
static MRBig big_zero(void){MRBig a={{0}};return a;}
static MRBig big_u64(uint64_t a){MRBig n=big_zero();n.limb[0]=(uint32_t)a;n.limb[1]=(uint32_t)(a>>32);return n;}
static uint32_t big_bit(MRBig a,uint32_t bit){return bit<256u?(a.limb[bit/32u]>>(bit%32u))&1u:0;}
static int32_t big_top(MRBig a){int32_t i,bit;for(i=7;i>=0;--i)if(a.limb[i]){for(bit=31;bit>=0;--bit)if((a.limb[i]>>(uint32_t)bit)&1u)return i*32+bit;}return -1;}
static uint32_t big_any(MRBig a,uint32_t bits){uint32_t i,whole=bits/32u,part=bits%32u;for(i=0;i<whole&&i<8;++i)if(a.limb[i])return 1;return whole<8&&part&&(a.limb[whole]&((1u<<part)-1u));}
static MRBig big_shift(MRBig a,uint32_t shift)
{
    MRBig b=big_zero();uint32_t i,target,whole=shift/32u,part=shift%32u;
    if(shift>255u)abort();
    for(i=0;i<8;++i){target=i+whole;if(target>=8){if(a.limb[i])abort();continue;}b.limb[target]|=a.limb[i]<<part;if(part){if(target+1<8)b.limb[target+1]|=a.limb[i]>>(32u-part);else if(a.limb[i]>>(32u-part))abort();}}
    return b;
}
static int32_t big_compare(MRBig a,MRBig b){int32_t i;for(i=7;i>=0;--i)if(a.limb[i]!=b.limb[i])return a.limb[i]>b.limb[i]?1:-1;return 0;}
static MRBig big_add(MRBig a,MRBig b)
{
    uint64_t carry=0,t;uint32_t i;for(i=0;i<8;++i){t=(uint64_t)a.limb[i]+b.limb[i]+carry;a.limb[i]=(uint32_t)t;carry=t>>32;}if(carry)abort();return a;
}
static MRBig big_sub(MRBig a,MRBig b)
{
    uint64_t borrow=0,rhs;uint32_t i;
    for(i=0;i<8;++i){rhs=(uint64_t)b.limb[i]+borrow;borrow=(uint64_t)a.limb[i]<rhs;a.limb[i]=(uint32_t)((uint64_t)a.limb[i]-rhs);}if(borrow)abort();return a;
}
static MRBig big_mul64(uint64_t a,uint64_t b)
{
    MRBig out=big_zero();uint32_t av[2]={(uint32_t)a,(uint32_t)(a>>32)},bv[2]={(uint32_t)b,(uint32_t)(b>>32)},i,j;uint64_t carry,t;
    for(i=0;i<2;++i){carry=0;for(j=0;j<2;++j){t=(uint64_t)av[i]*bv[j]+out.limb[i+j]+carry;out.limb[i+j]=(uint32_t)t;carry=t>>32;}out.limb[i+2]=(uint32_t)carry;}return out;
}
static MRBig big_div64(MRBig numerator,uint64_t denominator,uint32_t *sticky)
{
    MRBig quotient=big_zero(),remainder=big_zero(),divisor=big_u64(denominator);int32_t i;
    for(i=big_top(numerator);i>=0;--i){remainder=big_shift(remainder,1);remainder.limb[0]|=big_bit(numerator,(uint32_t)i);if(big_compare(remainder,divisor)>=0){remainder=big_sub(remainder,divisor);quotient.limb[(uint32_t)i/32u]|=1u<<((uint32_t)i%32u);}}
    *sticky=big_top(remainder)>=0;return quotient;
}
static uint32_t precision(uint16_t cw){uint32_t pc=(cw>>8)&3u;if(pc==0)return 24;if(pc==2)return 53;if(pc==3)return 64;abort();}
static uint64_t big_slice64(MRBig a,uint32_t shift)
{
    uint32_t word=shift/32u,part=shift%32u;uint64_t low=word<8?a.limb[word]:0;
    if(word+1<8)low|=(uint64_t)a.limb[word+1]<<32;
    if(part){low>>=part;if(word+2<8)low|=(uint64_t)a.limb[word+2]<<(64u-part);}return low;
}
static MRFloat fp_zero(uint32_t sign){MRFloat v={0,0,0,0};v.negative=sign;return v;}
static MRFloat fp_special(uint32_t kind,uint32_t sign){MRFloat v={0,0,0,0};v.kind=kind;v.negative=sign;return v;}
static MRFloat rounded(MRBig exact,int32_t least,uint32_t sign,uint32_t p,uint16_t cw,uint32_t sticky)
{
    MRFloat out=fp_zero(sign);int32_t top=big_top(exact),shift;uint64_t kept=0;uint32_t dropped,half,tail,increment=0,rc=(cw>>10)&3u;
    if(top<0){if(sticky)abort();return out;}
    shift=top-(int32_t)(p-1u);
    if(shift<=0){if(sticky)abort();kept=big_slice64(exact,0)<<(uint32_t)(-shift);dropped=0;}
    else{kept=big_slice64(exact,(uint32_t)shift);dropped=big_any(exact,(uint32_t)shift)||sticky;half=big_bit(exact,(uint32_t)shift-1u);tail=big_any(exact,(uint32_t)shift-1u)||sticky;
        if(rc==0)increment=half&&(tail||(kept&1u));else if(rc==1)increment=sign&&dropped;else if(rc==2)increment=!sign&&dropped;
    }
    out.exponent=least+top;
    if(increment){if(p==64&&kept==UINT64_MAX){kept=MR_TOP;++out.exponent;}else{++kept;if(p<64&&kept==(UINT64_C(1)<<p)){kept>>=1;++out.exponent;}}}
    out.significand=kept<<(64u-p);return out;
}
MRFloat mr_fp_i32(int32_t value)
{
    uint32_t sign=value<0;uint32_t magnitude=sign?0u-(uint32_t)value:(uint32_t)value;return rounded(big_u64(magnitude),0,sign,64,0x37f,0);
}
MRFloat mr_fp_binary64(uint64_t bits)
{
    uint32_t sign=(uint32_t)(bits>>63),exponent=(uint32_t)((bits>>52)&0x7ffu);uint64_t fraction=bits&UINT64_C(0xfffffffffffff);MRFloat out=fp_zero(sign);
    if(exponent==0x7ffu)return fp_special(fraction?2u:1u,fraction?1u:sign);
    if(exponent==0)return rounded(big_u64(fraction),-1074,sign,64,0x37f,0);
    out.significand=(MR_TOP|(fraction<<11));out.exponent=(int32_t)exponent-1023;return out;
}
MRFloat mr_fp_add(MRFloat a,MRFloat b,uint16_t cw)
{
    int32_t low,delta,order;MRBig av,bv,sum;uint32_t sign;
    if(a.kind==2||b.kind==2)return fp_special(2,1);
    if(a.kind==1||b.kind==1){if(a.kind==1&&b.kind==1&&a.negative!=b.negative)return fp_special(2,1);return a.kind==1?a:b;}
    if(a.significand==0&&b.significand==0)return fp_zero(a.negative==b.negative?a.negative:(((cw>>10)&3u)==1));
    if(a.significand==0)return rounded(big_u64(b.significand),b.exponent-63,b.negative,precision(cw),cw,0);
    if(b.significand==0)return rounded(big_u64(a.significand),a.exponent-63,a.negative,precision(cw),cw,0);
    low=a.exponent<b.exponent?a.exponent:b.exponent;delta=a.exponent>b.exponent?a.exponent-b.exponent:b.exponent-a.exponent;
    /* Required finite domain has exponent separation<=192. Map-r4 separation
       is at most53 (binary64 score lattice2^-47, weights25..100). */
    if(delta>192)abort();av=big_shift(big_u64(a.significand),(uint32_t)(a.exponent-low));bv=big_shift(big_u64(b.significand),(uint32_t)(b.exponent-low));
    if(a.negative==b.negative){sum=big_add(av,bv);sign=a.negative;}else{order=big_compare(av,bv);if(order==0)return fp_zero(((cw>>10)&3u)==1);if(order>0){sum=big_sub(av,bv);sign=a.negative;}else{sum=big_sub(bv,av);sign=b.negative;}}
    return rounded(sum,low-63,sign,precision(cw),cw,0);
}
MRFloat mr_fp_sub(MRFloat a,MRFloat b,uint16_t cw){b.negative^=1u;return mr_fp_add(a,b,cw);}
MRFloat mr_fp_mul(MRFloat a,MRFloat b,uint16_t cw)
{
    uint32_t sign=a.negative^b.negative;
    if(a.kind==2||b.kind==2)return fp_special(2,1);
    if(a.kind==1||b.kind==1){if((a.kind==0&&!a.significand)||(b.kind==0&&!b.significand))return fp_special(2,1);return fp_special(1,sign);}
    return rounded(big_mul64(a.significand,b.significand),a.exponent+b.exponent-126,sign,precision(cw),cw,0);
}
MRFloat mr_fp_div(MRFloat a,MRFloat b,uint16_t cw)
{
    uint32_t sign=a.negative^b.negative,sticky;MRBig quotient;
    if(a.kind==2||b.kind==2||(a.kind==1&&b.kind==1))return fp_special(2,1);
    if(a.kind==1)return fp_special(1,sign);
    if(b.kind==1)return fp_zero(sign);
    if(!b.significand)return fp_special(a.significand?1u:2u,a.significand?sign:1u);
    if(!a.significand)return fp_zero(sign);
    quotient=big_div64(big_shift(big_u64(a.significand),66),b.significand,&sticky);
    return rounded(quotient,a.exponent-b.exponent-66,sign,precision(cw),cw,sticky);
}
uint64_t mr_fp_store64(MRFloat value,uint16_t cw)
{
    uint64_t sign=(uint64_t)value.negative<<63;MRFloat rounded_value;
    if(value.kind==2)return UINT64_C(0xfff8000000000000);
    if(value.kind==1)return sign|UINT64_C(0x7ff0000000000000);
    if(!value.significand)return sign;
    rounded_value=rounded(big_u64(value.significand),value.exponent-63,value.negative,53,cw,0);
    /* Subnormal/overflow stores are outside the proven map operand domain. */
    if(rounded_value.exponent< -1022||rounded_value.exponent>1023)abort();
    return sign|((uint64_t)(rounded_value.exponent+1023)<<52)|((rounded_value.significand>>11)&UINT64_C(0xfffffffffffff));
}
void mr_fp_raw80(MRFloat value,uint8_t output[10])
{
    uint64_t sig=value.significand;uint32_t exponent,i;
    if(value.kind){sig=value.kind==2?UINT64_C(0xc000000000000000):MR_TOP;exponent=0x7fff;}else exponent=sig?(uint32_t)(value.exponent+16383):0;
    exponent|=value.negative<<15;for(i=0;i<8;++i)output[i]=(uint8_t)(sig>>(i*8));output[8]=(uint8_t)exponent;output[9]=(uint8_t)(exponent>>8);
}
uint16_t mr_fp_control_word(void)
{
    uint16_t cw;
    /* Owned Win32 ABI/environment query only. Arithmetic is integer C above. */
    __asm {fnstcw cw}
    return cw;
}
