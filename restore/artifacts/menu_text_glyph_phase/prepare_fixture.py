from pathlib import Path
import json
HERE=Path(__file__).resolve().parent
names='OP SEED FONT X Y WIDTH COLOR TEXT CHARACTER SUBSTART ENABLE DIRECT SHADOW OUTLINE COLLAPSE BLOCK READY POSITION TARGET SELECTOR CURSOR BASELINE COUNT METRICS GLYPHS MUTPHASE MUTFIELD MUTVALUE MUTORD REPEATS MARKERS LONGCOUNT SUBPROFILE RESULT'.split()
fields=[('height',0xd3b500,4),('style_reset',0xd3b504,4),('direct',0xd3b510,4),('shadow',0xd3b514,4),('outline',0xd3b518,4),('color',0xd3b51c,2),('glyph',0xd3b5c0,4),('center',0xe20770,4),('baseline',0xe20774,4),('cursor',0xe20778,4),('subindex',0xe2077c,4),('enabled',0xe20780,4),('position',0xe20784,4),('ready',0xe20788,4),('selected_width',0xe2078c,4),('selected_x',0xe20790,4),('selected_y',0xe20794,4),('collapse',0xe20798,1),('collapse_block',0xe20799,1),('selector',0xe20990,4),('small_font',0xf56f84,4),('heading_font',0xf56fb4,4),('alternate_font',0xf56fbc,4),('count',0x126517c,4),('glyph_A',0x5e04bd,1),('zero_width',0x10d6a0c,2),('zero_height',0x10d6a0e,2)]
regions=[(0x5e045c,0x160),(0xd3b4e0,0x100),(0xe20750,0x650),(0xe34318,0x440),(0xe44318,0x240),(0xf56f60,0x90),(0x1265160,0x40),(0x10d65f8,0x20420),(0x4ac300e0,12065)]
schema={'format':1,'names':names,'fields':[{'name':n,'address':a,'width':w} for n,a,w in fields],'regions':regions,'text':0x4ac30100,'maximum_text':12000,'image_base':0x10d69f8,'substitution':0xe34338,'event_fields':5+len(fields)}
(HERE/'fixture-schema.json').write_text(json.dumps(schema,indent=2)+'\n')
(HERE/'tests').mkdir(exist_ok=True)
header='#ifndef MENU_TEXT_GLYPH_CASE_H\n#define MENU_TEXT_GLYPH_CASE_H\n#include <stdint.h>\nenum{'+','.join('P_'+x for x in names)+',P_FIELDS};\n'
header+='static const uint32_t tg_fields[][2]={'+','.join('{0x%x,%d}'%(a,w) for n,a,w in fields)+'};\n'
header+='static const uint32_t tg_regions[][2]={'+','.join('{0x%x,%d}'%(a,n) for a,n in regions)+'};\n'
header+='#define TG_FIELDS %du\n#define TG_EVENT_FIELDS %du\n#define TG_TEXT 0x4ac30100u\n#endif\n'%(len(fields),5+len(fields))
(HERE/'tests/text_case.h').write_text(header)
print(json.dumps({'parameters':len(names),'fields':len(fields),'watched_bytes':sum(n for a,n in regions)}))
