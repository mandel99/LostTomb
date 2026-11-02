#include "high_score_submission.h"
#include "campaign_profile.h"
#include "family_menu.h"
#include "campaign_score.h"
#define SCORE_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SCORE_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define SCORE_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define s_save_highscore_jas_005e02a8 (*(char (*)[19])0x005e02a8u)
extern uint32_t FUN_004192b0(void);
extern int FUN_00562370(char *,char *);
extern restore_legacy_FILE *FUN_00561a0a(const char *,const char *);
extern uint32_t FUN_00561f5f(const void *,uint32_t,uint32_t,restore_legacy_FILE *);
extern uint32_t FUN_00561994(restore_legacy_FILE *);

static void score_copy_name(uint32_t destination,uint32_t source)
{
 uint32_t n=1;while(SCORE_B(source+n-1u))++n;
 while(n>=4){SCORE_U(destination)=SCORE_U(source);destination+=4u;source+=4u;n-=4u;}
 while(n){SCORE_B(destination)=SCORE_B(source);++destination;++source;--n;}
}
static void score_record(uint32_t base,int32_t level,uint32_t completed,int32_t score,uint32_t months)
{
 uint32_t scene=SCORE_U(0xeaac28u)*0x49d8u,record=base+(uint32_t)level*76u;
 SCORE_U(record+0x28u)=SCORE_U(0xea5b3cu+scene);
 SCORE_U(record+0x40u)=SCORE_U(0xea6018u+scene);
 SCORE_U(record+0x38u)=SCORE_U(0xea187cu+scene);
 SCORE_U(record+0x34u)=SCORE_U(0xea1894u+scene);
 SCORE_U(record+0x30u)=SCORE_U(0xea5b48u+scene);
 SCORE_U(record+4u)=(uint32_t)level;SCORE_U(record+0x48u)=1;SCORE_U(record+0x44u)=completed;
 score_copy_name(record+8u,0xe38e74u);
 SCORE_U(record+0x2cu)=SCORE_U(0xea5b40u+scene);
 SCORE_U(record)=(uint32_t)score;SCORE_U(record+0x3cu)=months;
}
uint8_t restore_high_scores_write(RestoreHighScores *self,const char *path)
{
 uint32_t written;restore_legacy_FILE *stream=FUN_00561a0a(path,(const char *)0x57c280u);
 if(!stream)return 0;
 written=FUN_00561f5f(self,0x1db0,1,stream);FUN_00561994(stream);return written==1;
}
uint8_t FUN_004c58d0(const char *path)
{return restore_high_scores_write(RESTORE_HIGH_SCORES,path);}
void FUN_004c5970(int32_t level,uint32_t completed,int32_t score,uint32_t months)
{
 score_record(0xd38708u,level,completed,score,months);
 (void)restore_high_scores_write(RESTORE_HIGH_SCORES,s_save_highscore_jas_005e02a8);
}
void FUN_0041c600(int32_t level,uint32_t completed,int32_t score,uint32_t months)
{
 char *path;score_record(0x7009dcu,level,completed,score,months);
 path=FUN_00516a20();(void)FUN_0041b8a0(RESTORE_CAMPAIGN_PROFILE,path);
}
void FUN_004c5a40(void)
{
 uint32_t gate=SCORE_U(0xe38ebcu),year,start,months,difference,mask,completed,record;
 int32_t score,level;
 SCORE_U(0xd38700u)=0;if(gate)return;
 year=(uint32_t)(int32_t)(int16_t)SCORE_W(0x7848d8u);start=SCORE_U(0x78d7bcu);months=SCORE_U(0x78d7c0u);
 difference=year-start;mask=(difference&0x80000000u)?UINT32_MAX:0;
 months+=((difference^mask)-mask)*12u;
 score=FUN_0041c380(months);completed=FUN_004192b0();
 if(SCORE_B(0xe38dd7u)){SCORE_B(0xe38dd7u)=0;SCORE_U(0xd38700u)=1;return;}
 (void)restore_high_scores_read(RESTORE_HIGH_SCORES,s_save_highscore_jas_005e02a8);
 level=(int8_t)SCORE_B(0xe38ea8u);record=0xd38708u+(uint32_t)level*76u;
 if(!SCORE_U(record+0x48u)){
  SCORE_U(0xd38700u)=2;FUN_004c5970(level,completed,score,months);
 }else{
  int equal=FUN_00562370((char *)(uintptr_t)(record+8u),(char *)0xe38e74u)==0;
  level=(int8_t)SCORE_B(0xe38ea8u);record=0xd38708u+(uint32_t)level*76u;
  if(score<=(int32_t)SCORE_U(record))SCORE_U(0xd38700u)=equal?3u:6u;
  else{
   SCORE_U(0xd38700u)=equal?4u:5u;
   if(!equal)score_copy_name(0xd386e0u,record+8u);
   FUN_004c5970(level,completed,score,months);
  }
 }
 level=(int8_t)SCORE_B(0xe38ea8u);
 if(score>(int32_t)SCORE_U(0x7009dcu+(uint32_t)level*76u))FUN_0041c600(level,completed,score,months);
}
