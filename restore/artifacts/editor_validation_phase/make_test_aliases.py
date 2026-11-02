"""Test-only aliases and fixture point metadata; no production C algorithms generated."""
from pathlib import Path
import json
P=Path(__file__).resolve().parent;D=P/'tests';D.mkdir(exist_ok=True)
groups=[(0x784b64,0x784b6c,2,4),(0x784cc0,0x784cd0,4,4),(0x784b74,0x784b84,2,8),(0x784c64,0x784c66,4,2),(0x784c8c,0x784c8e,4,2),(0x784bc0,0x784be0,2,8),(0x784bd0,0x784bf0,2,8)]
points=[]
for group,(x,y,stride,count) in enumerate(groups):
 for j in range(count):points.append({'x':x+j*stride,'y':y+j*stride,'width':4 if group==1 else 2,'group':group})
(D/'points.json').write_text(json.dumps(points,indent=2)+'\n')
(D/'editor_points.h').write_text('typedef struct EditorPoint {uint32_t x,y,width,group;} EditorPoint;\nstatic const EditorPoint points[36]={\n'+',\n'.join('{0x%xu,0x%xu,%u,%u}'%(p['x'],p['y'],p['width'],p['group']) for p in points)+'\n};\n')
callbacks=['00409830','004ec360','004095c0','00409650','004096e0','00409760','004097c0','00409620']
for variant in ['trace','plain']:
 out=['/* Test-only aliases; include exact staged or applied C algorithms. */']
 if variant=='trace':out+=['#define EDITOR_VALIDATION_TRACE']
 out+=['#define EV_PUBLIC(name) EV_BODY_##name']
 out+=['#define EV_BODY_FUN_'+a+' editor_'+variant+'_'+a for a in callbacks]
 out+=['#include "editor_validation.h"']
 def alias(a):out.append('#define FUN_'+a+' editor_'+variant+'_'+a)
 def undo(a):out.append('#undef FUN_'+a)
 def include(staged,root):out.extend(['#ifdef EDITOR_VALIDATION_STAGED','#include "../'+staged+'"','#else','#include "../../../'+root+'"','#endif'])
 include('runtime/editor_validation.c','runtime/editor_validation.c')
 out+=['#undef EV_PUBLIC']
 out+=['#undef EV_BODY_FUN_'+a for a in callbacks]
 alias('004ec410');include('src/unit_02079.c','src/unit_02079.c');undo('004ec410')
 alias('00409820');out+=['#include "../../../src/unit_00071.c"'];undo('00409820')
 alias('00443710');out+=['#include "../../../src/unit_00621.c"'];undo('00443710')
 alias('004cdff0');out+=['#include "../../../src/unit_01739.c"'];undo('004cdff0')
 alias('004ce3a0');out+=['#include "../../../src/unit_01745.c"'];undo('004ce3a0')
 (D/('editor_actual_'+variant+'.c')).write_text('\n'.join(out)+'\n')
