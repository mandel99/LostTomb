#include "building_options.h"
#include "town_menu.h"
#include "menu_building_kind.h"
#include "pyramid_summary.h"
#include "monument_base_queries.h"
#include "monument_progress.h"
#include "monument_complex_queries.h"
#include "campaign_score_numeric.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(a) ((int32_t)(int16_t)W(a))

/* Complete 4F5050. The progress calls execute even when force is nonzero.
 * Their native RET8/receiver contracts are normalized by the existing typed
 * providers. No incoming ECX is consumed by this entry. */
uint32_t FUN_004f5050(uint32_t resource,uint8_t force)
{
 uint32_t i,eax=0;
 for(i=0;i<3;++i){
  uint32_t offset=U(0xeaac28u)*0x49d8u+i*4u,kind=U(0xea5fa0u+offset),record;
  if(!kind){
   record=0x5d19c8u+(uint32_t)S(0x784de0u+i*2u)*16u;eax=record;
   if((uint32_t)S(record+6u)==resource||(uint32_t)S(record+8u)==resource)return(eax&0xffffff00u)|1u;
   eax=(uint32_t)S(record+10u);
   if(eax==resource)return(eax&0xffffff00u)|1u;
  }else{
   uint32_t building=U(0xea5facu+offset);int incomplete;int32_t progress;
   eax=building;
   if(building){
    record=0x5d19c8u+kind*16u;
    switch(S(0x7999e8u+building*264u)){
    case 0xb7:{int32_t summary[28];FUN_004f1310(building,summary);progress=summary[13];goto compare_progress;}
    case 0xd2:incomplete=S(0xea5baau+U(0xeaac28u)*0x49d8u)<16;break;
    case 0xcf:case 0xd0:incomplete=B(0x799a2du+building*264u)!=100;break;
    case 0xd5:progress=FUN_00542ad0(building,0);goto compare_progress;
    case 0xe1:progress=restore_monument_base_0040d940((void *)0x6fd5d8u,building,0);goto compare_progress;
    case 0xe3:progress=restore_monument_base_00417050((void *)0x6fd368u,building,0);goto compare_progress;
    case 0xe4:progress=restore_monument_base_004e6710((void *)0x6de548u,building,0);goto compare_progress;
    case 0xe5:progress=restore_monument_progress_00545740((void *)0x6f9320u,building,0);goto compare_progress;
    case 0xe6:progress=FUN_00401540(building,0);goto compare_progress;
    case 0xea:progress=restore_monument_progress_00558be0((void *)0x6f5648u,building,0);goto compare_progress;
    case 0xeb:progress=restore_monument_progress_00555e80((void *)0x6de9e0u,building,0);goto compare_progress;
    case 0xec:progress=restore_monument_progress_0054f470((void *)0x6e9188u,building,0);goto compare_progress;
    default:progress=FUN_004df040(building,0);
compare_progress:eax=(uint32_t)progress;incomplete=progress<100;break;
    }
    eax=(eax&0xffffff00u)|(uint32_t)incomplete;
    if(!incomplete)eax=(eax&0xffffff00u)|force;
    if(incomplete||force){
     eax=(uint32_t)S(record+6u);
     if(eax==resource||(uint32_t)S(record+8u)==resource||(uint32_t)S(record+10u)==resource)return(eax&0xffffff00u)|1u;
    }
   }
  }
 }
 return eax&0xffffff00u;
}
uint32_t FUN_005150a0(uint32_t kind)
{
 switch(kind){
 case 30:case 62:return 13;
 case 31:case 32:case 48:case 53:case 54:case 55:case 56:return 8;
 case 33:case 34:case 35:case 36:case 37:case 38:return 10;
 case 39:case 40:case 41:case 49:case 57:case 59:return 6;
 case 44:case 45:case 46:return 9;
 case 50:case 51:case 52:return 11;
 default:return 0;
 }
}
static int32_t selected_kind(void){return S(0x7999e8u+U(0xeadd6cu)*264u);}
static void workers(volatile uint32_t *out)
{uint32_t record=U(0xeadd6cu)*264u;int32_t kind=S(0x7999e8u+record);if(S(0x799a18u+record)<(int32_t)U(0x705eb8u+(uint32_t)kind*40u))out[1]=1;}
static int material(uint32_t kind)
{switch(kind){case 1:case 8:case 10:case 13:case 15:case 17:case 18:case 19:case 20:case 23:case 24:case 25:case 26:case 28:case 30:return 1;default:return 0;}}
static void supply(volatile uint32_t *out)
{uint32_t kind=B(0x799a1bu+U(0xeadd6cu)*264u);if(material(kind)&&U(0x784de8u+kind*4u))out[14]=1;}
/* Complete 515130: fifteen DWORD outputs, 264-byte building records,
 * 40-byte capacity records, three 16-byte monument classifications. */
void FUN_00515130(uint32_t output[15])
{
 volatile uint32_t *out=output;uint32_t i,kind,token;int32_t k;RestoreTownRecord *town;
 for(i=0;i<15;++i)out[i]=0;
 k=selected_kind();
 switch(k){
 case 0x1e:case 0x1f:case 0x20:case 0x21:case 0x22:case 0x23:case 0x24:case 0x25:case 0xe2:workers(out);out[10]=1;break;
 case 0x26:case 0x27:case 0x29:case 0x2a:case 0x2b:case 0x52:case 0x53:case 0x88:case 0x8a:out[13]=1;break;
 case 0x28:case 0x2c:case 0x2d:case 0xa9:out[2]=1;break;
 case 0x2e:case 0x2f:case 0x31:case 0xb4:case 0xce:out[8]=1;workers(out);break;
 case 0x33:case 0x35:out[9]=1;workers(out);break;
 case 0x37:case 0x51:case 0xa7:case 0xb8:out[13]=1;workers(out);break;
 case 0x46:case 0x4c:case 0xbe:case 0xc2:out[7]=1;workers(out);break;
 case 0x47:case 0x48:
  workers(out);i=0;out[k==0x47?7:6]=1;
  town=(RestoreTownRecord *)FUN_0044baf0(0);
  while(town){
   if((uint8_t)FUN_0044d4c0(town)){
    int32_t id;if(i>=5)break;id=*(const int16_t *)(town->bytes+4);
    if(k==0x47?(id>0&&id<=8):id<36){out[4]=1;break;}
    ++i;
   }
   town=(RestoreTownRecord *)FUN_0044baf0((const int16_t *)town);
  }break;
 case 0x4a:case 0xb5:case 0xb6:workers(out);out[3]=1;break;
 case 0x4b:case 0x6d:case 0x6e:case 0x6f:case 0x71:case 0x72:case 0xa2:case 0xc0:case 0xc3:out[6]=1;workers(out);supply(out);break;
 case 0x4d:case 0x4e:case 0x4f:out[4]=1;workers(out);break;
 case 0x56:case 0x57:case 0xa1:case 0xbb:case 0xbc:case 0xbd:out[12]=1;out[5]=1;workers(out);break;
 case 0x5a:workers(out);break;
 case 0x5c:out[8]=1;break;
 case 0x5e:case 0x5f:case 0xab:case 0xad:case 0xca:out[2]=1;workers(out);supply(out);break;
 case 0x6a:case 0x6b:case 0x6c:case 0xd8:case 0xdd:
  workers(out);token=k==0x6a?24u:k==0x6b?25u:k==0x6c?20u:k==0xd8?26u:30u;
  if((uint8_t)FUN_004f5050(token,1))out[14]=1;supply(out);break;
 case 0x70:case 0xcd:out[2]=1;out[6]=1;workers(out);supply(out);break;
 case 0x73:out[7]=1;workers(out);supply(out);break;
 case 0xb1:case 0xb2:case 0xb3:workers(out);out[14]=1;break;
 case 0xb7:case 0xcf:case 0xd0:case 0xd2:case 0xd5:case 0xde:out[14]=1;break;
 case 0xc7:
  workers(out);
  {uint32_t record=0xea5fa0u+U(0xeaac28u)*0x49d8u;
   for(i=0;i<3;++i){kind=U(record+i*4u);if(!kind)kind=(uint32_t)S(0x784de0u+i*2u);if(B(0x5d19d6u+kind*16u)){out[14]=1;break;}}
  }break;
 case 0xcb:out[6]=1;out[9]=1;workers(out);supply(out);break;
 case 0xcc:workers(out);if((uint8_t)FUN_004f5050(12,1))out[14]=1;break;
 }
 k=selected_kind();
 if(k>=10&&k<=29){
  supply(out);kind=B(0x799a5du+U(0xeadd6cu)*264u);if(kind<30)kind+=30;
  kind=kind>=68?6u:FUN_005150a0(kind);if(kind)out[kind]=1;out[7]=1;
 }
 if((uint8_t)FUN_00488030(U(0xeadd6cu))){out[7]=1;workers(out);}
 k=selected_kind();if(k>=140&&k<=149)out[11]=1;
 k=selected_kind();if((k>=60&&k<=69)||k==209){out[11]=1;workers(out);}
}
