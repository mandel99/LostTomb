"""Check observed caller EAX consumption before retaining void C interfaces."""
from pathlib import Path
import json,hashlib,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from native_test_pe import PE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ctx=json.loads((ROOT/'metadata/actor-scenario-spawn-context.json').read_text())
native=ROOT/'build/smoke-game/Pharaoh.exe';digest=hashlib.sha256(native.read_bytes()).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=PE(native);decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
catalogue=json.loads((ROOT/'metadata/actor-scenario-spawn-native.json').read_text())
retained=json.loads((ROOT/'metadata/actor-reserved-native.json').read_text())

def decoded(at,hex_bytes,prefix=None):
 raw=bytes.fromhex(hex_bytes)
 assert raw==bytes(pe.data[at-pe.base:at-pe.base+len(raw)])
 if prefix is not None:raw=raw[:prefix]
 result=list(decoder.disasm(raw,at));assert sum(i.size for i in result)==len(raw)
 return result

def bytes_evidence(e,prefix=None):
 assert e['authority']=='shipped-artifact' and e['subject']['digest']['sha256']==digest
 assert e['normalized_result']['complete']
 r=e['normalized_result'];return decoded(int(r['address'],16),r['bytes_hex'],prefix)

# The retained32-byte memory window ends inside a later CALL. Only the first
# three complete CALLs form this liveness prefix; all32 bytes still match PE.
window=bytes_evidence(ctx['caller_bytes'],15)
assert [(i.address,i.mnemonic,i.op_str) for i in window[:3]]==[
 (0x4de7d0,'call','0x4bbc10'),(0x4de7d5,'call','0x4bb3f0'),(0x4de7da,'call','0x4d5980')]
for key,entry,site in [('refs3',0x4bb3f0,0x4de7d5),('refs10',0x4bbc10,0x4de7d0)]:
 e=ctx[key];assert e['authority']=='shipped-artifact' and e['subject']['digest']['sha256']==digest
 calls=[r for r in e['normalized_result']['references'] if r['kind']['call']]
 assert len(calls)==1 and int(calls[0]['source_address'],16)==site and int(calls[0]['target_address'],16)==entry

masks={'eax':0xffffffff,'ax':65535,'al':255,'ah':0xff00}
def incoming_eax_kill(code):
 alive=0xffffffff;partial=[]
 for i in code:
  reads,writes=i.regs_access();read_mask=write_mask=0
  for reg in reads:read_mask|=masks.get(i.reg_name(reg),0)
  for reg in writes:write_mask|=masks.get(i.reg_name(reg),0)
  assert not read_mask&alive,(hex(i.address),i.mnemonic,i.op_str,'incoming EAX consumed')
  if write_mask&alive:partial.append(hex(i.address))
  alive&=~write_mask
  if not alive:return dict(full_kill=hex(i.address),overwrite_sites=partial)
  assert i.mnemonic not in ['ret','call','jmp'] and not i.mnemonic.startswith('j'),hex(i.address)
 raise AssertionError('EAX not killed in the verified prefix')

next_group=next(f for f in catalogue['functions'] if f['address']=='0x4bb3f0')
point_result=incoming_eax_kill(decoded(0x4bb3f0,next_group['bytes_hex']))
following=bytes_evidence(json.loads((ROOT/'metadata/actor-scenario-spawn-caller-bytes.json').read_text()))
assert [i.mnemonic for i in following]==['push','push','push','call']
assert following[-1].op_str=='0x40f8f0'
assert all('eax' not in [i.reg_name(r) for r in i.regs_access()[0]] for i in following[:-1])
fill=next(f for f in retained['functions'] if f['address']=='0x40f8f0')
group_result=incoming_eax_kill(decoded(0x40f8f0,fill['bytes_hex']))
sources=['direct_c/audit_actor_scenario_spawn_callers.py','direct_c/native_test_pe.py','metadata/actor-scenario-spawn-context.json','metadata/actor-scenario-spawn-caller-bytes.json','metadata/actor-scenario-spawn-native.json','metadata/actor-reserved-native.json','include/actor_mission_init.h']
proof=dict(passed=True,native_sha256=digest,direct_callsites=2,point_return_kill=point_result,group_return_kill=group_result,void_interfaces_supported_for_observed_callers=True,source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},limitations=['Exactly one provider-observed direct caller per entry.4BBC10 is followed by4BB3F0, whose first MOV EAX kills its incoming value.4BB3F0 is followed by4D5980, whose constant pushes lead to40F8F0: AL is replaced before its use and full EAX is then overwritten.','This checks EAX/AX/AL/AH liveness through these exact prefixes. Indirect/unrecognized callers, arbitrary stack corruption, other incidental volatile registers/flags and full end-to-end mission loading are outside this audit.'])
(ROOT/'metadata/actor-scenario-spawn-caller-audit.json').write_text(json.dumps(proof,indent=2)+'\n')
print('PASS both scenario-spawn void interfaces: exact direct callers replace EAX before meaningful use')
