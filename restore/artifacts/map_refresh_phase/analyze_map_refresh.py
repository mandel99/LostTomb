"""Read-only pinned native-call/data dossier; writes only this new phase's evidence."""
from pathlib import Path
import collections,hashlib,json,re,struct
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
image_path=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(image_path)==PIN;image=image_path.read_bytes()
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28)
def read(va,n):
    for k in range(u16(pe+6)):
        q=table+k*40;vs,rva,rawsz,raw=struct.unpack_from('<IIII',image,q+8)
        if base+rva<=va and va+n<=base+rva+max(vs,rawsz):
            pos=va-base-rva;part=image[raw+pos:raw+min(pos+n,rawsz)] if pos<rawsz else b''
            return part+b'\0'*(n-len(part))
    raise AssertionError(('unmapped native range',hex(va),n))
index_path=ROOT/'metadata/native-function-audit-cache/index.json';index=json.loads(index_path.read_text());assert index['native_sha256']==PIN and index['completed']
functions={};batches={}
for name in index['batches']:
    p=ROOT/name;batch=json.loads(p.read_text());assert batch['native_sha256']==PIN
    batches[name]=sha(p)
    for entry in batch['entries']:functions[int(entry['requested_address'],16)]=(name,entry)
selected={};queue=collections.deque([0x488e90,0x489030]);missing=[]
while queue:
    address=queue.popleft()
    if address in selected:continue
    assert address in functions,hex(address)
    origin,entry=functions[address];assembly=entry['assembly'];assert entry['status']=='exact_ghidra_function' and not assembly['has_more'] and len(assembly['items'])==assembly['total']
    calls=[];indirect_jumps=[]
    for item in assembly['items']:
        a=int(item['address'],16);b=bytes.fromhex(item['bytes']);assert read(a,len(b))==b,(hex(a),b.hex())
        if item['instruction'].startswith('CALL '):
            match=re.fullmatch(r'CALL 0x([0-9a-fA-F]+)',item['instruction']);assert match,item
            target=int(match[1],16);calls.append(f'{target:08x}');queue.append(target)
        if item['instruction'].startswith('JMP dword'):indirect_jumps.append(item)
    selected[address]={'cache_batch':origin,'info':entry['info'],'assembly':assembly,'direct_calls':sorted(set(calls)),'indirect_jumps':indirect_jumps}
constants={}
for address in [0x56f470,0x56f520,0x56f528,0x56f570,0x56f578,0x56f580,0x56f588,0x56f590,0x56f598,0x56f5a0,0x56f660,0x56f668,0x56f670]:
    b=read(address,8);constants[f'{address:08x}']={'bytes':b.hex(),'binary64_value':struct.unpack('<d',b)[0]}
native={'format':1,'native_sha256':PIN,'original_game_executed':False,'method':'Complete exact Ghidra function objects, every instruction byte independently matched against pinned original PE; direct-call closure only, no linear gap disassembly.','direct_call_closure_functions':len(selected),'direct_call_closure_instructions':sum(f['assembly']['total'] for f in selected.values()),'functions':{f'{a:08x}':selected[a] for a in sorted(selected)},'binary64_constants':constants,'cache_provenance':{'index':{'path':str(index_path.relative_to(ROOT)).replace('\\','/'),'sha256':sha(index_path)},'all_batches':batches},'limitations':['Indirect switch tables inside47A220 require separate data/target authentication before a whole-refresh proof. Direct-call closure is not a closed native catalogue.','No source algorithm equivalence is inferred from byte authentication.']}
(HERE/'map_refresh_native.json').write_text(json.dumps(native,indent=2)+'\n')
print(json.dumps({'functions':len(selected),'instructions':native['direct_call_closure_instructions'],'leaf_481aa0':selected[0x481aa0]['assembly']['total']}))
