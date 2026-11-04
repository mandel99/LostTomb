"""Audit native return consumers, bridge scratch liveness and dead branches.

This is static evidence for exact byte ranges, not a gameplay equivalence proof.
"""
from pathlib import Path
import hashlib,json,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from native_test_pe import PE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

native=ROOT/'build/smoke-game/Pharaoh.exe';digest=hashlib.sha256(native.read_bytes()).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=PE(native);decoder=Cs(CS_ARCH_X86,CS_MODE_32)
catalogue=json.loads((ROOT/'metadata/actor-command-transition-native.json').read_text())
records=[]
for file in ['actor-command-transition-next-rea-evidence.json','actor-command-transition-additions-rea-records.json','actor-command-transition-callers-rea-records.json']:
 records+=json.loads((ROOT/'metadata'/file).read_text())['records']
records={e['evidence_id']:e for e in records};instructions={}

def original_bytes(at,raw):
 assert raw==bytes(pe.data[at-pe.base:at-pe.base+len(raw)])

for f in catalogue['functions']:
 e=records[f['bytes_evidence_id']];assert e['authority']=='shipped-artifact' and e['subject']['digest']['sha256']==digest
 assert e['normalized_result']['complete'] and e['normalized_result']['bytes_hex']==f['bytes_hex']
 at=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);original_bytes(at,raw)
 for i in decoder.disasm(raw,at):instructions[i.address]=(i.mnemonic,i.op_str)

def exact(at,expected):
 assert instructions[at]==expected,(hex(at),instructions[at],expected)

exact(0x4ec5c1,('and','edx, 0x7fff'));exact(0x4ec5cc,('mov','eax, edx'))
exact(0x4ec5d4,('and','eax, 0x80000007'));exact(0x4ec5eb,('jns','0x4ec5f2'))
assert all(instructions[at][0]=='mov' for at in [0x4ec5d9,0x4ec5df,0x4ec5e5])
exact(0x4ec106,('mov','ebp, 1'));exact(0x4ec10b,('mov','dword ptr [esp + 0x18], 0'))
exact(0x4ec31d,('inc','eax'));exact(0x4ec31e,('add','ebp, 2'))
exact(0x4ec336,('cmp','eax, 0xe4'));exact(0x4ec345,('jl','0x4ec12d'))
for at,dest in [(0x4ec137,0x4ec193),(0x4ec1a9,0x4ec20a),(0x4ec225,0x4ec281),(0x4ec297,0x4ec2f4)]:exact(at,('jle',hex(dest)))
exact(0x4ec907,('and','eax, 0x7fff'));exact(0x4ec924,('mov','ecx, 0x64'));exact(0x4ec92c,('idiv','ecx'))
exact(0x4ec934,('xor','ecx, ecx'));exact(0x4ec93a,('add','edi, dword ptr [eax]'))
exact(0x4ec93c,('cmp','edx, edi'));exact(0x4ec93e,('jl','0x4ec949'))
exact(0x4ec944,('cmp','ecx, 4'));exact(0x4ec947,('jl','0x4ec93a'))
exact(0x4ec7db,('mov','ecx, 1'));exact(0x4ec7f7,('mov','eax, 0x32'))
for at in [0x4ec7fc,0x4ec810,0x4ec830,0x4ec84c]:exact(at,('xor','edi, edi'))
for at,operand in {
 0x4ec7fe:'dword ptr [esp + 0x10], eax',0x4ec802:'dword ptr [esp + 0x14], eax',
 0x4ec806:'dword ptr [esp + 0x18], edi',0x4ec80a:'dword ptr [esp + 0x1c], edi',
 0x4ec812:'dword ptr [esp + 0x10], 0x4b',0x4ec81a:'dword ptr [esp + 0x14], 5',
 0x4ec822:'dword ptr [esp + 0x18], 0x14',0x4ec82a:'dword ptr [esp + 0x1c], edi',
 0x4ec832:'dword ptr [esp + 0x10], 0x5a',0x4ec83a:'dword ptr [esp + 0x14], ecx',
 0x4ec83e:'dword ptr [esp + 0x18], 9',0x4ec846:'dword ptr [esp + 0x1c], edi',
 0x4ec84e:'dword ptr [esp + 0x10], 0x1d',0x4ec856:'dword ptr [esp + 0x14], 0x46',
 0x4ec85e:'dword ptr [esp + 0x18], edi',0x4ec862:'dword ptr [esp + 0x1c], ecx',
}.items():exact(at,('mov',operand))
weights=[[50,50,0,0],[75,5,20,0],[90,1,9,0],[29,70,0,1]]
for values in weights:
 assert sum(values)==100
 for roll in range(100):
  cumulative=0
  for choice,value in enumerate(values):
   cumulative+=value
   if roll<cumulative:break
  else:raise AssertionError((values,roll))

windows={}
for e in records.values():
 if e['operation']!='read_bytes':continue
 r=e['normalized_result'];at=int(r['address'],16);raw=bytes.fromhex(r['bytes_hex']);original_bytes(at,raw)
 windows[at]={i.address:(i.mnemonic,i.op_str) for i in decoder.disasm(raw,at)}
def prefix(at,expected):
 code=list(windows[at].values());assert code[:len(expected)]==expected,(hex(at),code[:len(expected)])
for site,entry,jump in [(0x49eadd,0x4ec7c0,0x49eaeb),(0x49f03c,0x4ec7c0,0x49f04a),(0x49f1dd,0x4ec990,0x49f1eb)]:
 prefix(site,[('call',hex(entry)),('mov','ecx, dword ptr [0xd056f8]'),('add','esp, 4'),('jmp','0x49f418')])
prefix(0x49f108,[('call','0x4ec990'),('mov','ecx, dword ptr [0xd056f8]'),('add','esp, 4'),('mov','al, byte ptr [esp + 0x12]'),('test','al, al')])
prefix(0x41cfa3,[('call','0x4ec990'),('add','esp, 0x18'),('inc','ebx'),('cmp','ebx, 8'),('jl','0x41cd5a')])
join=windows[0x49f418]
assert join[0x49f418]==('mov','al, byte ptr [0x7027e1]') and join[0x49f41d]==('test','al, al')
assert join[0x49f41f]==('lea','eax, [ecx + ecx*2]')
assert join[0x49f440]==('mov','edx, dword ptr [0xf56ea4]')
assert join[0x49f47e]==('mov','edx, dword ptr [0xf56ea4]')
assert join[0x49f499]==('mov','edx, dword ptr [0xf56ea4]')
assert join[0x49f4b5]==('mov','edx, dword ptr [0xbca2e0]')
exact(0x4ec4cf,('call','0x4ec990'))
# No EAX write between this call and RET: the default apply branch returns it.
default=[(at,op) for at,op in instructions.items() if 0x4ec4d4<=at<=0x4ec4f8]
assert all(op[0] in ['add','mov','pop','ret'] and not op[1].startswith('eax') for at,op in default)

dead=[0x4ec5ed,0x4ec5ee,0x4ec5f1]
dead_edges=[(0x4ec5eb,0x4ec5ed),(0x4ec137,0x4ec193),(0x4ec1a9,0x4ec20a),(0x4ec225,0x4ec281),(0x4ec297,0x4ec2f4),(0x4ec947,0x4ec949)]
sources=['direct_c/audit_actor_command_transition.py','metadata/actor-command-transition-native.json','metadata/actor-command-transition-next-rea-evidence.json','metadata/actor-command-transition-additions-rea-records.json','metadata/actor-command-transition-callers-rea-records.json','runtime/actor_command.c','runtime/actor_command_model.c','runtime/actor_civic.c','runtime/actor_civic_model.c','runtime/actor_routing_model.c','include/actor_command.h']
proof=dict(passed=True,native_sha256=digest,analytically_unreachable_instructions=[hex(a) for a in dead],analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in dead_edges],proofs=[
 'EC5C1 masks EDX to 15 bits; EC5CC copies it to EAX. EC5D4 ANDs with 0x80000007, so SF=0. Only MOVs intervene before JNS; its three-instruction negative correction cannot execute.',
 'Spiral EBP starts at 1 and increases by 2 only after a round. The counter starts at 0 and permits reentry only below 228, so every reached direction has positive span 1..455. Its four JLE skip edges cannot execute.',
 'Chooser remainder is (first & 32767) % 100, hence 0..99. Every native weight row sums to 100; some cumulative comparison must succeed by row 3. The ECX==4 loop exit is unreachable without private-stack/code corruption.'
 ],caller_bridge=dict(retained='Full EAX, RET0 ESP movement and nonvolatile registers.',scratch='ECX and flags are overwritten immediately at the animal callsites. EDX is replaced along the join branches; the 49F108 path calls the typed two-DWORD movement helper before its next EDX use. 41CD5A reloads EDX and ECX before RNG use. The EAEC0 suffix tests EAX then assigns ECX and DX before using them.',return_type='EC4A0 default returns the full EAX from EC990. Integer C interfaces retain partial-AL/AX and child return bit patterns.'),source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},limitations=[
 'Read-only Ghidra did not identify EAE20, 49E8B0 and 41CD00 procedures. Their existing raw byte/source models provide additional explicitly identified callsites; provider incoming references alone are incomplete there.',
 'This exact native/source audit does not certify arbitrary caller-stack aliases, self-modifying code, corrupted private locals, unknown indirect callers, unused volatile outputs or OS exception dispatch.'
 ])
(ROOT/'metadata/actor-command-transition-audit.json').write_text(json.dumps(proof,indent=2)+'\n')
print('PASS actor command return/bridge audit; 3 dead instructions and 6 dead conditional outcomes')
