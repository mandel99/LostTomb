"""Prepare explicit boundary declarations and an exact shared-helper extraction."""
from pathlib import Path
import hashlib,json,re
ROOT=Path(__file__).resolve().parents[2];HERE=Path(__file__).resolve().parent
d=json.loads((HERE/'native-provider.json').read_text());targets={int(i['instruction'].split()[-1],16) for i in d['assembly']['items'] if i['instruction'].startswith('CALL ')}
assert len(targets)==102
owned={0x41e100,0x41e130,0x41e140,0x41e170,0x5165b0};bare={0x51f780}
cache={};all_cache={}
for path in (ROOT/'metadata/native-function-audit-cache').glob('batch-*.json'):
 for entry in json.loads(path.read_text())['entries']:
  token=int(entry['requested_address'],16)
  all_cache[token]=(entry,path)
  if token in targets:cache[token]=(entry,path)
assert set(cache)==targets
source=(HERE/'src/unit_01867.c').read_text();prototypes={int(m[2],16):m[1] for m in re.finditer(r'extern ([^;\n]+?) FUN_([0-9a-f]{8})\(',source)}
args={0x4256a0:'int x,int y,void *buttons,int count',0x425730:'int x,int y,void *buttons,int count',0x4261d0:'int x,int y,void *buttons,int count',0x426450:'int x,int y,const void *buttons,int count,int origin_x,int origin_y',0x4d6740:'int x,int y,const void *buttons,int count,int hit',0x51ed30:'int first,int value',0x51efb0:'int value',0x5309b0:'int first,int second',0x538fa0:'int value',0x4cde80:'int first,int second',0x5013a0:'uint32_t record',0x55ad30:'uint32_t context',0x55f5a1:'RestoreSound *self,int channel',0x55f449:'RestoreSound *self,const char *filename,int channel,uint32_t loops',0x413470:'RestoreVideoPlayer *self'}
symbols={0x426450:'restore_refresh_hit',0x4d6740:'restore_refresh_hover',0x55ad30:'restore_initial_dispatch_training'}
returns={0x55f5a1:'uint32_t',0x55f449:'uint32_t',0x413470:'unsigned char',0x426450:'int',0x4d6740:'int'}
records=[];stubs=[]
def returns_for(token,seen=None):
 seen=set() if seen is None else seen
 assert token not in seen,('tail cycle',hex(token));seen=seen|{token}
 entry,path=all_cache[token];asm=entry['assembly'];assert not asm['has_more']
 addresses={int(i['address'],16) for i in asm['items']};pops=set();tails=[]
 for i in asm['items']:
  if i['instruction'].startswith('RET'):pops.add(int(i['instruction'].split()[1],16) if len(i['instruction'].split())>1 else 0)
  if i['instruction'].startswith('JMP 0x'):
   target=int(i['instruction'].split()[1],16)
   if target not in addresses:
    assert target in all_cache,('unowned tail',hex(token),hex(target))
    nested,names=returns_for(target,seen);pops|=nested;tails.append(f'{target:08x}');tails+=names
 return pops,tails
for token in sorted(targets):
 entry,path=cache[token];assert entry['status']=='exact_ghidra_function' and not entry['assembly']['has_more']
 pops,tails=returns_for(token)
 assert pops=={12 if token==0x55f449 else 4 if token==0x55f5a1 else 0},(hex(token),pops)
 if token in owned or token in bare:continue
 signature=args.get(token,'void');kind=returns.get(token,prototypes.get(token,'void'));symbol=symbols.get(token,'FUN_'+f'{token:08x}')
 values=[] if signature=='void' else [v.strip().split()[-1].lstrip('*') for v in signature.split(',')]
 normalized=['(uint32_t)(uintptr_t)'+v for v in values];native_count=len(values)-(1 if token in [0x55f5a1,0x55f449,0x413470] else 0)
 body=('uint32_t v[]={'+','.join(normalized)+'};' if values else '')+'refresh_fixture_event(0x'+f'{token:x}u,'+str(len(values))+','+('v' if values else 'NULL')+');'
 if kind!='void':body+='return ('+kind+')refresh_fixture_result(0x'+f'{token:x}u,'+(normalized[0] if normalized else '0')+');'
 stubs.append(kind+' '+symbol+'('+signature+'){'+body+'}')
 records.append({'native_entry':f'{token:08x}','C_symbol':symbol,'return_type':kind,'arguments':signature,'normalized_argument_count':len(values),'native_stack_argument_count':native_count,'receiver_ECX':token in [0x55f5a1,0x55f449,0x413470],'callee_pops':next(iter(pops)),'tail_return_entries':tails,'native_source':path.relative_to(ROOT).as_posix(),'native_source_sha256':hashlib.sha256(path.read_bytes()).hexdigest()})
assert len(records)==96
tests=HERE/'tests';tests.mkdir(exist_ok=True)
(tests/'boundaries.inc').write_text('/* Named controlled endpoints only; never linked into the product. */\n'+'\n'.join(stubs)+'\nvoid FUN_0051f780(void){} /* Authenticated native bareRET, not an unknown no-op. */\n')
mission=ROOT/'runtime/mission_menu.c';text=mission.read_text();start=text.index('void restore_mission_menu_buttons(void)');body=text[start:text.index('\n}',start)+2]
assert body.count('{')==body.count('}')==1
preamble='''/* Exact function text extracted from current ROOT; guarded below. */
#include "restore_types.h"
#include <stdint.h>
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void *FUN_0041e100(void),*FUN_0041e130(void);
extern uint32_t FUN_0041e140(void);
extern unsigned char FUN_0041e170(void);
extern void FUN_00425730(int,int,void *,int),FUN_0041f220(void);
'''
(tests/'actual_mission_buttons.c').write_text(preamble+body+'\n')
report={'format':1,'C_equivalence_verified':False,'native_sha256':d['native_sha256'],'controlled_endpoints':records,'actual_tiny_getters':[f'{a:08x}' for a in sorted(owned)],'actual_nested_file_getter':'0041dfe0','native_bare_RET':'0051f780','mission_helper_extraction':{'source':'runtime/mission_menu.c','source_sha256':hashlib.sha256(mission.read_bytes()).hexdigest(),'exact_function_text_sha256':hashlib.sha256(body.encode()).hexdigest(),'extracted':'artifacts/refresh_menu_dispatch_phase/tests/actual_mission_buttons.c'},'limitations':['ABI declarations and typed controlled endpoints are fixture preparation, not finished controller C equivalence.','Six actual small getters/file-list queries and the exact shared foreground helper will execute; complex rendering/audio/video/gameplay and button-engine algorithms remain named boundaries at this controller layer.']}
(HERE/'fixture-contracts.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'controlled_endpoints':len(records),'actual_small_getters':6,'actual_shared_helper_extracted':True,'product_files_modified':False,'C_equivalence_verified':False}))
