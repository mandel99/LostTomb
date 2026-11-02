/* Complete pinned 004f8960..004f8e56(417),00488030..0048804b(8).
 * EAX outputs are full DWORDs; no incoming register data input/ECX receiver.
 * Native488030/487f80 RET4 is normalized by these typed internal C calls. */
#include "menu_building_kind.h"
#define BK_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define BK_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
extern int32_t FUN_00487f80(int32_t kind);
uint32_t FUN_00488030(uint32_t building)
{return (uint32_t)FUN_00487f80((int32_t)(int16_t)BK_W(0x7999e8u+building*264u));}
uint32_t FUN_004f8960(void)
{
 uint32_t mode=BK_U(0xeadd50u),building;uint16_t kind;
 if(mode==1u){
  switch(BK_U(0xeadd4cu)){
  case 12:case 9:case 23:case 24:case 8:return 1;
  case 7:return 4;
  default:return 0;
  }
 }
 if(mode!=2u)return 0;
 building=BK_U(0xeadd6cu);kind=BK_W(0x7999e8u+building*264u);
 if(((int16_t)kind>=0x8c&&(int16_t)kind<=0x95)||kind==0x3c)return 1;
 if((uint8_t)FUN_00488030(building))return 5;
 building=BK_U(0xeadd6cu);kind=BK_W(0x7999e8u+building*264u);
 switch(kind){
 case 0x3d:case 0x3e:case 0x3f:case 0x40:case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:case 0x29:case 0x2a:case 0x2b:case 0x22:case 0x23:case 0x24:case 0x25:case 0xd1:case 0x2e:case 0x2f:case 0xce:case 0x31:case 0x62:case 0x63:case 0x5a:case 0xb4:case 0x58:case 0x59:case 0x5d:case 0x50:case 0x37:case 0x51:case 0x33:case 0x35:case 0x3a:case 0xaa:case 0xab:case 0xc8:case 0xc9:case 0xca:case 0x3b:case 0xac:case 0x39:case 0x5e:case 0xb9:case 0xba:case 0x5f:case 0x46:case 0xbe:case 0x4a:case 0x4b:case 0xc0:case 0x4c:case 0xb5:case 0xb6:case 0x4d:case 0x4e:case 0x4f:case 0xbb:case 0xbc:case 0xbd:case 0x56:case 0x57:case 0x6e:case 0x6f:case 0x70:case 0x71:case 0x72:case 0xcb:case 0xcc:case 0xe8:case 0xe9:case 0xcd:case 0xb1:case 0xb2:case 0xb3:case 0xe7:return 1;
 case 0x1f:case 0x20:case 0xe2:case 0x21:case 0x47:case 0xbf:case 0x54:case 0x55:return 2;
 case 0x1e:return 3;
 case 0x5c:return 4;
 default:return 0;
 }
}
