#include "pharaoh/monuments/image_queries.h"

static uint32_t group(const PharaohMonumentQueryContext *c,int32_t id)
{return c->surface->groups[id].first_image;}
static void load(PharaohMonumentQueryContext *c,const char *name,int32_t slot)
{(void)pharaoh_sprite_library_load_file(c->libraries,name,slot);}

/* 00401800: part kind and orientation precede the load; stage follows it. */
uint32_t pharaoh_abu_simbel_query_image(PharaohMonumentQueryContext *c,
    int32_t id,uint32_t *flags)
{
    const PharaohBuildingTypeRecord *const b=&c->surface->buildings[id];
    const int32_t kind=c->parts[b->subtype].kind;
    const uint32_t view=(uint32_t)(*c->surface->orientation/2);
    const int side=view==0 || view==3;
    *flags=0;
    load(c,"AbuSimbel",26);
    if (kind>=0 && kind<=6) {
        static const uint16_t groups[7][4]={
            {822,824,825,823},{824,825,823,822},{823,822,824,825},
            {819,818,821,820},{820,819,818,821},{818,821,820,819},{821,820,819,818}
        };
        return view<4?group(c,groups[kind][view]):0;
    }
    switch (kind) {
    case 7:return side?group(c,815)+(view&1)+(uint32_t)b->monument_stage*2u:
        group(c,814)+((~view)&1);
    case 8:return group(c,816)+(view&1)+(side?(uint32_t)b->monument_stage*2u:14u);
    case 9:return group(c,817)+(view&1)+(b->monument_stage>0?(uint32_t)b->monument_stage*2u:0u);
    case 10:case 11:return side?group(c,813)+(view&1)+(uint32_t)b->monument_stage*2u:
        group(c,812)+((~view)&1);
    default:return 0;
    }
}

/* 0040DE80: branch order retained from the pinned typed export. */
uint32_t pharaoh_library_query_image(PharaohMonumentQueryContext *c,
    int32_t id,uint32_t *flags)
{
  int32_t kind;
  int32_t orientation;
  int32_t stage;
  uint32_t view;
  const char *script;

  orientation = *c->surface->orientation;
  stage = c->surface->buildings[id].monument_stage;
  if (2 < stage) {
    stage = c->surface->buildings[id-c->surface->buildings[id].subtype].monument_stage;
  }
  kind = c->parts[c->surface->buildings[id].subtype].kind;
  *flags = 0;
  view = orientation / 2;
  switch(kind) {
  case 0:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (4 < stage) {
      if (stage < 8) {
        script = "LibMainb1";
      }
      else if (stage < 0xb) {
        script = "LibMainb2";
      }
      else if (stage < 0xe) {
        script = "LibMainb3";
      }
      else if (stage < 0x12) {
        script = "LibMainb4";
      }
      else {
        script = "LibMainb5";
      }
      load(c,script,0x15);
      return (view & 1) + (int32_t)group(c,767);
    }
    break;
  case 1:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (stage < 4) break;
    if (stage < 6) {
      if (1 < (int)view) goto LAB_0040ea3a;
      script = "LibMaina1";
      goto LAB_0040e060;
    }
    if (stage < 7) {
      if ((int)view < 2) goto LAB_0040dfa5;
    }
    else {
      if (8 < stage) {
        if (stage < 10) {
          if ((int)view < 2) goto LAB_0040dfd9;
        }
        else {
          if (0xb < stage) {
            if (stage < 0xd) {
              if ((int)view < 2) goto LAB_0040e007;
            }
            else {
              if (0xe < stage) {
                if (0x10 < stage) {
                  if (stage < 0x13) {
                    if (1 < (int)view) goto LAB_0040e044;
                  }
                  else if (1 < (int)view) {
LAB_0040e181:
                    script = "LibMainc5";
                    goto LAB_0040e188;
                  }
LAB_0040e059:
                  script = "LibMaina5";
LAB_0040e060:
                  load(c,script,0x14);
                  return (view & 1) + (int32_t)group(c,766);
                }
                if ((int)view < 2) goto LAB_0040e031;
LAB_0040e044:
                script = "LibMainc4";
                goto LAB_0040e188;
              }
              if ((int)view < 2) {
                script = "LibMaina4";
                goto LAB_0040e060;
              }
            }
LAB_0040e141:
            script = "LibMainc3";
            goto LAB_0040e188;
          }
          if ((int)view < 2) {
            script = "LibMaina3";
            goto LAB_0040e060;
          }
        }
LAB_0040e113:
        script = "LibMainc2";
        goto LAB_0040e188;
      }
      if ((int)view < 2) {
        script = "LibMaina2";
        goto LAB_0040e060;
      }
    }
LAB_0040e0df:
    script = "LibMainc1";
LAB_0040e188:
    load(c,script,0x16);
    return (view & 1) + (int32_t)group(c,768);
  case 2:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (stage < 4) break;
    if (stage < 6) {
      if ((int)view < 2) {
        stage = (int32_t)group(c,500);
        *flags = 1;
        return stage;
      }
    }
    else {
      if (6 < stage) {
        if (stage < 9) {
          if ((int)view < 2) goto LAB_0040e0df;
        }
        else {
          if (9 < stage) {
            if (stage < 0xc) {
              if ((int)view < 2) goto LAB_0040e113;
            }
            else {
              if (0xc < stage) {
                if (stage < 0xf) {
                  if ((int)view < 2) goto LAB_0040e141;
                }
                else {
                  if (0x10 < stage) {
                    if (stage < 0x13) {
                      if ((int)view < 2) {
                        script = "LibMainc4";
                        goto LAB_0040e188;
                      }
                    }
                    else if ((int)view < 2) goto LAB_0040e181;
                    goto LAB_0040e059;
                  }
                  if ((int)view < 2) {
                    script = "LibMainc4";
                    goto LAB_0040e188;
                  }
                }
LAB_0040e031:
                script = "LibMaina4";
                goto LAB_0040e060;
              }
              if ((int)view < 2) {
                script = "LibMainc3";
                goto LAB_0040e188;
              }
            }
LAB_0040e007:
            script = "LibMaina3";
            goto LAB_0040e060;
          }
          if ((int)view < 2) {
            script = "LibMainc2";
            goto LAB_0040e188;
          }
        }
LAB_0040dfd9:
        script = "LibMaina2";
        goto LAB_0040e060;
      }
      if ((int)view < 2) {
        script = "LibMainc1";
        goto LAB_0040e188;
      }
    }
LAB_0040dfa5:
    script = "LibMaina1";
    goto LAB_0040e060;
  case 3:
    if (stage < 2) {
      stage = (int32_t)group(c,769);
      *flags = 7;
      return stage;
    }
    if (stage < 0x10) break;
    if (stage < 0x14) {
      script = "LibExtra";
    }
    else {
      script = "LibExtra2";
      if (0x14 < stage) {
        load(c,"LibExtra2",0x17);
        if ((int)view < 2) {
          return (int32_t)group(c,770);
        }
        goto LAB_0040e334;
      }
    }
    load(c,script,0x17);
    if ((int)view < 2) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
LAB_0040e334:
    return (view & 1) + (int32_t)group(c,783);
  case 4:
    if (stage < 2) {
      stage = (int32_t)group(c,769);
      *flags = 7;
      return stage;
    }
    if (stage < 0x10) break;
    if (stage < 0x14) {
      script = "LibExtra";
    }
    else {
      script = "LibExtra2";
      if (0x14 < stage) {
        load(c,"LibExtra2",0x17);
        if ((int)view < 2) {
          return (int32_t)group(c,770);
        }
        goto LAB_0040e2b3;
      }
    }
    load(c,script,0x17);
    if ((int)view < 2) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
LAB_0040e2b3:
    return (view & 1) + (int32_t)group(c,782);
  case 5:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (stage < 0x10) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
    if (stage < 0x14) {
      script = "LibExtra";
    }
    else {
      script = "LibExtra2";
      if (0x14 < stage) {
        load(c,"LibExtra2",0x17);
        if ((int)view < 2) {
          return (int32_t)group(c,771);
        }
        goto LAB_0040e51e;
      }
    }
    load(c,script,0x17);
    if (1 < (int)view) {
LAB_0040e51e:
      return (view & 1) + (int32_t)group(c,779);
    }
    break;
  case 6:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (0xf < stage) {
      if (stage < 0x14) {
        load(c,"LibExtra",0x17);
        if ((int)view < 2) {
          stage = (int32_t)group(c,500);
          *flags = 1;
          return stage;
        }
      }
      else if (stage < 0x15) {
        load(c,"LibExtra2",0x17);
        if ((int)view < 2) {
          stage = (int32_t)group(c,500);
          *flags = 1;
          return stage;
        }
      }
      else {
        load(c,"LibExtra2",0x17);
        if ((int)view < 2) {
          return (int32_t)group(c,771);
        }
      }
LAB_0040e4a0:
      return (view & 1) + (int32_t)group(c,778);
    }
    break;
  case 7:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (stage < 0x10) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
    if (0x13 < stage) {
      load(c,"LibExtra2",0x17);
      if ((int)view < 2) {
        return (int32_t)group(c,773);
      }
      return (view & 1) + (int32_t)group(c,785);
    }
    load(c,"LibExtra",0x17);
    if ((int)view < 2) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
LAB_0040eadd:
    return (view & 1) + (int32_t)group(c,785);
  case 8:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (stage < 0x10) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
    if (0x13 < stage) {
      load(c,"LibExtra2",0x17);
      if ((int)view < 2) {
        return (int32_t)group(c,773);
      }
      return (view & 1) + (int32_t)group(c,784);
    }
    load(c,"LibExtra",0x17);
    if ((int)view < 2) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
LAB_0040ea66:
    return (view & 1) + (int32_t)group(c,784);
  case 9:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (stage < 0x10) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
    if (stage < 0x14) {
      script = "LibExtra";
    }
    else {
      script = "LibExtra2";
      if (0x14 < stage) {
        load(c,"LibExtra2",0x17);
        if ((int)view < 2) {
          return (int32_t)group(c,774);
        }
        goto LAB_0040e5fc;
      }
    }
    load(c,script,0x17);
    if ((int)view < 2) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
LAB_0040e5fc:
    return (view & 1) + (int32_t)group(c,777);
  case 10:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (stage < 0x15) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
    goto LAB_0040e8fa;
  case 0xb:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (stage < 0x15) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
LAB_0040e8fa:
    if (1 < (int)view) {
      return (int32_t)group(c,773);
    }
    return (int32_t)group(c,775);
  case 0xc:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (stage < 0x10) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
    if (stage < 0x14) {
      script = "LibExtra";
    }
    else {
      script = "LibExtra2";
      if (0x14 < stage) {
        load(c,"LibExtra2",0x17);
        if ((int)view < 2) {
          return (int32_t)group(c,774);
        }
        goto LAB_0040e59c;
      }
    }
    load(c,script,0x17);
    if ((int)view < 2) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
LAB_0040e59c:
    return (view & 1) + (int32_t)group(c,776);
  case 0xd:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (0xf < stage) {
      if (stage < 0x14) {
        script = "LibExtra";
      }
      else {
        script = "LibExtra2";
      }
      load(c,script,0x17);
      if (1 < (int)view) {
LAB_0040e232:
        return (view & 1) + (int32_t)group(c,781);
      }
LAB_0040e1e2:
      return (view & 1) + (int32_t)group(c,780);
    }
    break;
  case 0xe:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (0xf < stage) {
      if (stage < 0x14) {
        script = "LibExtra";
      }
      else {
        script = "LibExtra2";
      }
      load(c,script,0x17);
      if ((int)view < 2) goto LAB_0040e232;
      goto LAB_0040e1e2;
    }
    break;
  case 0xf:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (0xf < stage) {
      if (stage < 0x14) {
        script = "LibExtra";
      }
      else {
        script = "LibExtra2";
        if (0x14 < stage) {
          load(c,"LibExtra2",0x17);
          if (1 < (int)view) {
            return (int32_t)group(c,774);
          }
          goto LAB_0040e59c;
        }
      }
      load(c,script,0x17);
      if ((int)view < 2) {
        return (view & 1) + (int32_t)group(c,776);
      }
      goto LAB_0040ea3a;
    }
    break;
  case 0x10:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (0xf < stage) {
      if (stage < 0x14) {
        script = "LibExtra";
      }
      else {
        script = "LibExtra2";
        if (0x14 < stage) {
          load(c,"LibExtra2",0x17);
          if (1 < (int)view) {
            return (int32_t)group(c,774);
          }
          return (view & 1) + (int32_t)group(c,777);
        }
      }
      load(c,script,0x17);
      if ((int)view < 2) goto LAB_0040e5fc;
      goto LAB_0040ea3a;
    }
    break;
  case 0x11:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (stage < 0x15) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
    goto LAB_0040e9e5;
  case 0x12:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (stage < 0x15) {
      stage = (int32_t)group(c,500);
      *flags = 1;
      return stage;
    }
LAB_0040e9e5:
    if ((int)view < 2) {
      return (int32_t)group(c,773);
    }
    return (int32_t)group(c,775);
  case 0x13:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (0xf < stage) {
      if (stage < 0x14) {
        script = "LibExtra";
      }
      else {
        script = "LibExtra2";
        if (0x14 < stage) {
          load(c,"LibExtra2",0x17);
          if (1 < (int)view) {
            return (int32_t)group(c,771);
          }
          goto LAB_0040e4a0;
        }
      }
      load(c,script,0x17);
      if ((int)view < 2) {
        return (view & 1) + (int32_t)group(c,778);
      }
      goto LAB_0040ea3a;
    }
    break;
  case 0x14:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (0xf < stage) {
      if (stage < 0x14) {
        script = "LibExtra";
      }
      else {
        script = "LibExtra2";
        if (0x14 < stage) {
          load(c,"LibExtra2",0x17);
          if (1 < (int)view) {
            return (int32_t)group(c,771);
          }
          goto LAB_0040e51e;
        }
      }
      load(c,script,0x17);
      if ((int)view < 2) {
        return (view & 1) + (int32_t)group(c,779);
      }
      goto LAB_0040ea3a;
    }
    break;
  case 0x15:
    if (stage < 2) {
      stage = (int32_t)group(c,769);
      *flags = 7;
      return stage;
    }
    if (0xf < stage) {
      if (stage < 0x14) {
        script = "LibExtra";
      }
      else {
        script = "LibExtra2";
        if (0x14 < stage) {
          load(c,"LibExtra2",0x17);
          if (1 < (int)view) {
            return (int32_t)group(c,770);
          }
          goto LAB_0040e2b3;
        }
      }
      load(c,script,0x17);
      if ((int)view < 2) {
        return (view & 1) + (int32_t)group(c,782);
      }
      goto LAB_0040ea3a;
    }
    break;
  case 0x16:
    if (stage < 2) {
      stage = (int32_t)group(c,769);
      *flags = 7;
      return stage;
    }
    if (0xf < stage) {
      if (stage < 0x14) {
        script = "LibExtra";
      }
      else {
        script = "LibExtra2";
        if (0x14 < stage) {
          load(c,"LibExtra2",0x17);
          if (1 < (int)view) {
            return (int32_t)group(c,770);
          }
          goto LAB_0040e334;
        }
      }
      load(c,script,0x17);
      if ((int)view < 2) {
        return (view & 1) + (int32_t)group(c,783);
      }
      goto LAB_0040ea3a;
    }
    break;
  case 0x17:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (0xf < stage) {
      if (0x13 < stage) {
        load(c,"LibExtra2",0x17);
        if (1 < (int)view) {
          return (int32_t)group(c,773);
        }
        return (view & 1) + (int32_t)group(c,784);
      }
      load(c,"LibExtra",0x17);
      if ((int)view < 2) goto LAB_0040ea66;
    }
LAB_0040ea3a:
    stage = (int32_t)group(c,500);
    *flags = 1;
    return stage;
  case 0x18:
    if (stage < 2) {
      *flags = 1;
      return 0;
    }
    if (0xf < stage) {
      if (0x13 < stage) {
        load(c,"LibExtra2",0x17);
        if (1 < (int)view) {
          return (int32_t)group(c,773);
        }
        return (view & 1) + (int32_t)group(c,785);
      }
      load(c,"LibExtra",0x17);
      if (1 < (int)view) goto LAB_0040ea3a;
      goto LAB_0040eadd;
    }
    break;
  default:
    return 0;
  }
  stage = (int32_t)group(c,500);
  *flags = 1;
  return stage;
}

/* 00417310: offset is cached before loading; SG3 group is read afterwards. */
uint32_t pharaoh_caesareum_query_image(PharaohMonumentQueryContext *c,
    int32_t id,uint32_t *flags)
{
    static const uint8_t offsets[36]={0,1,2,3,4,5,6,7,8,2,5,8,1,4,7,0,3,6,
        8,7,6,5,4,3,2,1,0,6,3,0,7,4,1,8,5,2};
    const int32_t part=c->surface->buildings[id].subtype;
    const uint32_t offset=offsets[(*c->surface->orientation/2)*9+part];
    *flags=0;
    load(c,c->stage_scripts[c->surface->buildings[id-part].monument_stage],18);
    return group(c,764)+offset;
}

/* 004E6A70: kind1 and unsupported kinds leave the caller's flags untouched. */
uint32_t pharaoh_lighthouse_query_image(PharaohMonumentQueryContext *c,
    int32_t id,uint32_t *flags)
{
    const PharaohBuildingTypeRecord *const b=&c->surface->buildings[id];
    const int32_t kind=c->parts[b->subtype].kind;
    if (kind==1) return group(c,0);
    if (kind!=0) return 0;
    *flags=0;
    load(c,c->stage_scripts[b->monument_stage],19);
    return group(c,765);
}

/* 005414B0; orientation and groups after SG3 load remain live. */
uint32_t pharaoh_sun_temple_query_image(PharaohMonumentQueryContext *c,
    int32_t id,uint32_t *flags)
{
  static const char *const scripts[3]={"suntemple1","suntemple2","suntemple3"};
  int32_t variant;
  int32_t stage;

  *flags = 0;
  switch(c->parts[c->surface->buildings[id].subtype].kind) {
  case 0:
    if (c->surface->buildings[id].monument_stage < 9) {
      stage = 0;
    }
    else {
      stage = (0xc < c->surface->buildings[id].monument_stage) + 1;
    }
    variant = c->surface->buildings[id].sun_temple_variant;
    if (variant != 0) {
      if (variant == 1) {
        load(c,scripts[stage],6);
        return (*c->surface->orientation / 2 & 1U) + group(c,565);
      }
      if (variant == 2) {
        load(c,scripts[stage],7);
        return (*c->surface->orientation / 2 & 1U) + group(c,566);
      }
    }
    load(c,scripts[stage],5);
    return (*c->surface->orientation / 2 & 1U) + group(c,564);
  case 1:
    if (c->surface->buildings[id].monument_stage == 2) {
      return (*c->surface->orientation / 2 & 1U) + group(c,568);
    }
    break;
  case 2:
    if (c->surface->buildings[id].monument_stage == 2) {
      return ((8 - *c->surface->orientation) / 2 & 3U) + 2 + group(c,568);
    }
    break;
  case 3:
    if (c->surface->buildings[id].monument_stage == 2) {
      return ((8 - *c->surface->orientation) / 2 + 1U & 3) + 2 + group(c,568);
    }
    break;
  case 4:
    if (c->surface->buildings[id].monument_stage == 2) {
      return ((8 - *c->surface->orientation) / 2 - 2U & 3) + 2 + group(c,568);
    }
    break;
  case 5:
    if (c->surface->buildings[id].monument_stage == 2) {
      return ((8 - *c->surface->orientation) / 2 - 1U & 3) + 2 + group(c,568);
    }
    break;
  case 6:
    if (c->surface->buildings[id].monument_stage == 2) {
      return (*c->surface->orientation / 2 & 1U) + 6 + group(c,568);
    }
    goto LAB_00541723;
  case 7:
    if (c->surface->buildings[id].monument_stage == 2) {
      return (*c->surface->orientation / 2 - 1U & 1) + 6 + group(c,568);
    }
    goto LAB_00541723;
  case 8:
    *flags = 5;
    stage = c->surface->buildings[id].monument_stage;
    if (stage == 2) {
      return group(c,568) + 8;
    }
    goto LAB_0054172f;
  case 9:
    *flags = 5;
    stage = c->surface->buildings[id].monument_stage;
    if (stage == 2) {
      return group(c,568) + 9;
    }
    goto LAB_0054172f;
  case 10:
    if (c->surface->buildings[id].monument_stage == 2) {
      return group(c,568) + 10;
    }
    goto LAB_00541723;
  case 0xb:
    if (c->surface->buildings[id].monument_stage == 2) {
      return group(c,568) + 0xb;
    }
LAB_00541723:
    *flags = 5;
    stage = c->surface->buildings[id].monument_stage;
LAB_0054172f:
    return (stage==1?group(c,500):0u);
  case 0xc:
    if (c->surface->buildings[id].monument_stage == 4) {
      return group(c,568) + 0xc;
    }
    *flags = 5;
    if (c->surface->buildings[id].monument_stage < 1) {
      *flags = 7;
      return group(c,759);
    }
    return group(c,500);
  default:
    return 0;
  }
  *flags = 5;
  if (c->surface->buildings[id].monument_stage != 1) {
    return group(c,759);
  }
  return group(c,500);
}

static uint32_t abu(void *c,int32_t id,uint32_t *f){return pharaoh_abu_simbel_query_image(c,id,f);}
static uint32_t library(void *c,int32_t id,uint32_t *f){return pharaoh_library_query_image(c,id,f);}
static uint32_t caesar(void *c,int32_t id,uint32_t *f){return pharaoh_caesareum_query_image(c,id,f);}
static uint32_t lighthouse(void *c,int32_t id,uint32_t *f){return pharaoh_lighthouse_query_image(c,id,f);}
static uint32_t sun(void *c,int32_t id,uint32_t *f){return pharaoh_sun_temple_query_image(c,id,f);}
void pharaoh_monument_bind_image_query(PharaohMonumentImageContext *images,
    enum PharaohMonumentQueryMethod method,PharaohMonumentQueryContext *query)
{
    static uint32_t (*const methods[5])(void *,int32_t,uint32_t *)={abu,library,caesar,lighthouse,sun};
    images->query_image=methods[method];images->query_user_data=query;
}
