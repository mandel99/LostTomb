"""Stage the reviewed whole refresh-body repair; no product files are written."""
from pathlib import Path
import hashlib,json,re
ROOT=Path(__file__).resolve().parents[2];HERE=Path(__file__).resolve().parent
source=ROOT/'src/unit_01867.c';assert hashlib.sha256(source.read_bytes()).hexdigest()=='638bc1f8ed206a710ce6c3afa82e324dff781a3a3ecf0528bacd7d9f7ded6b9c'
d=json.loads((HERE/'native-review.json').read_text());text=source.read_text();changes=[]
assert len(next(x for x in d['fields'] if x['address']=='00e38dda')['uses'])==1
text=text.replace('#include "restore_types.h"','#include "refresh_menu_dispatch.h"',1)
for field in d['fields']:
 address=field['address'];widths=field['direct_widths'];old=field['current_C']
 if not old or not widths or address=='006fdc54':continue
 assert len(widths)==1
 width=widths[0];ctype='int32_t' if width==4 else 'uint8_t'
 pattern=r'^#define DAT_'+address+r' .*$'
 new='#define DAT_'+address+' (*(volatile '+ctype+' *)0x'+address+'u)'
 text,n=re.subn(pattern,new,text,flags=re.M);assert n==1,address;changes.append({'address':address,'native_width':width,'before':old,'after':new})
for address,ctype in [('0077f748','uint8_t'),('007999e8','int16_t'),('00ea4096','uint8_t')]:
 text,n=re.subn(r'^#define DAT_'+address+r' .*$', '#define DAT_'+address+' (*(volatile '+ctype+' *)0x'+address+'u)',text,flags=re.M);assert n==1
 # EA4096 is address-only here, then cast/dereferenced as WORD. A WORD lvalue
 # would introduce another factor2 before the explicit byte offset.
text,n=re.subn(r'^#define DAT_00e38dda .*\n','',text,flags=re.M);assert n==1
text=text.replace('DAT_00e38dda','mode')
anchor='  undefined **ppuVar9;';assert text.count(anchor)==1;text=text.replace(anchor,anchor+'\n  uint8_t mode = *(volatile uint8_t *)0x00e38ddau;',1)
# Existing typed facades retain the pushed ignored context and pointer values.
text=text.replace('FUN_0055ad30','restore_initial_dispatch_training')
text=text.replace('FUN_00426450','restore_refresh_hit').replace('FUN_004d6740','restore_refresh_hover')
# Calls use defined Win32 modulo32 arithmetic, including extreme coordinates.
text=text.replace('DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c','RD_ADD(RD_ADD(RD_MUL(DAT_00eadd84,16),-40),DAT_00eadd8c)')
text=text.replace('iVar3 * 4','RD_MUL(iVar3,4)').replace('(iVar3 + 7) * 0x10 + -8','RD_ADD(RD_MUL(RD_ADD(iVar3,7),16),-8)')
text=text.replace('(DAT_00f53f00 + -2) * 0x10 + DAT_00f53f08','RD_ADD(RD_MUL(RD_ADD(DAT_00f53f00,-2),16),DAT_00f53f08)')
for name in ['00e38da4','00e38da8','00e38dac','00e38db0','006fdc9c','006fdca0','00f53f64']:
 text=re.sub(r'DAT_'+name+r' \+ (-?0x[0-9a-f]+|[0-9]+)',lambda m:'RD_ADD(DAT_'+name+','+m[1]+')',text)
dest=HERE/'src/unit_01867.c';dest.parent.mkdir(parents=True,exist_ok=True);dest.write_text(text)
(HERE/'candidate-width-review.json').write_text(json.dumps({'production_modified':False,'C_equivalence_verified':False,'direct_bindings':changes,'indexed_bindings':{'0077f748':'BYTE cells,stride98','007999e8':'signedWORD records,stride264','00ea4096':'BYTE address-only base, explicitWORD dereference, playerstride49D8'},'mode':'One entry BYTE capture, as native917body; do not resample after helper calls.','remaining_requirements':['Actual-source Debug/O2 compilation','Full native/C controller graph, caller ABI and helper mutation comparison','Native helper signature and unused pushed argument qualification','Guarded product application only after complete proof']},indent=2)+'\n')
print(json.dumps({'staged':True,'direct_bindings_reviewed':len(changes),'indexed_bindings_corrected':3,'production_modified':False,'C_equivalence_verified':False}))
