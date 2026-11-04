"""Check direct-call EAX liveness from exact REA caller windows, offline."""
from pathlib import Path
import hashlib,json,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from native_test_pe import PE
j=json.loads((ROOT/'metadata/actor-reserved-caller-native.json').read_text())
bundle=json.loads((ROOT/'metadata/actor-reserved-rea-evidence.json').read_text());evidence={e['evidence_id']:e for e in bundle['records']}
native=ROOT/'build/smoke-game/Pharaoh.exe';assert hashlib.sha256(native.read_bytes()).hexdigest()==j['native_sha256']
pe=PE(native);decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
assert j['following_4bbc10']['assembly'].splitlines()[:2]==['0x4bbc10: PUSH ECX','0x4bbc11: MOV EAX, [0x00605a3c]']
assert '0x4de7d0: CALL 0x004bbc10' in j['caller_4de600']['assembly']
for key in ['following_4bbc10','caller_4de600']:
 e=evidence[j[key]['evidence_id']];assert e['authority']=='shipped-artifact' and e['normalized_result']==j[key]['assembly']
register_masks={'eax':0xffffffff,'ax':0xffff,'al':255,'ah':0xff00}
results=[]
for caller in j['callers']:
 start=int(caller['address'],16);raw=bytes.fromhex(caller['bytes_hex']);e=evidence[caller['bytes_evidence_id']]
 assert e['authority']=='shipped-artifact' and e['normalized_result']['complete'] and e['normalized_result']['bytes_hex']==caller['bytes_hex']
 assert raw==bytes(pe.data[start-pe.base:start-pe.base+len(raw)])
 code={i.address:i for i in decoder.disasm(raw,start)}
 assert code[start].mnemonic=='call' and int(code[start].op_str,16)==0x4bb090
 work=[(start+5,0xffffffff)];seen=set();kills=set()
 while work:
  pc,alive=work.pop()
  if not alive:continue
  if pc==0x4de7d0:kills.add('4BBC10 entry overwrites full EAX before use');continue
  if (pc,alive) in seen:continue
  seen.add((pc,alive));assert pc in code,(hex(start),hex(pc),'caller window escaped before kill')
  instruction=code[pc];reads,writes=instruction.regs_access()
  read_mask=0;write_mask=0
  for reg in reads:read_mask|=register_masks.get(instruction.reg_name(reg),0)
  for reg in writes:write_mask|=register_masks.get(instruction.reg_name(reg),0)
  assert not read_mask&alive,(hex(start),hex(pc),instruction.mnemonic,instruction.op_str,'incoming EAX consumed')
  remaining=alive&~write_mask
  if remaining!=alive:kills.add(hex(pc))
  if not remaining:continue
  if instruction.mnemonic.startswith('j'):
   work.append((int(instruction.op_str,16),remaining))
   if instruction.mnemonic!='jmp':work.append((pc+instruction.size,remaining))
  else:
   assert instruction.mnemonic not in ['ret','call'],(hex(start),hex(pc),'unresolved escape before EAX kill')
   work.append((pc+instruction.size,remaining))
 results.append(dict(callsite=hex(start),checked_states=len(seen),kill_sites=sorted(kills),incoming_EAX_consumed=False))
sources=['direct_c/audit_actor_reserved_callers.py','direct_c/native_test_pe.py','metadata/actor-reserved-caller-native.json','metadata/actor-reserved-rea-evidence.json']
report=dict(passed=True,native_sha256=j['native_sha256'],direct_callsites=3,void_interface_supported_for_observed_callers=True,results=results,source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},limitations=['Exact three analyzed direct references and their EAX/AX/AL liveness are checked; the4DE7D0 cutpoint uses the verified4BBC10 entry kill. Indirect/unrecognized callers, arbitrary stack corruption, all volatile-register states and physical runtime execution are not certified.'])
(ROOT/'metadata/actor-reserved-caller-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS three direct4BB090 callers: incoming EAX/AX/AL replaced before use; existing void interface supported')
