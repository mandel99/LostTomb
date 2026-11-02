"""Authenticated direct-dependency inventory; source presence is not a proof."""
from pathlib import Path
import hashlib,json,re,struct
ROOT=Path(__file__).resolve().parents[1]
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(image).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;section_table=opt+u16(pe+20);base=u32(opt+28);sections=[]
for i in range(u16(pe+6)):
    q=section_table+i*40;sections.append((base+u32(q+12),u32(q+16),u32(q+20)))
def original(address,n):
    for lo,size,raw in sections:
        if lo<=address and address+n<=lo+size:return image[raw+address-lo:raw+address-lo+n]
    raise ValueError(f'unbacked {address:x}/{n}')
status=json.loads((ROOT/'metadata/dropdown-action-status.json').read_text())
assert set(status['missing_tokens'])=={'00409540','00409830','00418510'}
roots=json.loads((ROOT/'metadata/dropdown-action-native-cfg.json').read_text());assert roots['native_sha256']==digest
cache={}
for path in (ROOT/'metadata/native-function-audit-cache').glob('batch-*.json'):
    batch=json.loads(path.read_text());assert batch['native_sha256']==digest
    for f in batch['entries']:cache[f['requested_address']]={**f,'cache':path.relative_to(ROOT).as_posix()}
definitions={}
for directory in ['src','runtime']:
    for path in (ROOT/directory).glob('*.c'):
        for m in re.finditer(r'(?m)^[ \t]*(?:[A-Za-z_][A-Za-z0-9_]*[ \t]+)+\**FUN_([0-9a-f]{8})\s*\([^;{}]*\)\s*\{',path.read_text(errors='replace')):definitions.setdefault(m[1],[]).append(path.relative_to(ROOT).as_posix())
def authenticate(f):
    a=f['assembly'];assert not a['has_more'] and len(a['items'])==a['total']
    for x in a['items']:
        raw=bytes.fromhex(x['bytes']);assert original(int(x['address'],16),len(raw))==raw
    return a['total']
out=[];deps={}
for entry in status['missing_tokens']:
    f=roots['functions'][entry];instructions=authenticate(f);calls=[]
    for x in f['assembly']['items']:
        match=re.fullmatch(r'CALL 0x([0-9a-fA-F]+)',x['instruction'])
        if match:
            target=f'{int(match[1],16):08x}';calls.append({'site':x['address'],'target':target})
            if target not in deps:
                native=cache.get(target);deps[target]={'entry':target,'C_sources':definitions.get(target,[]),'native_cache_available':native is not None,'native_instructions_authenticated':authenticate(native) if native else None,'cache':native['cache'] if native else None,'verified_by_this_inventory':False}
    out.append({'entry':entry,'native_instructions_authenticated':instructions,'unresolved_indirect_jumps':f['unresolved_indirect_jumps'],'direct_calls':calls})
report={'format':1,'native_sha256':digest,'complete_menu_reconstruction':False,'roots':out,'unique_direct_dependencies':len(deps),'dependencies':list(deps.values()),'limitations':['Only retained native instructions are authenticated; C definitions and inferred Ghidra signatures are not promoted to correct implementations.','This direct-call inventory does not close transitive dependencies or prove callback results/ABI.','Full menu completion also requires unfinished settings, two unrouted actions, input, rendering and modal behavior.']}
(ROOT/'metadata/remaining-dropdown-dependencies.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'missing_callbacks':len(out),'unique_direct_dependencies':len(deps),'authenticated_callee_bodies':sum(d['native_cache_available'] for d in deps.values())}))
