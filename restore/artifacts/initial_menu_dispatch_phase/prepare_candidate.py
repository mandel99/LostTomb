"""Stage a reviewed dispatcher repair and explicit test-only renderer boundaries."""
from pathlib import Path
import hashlib,json,re
ROOT=Path(__file__).resolve().parents[2];HERE=Path(__file__).resolve().parent
source=(ROOT/'src/unit_01866.c').read_bytes()
assert hashlib.sha256(source).hexdigest()=='707a3b53d695a2430e1778c664768f737798f81f4d2da9e59295ee46e96bc6fe'
text=source.decode().replace('\r\n','\n')
text=text.replace('#include "restore_types.h"','#include "initial_menu_dispatch.h"',1)
text=re.sub(r'^#define DAT_006059(?:4c|50|54).*\n','',text,flags=re.M)
for name,expression in {'00d3a6c4':'ID_READ32(0x00d3a6c4u)','00d3a6e4':'ID_READ32(0x00d3a6e4u)','00e38dca':'ID_READ8(0x00e38dcau)','00e38dda':'ID_READ8(0x00e38ddau)'}.items():
 text,n=re.subn(r'^#define DAT_'+name+r' .*$', '#define DAT_'+name+' '+expression,text,flags=re.M);assert n==1,name
old='  uVar1 = DAT_00d3a6c4;\n  if (DAT_00d3a6e4 == 0) {\n    uVar1 = (uint)DAT_00e38dda;\n  }'
new='  if (DAT_00d3a6e4 != 0) {\n    uVar1 = DAT_00d3a6c4;\n  }\n  else {\n    uVar1 = (uint)DAT_00e38dda;\n  }'
assert text.count(old)==1;text=text.replace(old,new,1)
text,n=re.subn(r'DAT_00e38dca = ([^;]+);',r'ID_WRITE8(0x00e38dcau,\1);',text);assert n==47,n
for address in ['00605950','0060594c','00605954']:
 old='*(int32_t *)0x'+address+'u';assert text.count(old)==1;text=text.replace(old,'(int32_t)ID_READ32(0x'+address+'u)')
types={'00520ab0':'int value','0055ad30':'uint32_t value','00530d70':'int x,int y,int rows'}
text=re.sub(r'extern (int|void) FUN_([0-9a-f]{8})\(\);',lambda m:'extern '+m[1]+' FUN_'+m[2]+'('+types.get(m[2],'void')+');',text)
prototypes=re.findall(r'extern (int|void) FUN_([0-9a-f]{8})\(([^;]*)\);',text)
native=json.loads((HERE/'native-provider.json').read_text())
calls={int(i['instruction'].split()[-1],16) for i in native['assembly']['items'] if i['instruction'].startswith('CALL ')}
assert calls=={int(p[1],16) for p in prototypes}
stubs=[]
for kind,token,args in prototypes:
 names=[] if args=='void' else [v.strip().split()[-1] for v in args.split(',')]
 values=names+['0']*(3-len(names))
 symbol='restore_initial_dispatch_training' if token=='0055ad30' else 'FUN_'+token
 stubs.append(kind+' '+symbol+'('+args+'){event(0x'+token+'u,'+str(len(names))+','+','.join('(uint32_t)('+v+')' for v in values)+');'+('return (int)p[20];' if kind=='int' else '')+'}')
text=text.replace('FUN_0055ad30','restore_initial_dispatch_training')
dest=HERE/'src/unit_01866.c';dest.parent.mkdir(parents=True,exist_ok=True);dest.write_text(text)
(HERE/'tests').mkdir(exist_ok=True)
(HERE/'tests/renderer_boundaries.inc').write_text('/* Exact named renderer/controller boundaries, not product implementations. */\n'+'\n'.join(stubs)+'\n')
for variant in ['trace','plain']:
 definitions='#define FUN_004d71c0 initial_dispatch_'+variant+'\n'
 if variant=='trace':definitions+='#define INITIAL_DISPATCH_TRACE\n'
 (HERE/f'tests/actual_{variant}.c').write_text(definitions+'#ifdef INITIAL_DISPATCH_STAGED\n#include "../src/unit_01866.c"\n#else\n#include "../../../src/unit_01866.c"\n#endif\n')
print(json.dumps({'candidate_created':True,'named_boundaries':len(calls),'production_changed':False}))
