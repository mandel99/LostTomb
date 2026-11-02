"""Authenticate the next whole dispatcher scope; no production edits or C proof."""
from pathlib import Path
import hashlib,json,struct,sys
ROOT=Path(__file__).resolve().parents[2];HERE=Path(__file__).resolve().parent
data=json.loads((HERE/'native-provider.json').read_text());image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
sha=lambda b:hashlib.sha256(b).hexdigest();assert sha(image)==data['native_sha256']
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;base=u32(opt+28);table=opt+u16(pe+20)
sections=[(u32(table+j*40+12),u32(table+j*40+16),u32(table+j*40+20)) for j in range(u16(pe+6))]
def raw(address,n):
 rva=address-base
 for start,length,offset in sections:
  if start<=rva and rva+n<=start+length:return image[offset+rva-start:offset+rva-start+n]
 raise AssertionError(('not file backed',hex(address),n))
a=data['assembly'];assert not a['has_more'] and a['offset']==0 and a['total']==len(a['items'])==179
for i in a['items']:
 value=bytes.fromhex(i['bytes']);assert raw(int(i['address'],16),len(value))==value,i['address']
m=data['tables'];assert m['gaps']==[] and m['length']==276
assert raw(int(m['address'],16),276)==bytes.fromhex(m['hex'])
pointers=struct.unpack('<47I',raw(0x4d74b8,188));indices=raw(0x4d7574,88)
addresses={int(i['address'],16) for i in a['items']}
assert max(indices)<len(pointers) and set(pointers)<=addresses
facts={0x4d71de:'a1e4a6d300',0x4d71e7:'a1c4a6d300',0x4d71f0:'a0da8de300',0x4d71f5:'83f857',0x4d7200:'8a8874754d00',0x4d7206:'ff248db8744d00'}
for address,want in facts.items():assert raw(address,len(bytes.fromhex(want)))==bytes.fromhex(want)
source=ROOT/'src/unit_01866.c';text=source.read_text()
assert '#define DAT_00d3a6c4 (*(undefined *)0x00d3a6c4u)' in text
assert '#define DAT_00d3a6e4 (*(undefined *)0x00d3a6e4u)' in text
out={'format':1,'native_sha256':data['native_sha256'],'C_equivalence_verified':False,'production_changed':False,'native_instructions_authenticated':179,'switch_records_authenticated':88,'distinct_target_records':47,
 'mode_routes':{str(mode):f'{pointers[slot]:08x}' for mode,slot in enumerate(indices)},
 'confirmed_source_mismatches':[{'source':'src/unit_01866.c','field':'00d3a6e4','native_instruction':'004d71de MOV EAX,[00d3a6e4]','native_width_bytes':4,'current_C_width_bytes':1,'impact':'High bytes alone can select the alternate full-DWORD mode; current BYTE test misses that branch.'},{'source':'src/unit_01866.c','field':'00d3a6c4','native_instruction':'004d71e7 MOV EAX,[00d3a6c4]','native_width_bytes':4,'current_C_width_bytes':1,'impact':'Native alternate mode is unsigned-bounded at0x57 before the two-level switch; truncating a value such as0x100 can select an unintended renderer.'}],
 'qualification':'Native scope and confirmed scalar-width defects only. Whole CFG reachability, complete caller/endpoint ABI, compiled-C equivalence and guarded source application remain required.',
 'input_hashes':{'src/unit_01866.c':sha(source.read_bytes()),'artifacts/initial_menu_dispatch_phase/native-provider.json':sha((HERE/'native-provider.json').read_bytes()),'artifacts/initial_menu_dispatch_phase/authenticate_native.py':sha(Path(__file__).read_bytes())}}
(HERE/'native-authentication.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'authenticated_instructions':179,'authenticated_modes':88,'confirmed_width_defects':2,'C_equivalence_verified':False}))
