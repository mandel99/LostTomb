"""Exact native caller/return evidence for the nullary Hippo Stomp action."""
from pathlib import Path
import json,hashlib,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from native_test_pe import PE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
native=ROOT/'build/smoke-game/Pharaoh.exe';sha=hashlib.sha256(native.read_bytes()).hexdigest()
assert sha=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=PE(native);decoder=Cs(CS_ARCH_X86,CS_MODE_32);records=[]
paths=['metadata/actor-command-nullary-next-context-rea-records.json','metadata/actor-command-nullary-next-caller-refs-rea-record.json','metadata/actor-command-spawn-eight-caller-rea-records.json','metadata/actor-command-spawn-eight-outer-caller-rea-records.json','metadata/actor-command-spawn-eight-event-caller-rea-record.json','metadata/actor-command-spawn-eight-input-refs-rea-record.json','metadata/actor-command-spawn-eight-input-caller-rea-records.json','metadata/actor-command-spawn-eight-return-kill-rea-records.json','metadata/actor-command-spawn-eight-return-kill-tail-rea-record.json']
for path in paths:
 d=json.loads((ROOT/path).read_text());records+=d['records'] if 'records' in d else [d]
windows={}
for e in records:
 assert e['authority']=='shipped-artifact' and e['subject']['digest']['sha256']==sha
 if e['operation']!='read_bytes':continue
 r=e['normalized_result'];assert r['complete'];at=int(r['address'],16);raw=bytes.fromhex(r['bytes_hex'])
 assert raw==bytes(pe.data[at-pe.base:at-pe.base+len(raw)])
 windows[at]=(raw,list(decoder.disasm(raw,at)))
assert windows[0x57c3ec][0]==bytes.fromhex('30c7570000cd410001000000')
encoded=windows[0x57c730][0].split(b'\0')[0];assert bytes(c^3 for c in encoded)==b'Hippo Stomp'
def prefix(at,expected):
 actual=[(i.mnemonic,i.op_str) for i in windows[at][1]][:len(expected)]
 assert actual==expected,(hex(at),actual,expected)
prefix(0x41d010,[('mov','ecx, dword ptr [esp + 4]'),('test','ecx, ecx'),('jl','0x41d044'),('cmp','ecx, 0x22')])
call=next(i for i in windows[0x41d010][1] if i.address==0x41d03a)
assert (call.mnemonic,call.op_str)==('call','dword ptr [ecx*4 + 0x57c3cc]')
assert [(i.mnemonic,i.op_str) for i in windows[0x41d010][1] if 0x41d041<=i.address<=0x41d043]==[('mov','al, 1'),('ret','')]
assert [(i.mnemonic,i.op_str) for i in windows[0x41d3f0][1] if 0x41d405<=i.address<=0x41d40d]==[('call','0x41d010'),('add','esp, 8'),('ret','')]
prefix(0x41d390,[('jmp','0x41d3f0'),('ret','')])
prefix(0x4dd819,[('call','0x41d310'),('pop','edi'),('pop','esi'),('pop','ebp'),('pop','ebx'),('add','esp, 0x7c'),('ret','')])
prefix(0x4d6c67,[('call','0x4d8310'),('call','0x4cf1f0')])
prefix(0x538ca3,[('call','0x4d8310'),('push','0'),('call','0x4d0280'),('call','0x4cf1f0')])
prefix(0x4cf1f0,[('xor','eax, eax')])
prefix(0x4d0280,[('cmp','dword ptr [0x5e06b0], 1'),('je','0x4d0392'),('mov','eax, dword ptr [0xf54054]')])
prefix(0x4d0392,[('ret','')])
for entry,expected in [(0x41d010,{0x41d405}),(0x41d3f0,{0x41d390}),(0x4d8310,{0x4d6c67,0x538ca3})]:
 e=next(e for e in records if e['operation']=='procedure_references' and int(e['parameters']['procedure'],16)==entry)
 actual={int(r['source_address'],16) for r in e['normalized_result']['references'] if int(r['target_address'],16)==entry}
 assert actual==expected,(hex(entry),actual)
sources=[*paths,'direct_c/audit_actor_spawn_hippos.py','runtime/actor_command.c','runtime/actor_command_model.c','runtime/action_tail.c','runtime/menu_callbacks.c','include/actor_command.h']
proof=dict(passed=True,native_sha256=sha,command_table_row=3,decoded_command_name='Hippo Stomp',void_supported_for_observed_UI_callers=True,return_analysis='The table invoker replaces AL only. Upper EAX bits propagate through tail/return paths, then both observed input-loop callers reach 4CF1F0 which XORs EAX,EAX before use. The intervening 4D0280 either returns without consuming incoming EAX or replaces EAX first.',reference_correction='Ghidra labels the 41D390 edge as a call; authenticated E9 bytes show a tail JMP. The byte-level control flow is used.',source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},limitations=['Exact command row and provider-observed UI caller closure; unidentified indirect callers and arbitrary register/stack corruption remain unknown.','This supports a void C callback in the existing nullary UI path; it does not claim that native incidental upper EAX bits equal a C void return. No game is launched.'])
(ROOT/'metadata/actor-spawn-hippos-caller-audit.json').write_text(json.dumps(proof,indent=2)+'\n')
print('PASS Hippo Stomp nullary/void contract: exact command row and two native input-loop EAX kills')
